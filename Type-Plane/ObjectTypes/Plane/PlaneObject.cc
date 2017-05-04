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
//  Plane Object
//
//=============================================================================

#define PLANEOBJECT_C

//== INCLUDES =================================================================

#include <OpenFlipper/common/Types.hh>
#include "Plane.hh"

//== DEFINES ==================================================================

//== TYPEDEFS =================================================================

//== CLASS DEFINITION =========================================================

/** Constructor for Plane Objects. This object class gets a Separator Node giving
*  the root node to which it should be connected. The plane is generated internally
*  and all nodes for visualization will be added below the scenegraph node.\n
*  You dont need to create an object of this type manually. Use
*  the functions from LoadSaveInterface ( addEmpty() )
*/
PlaneObject::PlaneObject( ) :
  BaseObjectData( ),
  planeNode_(NULL)
{
  setDataType(DATA_PLANE);
  init();
}

//=============================================================================


/**
 * Copy Constructor - generates a copy of the given object
 */
PlaneObject::PlaneObject(const PlaneObject & _object) :
  BaseObjectData(_object)
{

    init( &_object.plane_ );

    setName( name() );
}

/** Destructor for Plane Objects. The destructor deletes the Line and all
*  Scenegraph nodes associated with the Plane or the object.
*/
PlaneObject::~PlaneObject()
{
  // Delete the data attached to this object ( this will remove all perObject data)
  // Not the best way to do it but it will work.
  // This is only necessary if people use references to the plane below and
  // they do something with the plane in the destructor of their
  // perObjectData.
  deleteData();

  // No need to delete the scenegraph Nodes as this will be managed by baseplugin
  planeNode_    = NULL;
}

/** Cleanup Function for Plane Objects. Deletes the contents of the whole object and
* calls PlaneObject::init afterwards.
*/
void PlaneObject::cleanup() {

  BaseObjectData::cleanup();

  planeNode_   = NULL;

  setDataType(DATA_PLANE);
  setTypeIcon(DATA_PLANE,"PlaneType.png");

  init();

}

/**
 * Generate a copy
 */
BaseObject* PlaneObject::copy() {
    PlaneObject* object = new PlaneObject(*this);
    return dynamic_cast< BaseObject* >(object);
}

/** This function initializes the plane object. It creates the scenegraph nodes.
*/
void PlaneObject::init(const Plane* _plane) {

  if ( materialNode() == NULL) {
    std::cerr << "Error when creating Plane Object! materialNode is NULL!" << std::endl;
    return;
  }

  planeNode_ = new PlaneNode( plane_, materialNode() , "NEW PlaneNode" );

  if (_plane){
    plane_ = *_plane;
  } else {
    plane_.setPlane( ACG::Vec3d(0.0, 0.0, 0.0), ACG::Vec3d(0.0, 1.0, 0.0) );
    plane_.setSize( 5.0, 5.0 );
  }

  materialNode()->set_point_size(3.0);
}

// ===============================================================================
// Name/Path Handling
// ===============================================================================

/** Set the name of an object. All Scenegraph nodes are renamed too. It also calls
* BaseObjectData::setName.
*/
void PlaneObject::setName( QString _name ) {
  BaseObjectData::setName(_name);

  std::string nodename = std::string("PlaneNode for Plane "     + _name.toUtf8() );
  planeNode_->name( nodename );
}

// ===============================================================================
// Visualization
// ===============================================================================

PlaneNode* PlaneObject::planeNode() {
  return planeNode_;
}

// ===============================================================================
// Object information
// ===============================================================================

/** Returns a string containing all information about the current object. This also
* includes the information provided by BaseObjectData::getObjectinfo
*
* @return String containing the object information
*/
QString PlaneObject::getObjectinfo() {
  QString output;

  output += "========================================================================\n";
  output += BaseObjectData::getObjectinfo();

  if ( dataType( DATA_PLANE ) )
    output += "Object Contains Plane : ";

  ACG::Vec3d pos = planeNode_->position();
  ACG::Vec3d nor = planeNode_->normal();

  output += " Position ( " + QString::number(pos[0]) + ", " + QString::number(pos[1]) + ", " + QString::number(pos[2]) + ")";
  output += " Normal ( " + QString::number(nor[0]) + ", " + QString::number(nor[1]) + ", " + QString::number(nor[2]) + ")";

  output += "========================================================================\n";
  return output;
}

// ===============================================================================
// Content
// ===============================================================================

void PlaneObject::update(UpdateType _type) {
  if (planeNode_)
    planeNode_->update();
}

// ===============================================================================
// Data
// ===============================================================================

Plane& PlaneObject::plane() {
  return plane_;
}

void PlaneObject::plane(Plane _plane) {
  plane_ = _plane;
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
bool PlaneObject::picked( uint _node_idx ) {
  return ( _node_idx == planeNode_->id() );
}

void PlaneObject::enablePicking( bool _enable ) {
  planeNode_->enablePicking( _enable );
}

bool PlaneObject::pickingEnabled() {
  return planeNode_->pickingEnabled();
}

//=============================================================================

