Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

function Assert-Sys01PlainPath([string]$Path) {
    $itemPath = [IO.Path]::GetFullPath($Path)
    while ($itemPath) {
        if (Test-Path -LiteralPath $itemPath) {
            $item = Get-Item -LiteralPath $itemPath -Force
            if ($item.Attributes -band [IO.FileAttributes]::ReparsePoint) {
                throw "Reparse points are not permitted: $itemPath"
            }
        }
        $parent = [IO.Path]::GetDirectoryName($itemPath)
        if ($parent -eq $itemPath) { break }
        $itemPath = $parent
    }
}

function Get-Sys01Fixture([string]$FixtureRoot) {
    if ([Environment]::OSVersion.Platform -ne [PlatformID]::Win32NT) { throw 'Windows test fixture required.' }
    $root = [IO.Path]::GetFullPath($FixtureRoot).TrimEnd([IO.Path]::DirectorySeparatorChar)
    $temporary = [IO.Path]::GetFullPath([IO.Path]::GetTempPath()).TrimEnd([IO.Path]::DirectorySeparatorChar)
    if (-not $root.StartsWith($temporary + [IO.Path]::DirectorySeparatorChar, [StringComparison]::OrdinalIgnoreCase)) {
        throw 'Only a dedicated fixture below the current temporary directory is supported in Sprint 06.'
    }
    Assert-Sys01PlainPath $root
    $marker = Join-Path $root '.sys01-test-fixture.json'
    $fixture = Get-Content -LiteralPath $marker -Raw | ConvertFrom-Json
    if ($fixture.schema -ne 1 -or $fixture.product -ne 'SYS 01' -or $fixture.purpose -ne 'release-install-test') {
        throw 'An explicitly initialized SYS 01 test fixture is required.'
    }
    foreach ($item in Get-ChildItem -LiteralPath $root -Recurse -Force) {
        if ($item.Attributes -band [IO.FileAttributes]::ReparsePoint) { throw 'Fixture contains a reparse point.' }
    }
    return $root
}

function Initialize-Sys01Fixture([string]$FixtureRoot) {
    if ([Environment]::OSVersion.Platform -ne [PlatformID]::Win32NT) { throw 'Windows test fixture required.' }
    $root = [IO.Path]::GetFullPath($FixtureRoot).TrimEnd([IO.Path]::DirectorySeparatorChar)
    $temporary = [IO.Path]::GetFullPath([IO.Path]::GetTempPath()).TrimEnd([IO.Path]::DirectorySeparatorChar)
    if (-not $root.StartsWith($temporary + [IO.Path]::DirectorySeparatorChar, [StringComparison]::OrdinalIgnoreCase)) {
        throw 'Fixture must be a new directory below the current temporary directory.'
    }
    Assert-Sys01PlainPath $root
    if (Test-Path -LiteralPath $root) { throw 'Refusing to initialize an existing directory.' }
    [void][IO.Directory]::CreateDirectory($root)
    @{schema=1; product='SYS 01'; purpose='release-install-test'} | ConvertTo-Json |
        Set-Content -LiteralPath (Join-Path $root '.sys01-test-fixture.json') -Encoding UTF8
    return $root
}

function Get-Sys01SafeChild([string]$Root, [string]$Relative) {
    if ([string]::IsNullOrWhiteSpace($Relative) -or $Relative.Contains('\') -or
        $Relative.Contains(':') -or $Relative.StartsWith('/') -or
        @($Relative.Split('/') | Where-Object { $_ -eq '..' -or $_ -eq '.' -or $_ -eq '' -or $_ -match '[. ]$' -or $_ -match '^(CON|PRN|AUX|NUL|COM[1-9]|LPT[1-9])(\..*)?$' }).Count) {
        throw "Unsafe manifest path: $Relative"
    }
    $full = [IO.Path]::GetFullPath((Join-Path $Root $Relative))
    $prefix = [IO.Path]::GetFullPath($Root).TrimEnd([IO.Path]::DirectorySeparatorChar) + [IO.Path]::DirectorySeparatorChar
    if (-not $full.StartsWith($prefix, [StringComparison]::OrdinalIgnoreCase)) { throw 'Path escapes root.' }
    Assert-Sys01PlainPath $full
    return $full
}

function Get-Sys01Files([string]$Root, [string]$Exclude = '') {
    $files = @{}
    $base = [IO.Path]::GetFullPath($Root).TrimEnd([IO.Path]::DirectorySeparatorChar)
    Assert-Sys01PlainPath $base
    foreach ($item in Get-ChildItem -LiteralPath $base -Recurse -Force) {
        if ($item.Attributes -band [IO.FileAttributes]::ReparsePoint) { throw 'Reparse point in payload.' }
        if (-not $item.PSIsContainer) {
            $name = $item.FullName.Substring($base.Length + 1).Replace('\', '/')
            if ($name -eq $Exclude) { continue }
            if ($files.ContainsKey($name)) { throw 'Case-colliding file paths.' }
            $files[$name] = @{sha256=(Get-FileHash -LiteralPath $item.FullName -Algorithm SHA256).Hash.ToLowerInvariant(); size=$item.Length}
        }
    }
    return $files
}

function Assert-Sys01Inventory([string]$Root, $Expected, [string]$Exclude) {
    $actual = Get-Sys01Files $Root $Exclude
    $properties = @($Expected.PSObject.Properties)
    if ($actual.Count -ne $properties.Count) { throw 'Payload file-set mismatch; unexpected files are preserved.' }
    $seen = @{}
    foreach ($property in $properties) {
        [void](Get-Sys01SafeChild $Root $property.Name)
        if ($seen.ContainsKey($property.Name)) { throw 'Case-colliding manifest paths.' }
        $seen[$property.Name] = $true
        if (-not $actual.ContainsKey($property.Name) -or
            $actual[$property.Name].sha256 -ne $property.Value.sha256 -or
            $actual[$property.Name].size -ne $property.Value.size) { throw "Payload hash mismatch: $($property.Name)" }
    }
}

function Assert-Sys01Signature([string]$Module, [bool]$AllowUnsignedFixture) {
    if ($AllowUnsignedFixture) { return 'UNSIGNED-TEST-FIXTURE; not a trusted-signature result' }
    $signature = Get-AuthenticodeSignature -LiteralPath $Module
    if ($signature.Status -ne 'Valid' -or $null -eq $signature.TimeStamperCertificate) {
        throw 'A valid trusted Authenticode signature and timestamp are required. No unsigned normal install.'
    }
    return $signature.SignerCertificate.Thumbprint
}

function Assert-Sys01Candidate([string]$Candidate, [bool]$AllowUnsignedFixture) {
    $root = [IO.Path]::GetFullPath($Candidate)
    Assert-Sys01PlainPath $root
    $manifest = Get-Content -LiteralPath (Join-Path $root 'candidate.json') -Raw | ConvertFrom-Json
    if ($manifest.schema -ne 2 -or $manifest.product -ne 'SYS 01' -or $manifest.status -ne 'development-unqualified') {
        throw 'Expected a SYS 01 Sprint 06 development packet.'
    }
    Assert-Sys01Inventory $root $manifest.hashes 'candidate.json'
    $artifacts = @($manifest.artifacts.PSObject.Properties)
    if ($artifacts.Count -ne 1 -or $artifacts[0].Value -ne 'binaries/SYS 01.vst3') {
        throw 'Expected a Windows-only VST3 packet; macOS bundles cannot be installed on Windows.'
    }
    $module = Get-Sys01SafeChild $root 'binaries/SYS 01.vst3/Contents/x86_64-win/SYS 01.vst3'
    if (-not (Test-Path -LiteralPath $module -PathType Leaf)) { throw 'Windows x64 VST3 executable is missing.' }
    $signer = Assert-Sys01Signature $module $AllowUnsignedFixture
    return @{root=$root; signer=$signer; manifest_sha256=(Get-FileHash -LiteralPath (Join-Path $root 'candidate.json') -Algorithm SHA256).Hash.ToLowerInvariant()}
}

function Get-Sys01Receipt([string]$Directory) {
    $receipt = Get-Content -LiteralPath (Join-Path $Directory 'receipt.json') -Raw | ConvertFrom-Json
    if ($receipt.schema -ne 1 -or $receipt.product -ne 'SYS 01') { throw 'Unrecognized ownership receipt.' }
    Assert-Sys01Inventory $Directory $receipt.files 'receipt.json'
    return $receipt
}

function Open-Sys01Operation([string]$Root) {
    $state = Join-Path $Root '.sys01-state'
    Assert-Sys01PlainPath $state
    [void][IO.Directory]::CreateDirectory($state)
    return [IO.File]::Open((Join-Path $state 'operation.lock'), [IO.FileMode]::OpenOrCreate, [IO.FileAccess]::ReadWrite, [IO.FileShare]::None)
}

function Move-Sys01Directory([string]$Source, [string]$Destination) {
    # Unlike Move-Item, an existing destination must fail rather than nest a tree.
    [IO.Directory]::Move($Source, $Destination)
}

function Set-Sys01Deployment([string]$Root, [string]$Stage) {
    $active = Join-Path $Root 'installed'
    $state = Join-Path $Root '.sys01-state'
    $previous = $null
    $historyId = $null
    if (Test-Path -LiteralPath $active) {
        [void](Get-Sys01Receipt $active)
        $historyId = [Guid]::NewGuid().ToString('N')
        $previous = Join-Path (Join-Path $state 'history') $historyId
        [void][IO.Directory]::CreateDirectory((Split-Path -Parent $previous))
    }
    $receipt = Get-Sys01Receipt $Stage
    $receipt.rollback_id = $historyId
    $receipt | ConvertTo-Json -Depth 100 | Set-Content -LiteralPath (Join-Path $Stage 'receipt.json') -Encoding UTF8
    if ($previous) { Move-Sys01Directory $active $previous }
    try {
        Move-Sys01Directory $Stage $active
    } catch {
        if ($previous -and -not (Test-Path -LiteralPath $active)) {
            Move-Sys01Directory $previous $active
        }
        throw
    }
    $uninstalled = Join-Path $state 'uninstalled.json'
    if (Test-Path -LiteralPath $uninstalled) { Remove-Item -LiteralPath $uninstalled }
    return $active
}

function Invoke-Sys01Install([string]$FixtureRoot, [string]$Candidate, [bool]$HostsClosed, [bool]$AllowUnsignedFixture) {
    if (-not $HostsClosed) { throw 'Close hosts and supply -HostsClosed before changing the fixture scan path.' }
    $root = Get-Sys01Fixture $FixtureRoot
    $candidateInfo = Assert-Sys01Candidate $Candidate $AllowUnsignedFixture
    $operation = Open-Sys01Operation $root
    $stage = Join-Path (Join-Path $root '.sys01-state') ('stage-' + [Guid]::NewGuid().ToString('N'))
    try {
        if (Test-Path -LiteralPath (Join-Path $root 'installed')) { [void](Get-Sys01Receipt (Join-Path $root 'installed')) }
        [void][IO.Directory]::CreateDirectory((Join-Path $stage 'VST3'))
        Copy-Item -LiteralPath (Join-Path $candidateInfo.root 'binaries/SYS 01.vst3') -Destination (Join-Path $stage 'VST3/SYS 01.vst3') -Recurse
        $notices = Join-Path $stage 'Notices'
        [void][IO.Directory]::CreateDirectory($notices)
        foreach ($name in @('LICENSE', 'NOTICE', 'VERSION', 'source.tar.gz', 'candidate.json')) {
            Copy-Item -LiteralPath (Join-Path $candidateInfo.root $name) -Destination (Join-Path $notices $name)
        }
        Copy-Item -LiteralPath (Join-Path $candidateInfo.root 'licenses') -Destination (Join-Path $notices 'licenses') -Recurse
        # Recheck immutable source and staged bytes before the rename transaction.
        [void](Assert-Sys01Candidate $candidateInfo.root $AllowUnsignedFixture)
        $staged = Get-Sys01Files $stage
        foreach ($property in $staged.Keys) {
            $sourceName = if ($property.StartsWith('VST3/')) { 'binaries/' + $property.Substring(5) } else { $property.Substring(8) }
            $sourceFile = Get-Sys01SafeChild $candidateInfo.root $sourceName
            if ((Get-FileHash -LiteralPath $sourceFile -Algorithm SHA256).Hash.ToLowerInvariant() -ne $staged[$property].sha256) { throw 'Staging copy mismatch.' }
        }
        $receipt = @{schema=1; product='SYS 01'; candidate_manifest_sha256=$candidateInfo.manifest_sha256; signer=$candidateInfo.signer; unsigned_fixture=$AllowUnsignedFixture; rollback_id=$null; files=$staged}
        $receipt | ConvertTo-Json -Depth 100 | Set-Content -LiteralPath (Join-Path $stage 'receipt.json') -Encoding UTF8
        Set-Sys01Deployment $root $stage
    } finally {
        # Failed staging is retained for inspection; never delete unknown user files.
        $operation.Dispose()
    }
}

function Invoke-Sys01Uninstall([string]$FixtureRoot, [bool]$HostsClosed) {
    if (-not $HostsClosed) { throw 'Supply -HostsClosed.' }
    $root = Get-Sys01Fixture $FixtureRoot
    $operation = Open-Sys01Operation $root
    try {
        $active = Join-Path $root 'installed'
        [void](Get-Sys01Receipt $active)
        $id = [Guid]::NewGuid().ToString('N')
        $history = Join-Path (Join-Path $root '.sys01-state/history') $id
        [void][IO.Directory]::CreateDirectory((Split-Path -Parent $history))
        $marker = Join-Path $root '.sys01-state/uninstalled.json'
        # Write recovery information before moving any installed content.
        @{rollback_id=$id} | ConvertTo-Json | Set-Content -LiteralPath $marker -Encoding UTF8
        try { Move-Sys01Directory $active $history } catch { Remove-Item -LiteralPath $marker; throw }
        Write-Output 'Owned deployment removed from the scan path; backup retained. Projects/preferences outside it are untouched.'
    } finally { $operation.Dispose() }
}

function Invoke-Sys01Rollback([string]$FixtureRoot, [bool]$HostsClosed, [bool]$AllowUnsignedFixture) {
    if (-not $HostsClosed) { throw 'Supply -HostsClosed.' }
    $root = Get-Sys01Fixture $FixtureRoot
    $operation = Open-Sys01Operation $root
    try {
        $active = Join-Path $root 'installed'
        if (Test-Path -LiteralPath $active) {
            $id = (Get-Sys01Receipt $active).rollback_id
        } else {
            $id = (Get-Content -LiteralPath (Join-Path $root '.sys01-state/uninstalled.json') -Raw | ConvertFrom-Json).rollback_id
        }
        if ($id -notmatch '^[a-f0-9]{32}$') { throw 'No owned rollback snapshot is available.' }
        $snapshot = Join-Path (Join-Path $root '.sys01-state/history') $id
        [void](Get-Sys01Receipt $snapshot)
        [void](Assert-Sys01Signature (Join-Path $snapshot 'VST3/SYS 01.vst3/Contents/x86_64-win/SYS 01.vst3') $AllowUnsignedFixture)
        $stage = Join-Path (Join-Path $root '.sys01-state') ('stage-' + [Guid]::NewGuid().ToString('N'))
        Copy-Item -LiteralPath $snapshot -Destination $stage -Recurse
        [void](Get-Sys01Receipt $stage)
        Set-Sys01Deployment $root $stage
    } finally { $operation.Dispose() }
}

Export-ModuleMember -Function Initialize-Sys01Fixture, Invoke-Sys01Install, Invoke-Sys01Rollback, Invoke-Sys01Uninstall
