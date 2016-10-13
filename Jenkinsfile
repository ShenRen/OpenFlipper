

   
// Mark the code checkout 'stage'....
stage('Checkout') {


  parallel (

  a: { node ('Qt5.6.0&&VS2013') {

  // Checkout code from repository
  // Configured in jenkins !!
  checkout scm
  bat "echo \"Hello\" "

  }},

  b: { node ('Qt5.6.0&&VS2015') {

  // Checkout code from repository
  // Configured in jenkins !!
  checkout scm

  bat "echo \"Hello\" "

  }}


  )

}

// Mark the code checkout 'stage'....
stage('Configure') {


  parallel (

  a: { node ('Qt5.6.0&&VS2013') {

  bat "echo \"Hello\" "

  }},

  b: { node ('Qt5.6.0&&VS2015') {

  bat "echo \"Hello\" "

  }}


  )

}

