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


#include <Eigen/Dense>

namespace align{


template< class MeshT >
void moveToCOG(MeshT& _mesh) {

  ACG::Vec3d mean(0.0);
  for (typename MeshT::VertexIter v_it = _mesh.vertices_begin(); v_it != _mesh.vertices_end(); ++v_it) {
    mean += _mesh.point(*v_it);
  }

  mean /= (double)_mesh.n_vertices();

  for (TriMesh::VertexIter v_it = _mesh.vertices_begin(); v_it != _mesh.vertices_end(); ++v_it) {
    _mesh.set_point(*v_it, _mesh.point(*v_it) - mean);
  }
}


template< class MeshT >
void rotate(MeshT& _mesh) {
  using namespace Eigen;

  Matrix3Xd data = Matrix3Xd::Zero(3, _mesh.n_vertices());
  size_t i(0);
  for (typename MeshT::VertexIter v_it = _mesh.vertices_begin(); v_it != _mesh.vertices_end(); ++v_it, ++i) {
    const ACG::Vec3d tmp = _mesh.point(*v_it);
    data.col(i) = Vector3d(tmp[0], tmp[1], tmp[2]);
  }

  Matrix3d covar = (data * data.transpose()) / (double)_mesh.n_vertices();

  JacobiSVD<Matrix3d> svd(covar, ComputeThinU | ComputeThinV);

  const Matrix3d& u = svd.matrixU();

  Eigen::Vector3d v0 = u.col(0);
  Eigen::Vector3d v1 = u.col(1);
  Eigen::Vector3d v2 = v0.cross(v1);

  v0.normalize();
  v1.normalize();
  v2.normalize();

  Matrix3d trans;
  trans.col(0) = v0;
  trans.col(1) = v1;
  trans.col(2) = v2;

  Matrix3d invTrans = trans.inverse();

  ACG::Matrix4x4d mat;
  mat.identity();
  for (i = 0; i < 3; ++i) {
    for (size_t j = 0; j < 3; ++j) {
      mat(i,j) = invTrans(i,j);
    }
  }

  for (typename MeshT::VertexIter v_it = _mesh.vertices_begin(); v_it != _mesh.vertices_end(); ++v_it, ++i) {
    const ACG::Vec4d tmp(_mesh.point(*v_it)[0], _mesh.point(*v_it)[1], _mesh.point(*v_it)[2], 1.0);
    const ACG::Vec4d res = mat * tmp;

    _mesh.set_point(*v_it, ACG::Vec3d(res[0], res[1],res[2]));
  }
}

template< class MeshT >
void moveCenterOfBBToOrigin(MeshT& _mesh) {

  ACG::Vec3d min(DBL_MAX);
  ACG::Vec3d max(-DBL_MAX);
  for (typename MeshT::VertexIter v_it = _mesh.vertices_begin(); v_it != _mesh.vertices_end(); ++v_it) {
    min.minimize(_mesh.point(*v_it));
    max.maximize(_mesh.point(*v_it));
  }

  const ACG::Vec3d diag = max - min;
  const ACG::Vec3d center = min + 0.5*diag;

  for (typename MeshT::VertexIter v_it = _mesh.vertices_begin(); v_it != _mesh.vertices_end(); ++v_it) {
    _mesh.point(*v_it) -= center;
  }
}

} // namespace align

#define ALIGNT_CC
