<#
THE UNMADE — repeatable Windows first-PC handoff.
Nothing is installed or purchased. All engine steps fail closed with a log.
This does not create an authored map, packaged release or Pixel Streaming host.
#>
param(
    [Parameter(Mandatory=$true)][string]$UnrealRoot,
    [string]$TestFilter="Unmade",
    [int]$MinimumFreeDiskGB=100,
    [switch]$BuildOnly
)
$ErrorActionPreference="Stop"
$repo=(Resolve-Path (Join-Path $PSScriptRoot "..")).Path
$project=Join-Path $repo "TheUnmadeGame.uproject"
$build=Join-Path $UnrealRoot "Engine/Build/BatchFiles/Build.bat"
$stamp=[DateTime]::UtcNow.ToString("yyyyMMdd-HHmmss")
$reportDir=Join-Path (Join-Path $repo "TestReports") ("first-pc-"+$stamp)
New-Item -ItemType Directory -Force -Path $reportDir | Out-Null
$commit="unknown"
try {
    $commit=(& git -C $repo rev-parse HEAD 2>$null | Out-String).Trim()
} catch {}
Write-Host "THE UNMADE | commit=$commit | logs=$reportDir"
if(-not (Test-Path -PathType Leaf $project)){throw "PROJECT_NOT_FOUND: $project"}
if(-not (Test-Path -PathType Leaf $build)){throw "UNREAL_BUILD_NOT_FOUND: $build"}
if(-not [string]::IsNullOrWhiteSpace($TestFilter) -and
   $TestFilter -notmatch '^Unmade(\.[a-zA-Z0-9_]+)*$'){
    throw "INVALID_TEST_FILTER: expected Unmade or Unmade.<group>"
}
try {
    & (Join-Path $PSScriptRoot "check_unreal_host.ps1") -UnrealRoot $UnrealRoot -MinimumFreeDiskGB $MinimumFreeDiskGB 2>&1 |
        Tee-Object -FilePath (Join-Path $reportDir "preflight.log")
    if($LASTEXITCODE -ne 0){throw "HOST_PREFLIGHT_FAILED: exit $LASTEXITCODE"}
    Write-Host "Checking editable authoring pack against committed Unreal data tables."
    & python (Join-Path $PSScriptRoot "build_world_content.py") --check 2>&1 |
        Tee-Object -FilePath (Join-Path $reportDir "content-validation.log")
    if($LASTEXITCODE -ne 0){throw "WORLD_CONTENT_OUT_OF_SYNC: run generator and commit changes"}
    Write-Host "Compiling C++ Editor target; NOT creating content assets."
    & $build "TheUnmadeGameEditor" "Win64" "Development" "-Project=$project" "-WaitMutex" "-NoHotReloadFromIDE" 2>&1 |
        Tee-Object -FilePath (Join-Path $reportDir "editor-build.log")
    if($LASTEXITCODE -ne 0){throw "UNREAL_EDITOR_BUILD_FAILED: exit $LASTEXITCODE"}
    if(-not $BuildOnly){
        & (Join-Path $PSScriptRoot "run_ue_tests.ps1") -UnrealRoot $UnrealRoot -TestFilter $TestFilter 2>&1 |
            Tee-Object -FilePath (Join-Path $reportDir "automation.log")
        if($LASTEXITCODE -ne 0){throw "UNREAL_TEST_RUNNER_FAILED: exit $LASTEXITCODE"}
    }
    $result=if($BuildOnly){"BUILT_NOT_TESTED"}else{"COMPILED_AND_AUTOMATION_TESTED"}
    ("result=$result`ncommit=$commit`nengineRoot=$UnrealRoot`nfilter=$TestFilter`nUTC=$stamp") |
        Set-Content -Path (Join-Path $reportDir "result.txt") -Encoding UTF8
    Write-Host "$result. Reports in $reportDir. Real gameplay QA still required."
} catch {
    ("FAILED: "+$_.Exception.Message+"`ncommit=$commit`nUTC=$stamp") |
        Set-Content -Path (Join-Path $reportDir "failure.txt") -Encoding UTF8
    throw
}
