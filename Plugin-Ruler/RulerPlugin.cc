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


#include "RulerPlugin.hh"

#include <OpenFlipper/BasePlugin/PluginFunctions.hh>
#include <OpenFlipper/common/GlobalOptions.hh>

//------------------------------------------------------------------------------
RulerPlugin::RulerPlugin()
:
buttonAction_(0),
pickModeName_("MeasureDistance"),
lineDrag_(-1),
dblClickCheck_(false),
optionsWidget_(0),
textSizeSettingName_(name()+QString("/TextSize"))
{

}

//------------------------------------------------------------------------------
RulerPlugin::~RulerPlugin()
{
  reset();
}
//------------------------------------------------------------------------------
void RulerPlugin::initializePlugin()
{
  QToolBar *button = new QToolBar("Ruler");

  buttonAction_ = new QAction(tr("<B>Ruler</B><br> Display the distance between two points."),this);

  button->addAction(buttonAction_);
  buttonAction_->setIcon(QIcon(OpenFlipper::Options::iconDirStr()+OpenFlipper::Options::dirSeparator()+"ruler.png"));
  buttonAction_->setCheckable(true);
  WhatsThisGenerator whatsThisGen("Ruler");
  whatsThisGen.setWhatsThis(buttonAction_,tr("Measures the distance between two points.<br><b>LeftClick</b>: define/change the position.<br><b>DoubleClick</b>: reset the ruler.<br>"));

  buttonAction_->setChecked(false);

  connect(buttonAction_,SIGNAL(triggered()), this , SLOT(slotChangePickMode()) );
  emit addToolbar(button);
}

//------------------------------------------------------------------------------
void RulerPlugin::pluginsInitialized()
{
  emit addPickMode(pickModeName_);
}

//------------------------------------------------------------------------------
void RulerPlugin::slotMouseEvent(QMouseEvent* _event)
{
  if ( PluginFunctions::pickMode() != pickModeName_)
    return;

//////create or change ruler/////////
//set one of the points, depending on the hit state (first, second or modifying)
  if (_event->type() == QEvent::MouseButtonRelease )
  {
    unsigned int node_idx, target_idx;
    OpenMesh::Vec3d hitPoint;

    // Get picked object's identifier by picking in scenegraph
    if ( PluginFunctions::scenegraphPick(ACG::SceneGraph::PICK_ANYTHING ,_event->pos(), node_idx, target_idx, &hitPoint) && !dblClickCheck_)
    {
      if (!currentRuler_)
      {
        //create a new Ruler
        BaseObjectData* object;
        if ( PluginFunctions::getPickedObject(node_idx, object) )
        {
          currentRuler_.reset(new Ruler(object,name(),0));
          connect(currentRuler_.get(),SIGNAL(updateView()),this,SIGNAL(updateView()));
          unsigned textSize = OpenFlipperSettings().value(textSizeSettingName_,16).toUInt();
          currentRuler_->setTextSize(textSize);
          currentRuler_->setPoints(hitPoint,hitPoint);
          enableDragMode(1);
        }

      }
      else
      {
        //Ruler was created -> change position of a point
        if (!dragModeActive())
        {
          //dragmode is disabled ->  update position of nearest point
          float distToStart = (currentRuler_->points()[0] - hitPoint).length();
          float distToEnd = (currentRuler_->points()[1] - hitPoint).length();
          if (distToStart < distToEnd)
            currentRuler_->setStartPoint(hitPoint);
          else
            currentRuler_->setEndPoint(hitPoint);
        }
        else
        {
          //second: drag mode is enabled so we can easily update the position
          if (lineDrag_ == 0)
            currentRuler_->setStartPoint(hitPoint);
          else
            currentRuler_->setEndPoint(hitPoint);
          disableDragMode();
        }
      }
    }
    else // if nothing was picked
    {
      reset();
    }
  }

//////enable drag mode//////
  else if (_event->type() == QEvent::MouseButtonPress)
  {//enable drag mode

    if (currentRuler_)
    {
      //decides which point is the nearest one, so
      //it can be dragged
      unsigned int node_idx, target_idx;
      OpenMesh::Vec3d hitPoint;

      // Get picked object's identifier by picking in scenegraph
      if ( PluginFunctions::scenegraphPick(ACG::SceneGraph::PICK_ANYTHING ,_event->pos(), node_idx, target_idx, &hitPoint) )
      {
        float distToStart = (currentRuler_->points()[0] - hitPoint).length();
        float distToEnd = (currentRuler_->points()[1] - hitPoint).length();
        enableDragMode( (distToStart < distToEnd)? 0 : 1);
      }
    }
  }
////////modify ruler of drag mode was enabled/////////
  else if (_event->type() == QEvent::MouseMove && dragModeActive())
  {//mouse moved and drag mode is enabled

    unsigned int node_idx, target_idx;
    OpenMesh::Vec3d hitPoint;
    ACG::Vec3d hitPoints[2];
    std::copy(currentRuler_->points(),currentRuler_->points()+2,hitPoints);

    // Get picked object's identifier by picking in scenegraph
    if ( !PluginFunctions::scenegraphPick(ACG::SceneGraph::PICK_ANYTHING ,_event->pos(), node_idx, target_idx, &hitPoint) )
    {
      //if mouse is not over an object, get the unprojected coordinates and the last z-value
      QPoint position = _event->pos();
      ACG::Vec3d viewCoords = ACG::Vec3d(position.x(), PluginFunctions::viewerProperties().glState().context_height() - position.y(), 0.5);
      hitPoint = PluginFunctions::viewerProperties().glState().unproject(viewCoords);
      hitPoints[lineDrag_] = ACG::Vec3d(hitPoint.data()[0], hitPoint.data()[1], hitPoints[lineDrag_].data()[2] );
    }
    else
      hitPoints[lineDrag_] = hitPoint;

    currentRuler_->setPoints(hitPoints[0],hitPoints[1]);
  }

///////////////////////Reset/////////////////////////////////
  else if (_event->type() == QEvent::MouseButtonDblClick)
  {//reset
    reset();
    dblClickCheck_ = true;
  }
  if (dblClickCheck_ && _event->type() == QEvent::MouseButtonRelease)
    dblClickCheck_ = false;
}

//------------------------------------------------------------------------------
void RulerPlugin::reset()
{
  currentRuler_.reset();
  lineDrag_ = -1;
}
//------------------------------------------------------------------------------
void RulerPlugin::enableDragMode(const int _point)
{
  lineDrag_ = _point;
  emit setPickModeMouseTracking(pickModeName_,true);
}

//------------------------------------------------------------------------------
void RulerPlugin::disableDragMode()
{
  lineDrag_ = -1;
  emit setPickModeMouseTracking(pickModeName_,false);
}

//------------------------------------------------------------------------------
void RulerPlugin::slotChangePickMode()
{
  PluginFunctions::actionMode( Viewer::PickingMode );
  PluginFunctions::pickMode(pickModeName_);

}

//------------------------------------------------------------------------------
void RulerPlugin::slotPickModeChanged(const std::string& _mode)
{
  buttonAction_->setChecked(_mode == pickModeName_);
}

//------------------------------------------------------------------------------
void RulerPlugin::slotAllCleared()
{
  disableDragMode();
}

void RulerPlugin::objectDeleted(int _id)
{
  if (!currentRuler_)
    return;

  disableDragMode();
  if (_id == currentRuler_->getBaseObj()->id())
  {
    disconnect(currentRuler_.get(),SIGNAL(updateView()),this,SIGNAL(updateView()));
    currentRuler_.reset();
  }
}
//------------------------------------------------------------------------------
bool RulerPlugin::initializeOptionsWidget(QWidget*& _widget)
{
  if (!optionsWidget_)
  {
    optionsWidget_ = new RulerOptions();
  }

  unsigned textSize = OpenFlipperSettings().value(textSizeSettingName_,16).toUInt();
  optionsWidget_->textSizeSpinBox->setValue(textSize);
  _widget = optionsWidget_;
  return true;
}
//------------------------------------------------------------------------------
void RulerPlugin::applyOptions()
{
  int textSize = optionsWidget_->textSizeSpinBox->value();
  if (currentRuler_)
    currentRuler_->setTextSize(textSize);
  OpenFlipperSettings().setValue(textSizeSettingName_,textSize);
}
//------------------------------------------------------------------------------
void RulerPlugin::slotViewChanged()
{

  if (!currentRuler_)
    return;

  //compute line direction
  ACG::Vec3d lineVector = currentRuler_->points()[0] - currentRuler_->points()[1];
  ACG::Vec3d rightVec = (PluginFunctions::viewingDirection() % -PluginFunctions::upVector()).normalize();

  float cosAngleLineRight = lineVector.normalize() | rightVec;
  float cosAngleUpLine = PluginFunctions::upVector().normalized() | lineVector.normalized() ;


  rightVec *= -0.5f*currentRuler_->textScale();
  ACG::Vec3d updownVec = PluginFunctions::upVector()*currentRuler_->textScale();

  //compute up/down offset
  if (cosAngleLineRight > 0.f)
  {
    updownVec *= (cosAngleUpLine < 0.f) ? -2.f : 0.5f;
  }
  else
  {
    updownVec *= (cosAngleUpLine < 0.f) ? 0.5f : -2.0f;
  }

  // small offset to the right and big offset up/down depending on the line
  currentRuler_->setTextOffset(rightVec+updownVec);


}

#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2( rulerPlugin , RulerPlugin );
#endif

