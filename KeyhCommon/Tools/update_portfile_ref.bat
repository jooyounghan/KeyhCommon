@echo off
setlocal EnableExtensions

set "SCRIPT_DIR=%~dp0"
set "POWERSHELL_SCRIPT=%SCRIPT_DIR%update_portfile_ref.ps1"

if not exist "%POWERSHELL_SCRIPT%" (
    echo [vcpkg registry] Error: PowerShell script not found: "%POWERSHELL_SCRIPT%"
    exit /b 1
)

powershell -NoProfile -ExecutionPolicy Bypass -File "%POWERSHELL_SCRIPT%"

if errorlevel 1 (
    exit /b %ERRORLEVEL%
)

exit /b 0
