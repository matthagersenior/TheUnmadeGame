<# One-click Windows UE 5.8 first-PC source preparation; installs nothing. #>
[CmdletBinding()]
param(
    [string]$UnrealRoot="",
    [switch]$InspectOnly,
    [switch]$SkipStage,
    [switch]$DontOpenEditor,
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
    & $pythonExe.Source (Join-Path $PSScriptRoot "unreal_editor/first_pc_editor_batch.py") 2>&1 |
        Tee-Object -FilePath (Join-Path $report "staging-dry-run.log")
    if($LASTEXITCODE -ne 0){throw "STAGING_PLAN_INVALID"}
    if($InspectOnly){
        Write-Host "INSPECT_ONLY PASS: 7 source DataTables, 52 references, no editor writes"
        @("result=INSPECT_ONLY","commit=$commit","engineRoot=$UnrealRoot") |
            Set-Content -Path (Join-Path $report "result.txt") -Encoding UTF8
        exit 0
    }
    & (Join-Path $PSScriptRoot "first_pc_build_and_test.ps1") -UnrealRoot $UnrealRoot -MinimumFreeDiskGB $MinimumFreeDiskGB 2>&1 |
        Tee-Object -FilePath (Join-Path $report "build-automation-master.log")
    if($LASTEXITCODE -ne 0){throw "EDITOR_BUILD_OR_AUTOMATION_FAILED: see build-automation-master.log"}
    if(!$SkipStage){
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
               $result.staging.created+$result.staging.skipped -ne 52){
                throw "EDITOR_REFERENCE_IMPORT_INCOMPLETE"
            }
        }finally{
            Remove-Item Env:UNMADE_EDITOR_APPROVED -ErrorAction SilentlyContinue
            Remove-Item Env:UNMADE_EDITOR_RECEIPT -ErrorAction SilentlyContinue
            Remove-Item Env:UNMADE_COMMIT -ErrorAction SilentlyContinue
        }
    }
    $mode=if($SkipStage){"COMPILED_AND_TESTED_NOT_STAGED"}else{"EDITOR_COMPILED_TESTED_AND_REFERENCES_STAGED"}
    @("result=$mode","commit=$commit","engineRoot=$UnrealRoot","report=$report") |
        Set-Content -Path (Join-Path $report "result.txt") -Encoding UTF8
    Write-Host "PASS $mode; report and source commit in $report"
    if(!$DontOpenEditor){
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
