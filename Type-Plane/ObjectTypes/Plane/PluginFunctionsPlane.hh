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
//  Standard Functions
//
//=============================================================================

/**
 * \file PluginFunctionsPlane.hh
 * This file contains functions which can be used by plugins to access planes in the framework.
 */

//
#ifndef PLUGINFUNCTIONSPLANE_HH
#define PLUGINFUNCTIONSPLANE_HH

#include <OpenFlipper/common/Types.hh>
#include <OpenFlipper/common/ObjectTypeDLLDefines.hh>

/** The Namespace PluginFunctions contains functions for all plugins. */
namespace PluginFunctions {

/** This functions returns the object with the given id if it is a PlaneObject.
 * See get_object(  int _identifier , BaseObject*& _object ) for more details.
 */
OBJECTTYPEDLLEXPORT
bool getObject(  int _identifier , PlaneObject*& _object );

/** \brief Get a PlaneNode from an object.
 *
 * @param _object The object should be of type BaseDataObject. If the content is a plane, a
 *                PlaneNode will be returned. Otherwise a NULL pointer is returned.
 */
OBJECTTYPEDLLEXPORT
PlaneNode* planeNode( BaseObjectData* _object );

/** \brief Get a Plane from an object.
 *
 * @param _object The object should be of type BaseDataObject. If the content is a plane, a
 *                Plane will be returned. Otherwise a NULL pointer is returned.
 */
OBJECTTYPEDLLEXPORT
Plane* plane( BaseObjectData* _object );

/** \brief Cast an BaseObject to a PlaneObject if possible
 *
 * @param _object The object should be of type BaseDataObject. If the content is a plane, a
 *                a PlaneObject is returned. Otherwise a NULL pointer is returned.
 */
OBJECTTYPEDLLEXPORT
PlaneObject* planeObject( BaseObjectData* _object );

/** \brief Get a PlaneObject from an object id if possible
*
* @param _objectId If the object is a plane, a PlaneObject is returned. Otherwise a NULL pointer is returned.
*/
OBJECTTYPEDLLEXPORT
PlaneObject* planeObject( int _objectId );

}

#endif // PLUGINFUNCTIONSPLANE_HH
