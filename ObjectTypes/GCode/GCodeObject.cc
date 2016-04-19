 
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



//== INCLUDES =================================================================

#include "GCode.hh"
#include <OpenFlipper/common/Types.hh>


//== DEFINES ==================================================================

//== TYPEDEFS =================================================================

//== CLASS DEFINITION =========================================================

/** Constructor for PolyLine Objects. This object class gets a Separator Node giving
*  the root node to which it should be connected. The line is generated internally
*  and all nodes for visualization will be added below the scenegraph node.\n
*  You dont need to create an object of this type manually. Use
*  PluginFunctions::addPolyLine instead. ( see Types.hh::DataType )
*/
GCodeObject::GCodeObject() :
  BaseObjectData( ),
  gcode_(),
  node_(NULL)
{
  setDataType(DATA_GCODE);
  init();
}

//=============================================================================


/**
 * Copy Constructor - generates a copy of the given object
 */
GCodeObject::GCodeObject(const GCodeObject & _object) :
  BaseObjectData(_object)
{
    init(_object.gcode_);

    setName( name() );
}

/** Destructor for PolyLine Objects. The destructor deletes the Line and all
*  Scenegraph nodes associated with the Line or the object.
*/
GCodeObject::~GCodeObject()
{
  // Delete the data attached to this object ( this will remove all perObject data)
  // Not the best way to do it but it will work.
  // This is only necessary if people use references to the line below and
  // they do something with the polyline in the destructor of their
  // perObjectData.
  deleteData();

  // No need to delete the scenegraph Nodes as this will be managed by baseplugin
  node_    = NULL;
}

/** Cleanup Function for Line Objects. Deletes the contents of the whole object and
* calls PolyLineObject::init afterwards.
*/
void GCodeObject::cleanup() {
  // Delete the Line only, if this object contains a line
  gcode_.reset();

  BaseObjectData::cleanup();

  node_   = NULL;

  setDataType(DATA_GCODE);

  init();

}

/**
 * Generate a copy
 */
BaseObject* GCodeObject::copy() {
    GCodeObject* object = new GCodeObject(*this);
    return dynamic_cast< BaseObject* >(object);
}

/** This function initalizes the line object. It creates the scenegraph nodes,
*  the line.
*/
void GCodeObject::init(GCode::Shared _gcode) {

  if (!_gcode)
    gcode_.reset(new GCode());
  else
    gcode_ = _gcode;
  // request default properties (managed by the polyline collection)

  if ( materialNode() == NULL)
    std::cerr << "Error when creating Line Object! materialNode is NULL!" << std::endl;

  node_    = new ACG::SceneGraph::GCodeNode(gcode_, materialNode() , "NEW GCode");

  // Set default material of the polyLine
  materialNode()->set_random_color();
  materialNode()->set_line_width(3.0);
  materialNode()->set_point_size(8.0);
  materialNode()->set_round_points(true);

}

// ===============================================================================
// Name/Path Handling
// ===============================================================================

/** Set the name of an object. All Scenegraph nodes are renamed too. It also calls
* BaseObjectData::setName.
*/
void GCodeObject::setName( QString _name ) {
  BaseObjectData::setName(_name);

  std::string nodename = std::string("LineCollectionNode for LineCollection "     + _name.toUtf8() );
  node_->name( nodename );
}

// ===============================================================================
// Content
// ===============================================================================

/** Get a pointer to the objects line.
* @return Pointer to the line
*/
GCode::Shared GCodeObject::gcode() {
  return gcode_;
}


void GCodeObject::update(UpdateType _type) {
  node_->update();
}

// ===============================================================================
// Visualization
// ===============================================================================

ACG::SceneGraph::GCodeNode* GCodeObject::node() {
  return node_;
}

// ===============================================================================
// Object information
// ===============================================================================

/** Returns a string containing all information about the current object. This also
* includes the information provided by BaseObjectData::getObjectinfo
*
* @return String containing the object information
*/
QString GCodeObject::getObjectinfo() {
  QString output;

  output += "========================================================================\n";
  output += BaseObjectData::getObjectinfo();

  if ( dataType( DATA_GCODE ) )
    output += "Object Contains GCode : ";

  output += QString::number( gcode_->polyline()->n_edges() ) += " moves\n";

  output += "========================================================================\n";
  return output;
}

// ===============================================================================
// Picking
// ===============================================================================

/** Given an node index from PluginFunctions::scenegraphPick this function can be used to
* check if the polyLineNode of the object has been picked.
*
* @param _node_idx Index of the picked line node
* @return bool if the lineNode of this object is the picking target.
*/
bool GCodeObject::picked( uint _node_idx ) {
  return ( _node_idx == node_->id() );
}

void GCodeObject::enablePicking( bool _enable ) {
  node_->enablePicking( _enable );
}

bool GCodeObject::pickingEnabled() {
  return node_->pickingEnabled();
}

/// Refine picking on triangle meshes
ACG::Vec3d GCodeObject::refinePick(ACG::SceneGraph::PickTarget _pickTarget,
                             const ACG::Vec3d _hitPoint,
                             const ACG::Vec3d _start ,
                             const ACG::Vec3d _dir,
                             const unsigned int _targetIdx )
{
  if ( _pickTarget == ACG::SceneGraph::PICK_FACE) {
    return _hitPoint;
  }

  if ( _pickTarget == ACG::SceneGraph::PICK_EDGE) {
    return _hitPoint;
  }

  if ( _pickTarget == ACG::SceneGraph::PICK_VERTEX) {
    return _hitPoint;
  }

  return _hitPoint;
}

//=============================================================================

