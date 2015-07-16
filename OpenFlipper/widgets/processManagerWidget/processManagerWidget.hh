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


/*
 * processManagerWidget.hh
 *
 *  Created on: Apr 7, 2009
 *      Author: kremer
 */

#ifndef PROCESSMANAGERWIDGET_HH_
#define PROCESSMANAGERWIDGET_HH_


#include "ui_processManagerWidget.hh"

#if QT_VERSION >= 0x050000 
  #include <QtWidgets>
#else
  #include <QtGui>
#endif

// A button class that additionally stores
// an attached job's id.
class JobCancelButton : public QPushButton {
    Q_OBJECT
    
public:
    JobCancelButton(QString _caption, QString _jobId, QWidget* _parent = 0) :
    QPushButton(_caption, _parent),
    jobId_(_jobId) {};
    
    // Set job's id
    void setJobId(const QString& _jobId) { jobId_ = _jobId; };
    
    // Get job's id
    QString jobId() { return jobId_; }

private:
    QString jobId_;
};

class ProcessManagerWidget : public QWidget, public Ui::ProcessManagerWidget
{
  Q_OBJECT
  
  signals:
      
    void cancelJobRequested(QString _jobId);

  public:

    ProcessManagerWidget(QWidget* parent = 0) : QWidget(parent) {
        
        setupUi(this);
    };

    virtual ~ProcessManagerWidget() {};
    
    void updateStatus(QString _id, int _status);
    
    void setJobName(QString _id, QString _desc);
    
    void setJobDescription(QString _id, QString _desc);
    
    void addJob(QString _id, QString _description = "",
                    int _minSteps = 0, int _maxSteps = 100);
                    
    void removeJob(QString _jobName);
    
    size_t getNumJobs() { return processMap_.size(); }

  private slots:
      
    void cancelButtonPressed();
    
  private:
      
    // A container to hold the widget items
    struct JobContainer {
        QTableWidgetItem* id;
        QTableWidgetItem* description;
        QProgressBar*     progress;
        JobCancelButton*  button;
    };
    
    QHash<QString, JobContainer> processMap_;
};


#endif /* PROCESSMANAGERWIDGET_HH_ */
