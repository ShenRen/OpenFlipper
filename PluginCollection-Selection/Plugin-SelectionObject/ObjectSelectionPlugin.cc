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

#include "ObjectSelectionPlugin.hh"

#include <OpenFlipper/common/GlobalOptions.hh>

// Primitive type icons
#define OBJECT_TYPE         "selection_object.png"
// =======================================
// Define operations
// =======================================
// Vertices:
#define O_SELECT_ALL      "Select All Objects"
#define O_DESELECT_ALL    "Deselect All Objects"
#define O_INVERT          "Invert Object Selection"
#define O_DELETE          "Delete Selected Objects"

/// Default constructor
ObjectSelectionPlugin::ObjectSelectionPlugin() :
objectType_(0) {
}

//==============================================================================================
      
ObjectSelectionPlugin::~ObjectSelectionPlugin() {
}

//==============================================================================================

void ObjectSelectionPlugin::initializePlugin() {

    // Tell core about all scriptable slots
    updateSlotDescriptions();
}

//==============================================================================================

void ObjectSelectionPlugin::pluginsInitialized() {
    // Create new selection environment for polylines
    // and register polyline data type for the environment.
    
    QString iconPath = OpenFlipper::Options::iconDirStr() + OpenFlipper::Options::dirSeparator();
    
    emit addSelectionEnvironment("Object Selections", "Select objects.",
                                 iconPath + OBJECT_TYPE, environmentHandle_);

    // Register mesh object types
    emit registerType(environmentHandle_, DATA_ALL);
    
    emit addPrimitiveType(environmentHandle_, "Select Objects", iconPath + OBJECT_TYPE, objectType_);
    
    // Determine, which selection modes are requested
    emit showToggleSelectionMode(environmentHandle_, true, objectType_);
    emit showVolumeLassoSelectionMode(environmentHandle_, true, objectType_);
    
    // Define object operations
    QStringList objectOperations;
    objectOperations.append(O_SELECT_ALL);
    objectOperations.append(O_DESELECT_ALL);
    objectOperations.append(O_INVERT);
    objectOperations.append(O_DELETE);
    
    emit addSelectionOperations(environmentHandle_, objectOperations,  "Object Operations",   objectType_);
    
    // Register key shortcuts:
    
    // Select (a)ll
    emit registerKeyShortcut(Qt::Key_A, Qt::ControlModifier);
    // (C)lear selection
    emit registerKeyShortcut(Qt::Key_C,      Qt::NoModifier);
    // (I)nvert selection
    emit registerKeyShortcut(Qt::Key_I,      Qt::NoModifier);
    // Delete selected entities
    emit registerKeyShortcut(Qt::Key_Delete, Qt::NoModifier);
}

//==============================================================================================

void ObjectSelectionPlugin::updateSlotDescriptions() {
    
    emit setSlotDescription("selectAllObjects(int)", tr("Select all objects"),
                            QStringList("objectId"), QStringList("Id of object"));
    emit setSlotDescription("deselectAllObjects(int)", tr("Deselect all objects"),
                            QStringList("objectId"), QStringList("Id of object"));
    emit setSlotDescription("invertObjectSelection(int)", tr("Invert object selection"),
                            QStringList("objectId"), QStringList("Id of object"));
    emit setSlotDescription("deleteSelectedObjects(int)", tr("Delete selected objects"),
                            QStringList("objectId"), QStringList("Id of object"));
    emit setSlotDescription("selectObjects(int,IdList)", tr("Select the specified objects"),
                            QString("objectId,objectList").split(","), QString("Id of object,List of objects").split(","));

    emit setSlotDescription("loadSelection(int,QString)", tr("Load selection from selection file"),
                            QString("objectId,filename").split(","), QString("Id of an object,Selection file").split(","));
}

//==============================================================================================

void ObjectSelectionPlugin::slotSelectionOperation(QString _operation) {
    
    SelectionInterface::PrimitiveType type = 0u;
    emit getActivePrimitiveType(type);
    
    if((type & objectType_) == 0)
        return;
            
    if(_operation == O_SELECT_ALL) {
        // Select all objects
        selectAllObjects();
    } else if (_operation == O_DESELECT_ALL) {
        // Deselect all objects
        deselectAllObjects();
    } else if (_operation == O_INVERT) {
        // Invert object selection
        invertObjectSelection();
    } else if (_operation == O_DELETE) {
        // Delete selected objects
        deleteSelectedObjects();
    }
}

//==============================================================================================

void ObjectSelectionPlugin::slotToggleSelection(QMouseEvent* _event, SelectionInterface::PrimitiveType _currentType, bool _deselect) {

    // Return if none of the currently active types is handled by this plugin
    if((_currentType & objectType_) == 0) return;

    // Return if mouse event is not a left-button click
    if(_event->button() != Qt::LeftButton) return;
     
    size_t node_idx   = 0;
    size_t target_idx = 0;
    ACG::Vec3d hit_point;
    
    BaseObjectData* object = 0;

    bool successfullyPicked = PluginFunctions::scenegraphPick(ACG::SceneGraph::PICK_ANYTHING, _event->pos(), node_idx,
                              target_idx, &hit_point) && PluginFunctions::getPickedObject(node_idx, object);

    if(successfullyPicked) {

        if(object->target() || _deselect)
            object->target(false);
        else
            object->target(true);

        emit updatedObject(object->id(), UPDATE_SELECTION);
    }
}

//==============================================================================================

void ObjectSelectionPlugin::slotVolumeLassoSelection(QMouseEvent* _event, SelectionInterface::PrimitiveType _currentType, bool _deselect) {
    
    // Return if none of the currently active types is handled by this plugin
    if((_currentType & objectType_) == 0) return;
    
    ACG::Vec3d hit_point;
    
    if(_event->type() == QEvent::MouseButtonPress) {
        
        ACG::GLState &state = PluginFunctions::viewerProperties().glState();
        QPoint p(_event->pos().x(), state.viewport_height() - _event->pos().y());
        
        volumeLassoPoints_.append(p);
        
        return;

    } else if(_event->type() == QEvent::MouseButtonDblClick) {

        ACG::GLState &state = PluginFunctions::viewerProperties().glState();
        
        QPolygon polygon(volumeLassoPoints_);
        
        // Select all vertices that lie in this region
        for(PluginFunctions::ObjectIterator o_it(PluginFunctions::ALL_OBJECTS, DataType(DATA_ALL));
            o_it != PluginFunctions::objectsEnd(); ++o_it) {
            
            if (o_it->visible()) {

                // Look if COG of object lies within polygon
                ACG::Vec3d bbmin(0.0);
                ACG::Vec3d bbmax(0.0);
                o_it->getBoundingBox(bbmin, bbmax);
                ACG::Vec3d c = (bbmin + bbmax)/2.0;

                ACG::Vec3d pc = state.project(c);
                QPoint p((int)pc[0], (int)pc[1]);

                if(polygon.containsPoint(p, Qt::OddEvenFill)) {
                    o_it->target(!_deselect);
                }
            }
        
            emit updatedObject(o_it->id(), UPDATE_SELECTION);    
        }

        
        // Clear lasso points
        volumeLassoPoints_.clear();
    }
}

//==============================================================================================

void ObjectSelectionPlugin::loadSelection(int _objId, const QString& _filename) {

    // Load ini file
    INIFile file;

    if(!file.connect(_filename, false)) {
        emit log(LOGERR, QString("Could not read file '%1'!").arg(_filename));
        return;
    }

    // Load selection from file
    loadIniFile(file, _objId);
}

//==============================================================================================

void ObjectSelectionPlugin::loadIniFile(INIFile& _ini, int _id) {
    // From INI Interface
    // Load plugin specific settings
}

//==============================================================================================

void ObjectSelectionPlugin::saveIniFile(INIFile& _ini, int _id) {
    // From INI Interface
    // Save plugin specific settings
}

//==============================================================================================

void ObjectSelectionPlugin::slotLoadSelection(const INIFile& _file) {
    
    // Iterate over all polyline objects in the scene and save
    // the selections for all supported entity types
    for (PluginFunctions::ObjectIterator o_it(PluginFunctions::ALL_OBJECTS, DataType(DATA_ALL));
         o_it != PluginFunctions::objectsEnd(); ++o_it) {
        
        // Read section for each object
        // Append object name to section identifier
        QString section = QString("ObjectSelection") + "//" + o_it->name();
        if(!_file.section_exists(section)) {
            continue;
        }
        
        std::vector<int> ids;
        // Load vertex selection:
        _file.get_entry(ids, section, "Target");
        selectObjects(ids);
        ids.clear();
        
        emit updatedObject(o_it->id(), UPDATE_SELECTION);
    }
}

//==============================================================================================

void ObjectSelectionPlugin::slotSaveSelection(INIFile& _file) {
    
    // Iterate over all polyline objects in the scene and save
    // the selections for all vertices
    for (PluginFunctions::ObjectIterator o_it(PluginFunctions::ALL_OBJECTS, DataType(DATA_ALL));
         o_it != PluginFunctions::objectsEnd(); ++o_it) {
        
        // Create section for each object
        // Append object name to section identifier
        QString section = QString("ObjectSelection") + "//" + o_it->name();
        
        // Store vertex selection:
        _file.add_entry(section, "Target", getObjectSelection());
    }
}

//==============================================================================================

void ObjectSelectionPlugin::slotKeyShortcutEvent(int _key, Qt::KeyboardModifiers _modifiers) {
    
    SelectionInterface::PrimitiveType type = 0u;
    emit getActivePrimitiveType(type);
    
    if((type & objectType_) == 0) {
        // No supported type is active
        return;
    }

    if(_key == Qt::Key_A && _modifiers == Qt::ControlModifier) {
        // Select all objects
        selectAllObjects();
    } else if(_key == Qt::Key_C && _modifiers == Qt::NoModifier) {
        // Deselect all vertices
        deselectAllObjects();
    } else if(_key == Qt::Key_I && _modifiers == Qt::NoModifier) {
        // Invert object selection
        invertObjectSelection();
    } else if(_key == Qt::Key_Delete && _modifiers == Qt::NoModifier) {
        // Delete selected objects
        deleteSelectedObjects();
   }
}

//==============================================================================================

#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2(objectselectionplugin, ObjectSelectionPlugin);
#endif


