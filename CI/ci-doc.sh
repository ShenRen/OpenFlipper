#!/bin/bash

if [ ! -d build-release ]; then
  mkdir build-release
fi

cd build-release

cmake -DCMAKE_BUILD_TYPE=Release -DOPENFLIPPER_BUILD_UNIT_TESTS=FALSE ../

make doc >> Doxygen-warning.log 2> Doxygen-error.log

ls -l

# Copy to webserver
scp -r -P 2222 build-release/Build/share/OpenFlipper/Doc/DeveloperHTML/* hudson@web4-info8:OpenFlipper-Free-Daily/Doc/Free/Developer
scp -r -P 2222 build-release/Build/share/OpenFlipper/Doc/UserHTML/* hudson@web4-info8:OpenFlipper-Free-Daily/Doc/Free/User
