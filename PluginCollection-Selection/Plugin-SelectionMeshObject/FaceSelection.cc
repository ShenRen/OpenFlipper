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

#include <MeshTools/MeshSelectionT.hh>

//=========================================================
//==== Face selections
//=========================================================

void MeshObjectSelectionPlugin::selectFaces(int objectId , IdList _faceList) {
    
    if(_faceList.empty() ) return;
    
    BaseObjectData* object = 0;
    if (! PluginFunctions::getObject(objectId,object)) {
        emit log(LOGERR,tr("selectFaces : unable to get object"));
        return;
    }

    if (object->dataType() == DATA_TRIANGLE_MESH)
        MeshSelection::selectFaces(PluginFunctions::triMesh(object), _faceList);
    else if (object->dataType() == DATA_POLY_MESH)
        MeshSelection::selectFaces(PluginFunctions::polyMesh(object), _faceList);
    else {
        emit log(LOGERR,tr("selectFaces : Unsupported object Type"));
        return;
    }

    QString selection = "selectFaces(ObjectId(" + QString::number(objectId) + ") , [ " + QString::number(_faceList[0]);

    for (uint i = 1 ; i < _faceList.size(); ++i) {
        selection +=  " , " + QString::number(_faceList[i]);
    }

    selection += " ])";

    emit updatedObject(object->id(), UPDATE_SELECTION_FACES);
    emit scriptInfo(selection);
}

//=========================================================

void MeshObjectSelectionPlugin::unselectFaces(int objectId , IdList _faceList) {
    
    if(_faceList.empty() ) return;
    
    BaseObjectData* object = 0;
    if (! PluginFunctions::getObject(objectId,object)) {
        emit log(LOGERR,tr("unselectFaces : unable to get object"));
        return;
    }

    if (object->dataType() == DATA_TRIANGLE_MESH)
        MeshSelection::unselectFaces(PluginFunctions::triMesh(object), _faceList);
    else if (object->dataType() == DATA_POLY_MESH)
        MeshSelection::unselectFaces(PluginFunctions::polyMesh(object), _faceList);
    else {
        emit log(LOGERR,tr("unselectFaces : Unsupported object Type"));
        return;
    }

    QString selection = "unselectFaces(ObjectId(" + QString::number(objectId) + ") , [ " + QString::number(_faceList[0]);

    for (uint i = 1 ; i < _faceList.size(); ++i) {
        selection +=  " , " + QString::number(_faceList[i]);
    }

    selection += " ])";

    emit updatedObject(object->id(), UPDATE_SELECTION_FACES);
    emit scriptInfo(selection);
}

//=========================================================

void MeshObjectSelectionPlugin::selectAllFaces(int objectId) {
    
    BaseObjectData* object;
    if (! PluginFunctions::getObject(objectId,object)) {
        emit log(LOGERR,tr("selectAllFaces : unable to get object"));
        return;
    }

    if (object->dataType() == DATA_TRIANGLE_MESH)
        MeshSelection::selectAllFaces(PluginFunctions::triMesh(object));
    else if (object->dataType() == DATA_POLY_MESH)
        MeshSelection::selectAllFaces(PluginFunctions::polyMesh(object));
    else {
        emit log(LOGERR,tr("selectAllFaces : Unsupported object Type"));
        return;
    }

    emit updatedObject(object->id(), UPDATE_SELECTION_FACES);
    emit scriptInfo("selectAllFaces(ObjectId(" + QString::number(objectId) + "))");
}

//=========================================================

void MeshObjectSelectionPlugin::clearFaceSelection(int objectId) {
    
    BaseObjectData* object;
    if (! PluginFunctions::getObject(objectId,object)) {
        emit log(LOGERR,tr("clearFaceSelection : unable to get object"));
        return;
    }

    if (object->dataType() == DATA_TRIANGLE_MESH)
        MeshSelection::clearFaceSelection(PluginFunctions::triMesh(object));
    else if (object->dataType() == DATA_POLY_MESH)
        MeshSelection::clearFaceSelection(PluginFunctions::polyMesh(object));
    else {
        emit log(LOGERR,tr("clearFaceSelection : Unsupported object Type"));
        return;
    }

    emit updatedObject(object->id(), UPDATE_SELECTION_FACES);
    emit scriptInfo("clearFaceSelection(ObjectId(" + QString::number(objectId) + "))");
}

//=========================================================

void MeshObjectSelectionPlugin::invertFaceSelection(int objectId) {
    
    BaseObjectData* object;
    if (! PluginFunctions::getObject(objectId,object)) {
        emit log(LOGERR,tr("invertFaceSelection : unable to get object"));
        return;
    }

    if (object->dataType() == DATA_TRIANGLE_MESH)
        MeshSelection::invertFaceSelection(PluginFunctions::triMesh(object));
    else if (object->dataType() == DATA_POLY_MESH)
        MeshSelection::invertFaceSelection(PluginFunctions::polyMesh(object));
    else {
        emit log(LOGERR,tr("invertFaceSelection : Unsupported object Type"));
        return;
    }

    emit updatedObject(object->id(), UPDATE_SELECTION_FACES);
    emit scriptInfo("invertFaceSelection(ObjectId(" + QString::number(objectId) + "))");
}

//=========================================================

void MeshObjectSelectionPlugin::deleteFaceSelection(int _objectId) {

    BaseObjectData* object = 0;
    if (!PluginFunctions::getObject(_objectId,object)) {
        emit log(LOGERR,tr("deleteFaceSelection: unable to get object"));
        return;
    }

    if (object->dataType() == DATA_TRIANGLE_MESH)
        deleteSelection(PluginFunctions::triMesh(object), faceType_);
    else if (object->dataType() == DATA_POLY_MESH)
        deleteSelection(PluginFunctions::polyMesh(object), faceType_);
    else {
        emit log(LOGERR,tr("deleteFaceSelection: Unsupported object Type"));
        return;
    }

    emit updatedObject(object->id(), UPDATE_ALL);
    emit scriptInfo("deleteFaceSelection(ObjectId(" + QString::number(_objectId) + "))");
}

//=========================================================

int MeshObjectSelectionPlugin::createMeshFromFaceSelection( int _objectId) {
  return createMeshFromSelection(_objectId, faceType_ );
}

//=========================================================

void MeshObjectSelectionPlugin::selectBoundaryFaces(int objectId) {
    
    BaseObjectData* object;
    if (! PluginFunctions::getObject(objectId,object)) {
        emit log(LOGERR,tr("selectBoundaryFaces : unable to get object"));
        return;
    }

    if (object->dataType() == DATA_TRIANGLE_MESH)
        MeshSelection::selectBoundaryFaces(PluginFunctions::triMesh(object));
    else if (object->dataType() == DATA_POLY_MESH)
        MeshSelection::selectBoundaryFaces(PluginFunctions::polyMesh(object));
    else {
        emit log(LOGERR,tr("selectBoundaryFaces : Unsupported object Type"));
        return;
    }

    emit updatedObject(object->id(), UPDATE_SELECTION_FACES);
    emit scriptInfo("selectBoundaryFaces(ObjectId(" + QString::number(objectId) + "))");
}


//=========================================================

void MeshObjectSelectionPlugin::shrinkFaceSelection(int objectId) {
    
    BaseObjectData* object;
    if (! PluginFunctions::getObject(objectId,object)) {
        emit log(LOGERR,tr("shrinkFaceSelection : unable to get object"));
        return;
    }

    if (object->dataType() == DATA_TRIANGLE_MESH)
        MeshSelection::shrinkFaceSelection(PluginFunctions::triMesh(object));
    else if (object->dataType() == DATA_POLY_MESH)
        MeshSelection::shrinkFaceSelection(PluginFunctions::polyMesh(object));
    else {
        emit log(LOGERR,tr("shrinkFaceSelection : Unsupported object Type"));
        return;
    }

    emit updatedObject(object->id(), UPDATE_SELECTION_FACES);
    emit scriptInfo("shrinkFaceSelection(ObjectId(" + QString::number(objectId) + "))");
}

//=========================================================

void MeshObjectSelectionPlugin::growFaceSelection(int objectId) {
    
    BaseObjectData* object;
    if (! PluginFunctions::getObject(objectId,object)) {
        emit log(LOGERR,tr("growFaceSelection : unable to get object"));
        return;
    }

    if (object->dataType() == DATA_TRIANGLE_MESH)
        MeshSelection::growFaceSelection(PluginFunctions::triMesh(object));
    else if (object->dataType() == DATA_POLY_MESH)
        MeshSelection::growFaceSelection(PluginFunctions::polyMesh(object));
    else {
        emit log(LOGERR,tr("growFaceSelection : Unsupported object Type"));
        return;
    }

    emit updatedObject(object->id(), UPDATE_SELECTION_FACES);
    emit scriptInfo("growFaceSelection(ObjectId(" + QString::number(objectId) + "))");
}

//=========================================================

IdList MeshObjectSelectionPlugin::getFaceSelection(int objectId) {
    
    BaseObjectData* object;
    if (! PluginFunctions::getObject(objectId,object)) {
        emit log(LOGERR,tr("getFaceSelection : unable to get object"));
        return IdList(0);
    }

    emit scriptInfo("getFaceSelection(ObjectId(" + QString::number(objectId) + "))");

    if (object->dataType() == DATA_TRIANGLE_MESH)
        return MeshSelection::getFaceSelection(PluginFunctions::triMesh(object));
    else if (object->dataType() == DATA_POLY_MESH)
        return MeshSelection::getFaceSelection(PluginFunctions::polyMesh(object));
    else {
        emit log(LOGERR,tr("getFaceSelection : Unsupported object Type"));
        return IdList(0);
    }
}

//=========================================================

/// colorize the face selection
void MeshObjectSelectionPlugin::colorizeFaceSelection(int objectId, int r, int g, int b, int a) {
    
    BaseObjectData* object;
    if (! PluginFunctions::getObject(objectId,object)) {
        emit log(LOGERR,"colorizeFaceSelection : unable to get object");
        return;
    }

    if (object->dataType() == DATA_TRIANGLE_MESH) {
        colorizeSelection(PluginFunctions::triMesh(object), faceType_, r, g, b, a);
    } else if (object->dataType() == DATA_POLY_MESH) {
        colorizeSelection(PluginFunctions::polyMesh(object), faceType_, r, g, b, a);
    } else {
        emit log(LOGERR,"colorizeFaceSelection : Unsupported object Type");
        return;
    }


    emit scriptInfo("colorizeFaceSelection(ObjectId(" + QString::number(objectId) + "), "
                     + QString::number(r) + ", " + QString::number(g) + ", " + QString::number(b) + ")");

    emit updatedObject(objectId, UPDATE_COLOR);
}
