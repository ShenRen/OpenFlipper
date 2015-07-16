
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
 *   $Revision: 15713 $                                                       *
 *   $LastChangedBy: moebius $                                                *
 *   $Date: 2012-10-25 12:58:58 +0200 (Do, 25 Okt 2012) $                     *
 *                                                                            *
\*===========================================================================*/




#include "MeshRepairPlugin.hh"
#include "MeshFixingT.hh"
#include "NonManifoldVertexFixingT.hh"
#include "BoundarySnappingT.hh"

//-----------------------------------------------------------------------------

void MeshRepairPlugin::snapBoundary(int _objectId, double _eps)
{
  TriMesh* triMesh = 0;
  PolyMesh* polyMesh = 0;

  PluginFunctions::getMesh(_objectId, triMesh);
  PluginFunctions::getMesh(_objectId, polyMesh);
  if (triMesh) {
    BoundarySnappingT<TriMesh> snapper(*triMesh);
    snapper.snap(_eps);
  }
  else if (polyMesh) {
    BoundarySnappingT<PolyMesh> snapper(*polyMesh);
    snapper.snap(_eps);
  } else
  {
    emit log(LOGERR, tr("Unsupported Object Type."));
    return;
  }

  emit updatedObject(_objectId, UPDATE_ALL);
  emit createBackup(_objectId, "snapBoundary", UPDATE_ALL);
  emit scriptInfo("snapBoundary(" + QString::number(_objectId) + ", " + QString::number(_eps) +")");
}

//-----------------------------------------------------------------------------

void MeshRepairPlugin::fixNonManifoldVertices(int _objectId)
{
  TriMesh*  triMesh = 0;
  PolyMesh* polyMesh = 0;

  PluginFunctions::getMesh(_objectId, triMesh);
  PluginFunctions::getMesh(_objectId, polyMesh);
  if (triMesh) {
    NonManifoldVertexFixingT<TriMesh> fixer(*triMesh);
    fixer.fix();
  }
  else if (polyMesh) {
    NonManifoldVertexFixingT<PolyMesh> fixer(*polyMesh);
    fixer.fix();
  } else
  {
    emit log(LOGERR, tr("Unsupported Object Type."));
    return;
  }

  emit updatedObject(_objectId, UPDATE_ALL);
  emit createBackup(_objectId, "fixNonManifoldVertices", UPDATE_ALL);
  emit scriptInfo("fixNonManifoldVertices(" + QString::number(_objectId) + ")");
}

//-----------------------------------------------------------------------------

void
MeshRepairPlugin::fixMesh(int _objectId, double _epsilon) {
  // get the target mesh
   TriMesh* triMesh = 0;

   PluginFunctions::getMesh(_objectId,triMesh);

   if (triMesh) {
     MeshFixing<TriMesh> fixer(*triMesh,_epsilon);

     if ( !fixer.fix() )
       emit log(LOGERR, "Fixmesh encountered Problems! Object: " + QString::number(_objectId) + ".");

     // Recompute normals
     triMesh->update_normals();

     emit updatedObject(_objectId, UPDATE_ALL);
     emit createBackup(_objectId, "Fixed mesh", UPDATE_ALL);

     emit scriptInfo( "fixMesh(" + QString::number(_objectId) + ", " + QString::number(_epsilon) + ")" );

   } else
     emit log( LOGERR,tr("Unsupported Object Type for mesh fixing!") );

}

//-----------------------------------------------------------------------------
