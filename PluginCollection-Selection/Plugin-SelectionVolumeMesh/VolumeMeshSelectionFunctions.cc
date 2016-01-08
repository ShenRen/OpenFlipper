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


#include "VolumeMeshSelectionPlugin.hh"

/*
 * Vertices
 */
void VolumeMeshSelectionPlugin::selectAllVertices(int _objectId) {

    BaseObjectData* object = 0;

    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return;
    }

    if (getStatus(_objectId) == NULL)
        return;

    OpenVolumeMesh::StatusAttrib& status = *getStatus(_objectId);

    for(OpenVolumeMesh::StatusAttrib::vstatus_iterator vst_it = status.vstatus_begin();
            vst_it != status.vstatus_end(); ++vst_it) {
        vst_it->set_selected(true);
    }

    emit scriptInfo("selectAllVertices(ObjectId)");
    emit updatedObject(_objectId, UPDATE_SELECTION);
}

void VolumeMeshSelectionPlugin::deselectAllVertices(int _objectId) {

    BaseObjectData* object = 0;

    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return;
    }

    if (getStatus(_objectId) == NULL)
        return;

    OpenVolumeMesh::StatusAttrib& status = *getStatus(_objectId);

    for(OpenVolumeMesh::StatusAttrib::vstatus_iterator vst_it = status.vstatus_begin();
            vst_it != status.vstatus_end(); ++vst_it) {
        vst_it->set_selected(false);
    }

    emit scriptInfo("deselectAllVertices(ObjectId)");
    emit updatedObject(_objectId, UPDATE_SELECTION);
}

void VolumeMeshSelectionPlugin::invertVertexSelection(int _objectId) {

    BaseObjectData* object = 0;

    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return;
    }

    if (getStatus(_objectId) == NULL)
        return;

    OpenVolumeMesh::StatusAttrib& status = *getStatus(_objectId);

    for(OpenVolumeMesh::StatusAttrib::vstatus_iterator vst_it = status.vstatus_begin();
            vst_it != status.vstatus_end(); ++vst_it) {
        vst_it->set_selected(!vst_it->selected());
    }

    emit scriptInfo("invertVertexSelection(ObjectId)");
    emit updatedObject(_objectId, UPDATE_SELECTION);
}

void VolumeMeshSelectionPlugin::selectVertices(int _objectId, const IdList& _ids, bool _deselect) {

    if(_ids.empty()) return;

    BaseObjectData* object = 0;

    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return;
    }

    if (getStatus(_objectId) == NULL)
        return;

    OpenVolumeMesh::StatusAttrib& status = *getStatus(_objectId);

    for(IdList::const_iterator v_it = _ids.begin(); v_it != _ids.end(); ++v_it) {
        status[OpenVolumeMesh::VertexHandle(*v_it)].set_selected(!_deselect);
    }

    QString selection = "selectVertices(ObjectId, [ " + QString::number(_ids[0]);

    for (uint i = 1 ; i < _ids.size(); ++i) {
        selection +=  ", " + QString::number(_ids[i]);
    }

    selection += " ])";

    emit scriptInfo(selection);
    emit updatedObject(_objectId, UPDATE_SELECTION);
}

IdList VolumeMeshSelectionPlugin::getVertexSelection(int _objectId) {

    BaseObjectData* object = 0;

    IdList list;

    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return list;
    }

    if (getStatus(_objectId) == NULL)
        return list;

    OpenVolumeMesh::StatusAttrib& status = *getStatus(_objectId);

    int id = 0;
    for(OpenVolumeMesh::StatusAttrib::vstatus_iterator vst_it = status.vstatus_begin();
            vst_it != status.vstatus_end(); ++vst_it, ++id) {
        if(vst_it->selected()) list.push_back(id);
    }

    return list;
}

void VolumeMeshSelectionPlugin::deleteSelectedVertices(int _objectId, bool _preserveManifoldness) {

    BaseObjectData* object = 0;

    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return;
    }

    if (getStatus(_objectId) == NULL)
        return;

    OpenVolumeMesh::StatusAttrib& status = *getStatus(_objectId);

    for(OpenVolumeMesh::StatusAttrib::vstatus_iterator vst_it = status.vstatus_begin();
            vst_it != status.vstatus_end(); ++vst_it) {
        if(vst_it->selected()) vst_it->set_deleted(true);
    }

    status.garbage_collection(_preserveManifoldness);

    emit scriptInfo("deleteSelectedVertices(ObjectId)");
    emit updatedObject(_objectId, UPDATE_ALL);
}

/*
 * Edges
 */
void VolumeMeshSelectionPlugin::selectAllEdges(int _objectId) {

    BaseObjectData* object = 0;

    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return;
    }

    if (getStatus(_objectId) == NULL)
        return;

    OpenVolumeMesh::StatusAttrib& status = *getStatus(_objectId);

    for(OpenVolumeMesh::StatusAttrib::estatus_iterator est_it = status.estatus_begin();
            est_it != status.estatus_end(); ++est_it) {
        est_it->set_selected(true);
    }

    emit scriptInfo("selectAllEdges(ObjectId)");
    emit updatedObject(_objectId, UPDATE_SELECTION);
}

void VolumeMeshSelectionPlugin::deselectAllEdges(int _objectId) {

    BaseObjectData* object = 0;

    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return;
    }

    if (getStatus(_objectId) == NULL)
        return;

    OpenVolumeMesh::StatusAttrib& status = *getStatus(_objectId);

    for(OpenVolumeMesh::StatusAttrib::estatus_iterator est_it = status.estatus_begin();
            est_it != status.estatus_end(); ++est_it) {
        est_it->set_selected(false);
    }

    emit scriptInfo("deselectAllEdges(ObjectId)");
    emit updatedObject(_objectId, UPDATE_SELECTION);
}

void VolumeMeshSelectionPlugin::invertEdgeSelection(int _objectId) {

    BaseObjectData* object = 0;

    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return;
    }

    if (getStatus(_objectId) == NULL)
        return;

    OpenVolumeMesh::StatusAttrib& status = *getStatus(_objectId);

    for(OpenVolumeMesh::StatusAttrib::estatus_iterator est_it = status.estatus_begin();
            est_it != status.estatus_end(); ++est_it) {
        est_it->set_selected(!est_it->selected());
    }

    emit scriptInfo("invertEdgeSelection(ObjectId)");
    emit updatedObject(_objectId, UPDATE_SELECTION);
}

void VolumeMeshSelectionPlugin::selectEdges(int _objectId, const IdList& _ids, bool _deselect) {

    if(_ids.empty()) return;

    BaseObjectData* object = 0;

    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return;
    }

    if (getStatus(_objectId) == NULL)
        return;

    OpenVolumeMesh::StatusAttrib& status = *getStatus(_objectId);

    for(IdList::const_iterator v_it = _ids.begin(); v_it != _ids.end(); ++v_it) {
        status[OpenVolumeMesh::EdgeHandle(*v_it)].set_selected(!_deselect);
    }

    QString selection = "selectEdges(ObjectId, [ " + QString::number(_ids[0]);

    for (uint i = 1 ; i < _ids.size(); ++i) {
        selection +=  ", " + QString::number(_ids[i]);
    }

    selection += " ])";

    emit scriptInfo(selection);
    emit updatedObject(_objectId, UPDATE_SELECTION);
}

IdList VolumeMeshSelectionPlugin::getEdgeSelection(int _objectId) {

    BaseObjectData* object = 0;

    IdList list;

    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return list;
    }

    if (getStatus(_objectId) == NULL)
        return list;

    OpenVolumeMesh::StatusAttrib& status = *getStatus(_objectId);

    int id = 0;
    for(OpenVolumeMesh::StatusAttrib::estatus_iterator est_it = status.estatus_begin();
            est_it != status.estatus_end(); ++est_it, ++id) {
        if(est_it->selected()) list.push_back(id);
    }

    return list;
}

void VolumeMeshSelectionPlugin::deleteSelectedEdges(int _objectId, bool _preserveManifoldness) {

    BaseObjectData* object = 0;

    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return;
    }

    if (getStatus(_objectId) == NULL)
        return;

    OpenVolumeMesh::StatusAttrib& status = *getStatus(_objectId);

    for(OpenVolumeMesh::StatusAttrib::estatus_iterator est_it = status.estatus_begin();
            est_it != status.estatus_end(); ++est_it) {
        if(est_it->selected()) est_it->set_deleted(true);
    }

    status.garbage_collection(_preserveManifoldness);

    emit scriptInfo("deleteSelectedEdges(ObjectId)");
    emit updatedObject(_objectId, UPDATE_ALL);
}

/*
 * HalfEdges
 */
void VolumeMeshSelectionPlugin::selectAllHalfEdges(int _objectId) {

    BaseObjectData* object = 0;

    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return;
    }

    if (getStatus(_objectId) == NULL)
        return;

    OpenVolumeMesh::StatusAttrib& status = *getStatus(_objectId);

    for(OpenVolumeMesh::StatusAttrib::hestatus_iterator hest_it = status.hestatus_begin();
            hest_it != status.hestatus_end(); ++hest_it) {
        hest_it->set_selected(true);
    }

    emit scriptInfo("selectAllHalfEdges(ObjectId)");
    emit updatedObject(_objectId, UPDATE_SELECTION);
}

void VolumeMeshSelectionPlugin::deselectAllHalfEdges(int _objectId) {

    BaseObjectData* object = 0;

    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return;
    }

    if (getStatus(_objectId) == NULL)
        return;

    OpenVolumeMesh::StatusAttrib& status = *getStatus(_objectId);

    for(OpenVolumeMesh::StatusAttrib::hestatus_iterator hest_it = status.hestatus_begin();
            hest_it != status.hestatus_end(); ++hest_it) {
        hest_it->set_selected(false);
    }

    emit scriptInfo("deselectAllHalfEdges(ObjectId)");
    emit updatedObject(_objectId, UPDATE_SELECTION);
}

void VolumeMeshSelectionPlugin::invertHalfEdgeSelection(int _objectId) {

    BaseObjectData* object = 0;

    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return;
    }

    if (getStatus(_objectId) == NULL)
        return;

    OpenVolumeMesh::StatusAttrib& status = *getStatus(_objectId);

    for(OpenVolumeMesh::StatusAttrib::hestatus_iterator hest_it = status.hestatus_begin();
            hest_it != status.hestatus_end(); ++hest_it) {
        hest_it->set_selected(!hest_it->selected());
    }

    emit scriptInfo("invertHalfEdgeSelection(ObjectId)");
    emit updatedObject(_objectId, UPDATE_SELECTION);
}

void VolumeMeshSelectionPlugin::selectHalfEdges(int _objectId, const IdList& _ids, bool _deselect) {

    if(_ids.empty()) return;

    BaseObjectData* object = 0;

    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return;
    }

    if (getStatus(_objectId) == NULL)
        return;

    OpenVolumeMesh::StatusAttrib& status = *getStatus(_objectId);

    for(IdList::const_iterator v_it = _ids.begin(); v_it != _ids.end(); ++v_it) {
        status[OpenVolumeMesh::HalfEdgeHandle(*v_it)].set_selected(!_deselect);
    }

    QString selection = "selectHalfEdges(ObjectId, [ " + QString::number(_ids[0]);

    for (uint i = 1 ; i < _ids.size(); ++i) {
        selection +=  ", " + QString::number(_ids[i]);
    }

    selection += " ])";

    emit scriptInfo(selection);
    emit updatedObject(_objectId, UPDATE_SELECTION);
}

IdList VolumeMeshSelectionPlugin::getHalfEdgeSelection(int _objectId) {

    BaseObjectData* object = 0;

    IdList list;

    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return list;
    }

    if (getStatus(_objectId) == NULL)
        return list;

    OpenVolumeMesh::StatusAttrib& status = *getStatus(_objectId);

    int id = 0;
    for(OpenVolumeMesh::StatusAttrib::hestatus_iterator hest_it = status.hestatus_begin();
            hest_it != status.hestatus_end(); ++hest_it, ++id) {
        if(hest_it->selected()) list.push_back(id);
    }

    return list;
}

/*
 * Faces
 */
void VolumeMeshSelectionPlugin::selectAllFaces(int _objectId) {

    BaseObjectData* object = 0;

    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return;
    }

    if (getStatus(_objectId) == NULL)
        return;

    OpenVolumeMesh::StatusAttrib& status = *getStatus(_objectId);

    for(OpenVolumeMesh::StatusAttrib::fstatus_iterator fst_it = status.fstatus_begin();
            fst_it != status.fstatus_end(); ++fst_it) {
        fst_it->set_selected(true);
    }

    emit scriptInfo("selectAllFaces(ObjectId)");
    emit updatedObject(_objectId, UPDATE_SELECTION);
}

void VolumeMeshSelectionPlugin::deselectAllFaces(int _objectId) {

    BaseObjectData* object = 0;

    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return;
    }

    if (getStatus(_objectId) == NULL)
        return;

    OpenVolumeMesh::StatusAttrib& status = *getStatus(_objectId);

    for(OpenVolumeMesh::StatusAttrib::fstatus_iterator fst_it = status.fstatus_begin();
            fst_it != status.fstatus_end(); ++fst_it) {
        fst_it->set_selected(false);
    }

    emit scriptInfo("deselectAllFaces(ObjectId)");
    emit updatedObject(_objectId, UPDATE_SELECTION);
}

void VolumeMeshSelectionPlugin::invertFaceSelection(int _objectId) {

    BaseObjectData* object = 0;

    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return;
    }

    if (getStatus(_objectId) == NULL)
        return;

    OpenVolumeMesh::StatusAttrib& status = *getStatus(_objectId);

    for(OpenVolumeMesh::StatusAttrib::fstatus_iterator fst_it = status.fstatus_begin();
            fst_it != status.fstatus_end(); ++fst_it) {
        fst_it->set_selected(!fst_it->selected());
    }

    emit scriptInfo("invertFaceSelection(ObjectId)");
    emit updatedObject(_objectId, UPDATE_SELECTION);
}

void VolumeMeshSelectionPlugin::selectFaces(int _objectId, const IdList& _ids, bool _deselect) {

    if(_ids.empty()) return;

    BaseObjectData* object = 0;

    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return;
    }

    if (getStatus(_objectId) == NULL)
        return;

    OpenVolumeMesh::StatusAttrib& status = *getStatus(_objectId);

    for(IdList::const_iterator v_it = _ids.begin(); v_it != _ids.end(); ++v_it) {
        status[OpenVolumeMesh::FaceHandle(*v_it)].set_selected(!_deselect);
    }

    QString selection = "selectFaces(ObjectId, [ " + QString::number(_ids[0]);

    for (uint i = 1 ; i < _ids.size(); ++i) {
        selection +=  ", " + QString::number(_ids[i]);
    }

    selection += " ])";

    emit scriptInfo(selection);
    emit updatedObject(_objectId, UPDATE_SELECTION);
}

IdList VolumeMeshSelectionPlugin::getFaceSelection(int _objectId) {

    BaseObjectData* object = 0;

    IdList list;

    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return list;
    }

    if (getStatus(_objectId) == NULL)
        return list;

    OpenVolumeMesh::StatusAttrib& status = *getStatus(_objectId);

    int id = 0;
    for(OpenVolumeMesh::StatusAttrib::fstatus_iterator fst_it = status.fstatus_begin();
            fst_it != status.fstatus_end(); ++fst_it, ++id) {
        if(fst_it->selected()) list.push_back(id);
    }

    return list;
}

void VolumeMeshSelectionPlugin::deleteSelectedFaces(int _objectId, bool _preserveManifoldness) {

    BaseObjectData* object = 0;

    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return;
    }

    if (getStatus(_objectId) == NULL)
        return;

    OpenVolumeMesh::StatusAttrib& status = *getStatus(_objectId);

    for(OpenVolumeMesh::StatusAttrib::fstatus_iterator fst_it = status.fstatus_begin();
            fst_it != status.fstatus_end(); ++fst_it) {
        if(fst_it->selected()) fst_it->set_deleted(true);
    }

    status.garbage_collection(_preserveManifoldness);

    emit scriptInfo("deleteSelectedFaces(ObjectId)");
    emit updatedObject(_objectId, UPDATE_ALL);
}

/*
 * HalfFaces
 */
void VolumeMeshSelectionPlugin::selectAllHalfFaces(int _objectId) {

    BaseObjectData* object = 0;

    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return;
    }

    if (getStatus(_objectId) == NULL)
        return;

    OpenVolumeMesh::StatusAttrib& status = *getStatus(_objectId);

    for(OpenVolumeMesh::StatusAttrib::hfstatus_iterator hfst_it = status.hfstatus_begin();
            hfst_it != status.hfstatus_end(); ++hfst_it) {
        hfst_it->set_selected(true);
    }

    emit scriptInfo("selectAllHalfFaces(ObjectId)");
    emit updatedObject(_objectId, UPDATE_SELECTION);
}

void VolumeMeshSelectionPlugin::deselectAllHalfFaces(int _objectId) {

    BaseObjectData* object = 0;

    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return;
    }

    if (getStatus(_objectId) == NULL)
        return;

    OpenVolumeMesh::StatusAttrib& status = *getStatus(_objectId);

    for(OpenVolumeMesh::StatusAttrib::hfstatus_iterator hfst_it = status.hfstatus_begin();
            hfst_it != status.hfstatus_end(); ++hfst_it) {
        hfst_it->set_selected(false);
    }

    emit scriptInfo("deselectAllHalfFaces(ObjectId)");
    emit updatedObject(_objectId, UPDATE_SELECTION);
}

void VolumeMeshSelectionPlugin::invertHalfFaceSelection(int _objectId) {

    BaseObjectData* object = 0;

    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return;
    }

    if (getStatus(_objectId) == NULL)
        return;

    OpenVolumeMesh::StatusAttrib& status = *getStatus(_objectId);

    for(OpenVolumeMesh::StatusAttrib::hfstatus_iterator hfst_it = status.hfstatus_begin();
            hfst_it != status.hfstatus_end(); ++hfst_it) {
        hfst_it->set_selected(!hfst_it->selected());
    }

    emit scriptInfo("invertHalfFaceSelection(ObjectId)");
    emit updatedObject(_objectId, UPDATE_SELECTION);
}

void VolumeMeshSelectionPlugin::selectHalfFaces(int _objectId, const IdList& _ids, bool _deselect) {

    if(_ids.empty()) return;

    BaseObjectData* object = 0;

    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return;
    }

    if (getStatus(_objectId) == NULL)
        return;

    OpenVolumeMesh::StatusAttrib& status = *getStatus(_objectId);

    for(IdList::const_iterator v_it = _ids.begin(); v_it != _ids.end(); ++v_it) {
        status[OpenVolumeMesh::HalfFaceHandle(*v_it)].set_selected(!_deselect);
    }

    QString selection = "selectHalfFaces(ObjectId, [ " + QString::number(_ids[0]);

    for (uint i = 1 ; i < _ids.size(); ++i) {
        selection +=  ", " + QString::number(_ids[i]);
    }

    selection += " ])";

    emit scriptInfo(selection);
    emit updatedObject(_objectId, UPDATE_SELECTION);
}

IdList VolumeMeshSelectionPlugin::getHalfFaceSelection(int _objectId) {

    BaseObjectData* object = 0;

    IdList list;

    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return list;
    }

    if (getStatus(_objectId) == NULL)
        return list;

    OpenVolumeMesh::StatusAttrib& status = *getStatus(_objectId);

    int id = 0;
    for(OpenVolumeMesh::StatusAttrib::hfstatus_iterator hfst_it = status.hfstatus_begin();
            hfst_it != status.hfstatus_end(); ++hfst_it, ++id) {
        if(hfst_it->selected()) list.push_back(id);
    }

    return list;
}
/*
 * Cells
 */
void VolumeMeshSelectionPlugin::selectAllCells(int _objectId) {

    BaseObjectData* object = 0;

    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return;
    }

    if (getStatus(_objectId) == NULL)
        return;

    OpenVolumeMesh::StatusAttrib& status = *getStatus(_objectId);

    for(OpenVolumeMesh::StatusAttrib::cstatus_iterator cst_it = status.cstatus_begin();
            cst_it != status.cstatus_end(); ++cst_it) {
        cst_it->set_selected(true);
    }

    emit scriptInfo("selectAllCells(ObjectId)");
    emit updatedObject(_objectId, UPDATE_SELECTION);
}

void VolumeMeshSelectionPlugin::deselectAllCells(int _objectId) {

    BaseObjectData* object = 0;

    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return;
    }

    if (getStatus(_objectId) == NULL)
        return;

    OpenVolumeMesh::StatusAttrib& status = *getStatus(_objectId);

    for(OpenVolumeMesh::StatusAttrib::cstatus_iterator cst_it = status.cstatus_begin();
            cst_it != status.cstatus_end(); ++cst_it) {
        cst_it->set_selected(false);
    }

    emit scriptInfo("deselectAllCells(ObjectId)");
    emit updatedObject(_objectId, UPDATE_SELECTION);
}

void VolumeMeshSelectionPlugin::invertCellSelection(int _objectId) {

    BaseObjectData* object = 0;

    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return;
    }

    if (getStatus(_objectId) == NULL)
        return;

    OpenVolumeMesh::StatusAttrib& status = *getStatus(_objectId);

    for(OpenVolumeMesh::StatusAttrib::cstatus_iterator cst_it = status.cstatus_begin();
            cst_it != status.cstatus_end(); ++cst_it) {
        cst_it->set_selected(!cst_it->selected());
    }

    emit scriptInfo("invertCellSelection(ObjectId)");
    emit updatedObject(_objectId, UPDATE_SELECTION);
}

void VolumeMeshSelectionPlugin::selectCells(int _objectId, const IdList& _ids, bool _deselect) {

    if(_ids.empty()) return;

    BaseObjectData* object = 0;

    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return;
    }

    if (getStatus(_objectId) == NULL)
        return;

    OpenVolumeMesh::StatusAttrib& status = *getStatus(_objectId);

    for(IdList::const_iterator v_it = _ids.begin(); v_it != _ids.end(); ++v_it) {
        status[OpenVolumeMesh::CellHandle(*v_it)].set_selected(!_deselect);
    }

    QString selection = "selectCells(ObjectId, [ " + QString::number(_ids[0]);

    for (uint i = 1 ; i < _ids.size(); ++i) {
        selection +=  ", " + QString::number(_ids[i]);
    }

    selection += " ])";

    emit scriptInfo(selection);
    emit updatedObject(_objectId, UPDATE_SELECTION);
}

IdList VolumeMeshSelectionPlugin::getCellSelection(int _objectId) {

    BaseObjectData* object = 0;

    IdList list;

    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return list;
    }

    if (getStatus(_objectId) == NULL)
        return list;

    OpenVolumeMesh::StatusAttrib& status = *getStatus(_objectId);

    int id = 0;
    for(OpenVolumeMesh::StatusAttrib::cstatus_iterator cst_it = status.cstatus_begin();
            cst_it != status.cstatus_end(); ++cst_it, ++id) {
        if(cst_it->selected()) list.push_back(id);
    }

    return list;
}

void VolumeMeshSelectionPlugin::deleteSelectedCells(int _objectId, bool _preserveManifoldness) {

    BaseObjectData* object = 0;

    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return;
    }

    if (getStatus(_objectId) == NULL)
        return;

    OpenVolumeMesh::StatusAttrib& status = *getStatus(_objectId);

    for(OpenVolumeMesh::StatusAttrib::cstatus_iterator cst_it = status.cstatus_begin();
            cst_it != status.cstatus_end(); ++cst_it) {
        if(cst_it->selected()) cst_it->set_deleted(true);
    }

    status.garbage_collection(_preserveManifoldness);

    emit scriptInfo("deleteSelectedCells(ObjectId)");
    emit updatedObject(_objectId, UPDATE_ALL);
}
