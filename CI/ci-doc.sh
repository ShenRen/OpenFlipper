#!/bin/bash

if [ ! -d build-release ]; then
  mkdir build-release
fi

cd build-release

cmake -DCMAKE_BUILD_TYPE=Release -DOPENFLIPPER_BUILD_UNIT_TESTS=FALSE ../

make doc >> Doxygen-warning.log 2> Doxygen-error.log

ls -l

# Copy to webserver
scp -r -P 2222 Build/share/OpenFlipper/Doc/DeveloperHTML/* hudson@web4-info8:OpenFlipper-Free-Daily/Doc/Free/Developer
echo "copied the following files to hudson@web4-info8:OpenFlipper-Free-Daily/Doc/Free/Developer :"
ls Build/share/OpenFlipper/Doc/DeveloperHTML/

scp -r -P 2222 Build/share/OpenFlipper/Doc/UserHTML/* hudson@web4-info8:OpenFlipper-Free-Daily/Doc/Free/User
echo "copied the following files to hudson@web4-info8:OpenFlipper-Free-Daily/Doc/Free/User :"
ls Build/share/OpenFlipper/Doc/DeveloperHTML/