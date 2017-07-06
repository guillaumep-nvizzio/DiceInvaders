@echo off
rem Copy libraries to output directory
rem =======================================================================================================================

SET CURRENT_DIR=%~dp0
SET LIBRARY_DIR=%CURRENT_DIR%\..\..\External\Dice
SET BINARIES_DIR=%1

pushd "%~dp0"
echo "Copying libraries..."
xcopy /Y /F %LIBRARY_DIR%\*.dll %BINARIES_DIR%
popd

