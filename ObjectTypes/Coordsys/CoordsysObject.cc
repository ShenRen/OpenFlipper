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
#include "Coordsys.hh"

//== DEFINES ==================================================================

//== TYPEDEFS =================================================================

//== CLASS DEFINITION =========================================================

/** Constructor for Coordinate system Objects. This object class gets a Separator Node giving
*  the root node to which it should be connected. The Coordinate system is generated internally
*  and all nodes for visualization will be added below the scenegraph node.\n
*  You don't need to create an object of this type manually. Use
*  the functions from LoadSaveInterface ( addEmpty() )
*/
CoordsysObject::CoordsysObject( ) :
  BaseObjectData( ),
  coordsysNode_(NULL)
{
  setDataType(DATA_COORDSYS);
  init();
}

//=============================================================================


/**
 * Copy Constructor - generates a copy of the given object
 */
CoordsysObject::CoordsysObject(const CoordsysObject & _object) :
  BaseObjectData(_object)
{

    init(_object.coordsysNode_);

    setName( name() );
}

/** Destructor for Coordsys Objects. The destructor deletes the Line and all
*  Scenegraph nodes associated with the Coordinate System or the object.
*/
CoordsysObject::~CoordsysObject()
{
  // Delete the data attached to this object ( this will remove all perObject data)
  // Not the best way to do it but it will work.
  // This is only necessary if people use references to the coordsys below and
  // they do something with the coordsys in the destructor of their
  // perObjectData.
  deleteData();

  // No need to delete the scenegraph Nodes as this will be managed by baseplugin
  coordsysNode_    = NULL;
}

/** Cleanup Function for Coordsys Objects. Deletes the contents of the whole object and
* calls CoordsysObject::init afterwards.
*/
void CoordsysObject::cleanup() {

  BaseObjectData::cleanup();

  coordsysNode_   = NULL;

  setDataType(DATA_COORDSYS);
  setTypeIcon(DATA_COORDSYS,"CoordsysType.png");

  init();

}

/**
 * Generate a copy
 */
BaseObject* CoordsysObject::copy() {
  CoordsysObject* object = new CoordsysObject(*this);
    return dynamic_cast< BaseObject* >(object);
}

/** This function initalizes the coordsys object. It creates the scenegraph nodes.
*/
void CoordsysObject::init(CoordsysNode* _coordsys) {

  if ( materialNode() == NULL)
    std::cerr << "Error when creating Coordsys Object! materialNode is NULL!" << std::endl;

  coordsysNode_ = new CoordsysNode( materialNode() , "NEW CoordsysNode");

  if (_coordsys){
    coordsysNode_->position( _coordsys->position() );
    coordsysNode_->size( _coordsys->size() );
  } else {
    coordsysNode_->position( ACG::Vec3d(0.0, 0.0, 0.0) );
    coordsysNode_->size( 1.0 );
  }
}

// ===============================================================================
// Name/Path Handling
// ===============================================================================

/** Set the name of an object. All Scenegraph nodes are renamed too. It also calls
* BaseObjectData::setName.
*/
void CoordsysObject::setName( QString _name ) {
  BaseObjectData::setName(_name);

  std::string nodename = std::string("CoordsysNode for Coordinate system "     + _name.toUtf8() );
  coordsysNode_->name( nodename );
}

// ===============================================================================
// Visualization
// ===============================================================================

CoordsysNode* CoordsysObject::coordsysNode() {
  return coordsysNode_;
}

// ===============================================================================
// Object information
// ===============================================================================

/** Returns a string containing all information about the current object. This also
* includes the information provided by BaseObjectData::getObjectinfo
*
* @return String containing the object information
*/
QString CoordsysObject::getObjectinfo() {
  QString output;

  output += "========================================================================\n";
  output += BaseObjectData::getObjectinfo();

  if ( dataType( DATA_COORDSYS ) )
    output += "Object Contains a coordinate system : ";

  ACG::Vec3d pos  = coordsysNode_->position();
  double     size = coordsysNode_->size();

  output += " Position ( " + QString::number(pos[0]) + ", " + QString::number(pos[1]) + ", " + QString::number(pos[2]) + ")";
  output += " Size ( "+ QString::number(size) + ")";

  output += "========================================================================\n";
  return output;
}

// ===============================================================================
// Picking
// ===============================================================================

/** Given an node index from PluginFunctions::scenegraphPick this function can be used to
* check if the coordsysNode of the object has been picked.
*
* @param _node_idx Index of the picked coordsys node
* @return bool if the coordsys node of this object is the picking target.
*/
bool CoordsysObject::picked( uint _node_idx ) {
  return ( _node_idx == coordsysNode_->id() );
}

void CoordsysObject::enablePicking( bool _enable ) {
  coordsysNode_->enablePicking( _enable );
}

bool CoordsysObject::pickingEnabled() {
  return coordsysNode_->pickingEnabled();
}

//=============================================================================

