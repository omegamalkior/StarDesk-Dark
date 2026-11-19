# StarDesk Dark - uninstaller: puts your StarDesk shortcuts back and removes the dark launcher.
$ErrorActionPreference = 'Continue'
$cands = @((Join-Path $env:ProgramFiles 'StarDesk'))
$keys = 'HKLM:\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\*',
        'HKLM:\SOFTWARE\WOW6432Node\Microsoft\Windows\CurrentVersion\Uninstall\*',
        'HKCU:\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\*'
foreach ($k in $keys) {
    Get-ItemProperty $k -ErrorAction SilentlyContinue | Where-Object { $_.DisplayName -like 'StarDesk*' -and $_.InstallLocation } |
        ForEach-Object { $cands += $_.InstallLocation.Trim('"') }
}
$sd = $cands | Where-Object { Test-Path (Join-Path $_ 'StarDeskDark.exe') } | Select-Object -First 1
Write-Host ''
Write-Host '  StarDesk Dark uninstaller' -ForegroundColor Cyan
Write-Host '  -------------------------'
$shell = New-Object -ComObject WScript.Shell
if ($sd) {
    $backupFile = Join-Path $sd 'StarDeskDark-shortcut-backup.json'
    if (Test-Path $backupFile) {
        foreach ($b in @(Get-Content $backupFile -Raw | ConvertFrom-Json)) {
            if (Test-Path $b.Path) {
                $l = $shell.CreateShortcut($b.Path)
                $l.TargetPath = $b.TargetPath; $l.Arguments = $b.Arguments
                $l.WorkingDirectory = $b.WorkingDirectory; $l.IconLocation = $b.IconLocation; $l.Save()
                Write-Host "  Restored: $($b.Path)"
            }
        }
        Remove-Item $backupFile -Force
    }
}
$programs = [Environment]::GetFolderPath('Programs')
foreach ($n in 'StarDesk Dark.lnk', 'StarDesk Dark Settings.lnk') {
    $p = Join-Path $programs $n
    if (Test-Path $p) { Remove-Item $p -Force; Write-Host "  Removed Start menu entry: $n" }
}
Remove-ItemProperty 'HKCU:\Software\Microsoft\Windows\CurrentVersion\Run' -Name 'StarDesk Dark' -ErrorAction SilentlyContinue
# stop and remove the background update helper (restarts StarDesk after StarDesk Dark is updated)
Get-Process -ErrorAction SilentlyContinue | Where-Object { $_.ProcessName -like 'watcher-*' -and $_.Path -like "$env:LOCALAPPDATA\StarDesk Dark\*" } | Stop-Process -Force -ErrorAction SilentlyContinue
# StarDesk opened dark uses the Qt component copy in that folder: close it so the folder can be removed
$copy = Join-Path $env:LOCALAPPDATA 'StarDesk Dark\platforms'
Get-Process StarDesk -ErrorAction SilentlyContinue | Where-Object {
    try { $_.Modules | Where-Object { $_.FileName -like "$copy\*" } } catch { $false } } |
    ForEach-Object { Write-Host '  Closing StarDesk (it was open in dark mode)'; Stop-Process $_ -Force -ErrorAction SilentlyContinue }
Start-Sleep -Milliseconds 800
Remove-Item (Join-Path $env:LOCALAPPDATA 'StarDesk Dark') -Recurse -Force -ErrorAction SilentlyContinue
Remove-Item 'HKCU:\Software\StarDesk Dark' -Recurse -Force -ErrorAction SilentlyContinue
if ($sd) {
    foreach ($n in 'StarDeskDark.exe', 'StarDesk Dark.exe') {
        $p = Join-Path $sd $n
        if (Test-Path $p) {
            try { Remove-Item $p -Force -ErrorAction Stop; Write-Host "  Removed $n" }
            catch { Write-Host "  Could not remove $n (is it running?). Delete it later from $sd" -ForegroundColor Yellow }
        }
    }
}
Write-Host ''
Write-Host '  Done. StarDesk is back to normal (light). Restart StarDesk to see it.' -ForegroundColor Green
Write-Host ''
