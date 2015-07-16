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
 * \file PluginFunctionsSkeleton.hh
 * This file contains functions which can be used by plugins to access skeletons in the framework.
 */

//
#ifndef PLUGINFUNCTIONSSKELETON_HH
#define PLUGINFUNCTIONSSKELETON_HH

#include <OpenFlipper/common/Types.hh>

/** The Namespace PluginFunctions contains functions for all plugins. */
namespace PluginFunctions {

/** This functions returns the object with the given id if it is a SkeletonObject.
 * See get_object(  int _identifier , BaseObject*& _object ) for more details.
 */
DLLEXPORT
bool getObject(  int _identifier , SkeletonObject*& _object );

/** \brief Get a skeleton from an object.
 *
 * @param _object The object should be of type BaseDataObject. If the content is a skeleton, a
 *                skeleton will be returned. Otherwise a NULL pointer is returned.
 */
DLLEXPORT
Skeleton* skeleton( BaseObjectData* _object );

/** \brief Get a skeleton from an id.
 *
 * @param _identifier The skeleton is returned if the object with id is a skeleton object, otherwise 0.
 */
DLLEXPORT
Skeleton* skeleton(  int _identifier );

/** \brief Cast an BaseObject to a SkeletonObject if possible
 *
 * @param _object The object should be of type BaseDataObject. If the content is a polyLine, a
 *                a SkeletonObject is returned. Otherwise a NULL pointer is returned.
 */
DLLEXPORT
SkeletonObject* skeletonObject( BaseObjectData* _object );

/** \brief Get SkeletonObject with its id if possible
 *
 * @param _identifier The skeleton object if request was successfull or otherwise 0
 */
DLLEXPORT
SkeletonObject* skeletonObject( int _identifier );

}

#endif // PLUGINFUNCTIONSSKELETON_HH
