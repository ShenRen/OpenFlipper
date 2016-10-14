cd rel


set STRING_ARCH=32-Bit
set BUILD_PLATFORM=VS2015
set QT_VERSION=5.6.0
set VS_PATH="C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\IDE\devenv.com"

del *.exe

%VS_PATH% /Build "Release" OpenFlipper.sln /Project "PACKAGE"

move OpenFlipper-*.exe "OpenFlipper-Free-Git-Master-%GIT_COMMIT%-%BUILD_PLATFORM%-%STRING_ARCH%-%QT_VERSION%.exe"

