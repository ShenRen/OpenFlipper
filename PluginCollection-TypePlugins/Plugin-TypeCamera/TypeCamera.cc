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
        contextMenu_(0)
{

}

void TypeCameraPlugin::pluginsInitialized() {

  if ( OpenFlipper::Options::gui() ){
    contextMenu_ = new QMenu(tr("Rendering"));

    QAction* lastAction;

    lastAction = contextMenu_->addAction( tr("Show viewing frustum") );
    lastAction->setCheckable(true);
    lastAction->setChecked(false);
    lastAction->setToolTip(tr("Visualize cameras viewing frustum."));
    lastAction->setStatusTip( lastAction->toolTip() );

    // Add context menu
    emit addContextMenuItem(contextMenu_->menuAction(), DATA_CAMERA, CONTEXTOBJECTMENU);

    connect(contextMenu_, SIGNAL(triggered(QAction*)), this ,SLOT(contextMenuClicked(QAction*)));
  }
}

void TypeCameraPlugin::slotUpdateContextMenuObject(int _objectId) {

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

    object->target ( true );
    
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

