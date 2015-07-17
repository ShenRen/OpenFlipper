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

#include "ObjectSelectionPlugin.hh"

#include <OpenFlipper/common/GlobalOptions.hh>

/// Select all objects
void ObjectSelectionPlugin::selectAllObjects() {

    for (PluginFunctions::ObjectIterator o_it(PluginFunctions::ALL_OBJECTS, DataType(DATA_ALL));
            o_it != PluginFunctions::objectsEnd(); ++o_it) {

        o_it->target(true);
    }

    emit scriptInfo("selectAllObjects()");
}

/// Select specified objects
void ObjectSelectionPlugin::selectObjects(IdList _list) {

    for(IdList::iterator it = _list.begin(); it != _list.end(); ++it) {
        BaseObjectData* object = 0;
        PluginFunctions::getObject(*it, object);
        if(object) {
            object->target(true);
        }
    }

    emit scriptInfo("selectObjects(IdList)");
}

/// Deselect all objects
void ObjectSelectionPlugin::deselectAllObjects() {

    for (PluginFunctions::ObjectIterator o_it(PluginFunctions::ALL_OBJECTS, DataType(DATA_ALL));
            o_it != PluginFunctions::objectsEnd(); ++o_it) {

        o_it->target(false);
    }

    emit scriptInfo("deselectAllObjects()");
}

/// Invert object selection
void ObjectSelectionPlugin::invertObjectSelection() {

    for (PluginFunctions::ObjectIterator o_it(PluginFunctions::ALL_OBJECTS, DataType(DATA_ALL));
            o_it != PluginFunctions::objectsEnd(); ++o_it) {

        o_it->target(!o_it->target());
    }

    emit scriptInfo("invertObjectSelection()");
}

/// Delete selected objects
void ObjectSelectionPlugin::deleteSelectedObjects() {

    if(!OpenFlipper::Options::nogui()) {
        int ret = QMessageBox::warning(0, tr("Open Flipper"),
                                    tr("Do you really want to delete the selected objects?"),
                                    QMessageBox::Yes | QMessageBox::Cancel,
                                    QMessageBox::Cancel);

        if(ret == QMessageBox::Cancel) return;
    }

    std::vector<int> deleteIds;

    for (PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS, DataType(DATA_ALL));
            o_it != PluginFunctions::objectsEnd(); ++o_it) {

        deleteIds.push_back(o_it->id());
    }

    for(std::vector<int>::iterator it = deleteIds.begin(); it != deleteIds.end(); ++it) {
        emit deleteObject(*it);
    }

    emit scriptInfo("invertObjectSelection()");
}

/// Get selected objects
IdList ObjectSelectionPlugin::getObjectSelection() {

    IdList list;
    for (PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS, DataType(DATA_ALL));
            o_it != PluginFunctions::objectsEnd(); ++o_it) {

        list.push_back(o_it->id());
    }
    return list;
}
