#Requires -Version 5.1
[CmdletBinding()]
param(
    [Parameter(Mandatory=$true)][string]$Candidate,
    [Parameter(Mandatory=$true)][string]$FixtureRoot,
    [switch]$HostsClosed,
    [switch]$AllowUnsignedFixture
)
$ErrorActionPreference = 'Stop'
Import-Module (Join-Path $PSScriptRoot 'Sys01.Install.psm1') -Force
Invoke-Sys01Install $FixtureRoot $Candidate $HostsClosed.IsPresent $AllowUnsignedFixture.IsPresent
