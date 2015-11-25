#!/bin/bash

# Script abort on error
set -e

COMPILER=$1
LANGUAGE=$2
QTVERSION=$3

OPTIONS=""

BUILDPATH=""

if [ "$COMPILER" == "gcc" ]; then
  echo "Building with GCC";
  BUILDPATH="gcc"
elif [ "$COMPILER" == "clang" ]; then

  OPTIONS="$OPTIONS -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang -DGTEST_PREFIX=~/sw/gtest-1.7.0-clang/ "
  BUILDPATH="clang"
  echo "Building with CLANG";
fi  

if [ "$LANGUAGE" == "C++98" ]; then
  echo "Building with C++98";
  BUILDPATH="$BUILDPATH-cpp98"
elif [ "$LANGUAGE" == "C++11" ]; then
  echo "Building with C++11";
  OPTIONS="$OPTIONS -DCMAKE_CXX_FLAGS='-std=c++11' "
  BUILDPATH="$BUILDPATH-cpp11"
fi  

if [ "QTVERSION" == "QT4" ]; then
  echo "Building with QT4";
  OPTIONS="$OPTIONS -DFORCE_QT4=TRUE "
  BUILDPATH="$BUILDPATH-qt4"
elif [ "QTVERSTION" == "QT5" ]; then
  echo "Building with QT5";
  BUILDPATH="$BUILDPATH-qt5"
  OPTIONS="$OPTIONS -DFORCE_QT4=FALSE -DQWT6_INCLUDE_DIR=~/sw/qwt-6.1/include -DQWT6_LIBRARY_DIR=~/sw/qwt-6.1/lib -DQWT6_LIBRARY=~/sw/qwt-6.1/lib/libqwt.so -DQT5_INSTALL_PATH=/ACG/acgdev/gcc-4.0-x86_64/qt-5.5.0/5.5/gcc_64"
fi

echo "Building with path: build-release-$BUILDPATH"


########################################
# Fetch submodules
########################################
git submodule init
git submodule update --remote


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
  mkdir build-release-$BUILDPATH
fi

cd build-release-$BUILDPATH

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

