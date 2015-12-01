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



//=============================================================================
//
//  CLASS CoreWidget - IMPLEMENTATION
//
//=============================================================================


//== INCLUDES =================================================================

// -------------------- mview
#include "CoreWidget.hh"

//== IMPLEMENTATION ========================================================== 



void CoreWidget::statusMessage(QString _message, int _timeout) {
  statusBar_->showMessage(_message,_timeout);
}


//=============================================================================


void CoreWidget::setupStatusBar()
{
  statusBar_ = new ColorStatusBar();

  setStatusBar( statusBar_ );

  QPixmap pix;
  pix.load(OpenFlipper::Options::iconDirStr() +
          OpenFlipper::Options::dirSeparator() + 
          "status_green.png");
  
  
  statusIcon_ = new QLabel();
  statusIcon_->setPixmap(pix.scaled(12,12,Qt::KeepAspectRatio,Qt::SmoothTransformation));
  
  statusBar_->addPermanentWidget(statusIcon_);

  if ( ! OpenFlipperSettings().value("Core/Gui/StatusBar/hidden",false).toBool() )
    statusBar()->show();
  else
    statusBar()->hide();
}

//=============================================================================

void CoreWidget::clearStatusMessage()
{
  statusBar_->clearMessage();
}

//=============================================================================

void CoreWidget::setStatus( ApplicationStatus::applicationStatus _status)
{
  QPixmap pix;

  switch (_status) {
    case ApplicationStatus::READY :
      pix.load(OpenFlipper::Options::iconDirStr() +
          OpenFlipper::Options::dirSeparator() + 
          "status_green.png");
      break;
    case ApplicationStatus::PROCESSING :
      pix.load(OpenFlipper::Options::iconDirStr() +
          OpenFlipper::Options::dirSeparator() + 
          "status_yellow.png");
      break;
    case ApplicationStatus::BLOCKED :
      pix.load(OpenFlipper::Options::iconDirStr() +
          OpenFlipper::Options::dirSeparator() + 
          "status_red.png");
      break;
  }
  
  statusIcon_->setPixmap(pix.scaled(12,12,Qt::KeepAspectRatio,Qt::SmoothTransformation));
  
}

void CoreWidget::addWidgetToStatusbar(QWidget* _widget){

  statusBar_->addPermanentWidget(_widget);

  statusBar_->removeWidget(statusIcon_);
  statusBar_->addPermanentWidget(statusIcon_);
  statusIcon_->show();
  
  
}

//-----------------------------------------------------------------------------

/** Hide or show Status bar
  */
void
CoreWidget::toggleStatusBar() {

  //toggle
  showStatusBar( OpenFlipperSettings().value("Core/Gui/StatusBar/hidden",false).toBool() );
}

//-----------------------------------------------------------------------------

/** Hide or show  Status bar
  */
void
CoreWidget::showStatusBar( bool _state ) {

  //toggle
  OpenFlipperSettings().setValue("Core/Gui/StatusBar/hidden",!_state);

  if ( OpenFlipperSettings().value("Core/Gui/StatusBar/hidden",false).toBool() ){
    statusBar_->setVisible(false);
  }else{
    statusBar_->setVisible(true);
  }
  emit statusBarVisChanged(_state);
}

//=============================================================================
