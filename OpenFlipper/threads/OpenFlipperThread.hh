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


#ifndef OPENFLIPPERTHREAD_HH
#define OPENFLIPPERTHREAD_HH

#include <QThread>
#include <QSemaphore>

#include <OpenFlipper/common/GlobalDefines.hh>


class OpenFlipperJob;

/** \brief Thread handling class for OpenFlipper

    Instantiate this class in order to provide a thread
    to a plugin. Unless you don't need a specialized
    run() or cancel() function, it won't be necessary to
    reimplement this class. Just connect the signals
    for the thread to work properly.
        
    Note: Updating GUI elements of the main window from
    within a thread which is not the main thread can
    cause unexpected crashes to OpenFlipper. This introduces
    some major limitations to the usage of threads
    in plugins. These include avoiding the creation
    of new objects and generally every function that
    produces logging messages.

    The class should be used along with \ref processInterfacePage

    It is not necessary to derive from this class for most operations,
    but you can.
*/
class DLLEXPORT OpenFlipperThread : public QThread
{
  Q_OBJECT
  
  public:
    explicit OpenFlipperThread( QString _jobId );
    ~OpenFlipperThread();
    
  //===========================================================================
  /** @name Advanced job processing
  * @{ */
  //===========================================================================
  public:
    
    /** \brief Main processing
    *
    * Do not reimplement this function. Connect the function()
    * signal with your running function. Use signal state()
    * in order to inform the core about the job's current state.
    */
    void run();
    
    /** \brief Cancel the job
    *
    * Reimplement this function to correctly terminate your job.
    * If your job is able to terminate correctly then you can reimplement this function
    * and stop your process.
    *
    */
    virtual void cancel();   
    
    /** \brief get JobId
     * get the Id of the current Job
     */
    QString jobId();

  public slots:
    /** \brief Cancel this job
     *
     * Call this slot with the correct job id to abort processing
     * This directly calls cancel()
     * This is only useful when you derived from this class, as other jobs might not react on this slot
    */
    void slotCancel( QString _jobId);
    
  signals:
    /** \brief Tell core about job state
    *
    * Use this signal to inform core about the job's state if and
    * only if the thread class inherits from OpenFlipperThread
    * and the virtual run() function is overridden. In all other cases
    * this signal won't be used and does not have to be connected to from
    * within the plugins.
    */
    void state( QString _jobId, int _state );

  /** @} */
    
    
  //===========================================================================
  /** @name Simple job processing
  * @{ */
  //===========================================================================
    
  signals:
    /**  \brief job function
    *
    * If you do not specialize the OpenFlipperThread, The function connected to this 
    * signal will be used as the processing function for your thread. When you call
    * \code
    * OpenFlipper::startProcessing();
    * \endcode
    * 
    * You have connect one of your slot using
    * \code
    * connect( OpenFlipperThread ,SIGNAL(function()),YourPlugin,SLOT(YourSlot(),Qt::DirectConnection) );
    * \endcode
    *
    * The default implementation will call your slot inside the new thread and the core will still respond.
    *
    * The optional parameter contains the job's id. In some cases the function that is to be called needs
    * the job's id for further processing (status updates, etc.)
    */
    void function(const QString _jobId = "");
    
    /** \brief job done
    *
    * This signal is emitted if your job has finished and should be used to tell the core that your job has finished.
    *
    */
    void finished( QString _jobId );
    
    
    
  public slots:
    /** \brief start processing
    *
    * This function will start the actual processing of a job.
    */
    void startProcessing();
    
  /** @} */
    
    
  private slots:
    /** \brief job has finished
    * 
    * Called by the job wrapper, when the job is done.
    */
    void slotJobFinished();
  
  signals:
    /** \brief start processing of a function
    *
    * (INTERNAL!) \n
    * This signal is used to start the process execution through the wrapper
    */
    void startProcessingInternal();
    
  private: 
    /** \brief Internal job wrapper
    *
    * (INTERNAL!) \n
    * This wrapper is used to start slots inside the event queue of the current thread.
    * All slots defined in the QThread object live in the callers thread(The main thread).
    * All slots inside the wrapper live in the thread created by QThread. and therefore
    * do not lock the main event loop
    */
    OpenFlipperJob* job_;
    
    /** \brief Id of the current job
    *
    * This holds the id of the current job. Most functions only react if the correct id is passed
    * to the function.
    */
    QString jobId_;
    
    /** \brief Semaphore to control order of thread startup calls
    *
    * This variable is used to control the order of thread startup calls. The Thread itself is
    * started, and when it is up and running, the resource will be created. The core thread waits
    * for this resource and afterwards starts execution of the threads processing function.
    * Without this sync, the processing call of the main thread might get lost as the thread is
    * not online and listening for the signal.
    */
    QSemaphore startup_;

};


// ================================================================================
// Job wrapper
// ================================================================================

/** \brief Internal Job execution object
*
* This class is used to start a process within a thread. The thread object itself
* lives in the event queue of the object that created the thread. So every signal
* or slot within the QThread object will be dispatched in the core event loop and
* therefore lock the gui. 
* Objects created in the QThread will run in the threads event queue as well as their
* signals and slots. So QThread uses this object to actually run arbitrary threads
* inside the threads event queue.
*
* See \ref processInterfacePage for details.
*/
class DLLEXPORT OpenFlipperJob : public QObject
{
  Q_OBJECT
  
  public:
    explicit OpenFlipperJob(const QString _jobId) : jobId_(_jobId) {}
    ~OpenFlipperJob();
  
  signals:
    /** \brief connection to actual processing function
    *
    * This signal has to be connected via a Qt::DirectConnection to an
    * arbitrary slot. This slot will be executed in the local event queue.
    * If this object is created in a QThread, than the slot will be run 
    * inside this thread.
    */
    void process(const QString _jobId = "");
    
    /** \brief Job done
    *
    * This signal is emitted, when the job has finished
    */
    void finished();
    
  public slots:
    /** \brief slot to start processing
    *
    * If this slot is called, the slot connected to process() will be executed
    * in the current thread.
    */
    void startJobProcessing();
    
  public:
    /// Set job's id
    void jobId(const QString& _jobId) { jobId_ = _jobId; }
    
    /// Get job's id
    QString jobId() const { return jobId_; }   
    
  private:
    /// The job's id
    QString jobId_;
};



#endif //OPENFLIPPERTHREAD_HH
