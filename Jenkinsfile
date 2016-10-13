
parallel(

a: { node ('Qt5.6.0&&VS2015') {
   
   
   // Mark the code checkout 'stage'....
   stage('Checkout') {


     parallel (

     a: { node ('Qt5.6.0&&VS2013') {

     // Checkout code from repository
     // Configured in jenkins !!
     checkout scm

     }},
  
     b: { node ('Qt5.6.0&&VS2015') {

     // Checkout code from repository
     // Configured in jenkins !!
     checkout scm

     }}


     )

   }


   // Mark the code build 'stage'....
   stage('Build') {
     bat "echo \"Hello\" "
   }


} 

