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


/**
 * \file TriangleMesh.hh
 * This File contains all required includes for using Triangle Meshes
*/

#ifndef TRIANGLE_MESH_INCLUDE_HH
#define TRIANGLE_MESH_INCLUDE_HH


//== INCLUDES =================================================================


/** \def DATA_TRIANGLE_MESH
 * Use this macro to reference triangle meshes.
 */
#define DATA_TRIANGLE_MESH typeId("TriangleMesh")
#include <ObjectTypes/MeshObject/MeshObjectT.hh>
#include <ObjectTypes/TriangleMesh/TriangleMeshTypes.hh>
#include <OpenFlipper/common/GlobalDefines.hh>


/// Type for a MeshObject containing a triangle mesh
class DLLEXPORTONLY TriMeshObject : public MeshObject< TriMesh > {

public:
  /** \brief copy constructor
   *
   *  Create a copy of this object
   */
  TriMeshObject(const TriMeshObject& _object);

  /** \brief Constructor
  *
  * This is the standard constructor for MeshObjects. As triangle and Poly Meshes are handled by this class, the
  * typeId is passed to the MeshObject to specify it.
  *
  * @param _typeId   This is the type Id the Object will use. Should be typeId("TriangleMesh") or typeId("PolyMesh")
  */
  TriMeshObject(DataType _typeId);

  /// destructor
  virtual ~TriMeshObject();

  /** return a full copy of this object ( All scenegraph nodes will be created )
   *  but the object will not be a part of the object tree.
   */
  BaseObject* copy();

 public:
    /// Refine picking on triangle meshes
    ACG::Vec3d refinePick(ACG::SceneGraph::PickTarget _pickTarget, const ACG::Vec3d _hitPoint, const ACG::Vec3d _start , const ACG::Vec3d _dir,  const unsigned int _targetIdx  );

};

#include <ObjectTypes/TriangleMesh/PluginFunctionsTriangleMesh.hh>



//=============================================================================
#endif // TRIANGLE_MESH_INCLUDE_HH defined
//=============================================================================

