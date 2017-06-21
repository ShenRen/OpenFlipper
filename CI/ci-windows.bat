@echo off
::########################################
::# Fetch test data
::########################################
rmdir /Q /S TestData
:: clone libraries git (set env variable to GIT_SSH_COMMAND maybe use setx once as this key won't change) 
set GIT_SSH_COMMAND=ssh -i E:\\\gitlab\\\id_rsa 
git clone git@roosevelt.informatik.rwth-aachen.de:moebius/OpenFlipper-Test-Data.git TestData

mkdir rel
cd rel

del *.exe

:: use 4 threads for parallel compilation of the project
set CL=/MP4

:: determine VS version and set variables
if "%COMPILER%" == "VS2012" (
set QT_COMPILERPREFIX=msvc2012
set VS_COMPILERVERSION_LONG=11.0
set VS_COMPILERVERSION_SHORT=11
set VS_EDITION_YEAR=2012
)
if "%COMPILER%" == "VS2013" (
set QT_COMPILERPREFIX=msvc2013
set VS_COMPILERVERSION_LONG=12.0
set VS_COMPILERVERSION_SHORT=12
set VS_EDITION_YEAR=2013
)
if "%COMPILER%" == "VS2015" (
set QT_COMPILERPREFIX=msvc2015
set VS_COMPILERVERSION_LONG=14.0
set VS_COMPILERVERSION_SHORT=14
set VS_EDITION_YEAR=2015
)

set BUILD_PLATFORM=%COMPILER%

:: determine architecture and set variables
if "%ARCHITECTURE%" == "x64" (
set ARCHBITS=_64
set ARCH_VS= Win64
set STRING_ARCH=64-Bit
) else (
set ARCHBITS=
set ARCH_VS=
set STRING_ARCH=32-Bit
)

set GENERATOR=Visual Studio %VS_COMPILERVERSION_SHORT% %VS_EDITION_YEAR%%ARCH_VS%
set VS_PATH="C:\Program Files (x86)\Microsoft Visual Studio %VS_COMPILERVERSION_LONG%\Common7\IDE\devenv.com"

:: determine Qt Version and set variables
if "%QT_VERSION%" == "Qt5.5.1" (
set QT_REV_LONG=5.5.1
set QT_REV=5.5
)
if "%QT_VERSION%" == "Qt5.6.2" (
set QT_REV_LONG=5.6.2
set QT_REV=5.6
)
if "%QT_VERSION%" == "Qt5.7.0" (
set QT_REV_LONG=5.7.0
set QT_REV=5.7
)
if "%QT_VERSION%" == "Qt5.8.0" (
set QT_REV_LONG=5.8.0
set QT_REV=5.8
)
if "%QT_VERSION%" == "Qt5.9.0" (
set QT_REV_LONG=5.9.0
set QT_REV=5.9
)

set QT_SUFFIX=
set QT_BASE_CONFIG=-DQT5_INSTALL_PATH=E:\Qt\%QT_VERSION%\%QT_REV%\%QT_COMPILERPREFIX%%ARCHBITS%%QT_SUFFIX%

:: set up Libraty Paths
set LIBPATH_BASE=E:\libs\%COMPILER%

:: freeglut assume we use freeglut 3.0.0 if not try 2.8.1 if not sanitize the libs folder
if exist %LIBPATH_BASE%\%ARCHITECTURE%\freeglut-3.0.0\include (
   set GLUT_INCLUDE_DIR=%LIBPATH_BASE%\%ARCHITECTURE%\freeglut-3.0.0\include
   set GLUT_GLUT_LIBRARY=%LIBPATH_BASE%\%ARCHITECTURE%\freeglut-3.0.0\lib\freeglut.lib
) else (
   if exist %LIBPATH_BASE%\%ARCHITECTURE%\freeglut-2.8.1\include (
      set GLUT_INCLUDE_DIR=%LIBPATH_BASE%\%ARCHITECTURE%\freeglut-2.8.1\include
      set GLUT_GLUT_LIBRARY=%LIBPATH_BASE%\%ARCHITECTURE%\freeglut-2.8.1\lib\freeglut.lib
   ) else (
      echo "Error: No suitable version of freeglut found!"
	  exit
   )
)

:: check for gtest version 1.6 or 1.7 use the highest found version
for /l %%x in (6, 1, 7) do (
   if exist %LIBPATH_BASE%\%ARCHITECTURE%\gtest-1.%%x.0 (
      set GTESTVERSION=gtest-1.%%x.0
   )
)

:: check for glew version 6.1.1 to 6.1.3 use the highest found version
for /l %%x in (0, 1, 3) do (
   if exist %LIBPATH_BASE%\%ARCHITECTURE%\glew-1.1%%x.0\include (
      set GLEW_INCLUDE_DIR=%LIBPATH_BASE%\%ARCHITECTURE%\glew-1.1%%x.0\include
      set GLEW_LIBRARY_DIR=%LIBPATH_BASE%\%ARCHITECTURE%\glew-1.1%%x.0\lib\glew32.lib
   )
)

:: check for qwt version 6.1.1 to 6.1.3 use the highest found version
for /l %%x in (0, 1, 3) do (
   if exist %LIBPATH_BASE%\%ARCHITECTURE%\qwt-6.1.%%x-qt%QT_REV_LONG%\include (
      set QWT6_INCLUDE_DIR=%LIBPATH_BASE%\%ARCHITECTURE%\qwt-6.1.%%x-qt%QT_REV_LONG%\include
      set QWT6_LIBRARY=%LIBPATH_BASE%\%ARCHITECTURE%\qwt-6.1.%%x-qt%QT_REV_LONG%\lib\qwt.lib
      set QWT6_LIBRARY_DIR=%LIBPATH_BASE%\%ARCHITECTURE%\qwt-6.1.%%x-qt%QT_REV_LONG%\lib
   )
:: unfortunately qwt is not named consistently in our repos so we also have to check for a different folder named
   if exist %LIBPATH_BASE%\%ARCHITECTURE%\qwt-6.1.%%x-qt-%QT_REV_LONG%\include (
      set QWT6_INCLUDE_DIR=%LIBPATH_BASE%\%ARCHITECTURE%\qwt-6.1.%%x-qt-%QT_REV_LONG%\include
      set QWT6_LIBRARY=%LIBPATH_BASE%\%ARCHITECTURE%\qwt-6.1.%%x-qt-%QT_REV_LONG%\lib\qwt.lib
      set QWT6_LIBRARY_DIR=%LIBPATH_BASE%\%ARCHITECTURE%\qwt-6.1.%%x-qt-%QT_REV_LONG%\lib
   )
)


set CMAKE_CONFIGURATION=%QT_BASE_CONFIG% -DGLUT_INCLUDE_DIR="%GLUT_INCLUDE_DIR%" -DGLUT_glut_LIBRARY="%GLUT_GLUT_LIBRARY%" -DGLEW_INCLUDE_DIR="%GLEW_INCLUDE_DIR%" -DGLEW_LIBRARY="%GLEW_LIBRARY_DIR%" -DQWT6_INCLUDE_DIR=%QWT6_INCLUDE_DIR% -DQWT6_LIBRARY=%QWT6_LIBRARY% -DQWT6_LIBRARY_DIR=%QWT6_LIBRARY_DIR%

"C:\Program Files (x86)\CMake\bin\cmake.exe"  -DGTEST_PREFIX="%LIBPATH_BASE%\%ARCHITECTURE%\%GTESTVERSION%" -G "%GENERATOR%"  -DCMAKE_BUILD_TYPE=Release -DOPENFLIPPER_BUILD_UNIT_TESTS=TRUE %CMAKE_CONFIGURATION% ..

IF %errorlevel% NEQ 0 exit /b %errorlevel%

:: build Open-Flipper

%VS_PATH% /Build "Release" OpenFlipper.sln /Project "ALL_BUILD"

IF %errorlevel% NEQ 0 exit /b %errorlevel%

del *.exe

:: collect dlls and copy them to Openflipper dir

"C:\Program Files (x86)\CMake\bin\cmake.exe"  -DGTEST_PREFIX="%LIBPATH_BASE%\%ARCHITECTURE%\%GTESTVERSION%" -G "%GENERATOR%"  -DCMAKE_BUILD_TYPE=Release -DOPENFLIPPER_BUILD_UNIT_TESTS=TRUE %CMAKE_CONFIGURATION% ..

%VS_PATH% /Build "Release" OpenFlipper.sln /Project "PACKAGE"

IF %errorlevel% NEQ 0 exit /b %errorlevel%

move OpenFlipper-*.exe "OpenFlipper-Free-Git-Master-%CI_BUILD_REF%-%BUILD_PLATFORM%-%STRING_ARCH%-%QT_VERSION%.exe"

cd tests
copy ..\Build\Qt*.dll testBinaries
copy ..\Build\icu*.dll testBinaries
run_tests.bat

IF %errorlevel% NEQ 0 exit /b %errorlevel%

cd ..

cd Build

dir