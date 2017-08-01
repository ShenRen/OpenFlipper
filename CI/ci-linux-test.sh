#!/bin/bash

# Script abort on error
set -e

# Expected Settings via environment variables:
# COMPILER= gcc/clang
# LANGUAGE= C++98 / C++11
# QTVERSION= QT4/QT5


#include ci options script
MY_DIR=$(dirname $(readlink -f $0))
source $MY_DIR/ci-linux-config.sh

########################################
# Fetch test data
########################################
rm -rf TestData
git clone git@roosevelt:moebius/OpenFlipper-Test-Data.git TestData

#########################################
# Run Release Unittests
#########################################

# Run tests
cd build-release-$BUILDPATH

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

cmake -DCMAKE_BUILD_TYPE=Release -DOPENFLIPPER_BUILD_UNIT_TESTS=TRUE -DSTL_VECTOR_CHECKS=ON $OPTIONS ../

#tell the location to the libs from build jobs
export LD_LIBRARY_PATH=$(pwd)/Build/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$(pwd)/Build/systemlib:$LD_LIBRARY_PATH

#make test

cd tests
bash run_tests.sh

cd ..