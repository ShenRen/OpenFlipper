#!/bin/bash

OPTIONS=""
MAKE_OPTIONS=""
BUILDPATH=""

if [ "$COMPILER" == "gcc" ]; then
  echo "Setting Compiler to GCC";
  BUILDPATH="gcc"

  # without icecc: no options required
  OPTIONS="-DCMAKE_CXX_COMPILER=/usr/lib/icecc/bin/g++ -DCMAKE_C_COMPILER=/usr/lib/icecc/bin/gcc"
  MAKE_OPTIONS="-j16"
  export ICECC_CXX=/usr/bin/g++ ; export ICECC_CC=/usr/bin/gcc

elif [ "$COMPILER" == "clang" ]; then

  OPTIONS="$OPTIONS -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang -DGTEST_PREFIX=~/sw/gtest-1.8.0-clang/ "

#  Build options with icecc /not working due to strange symbol errors
#  OPTIONS="$OPTIONS -DCMAKE_CXX_COMPILER=/usr/lib/icecc/bin/g++ -DCMAKE_C_COMPILER=/usr/lib/icecc/bin/gcc -DGTEST_PREFIX=~/sw/gtest-1.7.0-clang/ "
#  export ICECC_CXX=/usr/bin/clang++ ; export ICECC_CC=/usr/bin/clang

  BUILDPATH="clang"
  MAKE_OPTIONS="-j6"

  echo "Setting compiler to CLANG";
fi  

if [ "$LANGUAGE" == "C++98" ]; then
  echo "Using C++98 standard";
  BUILDPATH="$BUILDPATH-cpp98"
elif [ "$LANGUAGE" == "C++11" ]; then
  echo "Using C++11 standard";
  OPTIONS="$OPTIONS -DCMAKE_CXX_FLAGS='-std=c++11' "
  BUILDPATH="$BUILDPATH-cpp11"
fi  

if [ "$QTVERSION" == "QT4" ]; then
  echo "Using QT4";
  OPTIONS="$OPTIONS -DFORCE_QT4=TRUE "
  BUILDPATH="$BUILDPATH-qt4"
elif [ "$QTVERSION" == "QT5" ]; then
  echo "Using QT5";
  BUILDPATH="$BUILDPATH-qt5"
  OPTIONS="$OPTIONS -DFORCE_QT4=FALSE -DQWT6_INCLUDE_DIR=~/sw/qwt-6.1.2-qt5/include -DQWT6_LIBRARY_DIR=~/sw/qwt-6.1.2-qt5/lib -DQWT6_LIBRARY=~/sw/qwt-6.1.2-qt5/lib/libqwt-qt5.so -DQT5_INSTALL_PATH=~/sw/qt-5.5.1/5.5/gcc_64"
  export LD_LIBRARY_PATH=~/sw/qt-5.5.1/5.5/gcc_64/lib:$LD_LIBRARY_PATH
elif [ "$QTVERSION" == "QT5.9.0" ]; then
  echo "Using QT5.9.0";
  BUILDPATH="$BUILDPATH-qt5.9.0"
  OPTIONS="$OPTIONS -DFORCE_QT4=FALSE -DQWT6_INCLUDE_DIR=~/sw/qwt-6.1.3-qt5.9.0/include -DQWT6_LIBRARY_DIR=~/sw/qwt-6.1.3-qt5.9.0/lib -DQWT6_LIBRARY=~/sw/qwt-6.1.3-qt5.9.0/lib/libqwt.so -DQT5_INSTALL_PATH=~/sw/Qt/5.9/gcc_64"
fi