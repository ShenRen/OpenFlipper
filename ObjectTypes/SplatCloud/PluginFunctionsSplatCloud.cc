
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

//================================================================
//
//  Plugin Functions for SplatClouds
//
//================================================================


//== INCLUDES ====================================================


#include "PluginFunctionsSplatCloud.hh"

#include <OpenFlipper/BasePlugin/PluginFunctions.hh>


//== NAMESPACES ==================================================


namespace PluginFunctions {


//== IMPLEMENTATION ==============================================


//================================================================
// Get objects
//================================================================


bool getObject( int _identifier, SplatCloudObject *&_object )
{
  if( _identifier == BaseObject::NOOBJECT )
  {
    _object = 0;
    return false;
  }

  // Get object by using the map accelerated plugin function
  BaseObjectData* object = 0;
  PluginFunctions::getObject(_identifier,object);

  _object = dynamic_cast<SplatCloudObject *>( object );
  return (_object != 0);
}


//================================================================
// Getting data from objects and casting between them
//================================================================


ShaderNode* splatShaderNode( BaseObjectData *_object )
{
  if( !_object )
    return 0;

  if( !_object->dataType( DATA_SPLATCLOUD ) )
    return 0;

  SplatCloudObject *object = dynamic_cast<SplatCloudObject *>( _object );

  if ( object == 0 )
    return 0;

  return object->shaderNode();
}


//----------------------------------------------------------------


SplatCloudNode* splatCloudNode( BaseObjectData *_object )
{
  if( !_object )
    return 0;

  if( !_object->dataType( DATA_SPLATCLOUD ) )
    return 0;

  SplatCloudObject *object = dynamic_cast<SplatCloudObject *>( _object );

  if ( object == 0 )
    return 0;

  return object->splatCloudNode();
}


//----------------------------------------------------------------


SplatCloud* splatCloud( BaseObjectData *_object )
{
  if( !_object )
    return 0;

  if( !_object->dataType( DATA_SPLATCLOUD ) )
    return 0;

  SplatCloudObject *object = dynamic_cast<SplatCloudObject *>( _object );

  if ( object == 0 )
    return 0;

  return object->splatCloud();
}


//----------------------------------------------------------------


SplatCloudObject* splatCloudObject( BaseObjectData *_object )
{
  if( !_object )
    return 0;

  if( !_object->dataType( DATA_SPLATCLOUD ) )
    return 0;

  return dynamic_cast<SplatCloudObject *>( _object );
}


//----------------------------------------------------------------


SplatCloudObject* splatCloudObject(  int _objectId ) {

  if  (_objectId == BaseObject::NOOBJECT)
    return 0;

  // Get object by using the map accelerated plugin function
  BaseObjectData* object = 0;
  PluginFunctions::getObject(_objectId,object);

  if ( object == 0 )
    return 0;

  SplatCloudObject* splatCloudObject = dynamic_cast< SplatCloudObject* >(object);

  return splatCloudObject;
}


//================================================================


} // namespace PluginFunctions
