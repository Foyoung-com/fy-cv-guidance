@echo off
SETLOCAL ENABLEEXTENSIONS

set output=fy_guidance
set parent=%~dp0
pushd "%parent%\..\.."
set project_root=%CD%
popd

set flavor=Release

if /I "%1" == "Debug" set flavor=Debug
if /I "%1" == "All" set flavor2=Debug


REM generate the VC workspace
call "%parent%generate.cmd"
if /I "%ERRORLEVEL%" NEQ "0" goto :error

set cmake=%project_root%\tools\cmake\3.3.0\win64\bin\cmake.exe
if not exist "%cmake%" set cmake=CMAKE && echo "Using system CMake"

"%cmake%" --build "%parent%%output%" --config "%flavor%" -- /m
if /I "%ERRORLEVEL%" NEQ "0" goto :error
if defined flavor2 "%cmake%" --build "%parent%%output%" --config "%flavor2%" -- /m
if /I "%ERRORLEVEL%" NEQ "0" goto :error

goto :end

:error
echo "build.cmd error, exiting..."
exit /B 1
goto :end

:end
