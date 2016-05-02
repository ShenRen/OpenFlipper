/* ========================================================================= *
 *                                                                           *
 *                               OpenFlipper                                 *
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
 * ========================================================================= */

/*===========================================================================*\
 *
 *   $Revision$
 *   $Date$
 *
\*===========================================================================*/





//=============================================================================
//
//  MyTypes
//
//=============================================================================

#define CAMERAOBJECT_C

//== INCLUDES =================================================================

#include <OpenFlipper/common/Types.hh>
#include <ObjectTypes/Camera/Camera.hh>

//== DEFINES ==================================================================

//== TYPEDEFS =================================================================

//== CLASS DEFINITION =========================================================

/** Constructor for Camera Objects. This object class gets a Separator Node giving
*  the root node to which it should be connected. The camera is generated internally
*  and all nodes for visualization will be added below the scenegraph node.\n
*  You dont need to create an object of this type manually. Use
*  PluginFunctions::addCamera instead. ( see Types.hh::DataType )
*/
CameraObject::CameraObject( ) :
  BaseObjectData( ),
  cameraNode_(NULL)
{
  setDataType(DATA_CAMERA);
  init();
}

//=============================================================================


/**
 * Copy Constructor - generates a copy of the given object
 */
CameraObject::CameraObject(const CameraObject & _object) :
  BaseObjectData(_object)
{

    init(_object.cameraNode_);

    setName( name() );
}

/** Destructor for Camera Objects. The destructor deletes the Line and all
*  Scenegraph nodes associated with the Camera or the object.
*/
CameraObject::~CameraObject()
{
  // Delete the data attached to this object ( this will remove all perObject data)
  // Not the best way to do it but it will work.
  // This is only necessary if people use references to the camera below and
  // they do something with the camera in the destructor of their
  // perObjectData.
  deleteData();

  // No need to delete the scenegraph Nodes as this will be managed by baseplugin
  cameraNode_    = NULL;
}

/** Cleanup Function for Camera Objects. Deletes the contents of the whole object and
* calls CameraObject::init afterwards.
*/
void CameraObject::cleanup() {

  BaseObjectData::cleanup();

  cameraNode_   = NULL;

  setDataType(DATA_CAMERA);

  init();

}

/**
 * Generate a copy
 */
BaseObject* CameraObject::copy() {
    CameraObject* object = new CameraObject(*this);
    return dynamic_cast< BaseObject* >(object);
}

/** This function initalizes the camera object. It creates the scenegraph nodes.
*/
void CameraObject::init(CameraNode* _camera) {

  if ( materialNode() == NULL)
    std::cerr << "Error when creating Camera Object! materialNode is NULL!" << std::endl;

  cameraNode_ = new CameraNode( materialNode() , "NEW CameraNode");

  // TODO: Set initial position
  
//   if (_camera){
//     cameraNode_->setPosition( _camera->position(), _camera->normal() );
//     cameraNode_->setSize( _camera->xDirection().norm(), _camera->yDirection().norm() );
//   } else {
//     cameraNode_->setPosition( ACG::Vec3f(0.0, 0.0, 0.0), ACG::Vec3f(0.0, 1.0, 0.0) );
//     cameraNode_->setSize( 5.0, 5.0 );
//   }
}

// ===============================================================================
// Name/Path Handling
// ===============================================================================

/** Set the name of an object. All Scenegraph nodes are renamed too. It also calls
* BaseObjectData::setName.
*/
void CameraObject::setName( QString _name ) {
  BaseObjectData::setName(_name);

  std::string nodename = std::string("CameraNode for Camera "     + _name.toUtf8() );
  cameraNode_->name( nodename );
}

// ===============================================================================
// Visualization
// ===============================================================================

CameraNode* CameraObject::cameraNode() {
  return cameraNode_;
}

// ===============================================================================
// Object information
// ===============================================================================

/** Returns a string containing all information about the current object. This also
* includes the information provided by BaseObjectData::getObjectinfo
*
* @return String containing the object information
*/
QString CameraObject::getObjectinfo() {
  QString output;

  output += "========================================================================\n";
  output += BaseObjectData::getObjectinfo();

  if ( dataType( DATA_CAMERA ) )
    output += "Object Contains Camera : ";

  // TODO: Write correct data
//   ACG::Vec3f pos = cameraNode_->position();
//   ACG::Vec3f nor = cameraNode_->normal();
// 
//   output += " Position ( " + QString::number(pos[0]) + ", " + QString::number(pos[1]) + ", " + QString::number(pos[2]) + ")";
//   output += " Normal ( " + QString::number(nor[0]) + ", " + QString::number(nor[1]) + ", " + QString::number(nor[2]) + ")";

  output += "========================================================================\n";
  return output;
}

// ===============================================================================
// Picking
// ===============================================================================

/** Given an node index from PluginFunctions::scenegraphPick this function can be used to
* check if the cameraNode of the object has been picked.
*
* @param _node_idx Index of the picked camera node
* @return bool if the cameraNode of this object is the picking target.
*/
bool CameraObject::picked( uint _node_idx ) {
  return ( _node_idx == cameraNode_->id() );
}

void CameraObject::enablePicking( bool _enable ) {
  cameraNode_->enablePicking( _enable );
}

bool CameraObject::pickingEnabled() {
  return cameraNode_->pickingEnabled();
}

// ===============================================================================
// Update
// ===============================================================================


void CameraObject::update(UpdateType _type) {
  BaseObject::update(_type);
}


//=============================================================================

