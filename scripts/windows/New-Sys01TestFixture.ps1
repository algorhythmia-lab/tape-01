#Requires -Version 5.1
[CmdletBinding()]
param([Parameter(Mandatory=$true)][string]$FixtureRoot)
$ErrorActionPreference = 'Stop'
Import-Module (Join-Path $PSScriptRoot 'Sys01.Install.psm1') -Force
Initialize-Sys01Fixture $FixtureRoot
