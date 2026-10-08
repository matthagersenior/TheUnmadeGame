param(
    [Parameter(Mandatory = $true)][string]$UnrealRoot,
    [string]$TestFilter = "Unmade.Bootstrap"
)
$ErrorActionPreference = "Stop"
$repo = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
$project = Join-Path $repo "TheUnmadeGame.uproject"
$editor = Join-Path $UnrealRoot "Engine/Binaries/Win64/UnrealEditor-Cmd.exe"
if (-not (Test-Path -PathType Leaf $project)) {
    throw "PROJECT_NOT_FOUND: $project"
}
if (-not (Test-Path -PathType Leaf $editor)) {
    throw "UNREAL_HOST_NOT_CONFIGURED: UnrealEditor-Cmd.exe not found at $editor"
}
if ([string]::IsNullOrWhiteSpace($TestFilter) -or $TestFilter -notmatch '^Unmade(\.[a-zA-Z0-9_]+)*$') {
    throw "INVALID_TEST_FILTER: provide an Unmade.* automation group"
}
$reportRoot = Join-Path $repo "TestReports"
$report = Join-Path $reportRoot ([DateTime]::UtcNow.ToString("yyyyMMdd-HHmmss"))
New-Item -ItemType Directory -Force -Path $report | Out-Null
Write-Host "Running Unreal automation $TestFilter; output $report"
& $editor $project "-unattended" "-nop4" "-NoSplash" "-ExecCmds=Automation RunTests $TestFilter; Quit" "-ReportExportPath=$report"
$editorCode = $LASTEXITCODE
if ($editorCode -ne 0) {
    throw "UNREAL_AUTOMATION_PROCESS_FAILED: exit $editorCode"
}
$index = Join-Path $report "index.json"
if (-not (Test-Path -PathType Leaf $index)) {
    throw "MISSING_AUTOMATION_REPORT: $index"
}
$json = Get-Content -Path $index -Raw | ConvertFrom-Json
if ($null -eq $json.succeeded -or $null -eq $json.failed -or $null -eq $json.notRun) {
    throw "UNRECOGNIZED_AUTOMATION_REPORT_SCHEMA: inspect $index"
}
if ([int]$json.succeeded -lt 1) {
    throw "NO_TESTS_EXECUTED: expected at least one passing automation test"
}
if ([int]$json.failed -gt 0 -or [int]$json.notRun -gt 0) {
    throw "UNREAL_AUTOMATION_TESTS_FAILED: passed=$($json.succeeded) failed=$($json.failed) notRun=$($json.notRun)"
}
Write-Host "PASS: $($json.succeeded) automation test(s), none failed or skipped"
