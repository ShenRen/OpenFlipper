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

echo "Building with path: build-release-$BUILDPATH"
echo "Full cmake options: $OPTIONS  "

########################################
# Fetch test data
########################################
rm -rf TestData
git clone git@roosevelt:moebius/OpenFlipper-Test-Data.git TestData


#########################################
# Build Release version and Unittests
#########################################

# Make release build folder
if [ ! -d build-release-$BUILDPATH ]; then
  mkdir build-release-$BUILDPATH
fi

cd build-release-$BUILDPATH

cmake -DCMAKE_BUILD_TYPE=Release -DOPENFLIPPER_BUILD_UNIT_TESTS=TRUE -DSTL_VECTOR_CHECKS=ON $OPTIONS ../

#build it
make $MAKE_OPTIONS

# copy the used shared libraries to the lib folder
cd Build

if [ ! -d systemlib ]; then
  mkdir systemlib
fi

ldd bin/OpenFlipper | grep "=> /" | awk '{print $3}' | xargs -I '{}' cp -v '{}' systemlib
cd ../..

#create an artifact directory
if [ ! -d artifacts ]; then
  mkdir artifacts
fi

#cp -R * artifacts
rsync -a --exclude=artifacts --exclude=.git . ./artifacts
cd artifacts
#rm -rf .git

# create an archive with all the build files so we can use them in the test script
#tar -cvf ../buildfiles.tar .
