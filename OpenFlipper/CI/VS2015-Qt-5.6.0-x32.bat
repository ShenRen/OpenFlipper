rmdir libs_required\OpenMesh /s /q


git submodule init
git submodule update --remote


mkdir rel
cd rel

del *.exe

set ARCHITECTURE=x32
set ARCH_VS=
set STRING_ARCH=32-Bit

set QT_REV=5.6
set QT_SUFFIX=

set LIBPATH_BASE=C:\libs\VS2015
set GTESTVERSION=gtest-1.7.0
set GENERATOR=Visual Studio 14%ARCH_VS%
set VS_PATH="C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\IDE\devenv.com"

set QT_VERSION=Qt5.6.0
set QT_BASE_CONFIG=-DQT5_INSTALL_PATH=C:\Qt\%QT_VERSION%-vs2015-%STRING_ARCH%\%QT_REV%\msvc2015%QT_SUFFIX%

set GLUT_INCLUDE_DIR=C:\libs\VS2015\%ARCHITECTURE%\freeglut-3.0.0\include
set GLUT_GLUT_LIBRARY=C:\libs\VS2015\%ARCHITECTURE%\freeglut-3.0.0\lib\freeglut.lib
set GLEW_INCLUDE_DIR=C:\libs\VS2015\%ARCHITECTURE%\glew-1.13.0\include
set GLEW_LIBRARY_DIR=C:\libs\VS2015\%ARCHITECTURE%\glew-1.13.0\lib\glew32.lib

set CMAKE_CONFIGURATION=%QT_BASE_CONFIG% -DGLUT_INCLUDE_DIR="%GLUT_INCLUDE_DIR%" -DGLUT_glut_LIBRARY="%GLUT_GLUT_LIBRARY%" -DGLEW_INCLUDE_DIR="%GLEW_INCLUDE_DIR%" -DGLEW_LIBRARY="%GLEW_LIBRARY_DIR%"

"C:\Program Files (x86)\CMake\bin\cmake.exe"  -DGTEST_PREFIX="%LIBPATH_BASE%\%ARCHITECTURE%\%GTESTVERSION%" -G "%GENERATOR%"  -DCMAKE_BUILD_TYPE=Release -DOPENFLIPPER_BUILD_UNIT_TESTS=TRUE %CMAKE_CONFIGURATION% ..

IF %errorlevel% NEQ 0 exit /b %errorlevel%

%VS_PATH% /Build "Release" OpenFlipper.sln /Project "ALL_BUILD"

IF %errorlevel% NEQ 0 exit /b %errorlevel%


set BUILD_PLATFORM=VS2015

del *.exe

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










