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
//  Plugin Functions PolyMesh
//
//=============================================================================

#include <OpenFlipper/common/Types.hh>

#include <ObjectTypes/PolyMesh/PluginFunctionsPolyMesh.hh>


namespace PluginFunctions {


bool getSourceMeshes( std::vector<PolyMesh*>& _meshes  ) {
  _meshes.clear();

  for ( ObjectIterator o_it(PluginFunctions::ALL_OBJECTS,DATA_POLY_MESH) ; o_it != PluginFunctions::objectsEnd(); ++o_it) {
    if (! o_it->source() )
      continue;
    _meshes.push_back ( dynamic_cast< PolyMeshObject* >( *o_it )->mesh() );
  }

  return ( !_meshes.empty() );
}

bool getTargetMeshes( std::vector<PolyMesh*>& _meshes  ) {
  _meshes.clear();

  for ( ObjectIterator o_it(PluginFunctions::ALL_OBJECTS,DATA_POLY_MESH) ; o_it != PluginFunctions::objectsEnd(); ++o_it) {
    if (! o_it->target() )
      continue;
    if ( dynamic_cast< PolyMeshObject* >( *o_it )->mesh() )
      _meshes.push_back ( dynamic_cast< PolyMeshObject* >( *o_it )->mesh() );
  }

  return ( !_meshes.empty() );
}

bool getObject(  int _identifier , PolyMeshObject*& _object ) {

  if ( _identifier == BaseObject::NOOBJECT ) {
    _object = 0;
    return false;
  }

  // Get object by using the map accelerated plugin function
  BaseObjectData* object = 0;
  PluginFunctions::getObject(_identifier,object);

  _object = dynamic_cast< PolyMeshObject* >(object);
  return ( _object != 0 );
}

bool getMesh(  int _identifier , PolyMesh*& _mesh ) {

  if ( _identifier == BaseObject::NOOBJECT ) {
    _mesh = 0;
    return false;
  }

  // Get object by using the map accelerated plugin function
  BaseObjectData* object = 0;
  PluginFunctions::getObject(_identifier,object);

  // Unable to find object
  if ( object == 0)
    return false;

  PolyMeshObject* polyMeshObject = dynamic_cast< PolyMeshObject* > (object);

  // Object is not a triangle mesh
  if ( polyMeshObject == 0) {
    _mesh = 0;
    return false;
  }

  _mesh = polyMeshObject->mesh();
  return true;
}

PolyMesh* polyMesh( BaseObjectData* _object ) {
  
  if ( _object == 0 )
    return 0;

  if ( _object->dataType(DATA_POLY_MESH) ) {
    PolyMeshObject* object = dynamic_cast< PolyMeshObject* >(_object);
    return object->mesh();
  } else
    return NULL;
}

PolyMesh* polyMesh( int _identifier ) {
  PolyMeshObject* object = polyMeshObject(_identifier);

   if ( object == 0)
     return 0;
   else
     return object->mesh();
}

PolyMeshObject* polyMeshObject( BaseObjectData* _object ) {
  
  if ( _object == 0 )
    return 0;

  if ( ! _object->dataType(DATA_POLY_MESH) )
    return NULL;
  return dynamic_cast< PolyMeshObject* >( _object );
}

PolyMeshObject* polyMeshObject( int _objectId ) {
  
  if  (_objectId == BaseObject::NOOBJECT)
    return 0;
  
  // Get object by using the map accelerated plugin function
  BaseObjectData* object = 0;
  PluginFunctions::getObject(_objectId,object);
  
  if ( object == 0 )
    return 0;
  
  PolyMeshObject* meshObject = dynamic_cast< PolyMeshObject* >(object);
  
  return meshObject;
}

}
