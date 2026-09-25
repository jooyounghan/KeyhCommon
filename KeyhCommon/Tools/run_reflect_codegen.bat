@echo off
setlocal EnableExtensions DisableDelayedExpansion

:: ============================================================
::  KeyhCommon Reflect Code Generator - batch wrapper
::
::  Usage:
::    run_reflect_codegen.bat <project_dir> [output_dir]
::
::  <project_dir>   Path to the Visual Studio project folder to scan.
::  [output_dir]    Destination for the generated .inl file.
::                  Defaults to the directory of each source header.
::  KEYHCOMMON_PYTHON may specify an absolute Python 3 executable path.
::
::  Pre-build event example (.vcxproj):
::    <PreBuildEvent>
::      <Command>call "$(KeyhCommonInstalledTripletDir)\tools\keyhcommon\run_reflect_codegen.bat" "$(ProjectDir)."</Command>
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

if not exist "%SCRIPT_DIR%reflect_codegen.py" (
    echo [Reflect] Error: reflect_codegen.py must be installed beside this batch file.
    exit /b 1
)

:: -----------------------------------------------------------
:: Locate a Python 3 interpreter
:: -----------------------------------------------------------
if defined KEYHCOMMON_PYTHON goto explicit_python

set "PYTHON_CMD=py -3"
%PYTHON_CMD% -c "import sys; sys.exit(sys.version_info.major != 3)" >nul 2>&1
if not errorlevel 1 goto run_codegen

set "PYTHON_CMD=python"
%PYTHON_CMD% -c "import sys; sys.exit(sys.version_info.major != 3)" >nul 2>&1
if not errorlevel 1 goto run_codegen

set "PYTHON_CMD=python3"
%PYTHON_CMD% -c "import sys; sys.exit(sys.version_info.major != 3)" >nul 2>&1
if not errorlevel 1 goto run_codegen

echo [Reflect] Error: Python 3 not found. Install Python 3 or set KEYHCOMMON_PYTHON.
exit /b 1

:explicit_python
set PYTHON_CMD="%KEYHCOMMON_PYTHON%"
%PYTHON_CMD% -c "import sys; sys.exit(sys.version_info.major != 3)" >nul 2>&1
if not errorlevel 1 goto run_codegen
echo [Reflect] Error: KEYHCOMMON_PYTHON must name a working Python 3 executable.
exit /b 1

:: -----------------------------------------------------------
:: Run the code generator
:: -----------------------------------------------------------
:run_codegen
:: Append a dot so a trailing backslash cannot escape Python's closing quote.
if "%OUTPUT_DIR%"=="" goto default_output
%PYTHON_CMD% "%SCRIPT_DIR%reflect_codegen.py" --project-dir "%PROJECT_DIR%\." --output-dir "%OUTPUT_DIR%\."
goto codegen_result

:default_output
%PYTHON_CMD% "%SCRIPT_DIR%reflect_codegen.py" --project-dir "%PROJECT_DIR%\."

:codegen_result
set "CODEGEN_EXIT_CODE=%ERRORLEVEL%"
if not "%CODEGEN_EXIT_CODE%"=="0" echo [Reflect] Code generation failed with error code %CODEGEN_EXIT_CODE%
exit /b %CODEGEN_EXIT_CODE%
