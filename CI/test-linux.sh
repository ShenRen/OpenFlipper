if [ "$QTVERSION" == "QT4" ]; then
  echo "Testing with QT4";
  OPTIONS="$OPTIONS -DFORCE_QT4=TRUE "
  BUILDPATH="$BUILDPATH-qt4"
elif [ "$QTVERSION" == "QT5" ]; then
  echo "Testing with QT5";
  BUILDPATH="$BUILDPATH-qt5"
  OPTIONS="$OPTIONS -DFORCE_QT4=FALSE -DQWT6_INCLUDE_DIR=~/sw/qwt-6.1.2-qt5/include -DQWT6_LIBRARY_DIR=~/sw/qwt-6.1.2-qt5/lib -DQWT6_LIBRARY=~/sw/qwt-6.1.2-qt5/lib/libqwt-qt5.so -DQT5_INSTALL_PATH=~/sw/qt-5.5.1/5.5/gcc_64"
  export LD_LIBRARY_PATH=~/sw/qt-5.5.1/5.5/gcc_64/lib
elif [ "$QTVERSION" == "QT5.9.0" ]; then
  echo "Testing with QT5.9.0";
  BUILDPATH="$BUILDPATH-qt5.9.0"
  OPTIONS="$OPTIONS -DFORCE_QT4=FALSE -DQWT6_INCLUDE_DIR=~/sw/qwt-6.1.3-qt5.9.0/include -DQWT6_LIBRARY_DIR=~/sw/qwt-6.1.3-qt5.9.0/lib -DQWT6_LIBRARY=~/sw/qwt-6.1.3-qt5.9.0/lib/libqwt.so -DQT5_INSTALL_PATH=~/sw/Qt/5.9/gcc_64"
  export LD_LIBRARY_PATH=~/sw/Qt/5.9/gcc_64/lib
fi

#########################################
# Run Release Unittests
#########################################

# Run tests
cd tests
bash run_tests.sh

cd ..