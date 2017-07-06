@echo off
rem Copy content to output directory
rem =======================================================================================================================

SET CURRENT_DIR=%~dp0
SET CONTENT_DIR=%CURRENT_DIR%\..\..\Content
SET BINARIES_DIR=%1

pushd "%~dp0"
echo "Creating directory %BINARIES_DIR%\data..."
if not exist %BINARIES_DIR%\data mkdir %BINARIES_DIR%\data
echo "Copying content..."
xcopy /Y /F %CONTENT_DIR%\Sprites\*.* %BINARIES_DIR%\data
popd

