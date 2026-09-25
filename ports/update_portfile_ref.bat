@echo off
setlocal EnableExtensions

set "SCRIPT_DIR=%~dp0"
for %%I in ("%SCRIPT_DIR%..") do set "REPO_ROOT=%%~fI"
set "PORTFILE_PATH=%SCRIPT_DIR%keyhcommon\portfile.cmake"

if not exist "%PORTFILE_PATH%" (
    echo [vcpkg REF] Error: portfile not found: "%PORTFILE_PATH%"
    exit /b 1
)

for /f %%I in ('git -C "%REPO_ROOT%" rev-parse HEAD 2^>nul') do set "HEAD_SHA=%%I"
if not defined HEAD_SHA (
    echo [vcpkg REF] Error: failed to resolve repository HEAD commit.
    exit /b 1
)

powershell -NoProfile -ExecutionPolicy Bypass -Command ^
  "$path = '%PORTFILE_PATH%';" ^
  "$sha = '%HEAD_SHA%';" ^
  "$content = Get-Content -Path $path -Raw;" ^
  "$regex = '(?m)^(\s*REF\s+"")([0-9a-f]{40})(""\s*)$';" ^
  "$match = [regex]::Match($content, $regex);" ^
  "if (-not $match.Success) { Write-Error '[vcpkg REF] Error: REF line not found in portfile.'; exit 2 };" ^
  "$current = $match.Groups[2].Value;" ^
  "if ($current -ieq $sha) { Write-Host ('[vcpkg REF] Already up to date: ' + $sha); exit 0 };" ^
  "$updated = [regex]::Replace($content, $regex, ('$1' + $sha + '$3'), 1);" ^
  "Set-Content -Path $path -Value $updated -NoNewline;" ^
  "Write-Host ('[vcpkg REF] Updated REF: ' + $current + ' -> ' + $sha);"

if errorlevel 1 (
    exit /b 1
)

exit /b 0
