
parallel(

VS2015-qt-56: { node ('Qt5.6.0&&VS2015') {
   
   
   // Mark the code checkout 'stage'....
   stage('Checkout') {

     // Checkout code from repository
     // Configured in jenkins !!
     checkout scm

   }

   // Mark the code build 'stage'....
   stage('Build') {
     bat "echo \"Hello\" "
   }
} } ,


VS2013-qt-56: { node ('Qt5.6.0&&VS2013') {


   // Mark the code checkout 'stage'....
   stage('Checkout') {

     // Checkout code from repository
     // Configured in jenkins !!
     checkout scm

   }

   // Mark the code build 'stage'....
   stage('Build') {
     bat "echo \"Hello\" "
   }
} }

)

