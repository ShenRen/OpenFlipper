
parallel(

'VS2015 Qt-5.6.0 x64': { 

  node ('Qt5.6.0 && VS2015 ') {
  
    String name = 'VS2015 Qt-5.6.0';
   
    // Mark the code checkout 'stage'....
    stage('Checkout - ' + name) {

      // Checkout code from repository
      // Configured in jenkins !!
      checkout scm

    }

    // Mark the code build 'stage'....
    stage('Configure - ' + name ) {

    bat '

        mkdir build-release
        cd build-release

        set ARCH_VS= Win64
        set STRING_ARCH=64-Bit 

        set QT_REV=5.6
        set QT_SUFFIX=

        set LIBPATH_BASE=C:\libs\VS2015
        set GTESTVERSION=gtest-1.7.0
        set GENERATOR=Visual Studio 14%ARCH_VS%
        set VS_PATH="C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\IDE\devenv.com"
    
        set QT_BASE_CONFIG=-DQT5_INSTALL_PATH=C:\Qt\%QT_VERSION%-vs2015-%STRING_ARCH%\%QT_REV%\msvc2015_64%QT_SUFFIX% 

        set GLUT_INCLUDE_DIR=C:\libs\VS2015\%ARCHITECTURE%\freeglut-3.0.0\include
        set GLUT_GLUT_LIBRARY=C:\libs\VS2015\%ARCHITECTURE%\freeglut-3.0.0\lib\freeglut.lib 
        set GLEW_INCLUDE_DIR=C:\libs\VS2015\%ARCHITECTURE%\glew-1.13.0\include
        set GLEW_LIBRARY_DIR=C:\libs\VS2015\%ARCHITECTURE%\glew-1.13.0\lib\glew32.lib

        set CMAKE_CONFIGURATION=%QT_BASE_CONFIG% -DGLUT_INCLUDE_DIR="%GLUT_INCLUDE_DIR%" -DGLUT_glut_LIBRARY="%GLUT_GLUT_LIBRARY%" -DGLEW_INCLUDE_DIR="%GLEW_INCLUDE_DIR%" -DGLEW_LIBRARY="%GLEW_LIBRARY_DIR%"

        "C:\Program Files (x86)\CMake\bin\cmake.exe"  -DGTEST_PREFIX="%LIBPATH_BASE%\%ARCHITECTURE%\%GTESTVERSION%" -G "%GENERATOR%"  -DCMAKE_BUILD_TYPE=Release -DOPENFLIPPER_BUILD_UNIT_TESTS=TRUE %CMAKE_CONFIGURATION% ..

        echo "Hello" 
      '
    }

  } 
} ,

'VS2013 Qt-5.6.0': { 

  node ('Qt5.6.0 && VS2013') {


    String name = 'VS2013 Qt-5.6.0';

    // Mark the code checkout 'stage'....
    stage('Checkout - ' + name) {

      // Checkout code from repository
      // Configured in jenkins !!
      checkout scm

    }

    // Mark the code build 'stage'....
    stage('Configure - ' + name) {
      bat "echo \"Hello\" "
    }
  } 

}

)

