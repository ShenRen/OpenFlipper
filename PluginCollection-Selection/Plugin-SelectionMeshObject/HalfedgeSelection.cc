/*===========================================================================*\
 *                                                                           *
 *                              OpenFlipper                                  *
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
 *                                                                           *
\*===========================================================================*/

/*===========================================================================*\
 *                                                                           *
 *   $Revision$                                                         *
 *   $Author$                                                      *
 *   $Date$                   *
 *                                                                           *
\*===========================================================================*/

#include "MeshObjectSelectionPlugin.hh"

#include <iostream>

#include <MeshTools/MeshSelectionT.hh>

#include <OpenFlipper/BasePlugin/PluginFunctions.hh>

//=========================================================
//==== Halfedge selections
//=========================================================

void MeshObjectSelectionPlugin::selectHalfedges(int objectId , IdList _halfedgeList) {
    
    if(_halfedgeList.empty() ) return;
    
    BaseObjectData* object = 0;
    if (! PluginFunctions::getObject(objectId,object)) {
        emit log(LOGERR,tr("selectHalfedges: unable to get object"));
        return;
    }

    if (object->dataType() == DATA_TRIANGLE_MESH)
        MeshSelection::selectHalfedges(PluginFunctions::triMesh(object), _halfedgeList);
    else if (object->dataType() == DATA_POLY_MESH)
        MeshSelection::selectHalfedges(PluginFunctions::polyMesh(object), _halfedgeList);
    else {
        emit log(LOGERR,tr("selectHalfedges: Unsupported object Type"));
        return;
    }

    QString selection = "selectHalfedges(ObjectId(" + QString::number(objectId) + ") , [ " + QString::number(_halfedgeList[0]);

    for (uint i = 1 ; i < _halfedgeList.size(); ++i) {
        selection +=  " , " + QString::number(_halfedgeList[i]);
    }

    selection += " ])";

    emit updatedObject(object->id(), UPDATE_SELECTION_EDGES);
    emit scriptInfo(selection);
}

//=========================================================

void MeshObjectSelectionPlugin::unselectHalfedges(int objectId , IdList _halfedgeList) {
    
    if(_halfedgeList.empty() ) return;
    
    BaseObjectData* object = 0;
    if (! PluginFunctions::getObject(objectId,object)) {
        emit log(LOGERR,tr("unselectHalfedges: unable to get object"));
        return;
    }

    if (object->dataType() == DATA_TRIANGLE_MESH)
        MeshSelection::unselectHalfedges(PluginFunctions::triMesh(object), _halfedgeList);
    else if (object->dataType() == DATA_POLY_MESH)
        MeshSelection::unselectHalfedges(PluginFunctions::polyMesh(object), _halfedgeList);
    else {
        emit log(LOGERR,tr("unselectHalfedges: Unsupported object Type"));
        return;
    }

    QString selection = "unselectVertices(ObjectId(" + QString::number(objectId) + ") , [ " + QString::number(_halfedgeList[0]);

    for (uint i = 1 ; i < _halfedgeList.size(); ++i) {
        selection +=  " , " + QString::number(_halfedgeList[i]);
    }

    selection += " ])";

    emit updatedObject(object->id(), UPDATE_SELECTION_EDGES);
    emit scriptInfo(selection);
}

//=========================================================

void MeshObjectSelectionPlugin::selectAllHalfedges(int objectId) {
    
    BaseObjectData* object;
    if (! PluginFunctions::getObject(objectId,object)) {
        emit log(LOGERR,tr("selectAllVertices: unable to get object"));
        return;
    }

    if (object->dataType() == DATA_TRIANGLE_MESH)
        MeshSelection::selectAllHalfedges(PluginFunctions::triMesh(object));
    else if (object->dataType() == DATA_POLY_MESH)
        MeshSelection::selectAllHalfedges(PluginFunctions::polyMesh(object));
    else {
        emit log(LOGERR,tr("selectAllHalfedges: Unsupported object Type"));
        return;
    }

    emit updatedObject(object->id(), UPDATE_SELECTION_EDGES);
    emit scriptInfo("selectAllHalfedges(ObjectId(" + QString::number(objectId) + "))");
}

//=========================================================

void MeshObjectSelectionPlugin::clearHalfedgeSelection(int objectId) {
    
    BaseObjectData* object;
    if (! PluginFunctions::getObject(objectId,object)) {
        emit log(LOGERR,tr("clearHalfedgeSelection: unable to get object"));
        return;
    }

    if (object->dataType() == DATA_TRIANGLE_MESH)
        MeshSelection::clearHalfedgeSelection(PluginFunctions::triMesh(object));
    else if (object->dataType() == DATA_POLY_MESH)
        MeshSelection::clearHalfedgeSelection(PluginFunctions::polyMesh(object));
    else {
        emit log(LOGERR,tr("clearHalfedgeSelection: Unsupported object Type"));
        return;
    }

    emit updatedObject(object->id(), UPDATE_SELECTION_EDGES);
    emit scriptInfo("clearHalfedgeSelection(ObjectId(" + QString::number(objectId) + "))");
}

//=========================================================

void MeshObjectSelectionPlugin::invertHalfedgeSelection(int objectId) {
    
    BaseObjectData* object;
    if (! PluginFunctions::getObject(objectId,object)) {
        emit log(LOGERR,tr("invertHalfedgeSelection: unable to get object"));
        return;
    }

    if (object->dataType() == DATA_TRIANGLE_MESH)
        MeshSelection::invertHalfedgeSelection(PluginFunctions::triMesh(object));
    else if (object->dataType() == DATA_POLY_MESH)
        MeshSelection::invertHalfedgeSelection(PluginFunctions::polyMesh(object));
    else {
        emit log(LOGERR,tr("invertHalfedgeSelection: Unsupported object Type"));
        return;
    }

    emit updatedObject(object->id(), UPDATE_SELECTION_EDGES);
    emit scriptInfo("invertHalfedgeSelection(ObjectId(" + QString::number(objectId) + "))");
}

//=========================================================

void MeshObjectSelectionPlugin::selectBoundaryHalfedges(int objectId) {
    
    BaseObjectData* object;
    if (! PluginFunctions::getObject(objectId,object)) {
        emit log(LOGERR,tr("selectBoundaryHalfedges: unable to get object"));
        return;
    }

    if (object->dataType() == DATA_TRIANGLE_MESH)
        MeshSelection::selectBoundaryHalfedges(PluginFunctions::triMesh(object));
    else if (object->dataType() == DATA_POLY_MESH)
        MeshSelection::selectBoundaryHalfedges(PluginFunctions::polyMesh(object));
    else {
        emit log(LOGERR,tr("selectBoundaryHalfedges: Unsupported object Type"));
        return;
    }

    emit updatedObject(object->id(), UPDATE_SELECTION_EDGES);
    emit scriptInfo("selectBoundaryHalfedges(ObjectId(" + QString::number(objectId) + "))");
}

//=========================================================

IdList MeshObjectSelectionPlugin::getHalfedgeSelection(int objectId) {
    
    BaseObjectData* object;
    if (! PluginFunctions::getObject(objectId,object)) {
        emit log(LOGERR,tr("getHalfedgeSelection: unable to get object"));
        return IdList(0);
    }

    emit scriptInfo("getHalfedgeSelection(ObjectId(" + QString::number(objectId) + "))");

    if (object->dataType() == DATA_TRIANGLE_MESH)
        return MeshSelection::getHalfedgeSelection(PluginFunctions::triMesh(object));
    else if (object->dataType() == DATA_POLY_MESH)
        return MeshSelection::getHalfedgeSelection(PluginFunctions::polyMesh(object));
    else {
        emit log(LOGERR,tr("getHalfedgeSelection: Unsupported object Type"));
        return IdList(0);
    }

    return IdList(0);

}

//=========================================================

IdList MeshObjectSelectionPlugin::convertHalfedgesToVertexPairs(int _id, const IdList& _halfedges) {

    IdList vertex_pairs;

    BaseObjectData* object = 0;
    if ( !PluginFunctions::getObject(_id,object) ) {
        emit log(LOGERR,tr("Cannot find object for id ") + QString::number(_id));
        return IdList(0);
    }

    if(object->dataType() == DATA_TRIANGLE_MESH) {

        TriMeshObject* obj = 0;
        if(!PluginFunctions::getObject(_id, obj)) {
            emit log(LOGERR, "Could not get mesh object!");
            return IdList(0);
        }

        TriMesh* mesh = obj->mesh();

        for(IdList::const_iterator it = _halfedges.begin(); it != _halfedges.end(); ++it) {
            vertex_pairs.push_back(mesh->from_vertex_handle(TriMesh::HalfedgeHandle(*it)).idx());
            vertex_pairs.push_back(mesh->to_vertex_handle(TriMesh::HalfedgeHandle(*it)).idx());
        }

    } else if(object->dataType() == DATA_POLY_MESH) {

        PolyMeshObject* obj = 0;
        if(!PluginFunctions::getObject(_id, obj)) {
            emit log(LOGERR, "Could not get mesh object!");
            return IdList(0);
        }

        PolyMesh* mesh = obj->mesh();

        for(IdList::const_iterator it = _halfedges.begin(); it != _halfedges.end(); ++it) {
            vertex_pairs.push_back(mesh->from_vertex_handle(PolyMesh::HalfedgeHandle(*it)).idx());
            vertex_pairs.push_back(mesh->to_vertex_handle(PolyMesh::HalfedgeHandle(*it)).idx());
        }
    }

    return vertex_pairs;
}

//=========================================================

IdList MeshObjectSelectionPlugin::convertVertexPairsToHalfedges(int _id, const IdList& _vertices) {

    if(_vertices.size() % 2 != 0) {
        emit log(LOGERR, "Number of vertices is not even!");
        return IdList(0);
    }

    IdList halfedges;

    BaseObjectData* object = 0;
    if ( !PluginFunctions::getObject(_id,object) ) {
        emit log(LOGERR,tr("Cannot find object for id ") + QString::number(_id));
        return IdList(0);
    }

    if(object->dataType() == DATA_TRIANGLE_MESH) {

        TriMeshObject* obj = 0;
        if(!PluginFunctions::getObject(_id, obj)) {
            emit log(LOGERR, "Could not get mesh object!");
            return IdList(0);
        }

        TriMesh* mesh = obj->mesh();

        for(IdList::const_iterator it = _vertices.begin(); it != _vertices.end(); it+=2) {
            TriMesh::VertexHandle vh = TriMesh::VertexHandle(*it);
            if(!vh.is_valid()) continue;
            for(TriMesh::VertexOHalfedgeIter voh_it = mesh->voh_iter(vh);
                    voh_it.is_valid(); ++voh_it) {
                if(mesh->to_vertex_handle(*voh_it).idx() == *(it+1)) {
                    halfedges.push_back(voh_it->idx());
                    continue;
                }
            }
        }

    } else if(object->dataType() == DATA_POLY_MESH) {

        PolyMeshObject* obj = 0;
        if(!PluginFunctions::getObject(_id, obj)) {
            emit log(LOGERR, "Could not get mesh object!");
            return IdList(0);
        }

        PolyMesh* mesh = obj->mesh();

        for(IdList::const_iterator it = _vertices.begin(); it != _vertices.end(); it+=2) {
            PolyMesh::VertexHandle vh = PolyMesh::VertexHandle(*it);
            if(!vh.is_valid()) continue;
            for(PolyMesh::VertexOHalfedgeIter voh_it = mesh->voh_iter(vh);
                    voh_it.is_valid(); ++voh_it) {
                if(mesh->to_vertex_handle(*voh_it).idx() == *(it+1)) {
                    halfedges.push_back(voh_it->idx());
                    continue;
                }
            }
        }
    }

    return halfedges;
}

//=========================================================

/// colorize the halfedge selection
void MeshObjectSelectionPlugin::colorizeHalfedgeSelection(int objectId, int r, int g, int b, int a) {
    
    BaseObjectData* object;
    if (! PluginFunctions::getObject(objectId,object)) {
        emit log(LOGERR,"colorizeHalfedgeSelection: unable to get object");
        return;
    }

    if (object->dataType() == DATA_TRIANGLE_MESH) {
        colorizeSelection(PluginFunctions::triMesh(object), halfedgeType_, r, g, b, a);
    } else if (object->dataType() == DATA_POLY_MESH) {
        colorizeSelection(PluginFunctions::polyMesh(object), halfedgeType_, r, g, b, a);
    } else {
        emit log(LOGERR,"colorizeHalfedgeSelection: Unsupported object Type");
        return;
    }

    emit scriptInfo("colorizeHalfedgeSelection(ObjectId(" + QString::number(objectId) + "), "
                     + QString::number(r) + ", " + QString::number(g) + ", " + QString::number(b) + ")");

    emit updatedObject(object->id(), UPDATE_COLOR);
}
