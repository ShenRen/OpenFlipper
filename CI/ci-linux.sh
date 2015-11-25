#!/bin/bash

COMPILER=$1
LANGUAGE=$2
QTVERSION=$3

OPTIONS=""

if [ "$COMPILER" == "gcc" ]; then
  echo "Building with GCC";
elif [ "$COMPILER" == "clang" ]; then

  OPTIONS="$OPTIONS -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang -DGTEST_PREFIX=~/sw/gtest-1.7.0-clang/ "
  echo "Building with CLANG";
fi  

if [ "$LANGUAGE" == "C++98" ]; then
  echo "Building with C++98";
elif [ "$LANGUAGE" == "C++11" ]; then
  echo "Building with C++11";
  OPTIONS="$OPTIONS -DCMAKE_CXX_FLAGS='-std=c++11' "
fi  

if [ "QTVERSION" == "QT4" ]; then
  echo "Building with QT4";
  OPTIONS="$OPTIONS -DFORCE_QT4=TRUE "
elif [ "QTVERSTION" == "QT5" ]; then
  echo "Building with QT5";
  OPTIONS="$OPTIONS -DFORCE_QT4=FALSE -DQWT6_INCLUDE_DIR=~/sw/qwt-6.1/include -DQWT6_LIBRARY_DIR=~/sw/qwt-6.1/lib -DQWT6_LIBRARY=~/sw/qwt-6.1/lib/libqwt.so -DQT5_INSTALL_PATH=/usr/lib/x86_64-linux-gnu/cmake/Qt5"
fi


########################################
# Fetch test data
########################################
rm -rf TestData
git clone git@roosevelt:moebius/OpenFlipper-Test-Data.git TestData


#########################################
# Build Release version and Unittests
#########################################

# Make release build folder
if [ ! -d build-release ]; then
  mkdir build-release
fi

cd build-release

cmake -DCMAKE_BUILD_TYPE=Release -DOPENFLIPPER_BUILD_UNIT_TESTS=TRUE -DSTL_VECTOR_CHECKS=ON $OPTIONS ../

#build it
make

#########################################
# Run Release Unittests
#########################################

# Run tests
cd tests
bash run_tests.sh

cd ..

