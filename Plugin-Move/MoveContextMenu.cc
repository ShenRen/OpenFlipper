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

#include "MovePlugin.hh"
#include <ACG/QtScenegraph/QtManipulatorNode.hh>

//***********************************************************************************

void MovePlugin::hideManipulator() {
  
  QVariant contextObject = contextActionHide_->data();
  int nodeId = contextObject.toInt();
  
  if ( nodeId == -1)
    return;
  
  // Get Node
  ACG::SceneGraph::BaseNode* node = ACG::SceneGraph::find_node( PluginFunctions::getSceneGraphRootNode(), nodeId );
  
  ACG::SceneGraph::QtTranslationManipulatorNode* mNode;
  mNode = dynamic_cast<ACG::SceneGraph::QtTranslationManipulatorNode*>(node);
  
  if(mNode == 0) {
    // Not a manipulator node
    return;
  }

  int objectId = mNode->getIdentifier();

  BaseObjectData* obj;
  if ( ! PluginFunctions::getObject(objectId,obj) )
    return;
  
  // Disconnect its signals to the plugin
  disconnect(obj->manipulatorNode() , SIGNAL(manipulatorMoved(QtTranslationManipulatorNode*,QMouseEvent*)),
             this , SLOT( manipulatorMoved(QtTranslationManipulatorNode*,QMouseEvent*)));
             
  disconnect(obj->manipulatorNode() , SIGNAL(positionChanged(QtTranslationManipulatorNode*)),
             this , SLOT( ManipulatorPositionChanged(QtTranslationManipulatorNode*)));    
  
  obj->manipPlaced(false);
  mNode->hide();
  
  emit nodeVisibilityChanged(obj->id());
  
}

void MovePlugin::showProps(){

  QVariant contextObject = contextAction_->data();
  int nodeId = contextObject.toInt();

  if (nodeId == -1)
    return;

  // Get Node
  ACG::SceneGraph::BaseNode* node = ACG::SceneGraph::find_node(PluginFunctions::getSceneGraphRootNode(), nodeId);

  ACG::SceneGraph::QtTranslationManipulatorNode* mNode;
  mNode = dynamic_cast<ACG::SceneGraph::QtTranslationManipulatorNode*>(node);

  if (mNode == 0) {
    // Not a manipulator node
    return;
  }

  int objectId = mNode->getIdentifier();

  BaseObjectData* obj;
  if (!PluginFunctions::getObject(objectId, obj))
    return;

  // Check if the widget has been created and show it.
  movePropsWidget* pW = getDialogWidget(obj);
  if (pW != 0) {
    pW->show();
    pW->raise();
    return;
  }
    
  pW = new movePropsWidget(obj->id());
  pW->setWindowTitle(QString((mNode->name()).c_str()));

  connect(pW->posButton, SIGNAL(clicked() ), this, SLOT(slotSetPosition()));
  connect(pW->axisAButton, SIGNAL(clicked() ), this, SLOT(slotToggleAxisA()));
  connect(pW->axisBButton, SIGNAL(clicked() ), this, SLOT(slotToggleAxisB()));
  connect(pW->dirButton, SIGNAL(clicked() ), this, SLOT(slotSetDirection()));

  connect(pW->transButton, SIGNAL(clicked() ), this, SLOT(slotTranslation()));
  connect(pW->rotButton, SIGNAL(clicked() ), this, SLOT(slotRotate()));
  connect(pW->scaleButton, SIGNAL(clicked() ), this, SLOT(slotScale()));

  connect(pW->projectTangentButton, SIGNAL(clicked() ), this, SLOT(slotProjectToTangentPlane()));
  connect(pW->moveManipToCOG, SIGNAL(clicked() ), this, SLOT(slotMoveManipToCOG()));

  connect(pW->selectionRadioButton, SIGNAL(clicked()), this, SLOT(slotEnableSelectionMode()));
  connect(pW->objectRadioButton, SIGNAL(clicked()), this, SLOT(slotEnableObjectMode()));

  // Values
  BaseObjectData* object;
  if (PluginFunctions::getObject(lastActiveManipulator_, object)) {
    if (object->manipulatorNode()->visible()) {
      const OpenMesh::Vec3d pos = object->manipulatorNode()->center();

      QString num;

      num = QString::number(pos[0]);
      pW->posx->setText(num);
      num = QString::number(pos[1]);
      pW->posy->setText(num);
      num = QString::number(pos[2]);
      pW->posz->setText(num);

      OpenMesh::Vec3d direction = object->manipulatorNode()->directionX();
      num = QString::number(direction[0]);
      pW->dirxx->setText(num);
      num = QString::number(direction[1]);
      pW->dirxy->setText(num);
      num = QString::number(direction[2]);
      pW->dirxz->setText(num);

      direction = object->manipulatorNode()->directionY();
      num = QString::number(direction[0]);
      pW->diryx->setText(num);
      num = QString::number(direction[1]);
      pW->diryy->setText(num);
      num = QString::number(direction[2]);
      pW->diryz->setText(num);

      direction = object->manipulatorNode()->directionZ();
      num = QString::number(direction[0]);
      pW->dirzx->setText(num);
      num = QString::number(direction[1]);
      pW->dirzy->setText(num);
      num = QString::number(direction[2]);
      pW->dirzz->setText(num);
    }
  }

  setPickModeProps(pW,PluginFunctions::pickMode());

  pW->show();
  propsWindows_.append(pW);
}
