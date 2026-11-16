#Requires -Version 5.1
[CmdletBinding()]
param([Parameter(Mandatory=$true)][string]$FixtureRoot, [switch]$HostsClosed)
$ErrorActionPreference = 'Stop'
Import-Module (Join-Path $PSScriptRoot 'Sys01.Install.psm1') -Force
Invoke-Sys01Uninstall $FixtureRoot $HostsClosed.IsPresent
