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

#include "BSplineCurveSelectionPlugin.hh"

void BSplineCurveSelectionPlugin::selectAllControlPoints(int _objectId) {
    
    BaseObjectData* object = 0;
    
    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return;
    }
    
    BSplineCurve* curve = PluginFunctions::splineCurve(object);
    BSplineCurveObject* co = PluginFunctions::bsplineCurveObject(object);
    
    if(curve) {    
        for(unsigned int i = 0; i < curve->n_control_points(); ++i) {
            curve->select_controlpoint(i);
        }
    }
    
    // Switch to control point selection mode
    if(co)
        co->splineCurveNode()->set_selection_draw_mode(ACG::SceneGraph::BSplineCurveNodeT<BSplineCurve>::CONTROLPOINT);
    
    emit scriptInfo("selectAllControlPoints(ObjectId)");
}

void BSplineCurveSelectionPlugin::deselectAllControlPoints(int _objectId) {
    
    BaseObjectData* object = 0;
    
    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return;
    }
    
    BSplineCurve* curve = PluginFunctions::splineCurve(object);
    if(curve) {
        for(unsigned int i = 0; i < curve->n_control_points(); ++i) {
            curve->deselect_controlpoint(i);
        }
    }
    
    emit scriptInfo("deselectAllControlPoints(ObjectId)");
}

void BSplineCurveSelectionPlugin::invertControlPointSelection(int _objectId) {
    
    BaseObjectData* object = 0;
    
    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return;
    }
    
    BSplineCurve* curve = PluginFunctions::splineCurve(object);
    BSplineCurveObject* co = PluginFunctions::bsplineCurveObject(object);
    
    if(curve) {    
        for(unsigned int i = 0; i < curve->n_control_points(); ++i) {
            if(curve->controlpoint_selected(i))
                curve->deselect_controlpoint(i);
            else
                curve->select_controlpoint(i);
        }
    }
    
    // Switch to control point selection mode
    if(co)
        co->splineCurveNode()->set_selection_draw_mode(ACG::SceneGraph::BSplineCurveNodeT<BSplineCurve>::CONTROLPOINT);
    
    emit scriptInfo("invertControlPointSelection(ObjectId)");
}

void BSplineCurveSelectionPlugin::deleteSelectedControlPoints(int _objectId) {
    
    BaseObjectData* object = 0;
    
    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return;
    }
    
    BSplineCurve* curve = PluginFunctions::splineCurve(object);
    if(curve) {

        // I know this is a bit crappy, but control point indices
        // change forcably after each delete operation so we have to
        // start over each time...
        bool breakWhile = false;
        while(!breakWhile) {
            bool oneFound = false;
            unsigned int i = 0;
            for(; i < curve->n_control_points(); ++i) {
                if(curve->controlpoint_selected(i)) {
                    curve->delete_control_point(i);
                    oneFound = true;
                    break;
                }
            }
            
            if((i >= curve->n_control_points()) && !oneFound) {
                // We are through
                breakWhile = true;
            }
        }
    }
    
    emit scriptInfo("deleteSelectedControlPoints(ObjectId)");
}

void BSplineCurveSelectionPlugin::selectControlPoints(int _objectId, const IdList& _ids, bool _deselect) {
    
    if( _ids.empty()) return;
    
    BaseObjectData* object = 0;
    
    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return;
    }
    
    BSplineCurve* curve = PluginFunctions::splineCurve(object);
    if(curve) {
        for(IdList::const_iterator it = _ids.begin(); it != _ids.end(); ++it) {
            if(*it < (int)curve->n_control_points()) {
                if(_deselect) curve->deselect_controlpoint(*it);
                else curve->select_controlpoint(*it);
            }
        }
    }
    
    QString selection = "selectControlPoints(ObjectId, [ " + QString::number(_ids[0]);

    for (uint i = 1 ; i < _ids.size(); ++i) {
        selection +=  ", " + QString::number(_ids[i]);
    }

    selection += " ])";
    

    emit scriptInfo(selection);
}

IdList BSplineCurveSelectionPlugin::getControlPointSelection(int _objectId) {
    
    BaseObjectData* object = 0;
    
    IdList list;
    
    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return list;
    }
    
    BSplineCurve* curve = PluginFunctions::splineCurve(object);
    if(curve) {
        for(unsigned int i = 0; i < curve->n_control_points(); ++i) {
            if(curve->controlpoint_selected(i))
                list.push_back((int)i);
        }
    }
    
    return list;
}

//=====================================================================

void BSplineCurveSelectionPlugin::selectAllKnots(int _objectId) {
    
    BaseObjectData* object = 0;
    
    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return;
    }
    
    BSplineCurve* curve = PluginFunctions::splineCurve(object);
    BSplineCurveObject* co = PluginFunctions::bsplineCurveObject(object);
    
    if(curve) {    
        for(unsigned int i = 0; i < curve->n_knots(); ++i) {
            curve->get_knotvector_ref()->select(i);
        }
    }
    
    // Switch to control point selection mode
    if(co)
        co->splineCurveNode()->set_selection_draw_mode(ACG::SceneGraph::BSplineCurveNodeT<BSplineCurve>::KNOTVECTOR);
    
    emit scriptInfo("selectAllKnots(ObjectId)");
}

void BSplineCurveSelectionPlugin::deselectAllKnots(int _objectId) {
    
    BaseObjectData* object = 0;
    
    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return;
    }
    
    BSplineCurve* curve = PluginFunctions::splineCurve(object);
    
    if(curve) {    
        for(unsigned int i = 0; i < curve->n_knots(); ++i) {
            curve->get_knotvector_ref()->deselect(i);
        }
    }
    
    emit scriptInfo("deselectAllKnots(ObjectId)");
}

void BSplineCurveSelectionPlugin::invertKnotSelection(int _objectId) {
    
    BaseObjectData* object = 0;
    
    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return;
    }
    
    BSplineCurve* curve = PluginFunctions::splineCurve(object);
    BSplineCurveObject* co = PluginFunctions::bsplineCurveObject(object);
    
    if(curve) {    
        for(unsigned int i = 0; i < curve->n_knots(); ++i) {
            if(curve->get_knotvector_ref()->selected(i))
                curve->get_knotvector_ref()->deselect(i);
            else
                curve->get_knotvector_ref()->select(i);
        }
    }
    
    // Switch to control point selection mode
    if(co)
        co->splineCurveNode()->set_selection_draw_mode(ACG::SceneGraph::BSplineCurveNodeT<BSplineCurve>::KNOTVECTOR);
    
    emit scriptInfo("invertKnotSelection(ObjectId)");
}

void BSplineCurveSelectionPlugin::deleteSelectedKnots(int _objectId) {
    
    BaseObjectData* object = 0;
    
    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return;
    }
    
    BSplineCurve* curve = PluginFunctions::splineCurve(object);
    if(curve) {

        // I know this is a bit crappy, but knot indices
        // change forcably after each delete operation so we have to
        // start over each time...
        bool breakWhile = false;
        while(!breakWhile) {
            bool oneFound = false;
            unsigned int i = 0;
            for(; i < curve->n_knots(); ++i) {
                if(curve->get_knotvector_ref()->selected(i)) {
                    curve->get_knotvector_ref()->deleteKnot(i);
                    oneFound = true;
                    break;
                }
            }
            
            if((i >= curve->n_knots()) && !oneFound) {
                // We are through
                breakWhile = true;
            }
        }
    }
    
    emit scriptInfo("deleteSelectedKnots(ObjectId)");
}

void BSplineCurveSelectionPlugin::selectKnots(int _objectId, const IdList& _ids, bool _deselect) {
    
    if(_ids.empty()) return;
    
    BaseObjectData* object = 0;
    
    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return;
    }
    
    BSplineCurve* curve = PluginFunctions::splineCurve(object);
    if(curve) {
        for(IdList::const_iterator it = _ids.begin(); it != _ids.end(); ++it) {
            if(*it < (int)curve->n_knots()) {
                if(_deselect) curve->get_knotvector_ref()->deselect(*it);
                else curve->get_knotvector_ref()->select(*it);
            }
        }
    }
    
    QString selection = "selectKnots(ObjectId, [ " + QString::number(_ids[0]);

    for (uint i = 1 ; i < _ids.size(); ++i) {
        selection +=  ", " + QString::number(_ids[i]);
    }

    selection += " ])";
    

    emit scriptInfo(selection);
}

IdList BSplineCurveSelectionPlugin::getKnotSelection(int _objectId) {
    
    BaseObjectData* object = 0;
    
    IdList list;
    
    if(!PluginFunctions::getObject(_objectId, object)) {
        emit log(LOGERR, tr("Could not get object with id %1").arg(_objectId));
        return list;
    }
    
    BSplineCurve* curve = PluginFunctions::splineCurve(object);
    if(curve) {
        for(unsigned int i = 0; i < curve->n_knots(); ++i) {
            if(curve->get_knotvector_ref()->selected(i))
                list.push_back((int)i);
        }
    }
    
    return list;
}
