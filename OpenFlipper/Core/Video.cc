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
//  CLASS Core - IMPLEMENTATION
//
//=============================================================================


//== INCLUDES =================================================================

// -------------------- mview
#include "Core.hh"

//== IMPLEMENTATION ==========================================================

//-----------------------------------------------------------------------------

void Core::startVideoCapture(QString _baseName, int _fps, bool _captureViewers) {
  
    connect( &videoTimer_ , SIGNAL(timeout()), this, SLOT( viewUpdated() ) ,Qt::DirectConnection );
  // set track to true;
  videoTimer_.setSingleShot(false);

  // 25 fps
  videoTimer_.start( 1000 / _fps);

  lastVideoTime_.start();

  QString name = _baseName;

  if (_captureViewers){
    applicationSnapshotName(name);
    captureType_ = 1;
  } else {
    applicationSnapshotName(name);
    captureType_ = 0;
  }

  capture_ = true;
}

void Core::captureVideo() {
  int elapsed = lastVideoTime_.elapsed();

  if ( elapsed < videoTimer_.interval() ) {
//     std::cerr << "Too early to capture" << std::endl;
    return;
  }

  lastVideoTime_.restart();

  if (captureType_ == 1)
    viewerSnapshot();
  else
    applicationSnapshot();
}


void Core::stopVideoCapture() {
  videoTimer_.stop();
  capture_ = false;

  if ( OpenFlipper::Options::gui() ){

    if (captureType_ == 1){
      coreWidget_->glView_->resize(lastWidth_, lastHeight_);
    } else {
      coreWidget_->resize(lastWidth_, lastHeight_);
    }
  }
}

void Core::viewUpdated( ) {
  
  if ( capture_ )
    captureVideo();

  // Only call the view updates, if the scenegraph updates are not currently locked!
  if ( ! OpenFlipper::Options::sceneGraphUpdatesBlocked() )
    emit pluginViewChanged();
}



//=============================================================================
