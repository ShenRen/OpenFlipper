
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




#include "MeshRepairPlugin.hh"
#include <MeshTools/MeshSelectionT.hh>

//-----------------------------------------------------------------------------


void MeshRepairPlugin::detectFlatValence3Vertices(int _objectId, double _angle) {

  unsigned int count(0);

  // get the target mesh
  TriMesh* mesh = 0;
  PluginFunctions::getMesh(_objectId,mesh);

  if ( mesh ) {

    // Clear current triangle selection
    MeshSelection::clearVertexSelection(mesh);

    TriMesh::VertexIter                 v_it, v_end(mesh->vertices_end());
    TriMesh::VVIter                     vv_it;
    TriMesh::VFIter                     vf_it;
    TriMesh::FaceHandle                 fh;
    std::vector<TriMesh::VertexHandle>  vh(3);
    TriMesh::Scalar                     cosangle(cos(_angle/180.0*M_PI));

    for (v_it=mesh->vertices_begin(); v_it!=v_end; ++v_it)
    {
      if (!mesh->status(*v_it).deleted() && !mesh->is_boundary(*v_it) && mesh->valence(*v_it) == 3)
      {
        vf_it = mesh->vf_iter(*v_it);
        const TriMesh::Normal& n0 = mesh->normal(*vf_it);
        const TriMesh::Normal& n1 = mesh->normal(*(++vf_it));
        const TriMesh::Normal& n2 = mesh->normal(*(++vf_it));

        if ( (n0|n1) > cosangle &&
            (n0|n2) > cosangle &&
            (n1|n2) > cosangle )
        {

          mesh->status(*v_it).set_selected(true);
          ++count;
        }
      }
    }
  }
  else {
    emit log(LOGERR, "Cannot detect flat triangles on non-trimesh " + QString::number(_objectId) + ".");
  }

  if (count > 0) {
    emit updatedObject(_objectId, UPDATE_SELECTION);
    emit createBackup(_objectId, "Select vertices", UPDATE_SELECTION);
  }

  emit log (LOGINFO,"Selected " + QString::number(count) + " vertices on object " + QString::number(_objectId) + " with face angle difference smaller than " + QString::number(_angle) + ".");
  emit scriptInfo( "detectFlatValence3Vertices(" + QString::number(_objectId) + ", " + QString::number(_angle) + ")" );

}

//-----------------------------------------------------------------------------

void MeshRepairPlugin::removeSelectedVal3Vertices(int _objectId) {

  unsigned int count = 0;

  // get the target mesh
  TriMesh* mesh = 0;
  PluginFunctions::getMesh(_objectId, mesh);

  if (mesh) {

    TriMesh::VertexIter v_it, v_end(mesh->vertices_end());
    TriMesh::VVIter vv_it;
    TriMesh::VFIter vf_it;
    int i;
    std::vector<TriMesh::VertexHandle> vh(3);

    for (v_it = mesh->vertices_begin(); v_it != v_end; ++v_it) {
      vf_it = mesh->vf_iter(*v_it);
      if ((mesh->status(*v_it).selected()) && !mesh->status(*v_it).feature() && mesh->valence(*v_it) == 3) {
        for (i = 0, vv_it = mesh->vv_iter(*v_it); vv_it.is_valid(); ++vv_it, ++i)
          vh[2 - i] = *vv_it;

        mesh->delete_vertex(*v_it, false);
        mesh->add_face(vh);

        ++count;
      }
    }
    if (count > 0)
      mesh->garbage_collection();
  }

  if (count > 0) {
    emit updatedObject(_objectId, UPDATE_ALL);
    emit createBackup(_objectId, "Delete/merge selected vertices", UPDATE_ALL);
  }
  emit log("Deleted " + QString::number(count) + " vertices on object " + QString::number(_objectId) + ".");
}

//-----------------------------------------------------------------------------
