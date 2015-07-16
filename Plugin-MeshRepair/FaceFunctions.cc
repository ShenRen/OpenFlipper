
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

void
MeshRepairPlugin::detectTriangleAspect(int _objectId, float _aspect) {

  // get the target mesh
  TriMesh* mesh = 0;

  PluginFunctions::getMesh(_objectId, mesh);

  if (mesh) {

    unsigned int count(0);

    // Clear current face selection
    MeshSelection::clearFaceSelection(mesh);

    TriMesh::FaceIter f_it, f_end(mesh->faces_end());
    TriMesh::FVIter fv_it;
    TriMesh::FEIter fe_it;

    for (f_it = mesh->faces_begin(); f_it != f_end; ++f_it) {
      fv_it = mesh->fv_iter(*f_it);

      const TriMesh::Point& p0 = mesh->point(*fv_it);
      const TriMesh::Point& p1 = mesh->point(*(++fv_it));
      const TriMesh::Point& p2 = mesh->point(*(++fv_it));

      if (ACG::Geometry::aspectRatio(p0, p1, p2) > _aspect) {
        mesh->status(*f_it).set_selected(true);
        ++count;
      }
    }
    if (count > 0) {
      emit updatedObject(_objectId, UPDATE_SELECTION);
      emit createBackup(_objectId, "Select triangles", UPDATE_SELECTION);
      emit scriptInfo( "detectTriangleAspect(" + QString::number(_objectId) + ", " + QString::number(_aspect) + ")" );
    }
    emit log(
        "Selected " + QString::number(count) + " triangles on object " + QString::number(_objectId)
    + " with aspect ratio greater than " + QString::number(_aspect) + ".");
  } else {
    emit log("Cannot detect skinny triangles on non-trimesh " + QString::number(_objectId) + ".");
  }
}

//-----------------------------------------------------------------------------

void
MeshRepairPlugin::flipOrientation(int _objectId) {

  // get the target mesh
  TriMesh* triMesh = 0;
  PolyMesh* polyMesh = 0;

  PluginFunctions::getMesh(_objectId,triMesh);
  PluginFunctions::getMesh(_objectId,polyMesh);

  if (triMesh)
    flipOrientation(*triMesh);
  else if (polyMesh)
    flipOrientation(*polyMesh);
  else
    emit log( LOGERR,tr("Unsupported Object Type for normal flipping!") );


  emit updatedObject(_objectId, UPDATE_ALL);
  emit createBackup( _objectId, "Flipped Normals", UPDATE_ALL);
  emit scriptInfo( "flipOrientation(" + QString::number(_objectId) + ")" );
}


//-----------------------------------------------------------------------------
