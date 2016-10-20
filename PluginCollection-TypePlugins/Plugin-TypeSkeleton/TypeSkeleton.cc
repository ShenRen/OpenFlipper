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


#include "TypeSkeleton.hh"

#include "OpenFlipper/BasePlugin/PluginFunctions.hh"
#include <OpenFlipper/common/BackupData.hh>
#include <OpenFlipper/common/GlobalOptions.hh>
#include "SkeletonBackup.hh"

TypeSkeletonPlugin::TypeSkeletonPlugin()
  : showIndicesAction_(0),
    showCoordFramesAction_(0),
    showMotionAction_(0)
{
}

/** \brief Second initialization phase
 *
 */
void TypeSkeletonPlugin::pluginsInitialized()
{
  
  if ( OpenFlipper::Options::gui() ){
  
    QMenu* contextMenu = new QMenu("Options");
    
    QString iconPath = OpenFlipper::Options::iconDirStr()+OpenFlipper::Options::dirSeparator();

    //Show indices
    showIndicesAction_ = new QAction(tr("Show &Indices"), this);
    showIndicesAction_->setStatusTip(tr("Show Joint Indices"));
    showIndicesAction_->setIcon( QIcon(iconPath + "showIndices.png") );
    showIndicesAction_->setCheckable(true);
    showIndicesAction_->setChecked(false);
    //Show coord-frames
    showCoordFramesAction_ = new QAction(tr("Show &Coordinate Frames"), this);
    showCoordFramesAction_->setStatusTip(tr("Show Coordinate Frames for joints"));
    showCoordFramesAction_->setIcon( QIcon(iconPath + "coordsys.png") );
    showCoordFramesAction_->setCheckable(true);
    showCoordFramesAction_->setChecked(false);
    //Show motion space
    showMotionAction_ = new QAction(tr("Show &Motion Path"), this);
    showMotionAction_->setStatusTip(tr("Show path of motions for joints"));
    showMotionAction_->setIcon( QIcon(iconPath + "motionPath.png") );
    showMotionAction_->setCheckable(true);
    showMotionAction_->setChecked(false);
    
    connect(showIndicesAction_,     SIGNAL(triggered()), this, SLOT(slotShowIndices()) );
    connect(showCoordFramesAction_, SIGNAL(triggered()), this, SLOT(slotShowCoordFrames()) );
    connect(showMotionAction_,      SIGNAL(triggered()), this, SLOT(slotShowMotionPath()) );
    
    contextMenu->addAction(showIndicesAction_);
    contextMenu->addAction(showCoordFramesAction_);
    contextMenu->addAction(showMotionAction_);

    emit addContextMenuItem(contextMenu->menuAction(), DATA_SKELETON, CONTEXTOBJECTMENU);
  }
}

void TypeSkeletonPlugin::slotUpdateContextMenu( int _objectId )
{
  if ( _objectId == -1)
    return;

  BaseObjectData* object;
  if ( !PluginFunctions::getObject(_objectId,object) )
    return;

  SkeletonObject* skeletonObject = dynamic_cast<SkeletonObject*>(object);
  
  if(skeletonObject != 0){
    showIndicesAction_->setChecked( skeletonObject->indicesVisible() );
    showCoordFramesAction_->setChecked( skeletonObject->skeletonNode()->coordFramesVisible() );
    showMotionAction_->setChecked( skeletonObject->motionPathVisible() );
  }
}

void TypeSkeletonPlugin::slotShowIndices(){
  
  QVariant contextObject = showIndicesAction_->data();
  int objectId = contextObject.toInt();

  if ( objectId == -1)
    return;

  BaseObjectData* object;
  if ( !PluginFunctions::getObject(objectId,object) )
    return;

  SkeletonObject* skeletonObject = dynamic_cast<SkeletonObject*>(object);
  
  if(skeletonObject != 0){
    skeletonObject->showIndices( showIndicesAction_->isChecked() );
    emit updatedObject( objectId, UPDATE_ALL );
  }
}

void TypeSkeletonPlugin::slotShowCoordFrames(){
  
  QVariant contextObject = showCoordFramesAction_->data();
  int objectId = contextObject.toInt();

  if ( objectId == -1)
    return;

  BaseObjectData* object;
  if ( !PluginFunctions::getObject(objectId,object) )
    return;

  SkeletonObject* skeletonObject = dynamic_cast<SkeletonObject*>(object);
  
  if(skeletonObject != 0){
    skeletonObject->skeletonNode()->showCoordFrames( showCoordFramesAction_->isChecked() );
    emit updatedObject( objectId, UPDATE_ALL );
  }
}

void TypeSkeletonPlugin::slotShowMotionPath(){
  
  QVariant contextObject = showMotionAction_->data();
  int objectId = contextObject.toInt();

  if ( objectId == -1)
    return;

  BaseObjectData* object;
  if ( !PluginFunctions::getObject(objectId,object) )
    return;

  SkeletonObject* skeletonObject = dynamic_cast<SkeletonObject*>(object);
  
  if(skeletonObject != 0){
    skeletonObject->showMotionPath( showMotionAction_->isChecked() );
    emit updatedObject( objectId, UPDATE_ALL );
  }
}

bool TypeSkeletonPlugin::registerType() {
  addDataType("Skeleton",tr("Skeleton"));
  setTypeIcon("Skeleton", "SkeletonType.png");
  return true;
}

int TypeSkeletonPlugin::addEmpty(){
    
  // new object data struct
  SkeletonObject * object = new SkeletonObject();

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

  QString name = QString(tr("New Skeleton %1.skl").arg( object->id() ));

  // call the local function to update names
  QFileInfo f(name);
  object->setName( f.fileName() );

  // set the default color
  object->materialNode()->set_color(ACG::Vec4f(0.654f, 0.8f, 1.0f, 1.0f));

  object->update();

  object->show();

  emit emptyObjectAdded (object->id() );

  return object->id();
}

void TypeSkeletonPlugin::generateBackup( int _id, QString _name, UpdateType _type ){
  
  BaseObjectData* object = 0;
  PluginFunctions::getObject(_id, object);
  
  SkeletonObject* skelObj = PluginFunctions::skeletonObject(object);
  
  if ( skelObj != 0 ){

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
    SkeletonBackup* backup = new SkeletonBackup(skelObj, _name, _type);
    backupData->storeBackup( backup );
  }
}

#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2( typeskeletonplugin , TypeSkeletonPlugin );
#endif

