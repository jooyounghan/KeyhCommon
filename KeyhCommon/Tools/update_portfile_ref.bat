@echo off
setlocal EnableExtensions

set "SCRIPT_DIR=%~dp0"
set "PYTHON_SCRIPT=%SCRIPT_DIR%update_portfile_ref.py"

if not exist "%PYTHON_SCRIPT%" (
    echo [vcpkg registry] Error: Python script not found: "%PYTHON_SCRIPT%"
    exit /b 1
)

where py >nul 2>nul
if not errorlevel 1 (
    py -3 "%PYTHON_SCRIPT%"
) else (
    where python >nul 2>nul
    if errorlevel 1 (
        echo [vcpkg registry] Error: Neither py nor python was found.
        exit /b 1
    )
    python "%PYTHON_SCRIPT%"
)

if errorlevel 1 (
    exit /b %ERRORLEVEL%
)

exit /b 0
