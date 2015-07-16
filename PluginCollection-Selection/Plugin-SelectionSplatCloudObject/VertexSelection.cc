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
 *   $Revision$                                                      *
 *   $Author$                                                       *
 *   $Date$                   *
 *                                                                           *
\*===========================================================================*/

//================================================================
//
//  CLASS SplatCloudObjectSelectionPlugin - IMPLEMENTATION (continued)
//
//================================================================


//== INCLUDES ====================================================


#include "SplatCloudObjectSelectionPlugin.hh"
#include "SplatCloudSelection.hh"

#include <OpenFlipper/BasePlugin/PluginFunctions.hh>

#include <iostream>


//== IMPLEMENTATION ==============================================


//=========================================================
//==== Vertex selections
//=========================================================


void SplatCloudObjectSelectionPlugin::selectVertices(int _objectId, IdList _vertexList) {
    
    if( _vertexList.empty() ) return;
    
    BaseObjectData* object = 0;
    if (!PluginFunctions::getObject(_objectId,object)) {
        emit log(LOGERR,tr("selectVertices: unable to get object")); 
        return;
    }

    if (object->dataType() == DATA_SPLATCLOUD)
        SplatCloudSelection::selectVertices(PluginFunctions::splatCloud(object), _vertexList);
    else {
        emit log(LOGERR,tr("selectAllVertices: Unsupported object Type")); 
        return;
    }

    QString selection = "selectVertices(ObjectId, [ " + QString::number(_vertexList[0]);

    for (uint i = 1 ; i < _vertexList.size(); ++i) {
        selection +=  ", " + QString::number(_vertexList[i]);
    }

    selection += " ])";

    emit updatedObject(object->id(), UPDATE_SELECTION_VERTICES);
    emit scriptInfo(selection);
}


//----------------------------------------------------------------


void SplatCloudObjectSelectionPlugin::unselectVertices(int _objectId, IdList _vertexList) {
    
    if( _vertexList.empty() ) return;
    
    BaseObjectData* object = 0;
    if (!PluginFunctions::getObject(_objectId,object)) {
        emit log(LOGERR,tr("unselectVertices: unable to get object")); 
        return;
    }

    if (object->dataType() == DATA_SPLATCLOUD)
        SplatCloudSelection::unselectVertices(PluginFunctions::splatCloud(object), _vertexList);
    else {
        emit log(LOGERR,tr("unselectVertices: Unsupported object Type")); 
        return;
    }

    QString selection = "unselectVertices(ObjectId, [ " + QString::number(_vertexList[0]);

    for (uint i = 1 ; i < _vertexList.size(); ++i) {
        selection +=  ", " + QString::number(_vertexList[i]);
    }

    selection += " ])";

    emit updatedObject(object->id(), UPDATE_SELECTION_VERTICES);
    emit scriptInfo(selection);
}


//----------------------------------------------------------------


void SplatCloudObjectSelectionPlugin::selectAllVertices(int _objectId) {
  
    BaseObjectData* object;
    if (!PluginFunctions::getObject(_objectId,object)) {
        emit log(LOGERR,tr("selectAllVertices: unable to get object")); 
        return;
    }

    if (object->dataType() == DATA_SPLATCLOUD)
        SplatCloudSelection::selectAllVertices(PluginFunctions::splatCloud(object));
    else {
        emit log(LOGERR,tr("selectAllVertices: Unsupported object Type")); 
        return;
    }

    emit updatedObject(object->id(), UPDATE_SELECTION_VERTICES);
    emit scriptInfo("selectAllVertices(ObjectId)");
}


//----------------------------------------------------------------


void SplatCloudObjectSelectionPlugin::clearVertexSelection(int _objectId) {
    
    BaseObjectData* object;
    if (!PluginFunctions::getObject(_objectId,object)) {
        emit log(LOGERR,tr("clearVertexSelection: unable to get object")); 
        return;
    }

    if (object->dataType() == DATA_SPLATCLOUD)
        SplatCloudSelection::clearVertexSelection(PluginFunctions::splatCloud(object));
    else {
        emit log(LOGERR,tr("clearVertexSelection: Unsupported object Type")); 
        return;
    }

    emit updatedObject(object->id(), UPDATE_SELECTION_VERTICES);
    emit scriptInfo("clearVertexSelection(ObjectId)");
}


//----------------------------------------------------------------


void SplatCloudObjectSelectionPlugin::invertVertexSelection(int _objectId) {
  
    BaseObjectData* object;
    if (!PluginFunctions::getObject(_objectId,object)) {
        emit log(LOGERR,tr("invertVertexSelection: unable to get object")); 
        return;
    }

    if (object->dataType() == DATA_SPLATCLOUD)
        SplatCloudSelection::invertVertexSelection(PluginFunctions::splatCloud(object));
    else {
        emit log(LOGERR,tr("invertVertexSelection: Unsupported object Type")); 
        return;
    }

    emit updatedObject(object->id(), UPDATE_SELECTION_VERTICES);
    emit scriptInfo("invertVertexSelection(ObjectId)");
}


//----------------------------------------------------------------


IdList SplatCloudObjectSelectionPlugin::getVertexSelection(int _objectId) {
  
    BaseObjectData* object;
    if (!PluginFunctions::getObject(_objectId,object)) {
        emit log(LOGERR,tr("getVertexSelection: unable to get object")); 
        return IdList(0);
    }

    emit scriptInfo("getVertexSelection(ObjectId)");

    if (object->dataType() == DATA_SPLATCLOUD)
        return SplatCloudSelection::getVertexSelection(PluginFunctions::splatCloud(object));
    else {
        emit log(LOGERR,tr("getVertexSelection: Unsupported object Type")); 
        return IdList(0);
    }

    return IdList(0);
}


//----------------------------------------------------------------


void SplatCloudObjectSelectionPlugin::deleteVertexSelection(int _objectId) {
  
    BaseObjectData* object;
    if (!PluginFunctions::getObject(_objectId,object)) {
        emit log(LOGERR,tr("deleteVertexSelection: unable to get object"));
        return;
    }

    if (object->dataType() == DATA_SPLATCLOUD)
        splatCloudDeleteSelection(PluginFunctions::splatCloud(object), vertexType_);
    else {
        emit log(LOGERR,tr("deleteVertexSelection: Unsupported object Type"));
        return;
    }

    emit updatedObject(object->id(), UPDATE_ALL);
    emit scriptInfo("deleteVertexSelection(ObjectId)");
}


//----------------------------------------------------------------


void SplatCloudObjectSelectionPlugin::colorizeVertexSelection(int _objectId, int r, int g, int b, int a) {
  
    BaseObjectData* object;
    if (!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR,"colorizeVertexSelection: unable to get object");
        return;
    }

    if (object->dataType() == DATA_SPLATCLOUD) {
        splatCloudColorizeSelection(PluginFunctions::splatCloud(object), vertexType_, r, g, b, a);
    } else {
        emit log(LOGERR,"colorizeVertexSelection: Unsupported object Type");
        return;
    }

    emit scriptInfo("colorizeVertexSelection(ObjectId, "
              + QString::number(r) + ", " + QString::number(g) + ", " + QString::number(b) + ")");

    emit updatedObject(_objectId, UPDATE_COLOR);
}
