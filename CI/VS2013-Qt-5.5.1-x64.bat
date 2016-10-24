git submodule init
git submodule update --remote


mkdir rel
cd rel

del *.exe

set ARCHITECTURE=x64
set ARCH_VS= Win64
set STRING_ARCH=64-Bit

set QT_REV=5.5
set QT_SUFFIX=

set LIBPATH_BASE=C:\libs\VS2013
set GTESTVERSION=gtest-1.6.0
set GENERATOR=Visual Studio 12%ARCH_VS%
set VS_PATH="C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\IDE\devenv.com"

set QT_VERSION=Qt5.5.1
set QT_BASE_CONFIG=-DQT5_INSTALL_PATH=C:\Qt\%QT_VERSION%-vs2013-%STRING_ARCH%\%QT_REV%\msvc2013_64%QT_SUFFIX%

set GLUT_INCLUDE_DIR=C:\libs\VS2013\%ARCHITECTURE%\freeglut-2.8.1\include
set GLUT_GLUT_LIBRARY=C:\libs\VS2013\%ARCHITECTURE%\freeglut-2.8.1\lib\freeglut.lib
set GLEW_INCLUDE_DIR=C:\libs\VS2013\%ARCHITECTURE%\glew-1.10.0\include
set GLEW_LIBRARY_DIR=C:\libs\VS2013\%ARCHITECTURE%\glew-1.10.0\lib\glew32.lib

set CMAKE_CONFIGURATION=%QT_BASE_CONFIG% -DGLUT_INCLUDE_DIR="%GLUT_INCLUDE_DIR%" -DGLUT_glut_LIBRARY="%GLUT_GLUT_LIBRARY%" -DGLEW_INCLUDE_DIR="%GLEW_INCLUDE_DIR%" -DGLEW_LIBRARY="%GLEW_LIBRARY_DIR%"

"C:\Program Files (x86)\CMake\bin\cmake.exe"  -DGTEST_PREFIX="%LIBPATH_BASE%\%ARCHITECTURE%\%GTESTVERSION%" -G "%GENERATOR%"  -DCMAKE_BUILD_TYPE=Release -DOPENFLIPPER_BUILD_UNIT_TESTS=TRUE %CMAKE_CONFIGURATION% ..

IF %errorlevel% NEQ 0 exit /b %errorlevel%

%VS_PATH% /Build "Release" OpenFlipper.sln /Project "ALL_BUILD"

IF %errorlevel% NEQ 0 exit /b %errorlevel%


set BUILD_PLATFORM=VS2013

del *.exe

%VS_PATH% /Build "Release" OpenFlipper.sln /Project "PACKAGE"

IF %errorlevel% NEQ 0 exit /b %errorlevel%

move OpenFlipper-*.exe "OpenFlipper-Free-Git-Master-%CI_BUILD_REF%-%BUILD_PLATFORM%-%STRING_ARCH%-%QT_VERSION%.exe"

cd tests
copy ..\Build\Qt*.dll testBinaries
copy ..\Build\icu*.dll testBinaries
run_tests.bat

IF %errorlevel% NEQ 0 exit /b %errorlevel%










