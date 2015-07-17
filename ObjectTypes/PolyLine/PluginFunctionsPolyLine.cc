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
//  Plugin Functions for PolyLines
//
//=============================================================================

#include <OpenFlipper/common/Types.hh>
#include "PolyLine.hh"

#include "PluginFunctionsPolyLine.hh"
#include <OpenFlipper/BasePlugin/PluginFunctions.hh>

namespace PluginFunctions {

// ===============================================================================
// Get source polylines
// ===============================================================================


bool getSourcePolylines( std::vector<PolyLine*>& _polylines  )
{
  _polylines.clear();

  for ( ObjectIterator o_it(PluginFunctions::SOURCE_OBJECTS,DATA_POLY_LINE ) ;
	o_it != PluginFunctions::objectsEnd(); ++o_it) {
    _polylines.push_back ( PluginFunctions::polyLine( *o_it ) );
    if( _polylines.back() == 0)
      std::cerr << "ERROR: Polyine get_source_polylines fatal error\n";
  }

  return ( !_polylines.empty() );
}


// ===============================================================================
// Get target polylines
// ===============================================================================


bool getTargetPolylines( std::vector<PolyLine*>& _polylines  )
{
  _polylines.clear();

  for ( ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS,DATA_POLY_LINE ) ;
	o_it != PluginFunctions::objectsEnd(); ++o_it) {
    _polylines.push_back ( PluginFunctions::polyLine( *o_it ) );
    if( _polylines.back() == 0)
      std::cerr << "ERROR: Polyine getTargetPolylines fatal error\n";
  }

  return ( !_polylines.empty() );
}


// ===============================================================================
// Get objects
// ===============================================================================

bool getObject(  int _identifier , PolyLineObject*& _object ) {
  
  if ( _identifier == BaseObject::NOOBJECT ) {
    _object = 0;
    return false;
  }

  // Get object by using the map accelerated plugin function
  BaseObjectData* object = 0;
  PluginFunctions::getObject(_identifier,object);

  _object = dynamic_cast< PolyLineObject* >(object);
  return ( _object != 0 );
}


// ===============================================================================
// Getting data from objects and casting between them
// ===============================================================================

PolyLine* polyLine( BaseObjectData* _object ) {
  if ( _object->dataType(DATA_POLY_LINE) ) {
    PolyLineObject* object = dynamic_cast< PolyLineObject* >(_object);
    return object->line();
  } else
    return 0;
}


PolyLineObject* polyLineObject( BaseObjectData* _object ) {
  if ( ! _object->dataType(DATA_POLY_LINE) )
    return 0;
  return dynamic_cast< PolyLineObject* >( _object );
}

PolyLineObject* polyLineObject( int _objectId ) {
  
  if  (_objectId == BaseObject::NOOBJECT)
    return 0;
  
  // Get object by using the map accelerated plugin function
  BaseObjectData* object = 0;
  PluginFunctions::getObject(_objectId,object);
  
  if ( object == 0 )
    return 0;
  
  PolyLineObject* meshObject = dynamic_cast< PolyLineObject* >(object);
  
  return meshObject;
}


}
