@echo off

echo "Generating Visual Studio 2013 Workspace..."
echo "To run this batch script, you must have installed 64bit Visual Studio 2013"

SETLOCAL ENABLEEXTENSIONS

set output=guidance_examples
set parent=%~dp0
pushd "%parent%"
pushd ..\..
set project_root=%CD%
popd
popd

set cmake=%project_root%\tools\cmake\3.3.0\win64\bin\cmake.exe
if not exist "%cmake%" set cmake=CMAKE && echo "Using system CMake"

mkdir "%parent%%output%" >NUL 2>&1
pushd "%parent%%output%"
"%cmake%" -G"Visual Studio 12 2013 Win64" "%project_root%\guidance-sdk\examples"
if /I "%ERRORLEVEL%" NEQ "0" echo "generate_guidance_examples.cmd failed, exiting..." && popd && exit /B 1
popd
