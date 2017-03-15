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
 *   $Author$                                                       *
 *   $Date$                   *
 *                                                                           *
\*===========================================================================*/


#include "TypeCamera.hh"

#include "OpenFlipper/BasePlugin/PluginFunctions.hh"
#include <OpenFlipper/common/GlobalOptions.hh>

TypeCameraPlugin::TypeCameraPlugin() :
        contextMenu_(0), showFrustumAction_(0)
{

}

void TypeCameraPlugin::pluginsInitialized() {

  if ( OpenFlipper::Options::gui() ){
    contextMenu_ = new QMenu(tr("CameraNode"));

    showFrustumAction_ = contextMenu_->addAction( tr("Show viewing frustum") );
    showFrustumAction_->setCheckable(true);
    showFrustumAction_->setChecked(false);
    showFrustumAction_->setToolTip(tr("Visualize cameras viewing frustum."));
    showFrustumAction_->setStatusTip( showFrustumAction_->toolTip() );

    QAction* flyAction = contextMenu_->addAction( tr("Fly to") );
    flyAction->setToolTip(tr("Fly viewer to the camera position."));
    flyAction->setStatusTip( flyAction->toolTip() );

    // Add context menu
    emit addContextMenuItem(contextMenu_->menuAction(), DATA_CAMERA, CONTEXTOBJECTMENU);

    connect(contextMenu_, SIGNAL(triggered(QAction*)), this ,SLOT(contextMenuClicked(QAction*)));
  }
}

void TypeCameraPlugin::slotUpdateContextMenu(int _objectId) {

  if (_objectId < 0)
    return;

  CameraObject* object;
  if (!PluginFunctions::getObject(_objectId, object))
    return;

  if (showFrustumAction_)
    showFrustumAction_->setChecked(object->cameraNode()->showFrustum());
}

void TypeCameraPlugin::contextMenuClicked(QAction* _contextAction) {

  QVariant contextObject = _contextAction->data();
  int objectId = contextObject.toInt();

  if (objectId == -1)
    return;

  CameraObject* object;
  if (!PluginFunctions::getObject(objectId, object))
    return;

  if (_contextAction->text() == tr("Show viewing frustum")) {

    // Set frustum flag to whether action is checked or not
    object->cameraNode()->showFrustum(_contextAction->isChecked());

    emit updatedObject(objectId, UPDATE_VISIBILITY);
  }
  else if (_contextAction->text() == tr("Fly to")) {

    // calculate camera position and view direction in world space

    ACG::GLMatrixd m = object->cameraNode()->modelview();
    ACG::GLMatrixd mInv = m;
    mInv.invert();

    ACG::Vec3d camPosWS(mInv(0,3), mInv(1,3), mInv(2,3));
    ACG::Vec3d camViewWS(mInv(0,2), mInv(1,2), mInv(2,2));

    ACG::Vec3d camCenterWS = camPosWS - camViewWS;

    // target up vector can't be specified unfortunately
    PluginFunctions::flyTo(camPosWS, camCenterWS, 500.0);
  }

}

bool TypeCameraPlugin::registerType() {
    
    addDataType ( "Camera",tr ( "Camera" ) );
    setTypeIcon ( "Camera", "camera.png" );
    return true;
}

int TypeCameraPlugin::addEmpty() {

    // new object data struct
    CameraObject* object = new CameraObject ();

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
    
    QString name = QString(tr("New Camera %1.cam").arg( object->id() ));

    // call the local function to update names
    object->setName ( name );

    object->update();

    object->show();

    emit log ( LOGINFO, object->getObjectinfo() );

    emit emptyObjectAdded ( object->id() );

    return object->id();
}

#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2 ( typecameraplugin , TypeCameraPlugin );
#endif

