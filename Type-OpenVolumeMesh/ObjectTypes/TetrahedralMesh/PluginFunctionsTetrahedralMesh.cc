/*===========================================================================*\
*                                                                            *
 *                              OpenFlipper                                   *
 *      Copyright (C) 2001-2014 by Computer Graphics Group, RWTH Aachen       *
 *                           www.openflipper.org                              *
 *                                                                            *
 *--------------------------------------------------------------------------- *
 *  This file is part of OpenFlipper.                                         *
 *                                                                            *
 *  OpenFlipper is free software: you can redistribute it and/or modify       *
 *  it under the terms of the GNU Lesser General Public License as            *
 *  published by the Free Software Foundation, either version 3 of            *
 *  the License, or (at your option) any later version with the               *
 *  following exceptions:                                                     *
 *                                                                            *
 *  If other files instantiate templates or use macros                        *
 *  or inline functions from this file, or you compile this file and          *
 *  link it with other files to produce an executable, this file does         *
 *  not by itself cause the resulting executable to be covered by the         *
 *  GNU Lesser General Public License. This exception does not however        *
 *  invalidate any other reasons why the executable file might be             *
 *  covered by the GNU Lesser General Public License.                         *
 *                                                                            *
 *  OpenFlipper is distributed in the hope that it will be useful,            *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 *  GNU Lesser General Public License for more details.                       *
 *                                                                            *
 *  You should have received a copy of the GNU LesserGeneral Public           *
 *  License along with OpenFlipper. If not,                                   *
 *  see <http://www.gnu.org/licenses/>.                                       *
 *                                                                            *
 \*===========================================================================*/

/*===========================================================================*\
 *                                                                           *
 *   $Revision$                                                          *
 *   $Date$                   *
 *   $LastChangedBy$                                                  *
 *                                                                           *
\*===========================================================================*/

#include <OpenFlipper/common/Types.hh>

#include <ObjectTypes/TetrahedralMesh/PluginFunctionsTetrahedralMesh.hh>
#include <OpenFlipper/BasePlugin/PluginFunctions.hh>

namespace PluginFunctions {

// ===============================================================================
// Get source meshes
// ===============================================================================


bool getSourceMeshes( std::vector<TetrahedralMesh*>& _meshes  )
{
  _meshes.clear();

  for ( ObjectIterator o_it(PluginFunctions::SOURCE_OBJECTS,DATA_TETRAHEDRAL_MESH) ;
    o_it != PluginFunctions::objectsEnd(); ++o_it) {
    _meshes.push_back ( PluginFunctions::tetrahedralMesh( *o_it ) );
    if( _meshes.back() == NULL)
      std::cerr << "ERROR: TetrahedralMesh getSourceMeshes fatal error\n";
  }

  return ( !_meshes.empty() );
}


// ===============================================================================
// Get target meshes
// ===============================================================================


bool getTargetMeshes( std::vector<TetrahedralMesh*>& _meshes  )
{
  _meshes.clear();

  for ( ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS,DATA_TETRAHEDRAL_MESH ) ;
    o_it != PluginFunctions::objectsEnd(); ++o_it) {
    _meshes.push_back ( PluginFunctions::tetrahedralMesh( *o_it ) );
    if( _meshes.back() == NULL)
      std::cerr << "ERROR: TetrahedralMesh getTargetMeshes fatal error\n";
  }

  return ( !_meshes.empty() );
}


// ===============================================================================
// Get objects
// ===============================================================================

bool getObject( int _identifier, TetrahedralMeshObject*& _object ) {

  if ( _identifier == BaseObject::NOOBJECT ) {
    _object = 0;
    return false;
  }

  // Get object by using the map accelerated plugin function
  BaseObjectData* object = 0;
  PluginFunctions::getObject(_identifier,object);

  _object = dynamic_cast< TetrahedralMeshObject* >(object);
  return ( _object != 0 );
}


// ===============================================================================
// Getting data from objects and casting between them
// ===============================================================================

TetrahedralMesh* tetrahedralMesh( BaseObjectData* _object ) {

  if ( _object->dataType(DATA_TETRAHEDRAL_MESH) ) {
    TetrahedralMeshObject* object = dynamic_cast< TetrahedralMeshObject* >(_object);
    return object->mesh();
  } else
    return 0;
}

TetrahedralMesh* tetrahedralMesh( int _identifier ) {
  TetrahedralMeshObject* object = tetrahedralMeshObject(_identifier);

   if ( object == 0)
     return 0;
   else
     return object->mesh();
}


TetrahedralMeshObject* tetrahedralMeshObject( BaseObjectData* _object ) {
  if ( ! _object->dataType(DATA_TETRAHEDRAL_MESH) )
    return 0;
  return dynamic_cast< TetrahedralMeshObject* >( _object );
}


TetrahedralMeshObject* tetrahedralMeshObject( int _identifier )
{
  TetrahedralMeshObject* pol_obj;
  if(getObject(_identifier, pol_obj))
    return pol_obj;
  else
    return 0;
}


}

