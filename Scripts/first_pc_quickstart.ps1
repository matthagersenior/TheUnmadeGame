<# One-click Windows UE 5.8 first-PC source preparation; installs nothing. #>
[CmdletBinding()]
param(
    [string]$UnrealRoot="",
    [switch]$InspectOnly,
    [switch]$SkipStage,
    [switch]$DontOpenEditor,
    [switch]$BuildOnly,
    [int]$MinimumFreeDiskGB=100
)
$ErrorActionPreference="Stop"
$repo=(Resolve-Path (Join-Path $PSScriptRoot "..")).Path
$project=Join-Path $repo "TheUnmadeGame.uproject"
$version=(Get-Content $project -Raw | ConvertFrom-Json).EngineAssociation
if($version -ne "5.8"){throw "ENGINE_ASSOCIATION_CHANGED: expected UE 5.8, found $version"}
$report=Join-Path $repo ("TestReports/quickstart-"+[DateTime]::UtcNow.ToString("yyyyMMdd-HHmmss"))
New-Item -ItemType Directory -Force -Path $report | Out-Null
$receipt=Join-Path $report "editor-import-receipt.json"
$commit="zip/no-git"
if(Get-Command git -ErrorAction SilentlyContinue){
    try{$commit=(& git -C $repo rev-parse HEAD 2>$null | Out-String).Trim()}catch{}
}
Write-Host "THE UNMADE | Windows UE $version | source $commit"
Write-Host "Results: $report"
$originalPath=$env:PATH
try{
    if([string]::IsNullOrWhiteSpace($UnrealRoot)){
        $base=Join-Path $env:ProgramFiles "Epic Games"
        $candidate=Join-Path $base ("UE_"+$version)
        if(Test-Path (Join-Path $candidate "Engine/Binaries/Win64/UnrealEditor.exe")){
            $UnrealRoot=$candidate
        } else {
            $candidates=@(Get-ChildItem $base -Directory -Filter "UE_*" -ErrorAction SilentlyContinue |
                Where-Object {$_.Name -eq ("UE_"+$version) -and
                    (Test-Path (Join-Path $_.FullName "Engine/Binaries/Win64/UnrealEditor.exe"))})
            if($candidates.Count -eq 1){$UnrealRoot=$candidates[0].FullName}
        }
    }
    if([string]::IsNullOrWhiteSpace($UnrealRoot)){
        throw "UNREAL_5_8_NOT_FOUND: install Unreal Engine 5.8, or rerun -UnrealRoot with its installed path"
    }
    $editor=Join-Path $UnrealRoot "Engine/Binaries/Win64/UnrealEditor.exe"
    $cmdEditor=Join-Path $UnrealRoot "Engine/Binaries/Win64/UnrealEditor-Cmd.exe"
    $bundledPython=Join-Path $UnrealRoot "Engine/Binaries/ThirdParty/Python3/Win64/python.exe"
    if(!(Test-Path $editor) -or !(Test-Path $cmdEditor)){
        throw "UNREAL_BINARIES_MISSING: verify UnrealRoot points at installed UE 5.8"
    }
    if(Test-Path $bundledPython){
        $env:PATH=(Split-Path $bundledPython -Parent)+";"+$env:PATH
    }
    $pythonExe=Get-Command python.exe -ErrorAction SilentlyContinue
    if(!$pythonExe){$pythonExe=Get-Command python -ErrorAction SilentlyContinue}
    if(!$pythonExe){throw "PYTHON_MISSING: need Python 3.11+ or Unreal bundled Python; no installer run"}
    & $pythonExe.Source -c 'import sys;assert sys.version_info >= (3,11)' 2>&1 | Out-Null
    if($LASTEXITCODE -ne 0){throw "PYTHON_TOO_OLD_OR_BROKEN: need Python 3.11+"}
    & $pythonExe.Source (Join-Path $PSScriptRoot "prepare_unreal_authoring.py") --check 2>&1 |
        Tee-Object -FilePath (Join-Path $report "authoring-plan.log")
    if($LASTEXITCODE -ne 0){throw "AUTHORING_PLAN_DRIFT: source Unreal CSVs invalid"}
    # Produce source-linked dossiers before any editor build or mutation.
    $productionPack=Join-Path $report "production-pack"
    & $pythonExe.Source (Join-Path $PSScriptRoot "build_first_pc_production_pack.py") --out-dir $productionPack 2>&1 |
        Tee-Object -FilePath (Join-Path $report "production-pack-validation.log")
    if($LASTEXITCODE -ne 0){throw "CANONICAL_PRODUCTION_PACK_INVALID: repair IDs or authoring contracts"}
    # Build actual importable, source-authored low-poly static models. These are
    # separate prototype assets; never overwrite the user's Unreal game worlds.
    $geometryDir=Join-Path $repo "Build/UnmadePrototypeGeometry"
    & $pythonExe.Source (Join-Path $PSScriptRoot "build_realm_geometry.py") --out-dir $geometryDir 2>&1 |
        Tee-Object -FilePath (Join-Path $report "prototype-geometry-validation.log")
    if($LASTEXITCODE -ne 0){throw "PROTOTYPE_GEOMETRY_INVALID: inspect geometry generation log"}
    Write-Host "Prototype models ready for later Unreal import: $geometryDir"
    # Every canonical item and regional reward gets a real, independent
    # prototype mesh before Unreal import, with no loot/progression side effect.
    $equipmentDir=Join-Path $repo "Build/UnmadePrototypeEquipment"
    & $pythonExe.Source (Join-Path $PSScriptRoot "build_equipment_geometry.py") --out-dir $equipmentDir 2>&1 |
        Tee-Object -FilePath (Join-Path $report "prototype-equipment-validation.log")
    if($LASTEXITCODE -ne 0){throw "PROTOTYPE_EQUIPMENT_INVALID: inspect equipment generation log"}
    Write-Host "Prototype equipment meshes ready for later import: $equipmentDir"
    & $pythonExe.Source (Join-Path $PSScriptRoot "unreal_editor/first_pc_editor_batch.py") 2>&1 |
        Tee-Object -FilePath (Join-Path $report "staging-dry-run.log")
    if($LASTEXITCODE -ne 0){throw "STAGING_PLAN_INVALID"}
    $audioDir=Join-Path $repo "Build/UnpaidShiftAudio"
    & $pythonExe.Source (Join-Path $PSScriptRoot "build_unpaid_shift_audio.py") --out-dir $audioDir --check 2>&1 |
        Tee-Object -FilePath (Join-Path $report "unpaid-shift-stereo-foley-validation.log")
    if($LASTEXITCODE -ne 0){throw "UNPAID_SHIFT_AUDIO_GENERATION_INVALID"}
    & $pythonExe.Source (Join-Path $PSScriptRoot "unreal_editor/import_unpaid_shift_audio.py") 2>&1 |
        Tee-Object -FilePath (Join-Path $report "unpaid-shift-import-dry-run.log")
    if($LASTEXITCODE -ne 0){throw "UNPAID_SHIFT_AUDIO_MANIFEST_INVALID"}

    if($InspectOnly){
        Write-Host "INSPECT_ONLY PASS: 7 source DataTables, 52 references, no editor writes"
        @("result=INSPECT_ONLY","commit=$commit","engineRoot=$UnrealRoot") |
            Set-Content -Path (Join-Path $report "result.txt") -Encoding UTF8
        exit 0
    }
    & (Join-Path $PSScriptRoot "first_pc_build_and_test.ps1") -UnrealRoot $UnrealRoot -MinimumFreeDiskGB $MinimumFreeDiskGB -BuildOnly:$BuildOnly 2>&1 |
        Tee-Object -FilePath (Join-Path $report "build-automation-master.log")
    if($LASTEXITCODE -ne 0){throw "EDITOR_BUILD_OR_AUTOMATION_FAILED: see build-automation-master.log"}
    if(!$SkipStage -and !$BuildOnly){
        $env:UNMADE_EDITOR_APPROVED="1"
        $env:UNMADE_EDITOR_RECEIPT=$receipt
        $env:UNMADE_COMMIT=$commit
        $batch=Join-Path $PSScriptRoot "unreal_editor/first_pc_editor_batch.py"
        try{
            & $cmdEditor $project "-unattended" "-nop4" "-nosplash" "-run=pythonscript" "-script=$batch" 2>&1 |
                Tee-Object -FilePath (Join-Path $report "editor-import.log")
            if($LASTEXITCODE -ne 0){throw "EDITOR_PYTHON_IMPORT_FAILED: see editor-import.log"}
            if(!(Test-Path $receipt)){throw "NO_EDITOR_IMPORT_RECEIPT: commandlet did not run the script"}
            $result=Get-Content $receipt -Raw | ConvertFrom-Json
            if($result.status -ne "EDITOR_IMPORT_ATTEMPT_COMPLETE" -or
               @($result.tables).Count -ne 7 -or
               $result.staging.created+$result.staging.skipped -ne 52 -or
               $result.lived_staging.created+$result.lived_staging.skipped -ne 90 -or
               $result.city_assembly.created+$result.city_assembly.skipped -ne 402){
                throw "EDITOR_REFERENCE_IMPORT_INCOMPLETE"
            }
            # Install only original, generated prototype Foley (never overwrite
            # user-recorded voices or replaced SoundWaves).
            $env:UNMADE_AUDIO_IMPORT_APPROVED="1"
            $audioReceipt=Join-Path $report "unpaid-shift-audio-import-receipt.json"
            $env:UNMADE_AUDIO_IMPORT_RECEIPT=$audioReceipt
            $audioBatch=Join-Path $PSScriptRoot "unreal_editor/import_unpaid_shift_audio.py"
            try{
                & $cmdEditor $project "-unattended" "-nop4" "-nosplash" "-run=pythonscript" "-script=$audioBatch" 2>&1 |
                    Tee-Object -FilePath (Join-Path $report "unpaid-shift-audio-import.log")
                if($LASTEXITCODE -ne 0){throw "UNPAID_SHIFT_SOUNDWAVE_IMPORT_FAILED"}
                if(!(Test-Path $audioReceipt)){throw "AUDIO_IMPORT_RECEIPT_MISSING"}
                $ar=Get-Content $audioReceipt -Raw | ConvertFrom-Json
                if($ar.mode -ne "unreal_import_complete" -or $ar.count -ne 35){
                    throw "UNPAID_SHIFT_SOUNDWAVES_INCOMPLETE"
                }
            }finally{
                Remove-Item Env:UNMADE_AUDIO_IMPORT_APPROVED -ErrorAction SilentlyContinue
                Remove-Item Env:UNMADE_AUDIO_IMPORT_RECEIPT -ErrorAction SilentlyContinue
            }

        }finally{
            Remove-Item Env:UNMADE_EDITOR_APPROVED -ErrorAction SilentlyContinue
            Remove-Item Env:UNMADE_EDITOR_RECEIPT -ErrorAction SilentlyContinue
            Remove-Item Env:UNMADE_COMMIT -ErrorAction SilentlyContinue
        }
    }
    if($BuildOnly){$mode="BUILT_NOT_TESTED_NOT_STAGED"}
    elseif($SkipStage){$mode="COMPILED_AND_TESTED_NOT_STAGED"}
    else{$mode="EDITOR_COMPILED_TESTED_AND_REFERENCES_STAGED"}
    @("result=$mode","commit=$commit","engineRoot=$UnrealRoot","report=$report") |
        Set-Content -Path (Join-Path $report "result.txt") -Encoding UTF8
    Write-Host "PASS $mode; report and source commit in $report"
    if(!$DontOpenEditor -and !$BuildOnly){
        Start-Process -FilePath $editor -ArgumentList ('"'+$project+'"')
        Write-Host "Opening Unreal Editor. Staging reference blocks are NOT finished gameplay art."
    }
    Write-Host "Still required: Unreal physical playtest, level art, animations, accessible UMG, full QA."
} catch{
    @("FAILED: "+$_.Exception.Message,"commit=$commit","report=$report") |
        Set-Content -Path (Join-Path $report "failure.txt") -Encoding UTF8
    Write-Error $_.Exception.Message
    exit 1
} finally {
    $env:PATH=$originalPath
}
