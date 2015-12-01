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

#include <ACG/Scenegraph/DrawModes.hh>
#include "ViewerProperties.hh"

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
  }

  void ViewerProperties::viewerId(int _id) {
    viewerId_ = _id;
    settingsSection_ = "Viewer" + QString::number(_id) + "/";
  }

  void ViewerProperties::drawMode(ACG::SceneGraph::DrawModes::DrawMode _mode) {
    currentDrawMode_ = _mode;
    emit updated();
    emit drawModeChanged(viewerId_);
  }

  ACG::SceneGraph::DrawModes::DrawMode ViewerProperties::drawMode() {
    return currentDrawMode_;
  }

  void ViewerProperties::snapshotCounter(const int _counter){
    snapshotCounter_ = _counter;
  }

  QString ViewerProperties::snapshotFileType() {
    return snapshotFileType_;
  }

  QString ViewerProperties::snapshotName() {
    return snapshotName_;
  }

  int ViewerProperties::snapshotCounter() {
    return snapshotCounter_++;
  }

  double ViewerProperties::wheelZoomFactor() {
    return wZoomFactor_;
  }

  double ViewerProperties::wheelZoomFactorShift() {
    return wZoomFactorShift_;
  }

  void ViewerProperties::wheelZoomFactor(double _factor) {
    wZoomFactor_ = _factor;
  }

  void ViewerProperties::wheelZoomFactorShift(double _factor) {
    wZoomFactorShift_ = _factor;
  }

  bool ViewerProperties::wheelInvert() {
    return wInvert_;
  }

  void ViewerProperties::wheelInvert(bool _invert) {
    wInvert_ = _invert;
  }

  bool ViewerProperties::isCCWFront(){
    return CCWFront_;
  }

  void ViewerProperties::ccwFront() {
    CCWFront_ = true;
    emit updated();
  }

  void ViewerProperties::cwFront() {
    CCWFront_ = false;
    emit updated();
  }

  ACG::Vec4f ViewerProperties::backgroundColor() {
    return backgroundColor_;
  }

  QRgb ViewerProperties::backgroundColorRgb(){
    QColor c;
    c.setRedF(  backgroundColor_[0]);
    c.setGreenF(backgroundColor_[1]);
    c.setBlueF( backgroundColor_[2]);
    c.setAlphaF(backgroundColor_[3]);
    return c.rgba();
  }

  QColor ViewerProperties::backgroundQColor(){
    QColor c;
    c.setRedF(  backgroundColor_[0]);
    c.setGreenF(backgroundColor_[1]);
    c.setBlueF( backgroundColor_[2]);
    c.setAlphaF(backgroundColor_[3]);
    return c;
  }

  void ViewerProperties::backgroundColor( ACG::Vec4f _color ) {
    backgroundColor_ = _color; emit updated();
  };

  void ViewerProperties::backgroundColor( QRgb _color ) {
    QColor c(_color);
    backgroundColor_[0] = c.redF();
    backgroundColor_[1] = c.greenF();
    backgroundColor_[2] = c.blueF();
    backgroundColor_[3] = c.alphaF();
    emit updated();
  }

  void ViewerProperties::backgroundColor( QColor _color ) {
    backgroundColor_[0] = _color.redF();
    backgroundColor_[1] = _color.greenF();
    backgroundColor_[2] = _color.blueF();
    backgroundColor_[3] = _color.alphaF();
    emit updated();
  }

  void ViewerProperties::lockUpdate()  {
    locked_++;
  }

  void ViewerProperties::unLockUpdate(){
    locked_-- ;
    if ( locked_ <0 ) {
      std::cerr << "More unlocks then locks" << std::endl;
      locked_ = 0;
    }
  }

  bool ViewerProperties::updateLocked() {
    return (locked_ != 0);
  }

  bool ViewerProperties::backFaceCulling() {
    return backFaceCulling_;
  }

  void ViewerProperties::backFaceCulling(bool _state ) {
    backFaceCulling_ = _state;
    emit updated();
  }

  void ViewerProperties::twoSidedLighting(bool _state ) {
    twoSidedLighting_ = _state;
    emit updated();
  }

  bool ViewerProperties::twoSidedLighting() {
    return twoSidedLighting_;
  }

  void ViewerProperties::multisampling(bool _state ) {
    multisampling_ = _state;
    emit updated();
  }

  bool ViewerProperties::multisampling() {
    return multisampling_;
  }

  void ViewerProperties::mipmapping(bool _state ) {
    glState_->allow_mipmapping(_state); mipmapping_ = _state; emit updated();
  }

  bool ViewerProperties::mipmapping() {
    return mipmapping_;
  }

  void ViewerProperties::animation(bool _state ) {
    animation_ = _state;
    emit updated();
  }

  bool ViewerProperties::animation() {
    return animation_;
  }

  ACG::GLState& ViewerProperties::glState() {
    return (*glState_);
  }

  const ACG::GLState& ViewerProperties::glState() const {
    return (*glState_);
  }

  void ViewerProperties::setglState(ACG::GLState* _glState) {
    glState_ = _glState;
  }

  void ViewerProperties::objectMarker (ViewObjectMarker* _marker) {
    objectMarker_ = _marker;
    emit updated();
  }

  ViewObjectMarker* ViewerProperties::objectMarker() {
    return objectMarker_;
  }

  double ViewerProperties::orthoWidth() {
    return orthoWidth_;
  }

  void ViewerProperties::orthoWidth(double _width){
    orthoWidth_ = _width;
    emit updated();
  }

  double ViewerProperties::nearPlane(){
    return nearPlane_;
  }

  void ViewerProperties::setPlanes( double _near, double _far  ) {
    nearPlane_ = _near;
    farPlane_ = _far;
    emit updated();
  }

  double ViewerProperties::farPlane(){
    return farPlane_;
  }

  ACG::Vec3d ViewerProperties::sceneCenter(){
    return sceneCenter_;
  }

  void ViewerProperties::sceneCenter(ACG::Vec3d _center){
    sceneCenter_ = _center;
    emit updated();
  }


  double ViewerProperties::sceneRadius() {
    return sceneRadius_;
  }

  void ViewerProperties::sceneRadius(double _radius ) {
    sceneRadius_ = _radius;
    emit updated();}


  ACG::Vec3d ViewerProperties::trackballCenter(){
    return trackballCenter_;
  }

  void ViewerProperties::trackballCenter(ACG::Vec3d _center){
    trackballCenter_ = _center;
    emit updated();
  }

  double ViewerProperties::trackballRadius() {
    return trackballRadius_;
  }

  void ViewerProperties::trackballRadius(double _radius ) {
    trackballRadius_ = _radius; emit updated();
  }

  void ViewerProperties::stereo(bool _stereo) {
    stereo_ = _stereo;
    emit updated();
  }

  bool ViewerProperties::stereo() {
    return stereo_;
  }

  CursorPainter* ViewerProperties::cursorPainter() {
    return cursorPainter_;
  }

  void ViewerProperties::cursorPainter( CursorPainter* _painter ) {
    cursorPainter_ = _painter;
  }

  ACG::Vec3d ViewerProperties::cursorPoint3D() {
    return cursorPoint3D_;
  }

  void ViewerProperties::cursorPoint3D(ACG::Vec3d _pos) {
    cursorPoint3D_ = _pos;
  }

  bool ViewerProperties::cursorPositionValid() {
    return cursorPositionValid_;
  }

  void ViewerProperties::cursorPositionValid(bool _valid) {
    cursorPositionValid_ = _valid;
  }



}

