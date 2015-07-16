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
 * \file PluginFunctionsTriangleMesh.hh
 * This file contains functions which can be used by plugins to access Triangle Meshes in the framework.
 */

//
#ifndef PLUGINFUNCTIONSTRIANGLEMESH_HH
#define PLUGINFUNCTIONSTRIANGLEMESH_HH

#include <OpenFlipper/common/Types.hh>
#include <ObjectTypes/TriangleMesh/TriangleMesh.hh>

/** The Namespace PluginFunctions contains functions for all plugins. These functions should be used to get the
 *  objects to work on or to set modes in the examiner widget. */
namespace PluginFunctions {

//=======================================
// Get Source/Target objects
/** @name Active Objects
* @{ */
//=======================================

/** \brief Get a pointer to every Triangle Mesh which is marked as a source mesh.
 *
 * @param _meshes ( vector returning the source meshes )
 * @return false, if no mesh is selected as source
*/
DLLEXPORT
bool getSourceMeshes( std::vector<TriMesh*>& _meshes  );

/** \brief Get a pointer to every Triangle Mesh which is marked as a target mesh.
 *
 * @param _meshes ( vector returning the target meshes )
 * @return false, if no mesh is selected as target
*/
DLLEXPORT
bool getTargetMeshes( std::vector<TriMesh*>& _meshes  );

/** @} */

//=======================================
// Get Objects by their identifier
    /** @name Identifier handling
    * @{ */
//=======================================

/** This functions returns the object with the given id if it is a TriMeshObject.
 * See get_object(  int _identifier , BaseObject*& _object ) for more details.
 */
DLLEXPORT
bool getObject(  int _identifier , TriMeshObject*& _object );


/** \brief Get the Triangle Mesh which has the given identifier.
 *
 *   Every loaded object has a unique identifier which is stored in the id field of the object container.
 *   Use this function to get the mesh which has this id. This can be used for a consistent mapping
 *   even if the data objects change during plugin operations (e.g. selection and main algorithm).\n
 *   This function checks, if the object requested contains a mesh.
 * @param _identifier Object id to search for
 * @param _mesh  returns the mesh
 * @return Mesh found?
 */
DLLEXPORT
bool getMesh(  int _identifier , TriMesh*& _mesh );

/** @} */

//=======================================
    /** @name Getting data from objects and casting between them
     * @{ */
//=======================================

/** \brief Get a triangle mesh from an object.
 *
 * @param _object The object should be of type BaseDataObject. If the content is a triangle Mesh, a
 *                triangle mesh will be returned. Otherwise a NULL pointer is returned.
 */
DLLEXPORT
TriMesh* triMesh( BaseObjectData* _object );

/** \brief Get a triangle mesh from an object id.
 *
 * @param _identifier Identifier of the object. If its a triangle mesh, the function will return the pointer to the mesh
 *                    otherwise 0
 */
DLLEXPORT
TriMesh* triMesh( int _identifier );

/** \brief Cast an BaseObject to a TriMeshObject if possible
 *
 * @param _object The object should be of type BaseDataObject. If the content is a triangle Mesh, a
 *                a TriMeshObject is returned. Otherwise a NULL pointer is returned.
 */
DLLEXPORT
TriMeshObject* triMeshObject( BaseObjectData* _object );

/** \brief Get an TriMeshObject from the given id If possible
*
* @param _objectId Id of the requested Object. 
* @return If the content is a volume, a pointer to the TriMeshObject is returned, otherwise a NULL pointer.
*/
DLLEXPORT
TriMeshObject* triMeshObject( int _objectId );

/** @} */
}

#endif //PLUGINFUNCTIONSTRIANGLEMESH_HH
