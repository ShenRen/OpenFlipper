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
//  MyTypes
//
//=============================================================================

#define POLYLINECOLLECTIONOBJECT_C

//== INCLUDES =================================================================

#include <OpenFlipper/common/Types.hh>
#include "PolyLineCollection.hh"
#include "PolyLineCollectionObject.hh"

//== DEFINES ==================================================================

//== TYPEDEFS =================================================================

//== CLASS DEFINITION =========================================================

/** Constructor for PolyLine Collection Objects. This object class gets a Separator Node giving
*  the root node to which it should be connected. The collection is generated internally
*  and all nodes for visualization will be added below the scenegraph node.\n
*  You don't need to create an object of this type manually. Use
*  PluginFunctions::addPolyLineCollection instead. ( see Types.hh::DataType )
*/
PolyLineCollectionObject::PolyLineCollectionObject() :
  BaseObjectData( ),
  collection_(NULL),
  collectionNode_(NULL)
{
  setDataType(DATA_POLY_LINE_COLLECTION);
  init();
}

//=============================================================================


/**
 * Copy Constructor - generates a copy of the given object
 */
PolyLineCollectionObject::PolyLineCollectionObject(const PolyLineCollectionObject & _object) :
  BaseObjectData(_object)
{
    init(_object.collection_);

    setName( name() );
}

/** Destructor for PolyLineCollection Objects. The destructor deletes the Collection and all
*  Scenegraph nodes associated with the Collection or the object.
*/
PolyLineCollectionObject::~PolyLineCollectionObject()
{
  // Delete the data attached to this object ( this will remove all perObject data)
  // Not the best way to do it but it will work.
  // This is only necessary if people use references to the line below and
  // they do something with the polyline in the destructor of their
  // perObjectData.
  deleteData();

  // Delete the collection only, if this object contains a collection
  if ( collection_ != NULL)  {
    delete collection_;
    collection_ = NULL;
  } else {
    std::cerr << "Destructor error : Line Collection already deleted" << std::endl;
  }

  // No need to delete the scenegraph Nodes as this will be managed by BasePlugin
  collectionNode_    = NULL;
}

/** Cleanup Function for Collection Objects. Deletes the contents of the whole object and
* calls PolyLineCollection::init afterwards.
*/
void PolyLineCollectionObject::cleanup() {

  // Delete the Collection only, if this object contains a Collection
  if ( collection_ != NULL)  {
    delete collection_;
    collection_ = NULL;
  } else {
    std::cerr << "Cleanup error : Line already deleted" << std::endl;
  }

  BaseObjectData::cleanup();

  collectionNode_   = NULL;

  setDataType(DATA_POLY_LINE_COLLECTION);

  init();

}

/**
 * Generate a copy
 */
BaseObject* PolyLineCollectionObject::copy() {
    PolyLineCollectionObject* object = new PolyLineCollectionObject(*this);
    return dynamic_cast< BaseObject* >(object);
}

/** This function initializes the Collection object. It creates the scenegraph nodes,
*  the Collection.
*/
void PolyLineCollectionObject::init(PolyLineCollection* _collection) {

  if (_collection == 0)
    collection_ = new PolyLineCollection();
  else
    collection_ = new PolyLineCollection(*_collection);

  // request default properties (managed by the polyline collection)

  if ( materialNode() == NULL)
    std::cerr << "Error when creating Line Object! materialNode is NULL!" << std::endl;

  collectionNode_    = new ACG::SceneGraph::PolyLineCollectionNodeT< PolyLineCollection >(*collection_, materialNode() , "NEW PolyLineCollection");

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
void PolyLineCollectionObject::setName( QString _name ) {
  BaseObjectData::setName(_name);

  std::string nodename = std::string("PolyLineCollectionNode for PolyLineCollection "     + _name.toUtf8() );
  collectionNode_->name( nodename );
}

// ===============================================================================
// Content
// ===============================================================================

/** Get a pointer to the objects Collection.
* @return Pointer to the Collection
*/
PolyLineCollection* PolyLineCollectionObject::collection() {
  return collection_;
}


void PolyLineCollectionObject::update(UpdateType _type) {
  if(_type == UPDATE_COLOR || _type == UPDATE_ALL){
      collectionNode_->resetVBO();
  }

  collectionNode_->update();
}

// ===============================================================================
// Visualization
// ===============================================================================

ACG::SceneGraph::PolyLineCollectionNodeT< PolyLineCollection >* PolyLineCollectionObject::collectionNode() {
  return collectionNode_;
}

// ===============================================================================
// Object information
// ===============================================================================

/** Returns a string containing all information about the current object. This also
* includes the information provided by BaseObjectData::getObjectinfo
*
* @return String containing the object information
*/
QString PolyLineCollectionObject::getObjectinfo() {
  QString output;

  output += "========================================================================\n";
  output += BaseObjectData::getObjectinfo();

  if ( dataType( DATA_POLY_LINE_COLLECTION ) )
    output += "Object Contains PolyLineCollection : ";

  output += QString::number( collection()->n_polylines() ) + " polylines, ";

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
bool PolyLineCollectionObject::picked( uint _node_idx ) {
  return ( _node_idx == collectionNode_->id() );
}

void PolyLineCollectionObject::enablePicking( bool _enable ) {
  collectionNode_->enablePicking( _enable );
}

bool PolyLineCollectionObject::pickingEnabled() {
  return collectionNode_->pickingEnabled();
}

/// Refine picking on poly line collections (!TODO)
ACG::Vec3d PolyLineCollectionObject::refinePick(ACG::SceneGraph::PickTarget _pickTarget,
                                                const ACG::Vec3d _hitPoint,
                                                const ACG::Vec3d _start ,
                                                const ACG::Vec3d _dir,
                                                const unsigned int _targetIdx )
{
  return _hitPoint;
}

//=============================================================================

