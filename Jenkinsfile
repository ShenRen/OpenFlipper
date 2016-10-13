
parallel(

'VS2015 Qt-5.6.0 x64': { 

  node ('Qt5.6.0 && VS2015 ') {
  
    String VisualStudio = 'VS2015';
    String QtVersion    = 'Qt-5.6.0';
    String Arch         = 'x64';
    String name      = VisualStudio + ' ' + QtVersion + ' ' + Arch;
    String stageName = VisualStudio + '-' + QtVersion + '-' + Arch; 
   
    stage('Checkout - ' + name) {

      // Configured in jenkins !!
      checkout scm

    }


    stage('Configure - ' + name ) {
      bat 'JI/Configure-'+stageName+'.bat'
    }

    stage('Build - ' + name ) {
      bat 'JI/Build-'+stageName+'.bat'
    }

    stage('Test - ' + name ) {
      bat 'JI/Test.bat'
    }

    stage('Package - ' + name ) {
      bat 'JI/Package-'+stageName+'.bat'
    }
    
    archiveArtifacts artifacts: '**/release/*.exe', fingerprint: true

  } 
} ,

'VS2015 Qt-5.6.0 x32': { 

  node ('Qt5.6.0 && VS2015') {

    String VisualStudio = 'VS2015';
    String QtVersion    = 'Qt-5.6.0';
    String Arch         = 'x32';
    String name      = VisualStudio + ' ' + QtVersion + ' ' + Arch;
    String stageName = VisualStudio + '-' + QtVersion + '-' + Arch;

    stage('Checkout - ' + name) {

      // Configured in jenkins !!
      checkout scm

    }


    stage('Configure - ' + name ) {
      bat 'JI/Configure-'+stageName+'.bat'
    }

    stage('Build - ' + name ) {
      bat 'JI/Build-'+stageName+'.bat'
    }

    stage('Test - ' + name ) {
      bat 'JI/Test.bat'
    }

    stage('Package - ' + name ) {
      bat 'JI/Package-'+stageName+'.bat'
    }

    archiveArtifacts artifacts: '**/release/*.exe', fingerprint: true
  } 

} ,

'VS2013 Qt-5.5.1 x64': {

  node ('Qt5.5.1 && VS2013 ') {

    String VisualStudio = 'VS2013';
    String QtVersion    = 'Qt-5.5.1';
    String Arch         = 'x64';
    String name      = VisualStudio + ' ' + QtVersion + ' ' + Arch;
    String stageName = VisualStudio + '-' + QtVersion + '-' + Arch;

    stage('Checkout - ' + name) {

      // Configured in jenkins !!
      checkout scm

    }


    stage('Configure - ' + name ) {
      bat 'JI/Configure-'+stageName+'.bat'
    }

    stage('Build - ' + name ) {
      bat 'JI/Build-'+stageName+'.bat'
    }

    stage('Test - ' + name ) {
      bat 'JI/Test.bat'
    }

    stage('Package - ' + name ) {
      bat 'JI/Package-'+stageName+'.bat'
    }
    
    archiveArtifacts artifacts: '**/release/*.exe', fingerprint: true

  }
}

)

