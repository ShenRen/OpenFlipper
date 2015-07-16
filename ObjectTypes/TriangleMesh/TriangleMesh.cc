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
*   $Revision: 18128 $                                                       *
*   $LastChangedBy: moebius $                                                *
*   $Date: 2014-02-05 10:20:28 +0100 (Mi, 05. Feb 2014) $                     *
*                                                                            *
\*===========================================================================*/


/**
 * \file TriangleMesh.cc
 * This File contains all required includes for using Triangle Meshes
*/




//== INCLUDES =================================================================

#include <ObjectTypes/TriangleMesh/TriangleMesh.hh>
#include <ACG/Geometry/Algorithms.hh>



TriMeshObject::TriMeshObject(const TriMeshObject& _object) : MeshObject< TriMesh >(_object) {

}


TriMeshObject::TriMeshObject(DataType _typeId) : MeshObject< TriMesh >(_typeId) {

}


TriMeshObject::~TriMeshObject() {

}

BaseObject* TriMeshObject::copy() {
  TriMeshObject* object = new TriMeshObject(*this );
  return object;
}

/// Refine picking on triangle meshes
ACG::Vec3d TriMeshObject::refinePick(ACG::SceneGraph::PickTarget _pickTarget, const ACG::Vec3d _hitPoint, const ACG::Vec3d _start , const ACG::Vec3d _dir,  const unsigned int _targetIdx  ) {
  if ( _pickTarget == ACG::SceneGraph::PICK_FACE) {

    // get picked face handle
    TriMesh::FaceHandle fh = mesh()->face_handle(_targetIdx);

    TriMesh::FaceVertexIter fv_it = mesh()->fv_begin(fh);

    // Get vertices of the face
    ACG::Vec3d p1 = mesh()->point(*fv_it);
    ++fv_it;

    ACG::Vec3d p2 = mesh()->point(*fv_it);
    ++fv_it;

    ACG::Vec3d p3 = mesh()->point(*fv_it);
    ++fv_it;

    ACG::Vec3d hitpointNew = _hitPoint;

    TriMesh::Scalar t,u,v;
    if ( ACG::Geometry::triangleIntersection( _start, _dir, p1 , p2 , p3 , t , u , v) ) {
      hitpointNew = _start + t * _dir;
    }

    return hitpointNew;
  }

  if ( _pickTarget == ACG::SceneGraph::PICK_EDGE) {
    // get picked edge handle
    TriMesh::EdgeHandle eh = mesh()->edge_handle(_targetIdx);
    if(eh.is_valid())
    {
      TriMesh::HalfedgeHandle heh = mesh()->halfedge_handle(eh,0);

      //get vertices of the edge
      TriMesh::VertexHandle vhbegin = mesh()->to_vertex_handle(heh);
      TriMesh::VertexHandle vhend = mesh()->from_vertex_handle(heh);
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
    TriMesh::VertexHandle vh = mesh()->vertex_handle(_targetIdx);
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

