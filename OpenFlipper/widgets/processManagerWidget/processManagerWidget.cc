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
 * processManagerWidget.cc
 *
 *  Created on: Apr 7, 2009
 *      Author: kremer
 */

#include "processManagerWidget.hh"

#include <OpenFlipper/common/GlobalOptions.hh>

void ProcessManagerWidget::updateStatus(QString _id, int _status) {

    QHash<QString, JobContainer>::iterator it = processMap_.find(_id);
           
    // No such item has been found -> returning
    if(it == processMap_.end()) return;

    // Set new status
    it.value().progress->setValue(_status);
}

void ProcessManagerWidget::setJobName(QString _id, QString _name) {
    
    QHash<QString, JobContainer>::iterator it = processMap_.find(_id);

    // No such item has been found -> returning
    if(it == processMap_.end()) return;
    
    it.value().id->setText(_name);
}

void ProcessManagerWidget::setJobDescription(QString _id, QString _desc) {
    
    QHash<QString, JobContainer>::iterator it = processMap_.find(_id);

    // No such item has been found -> returning
    if(it == processMap_.end()) return;
    
    // Set new description
    it.value().description->setText(_desc);
}

void ProcessManagerWidget::addJob(QString _id, QString _description,
                                      int _minSteps, int _maxSteps) {
    
    // Item with the same id has already been added
    if(processMap_.contains(_id)) return;
    
    QTableWidgetItem* name      = new QTableWidgetItem(_id);
    QTableWidgetItem* desc      = new QTableWidgetItem(_description);
    JobCancelButton* button     = new JobCancelButton("Cancel", _id, this->processList);
    QProgressBar* progressBar   = new QProgressBar(this->processList);
    progressBar->setMaximum(_maxSteps);
    progressBar->setMinimum(_minSteps);
    progressBar->setValue(0);
    progressBar->setTextVisible(true);
    
    // Connect cancel button to event handler
    connect(button, SIGNAL(pressed()), this, SLOT(cancelButtonPressed()));
    
    int newRow = this->processList->rowCount();
    
    this->processList->insertRow(newRow);
    
    this->processList->setItem(newRow, 0, name);
    this->processList->setItem(newRow, 1, desc);
    this->processList->setCellWidget(newRow, 2, progressBar);
    this->processList->setCellWidget(newRow, 3, button);
    
    // Insert job into local map
    JobContainer job;
    job.id          = name;
    job.description = desc;
    job.progress    = progressBar;
    job.button      = button;
    
    processMap_.insert(_id, job);
}

void ProcessManagerWidget::cancelButtonPressed() {
    
    // Get pushed button
    JobCancelButton* button = 0;
    button = dynamic_cast<JobCancelButton*>(QObject::sender());
    
    // Some error has occurred
    if(button == 0) return;
    
    // Emit signal to cancel job
    emit cancelJobRequested(button->jobId());
}

void ProcessManagerWidget::removeJob(QString _id) {

    QHash<QString, JobContainer>::iterator it = processMap_.find(_id);

    // No such item has been found -> returning
    if(it == processMap_.end()) return;

    processList->removeRow(processList->row(it.value().id));
    
    // Remove from local map
    processMap_.erase(it);
}
