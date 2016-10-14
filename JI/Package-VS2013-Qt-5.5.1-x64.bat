cd VS2013-5.5-64


set STRING_ARCH=64-Bit
set BUILD_PLATFORM=VS2013
set QT_VERSION=5.5.1
set VS_PATH="C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\IDE\devenv.com"

del *.exe

%VS_PATH% /Build "Release" OpenFlipper.sln /Project "PACKAGE"

move OpenFlipper-*.exe "OpenFlipper-Free-Git-Master-%GIT_COMMIT%-%BUILD_PLATFORM%-%STRING_ARCH%-%QT_VERSION%.exe"

