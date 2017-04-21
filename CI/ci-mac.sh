########################################
# Build daemon cleanup code
########################################
rm -rf libs_required
rm -rf OpenFlipper
rm -rf Type-OpenVolumeMesh



########################################
# Fetch submodules
########################################
git submodule init
git submodule update --remote

########################################
# Show information for easier debugging
########################################
git submodule status
ls OpenFlipper/libs_required/


########################################
# Fetch test data
########################################
rm -rf TestData
git clone git@roosevelt:moebius/OpenFlipper-Test-Data.git TestData

########################################
# Build
########################################

if [ ! -d build-release ]; then
  mkdir build-release
fi

cd build-release

# Build without ports to avoid qt4 collision
/opt/local/bin/cmake ../ -DCMAKE_CXX_FLAGS='-std=c++11' -DQWT6_INCLUDE_DIR=/Users/jenkins/sw/qwt-6.1.2-Qt5-installed/lib/qwt.framework/Headers -DQWT6_LIBRARY=/Users/jenkins/sw/qwt-6.1.2-Qt5-installed/lib/qwt.framework -DQT5_INSTALL_PATH=/Qt/Qt5.5.1/5.5/clang_64  -DGLEW_INCLUDE_DIR=/Users/jenkins/sw/glew-1.10.0-installed/include/ -DGLEW_LIBRARY=/Users/jenkins/sw/glew-1.10.0-installed/lib/libGLEW.dylib 

make

# Required for the tests to build, if a package was generated
make fixbundle

########################################
# Test
########################################

# Run tests
cd tests
bash run_tests.sh

cd ..

########################################
# Doc
########################################

make doc >> ../Doxygen-warning.log 2> ../Doxygen-error.log

########################################
# Package
########################################

rm -f *.dmg 

make OpenFlipper_package

mv *.dmg OpenFlipper-Free-GIT-$CI_BUILD_REF-QT5.5.1-CPP11.dmg


