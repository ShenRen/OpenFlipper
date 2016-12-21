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
//  Standard Functions
//
//=============================================================================

/**
 * \file PluginFunctionsPolyLineCollection.hh
 * This file contains functions which can be used by plugins to access Polyline Collections in the framework.
 */

#pragma once

#include <OpenFlipper/common/Types.hh>
#include "PolyLineCollectionTypes.hh"
#include "PolyLineCollectionObject.hh"

/** The Namespace PluginFunctions contains functions for all plugins. */
namespace PluginFunctions {


/** \brief Get a pointer to every Polyline Collection which is marked as a source.
 *
 * @param _polylines ( vector returning the source Polyline Collections )
 * @return false, if no PolylineCollection is selected as source
*/

OBJECTTYPEDLLEXPORT
bool getSourcePolylineCollections( std::vector<PolyLineCollection*>& _polylines  );

/** \brief Get a pointer to every Polyline Collection which is marked as a target.
 *
 * @param _polylines ( vector returning the target Polyline Collections )
 * @return false, if no PolylineCollection is selected as target
*/

OBJECTTYPEDLLEXPORT
bool getTargetPolylineCollections( std::vector<PolyLineCollection*>& _polylines  );


/** This functions returns the object with the given id if it is a Polyline Collection.
 * See get_object(  int _identifier , BaseObject*& _object ) for more details.
 */

OBJECTTYPEDLLEXPORT
bool getObject(  int _identifier , PolyLineCollectionObject*& _object );

/** \brief Get a polyLine Collection from an object.
 *
 * @param _object The object should be of type BaseDataObject. If the content is a Polyline Collection, a
 *                Polyline Collection will be returned. Otherwise a NULL pointer is returned.
 */

OBJECTTYPEDLLEXPORT
PolyLineCollection* polyLineCollection( BaseObjectData* _object );

/** \brief Cast an BaseObject to a Polyline Collection Object if possible
 *
 * @param _object The object should be of type BaseDataObject. If the content is a Polyline Collection, a
 *                a Polyline Collection Object is returned. Otherwise a NULL pointer is returned.
 */

OBJECTTYPEDLLEXPORT
PolyLineCollectionObject* polyLineCollectionObject( BaseObjectData* _object );

/** \brief Get an Polyline Collection Object from the given id If possible
*
* @param _objectId Id of the requested Object. If the content is a Polyline Collection,
*                  a Polyline Collection Object is returned. Otherwise a NULL pointer is returned.
*/

OBJECTTYPEDLLEXPORT
PolyLineCollectionObject* polyLineCollectionObject( int _objectId );

}

