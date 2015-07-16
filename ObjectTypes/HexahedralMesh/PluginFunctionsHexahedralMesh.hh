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
 *   $Revision: 13489 $                                                       *
 *   $LastChangedBy: kremer $                                                 *
 *   $Date: 2012-01-25 12:30:09 +0100 (Wed, 25 Jan 2012) $                    *
 *                                                                            *
 \*===========================================================================*/

/**
 * \file PluginFunctionsHexahedralMesh.hh
 * This file contains functions which can be used by plugins to access
 * HexahedralMeshes in the framework.
 */

//
#ifndef PLUGINFUNCTIONSHEXAHEDRALMESH_HH
#define PLUGINFUNCTIONSHEXAHEDRALMESH_HH

#include <ObjectTypes/VolumeMeshObject/VolumeMeshObject.hh>
#include <ObjectTypes/HexahedralMesh/HexahedralMesh.hh>
#include <OpenFlipper/common/Types.hh>

//== NAMESPACES ===============================================================

/** The Namespace PluginFunctions contains functions for all plugins. */
namespace PluginFunctions {


/** \brief Get a pointer to every HexahedralMesh which is marked as a source.
 *
 * @param _meshes ( vector returning the source hexahedral meshes )
 * @return false, if no hexahedral mesh is selected as source
*/
DLLEXPORT
bool getSourceMeshes( std::vector<HexahedralMesh*>& _meshes  );

/** \brief Get a pointer to every HexahedralMesh which is marked as a target.
 *
 * @param _meshes ( vector returning the target hexahedral meshes )
 * @return false, if no hexahedral mesh is selected as target
*/
DLLEXPORT
bool getTargetMeshes( std::vector<HexahedralMesh*>& _meshes  );


/** This functions returns the object with the given id if it is an HexahedralMeshObject.
 * See get_object(  int _identifier , BaseObject*& _object ) for more details.
 */
DLLEXPORT
bool getObject( int _identifier, HexahedralMeshObject*& _object );

/** \brief Get an HexahedralMesh from an object.
 *
 * @param _object The object should be of type BaseDataObject. If the content is
 * a hexahedral mesh, a hexahedral mesh will be returned. Otherwise a NULL pointer is returned.
 */
DLLEXPORT
HexahedralMesh* hexahedralMesh( BaseObjectData* _object );

/** \brief Get a HexahedralMesh from an object id.
 *
 * @param _identifier Identifier of the object. If its a HexahedralMesh mesh, the function will return the pointer to the mesh
 *                    otherwise 0
 */
DLLEXPORT
HexahedralMesh* hexahedralMesh( int _identifier );

/** \brief Cast an BaseObject to an HexahedralMeshObject if possible
 *
 * @param _object The object should be of type BaseDataObject. If the content is
 * an HexahedralMesh, an HexahedralMeshObject is returned. Otherwise a NULL pointer is returned.
 */
DLLEXPORT
HexahedralMeshObject* hexahedralMeshObject( BaseObjectData* _object );


/** \brief Cast an int to an HexahedralMeshObject if possible
 *
 * @param _identifier
 */

DLLEXPORT
HexahedralMeshObject* hexahedralMeshObject( int _identifier );

}

#endif // PLUGINFUNCTIONSHEXAHEDRALMESH_HH
