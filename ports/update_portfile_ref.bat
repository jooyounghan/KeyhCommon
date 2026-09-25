@echo off
setlocal EnableExtensions

set "SCRIPT_DIR=%~dp0"
for %%I in ("%SCRIPT_DIR%..") do set "REPO_ROOT=%%~fI"
set "PORTFILE_PATH=%SCRIPT_DIR%keyhcommon\portfile.cmake"
set "PS_PORTFILE_PATH=%PORTFILE_PATH:'=''%"

if not exist "%PORTFILE_PATH%" (
    echo [vcpkg REF] Error: portfile not found: "%PORTFILE_PATH%"
    exit /b 1
)

for /f %%I in ('git -C "%REPO_ROOT%" rev-parse --verify HEAD 2^>nul') do set "HEAD_SHA=%%I"
if not defined HEAD_SHA (
    echo [vcpkg REF] Error: failed to resolve repository HEAD commit.
    exit /b 1
)
powershell -NoProfile -ExecutionPolicy Bypass -Command "if ('%HEAD_SHA%' -notmatch '^[0-9A-Fa-f]{40}$') { exit 1 }"
if errorlevel 1 (
    echo [vcpkg REF] Error: invalid HEAD commit hash.
    exit /b 1
)

powershell -NoProfile -ExecutionPolicy Bypass -Command ^
  "$path = '%PS_PORTFILE_PATH%';" ^
  "$sha = '%HEAD_SHA%';" ^
  "$bytes = [System.IO.File]::ReadAllBytes($path);" ^
  "$isUtf8Bom = ($bytes.Length -ge 3 -and $bytes[0] -eq 0xEF -and $bytes[1] -eq 0xBB -and $bytes[2] -eq 0xBF);" ^
  "$isUtf16Le = ($bytes.Length -ge 2 -and $bytes[0] -eq 0xFF -and $bytes[1] -eq 0xFE);" ^
  "$isUtf16Be = ($bytes.Length -ge 2 -and $bytes[0] -eq 0xFE -and $bytes[1] -eq 0xFF);" ^
  "$isUtf8NoBom = $false;" ^
  "if (-not $isUtf8Bom -and -not $isUtf16Le -and -not $isUtf16Be) { try { $utf8 = New-Object System.Text.UTF8Encoding($false, $true); $utf8RoundTrip = $utf8.GetBytes($utf8.GetString($bytes)); if ($utf8RoundTrip.Length -eq $bytes.Length) { $same = $true; for ($i = 0; $i -lt $bytes.Length; $i++) { if ($bytes[$i] -ne $utf8RoundTrip[$i]) { $same = $false; break } }; if ($same) { $isUtf8NoBom = $true } } } catch { $isUtf8NoBom = $false } }" ^
  "$useDirectTextEncoding = $isUtf8Bom -or $isUtf8NoBom -or $isUtf16Le -or $isUtf16Be;" ^
  "$encoding = [System.Text.Encoding]::GetEncoding(28591);" ^
  "if ($isUtf8Bom) { $encoding = New-Object System.Text.UTF8Encoding($true) } elseif ($isUtf8NoBom) { $encoding = New-Object System.Text.UTF8Encoding($false) } elseif ($isUtf16Le) { $encoding = [System.Text.Encoding]::Unicode } elseif ($isUtf16Be) { $encoding = [System.Text.Encoding]::BigEndianUnicode }" ^
  "$content = if ($useDirectTextEncoding) { [System.IO.File]::ReadAllText($path, $encoding) } else { $encoding.GetString($bytes) };" ^
  "$regex = '(?m)^(\s*REF\s+"")([0-9A-Fa-f]{40})(""\s*)$';" ^
  "$match = [regex]::Match($content, $regex);" ^
  "if (-not $match.Success) { Write-Error '[vcpkg REF] Error: REF line not found in portfile.'; exit 2 };" ^
  "$current = $match.Groups[2].Value;" ^
  "if ($current -ieq $sha) { Write-Host ('[vcpkg REF] Already up to date: ' + $sha); exit 0 };" ^
  "$updated = [regex]::Replace($content, $regex, ('$1' + $sha + '$3'), 1);" ^
  "if ($useDirectTextEncoding) { [System.IO.File]::WriteAllText($path, $updated, $encoding) } else { [System.IO.File]::WriteAllBytes($path, $encoding.GetBytes($updated)) };" ^
  "Write-Host ('[vcpkg REF] Updated REF: ' + $current + ' -> ' + $sha);"

if errorlevel 1 (
    exit /b %ERRORLEVEL%
)

exit /b 0
