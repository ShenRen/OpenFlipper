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
//  CLASS Type BSpline Surface Plugin - IMPLEMENTATION
//
//  Author:  Ellen Dekkers <dekkers@cs.rwth-aachen.de>
//
//  Date: 2010-02-02 11:01:53 +0100 (Di, 02. Feb 2010) 
//
//=============================================================================


#include "TypeBSplineSurface.hh"

#include "OpenFlipper/BasePlugin/PluginFunctions.hh"
#include <OpenFlipper/common/GlobalOptions.hh>

//-----------------------------------------------------------------------------

TypeBSplineSurfacePlugin::
TypeBSplineSurfacePlugin() :
renderControlNetAction_(0),
renderSurfaceAction_(0),
renderCPSelectionAction_(0),
renderKnotSelectionAction_(0),
renderNoSelectionAction_(0)
{
}

//-----------------------------------------------------------------------------

/** \brief Second initialization phase
 *
 */
void TypeBSplineSurfacePlugin::pluginsInitialized()
{

  if ( OpenFlipper::Options::gui() ){

    QMenu* contextMenu = new QMenu("Rendering");

    QString iconPath = OpenFlipper::Options::iconDirStr()+OpenFlipper::Options::dirSeparator();

    // Render Control Net
    renderControlNetAction_ = new QAction(tr("Render Control Net"), this);
    renderControlNetAction_->setStatusTip(tr("Render Control Net"));
//    renderControlNetAction_->setIcon( QIcon(iconPath + "showIndices.png") );
    renderControlNetAction_->setCheckable(true);
    renderControlNetAction_->setChecked(false);

    // Render Surface
    renderSurfaceAction_ = new QAction(tr("Render Surface"), this);
    renderSurfaceAction_->setStatusTip(tr("Render Surface"));
//    renderSurfaceAction_->setIcon( QIcon(iconPath + "coordsys.png") );
    renderSurfaceAction_->setCheckable(true);
    renderSurfaceAction_->setChecked(true);

    QActionGroup* group = new QActionGroup(this);
    group->setExclusive(true);

    // Render Control Point Selection
    renderCPSelectionAction_ = new QAction(tr("Render Control Point Selection"), group);
    renderCPSelectionAction_->setStatusTip(tr("Render Control Point Selection"));
//    renderCPSelectionAction_->setIcon( QIcon(iconPath + "coordsys.png") );
    renderCPSelectionAction_->setCheckable(true);
    renderCPSelectionAction_->setChecked(true);

    // Render Knot Selection
    renderKnotSelectionAction_ = new QAction(tr("Render Knot Selection"), group);
    renderKnotSelectionAction_->setStatusTip(tr("Render Knot Selection"));
//    renderKnotSelectionAction_->setIcon( QIcon(iconPath + "coordsys.png") );
    renderKnotSelectionAction_->setCheckable(true);
    renderKnotSelectionAction_->setChecked(true);

    // Render No Selection
    renderNoSelectionAction_ = new QAction(tr("Don't Render Selection"), group);
    renderNoSelectionAction_->setStatusTip(tr("Don't Render Selection"));
//    renderNoSelectionAction_->setIcon( QIcon(iconPath + "coordsys.png") );
    renderNoSelectionAction_->setCheckable(true);
    renderNoSelectionAction_->setChecked(true);


    connect(renderControlNetAction_,     SIGNAL(triggered()), this, SLOT(slotRenderControlNet()) );
    connect(renderSurfaceAction_, SIGNAL(triggered()), this, SLOT(slotRenderSurface()) );

    connect(group, SIGNAL(triggered(QAction*)), this, SLOT(slotRenderSelection(QAction*)));

    contextMenu->addAction(renderControlNetAction_);
    contextMenu->addAction(renderSurfaceAction_);
    contextMenu->addSeparator();
    contextMenu->addAction(renderCPSelectionAction_);
    contextMenu->addAction(renderKnotSelectionAction_);
    contextMenu->addAction(renderNoSelectionAction_);

    emit addContextMenuItem(contextMenu->menuAction(), DATA_BSPLINE_SURFACE, CONTEXTOBJECTMENU);
  }
}

//-----------------------------------------------------------------------------

void
TypeBSplineSurfacePlugin::
slotUpdateContextMenu( int _objectId ) {
  if ( _objectId == -1)
    return;

  BaseObjectData* object;
  if ( !PluginFunctions::getObject(_objectId,object) )
    return;

  BSplineSurfaceObject* bsplineSurfaceObject = dynamic_cast<BSplineSurfaceObject*>(object);

  if(bsplineSurfaceObject != 0){
    renderControlNetAction_->setChecked( bsplineSurfaceObject->splineSurfaceNode()->render_control_net() );
    renderSurfaceAction_->setChecked( bsplineSurfaceObject->splineSurfaceNode()->render_bspline_surface() );
    renderCPSelectionAction_->setChecked( bsplineSurfaceObject->splineSurfaceNode()->get_selection_draw_mode() == ACG::SceneGraph::BSplineSurfaceNodeT<BSplineSurface>::CONTROLPOINT );
    renderKnotSelectionAction_->setChecked( bsplineSurfaceObject->splineSurfaceNode()->get_selection_draw_mode() == ACG::SceneGraph::BSplineSurfaceNodeT<BSplineSurface>::KNOTVECTOR );
    renderNoSelectionAction_->setChecked( bsplineSurfaceObject->splineSurfaceNode()->get_selection_draw_mode() == ACG::SceneGraph::BSplineSurfaceNodeT<BSplineSurface>::NONE );
  }
}

//-----------------------------------------------------------------------------

void TypeBSplineSurfacePlugin::slotRenderControlNet(){

  QVariant contextObject = renderControlNetAction_->data();
  int objectId = contextObject.toInt();

  if ( objectId == -1)
    return;

  BaseObjectData* object;
  if ( !PluginFunctions::getObject(objectId,object) )
    return;

  BSplineSurfaceObject* bsplineSurfaceObject = dynamic_cast<BSplineSurfaceObject*>(object);

  if(bsplineSurfaceObject != 0){
    bsplineSurfaceObject->splineSurfaceNode()->render_control_net(renderControlNetAction_->isChecked());
    emit updatedObject( objectId, UPDATE_ALL );
  }
}

//-----------------------------------------------------------------------------

void TypeBSplineSurfacePlugin::slotRenderSurface(){

  QVariant contextObject = renderSurfaceAction_->data();
  int objectId = contextObject.toInt();

  if ( objectId == -1)
    return;

  BaseObjectData* object;
  if ( !PluginFunctions::getObject(objectId,object) )
    return;

  BSplineSurfaceObject* bsplineSurfaceObject = dynamic_cast<BSplineSurfaceObject*>(object);

  if(bsplineSurfaceObject != 0){
    bsplineSurfaceObject->splineSurfaceNode()->render_bspline_surface(renderSurfaceAction_->isChecked());
    emit updatedObject( objectId, UPDATE_ALL );
  }
}

//-----------------------------------------------------------------------------

void TypeBSplineSurfacePlugin::slotRenderSelection(QAction* _action) {

  QVariant contextObject = _action->data();
  int objectId = contextObject.toInt();

  if ( objectId == -1)
    return;

  BaseObjectData* object;
  if ( !PluginFunctions::getObject(objectId,object) )
    return;

  BSplineSurfaceObject* bsplineSurfaceObject = dynamic_cast<BSplineSurfaceObject*>(object);

  if(bsplineSurfaceObject != 0){
    if(_action == renderCPSelectionAction_) {
      bsplineSurfaceObject->splineSurfaceNode()->set_selection_draw_mode(ACG::SceneGraph::BSplineSurfaceNodeT<BSplineSurface>::CONTROLPOINT);
      emit updatedObject( objectId, UPDATE_ALL );
    } else if(_action == renderKnotSelectionAction_) {
      bsplineSurfaceObject->splineSurfaceNode()->set_selection_draw_mode(ACG::SceneGraph::BSplineSurfaceNodeT<BSplineSurface>::KNOTVECTOR);
      emit updatedObject( objectId, UPDATE_ALL );
    } else if(_action == renderNoSelectionAction_) {
      bsplineSurfaceObject->splineSurfaceNode()->set_selection_draw_mode(ACG::SceneGraph::BSplineSurfaceNodeT<BSplineSurface>::NONE);
      emit updatedObject( objectId, UPDATE_ALL );

    }
  }
}

//-----------------------------------------------------------------------------

bool
TypeBSplineSurfacePlugin::
registerType() 
{
  addDataType("BSplineSurface",tr("B-Spline Surface"));
  setTypeIcon("BSplineSurface", "BSplineSurfaceType.png");
  return true;
}

//-----------------------------------------------------------------------------

DataType
TypeBSplineSurfacePlugin::
supportedType()
{
  DataType type = DATA_BSPLINE_SURFACE;
  return type;
}

//-----------------------------------------------------------------------------

int
TypeBSplineSurfacePlugin::
addEmpty()
{
  // new object data struct
  BSplineSurfaceObject * object = new BSplineSurfaceObject();

  object->target(true);

  QString name = "BSplineSurface_" + QString::number( object->id() ) + ".bss";

  // call the local function to update names
  QFileInfo f(name);
  object->setName( f.fileName() );

  // set the default colors
  const QColor color = OpenFlipper::Options::defaultColor();
  const ACG::Vec4f default_color(color.redF(), color.greenF(), color.blueF(), color.alphaF());
  object->materialNode()->set_color(default_color);

  object->update();

  object->show();

  emit log(LOGINFO,object->getObjectinfo());

  emit emptyObjectAdded (object->id() );

  return object->id();
}

//-----------------------------------------------------------------------------

#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2( typebsplinesurfaceplugin , TypeBSplineSurfacePlugin );
#endif


