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
*   $Date$                    *
*                                                                            *
\*===========================================================================*/

#ifndef PROGRESSEMITTER_HH
#define PROGRESSEMITTER_HH

#include <QObject>
#include <OpenFlipper/BasePlugin/PluginFunctions.hh>

class ProgressEmitter : public QObject
{
  Q_OBJECT
  
  signals:
    void signalJobState(QString _jobId, int _value);
    void changeDescription(QString _jobId, QString _description);

    void backupRequest(int _objId, QString _desc);
  
  public:
    ProgressEmitter() : valid_(false) {}
    
    void attachJob(const QString& _jobId) {
        jobId_ = _jobId;
        valid_ = true;
    }

    void detachJob() {
        jobId_ = "";
        valid_ = false;
    }

    void sendProgressSignal(double _percent) {
        if(!valid_) return;
        emit signalJobState(jobId_, (int)_percent);
//       std::cerr << "Job State for " << jobId_.toStdString() << ": " << (int)_percent << "%" << std::endl;
    }
    
    void sendChangeDescriptionSignal(QString _description) {
        if(!valid_) return;
        emit changeDescription(jobId_, _description);
//       std::cerr << "Job description for " << jobId_.toStdString() << ": '" << _description.toStdString() << "'" << std::endl;
    }

  private:
    QString jobId_;

    bool valid_;
};


#endif //PROGRESSEMITTER_HH
