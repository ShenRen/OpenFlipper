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
 *   $Revision: 13489 $                                                       *
 *   $LastChangedBy: kremer $                                                 *
 *   $Date: 2012-01-25 12:30:09 +0100 (Mi, 25 Jan 2012) $                    *
 *                                                                            *
 \*===========================================================================*/

#include <OpenFlipper/common/Types.hh>

#include <ObjectTypes/PolyhedralMesh/PluginFunctionsPolyhedralMesh.hh>

namespace PluginFunctions {

// ===============================================================================
// Get source meshes
// ===============================================================================


bool getSourceMeshes( std::vector<PolyhedralMesh*>& _meshes  )
{
  _meshes.clear();

  for ( ObjectIterator o_it(PluginFunctions::SOURCE_OBJECTS,DATA_POLYHEDRAL_MESH) ;
    o_it != PluginFunctions::objectsEnd(); ++o_it) {
    _meshes.push_back ( PluginFunctions::polyhedralMesh( *o_it ) );
    if( _meshes.back() == NULL)
      std::cerr << "ERROR: PolyhedralMesh getSourceMeshes fatal error\n";
  }

  return ( !_meshes.empty() );
}


// ===============================================================================
// Get target meshes
// ===============================================================================


bool getTargetMeshes( std::vector<PolyhedralMesh*>& _meshes  )
{
  _meshes.clear();

  for ( ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS,DATA_POLYHEDRAL_MESH ) ;
    o_it != PluginFunctions::objectsEnd(); ++o_it) {
    _meshes.push_back ( PluginFunctions::polyhedralMesh( *o_it ) );
    if( _meshes.back() == NULL)
      std::cerr << "ERROR: PolyhedralMesh getTargetMeshes fatal error\n";
  }

  return ( !_meshes.empty() );
}


// ===============================================================================
// Get objects
// ===============================================================================

bool getObject( int _identifier, PolyhedralMeshObject*& _object ) {

  if ( _identifier == BaseObject::NOOBJECT ) {
    _object = 0;
    return false;
  }

  // Get object by using the map accelerated plugin function
  BaseObjectData* object = 0;
  PluginFunctions::getObject(_identifier,object);

  _object = dynamic_cast< PolyhedralMeshObject* >(object);
  return ( _object != 0 );
}


// ===============================================================================
// Getting data from objects and casting between them
// ===============================================================================

PolyhedralMesh* polyhedralMesh( BaseObjectData* _object ) {

  if ( _object->dataType(DATA_POLYHEDRAL_MESH) ) {
    PolyhedralMeshObject* object = dynamic_cast< PolyhedralMeshObject* >(_object);
    return object->mesh();
  } else
    return 0;
}

PolyhedralMesh* polyhedralMesh( int _identifier ) {
  PolyhedralMeshObject* object = polyhedralMeshObject(_identifier);

   if ( object == 0)
     return 0;
   else
     return object->mesh();
}


PolyhedralMeshObject* polyhedralMeshObject( BaseObjectData* _object ) {
  if ( ! _object->dataType(DATA_POLYHEDRAL_MESH) )
    return 0;
  return dynamic_cast< PolyhedralMeshObject* >( _object );
}


PolyhedralMeshObject* polyhedralMeshObject( int _identifier )
{
  PolyhedralMeshObject* pol_obj;
  if(getObject(_identifier, pol_obj))
    return pol_obj;
  else
    return 0;
}


}
