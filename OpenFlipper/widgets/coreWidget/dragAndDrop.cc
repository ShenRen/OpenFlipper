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

#include <OpenFlipper/BasePlugin/PluginFunctions.hh>
#include <OpenFlipper/common/GlobalOptions.hh>



//== IMPLEMENTATION ==========================================================

// Drag evencts view Magic ( header for drag and drop of views )
static const char VIEW_MAGIC[] = "ACG::QtWidgets::QGLViewerWidget encoded view";

//=============================================================================

void CoreWidget::startDrag ( QMouseEvent* _event )
{
  QObject* senderPointer = sender();
  int examinerId = -1;

  if ( senderPointer != 0 ) {
    for ( unsigned int i = 0 ; i < OpenFlipper::Options::examinerWidgets(); ++i ) {
      if ( senderPointer == examiner_widgets_[i] ) {
        examinerId = i;
        break;
      }
    }

  }

  if ( examinerId == -1 ) {
    emit log(LOGERR , tr("startDrag in Core called by non examiner, stopping here"));
    return;
  }

  PluginFunctions::setActiveExaminer(examinerId);

  // Get the correct position in the widget
  QPoint position = _event->pos();

  // -1 if no object id found for the current picking position
  // otherwise the id of the object
  int objectId = -1;

  // Do picking in the gl area to find an object
  unsigned int    node_idx, target_idx;
  ACG::Vec3d      hit_point;
  BaseObjectData* object;
  if ( PluginFunctions::scenegraphPick ( ACG::SceneGraph::PICK_ANYTHING,
                                          position,
                                          node_idx,
                                          target_idx,
                                          &hit_point ) ) {
    if ( PluginFunctions::getPickedObject ( node_idx, object ) )
        objectId = object->id();
  }

  if ( objectId != -1 ) {
    emit log(LOGERR , tr("dragEvent Picked Object"));
  }




  QString view;
  examiner_widgets_[PluginFunctions::activeExaminer()]->encodeView ( view );

  QDrag     * drag = new QDrag ( this );
  QMimeData * mime_data = new QMimeData;

  mime_data->setText ( view );
  drag->setMimeData ( mime_data );
  drag->start();

}

void CoreWidget::dragEnterEvent ( QDragEnterEvent* _event ) {

  if ( _event->mimeData()->hasFormat ( "text/plain" ) ) {
      QString view ( _event->mimeData()->text() );

      // view information entering via drag
      if ( view.left ( sizeof ( VIEW_MAGIC ) - 1 ) == QString ( VIEW_MAGIC ) ) {
        _event->acceptProposedAction();
      }

  }

}

void CoreWidget::dropEvent ( QDropEvent* _event ) {
  QObject* senderPointer = sender();
  int examinerId = -1;

  if ( senderPointer != 0 ) {
    for ( unsigned int i = 0 ; i < OpenFlipper::Options::examinerWidgets(); ++i ) {
      if ( senderPointer == examiner_widgets_[i] ) {
        examinerId = i;
        break;
      }
    }
  }

  if ( examinerId == -1 ) {
    emit log(LOGERR , tr("startDrag in Core called by non examiner, stopping here"));
    return;
  }

  PluginFunctions::setActiveExaminer(examinerId);

  if ( _event->mimeData()->hasUrls() ) {
    QList<QUrl> urls = _event->mimeData()->urls();
    for ( int j = 0 ; j < urls.size() ; ++j ) {
      emit log(LOGWARN , tr("Dropped URL: %1").arg(urls[j].toLocalFile()));
      emit dragOpenFile(urls[j].toLocalFile());
    }

    return;
  }

  if ( _event->mimeData()->hasFormat ( "text/plain" ) ) {

    QString view ( _event->mimeData()->text() );

    // Dropped view information
    if ( view.left ( sizeof ( VIEW_MAGIC ) - 1 ) == QString ( VIEW_MAGIC ) ) {
      examiner_widgets_[PluginFunctions::activeExaminer()]->decodeView ( view );
      _event->acceptProposedAction();
      return;
    }
    
    ///\todo This is deprecated as its now handled by the url code above which should be used. Normally this code is not reached at all now. Remove it soon!
    // Dropped file information
    if ( view.left ( 7 ) == QString("file://") ) {
      _event->acceptProposedAction();
      emit dragOpenFile(view.remove(0,7));
      
      return;
    } 

    emit log(LOGERR , tr("Unknown drop event! Unable to handle the dropped data! Received data: %1").arg(view));
  }

  emit log(LOGERR , tr("Unknown drop event!"));


}

//=============================================================================
