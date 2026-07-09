@echo off
setlocal EnableDelayedExpansion

:: ============================================================
::  KeyhEngine Reflect Code Generator – batch wrapper
::
::  Usage:
::    run_reflect_codegen.bat <project_dir> [output_dir]
::
::  <project_dir>   Path to the Visual Studio project folder to scan.
::  [output_dir]    Destination for the generated .inl file.
::                  Defaults to <project_dir> when omitted.
::
::  Pre-build event example (.vcxproj):
::    <PreBuildEvent>
::      <Command>call "$(SolutionDir)..^Tools\run_reflect_codegen.bat"
::               "$(ProjectDir)."</Command>
::    </PreBuildEvent>
:: ============================================================

set "SCRIPT_DIR=%~dp0"
set "PROJECT_DIR=%~1"
set "OUTPUT_DIR=%~2"

if "%PROJECT_DIR%"=="" (
    echo [Reflect] Error: project directory argument is required.
    echo [Reflect] Usage: run_reflect_codegen.bat ^<project_dir^> [output_dir]
    exit /b 1
)

if "%OUTPUT_DIR%"=="" (
    set "OUTPUT_DIR=%PROJECT_DIR%"
)

:: -----------------------------------------------------------
:: Locate a Python 3 interpreter
:: -----------------------------------------------------------
set "PYTHON_CMD="

where python >nul 2>&1
if !ERRORLEVEL! equ 0 (
    :: Verify it is Python 3 (not Python 2)
    python -c "import sys; sys.exit(0 if sys.version_info.major>=3 else 1)" >nul 2>&1
    if !ERRORLEVEL! equ 0 set "PYTHON_CMD=python"
)

if "!PYTHON_CMD!"=="" (
    where python3 >nul 2>&1
    if !ERRORLEVEL! equ 0 set "PYTHON_CMD=python3"
)

if "!PYTHON_CMD!"=="" (
    echo [Reflect] Error: Python 3 not found. Please install Python 3 and ensure it is on PATH.
    exit /b 1
)

:: -----------------------------------------------------------
:: Run the code generator
:: -----------------------------------------------------------
!PYTHON_CMD! "%SCRIPT_DIR%reflect_codegen.py" ^
    --project-dir "%PROJECT_DIR%" ^
    --output-dir  "%OUTPUT_DIR%"

if !ERRORLEVEL! neq 0 (
    echo [Reflect] Code generation failed with error code !ERRORLEVEL!
    exit /b !ERRORLEVEL!
)

exit /b 0
