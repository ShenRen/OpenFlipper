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
//  Sphere Object
//
//=============================================================================

#define SPHEREOBJECT_C

//== INCLUDES =================================================================

#include <OpenFlipper/common/Types.hh>
#include "Sphere.hh"

//== DEFINES ==================================================================

//== TYPEDEFS =================================================================

//== CLASS DEFINITION =========================================================

/** Constructor for Sphere Objects. This object class gets a Separator Node giving
*  the root node to which it should be connected. The plane is generated internally
*  and all nodes for visualization will be added below the scenegraph node.\n
*  You dont need to create an object of this type manually. Use
*  PluginFunctions::addSphere instead. ( see Types.hh::DataType )
*/
SphereObject::SphereObject( ) :
  BaseObjectData( ),
  sphereNode_(NULL)
{
  setDataType(DATA_SPHERE);
  init();
}

//=============================================================================


/**
 * Copy Constructor - generates a copy of the given object
 */
SphereObject::SphereObject(const SphereObject & _object) :
  BaseObjectData(_object)
{

    init(_object.sphereNode_);

    setName( name() );
}

/** Destructor for Sphere Objects. The destructor deletes the Line and all
*  Scenegraph nodes associated with the Sphere or the object.
*/
SphereObject::~SphereObject()
{
  // Delete the data attached to this object ( this will remove all perObject data)
  // Not the best way to do it but it will work.
  // This is only necessary if people use references to the plane below and
  // they do something with the plane in the destructor of their
  // perObjectData.
  deleteData();

  // No need to delete the scenegraph Nodes as this will be managed by baseplugin
  sphereNode_    = NULL;
}

/** Cleanup Function for Sphere Objects. Deletes the contents of the whole object and
* calls SphereObject::init afterwards.
*/
void SphereObject::cleanup() {

  BaseObjectData::cleanup();

  sphereNode_   = NULL;

  setDataType(DATA_SPHERE);

  init();

}

/**
 * Generate a copy
 */
BaseObject* SphereObject::copy() {
    SphereObject* object = new SphereObject(*this);
    return dynamic_cast< BaseObject* >(object);
}

/** This function initalizes the plane object. It creates the scenegraph nodes.
*/
void SphereObject::init(SphereNode* _sphere) {

  if ( materialNode() == NULL)
    std::cerr << "Error when creating Sphere Object! materialNode is NULL!" << std::endl;

  sphereNode_ = new SphereNode( SphereNode::SPHERE, materialNode() , "NEW SphereNode");
  

  if (_sphere){
    sphereNode_->get_primitive(0).position = _sphere->get_primitive(0).position;
    sphereNode_->get_primitive(0).color    = _sphere->get_primitive(0).color;
    sphereNode_->get_primitive(0).size     = _sphere->get_primitive(0).size;
    sphereNode_->get_primitive(0).slices   = _sphere->get_primitive(0).slices;
    sphereNode_->get_primitive(0).stacks   = _sphere->get_primitive(0).stacks;
    sphereNode_->setColorInternal(false);
  } else {
    sphereNode_->get_primitive(0).position = ACG::Vec3f(0.0, 0.0, 0.0);
    sphereNode_->get_primitive(0).size     = 1.0;
    sphereNode_->get_primitive(0).slices   = 40;
    sphereNode_->get_primitive(0).stacks   = 40;
    sphereNode_->get_primitive(0).color    = ACG::Vec4f(0.5, 0.5, 0.5, 1.0);
    sphereNode_->setColorInternal(false);
  }
  
}

// ===============================================================================
// Name/Path Handling
// ===============================================================================

/** Set the name of an object. All Scenegraph nodes are renamed too. It also calls
* BaseObjectData::setName.
*/
void SphereObject::setName( QString _name ) {
  BaseObjectData::setName(_name);

  std::string nodename = std::string("SphereNode for Sphere "     + _name.toUtf8() );
  sphereNode_->name( nodename );
}

// ===============================================================================
// Visualization
// ===============================================================================

SphereNode* SphereObject::sphereNode() {
  return sphereNode_;
}

// ===============================================================================
// Object information
// ===============================================================================

/** Returns a string containing all information about the current object. This also
* includes the information provided by BaseObjectData::getObjectinfo
*
* @return String containing the object information
*/
QString SphereObject::getObjectinfo() {
  QString output;

  output += "========================================================================\n";
  output += BaseObjectData::getObjectinfo();

  if ( dataType( DATA_SPHERE ) )
    output += "Object Contains Sphere : ";

  ACG::Vec3d  pos  = sphereNode_->get_primitive(0).position;
  double      size = sphereNode_->get_primitive(0).size;

  output += " Position ( " + QString::number(pos[0]) + ", " + QString::number(pos[1]) + ", " + QString::number(pos[2]) + ")";
  output += " Size ( " + QString::number(size) + ")";

  output += "========================================================================\n";
  return output;
}

// ===============================================================================
// Picking
// ===============================================================================

/** Given an node index from PluginFunctions::scenegraphPick this function can be used to
* check if the planeNode of the object has been picked.
*
* @param _node_idx Index of the picked plane node
* @return bool if the planeNode of this object is the picking target.
*/
bool SphereObject::picked( uint _node_idx ) {
  return ( _node_idx == sphereNode_->id() );
}

void SphereObject::enablePicking( bool _enable ) {
  sphereNode_->enablePicking( _enable );
}

bool SphereObject::pickingEnabled() {
  return sphereNode_->pickingEnabled();
}

// ===============================================================================
// Update
// ===============================================================================
/// Update the whole Object (Selection,Topology,...)
void SphereObject::update(UpdateType _type) {
  BaseObject::update(_type);
}

//=============================================================================

