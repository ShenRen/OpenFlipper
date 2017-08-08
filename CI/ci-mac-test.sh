########################################
# Fetch test data
########################################
rm -rf TestData
git clone git@roosevelt:moebius/OpenFlipper-Test-Data.git TestData

#expect the build-release folder to be present the program binaries are there
cd build-release

#clean old cmake cache as the path might have changed
find . -name "CMakeCache.txt" -type f -delete

#just to be safe clean the test file definitions too
if [ -f CTestTestfile.cmake ]
then
	rm CTestTestfile.cmake
fi
#just to be safe clean the test file definitions too
if [ -f DartConfiguration.tcl ]
then
	rm DartConfiguration.tcl
fi

# Build without ports to avoid qt4 collision
# run this prior testing to fix paths
/opt/local/bin/cmake ../ -DCMAKE_CXX_FLAGS='-std=c++11' -DQWT6_INCLUDE_DIR=/Users/jenkins/sw/qwt-6.1.2-Qt5-installed/lib/qwt.framework/Headers -DQWT6_LIBRARY=/Users/jenkins/sw/qwt-6.1.2-Qt5-installed/lib/qwt.framework -DQT5_INSTALL_PATH=/Qt/Qt5.5.1/5.5/clang_64  -DGLEW_INCLUDE_DIR=/Users/jenkins/sw/glew-1.10.0-installed/include/ -DGLEW_LIBRARY=/Users/jenkins/sw/glew-1.10.0-installed/lib/libGLEW.dylib 


########################################
# Test
########################################

# Run tests
cd tests
bash run_tests.sh