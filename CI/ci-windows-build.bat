@echo off
::########################################
::# Fetch test data
::########################################
rmdir /Q /S TestData
:: clone libraries git (set env variable to GIT_SSH_COMMAND maybe use setx once as this key won't change) 
set GIT_SSH_COMMAND=ssh -i E:\\\gitlab\\\id_rsa 
git clone git@roosevelt.informatik.rwth-aachen.de:moebius/OpenFlipper-Test-Data.git TestData

::load configuration and settings
call %~dp0\ci-windows-config.bat

mkdir rel
cd rel

::clear any old binary files
del *.exe

::invoke cmake
"C:\Program Files (x86)\CMake\bin\cmake.exe"  -DGTEST_PREFIX="%LIBPATH_BASE%\%ARCHITECTURE%\%GTESTVERSION%" -G "%GENERATOR%"  -DCMAKE_BUILD_TYPE=Release -DOPENFLIPPER_BUILD_UNIT_TESTS=TRUE %CMAKE_CONFIGURATION% ..

IF %errorlevel% NEQ 0 exit /b %errorlevel%

:: build Open-Flipper

%VS_PATH% /Build "Release" OpenFlipper.sln /Project "ALL_BUILD"

IF %errorlevel% NEQ 0 exit /b %errorlevel%

:: copy all files to a new artifacts folder except the .git folder
robocopy . artifacts /e /xd artifacts /xd .git