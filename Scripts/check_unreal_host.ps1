param(
    [Parameter(Mandatory = $true)][string]$UnrealRoot,
    [int]$MinimumFreeDiskGB = 100
)
$ErrorActionPreference = "Stop"
$issues = New-Object 'System.Collections.Generic.List[string]'
$editor = Join-Path $UnrealRoot "Engine/Binaries/Win64/UnrealEditor.exe"
$commandlet = Join-Path $UnrealRoot "Engine/Binaries/Win64/UnrealEditor-Cmd.exe"
Write-Host "THE UNMADE: prerequisites only; no installation or cloud provisioning"
if (-not (Test-Path $editor -PathType Leaf) -or -not (Test-Path $commandlet -PathType Leaf)) {
    $issues.Add("UnrealEditor-Cmd.exe / UnrealEditor.exe not found")
}
$project = Join-Path (Resolve-Path (Join-Path $PSScriptRoot "..")).Path "TheUnmadeGame.uproject"
if (-not (Test-Path $project -PathType Leaf)) { $issues.Add("TheUnmadeGame.uproject not found") }
if ($MinimumFreeDiskGB -lt 1) { $issues.Add("MinimumFreeDiskGB must be positive") }
try {
    $drive = [System.IO.Path]::GetPathRoot($UnrealRoot)
    $disk = Get-CimInstance Win32_LogicalDisk -Filter ("DeviceID='" + $drive.TrimEnd('\') + "'")
    if ($null -eq $disk -or $null -eq $disk.FreeSpace) {
        $issues.Add("Unable to check free disk space")
    } else {
        $free = [math]::Floor([double]$disk.FreeSpace / 1GB)
        Write-Host "Free space: $free GB"
        if ($free -lt $MinimumFreeDiskGB) { $issues.Add("At least $MinimumFreeDiskGB GB of free space required") }
    }
} catch { $issues.Add("Unable to inspect free disk space") }
$vswhere = Join-Path ([Environment]::GetFolderPath("ProgramFilesX86")) "Microsoft Visual Studio/Installer/vswhere.exe"
if (-not (Test-Path $vswhere -PathType Leaf)) {
    $issues.Add("Visual Studio vswhere.exe missing")
} else {
    $visualStudio = & $vswhere -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
    if ([string]::IsNullOrWhiteSpace($visualStudio)) { $issues.Add("Visual Studio C++ x64 toolset not detected") }
    else { Write-Host "Visual Studio C++ toolset detected" }
}
try {
    $gpus = @(Get-CimInstance Win32_VideoController | Select-Object -ExpandProperty Name)
    if ($gpus.Count -eq 0) { $issues.Add("No display adapter reported") }
    else { Write-Host "Adapters: $($gpus -join ', ')" }
} catch { $issues.Add("Cannot enumerate display adapters") }
if ($issues.Count -gt 0) {
    foreach ($issue in $issues) { Write-Warning "NOT_READY: $issue" }
    exit 3
}
Write-Host "PREFLIGHT PASSED. This is NOT proof of Unreal compilation, game launch or GPU performance."
exit 0
