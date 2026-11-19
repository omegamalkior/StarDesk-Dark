@echo off
rem Install StarDesk Dark - asks for administrator rights (needed to write into the StarDesk folder)
fltmc >nul 2>&1 || (
  powershell -NoProfile -Command "Start-Process -FilePath \"%~f0\" -Verb RunAs"
  exit /b
)
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0install.ps1"
pause
