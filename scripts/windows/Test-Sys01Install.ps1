#Requires -Version 5.1
# Synthetic filesystem controls only; no real plugin/DAW, signature or OS qualification.
$ErrorActionPreference = 'Stop'
Set-StrictMode -Version Latest
foreach ($script in Get-ChildItem -LiteralPath $PSScriptRoot -File | Where-Object { $_.Extension -in @('.ps1', '.psm1') }) {
    $tokens = $null; $errors = $null
    [void][System.Management.Automation.Language.Parser]::ParseFile($script.FullName, [ref]$tokens, [ref]$errors)
    if ($errors.Count) { throw ($errors | Out-String) }
}
Import-Module (Join-Path $PSScriptRoot 'Sys01.Install.psm1') -Force
function Require([bool]$Condition, [string]$Message) { if (-not $Condition) { throw $Message } }
function Reject([scriptblock]$Action) {
    $rejected = $false
    try { & $Action | Out-Null } catch { $rejected = $true }
    Require $rejected 'Unsafe or invalid operation was accepted.'
}
$base = Join-Path ([IO.Path]::GetTempPath()) ('sys01-install-controls-' + [Guid]::NewGuid().ToString('N'))
[void][IO.Directory]::CreateDirectory($base)
function New-TestCandidate([string]$Name, [string]$Payload) {
    $path = Join-Path $base $Name
    [void][IO.Directory]::CreateDirectory((Join-Path $path 'binaries/SYS 01.vst3/Contents/x86_64-win'))
    [void][IO.Directory]::CreateDirectory((Join-Path $path 'licenses'))
    Set-Content -LiteralPath (Join-Path $path 'binaries/SYS 01.vst3/Contents/x86_64-win/SYS 01.vst3') -Value $Payload -Encoding ASCII
    foreach ($file in @('LICENSE','NOTICE','VERSION','source.tar.gz','licenses/test-notice.txt')) {
        Set-Content -LiteralPath (Join-Path $path $file) -Value 'SYNTHETIC TEST FIXTURE' -Encoding ASCII
    }
    $hashes = @{}
    foreach ($file in Get-ChildItem -LiteralPath $path -Recurse -File) {
        $name = $file.FullName.Substring($path.Length + 1).Replace('\','/')
        $hashes[$name] = @{sha256=(Get-FileHash -LiteralPath $file.FullName -Algorithm SHA256).Hash.ToLowerInvariant(); size=$file.Length}
    }
    @{schema=2; product='SYS 01'; status='development-unqualified'; artifacts=@{'test-fingerprint'='binaries/SYS 01.vst3'}; hashes=$hashes} |
        ConvertTo-Json -Depth 100 | Set-Content -LiteralPath (Join-Path $path 'candidate.json') -Encoding UTF8
    return $path
}
$one = New-TestCandidate 'one' 'VERSION ONE: NOT EXECUTABLE'
$two = New-TestCandidate 'two' 'VERSION TWO: NOT EXECUTABLE'
$fixture = Join-Path $base 'fixture'
[void](Initialize-Sys01Fixture $fixture)
Reject { Initialize-Sys01Fixture $fixture }
Reject { Initialize-Sys01Fixture ([IO.Path]::GetTempPath()) }
Reject { Invoke-Sys01Install (Join-Path $base 'unmarked') $one $true $true }
Reject { Invoke-Sys01Install $fixture $one $false $true }
Reject { Invoke-Sys01Install $fixture $one $true $false }
foreach ($relative in @('../escape', 'VST3/.. /escape', 'C:/escape', 'VST3/CON', 'VST3/bad\path')) {
    Reject { & (Get-Module 'Sys01.Install') { param($root, $name) Get-Sys01SafeChild $root $name } $fixture $relative }
}
$junction = Join-Path $base 'fixture-junction'
[void](New-Item -ItemType Junction -Path $junction -Target $fixture)
try { Reject { Invoke-Sys01Install $junction $one $true $true } }
finally { [IO.Directory]::Delete($junction) }
$project = Join-Path $fixture 'preserved-project.als'
Set-Content -LiteralPath $project -Value 'USER PROJECT SENTINEL' -Encoding ASCII
[void](Invoke-Sys01Install $fixture $one $true $true)
$module = Join-Path $fixture 'installed/VST3/SYS 01.vst3/Contents/x86_64-win/SYS 01.vst3'
$first = (Get-FileHash -LiteralPath $module -Algorithm SHA256).Hash
$lock = [IO.File]::Open((Join-Path $fixture '.sys01-state/operation.lock'), [IO.FileMode]::Open, [IO.FileAccess]::ReadWrite, [IO.FileShare]::None)
try { Reject { Invoke-Sys01Install $fixture $two $true $true } }
finally { $lock.Dispose() }
[void](Invoke-Sys01Install $fixture $two $true $true)
Require ((Get-FileHash -LiteralPath $module -Algorithm SHA256).Hash -ne $first) 'Upgrade did not change fixture bytes.'
[void](Invoke-Sys01Rollback $fixture $true $true)
Require ((Get-FileHash -LiteralPath $module -Algorithm SHA256).Hash -eq $first) 'Rollback did not restore exact bytes.'
[void](Invoke-Sys01Uninstall $fixture $true)
Require (-not (Test-Path -LiteralPath (Join-Path $fixture 'installed'))) 'Uninstall left active deployment.'
[void](Invoke-Sys01Rollback $fixture $true $true)
Require ((Get-FileHash -LiteralPath $module -Algorithm SHA256).Hash -eq $first) 'Reinstall after uninstall lost bytes.'
$unknown = Join-Path $fixture 'installed/user-added.txt'
Set-Content -LiteralPath $unknown -Value 'PRESERVE ME'
Reject { Invoke-Sys01Uninstall $fixture $true }
Require (Test-Path -LiteralPath $unknown) 'Unknown file was deleted.'
Remove-Item -LiteralPath $unknown
$data = [IO.File]::ReadAllBytes($module)
[IO.File]::WriteAllText($module, 'tampered')
Reject { Invoke-Sys01Install $fixture $two $true $true }
[IO.File]::WriteAllBytes($module, $data)
# Inject a rename failure after moving the old owned tree; recovery must restore it.
$originalMover = & (Get-Module 'Sys01.Install') { (Get-Item Function:Move-Sys01Directory).ScriptBlock }
& (Get-Module 'Sys01.Install') {
    function script:Move-Sys01Directory {
        param([string]$Source, [string]$Destination)
        if ((Split-Path -Leaf $Source) -like 'stage-*' -and (Split-Path -Leaf $Destination) -eq 'installed') { throw 'Injected commit rename failure' }
        [IO.Directory]::Move($Source, $Destination)
    }
}
try { Reject { Invoke-Sys01Install $fixture $two $true $true } }
finally { & (Get-Module 'Sys01.Install') { param($body) Set-Item Function:script:Move-Sys01Directory -Value $body } $originalMover }
Require ((Get-FileHash -LiteralPath $module -Algorithm SHA256).Hash -eq $first) 'Failed upgrade did not recover previous owned deployment.'
Require ((Get-Content -LiteralPath $project -Raw).Trim() -eq 'USER PROJECT SENTINEL') 'Project sentinel changed.'
# Unsafe source paths must fail before any live deployment mutation.
$bad = New-TestCandidate 'bad' 'BAD PATH FIXTURE'
$manifestPath = Join-Path $bad 'candidate.json'
$manifest = Get-Content -LiteralPath $manifestPath -Raw | ConvertFrom-Json
$manifest.hashes | Add-Member -NotePropertyName '../escape' -NotePropertyValue @{sha256=('0' * 64); size=0}
$manifest | ConvertTo-Json -Depth 100 | Set-Content -LiteralPath $manifestPath -Encoding UTF8
Reject { Invoke-Sys01Install $fixture $bad $true $true }
Write-Output "PASS: synthetic parser/install/upgrade/rollback/uninstall/reinstall, guards, tamper and failed-rename recovery controls. Retained fixture: $base"
Write-Output 'No real plugin, trusted signature, host scan or clean supported-system qualification is established.'
