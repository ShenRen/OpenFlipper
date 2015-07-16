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
// Property storage Class for glWidgets
//
//=============================================================================

#include <OpenFlipper/common/GlobalDefines.hh>
#include <ACG/Scenegraph/DrawModes.hh>
#include "ViewerProperties.hh"
#include <iostream>

namespace Viewer {

  ViewerProperties::ViewerProperties(int _id):
    currentDrawMode_(ACG::SceneGraph::DrawModes::SOLID_FLAT_SHADED),
    snapshotName_("snap"),
    snapshotFileType_("png"),
    snapshotCounter_(0),
    wZoomFactor_(1.0),
    wZoomFactorShift_(0.2),
    wInvert_(false),
    CCWFront_(true),
    backgroundColor_(0.0f,0.0f,0.0f,1.0f),
    locked_(0),
    backFaceCulling_(false),
    twoSidedLighting_(false),
    multisampling_(true),
    mipmapping_(true),
    animation_(false),
    glState_(0),
    objectMarker_(0),
    currentViewingDirection_(0),
    rotationLocked_(false),
    orthoWidth_(2.0),
    nearPlane_(0.01),
    farPlane_(100.0),
    sceneCenter_(ACG::Vec3d( 0.0, 0.0, 0.0 )),
    sceneRadius_(1.0),
    trackballCenter_(ACG::Vec3d( 0.0, 0.0, 0.0 )),
    trackballRadius_(1.0),
    stereo_(false),
    cursorPainter_(0),
    cursorPoint3D_(ACG::Vec3d(0.0,0.0,0.0)),
    cursorPositionValid_(false),
    viewerId_(_id)
  {
    settingsSection_ = "Viewer" + QString::number(_id) + "/";
  }

  ViewerProperties::~ViewerProperties() {

    if ( glState_ != 0 )
      delete glState_;

  }

  void ViewerProperties::snapshotBaseFileName(const QString& _fname) {
    snapshotName_    = _fname;
    snapshotCounter_ = 0;
  }
  
  void ViewerProperties::snapshotFileType(const QString& _type) {
    snapshotFileType_  = _type.trimmed();
  }

  std::string ViewerProperties::pickMode(){

    std::string mode;
    emit getPickMode(mode);
    return mode;
  }

  void ViewerProperties::pickMode(const std::string& _name){
    emit setPickMode(_name);
  }

  Viewer::ActionMode ViewerProperties::actionMode(){
    Viewer::ActionMode am;
    emit getActionMode(am);
    return am;
  }

  void ViewerProperties::actionMode(const Viewer::ActionMode _am){
    emit setActionMode(_am);
  }

  int ViewerProperties::currentViewingDirection(){
    return currentViewingDirection_;
  }

  void ViewerProperties::currentViewingDirection(int _dir){
    currentViewingDirection_ = _dir;
  }
  
  bool ViewerProperties::rotationLocked(){
    return rotationLocked_;
  }

  void ViewerProperties::rotationLocked(bool _locked){
    rotationLocked_ = _locked;
  }

  int ViewerProperties::viewerId() {
    return viewerId_;
  };

  void ViewerProperties::viewerId(int _id) {
    viewerId_ = _id;
    settingsSection_ = "Viewer" + QString::number(_id) + "/";
  };

}

