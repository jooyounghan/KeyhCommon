$ErrorActionPreference = 'Stop'

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$repoRoot = [System.IO.Path]::GetFullPath((Join-Path $scriptDir '..\..'))
$portDir = Join-Path $repoRoot 'ports\keyhcommon'
$portfilePath = Join-Path $portDir 'portfile.cmake'
$vcpkgJsonPath = Join-Path $portDir 'vcpkg.json'
$versionsPath = Join-Path $repoRoot 'versions\k-\keyhcommon.json'
$baselinePath = Join-Path $repoRoot 'versions\baseline.json'

function Read-Json([string]$path) {
    return Get-Content -LiteralPath $path -Raw | ConvertFrom-Json
}

function Get-VersionFieldInfo($value) {
    foreach ($fieldName in @('version', 'version-semver', 'version-date', 'version-string')) {
        $property = $value.PSObject.Properties[$fieldName]
        if ($null -ne $property) {
            return [pscustomobject]@{
                Name = $fieldName
                Value = [string]$property.Value
            }
        }
    }

    return $null
}

function Write-Utf8NoBom([string]$path, [string]$content) {
    $utf8 = New-Object System.Text.UTF8Encoding($false)
    [System.IO.File]::WriteAllText($path, $content, $utf8)
}

function Write-Json([string]$path, $value) {
    $json = $value | ConvertTo-Json -Depth 100
    Write-Utf8NoBom $path ($json + [Environment]::NewLine)
}

function Get-RepoRelativePath([string]$repoRootPath, [string]$filePath) {
    $normalizedRootPath = [System.IO.Path]::GetFullPath($repoRootPath).TrimEnd('\', '/')
    $normalizedFilePath = [System.IO.Path]::GetFullPath($filePath)
    $comparison = [System.StringComparison]::OrdinalIgnoreCase
    $rootPrefix = $normalizedRootPath + [System.IO.Path]::DirectorySeparatorChar

    if ($normalizedFilePath.Equals($normalizedRootPath, $comparison)) {
        return ''
    }

    if (-not $normalizedFilePath.StartsWith($rootPrefix, $comparison)) {
        throw ('Path is not inside root: ' + $filePath)
    }

    return $normalizedFilePath.Substring($rootPrefix.Length).Replace('\', '/')
}

function Get-PortTreeHash([string]$repoRootPath, [string]$portDirectory) {
    $tempIndex = [System.IO.Path]::GetTempFileName()
    Remove-Item -LiteralPath $tempIndex -Force

    try {
        $env:GIT_INDEX_FILE = $tempIndex
        & git -C $repoRootPath read-tree --empty | Out-Null
        if ($LASTEXITCODE -ne 0) {
            throw 'Unable to initialize temporary git index.'
        }

        $files = @(Get-ChildItem -LiteralPath $portDirectory -Recurse -File | Sort-Object FullName)
        if ($files.Count -eq 0) {
            throw 'No files were found under ports/keyhcommon.'
        }

        foreach ($file in $files) {
            $blobHash = (& git -C $repoRootPath hash-object -w -- $file.FullName).Trim()
            if ($LASTEXITCODE -ne 0 -or [string]::IsNullOrWhiteSpace($blobHash)) {
                throw ('Unable to hash file: ' + $file.FullName)
            }

            $relativePath = (Get-RepoRelativePath $portDirectory $file.FullName).Replace('\', '/')
            & git -C $repoRootPath update-index --add --cacheinfo ('100644,' + $blobHash + ',' + $relativePath) | Out-Null
            if ($LASTEXITCODE -ne 0) {
                throw ('Unable to add file to temporary git index: ' + $relativePath)
            }
        }

        $treeHash = (& git -C $repoRootPath write-tree).Trim()
        if ($LASTEXITCODE -ne 0 -or [string]::IsNullOrWhiteSpace($treeHash)) {
            throw 'Unable to compute git tree hash for ports/keyhcommon.'
        }

        return $treeHash
    }
    finally {
        Remove-Item Env:\GIT_INDEX_FILE -ErrorAction SilentlyContinue
        if (Test-Path -LiteralPath $tempIndex) {
            Remove-Item -LiteralPath $tempIndex -Force
        }
    }
}

foreach ($requiredPath in @($portfilePath, $vcpkgJsonPath, $versionsPath, $baselinePath)) {
    if (-not (Test-Path -LiteralPath $requiredPath)) {
        throw ('Required file not found: ' + $requiredPath)
    }
}

$headSha = (& git -C $repoRoot rev-parse --verify HEAD).Trim()
if ($LASTEXITCODE -ne 0 -or $headSha -notmatch '^[0-9A-Fa-f]{40}$') {
    Write-Host '[vcpkg registry] Skip: repository HEAD commit is unavailable or invalid.'
    exit 0
}

$portBytes = [System.IO.File]::ReadAllBytes($portfilePath)
$isUtf8Bom = ($portBytes.Length -ge 3 -and $portBytes[0] -eq 0xEF -and $portBytes[1] -eq 0xBB -and $portBytes[2] -eq 0xBF)
$isUtf16Le = ($portBytes.Length -ge 2 -and $portBytes[0] -eq 0xFF -and $portBytes[1] -eq 0xFE)
$isUtf16Be = ($portBytes.Length -ge 2 -and $portBytes[0] -eq 0xFE -and $portBytes[1] -eq 0xFF)
$isUtf8NoBom = $false

if (-not $isUtf8Bom -and -not $isUtf16Le -and -not $isUtf16Be) {
    try {
        $utf8 = New-Object System.Text.UTF8Encoding($false, $true)
        $utf8RoundTrip = $utf8.GetBytes($utf8.GetString($portBytes))
        if ($utf8RoundTrip.Length -eq $portBytes.Length) {
            $same = $true
            for ($i = 0; $i -lt $portBytes.Length; $i++) {
                if ($portBytes[$i] -ne $utf8RoundTrip[$i]) {
                    $same = $false
                    break
                }
            }

            if ($same) {
                $isUtf8NoBom = $true
            }
        }
    }
    catch {
        $isUtf8NoBom = $false
    }
}

$useDirectTextEncoding = $isUtf8Bom -or $isUtf8NoBom -or $isUtf16Le -or $isUtf16Be
$portfileEncoding = [System.Text.Encoding]::GetEncoding(28591)
if ($isUtf8Bom) {
    $portfileEncoding = New-Object System.Text.UTF8Encoding($true)
}
elseif ($isUtf8NoBom) {
    $portfileEncoding = New-Object System.Text.UTF8Encoding($false)
}
elseif ($isUtf16Le) {
    $portfileEncoding = [System.Text.Encoding]::Unicode
}
elseif ($isUtf16Be) {
    $portfileEncoding = [System.Text.Encoding]::BigEndianUnicode
}

$portfileContent =
if ($useDirectTextEncoding) {
    [System.IO.File]::ReadAllText($portfilePath, $portfileEncoding)
}
else {
    $portfileEncoding.GetString($portBytes)
}

$refRegex = '(?m)^(\s*REF\s+"")([0-9A-Fa-f]{40})(""\s*)$'
$refMatch = [regex]::Match($portfileContent, $refRegex)
if (-not $refMatch.Success) {
    throw 'REF line not found in portfile.'
}

$currentRef = $refMatch.Groups[2].Value
if ($currentRef -ine $headSha) {
    $updatedPortfileContent = [regex]::Replace($portfileContent, $refRegex, ('$1' + $headSha + '$3'), 1)
    if ($useDirectTextEncoding) {
        [System.IO.File]::WriteAllText($portfilePath, $updatedPortfileContent, $portfileEncoding)
    }
    else {
        [System.IO.File]::WriteAllBytes($portfilePath, $portfileEncoding.GetBytes($updatedPortfileContent))
    }
}

$manifest = Read-Json $vcpkgJsonPath
$versionField = Get-VersionFieldInfo $manifest
if ($null -eq $versionField -or [string]::IsNullOrWhiteSpace($versionField.Value)) {
    throw 'A supported version field is missing from ports/keyhcommon/vcpkg.json.'
}
$versionFieldName = $versionField.Name
$versionValue = $versionField.Value

$portVersion = 0
$manifestHasPortVersion = $null -ne $manifest.PSObject.Properties['port-version']
if ($manifestHasPortVersion) {
    $portVersion = [int]$manifest.'port-version'
}

$treeHash = Get-PortTreeHash $repoRoot $portDir

$versionsDocument = Read-Json $versionsPath
$existingEntries = @()
if ($null -ne $versionsDocument.versions) {
    $existingEntries = @($versionsDocument.versions)
}

$filteredEntries = New-Object System.Collections.ArrayList
foreach ($entry in $existingEntries) {
    $entryPortVersion = 0
    if ($null -ne $entry.'port-version') {
        $entryPortVersion = [int]$entry.'port-version'
    }

    $entryVersionField = Get-VersionFieldInfo $entry
    if ($null -ne $entryVersionField -and $entryVersionField.Name -eq $versionFieldName -and $entryVersionField.Value -eq $versionValue -and $entryPortVersion -eq $portVersion) {
        continue
    }

    [void]$filteredEntries.Add($entry)
}

$updatedEntryProperties = [ordered]@{
    'git-tree' = $treeHash
}
$updatedEntryProperties[$versionFieldName] = $versionValue
if ($manifestHasPortVersion) {
    $updatedEntryProperties['port-version'] = $portVersion
}
$updatedEntry = [pscustomobject]$updatedEntryProperties

$updatedVersionsDocumentProperties = [ordered]@{}
foreach ($property in $versionsDocument.PSObject.Properties) {
    if ($property.Name -ne 'versions') {
        $updatedVersionsDocumentProperties[$property.Name] = $property.Value
    }
}

$updatedVersionsDocumentProperties['versions'] = @($updatedEntry) + @($filteredEntries.ToArray())
Write-Json $versionsPath ([pscustomobject]$updatedVersionsDocumentProperties)

$baselineDocument = Read-Json $baselinePath
$defaultEntries = [ordered]@{}
if ($null -ne $baselineDocument.default) {
    foreach ($property in $baselineDocument.default.PSObject.Properties) {
        $defaultEntries[$property.Name] = $property.Value
    }
}

$keyhcommonBaselineEntry = [ordered]@{}
if ($null -ne $defaultEntries['keyhcommon']) {
    foreach ($property in $defaultEntries['keyhcommon'].PSObject.Properties) {
        $keyhcommonBaselineEntry[$property.Name] = $property.Value
    }
}

$keyhcommonBaselineEntry['baseline'] = $versionValue
if ($manifestHasPortVersion) {
    $keyhcommonBaselineEntry['port-version'] = $portVersion
}
else {
    $null = $keyhcommonBaselineEntry.Remove('port-version')
}
$defaultEntries['keyhcommon'] = [pscustomobject]$keyhcommonBaselineEntry

$updatedBaselineDocumentProperties = [ordered]@{}
foreach ($property in $baselineDocument.PSObject.Properties) {
    if ($property.Name -ne 'default') {
        $updatedBaselineDocumentProperties[$property.Name] = $property.Value
    }
}

$updatedBaselineDocumentProperties['default'] = [pscustomobject]$defaultEntries
Write-Json $baselinePath ([pscustomobject]$updatedBaselineDocumentProperties)

if ($currentRef -ieq $headSha) {
    Write-Host ('[vcpkg registry] REF already up to date: ' + $headSha)
}
else {
    Write-Host ('[vcpkg registry] Updated REF: ' + $currentRef + ' -> ' + $headSha)
}

$versionLabel = $versionValue
if ($manifestHasPortVersion) {
    $versionLabel += ('#' + $portVersion)
}

Write-Host ('[vcpkg registry] Synced versions/k-/keyhcommon.json to git-tree ' + $treeHash + ' for version ' + $versionLabel)
Write-Host ('[vcpkg registry] Synced versions/baseline.json to version ' + $versionLabel)
