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
//  Standard Functions
//
//================================================================


/**
 * \file PluginFunctionsSplatCloud.hh
 * This file contains functions which can be used by plugins to access splat clouds in the framework
 */


#ifndef PLUGINFUNCTIONSSPLATCLOUD_HH
#define PLUGINFUNCTIONSSPLATCLOUD_HH


//== INCLUDES ====================================================


#include "SplatCloud.hh"

#include <OpenFlipper/common/Types.hh>


//== NAMESPACES ==================================================


/** The Namespace PluginFunctions contains functions for all plugins. */
namespace PluginFunctions {


//== DEFINITIONS =================================================


/** This functions returns the object with the given id if it is a SplatCloudObject.
 * See get_object(  int _identifier , BaseObject*& _object ) for more details.
 */
DLLEXPORT
bool getObject( int _identifier, SplatCloudObject *&_object );


/** \brief Get a ShaderNode from an object.
 *
 * @param _object The object should be of type BaseDataObject. If the content is a SplatCloud, a
 *                ShaderNode will be returned. Otherwise a NULL pointer is returned.
 */
DLLEXPORT
ShaderNode* splatShaderNode( BaseObjectData *_object );


/** \brief Get a SplatCloudNode from an object.
 *
 * @param _object The object should be of type BaseDataObject. If the content is a SplatCloud, a
 *                SplatCloudNode will be returned. Otherwise a NULL pointer is returned.
 */
DLLEXPORT
SplatCloudNode* splatCloudNode( BaseObjectData *_object );


/** \brief Get a SplatCloud from an object.
 *
 * @param _object The object should be of type BaseDataObject. If the content is a SplatCloud, a
 *                SplatCloud will be returned. Otherwise a NULL pointer is returned.
 */
DLLEXPORT
SplatCloud* splatCloud( BaseObjectData *_object );


/** \brief Cast an SplatCloudObject to a SplatCloudObject if possible
 *
 * @param _object The object should be of type BaseDataObject. If the content is a SplatCloud, a
 *                SplatCloudObject is returned. Otherwise a NULL pointer is returned.
 */
DLLEXPORT
SplatCloudObject* splatCloudObject( BaseObjectData *_object );


/** \brief Get an SplatCloudObject by its id
 *
 * @param _objectId Id of the object. If the object exists and is a SplatCloud, a
 *                SplatCloudObject is returned. Otherwise a NULL pointer is returned.
 */
DLLEXPORT
SplatCloudObject* splatCloudObject(  int _objectId );


//================================================================


} // namespace PluginFunctions


//================================================================


#endif // PLUGINFUNCTIONSSPLATCLOUD_HH
