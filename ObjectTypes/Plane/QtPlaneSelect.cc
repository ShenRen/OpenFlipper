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

#include "QtPlaneSelect.hh"

#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <ACG/Scenegraph/GlutPrimitiveNode.hh>
#define PLUGINFUNCTIONS_C
#include <OpenFlipper/BasePlugin/PluginFunctions.hh>

#include <ACG/QtWidgets/QtColorTranslator.hh>


/*******************************************************************************
         Initialization and de initialization
*******************************************************************************/
QtPlaneSelect::QtPlaneSelect( ACG::GLState& glState )
  : glState( glState ),
    nodeIdx_(0),
    targetIdx_(0),
    isDragging( false ),
    planeNode_(0)
{

}

QtPlaneSelect::~QtPlaneSelect( )
{
}


/*******************************************************************************
        Implementation of public slots
*******************************************************************************/

void QtPlaneSelect::slotKeyReleaseEvent(QKeyEvent* event)
{
  if (event->key() == Qt::Key_Escape){
    if( planeNode_ )
    {
        planeNode_->delete_subtree( );
        planeNode_ = 0;
    }

    emit updateViewProxy( );

    // Trigger the event
    isDragging = false;
  }
}

void QtPlaneSelect::slotMouseEvent(QMouseEvent* event)
{
//  unsigned int width  = glState.viewport_width();
  unsigned int height = glState.viewport_height();

  //cancel on rightclick
  if (event->button() == Qt::RightButton){
    if( planeNode_ )
    {
        planeNode_->delete_subtree( );
        planeNode_ = 0;
    }

    emit updateViewProxy( );

    // Trigger the event
    isDragging = false;

    return;
  }


  switch( event->type() )
  {
    case QEvent::MouseButtonPress:
    {

      // Only react on the left button and ignore the others
      if ( event->button() != Qt::LeftButton )
        return;

      
      size_t     node_idx, target_idx;

      if (PluginFunctions::scenegraphPick(ACG::SceneGraph::PICK_FACE,
                                            event->pos(),
                                            node_idx,
                                            target_idx,
                                            &sourcePoint3D))
      {

        // Shift toward viewer
        //sourcePoint3D = sourcePoint3D + PluginFunctions::viewingDirection();

        isDragging = true;

        if ( planeNode_ == 0 ) {
          planeNode_ = new PlaneNode(plane_,PluginFunctions::getRootNode(),"PolyLine generation Plane"  );
        }

        setPlaneAndSize(sourcePoint3D,ACG::Vec3d(event->pos().x(), height-event->pos().y()-1.0, 0.0));

        planeNode_->show();
        emit nodeVisChangedProxy(planeNode_->id());

        nodeIdx_   = node_idx;
        targetIdx_ = target_idx;


        emit updateViewProxy( );
      }
    }break;
    case QEvent::MouseMove:
      {
          if( isDragging )
          {
            setPlaneAndSize(sourcePoint3D,ACG::Vec3d(event->pos().x(), height-event->pos().y()-1.0, 0.0));

              emit updateViewProxy( );
          }
      } break;

  case QEvent::MouseButtonRelease:
      {
          if( isDragging )
          {
              if( planeNode_ )
              {
                  planeNode_->delete_subtree( );
                  planeNode_ = NULL;
              }

              emit updateViewProxy( );

              emit( signalTriggerCut( ) );
              // Trigger the event
              isDragging = false;
          }
      } break;

  default:
      break;
  }


}


void QtPlaneSelect::setPlaneAndSize(const ACG::Vec3d& _sourcePoint3D,const ACG::Vec3d& _target2D)
{

  ACG::Vec3d source2D = glState.project( _sourcePoint3D );

  source2D[2] = 0;


  ACG::Vec3d diff = source2D - _target2D;
  //diff.normalize( );  <- this is bad
  ACG::Vec3d ortho(-diff[1], diff[0], 0 );

  ACG::Vec3d left = glState.unproject( source2D+ortho*10 + ACG::Vec3d(0,0,0) );
  ACG::Vec3d right= glState.unproject( source2D-ortho*10 + ACG::Vec3d(0,0,0) );

  ACG::Vec3d leftvec = left-sourcePoint3D;
  leftvec.normalize( );
  ACG::Vec3d rightvec = right-sourcePoint3D;
  rightvec.normalize( );

  normal = cross( rightvec, leftvec );
  normal.normalize( );

  ACG::Vec3d sourcePoint3Df(sourcePoint3D);
  ACG::Vec3d normald(normal);
  plane_.setPlane(sourcePoint3Df,normald);
  plane_.setSize(PluginFunctions::sceneRadius(),PluginFunctions::sceneRadius());
  planeNode_->update();
}




