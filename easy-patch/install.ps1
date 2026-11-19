# StarDesk Dark - installer
# Makes your normal StarDesk shortcuts open StarDesk in the dark theme, and starts it dark at login.
# Nothing inside StarDesk is changed on disk: the colours are swapped in memory each time it starts.
$ErrorActionPreference = 'Stop'
$here = Split-Path -Parent $MyInvocation.MyCommand.Path

function Find-StarDesk {
    $cands = @()
    $keys = 'HKLM:\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\*',
            'HKLM:\SOFTWARE\WOW6432Node\Microsoft\Windows\CurrentVersion\Uninstall\*',
            'HKCU:\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\*'
    foreach ($k in $keys) {
        $items = Get-ItemProperty $k -ErrorAction SilentlyContinue | Where-Object { $_.DisplayName -like 'StarDesk*' }
        foreach ($i in $items) {
            if ($i.InstallLocation) { $cands += $i.InstallLocation.Trim('"') }
            $p = (($i.DisplayIcon -replace '"', '') -replace ',\d+$', '');       if ($p) { $cands += (Split-Path $p) }
            $p = (($i.UninstallString -replace '"', '') -replace '\s+/.*$', ''); if ($p) { $cands += (Split-Path $p) }
        }
    }
    $cands += (Join-Path $env:ProgramFiles 'StarDesk')
    foreach ($c in ($cands | Where-Object { $_ })) {
        foreach ($d in @($c, (Split-Path $c))) {
            if ($d -and (Test-Path (Join-Path $d 'bin\StarDesk.exe'))) { return (Resolve-Path $d).Path }
        }
    }
    return $null
}

function Get-ShortcutFolders {
    $f = @(
        [Environment]::GetFolderPath('Desktop'),
        [Environment]::GetFolderPath('CommonDesktopDirectory'),
        [Environment]::GetFolderPath('Programs'),
        [Environment]::GetFolderPath('CommonPrograms'),
        (Join-Path $env:APPDATA 'Microsoft\Internet Explorer\Quick Launch\User Pinned\TaskBar'),
        (Join-Path $env:APPDATA 'Microsoft\Internet Explorer\Quick Launch\User Pinned\StartMenu')
    )
    return $f | Where-Object { $_ -and (Test-Path $_) }
}

Write-Host ''
Write-Host '  StarDesk Dark installer' -ForegroundColor Cyan
Write-Host '  -----------------------'
$sd = Find-StarDesk
if (-not $sd) { Write-Host '  StarDesk was not found. Install StarDesk first, then run this again.' -ForegroundColor Red; exit 1 }
Write-Host "  StarDesk found in: $sd"

# 1. copy the launcher next to StarDesk
$dark = Join-Path $sd 'StarDeskDark.exe'
try { Copy-Item (Join-Path $here 'StarDeskDark.exe') $dark -Force }
catch { Write-Host '  Could not copy StarDeskDark.exe. If StarDesk Dark is running, close it and try again.' -ForegroundColor Red; exit 1 }
Write-Host '  Installed StarDeskDark.exe'

# 2. point the normal StarDesk shortcuts at the dark launcher (originals are backed up)
$shell = New-Object -ComObject WScript.Shell
$backupFile = Join-Path $sd 'StarDeskDark-shortcut-backup.json'
$backup = @()
if (Test-Path $backupFile) { $backup = @(Get-Content $backupFile -Raw | ConvertFrom-Json) }
$targets = @((Join-Path $sd 'StarDesk.exe').ToLower(), (Join-Path $sd 'bin\StarDesk.exe').ToLower())
$changed = 0
foreach ($folder in Get-ShortcutFolders) {
    foreach ($f in Get-ChildItem $folder -Filter *.lnk -Recurse -ErrorAction SilentlyContinue) {
        $l = $shell.CreateShortcut($f.FullName)
        if ($targets -contains ([string]$l.TargetPath).ToLower()) {
            if (-not ($backup | Where-Object { $_.Path -eq $f.FullName })) {
                $backup += [pscustomobject]@{ Path = $f.FullName; TargetPath = $l.TargetPath; Arguments = $l.Arguments;
                                              WorkingDirectory = $l.WorkingDirectory; IconLocation = $l.IconLocation }
            }
            $l.TargetPath = $dark
            $l.WorkingDirectory = $sd
            $l.IconLocation = "$dark,0"
            $l.Save()
            $changed++
            Write-Host "  Shortcut now opens dark: $($f.FullName)"
        }
    }
}
if ($backup.Count -gt 0) { ConvertTo-Json @($backup) -Depth 3 | Set-Content $backupFile -Encoding UTF8 }
if ($changed -eq 0) { Write-Host '  (No existing StarDesk shortcuts needed changing)' }

# 3. a "StarDesk Dark" and a "StarDesk Dark Settings" entry in the Start menu
$programs = [Environment]::GetFolderPath('Programs')
foreach ($s in @(@('StarDesk Dark.lnk', '', 'Start StarDesk in dark mode'),
                 @('StarDesk Dark Settings.lnk', '--settings', 'Choose the StarDesk Dark theme'))) {
    $l = $shell.CreateShortcut((Join-Path $programs $s[0]))
    $l.TargetPath = $dark; $l.Arguments = $s[1]; $l.WorkingDirectory = $sd
    $l.IconLocation = "$dark,0"; $l.Description = $s[2]; $l.Save()
}
Write-Host '  Added Start menu entries: StarDesk Dark, StarDesk Dark Settings'

# 4. start dark at login (StarDesk's own login start opens it light; this restarts it dark a few seconds later)
Set-ItemProperty 'HKCU:\Software\Microsoft\Windows\CurrentVersion\Run' -Name 'StarDesk Dark' -Value ('"' + $dark + '" --autostart')
Write-Host '  StarDesk will open in dark mode when you sign in'

Write-Host ''
Write-Host '  Done.' -ForegroundColor Green
Write-Host '  Choose Dark or AMOLED any time from Start > StarDesk Dark Settings.'
Write-Host '  After a StarDesk update, run this installer again if your shortcuts open light again.'
Write-Host ''
$a = Read-Host '  Open StarDesk in dark mode now? (Y/N)'
if ($a -match '^[Yy]') { Start-Process explorer.exe -ArgumentList ('"' + $dark + '"') }   # via Explorer so it runs as you, not as administrator
