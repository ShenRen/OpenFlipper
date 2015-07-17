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


#include "PolyLineSelectionPlugin.hh"

void PolyLineSelectionPlugin::selectAllVertices(int _objectId) {
    
    BaseObjectData* object = 0;
    
    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return;
    }
    
    PolyLine* polyline = PluginFunctions::polyLine(object);
    if(polyline) {
        
        for(unsigned int i = 0; i < polyline->n_vertices(); ++i) {
            polyline->select_vertex(i);
        }
    }
    
    emit scriptInfo("selectAllVertices(ObjectId)");
}

void PolyLineSelectionPlugin::deselectAllVertices(int _objectId) {
    
    BaseObjectData* object = 0;
    
    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return;
    }
    
    PolyLine* polyline = PluginFunctions::polyLine(object);
    if(polyline) {
        
        for(unsigned int i = 0; i < polyline->n_vertices(); ++i) {
            polyline->deselect_vertex(i);
        }
    }
    
    emit scriptInfo("deselectAllVertices(ObjectId)");
}

void PolyLineSelectionPlugin::invertVertexSelection(int _objectId) {
    
    BaseObjectData* object = 0;
    
    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return;
    }
    
    PolyLine* polyline = PluginFunctions::polyLine(object);
    if(polyline) {
        
        for(unsigned int i = 0; i < polyline->n_vertices(); ++i) {
            
            if(polyline->vertex_selected(i))
                polyline->deselect_vertex(i);
            else
                polyline->select_vertex(i);
        }
    }
    
    emit scriptInfo("invertVertexSelection(ObjectId)");
}

void PolyLineSelectionPlugin::deleteSelectedVertices(int _objectId) {
    
    BaseObjectData* object = 0;
    
    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return;
    }
    
    PolyLine* polyline = PluginFunctions::polyLine(object);
    if(polyline) {
        
        bool breakWhile = false;
        while(!breakWhile) {
            
            // Go over all vertices and delete the selected ones
            bool oneFound = false;
            unsigned int i = 0;
            for(; i < polyline->n_vertices(); ++i) {
            
                if(polyline->vertex_selected(i)) {
                    polyline->delete_point(i);
                    oneFound = true;
                }
                if(oneFound)
                    break;
            }
            
            if(i == polyline->n_vertices() && !oneFound) {
                // We are through
                breakWhile = true;
            }
        }
    }
    
    emit scriptInfo("deleteSelectedVertices(ObjectId)");
}

void PolyLineSelectionPlugin::selectVertices(int _objectId, const IdList& _ids, bool _deselect) {
   
    if(_ids.empty() ) return;
    
    BaseObjectData* object = 0;
    
    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return;
    }
    
    PolyLine* polyline = PluginFunctions::polyLine(object);
    if(polyline) {
        for(IdList::const_iterator it = _ids.begin(); it != _ids.end(); ++it) {
            if(*it < (int)polyline->n_vertices()) {
                if(_deselect) polyline->deselect_vertex(*it);
                else          polyline->select_vertex(*it);
            }
        }
    }
    
    QString selection = "selectVertices(ObjectId, [ " + QString::number(_ids[0]);

    for (uint i = 1 ; i < _ids.size(); ++i) {
        selection +=  ", " + QString::number(_ids[i]);
    }

    selection += " ])";

    emit scriptInfo(selection);
}

IdList PolyLineSelectionPlugin::getVertexSelection(int _objectId) {
    
    BaseObjectData* object = 0;
    
    IdList list;
    
    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return list;
    }
    
    PolyLine* polyline = PluginFunctions::polyLine(object);
    if(polyline) {
        
        for(unsigned int i = 0; i < polyline->n_vertices(); ++i) {
            if(polyline->vertex_selected(i))
                list.push_back(i);
        }
    }
    
    return list;
}

//=======================================================================================

void PolyLineSelectionPlugin::selectAllEdges(int _objectId) {

    BaseObjectData* object = 0;

    if (!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return;
    }

    PolyLine* polyline = PluginFunctions::polyLine(object);
    if (polyline) {
        
        for(unsigned int i = 0; i < polyline->n_edges(); ++i) {
            polyline->select_edge(i);
        }
    }

    emit scriptInfo("selectAllEdges(ObjectId)");
}

void PolyLineSelectionPlugin::deselectAllEdges(int _objectId) {

    BaseObjectData* object = 0;

    if (!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return;
    }

    PolyLine* polyline = PluginFunctions::polyLine(object);
    if (polyline) {
        
        for(unsigned int i = 0; i < polyline->n_edges(); ++i) {
            polyline->deselect_edge(i);
        }
    }

    emit scriptInfo("deselectAllEdges(ObjectId)");
}

void PolyLineSelectionPlugin::invertEdgeSelection(int _objectId) {

    BaseObjectData* object = 0;

    if (!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return;
    }

    PolyLine* polyline = PluginFunctions::polyLine(object);
    if (polyline) {
        
        for(unsigned int i = 0; i < polyline->n_edges(); ++i) {
            if(polyline->edge_selected(i))
                polyline->deselect_edge(i);
            else
                polyline->select_edge(i);
        }
    }

    emit scriptInfo("deselectAllEdges(ObjectId)");
}

void PolyLineSelectionPlugin::deleteSelectedEdges(int _objectId) {

    BaseObjectData* object = 0;
    
    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return;
    }
    
    PolyLine* polyline = PluginFunctions::polyLine(object);
    if(polyline) {
        
        bool breakWhile = false;
        while(!breakWhile) {
            
            // Go over all vertices and delete the selected ones
            bool oneFound = false;
            unsigned int i = 0;
            for(; i < polyline->n_edges(); ++i) {
            
                if(polyline->edge_selected(i)) {
                    if(i+1 < polyline->n_vertices() /*Should always be the case*/) {
                        
                        if(!polyline->is_closed() && (i == 0 || i == polyline->n_edges()-1)) {
                            // Test if we're considering a boundary edge
                            // (only in not closed polylines)
                            
                            // Only delete one point (the boundary vertex)
                            if(i == 0)
                                polyline->delete_point(i);
                            else
                                polyline->delete_point(i+1);
                            
                            oneFound = true;
                            
                        } else {
                            // Delete both incident points
                            polyline->delete_point(i);
                            // Note: Index i+1 is now i after deletion
                            polyline->delete_point(i);
                            oneFound = true;
                        }
                    }
                }
                if(oneFound)
                    break;
            }
            
            if(i == polyline->n_edges() && !oneFound) {
                // We are through
                breakWhile = true;
            }
        }
    }
    
    emit scriptInfo("deleteSelectedEdges(ObjectId)");
}

void PolyLineSelectionPlugin::selectEdges(int _objectId, const IdList& _ids, bool _deselect) {

    if(_ids.empty() ) return;
    
    BaseObjectData* object = 0;

    if (!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return;
    }

    PolyLine* polyline = PluginFunctions::polyLine(object);
    if (polyline) {
        for (IdList::const_iterator it = _ids.begin(); it != _ids.end(); ++it) {
            
            if(*it < (int)polyline->n_edges()) {
                if(_deselect) polyline->deselect_edge(*it);
                else          polyline->select_edge(*it);
            }
        }
    }

    QString selection = "selectEdges(ObjectId, [ " + QString::number(_ids[0]);

    for (uint i = 1 ; i < _ids.size(); ++i) {
        selection +=  ", " + QString::number(_ids[i]);
    }

    selection += " ])";

    emit scriptInfo(selection);
}

IdList PolyLineSelectionPlugin::getEdgeSelection(int _objectId) {

    BaseObjectData* object = 0;
    
    IdList list;
    
    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return list;
    }
    
    PolyLine* polyline = PluginFunctions::polyLine(object);
    if(polyline) {
        
        for(unsigned int i = 0; i < polyline->n_edges(); ++i) {
            if(polyline->edge_selected(i))
                list.push_back(i);
        }
    }
    
    return list;
}
