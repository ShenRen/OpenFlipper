
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


//-----------------------------------------------------------------------------


void MeshRepairPlugin::updateFaceNormals(int _objectId) {
  BaseObjectData* object = 0;
  PluginFunctions::getObject(_objectId,object);

  if ( object == 0) {
    emit log(LOGERR,tr("updateFaceNormals: Unable to get object %1. ").arg(_objectId) );
    return;
  }

  if ( object->dataType(DATA_TRIANGLE_MESH) ) {
    TriMesh* mesh = PluginFunctions::triMesh(object);
    mesh->update_face_normals();
    emit updatedObject(_objectId, UPDATE_ALL);
    emit createBackup( _objectId, "Updated Face Normals", UPDATE_ALL);
    emit scriptInfo( "updateFaceNormals(" + QString::number(_objectId) + ")" );
  } else if ( object->dataType(DATA_POLY_MESH) ) {
    PolyMesh* mesh = PluginFunctions::polyMesh(object);
    mesh->update_face_normals();
    emit updatedObject(_objectId, UPDATE_ALL);
    emit createBackup( _objectId, "Updated Face Normals", UPDATE_ALL);
    emit scriptInfo( "updateFaceNormals(" + QString::number(_objectId) + ")" );
  } else
    emit log(LOGERR,tr("updateFaceNormals: MeshRepair only works on triangle and poly meshes!") );

}


//-----------------------------------------------------------------------------

void MeshRepairPlugin::updateHalfedgeNormals(int _objectId) {
  BaseObjectData* object = 0;
  PluginFunctions::getObject(_objectId,object);

  if ( object == 0) {
    emit log(LOGERR,tr("updateFaceNormals: Unable to get object %1. ").arg(_objectId) );
    return;
  }

  if ( object->dataType(DATA_TRIANGLE_MESH) ) {
    TriMesh* mesh = PluginFunctions::triMesh(object);
    mesh->request_halfedge_normals();
    mesh->update_halfedge_normals();
    emit updatedObject(_objectId, UPDATE_ALL);
    emit createBackup( _objectId, "Updated Face Normals", UPDATE_ALL);
    emit scriptInfo( "updateFaceNormals(" + QString::number(_objectId) + ")" );
  } else if ( object->dataType(DATA_POLY_MESH) ) {
    PolyMesh* mesh = PluginFunctions::polyMesh(object);
    mesh->request_halfedge_normals();
    mesh->update_halfedge_normals();
    emit updatedObject(_objectId, UPDATE_ALL);
    emit createBackup( _objectId, "Updated Face Normals", UPDATE_ALL);
    emit scriptInfo( "updateFaceNormals(" + QString::number(_objectId) + ")" );
  } else
    emit log(LOGERR,tr("updateFaceNormals: MeshRepair only works on triangle and poly meshes!") );

}


//-----------------------------------------------------------------------------


void MeshRepairPlugin::updateVertexNormals(int _objectId){
  BaseObjectData* object = 0;
  PluginFunctions::getObject(_objectId,object);

  if ( object == 0) {
    emit log(LOGERR,tr("updateVertexNormals: Unable to get object %1. ").arg(_objectId) );
    return;
  }

  if ( object->dataType(DATA_TRIANGLE_MESH) ) {
    TriMesh* mesh = PluginFunctions::triMesh(object);
    mesh->update_vertex_normals();
    emit updatedObject(_objectId, UPDATE_ALL);
    emit createBackup( _objectId, "Updated Vertex Normals", UPDATE_ALL);
    emit scriptInfo( "updateVertexNormals(" + QString::number(_objectId) + ")" );
  } else if ( object->dataType(DATA_POLY_MESH) ) {
    PolyMesh* mesh = PluginFunctions::polyMesh(object);
    mesh->update_vertex_normals();
    emit updatedObject(_objectId, UPDATE_ALL);
    emit createBackup( _objectId, "Updated Vertex Normals", UPDATE_ALL);
    emit scriptInfo( "updateVertexNormals(" + QString::number(_objectId) + ")" );
  } else
    emit log(LOGERR,tr("updateVertexNormals: MeshRepair only works on triangle and poly meshes!") );
}

//-----------------------------------------------------------------------------

void MeshRepairPlugin::updateNormals(int _objectId) {
  BaseObjectData* object = 0;
  PluginFunctions::getObject(_objectId,object);

  if ( object == 0) {
    emit log(LOGERR,tr("updateNormals: Unable to get object %1. ").arg(_objectId) );
    return;
  }

  if ( object->dataType(DATA_TRIANGLE_MESH) ) {
    TriMesh* mesh = PluginFunctions::triMesh(object);
    mesh->update_normals();
    emit scriptInfo( "updateNormals(" + QString::number(_objectId) + ")" );
  } else if ( object->dataType(DATA_POLY_MESH) ) {
    PolyMesh* mesh = PluginFunctions::polyMesh(object);
    mesh->update_normals();
    emit updatedObject(_objectId, UPDATE_ALL);
    emit createBackup( _objectId, "Updated All Normals", UPDATE_ALL);
    emit scriptInfo( "updateNormals(" + QString::number(_objectId) + ")" );
  } else
    emit log(LOGERR,tr("updateNormals: MeshRepair only works on triangle and poly meshes!") );
}


