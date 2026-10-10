@echo off
setlocal
cd /d "%~dp0"
echo.
echo ===============================================
echo        THE UNMADE - FIRST PC QUICKSTART
echo ===============================================
echo.
echo Checks local Unreal Engine 5.8 and prerequisites,
echo builds and tests the project, imports reference
echo tables and creates a separate safe staging map.
echo No software or paid services will be installed.
echo.
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0Scripts\first_pc_quickstart.ps1" %*
set "RESULT=%ERRORLEVEL%"
echo.
if not "%RESULT%"=="0" (
  echo STOPPED: open TestReportsquickstart-* to see the failure.
  echo No claim is made that this game was compiled or tested.
) else (
  echo SUCCESS: Unreal Editor is opening if installed and verified.
)
echo.
pause
exit /b %RESULT%
