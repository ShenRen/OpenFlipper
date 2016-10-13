
parallel(

'VS2015 Qt-5.6.0 x64': { 

  node ('Qt5.6.0 && VS2015 ') {
  
    String VisualStudio = 'VS2015';
    String QtVersion    = 'Qt-5.6.0';
    String Arch         = 'x64';
    String name      = VisualStudio + ' ' + QtVersion + ' ' + Arch;
    String stageName = VisualStudio + '-' + QtVersion + '-' + Arch; 
   
    stage('Checkout - ' + name) {

      // Checkout code from repository
      // Configured in jenkins !!
      checkout scm

    }


    stage('Configure - ' + name ) {
      bat 'JI/Configure-$stageName.bat'
    }

    stage('Build - ' + name ) {
      bat 'JI/Build-$stageName.bat'
    }

    stage('Test - ' + name ) {
      bat 'JI/Test-$stageName.bat'
    }

    stage('Package - ' + name ) {
      bat 'JI/Package-$stageName.bat'
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

