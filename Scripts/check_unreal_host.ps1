param(
    [Parameter(Mandatory = $true)][string]$UnrealRoot,
    [int]$MinimumFreeDiskGB = 100
)
$ErrorActionPreference = "Stop"
$issues = New-Object 'System.Collections.Generic.List[string]'
$project = Join-Path (Resolve-Path (Join-Path $PSScriptRoot "..")).Path "TheUnmadeGame.uproject"
$os = Get-CimInstance Win32_OperatingSystem
Write-Host "Windows: $($os.Caption) | version $($os.Version) | build $($os.BuildNumber) | $($os.OSArchitecture)"
if ($os.OSArchitecture -notmatch '64') { $issues.Add("A 64-bit Windows host is required for the Win64 Editor target") }
$editor = Join-Path $UnrealRoot "Engine/Binaries/Win64/UnrealEditor.exe"
$commandlet = Join-Path $UnrealRoot "Engine/Binaries/Win64/UnrealEditor-Cmd.exe"
Write-Host "THE UNMADE: prerequisites only; no installation or cloud provisioning"
if (-not (Test-Path $editor -PathType Leaf) -or -not (Test-Path $commandlet -PathType Leaf)) {
    $issues.Add("UnrealEditor-Cmd.exe / UnrealEditor.exe not found under $UnrealRoot")
}
$buildBat = Join-Path $UnrealRoot "Engine/Build/BatchFiles/Build.bat"
if (-not (Test-Path $buildBat -PathType Leaf)) { $issues.Add("Build.bat not found under $UnrealRoot") }
$projectInfo = $null
if (Test-Path $project -PathType Leaf) {
    try { $projectInfo = Get-Content $project -Raw | ConvertFrom-Json }
    catch { $issues.Add("TheUnmadeGame.uproject is not valid JSON") }
}
$buildVersionPath = Join-Path $UnrealRoot "Engine/Build/Build.version"
if (-not (Test-Path $buildVersionPath -PathType Leaf)) {
    $issues.Add("Engine/Build/Build.version missing; UnrealRoot may not be a complete UE installation")
} else {
    try {
        $buildVersion = Get-Content $buildVersionPath -Raw | ConvertFrom-Json
        $installedAssociation = "$($buildVersion.MajorVersion).$($buildVersion.MinorVersion)"
        Write-Host "Installed Unreal: $installedAssociation.$($buildVersion.PatchVersion) | changelist $($buildVersion.Changelist)"
        if ($null -ne $projectInfo -and $installedAssociation -ne $projectInfo.EngineAssociation) {
            $issues.Add("Project targets UE $($projectInfo.EngineAssociation), but this installation is UE $installedAssociation")
        }
    } catch { $issues.Add("Could not parse Engine/Build/Build.version") }
}
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
    if ([string]::IsNullOrWhiteSpace($visualStudio)) { $issues.Add("Visual Studio C++ x64 toolset not detected; install the C++ x64/x86 build tools workload") }
    else {
        $vsVersion = & $vswhere -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationVersion
        $compilerRoot = Join-Path $visualStudio "VC/Tools/MSVC"
        $compilerDirs = @(Get-ChildItem $compilerRoot -Directory -ErrorAction SilentlyContinue | Sort-Object { [version]$_.Name } -Descending)
        $cl = $null
        foreach ($dir in $compilerDirs) {
            $candidateCl = Join-Path $dir.FullName "bin/Hostx64/x64/cl.exe"
            if (Test-Path $candidateCl -PathType Leaf) { $cl = $candidateCl; break }
        }
        if ($null -eq $cl) { $issues.Add("MSVC x64 compiler cl.exe not found under $compilerRoot") }
        Write-Host "Visual Studio: $vsVersion | $visualStudio"
        if ($null -ne $cl) { Write-Host "MSVC compiler: $cl" }
    }
}
$sdkRoot = Join-Path ([Environment]::GetFolderPath("ProgramFilesX86")) "Windows Kits/10/Include"
$sdkVersions = @()
if (Test-Path $sdkRoot -PathType Container) {
    $sdkVersions = @(Get-ChildItem $sdkRoot -Directory -ErrorAction SilentlyContinue |
        Where-Object {
            (Test-Path (Join-Path $_.FullName "um")) -and
            (Test-Path (Join-Path $_.FullName "shared")) -and
            (Test-Path (Join-Path $_.FullName "ucrt"))
        } | Select-Object -ExpandProperty Name)
}
if ($sdkVersions.Count -eq 0) {
    $issues.Add("Windows 10/11 SDK headers not found at $sdkRoot; add a Windows SDK component in Visual Studio Installer")
} else {
    Write-Host "Windows SDK versions: $($sdkVersions -join ', ')"
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
