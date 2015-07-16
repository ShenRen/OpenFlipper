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

#ifndef PROCESSINTERFACE_HH
#define PROCESSINTERFACE_HH

#include <OpenFlipper/threads/OpenFlipperThread.hh>

#if QT_VERSION >= 0x050000 
  #include <QtWidgets>
#else
  #include <QtGui>
#endif


/** \file ProcessInterface.hh
*
* Interface class for Thread and Process handling.\ref processInterfacePage
*/

/** \brief Interface class for Thread handling.
  *
  * \ref processInterfacePage "Detailed description"
  * \n
  *
  * This Interface is used to handle threads in OpenFlipper. You can run arbitrary slots within
  * separate threads.
 */
class ProcessInterface {

   public :

      /// Destructor
      virtual ~ProcessInterface() {};

    signals :
      
      /** \brief announce a new job
      *
      * Use this function to announce a new job. The Job Id should be a unique id for your job.
      * Prepend your PluginName to your id to make it unique across plugins.
      *
      * The range between _min and _max will be used to compute the progress percentage based on
      * the ProcessInterface::setJobState() values provided by the thread. You could for example
      * set it to the number of steps performed in your algorithm.
      *
      * @param _jobId       String which is used as the id of the thread
      * @param _description The description will be the string visible to the user.
      * @param _min         minimum of the status value range
      * @param _max         maximum of the status range
      * @param _blocking    If the thread is blocking, no other interaction will be allowed
      */
      virtual void startJob( QString _jobId, QString _description , int _min , int _max , bool _blocking  = false) {};

      /** \brief update job state
      *
      * Emit this signal to tell the core about your job status.
      *
      * @param _jobId       String which is used as the id of the thread
      * @param _value The status value has to be in the range you defined in ProcessInterface::startJob()
      */
      virtual void setJobState(QString _jobId, int _value ) {};
      
      /** \brief update job's name
      *
      * Emit this signal to tell the core to change a job's name.
      *
      * @param _jobId String which is used as the id of the thread
      * @param _name  The new name of the job
      */
      virtual void setJobName(QString _jobId, QString _name ) {};
      
      /** \brief update job's description text
      *
      * Emit this signal to tell the core to change a job's widget's description.
      *
      * @param _jobId String which is used as the id of the thread
      * @param _text The text of the job's description
      */
      virtual void setJobDescription(QString _jobId, QString _text ) {};
      
      /** \brief Cancel your job
       *
       * @param _jobId String which is used as the id of the thread
      */
      virtual void cancelJob(QString _jobId ) {};
      
      /** \brief Finish your job
       *
       * This signal has to be emitted when your job has finished. You can
       * directly connect the threads finished signal to this signal.
       *
       * @param _jobId String which is used as the id of the thread
      */
      virtual void finishJob(QString _jobId ) {};

      private slots :

        /** \brief A job has been canceled 
        *
        * This function is called when the user cancels a job. 
        * The returned name is the name of the job which has been canceled
        *
        * @param _job String which is used as the id of the thread
        */
        virtual void canceledJob (QString _job ) {};
      
};


/** \page processInterfacePage Process Interface
 * \n
\image html ProcessInterface.png
\n

The Process interface can be used to run arbitrary slots of your plugin within a separate thread of OpenFlipper.
OpenFlipper supports a process manager, visualizing currently running operations to the user. Process dialogs
and user information can be provided via special signals.

As in qt user interface operations are only allowed from within the main thread, you can not interact with the ui
in your threads. Feedback like the progress or other messages need to be passed through the ProcessInterface.
Additionally you have to make sure, that your operations can actually run in parallel to others. If not, you have
to mark your thread as blocking, which would prevent the user from starting additional operations until your
job gets finished.

Using threads in OpenFlipper is quite simple. All necessary class definitions are already available to plugins.
A tutorial for the thread interface is available:
\ref tutorial_thread

\section processInterface_setup Process Setup
So the first thing we have to do is instantiate an OpenFlipperThread object and tell the core that we just
created a thread via ProcessInterface::startJob().

\code
// Create job named "My Thread" .. Name will e visible in OpenFlippers process manager.
OpenFlipperThread* thread = new OpenFlipperThread(name() + "My thread");

// Tell OpenFlipper about the newly created job. In this case, it gets a description and
// a span for its steps. This job has 100 steps from 0 to 100. These values are used
// to display a progress bar.
// The last parameter means that the job will be non-blocking, so the ui will still be available.
emit startJob( name() + "My thread", "My thread's description" , 0 , 100 , false);
\endcode

Next thing to do is connecting the thread's signal to local signals/slots in order to keep track of
updates concerning the processing of our thread.

\code
// Slot connection required to tell the core, that the thread has finished
connect(thread, SIGNAL(finished(QString)),   this, SIGNAL(finishJob(QString)));

// Slot connection required setting the function in your plugin that should run inside the thread.
// It will get the job name which can be used to control the ui elements like progress.
// If you don't want to show progress you can skip the jobId QString.
connect(thread, SIGNAL(function(QString)),   this, SLOT(testFunctionThread(QString)), Qt::DirectConnection);
\endcode

\section processInterface_start Process Startup
The function() signal needs to be connected to the function that will actually be processed in the thread.
Once having connected these signals, we're about to start our thread:

\code
// Launch the thread
thread->start();

// Start processing of the function code (connected via signal OpenFlipperThread::function() )
// This function is non-blocking and will return immediately.
thread->startProcessing();
\endcode

\section processInterface_status Status Reports
There are two signals that you can use to inform the user about your threads state. The first is
ProcessInterface::setJobState(). It gets the jobId and an int. The int is the process status.
It has to be in the values that have been given with ProcessInterface::startJob() before.
OpenFlipper will calculate the percentage on its own and display a progress bar at the specific position.

The second function is  ProcessInterface::setJobDescription() getting a description, that can be changed
wile the thread is running. The following example shows a simple thread slot,using these signals.
\code

// Useless example function which runs in a thread and updates the ui state.
// The QString parameter is only necessary if you want to provide feedback.
void testFunctionThread(QString _jobId) {

    for (int i = 0 ; i < 1000000; ++i) {

        // Emit new progress state
        if(i % 10000 == 0)
            emit setJobState(_jobId, (int)(i / 10000));

        if(i == 500000) {
            // When half of the processing is finished
            // change the job's description
            emit setJobDescription(_jobId, "Half way done");
        }
    }
}
\endcode

\section processInterface_usage Usage

To use the ProcessInterface:
<ul>
<li> include ProcessInterface.hh in your plugins header file
<li> derive your plugin from the class ProcessInterface
<li> add Q_INTERFACES(ProcessInterface) to your plugin class
<li> And add the signals or slots you want to use to your plugin class (You don't need to implement all of them)
</ul>

\note In general, OpenFlipper signals should not be emitted from other threads than the main thread. However, there are
some signals, which are save to call outside of another thread. Some will block, until the main thread executed the function,
some run asnyc. If a function is guaranteed to be threadsafe, you will find a note in the documentation e.g. \c LoggingInterface::log()

\section processInterface_quickstart Quick example
A quick example for stating a thread:

\code
 OpenFlipperThread* thread = new OpenFlipperThread(name() + "unique id");                                   // Create your thread containing a unique id
 connect(thread,SIGNAL( finished(QString)), this,SIGNAL(finishJob(QString)));                               // connect your threads finish info to the global one ( you can do the same for a local one )
 connect(thread,SIGNAL( function(QString)), this,SLOT(testFunctionThread(QString)),Qt::DirectConnection);   // You can directly give a slot of your app that gets called
 emit startJob( name() + "unique id", "Description" , 0 , 100 , false);                                     // Tell the core about your thread
 thread->start();                                                                                           // start thread
 thread->startProcessing();                                                                                 // start processing
\endcode

Note: If your class is derived from OpenFlipperThread, do not reimplement the
\a run() function. Instead, use the \a function(QString)
to connect the your running function with \a Qt::DirectConnection. Short example:
Be \a MyThread the derived thread class
\code{.cpp}
 class MyThread : public OpenFlipperThread
\endcode

and \a parallelFunction(QString) the function which should run by different threads.
\code
void MyThread::parallelFunction(QString) // function which will run in parallel
{
  std::cout << "I am running in another thread." << currentThreadId() << std::endl;
}
\endcode

Instead of reimplementing the run function, connect the \a parallelFunction(QString) with
the \a function(QString) signal in e.g. the constructor:
\code
MyThread::MyThread(QString _jobId):OpenFlipperThread(_jobId)
{
  // connect signal function(QString) with the self defined slot function
  connect(this,SIGNAL(function(QString)),this,SLOT(parallelFunction(QString)), Qt::DirectConnection);
}
\endcode

Note: The OpenFlipperThread will not be notified about job cancelling. You plugin
has to be aware of it.

Note: You might also want to connect the signal OpenFlipperThread::state()
to the plugin's signal setJobState():

\code
 connect(thread, SIGNAL(state(QString, int)), this, SIGNAL(setJobState(QString, int)));
\endcode


Use signal OpenFlipperThread::state() within your run() function in order to
correctly update the job's state.
*/

Q_DECLARE_INTERFACE(ProcessInterface,"OpenFlipper.ProcessInterface/1.0")

#endif // PROCESSINTERFACE_HH
