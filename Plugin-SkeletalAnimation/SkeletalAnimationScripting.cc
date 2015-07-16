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
*   $LastChangedBy$                                                 *
*   $Date$                    *
*                                                                            *
\*===========================================================================*/

#include "SkeletalAnimationPlugin.hh"

void SkeletalAnimationPlugin::setDescriptions(){
  
  emit setSlotDescription("attachSkin(int,int)",tr("Attaches a mesh to a skeleton as skin. Each are identified by their objectIds"),
                          QString(tr("SkeletonObjectId,SkinObjectId")).split(","),
                          QString(tr("id of the skeleton object, id of the skin object")).split(","));

  emit setSlotDescription("detachSkin(int,int)",tr("Remove a specific skin from the skeleton"),
                          QString(tr("SkeletonObjectId,SkinObjectId")).split(","),
                          QString(tr("id of the skeleton object, id of the skin object")).split(","));

  emit setSlotDescription("clearSkins(int)",tr("Remove all skins from a skeleton"),
                          QStringList(tr("SkeletonObjectId")), QStringList(tr("ID of the skeleton object")));
}

bool SkeletalAnimationPlugin::attachSkin(int skeletonId, int skinId) {
  BaseObjectData* skeleton = NULL;
  if (!PluginFunctions::getObject(skeletonId, skeleton)) {
    emit log(LOGERR, "Could not retrieve skeleton");
    return false;
  }
  
  BaseObjectData* skin = NULL;
  if (!PluginFunctions::getObject(skinId, skin)) {
    emit log(LOGERR, "Could not retrieve skin");
    return false;
  }
  
  attachSkin(skin, skeleton);

  return true;
}

bool SkeletalAnimationPlugin::detachSkin(int skeletonId, int skinId) {
  BaseObjectData* skeleton = NULL;
  if (!PluginFunctions::getObject(skeletonId, skeleton)) {
    emit log(LOGERR, "Could not retrieve skeleton");
    return false;
  }
  
  BaseObjectData* skin = NULL;
  if (!PluginFunctions::getObject(skinId, skin)) {
    emit log(LOGERR, "Could not retrieve skin");
    return false;
  }

  detachSkin(skin, skeleton);

  return true;
}

bool SkeletalAnimationPlugin::clearSkins(int skeletonId) {
  BaseObjectData* skeleton = NULL;
  if (!PluginFunctions::getObject(skeletonId, skeleton)) {
    emit log(LOGERR, "Could not retrieve skeleton");
    return false;
  }

  clearSkins(skeleton);

  return true;
}

void SkeletalAnimationPlugin::updateSkins(){
  
  for (unsigned int i=0; i < activeSkeletons_.size(); i++){

    //get active skeleton
    BaseObjectData* baseObject = 0;
    PluginFunctions::getObject(activeSkeletons_[i], baseObject);

    if ( baseObject == 0 )
      continue;

    SkeletonObject* skeletonObject = dynamic_cast<SkeletonObject*>(baseObject);

    AnimationHandle hAni = skeletonObject->skeletonNode()->activePose();
    UpdateSkins(skeletonObject, hAni);
  }
}