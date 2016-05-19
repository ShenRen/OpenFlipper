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
 * \file PluginFunctionsPolyLine.hh
 * This file contains functions which can be used by plugins to access polylines in the framework.
 */

#ifndef PLUGIN_FUNCTIONS_POLYLINE_COLLECTION_HH
#define PLUGIN_FUNCTIONS_POLYLINE_COLLECTION_HH

#include <OpenFlipper/common/Types.hh>
#include "PolyLineCollectionTypes.hh"
#include "PolyLineCollectionObject.hh"

/** The Namespace PluginFunctions contains functions for all plugins. */
namespace PluginFunctions {


/** \brief Get a pointer to every Polyline which is marked as a source.
 *
 * @param _polylines ( vector returning the source polylines )
 * @return false, if no polyline is selected as source
*/

DLLEXPORT
bool getSourcePolylineCollections( std::vector<PolyLineCollection*>& _polylines  );

/** \brief Get a pointer to every Polyline which is marked as a target.
 *
 * @param _polylines ( vector returning the target polylines )
 * @return false, if no polyline is selected as target
*/

DLLEXPORT
bool getTargetPolylineCollections( std::vector<PolyLineCollection*>& _polylines  );


/** This functions returns the object with the given id if it is a PolyLineObject.
 * See get_object(  int _identifier , BaseObject*& _object ) for more details.
 */

DLLEXPORT
bool getObject(  int _identifier , PolyLineCollectionObject*& _object );

/** \brief Get a poly Line from an object.
 *
 * @param _object The object should be of type BaseDataObject. If the content is a poly Line, a
 *                poly line will be returned. Otherwise a NULL pointer is returned.
 */

DLLEXPORT
PolyLineCollection* polyLineCollection( BaseObjectData* _object );

/** \brief Cast an BaseObject to a PolyLineObject if possible
 *
 * @param _object The object should be of type BaseDataObject. If the content is a polyLine, a
 *                a PolyLineObject is returned. Otherwise a NULL pointer is returned.
 */

DLLEXPORT
PolyLineCollectionObject* polyLineCollectionObject( BaseObjectData* _object );

/** \brief Get an PolyLineObject from the given id If possible
*
* @param _objectId Id of the requested Object. If the content is a volume, a
*                  a PolyLineObject is returned. Otherwise a NULL pointer is returned.
*/

DLLEXPORT
PolyLineCollectionObject* polyLineCollectionObject( int _objectId );

}


#endif // PLUGIN_FUNCTIONS_POLYLINE_COLLECTION_HH