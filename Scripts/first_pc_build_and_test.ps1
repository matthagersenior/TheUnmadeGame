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
    Write-Host "Validating quest branches, 82-person memory and repeatable scene build plan."
    & python (Join-Path $PSScriptRoot "build_story_scene_handoff.py") --check 2>&1 |
        Tee-Object -FilePath (Join-Path $reportDir "story-scene-handoff-validation.log")
    if($LASTEXITCODE -ne 0){throw "QUEST_SCENE_CONTENT_OUT_OF_SYNC: run generator and commit outputs"}
    Write-Host "Validating 11 canonical city assembly kits, 82 residents, 54 quest beats and 10 rites."
    & python (Join-Path $PSScriptRoot "build_city_assembly.py") --check 2>&1 |
        Tee-Object -FilePath (Join-Path $reportDir "city-assembly-source-check.log")
    if($LASTEXITCODE -ne 0){throw "CITY_ASSEMBLY_SOURCE_DRIFT"}
    Write-Host "Checking cinematic explainer and nine-realm production handoff."
    & python (Join-Path $PSScriptRoot "build_cinematic_handoff.py") --check 2>&1 |
        Tee-Object -FilePath (Join-Path $reportDir "cinematic-production-validation.log")
    if($LASTEXITCODE -ne 0){throw "CINEMATIC_CONTENT_OUT_OF_SYNC: regenerate and commit data tables"}
    Write-Host "Checking nine-realm lived-universe authored work orders."
    & python (Join-Path $PSScriptRoot "validate_lived_universe.py") --check 2>&1 |
        Tee-Object -FilePath (Join-Path $reportDir "lived-universe-validation.log")
    if($LASTEXITCODE -ne 0){throw "LIVED_UNIVERSE_CONTENT_OUT_OF_SYNC: regenerate work order manifest"}
    Write-Host "Checking complete-universe director cinema source and audio-export contract."
    & python (Join-Path $PSScriptRoot "validate_directors_cut.py") 2>&1 |
        Tee-Object -FilePath (Join-Path $reportDir "directors-cinema-validation.log")
    if($LASTEXITCODE -ne 0){throw "DIRECTOR_CINEMATIC_INDEX_INVALID"}
    Write-Host "Validating canonical transmedia lore, actor identities and expansion callbacks."
    & python (Join-Path $PSScriptRoot "validate_transmedia_storyworld.py") --check 2>&1 |
        Tee-Object -FilePath (Join-Path $reportDir "transmedia-continuity-validation.log")
    if($LASTEXITCODE -ne 0){throw "TRANSMEDIA_LORE_DRIFT"}
    Write-Host "Checking physical Witness Echo gameplay integration and save safety."
    & python -m unittest discover -s (Join-Path $repo "Scripts/tests") -p "test_witness_echo_wiring.py" 2>&1 |
        Tee-Object -FilePath (Join-Path $reportDir "witness-echo-source-validation.log")
    if($LASTEXITCODE -ne 0){throw "WITNESS_ECHO_INTEGRATION_INVALID"}
    Write-Host "Checking three-city Witness Braid and optional persistent outcomes."
    & python -m unittest discover -s (Join-Path $repo "Scripts/tests") -p "test_witness_braid_wiring.py" 2>&1 |
        Tee-Object -FilePath (Join-Path $reportDir "witness-braid-source-validation.log")
    if($LASTEXITCODE -ne 0){throw "WITNESS_BRAID_INTEGRATION_INVALID"}
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
