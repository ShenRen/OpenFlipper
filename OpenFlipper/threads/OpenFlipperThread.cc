/*===========================================================================*\
*                                                                            *
*                              OpenFlipper                                   *
 *           Copyright (c) 2001-2015, RWTH-Aachen University                 *
 *           Department of Computer Graphics and Multimedia                  *
 *                          All rights reserved.                             *
 *                            www.openflipper.org                            *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * This file is part of OpenFlipper.                                         *
 *---------------------------------------------------------------------------*
 *                                                                           *
 * Redistribution and use in source and binary forms, with or without        *
 * modification, are permitted provided that the following conditions        *
 * are met:                                                                  *
 *                                                                           *
 * 1. Redistributions of source code must retain the above copyright notice, *
 *    this list of conditions and the following disclaimer.                  *
 *                                                                           *
 * 2. Redistributions in binary form must reproduce the above copyright      *
 *    notice, this list of conditions and the following disclaimer in the    *
 *    documentation and/or other materials provided with the distribution.   *
 *                                                                           *
 * 3. Neither the name of the copyright holder nor the names of its          *
 *    contributors may be used to endorse or promote products derived from   *
 *    this software without specific prior written permission.               *
 *                                                                           *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       *
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED *
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A           *
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER *
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,  *
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,       *
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR        *
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF    *
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING      *
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS        *
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.              *
*                                                                            *
\*===========================================================================*/

/*===========================================================================*\
*                                                                            *
*   $Revision$                                                       *
*   $LastChangedBy$                                                *
*   $Date$                     *
*                                                                            *
\*===========================================================================*/


#include "OpenFlipperThread.hh"
#include <iostream>


OpenFlipperThread::OpenFlipperThread( QString _jobId ) :
  job_(0),
  jobId_(_jobId)
{
}

OpenFlipperThread::~OpenFlipperThread() {
}

QString OpenFlipperThread::jobId() {
  return jobId_;
}

void OpenFlipperThread::run()
{
  if ( job_ == 0 ) {
    // Create a job wrapper to run a slot from within this thread
    job_ = new OpenFlipperJob( jobId_ );

    // Connect the slot which should run in this thread. This has to be a DirectConnection !
    // Otherwise the slot will run inside its owner context which will be the main loop!!
    connect(job_, SIGNAL(process(QString)),this,SIGNAL(function(QString) ),Qt::DirectConnection);
    
    // Connect the jobs finished function
    connect(job_, SIGNAL(finished()),this,SLOT(slotJobFinished() ) );
    
    // connect the function to start the job
    connect(this,SIGNAL(startProcessingInternal()),job_,SLOT(startJobProcessing()),Qt::QueuedConnection);
  }

  // Thread is ready for processing now, tell core that we can continue.
  startup_.release(1);

  // Start event queue (possibly added events are already queued here
  exec();
  
// TODO: Self destuction sometimes does not work! 
// Seems to be a race condition!!!

//   std::cerr << "Delete thread Object " << std::endl;
//   
//   deleteLater();
  
//   std::cerr << "Deleted Thread Object" << std::endl;
}

void OpenFlipperThread::cancel() {
  std::cerr << "Cancel not implemented" << std::endl;
}

void OpenFlipperThread::slotCancel( QString _jobId) {
 std::cerr << "Thread : cancel received" << std::endl;
 if ( _jobId == jobId_ )
  cancel(); 
}

void OpenFlipperThread::slotJobFinished( ) {
  emit finished( jobId_ );
  job_ = 0;
  quit();
}


void OpenFlipperThread::startProcessing() {
  
  // Wait for thread to come up and connect its signals ... otherwise the signals might get lost
  startup_.acquire(1);
  
  // Tell internal wrapper to start with the processing
  emit startProcessingInternal();
}

OpenFlipperJob::~OpenFlipperJob() 
{ 
}

void OpenFlipperJob::startJobProcessing() {
  
  // Actually start the process ( This function will block as it uses a direct connection )
  // But it only blocks the current thread.
  emit process(jobId_); 
  
  // Tell thread that the job is done.
  emit finished();
  
  // Cleanup this object
  deleteLater();
}
