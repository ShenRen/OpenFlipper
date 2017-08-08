#!/bin/bash

if [ ! -d build-release ]; then
  mkdir build-release
fi

cd build-release

#clean old cmake cache as the path might have changed (there should be no cmakecache)
find . -name "CMakeCache.txt" -type f -delete

cmake -DCMAKE_BUILD_TYPE=Release -DOPENFLIPPER_BUILD_UNIT_TESTS=FALSE ../

make doc >> Doxygen-warning.log 2> Doxygen-error.log

# Copy to webserver
scp -r -P 2222 Build/share/OpenFlipper/Doc/DeveloperHTML/* hudson@web4-info8:OpenFlipper-Free-Daily/Doc/Free/Developer
scp -r -P 2222 Build/share/OpenFlipper/Doc/UserHTML/* hudson@web4-info8:OpenFlipper-Free-Daily/Doc/Free/User
