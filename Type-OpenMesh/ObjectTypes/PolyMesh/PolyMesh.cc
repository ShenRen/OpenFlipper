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
*   $Revision: 21016 $                                                       *
*   $LastChangedBy: schultz $                                                *
*   $Date: 2015-07-16 16:48:42 +0200 (Do, 16 Jul 2015) $                     *
*                                                                            *
\*===========================================================================*/

/**
 * \file PolyMesh.cc
 * This File contains all required includes for using Poly Meshes
*/




//== INCLUDES =================================================================

#include <ObjectTypes/PolyMesh/PolyMesh.hh>


PolyMeshObject::PolyMeshObject(const PolyMeshObject& _object) : MeshObject< PolyMesh >(_object) {

}


PolyMeshObject::PolyMeshObject(DataType _typeId) : MeshObject< PolyMesh >(_typeId) {

}


PolyMeshObject::~PolyMeshObject() {

}

BaseObject* PolyMeshObject::copy() {
  PolyMeshObject* object = new PolyMeshObject(*this );
  return object;
}

/// Refine picking on triangle meshes
ACG::Vec3d PolyMeshObject::refinePick(ACG::SceneGraph::PickTarget _pickTarget, const ACG::Vec3d _hitPoint, const ACG::Vec3d _start , const ACG::Vec3d _dir,  const unsigned int _targetIdx  ) {
  if ( _pickTarget == ACG::SceneGraph::PICK_FACE) {
    //don't refine poly faces
    return _hitPoint;
  }

  if ( _pickTarget == ACG::SceneGraph::PICK_EDGE) {
    // get picked edge handle
    PolyMesh::EdgeHandle eh = mesh()->edge_handle(_targetIdx);
    if(eh.is_valid())
    {
      PolyMesh::HalfedgeHandle heh = mesh()->halfedge_handle(eh,0);

      //get vertices of the edge
      PolyMesh::VertexHandle vhbegin = mesh()->to_vertex_handle(heh);
      PolyMesh::VertexHandle vhend = mesh()->from_vertex_handle(heh);
      ACG::Vec3d edgeStart = mesh()->point(vhbegin);
      ACG::Vec3d edgeEnd = mesh()->point(vhend);

      //retrieve the point on the edge that is closest to the backprojected hitpoint
      ACG::Vec3d hitPointNew;
      ACG::Geometry::distPointLineSquared(_hitPoint,edgeStart,edgeEnd,&hitPointNew);


      return hitPointNew;
    }
  }

  if ( _pickTarget == ACG::SceneGraph::PICK_VERTEX) {
    // get picked vertex handle
    PolyMesh::VertexHandle vh = mesh()->vertex_handle(_targetIdx);
    if(vh.is_valid())
    {
      ACG::Vec3d hitpointNew = mesh()->point(vh);

      //just return the vertex position
      return hitpointNew;
    }
  }

  return _hitPoint;
}


//=============================================================================



