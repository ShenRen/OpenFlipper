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
//  Plugin Functions for Cameras
//
//=============================================================================

#include <OpenFlipper/common/Types.hh>
#include "Camera.hh"

#include <OpenFlipper/BasePlugin/PluginFunctions.hh>

namespace PluginFunctions {

// ===============================================================================
// Get source cameras
// ===============================================================================


bool getSourceCameras( std::vector<CameraNode*>& _cameras  )
{
  _cameras.clear();

  for ( ObjectIterator o_it(PluginFunctions::SOURCE_OBJECTS,DATA_CAMERA ) ;
  o_it != PluginFunctions::objectsEnd(); ++o_it) {
    _cameras.push_back ( PluginFunctions::cameraNode( *o_it ) );
    if( _cameras.back() == NULL)
      std::cerr << "ERROR: Camera - get_source_cameras fatal error\n";
  }

  return ( !_cameras.empty() );
}


// ===============================================================================
// Get target cameras
// ===============================================================================


bool getTargetCameras( std::vector<CameraNode*>& _cameras  )
{
  _cameras.clear();

  for ( ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS,DATA_CAMERA ) ;
  o_it != PluginFunctions::objectsEnd(); ++o_it) {
    _cameras.push_back ( PluginFunctions::cameraNode( *o_it ) );
    if( _cameras.back() == NULL)
      std::cerr << "ERROR: Camera - getTargetPolylines fatal error\n";
  }

  return ( !_cameras.empty() );
}


// ===============================================================================
// Get objects
// ===============================================================================

bool getObject(  int _identifier , CameraObject*& _object ) {
  
  if ( _identifier == BaseObject::NOOBJECT ) {
    _object = 0;
    return false;
  }

  // Get object by using the map accelerated plugin function
  BaseObjectData* object = 0;
  PluginFunctions::getObject(_identifier,object);

  _object = dynamic_cast< CameraObject* >(object);
  return ( _object != 0 );
}


CameraObject* cameraObject( int _objectId ) {
  if ( _objectId == BaseObject::NOOBJECT ) {
    return 0;
  }
  
  // Get object by using the map accelerated plugin function
  BaseObjectData* object = 0;
  PluginFunctions::getObject(_objectId,object);

  CameraObject* cam  = dynamic_cast< CameraObject* >(object);
  return cam;
}


// ===============================================================================
// Getting data from objects and casting between them
// ===============================================================================

CameraNode* cameraNode( BaseObjectData* _object ) {
  if ( _object->dataType(DATA_CAMERA) ) {
    CameraObject* object = dynamic_cast< CameraObject* >(_object);
    return object->cameraNode();
  } else
    return 0;
}


CameraObject* cameraObject( BaseObjectData* _object ) {
  if ( ! _object->dataType(DATA_CAMERA) )
    return 0;
  return dynamic_cast< CameraObject* >( _object );
}


}
