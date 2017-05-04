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
 * \file HexahedralMesh/HexahedralMesh.hh
 * This file contains all required includes for using polyhedral meshes
*/

#ifndef HEXAHEDRAL_MESH_INCLUDE_HH
#define HEXAHEDRAL_MESH_INCLUDE_HH

//== INCLUDES =================================================================

/** \def DATA_HEXAHEDRAL_MESH
 * Use this macro to reference Hexahedral Meshes
 */
#define DATA_HEXAHEDRAL_MESH typeId("HexahedralMesh")
#include <ObjectTypes/VolumeMeshObject/VolumeMeshObject.hh>
#include <OpenVolumeMesh/Mesh/HexahedralMesh.hh>
#include <OpenVolumeMesh/Core/PropertyDefines.hh>

typedef OpenVolumeMesh::GeometryKernel<ACG::Vec3d, OpenVolumeMesh::HexahedralMeshTopologyKernel> HexahedralMesh;

/// Typedef for a mesh object containing a polyhedral mesh
typedef VolumeMeshObject<HexahedralMesh> HexahedralMeshObject;

#include <ObjectTypes/HexahedralMesh/PluginFunctionsHexahedralMesh.hh>

//=============================================================================
#endif // HEXAHEDRAL_MESH_INCLUDE_HH defined
//=============================================================================

