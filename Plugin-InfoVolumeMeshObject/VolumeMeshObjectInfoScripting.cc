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
*   $Revision: 16055 $                                                       *
*   $LastChangedBy: moebius $                                                *
*   $Date: 2013-01-08 10:57:31 +0100 (Tue, 08 Jan 2013) $                     *
*                                                                            *
\*===========================================================================*/

#include "VolumeMeshObjectInfoPlugin.hh"

#include <OpenFlipper/BasePlugin/PluginFunctions.hh>

#include <ObjectTypes/HexahedralMesh/HexahedralMesh.hh>
#include <ObjectTypes/PolyhedralMesh/PolyhedralMesh.hh>

//------------------------------------------------------------------------------

/** \brief set the descriptions for scripting slots
 * 
 */
void InfoVolumeMeshObjectPlugin::setDescriptions(){

  emit setSlotDescription("vertexCount(int)",tr("get total number of vertices for a given object"),
                          QStringList(tr("objectID")), QStringList(tr("id of an object")));

  emit setSlotDescription("edgeCount(int)",tr("get total number of edges for a given object"),
                          QStringList(tr("objectID")), QStringList(tr("id of an object")));

  emit setSlotDescription("faceCount(int)",tr("get total number of faces for a given object"),
                          QStringList(tr("objectID")), QStringList(tr("id of an object")));

  emit setSlotDescription("cellCount(int)",tr("get total number of cells for a given object"),
                          QStringList(tr("objectID")), QStringList(tr("id of an object")));
}


//------------------------------------------------------------------------------

/** \brief get total number of vertices for a given object
 * 
 * @param _id object id
 * @return number of vertices or -1 if an error occured
 */
int InfoVolumeMeshObjectPlugin::vertexCount(int _id)
{

  BaseObjectData* object;
  if ( ! PluginFunctions::getObject(_id,object) )
    return -1;

  if ( object == 0){
    emit log(LOGERR, tr("Unable to get object"));
    return -1;
  }

  if ( object->dataType(DATA_HEXAHEDRAL_MESH) ) {
    HexahedralMesh* mesh = PluginFunctions::hexahedralMesh(object);

    if ( mesh == 0 ) {
      emit log(LOGERR, tr("Unable to get mesh"));
      return -1;
    }

    return mesh->n_vertices();

  } else {
    PolyhedralMesh* mesh = PluginFunctions::polyhedralMesh(object);

    if ( mesh == 0 ) {
      emit log(LOGERR, tr("Unable to get mesh"));
      return -1;
    }

    return mesh->n_vertices();
  }
}


//------------------------------------------------------------------------------

/** \brief get total number of edges for a given object
 * 
 * @param _id object id
 * @return number of edges or -1 if an error occured
 */
int InfoVolumeMeshObjectPlugin::edgeCount(int _id)
{

  BaseObjectData* object;
  if ( ! PluginFunctions::getObject(_id,object) )
    return -1;

  if ( object == 0){
    emit log(LOGERR, tr("Unable to get object"));
    return -1;
  }

  if ( object->dataType(DATA_HEXAHEDRAL_MESH) ) {
    HexahedralMesh* mesh = PluginFunctions::hexahedralMesh(object);

    if ( mesh == 0 ) {
      emit log(LOGERR, tr("Unable to get mesh"));
      return -1;
    }

    return mesh->n_edges();

  } else {
    PolyhedralMesh* mesh = PluginFunctions::polyhedralMesh(object);

    if ( mesh == 0 ) {
      emit log(LOGERR, tr("Unable to get mesh"));
      return -1;
    }

    return mesh->n_edges();
  }
}


//------------------------------------------------------------------------------

/** \brief get total number of faces for a given object
 *
 * @param _id object id
 * @return number of faces or -1 if an error occured
 */
int InfoVolumeMeshObjectPlugin::faceCount(int _id)
{

  BaseObjectData* object;
  if ( ! PluginFunctions::getObject(_id,object) )
    return -1;

  if ( object == 0){
    emit log(LOGERR, tr("Unable to get object"));
    return -1;
  }

  if ( object->dataType(DATA_HEXAHEDRAL_MESH) ) {
    HexahedralMesh* mesh = PluginFunctions::hexahedralMesh(object);

    if ( mesh == 0 ) {
      emit log(LOGERR, tr("Unable to get mesh"));
      return -1;
    }

    return mesh->n_faces();

  } else {
    PolyhedralMesh* mesh = PluginFunctions::polyhedralMesh(object);

    if ( mesh == 0 ) {
      emit log(LOGERR, tr("Unable to get mesh"));
      return -1;
    }

    return mesh->n_faces();
  }
}


//------------------------------------------------------------------------------

/** \brief get total number of cells for a given object
 *
 * @param _id object id
 * @return number of cells or -1 if an error occured
 */
int InfoVolumeMeshObjectPlugin::cellCount(int _id)
{

  BaseObjectData* object;
  if ( ! PluginFunctions::getObject(_id,object) )
    return -1;

  if ( object == 0){
    emit log(LOGERR, tr("Unable to get object"));
    return -1;
  }

  if ( object->dataType(DATA_HEXAHEDRAL_MESH) ) {
    HexahedralMesh* mesh = PluginFunctions::hexahedralMesh(object);

    if ( mesh == 0 ) {
      emit log(LOGERR, tr("Unable to get mesh"));
      return -1;
    }

    return mesh->n_faces();

  } else {
    PolyhedralMesh* mesh = PluginFunctions::polyhedralMesh(object);

    if ( mesh == 0 ) {
      emit log(LOGERR, tr("Unable to get mesh"));
      return -1;
    }

    return mesh->n_cells();
  }
}

//------------------------------------------------------------------------------


