
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
      
      bat 'JI/Configure-VS2015-Qt-5.6.0-x64.bat'

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

