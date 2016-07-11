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
//  CLASS Type BSpline Curve Plugin - IMPLEMENTATION
//
//  Author:  Ellen Dekkers <dekkers@cs.rwth-aachen.de>
//
//  Date: 2010-02-02 11:02:14 +0100 (Di, 02. Feb 2010) 
//
//=============================================================================


#include "TypeBSplineCurve.hh"

#include "OpenFlipper/BasePlugin/PluginFunctions.hh"
#include <OpenFlipper/common/BackupData.hh>
#include <OpenFlipper/common/GlobalOptions.hh>
#include "BSplineCurveBackup.hh"

//-----------------------------------------------------------------------------

TypeBSplineCurvePlugin::
TypeBSplineCurvePlugin():
renderControlPolygonAction_(0),
renderCurveAction_(0),
renderCPSelectionAction_(0),
renderKnotSelectionAction_(0),
renderNoSelectionAction_(0)
{
}


//-----------------------------------------------------------------------------

/** \brief Second initialization phase
 *
 */
void TypeBSplineCurvePlugin::pluginsInitialized()
{

  if ( OpenFlipper::Options::gui() ){

    QMenu* contextMenu = new QMenu("Rendering");

    QString iconPath = OpenFlipper::Options::iconDirStr()+OpenFlipper::Options::dirSeparator();

    // Render Control Net
    renderControlPolygonAction_ = new QAction(tr("Render Control Polygon"), this);
    renderControlPolygonAction_->setStatusTip(tr("Render Control Polygon"));
//    renderControlPolygonAction_->setIcon( QIcon(iconPath + "showIndices.png") );
    renderControlPolygonAction_->setCheckable(true);
    renderControlPolygonAction_->setChecked(false);

    // Render Surface
    renderCurveAction_ = new QAction(tr("Render Curve"), this);
    renderCurveAction_->setStatusTip(tr("Render Curve"));
//    renderCurveAction_->setIcon( QIcon(iconPath + "coordsys.png") );
    renderCurveAction_->setCheckable(true);
    renderCurveAction_->setChecked(true);

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


    connect(renderControlPolygonAction_, SIGNAL(triggered()), this, SLOT(slotRenderControlPolygon()) );
    connect(renderCurveAction_,          SIGNAL(triggered()), this, SLOT(slotRenderCurve()) );

    connect(group, SIGNAL(triggered(QAction*)), this, SLOT(slotRenderSelection(QAction*)));

    contextMenu->addAction(renderControlPolygonAction_);
    contextMenu->addAction(renderCurveAction_);
    contextMenu->addSeparator();
    contextMenu->addAction(renderCPSelectionAction_);
    contextMenu->addAction(renderKnotSelectionAction_);
    contextMenu->addAction(renderNoSelectionAction_);

    emit addContextMenuItem(contextMenu->menuAction(), DATA_BSPLINE_CURVE, CONTEXTOBJECTMENU);
  }
}

//-----------------------------------------------------------------------------

void
TypeBSplineCurvePlugin::
slotUpdateContextMenu( int _objectId ) {
  if ( _objectId == -1)
    return;

  BaseObjectData* object;
  if ( !PluginFunctions::getObject(_objectId,object) )
    return;

  BSplineCurveObject* bsplineCurveObject = dynamic_cast<BSplineCurveObject*>(object);

  if(bsplineCurveObject != 0){
    renderControlPolygonAction_->setChecked( bsplineCurveObject->splineCurveNode()->render_control_polygon() );
    renderCurveAction_->setChecked( bsplineCurveObject->splineCurveNode()->render_bspline_curve() );
    renderCPSelectionAction_->setChecked( bsplineCurveObject->splineCurveNode()->get_selection_draw_mode() == ACG::SceneGraph::BSplineCurveNodeT<BSplineCurve>::CONTROLPOINT );
    renderKnotSelectionAction_->setChecked( bsplineCurveObject->splineCurveNode()->get_selection_draw_mode() == ACG::SceneGraph::BSplineCurveNodeT<BSplineCurve>::KNOTVECTOR );
    renderNoSelectionAction_->setChecked( bsplineCurveObject->splineCurveNode()->get_selection_draw_mode() == ACG::SceneGraph::BSplineCurveNodeT<BSplineCurve>::NONE );
  }
}

//-----------------------------------------------------------------------------

void TypeBSplineCurvePlugin::slotRenderControlPolygon(){

  QVariant contextObject = renderControlPolygonAction_->data();
  int objectId = contextObject.toInt();

  if ( objectId == -1)
    return;

  BaseObjectData* object;
  if ( !PluginFunctions::getObject(objectId,object) )
    return;

  BSplineCurveObject* bsplineCurveObject = dynamic_cast<BSplineCurveObject*>(object);

  if(bsplineCurveObject != 0){
    bsplineCurveObject->splineCurveNode()->render_control_polygon(renderControlPolygonAction_->isChecked());
    emit updatedObject( objectId, UPDATE_ALL );
  }
}

//-----------------------------------------------------------------------------

void TypeBSplineCurvePlugin::slotRenderCurve(){

  QVariant contextObject = renderCurveAction_->data();
  int objectId = contextObject.toInt();

  if ( objectId == -1)
    return;

  BaseObjectData* object;
  if ( !PluginFunctions::getObject(objectId,object) )
    return;

  BSplineCurveObject* bsplineCurveObject = dynamic_cast<BSplineCurveObject*>(object);

  if(bsplineCurveObject != 0){
    bsplineCurveObject->splineCurveNode()->render_bspline_curve(renderCurveAction_->isChecked());
    emit updatedObject( objectId, UPDATE_ALL );
  }
}

//-----------------------------------------------------------------------------

void TypeBSplineCurvePlugin::slotRenderSelection(QAction* _action) {

  QVariant contextObject = _action->data();
  int objectId = contextObject.toInt();

  if ( objectId == -1)
    return;

  BaseObjectData* object;
  if ( !PluginFunctions::getObject(objectId,object) )
    return;

  BSplineCurveObject* bsplineCurveObject = dynamic_cast<BSplineCurveObject*>(object);

  if(bsplineCurveObject != 0){
    if(_action == renderCPSelectionAction_) {
      bsplineCurveObject->splineCurveNode()->set_selection_draw_mode(ACG::SceneGraph::BSplineCurveNodeT<BSplineCurve>::CONTROLPOINT);
      emit updatedObject( objectId, UPDATE_ALL );
    } else if(_action == renderKnotSelectionAction_) {
      bsplineCurveObject->splineCurveNode()->set_selection_draw_mode(ACG::SceneGraph::BSplineCurveNodeT<BSplineCurve>::KNOTVECTOR);
      emit updatedObject( objectId, UPDATE_ALL );
    } else if(_action == renderNoSelectionAction_) {
      bsplineCurveObject->splineCurveNode()->set_selection_draw_mode(ACG::SceneGraph::BSplineCurveNodeT<BSplineCurve>::NONE);
      emit updatedObject( objectId, UPDATE_ALL );
    }
  }
}

//-----------------------------------------------------------------------------

bool
TypeBSplineCurvePlugin::
registerType() 
{
  addDataType("BSplineCurve",tr("B-Spline Curve"));
  setTypeIcon("BSplineCurve", "BSplineCurveType.png");
  return true;
}

//-----------------------------------------------------------------------------

DataType
TypeBSplineCurvePlugin::
supportedType()
{
  DataType type = DATA_BSPLINE_CURVE;
  return type;
}

//-----------------------------------------------------------------------------

int
TypeBSplineCurvePlugin::
addEmpty()
{
  // new object data struct
  BSplineCurveObject * object = new BSplineCurveObject();


  if ( OpenFlipperSettings().value("Core/File/AllTarget",false).toBool() )
    object->target(true);
  else {

    // Only the first object in the scene will be target
    if ( PluginFunctions::objectCount() == 1 )
       object->target(true);

    // If no target is available, we set the new object as target
    if (PluginFunctions::targetCount() == 0 )
       object->target(true);
  }

  QString name = "BSplineCurve_" + QString::number( object->id() ) + ".bsc";

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

void TypeBSplineCurvePlugin::generateBackup( int _id, QString _name, UpdateType _type ){
  
  BaseObjectData* object = 0;
  PluginFunctions::getObject(_id, object);
  
  BSplineCurveObject* splineObj = PluginFunctions::bsplineCurveObject(object);
  
  if ( splineObj != 0 ){

    //get backup object data
    BackupData* backupData = 0;

    if ( object->hasObjectData( OBJECT_BACKUPS ) )
      backupData = dynamic_cast< BackupData* >(object->objectData(OBJECT_BACKUPS));
    else{
      //add backup data
      backupData = new BackupData(object);
      object->setObjectData(OBJECT_BACKUPS, backupData);
    }
    
    //store a new backup
    BSplineCurveBackup* backup = new BSplineCurveBackup(splineObj, _name, _type);
    backupData->storeBackup( backup );
  }
}

//-----------------------------------------------------------------------------

#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2( typebsplinecurveplugin , TypeBSplineCurvePlugin );
#endif


