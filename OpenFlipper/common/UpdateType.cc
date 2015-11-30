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

#include "TypesInternal.hh"
#include "UpdateType.hh"
#include <QCoreApplication>
#include <OpenFlipper/common/GlobalOptions.hh>


/** This map maps an updateType id to an typeName
 */
static std::map< UpdateType, QString > updateTypeToString;

/** This map maps an updateType name to its id in the types vector
 */
static std::map< QString , size_t > stringToUpdateTypeInfo;

/** This map maps an updateType id to its id in the types vector
 */
static std::map< UpdateType , size_t > updateTypeToTypeInfo;

/** This field defines the start id for custom updatetypes.
*/
static UpdateType firstFreeID_(UPDATE_UNUSED);

UpdateType::UpdateType()
:type_(0)
{

}

UpdateType::UpdateType(const UpdateType& _type)
:type_(_type.type_)
{
  
}

UpdateType::UpdateType(UpdateTypeSet _set)
: type_(_set)
{
  
}

bool UpdateType::operator==(const UpdateType& _type) const {
  return ((type_ & _type.type_).any());
};

UpdateType 
UpdateType::operator|(const UpdateType& _type) const
{
  return (type_ | _type.type_);
}


UpdateType& 
UpdateType::operator|=(const UpdateType& _type)
{
  type_ |= _type.type_;

  return(*this);
}


/// Check if this update contains the given UpdateType
bool UpdateType::contains( const UpdateType& _type ) const {
  
  if ( type_ == UPDATE_ALL.type_ )
    return true;
  
  // Catch the specialization of updates
  if ( _type == UPDATE_SELECTION ) {
    if ( type_ == UPDATE_SELECTION_VERTICES.type_ || type_ == UPDATE_SELECTION_EDGES.type_ ||  type_ == UPDATE_SELECTION_HALFEDGES.type_ || type_ == UPDATE_SELECTION_FACES.type_ || type_ == UPDATE_SELECTION_KNOTS.type_ )
      return true;
  }  
  
  return ((type_ & _type.type_).any());
}

UpdateType& UpdateType::operator++() {
  if ( type_.count() != 1 ) {
    std::cerr << "Operator ++ for UpdateType which is not atomic!!" << std::endl;
  }
  
  type_ <<= 1;
  
  return (*this);
}

bool UpdateType::operator<( const UpdateType& _i ) const {
  return (type_.to_ulong() < _i.type_.to_ulong()); 
}

class UpdateTypeInfo {

  public:

  UpdateTypeInfo(UpdateType _type, QString _name, bool _needsScenegraphReset ) :
    type(_type),
    name(_name),
    resetNeeded(_needsScenegraphReset)
  {
  }

  /// The id of the UpdateType
  UpdateType type;

  /// The name of the UpdateType
  QString  name;

  /// is a sceneGraph reset needed for this update
  bool resetNeeded;
};

static std::vector< UpdateTypeInfo > updateTypes;


// functions -------------------------------------------------


void initializeUpdateTypes() {

  stringToUpdateTypeInfo["All"]      = updateTypes.size();
  updateTypeToTypeInfo[UPDATE_ALL]   = updateTypes.size();
  updateTypes.push_back( UpdateTypeInfo(UPDATE_ALL, "All", true) );

  stringToUpdateTypeInfo["Visibility"]      = updateTypes.size();
  updateTypeToTypeInfo[UPDATE_VISIBILITY]   = updateTypes.size();
  updateTypes.push_back( UpdateTypeInfo(UPDATE_VISIBILITY, "Visibility", true) );
  
  stringToUpdateTypeInfo["Geometry"]      = updateTypes.size();
  updateTypeToTypeInfo[UPDATE_GEOMETRY]   = updateTypes.size();
  updateTypes.push_back( UpdateTypeInfo(UPDATE_GEOMETRY, "Geometry", true) );
  
  stringToUpdateTypeInfo["Topology"]      = updateTypes.size();
  updateTypeToTypeInfo[UPDATE_TOPOLOGY]   = updateTypes.size();
  updateTypes.push_back( UpdateTypeInfo(UPDATE_TOPOLOGY, "Topology", true) );
  
  stringToUpdateTypeInfo["Selection"]      = updateTypes.size();
  updateTypeToTypeInfo[UPDATE_SELECTION]   = updateTypes.size();
  updateTypes.push_back( UpdateTypeInfo(UPDATE_SELECTION, "Selection", true) );
  
  stringToUpdateTypeInfo["VertexSelection"]      = updateTypes.size();
  updateTypeToTypeInfo[UPDATE_SELECTION_VERTICES]   = updateTypes.size();
  updateTypes.push_back( UpdateTypeInfo(UPDATE_SELECTION_VERTICES, "VertexSelection", true) );
  
  stringToUpdateTypeInfo["EdgeSelection"]      = updateTypes.size();
  updateTypeToTypeInfo[UPDATE_SELECTION_EDGES]   = updateTypes.size();
  updateTypes.push_back( UpdateTypeInfo(UPDATE_SELECTION_EDGES, "EdgeSelection", true) );
  
  stringToUpdateTypeInfo["HalfedgeSelection"]    = updateTypes.size();
  updateTypeToTypeInfo[UPDATE_SELECTION_HALFEDGES]   = updateTypes.size();
  updateTypes.push_back( UpdateTypeInfo(UPDATE_SELECTION_HALFEDGES, "HalfedgeSelection", true) );

  stringToUpdateTypeInfo["FaceSelection"]      = updateTypes.size();
  updateTypeToTypeInfo[UPDATE_SELECTION_FACES]   = updateTypes.size();
  updateTypes.push_back( UpdateTypeInfo(UPDATE_SELECTION_FACES, "FaceSelection", true) );
  
  stringToUpdateTypeInfo["KnotSelection"]      = updateTypes.size();
  updateTypeToTypeInfo[UPDATE_SELECTION_KNOTS] = updateTypes.size();
  updateTypes.push_back( UpdateTypeInfo(UPDATE_SELECTION_KNOTS, "KnotSelection", true) );

  stringToUpdateTypeInfo["Color"]      = updateTypes.size();
  updateTypeToTypeInfo[UPDATE_COLOR]   = updateTypes.size();
  updateTypes.push_back( UpdateTypeInfo(UPDATE_COLOR, "Color", true) );
  
  stringToUpdateTypeInfo["Texture"]      = updateTypes.size();
  updateTypeToTypeInfo[UPDATE_TEXTURE]   = updateTypes.size();
  updateTypes.push_back( UpdateTypeInfo(UPDATE_TEXTURE, "Texture", true) );

  stringToUpdateTypeInfo["State"]      = updateTypes.size();
  updateTypeToTypeInfo[UPDATE_STATE]   = updateTypes.size();
  updateTypes.push_back( UpdateTypeInfo(UPDATE_STATE, "State", true) );

  updateTypeToString[UPDATE_ALL]                = "All";
  updateTypeToString[UPDATE_VISIBILITY]         = "Visibility";
  updateTypeToString[UPDATE_GEOMETRY]           = "Geometry";
  updateTypeToString[UPDATE_TOPOLOGY]           = "Topology";
  updateTypeToString[UPDATE_SELECTION]          = "Selection";
  updateTypeToString[UPDATE_SELECTION_VERTICES] = "VertexSelection";
  updateTypeToString[UPDATE_SELECTION_EDGES]    = "EdgeSelection";
  updateTypeToString[UPDATE_SELECTION_HALFEDGES]= "HalfedgeSelection";
  updateTypeToString[UPDATE_SELECTION_FACES]    = "FaceSelection";
  updateTypeToString[UPDATE_SELECTION_KNOTS]    = "KnotSelection";
  updateTypeToString[UPDATE_COLOR]              = "Color";
  updateTypeToString[UPDATE_TEXTURE]            = "Texture";
  updateTypeToString[UPDATE_STATE]              = "State";
}

/// Adds a updateType and returns the id for the new type
UpdateType addUpdateType(QString _name, bool _resetNeeded) {

  //first check if it's already available
  std::map<QString, size_t>::iterator index = stringToUpdateTypeInfo.find( _name );

  if ( index != stringToUpdateTypeInfo.end() )
    return updateTypes[ index->second ].type;
  else {
  
    UpdateType type = firstFreeID_;

    stringToUpdateTypeInfo[ _name ] = updateTypes.size();
    updateTypeToTypeInfo[ type ] = updateTypes.size();
    updateTypes.push_back( UpdateTypeInfo(type, _name, _resetNeeded ) );

    updateTypeToString[type] = _name;

    ++firstFreeID_;
    return( type );
  }
}

/// Get the id of a type with given name
UpdateType updateType(QString _name) {

  std::map<QString, size_t>::iterator index = stringToUpdateTypeInfo.find( _name );

  if ( index != stringToUpdateTypeInfo.end() )
    return updateTypes[ index->second ].type;
  else {
    #ifdef DEBUG
      std::cerr << "Unknown UpdateType with name " << _name.toStdString() << std::endl;
    #endif
    return UPDATE_ALL;
  }
}

/// Get the name of a type with given id
QString updateTypeName(UpdateType _id) {

  std::map<UpdateType, QString>::iterator name = updateTypeToString.find(_id);

  if ( name != updateTypeToString.end() )
    return name->second;
  else {
    #ifdef DEBUG
    std::cerr << "Unable to retrieve updateTypeName" << std::endl;
    #endif
    return "Unknown";
  }
}

/// Return the number of registered types
size_t updateTypeCount() {
  return updateTypes.size();
}
