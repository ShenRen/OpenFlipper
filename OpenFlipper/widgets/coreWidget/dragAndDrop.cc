/*===========================================================================*\
 *                                                                           *
 *                              OpenFlipper                                  *
 *      Copyright (C) 2001-2009 by Computer Graphics Group, RWTH Aachen      *
 *                           www.openflipper.org                             *
 *                                                                           *
 *---------------------------------------------------------------------------*
 *  This file is part of OpenFlipper.                                        *
 *                                                                           *
 *  OpenFlipper is free software: you can redistribute it and/or modify      *
 *  it under the terms of the GNU Lesser General Public License as           *
 *  published by the Free Software Foundation, either version 3 of           *
 *  the License, or (at your option) any later version with the              *
 *  following exceptions:                                                    *
 *                                                                           *
 *  If other files instantiate templates or use macros                       *
 *  or inline functions from this file, or you compile this file and         *
 *  link it with other files to produce an executable, this file does        *
 *  not by itself cause the resulting executable to be covered by the        *
 *  GNU Lesser General Public License. This exception does not however       *
 *  invalidate any other reasons why the executable file might be            *
 *  covered by the GNU Lesser General Public License.                        *
 *                                                                           *
 *  OpenFlipper is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *  GNU Lesser General Public License for more details.                      *
 *                                                                           *
 *  You should have received a copy of the GNU LesserGeneral Public          *
 *  License along with OpenFlipper. If not,                                  *
 *  see <http://www.gnu.org/licenses/>.                                      *
 *                                                                           *
\*===========================================================================*/

/*===========================================================================*\
 *                                                                           *
 *   $Revision$                                                         *
 *   $Author$                                                      *
 *   $Date$                   *
 *                                                                           *
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

  if ( _event->mimeData()->hasFormat ( "text/plain" ) ) {

    QString view ( _event->mimeData()->text() );

    // Dropped view information
    if ( view.left ( sizeof ( VIEW_MAGIC ) - 1 ) == QString ( VIEW_MAGIC ) ) {
      examiner_widgets_[PluginFunctions::activeExaminer()]->decodeView ( view );
      _event->acceptProposedAction();
      return;
    }
    
    // Dropped file information
    if ( view.left ( 7 ) == QString("file://") ) {
      _event->acceptProposedAction();
      emit dragOpenFile(view.remove(0,7));
      return;
    } 
     
    emit log(LOGERR , tr("Unknown drop event! Unable to handle the dropped data!"));
     
  }


}

//=============================================================================
