/*===========================================================================*\
 *                                                                           *
 *                              OpenFlipper                                  *
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
 *                                                                           *
\*===========================================================================*/

/*===========================================================================*\
 *                                                                           *
 *   $Revision$                                                       *
 *   $Author$                                                      *
 *   $Date$                   *
 *                                                                           *
\*===========================================================================*/



//=============================================================================
//
//  CLASS QtBaseViewer - IMPLEMENTATION
//
//=============================================================================


//== INCLUDES =================================================================

#include "QtBaseViewer.hh"
#include <QInputDialog>

//== NAMESPACES ===============================================================

namespace ACG {
namespace QtWidgets {


//== IMPLEMENTATION ==========================================================

void QtBaseViewer::disableKeyHandling(bool _state ) {
  disableKeyHandling_ = _state;
};

//-----------------------------------------------------------------------------

bool QtBaseViewer::keyHandlingState() {
  return disableKeyHandling_;
};

//-----------------------------------------------------------------------------

void QtBaseViewer::glKeyReleaseEvent(QKeyEvent* _event) {
  _event->ignore();
};

//-----------------------------------------------------------------------------

void QtBaseViewer::glKeyPressEvent(QKeyEvent* _event)
{
  // Pass key event to parent widget without any handling
  if ( disableKeyHandling_ ) {
    _event->ignore();
    return;
  }

  bool handled(false);


  // CTRL + ALT events
  if ((_event->modifiers() & Qt::ControlModifier) &&
      (_event->modifiers() & Qt::AltModifier))
  {

    switch (_event->key())
    {
      // Stereo steeing: eye distance
      case Qt::Key_E:
      {
        bool save(glareaGrabbed_);
        bool ok(false);

        // release grabbing to process input dialog
        if(save) releaseGLArea();

        double val = QInputDialog::getDouble( this, "Eye Dist", "Eye Dist:",
                                              eyeDist_,
                                              0.0, 100.0, 10,
                                              &ok);
        // restore old setting
        if(save) grabGLArea();

        if (ok) {
          eyeDist_ = val;
          handled = true;
          updateGL();
        }

        break;
      }


      // Stereo setting: focal length
      case Qt::Key_F:
      {
        bool save(glareaGrabbed_);
        bool ok(false);

        // release grabbing to process input dialog
        if(save) releaseGLArea();

        double val = QInputDialog::getDouble( this, "Focal Dist", "Focal Dist:",
                                              focalDist_,
                                              0.0, 100.0, 10,
                                              &ok);
        //restore old setting
        if(save) grabGLArea();

        if (ok) {
          focalDist_ = val;
          handled = true;
          updateGL();
        }
        break;
      }
    }
  } else { // Normal events

    switch (_event->key())
    {
      // Lock / unlock update
      case Qt::Key_ScrollLock:
      {
        if (!updateLocked_) {
          lockUpdate();
          handled = true;
          std::cerr << "Display update locked\n";

        } else {
          unlockAndUpdate();
          handled = true;
          std::cerr << "Display update un-locked\n";
        }
        break;
      }

      case Qt::Key_Escape:
      {
        actionMode(lastActionMode_);
        handled = true;
        break;
      }
    }
  }

  // If the event has not been handled by the baseviewer, check for Key events from subclasses
  if(!handled) {
    handled = viewKeyPressEvent(_event);
  }


  // give event to application
  if (!handled) {
    _event->ignore();
    emit(signalKeyPressEvent(_event));
  }

}




//=============================================================================
} // namespace QtWidgets
} // namespace ACG
//=============================================================================
