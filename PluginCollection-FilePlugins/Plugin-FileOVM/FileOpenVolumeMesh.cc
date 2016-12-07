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
 *   $Revision: 13576 $                                                       *
 *   $LastChangedBy: kremer $                                                 *
 *   $Date: 2012-01-30 11:22:27 +0100 (Mo, 30 Jan 2012) $                    *
 *                                                                            *
 \*===========================================================================*/

#include <iostream>

#include <ACG/GL/GLState.hh>

#include <OpenFlipper/BasePlugin/PluginFunctions.hh>
#include <OpenFlipper/common/GlobalOptions.hh>

#include "FileOpenVolumeMesh.hh"

#if QT_VERSION >= 0x050000
#else
  #include <QtGui>
#endif


FileOpenVolumeMeshPlugin::FileOpenVolumeMeshPlugin() :
loadOptions_(0),
saveOptions_(0),
typeCheck_(0),
loadCompMode_(0),
loadTopCheck_(0),
saveCompMode_(0) {

}

//----------------------------------------------------------------------------

void FileOpenVolumeMeshPlugin::initializePlugin() {

    loadOptions_ = new QWidget();

    QVBoxLayout* llayout = new QVBoxLayout();
    llayout->setAlignment(Qt::AlignTop);

    typeCheck_ = new QComboBox();
    typeCheck_->addItem("Autodetect");
    typeCheck_->addItem("Polyhedral Mesh");
    typeCheck_->addItem("Hexahedral Mesh");
#ifdef ENABLE_OPENVOLUMEMESH_TETRAHEDRAL_SUPPORT
    typeCheck_->addItem("Tetrahedral Mesh");
#endif
    typeCheck_->setCurrentIndex(0);
    loadCompMode_ = new QCheckBox("Load PolyVolMesh format");
    loadTopCheck_ = new QCheckBox("Perform topology checks");
    llayout->addWidget(typeCheck_);
    llayout->addWidget(loadCompMode_);
    llayout->addWidget(loadTopCheck_);

    loadOptions_->setLayout(llayout);

    saveOptions_ = new QWidget();

    QVBoxLayout* slayout = new QVBoxLayout();
    slayout->setAlignment(Qt::AlignTop);

    saveCompMode_ = new QCheckBox("Save in PolyVolMesh format");
    slayout->addWidget(saveCompMode_);

    saveOptions_->setLayout(slayout);
}

//----------------------------------------------------------------------------


QString FileOpenVolumeMeshPlugin::getLoadFilters() {
    return QString(tr("Polyhedral Volume Mesh files ( *.ovm *.polyvolmesh *.tetmesh )"));
}
;

//----------------------------------------------------------------------------


QString FileOpenVolumeMeshPlugin::getSaveFilters() {
    return QString(tr("Polyhedral Volume Mesh files ( *.ovm )"));
}
;

//----------------------------------------------------------------------------


DataType FileOpenVolumeMeshPlugin::supportedType() {

    DataType type = DATA_POLYHEDRAL_MESH | DATA_HEXAHEDRAL_MESH;
#ifdef ENABLE_OPENVOLUMEMESH_TETRAHEDRAL_SUPPORT
    type |= DATA_TETRAHEDRAL_MESH;
#endif
    return type;
}

//----------------------------------------------------------------------------


int FileOpenVolumeMeshPlugin::loadObject(QString _filename) {

    bool compatibility_mode = false;
    if(!OpenFlipper::Options::nogui()) {
        compatibility_mode = loadCompMode_->isChecked();
    }

    bool topology_checks = true;
    if(!OpenFlipper::Options::nogui()) {
        topology_checks = loadTopCheck_->isChecked();
    }

    int id = -1;
    bool hexMesh = false;

    if(!OpenFlipper::Options::nogui() && typeCheck_->currentIndex() == 0) {
        hexMesh = fileManager_.isHexahedralMesh(_filename.toStdString());
    } else if (!OpenFlipper::Options::nogui() && typeCheck_->currentIndex() == 2) {
        hexMesh = true;
    }

#ifdef ENABLE_OPENVOLUMEMESH_TETRAHEDRAL_SUPPORT
    bool tetMesh = false;
    if(!OpenFlipper::Options::nogui() && typeCheck_->currentIndex() == 0) {
        tetMesh = fileManager_.isTetrahedralMesh(_filename.toStdString());
    } else if (!OpenFlipper::Options::nogui() && typeCheck_->currentIndex() == 3) {
        tetMesh = true;
    }
#endif // ENABLE_OPENVOLUMEMESH_TETRAHEDRAL_SUPPORT

    BaseObjectData* baseObj = 0;

    if(hexMesh) {

        emit addEmptyObject(DATA_HEXAHEDRAL_MESH, id);
        HexahedralMeshObject* obj(0);

        if (PluginFunctions::getObject(id, obj)) {
            baseObj = obj;

            if(compatibility_mode) {

                loadMesh((const char*) _filename.toLatin1(), *(obj->mesh()), compatibility_mode,
                         topology_checks);

            } else {
                if(!fileManager_.readFile(_filename.toStdString(), *(obj->mesh()),
                                          topology_checks,true)) {
                    emit log(LOGERR, QString("Could not open file %1!").arg(_filename));
                }
            }

            // Scale hexahedra a bit
            obj->meshNode()->set_scaling(0.8);

        }

    }
#ifdef ENABLE_OPENVOLUMEMESH_TETRAHEDRAL_SUPPORT
    else if(tetMesh) {

        emit addEmptyObject(DATA_TETRAHEDRAL_MESH, id);
        TetrahedralMeshObject* obj(0);

        if (PluginFunctions::getObject(id, obj)) {
            baseObj = obj;

            if(compatibility_mode) {

                loadMesh((const char*) _filename.toLatin1(), *(obj->mesh()), compatibility_mode,
                         topology_checks);

            } else {
                if(!fileManager_.readFile(_filename.toStdString(), *(obj->mesh()),
                                          topology_checks,true)) {
                    emit log(LOGERR, QString("Could not open file %1!").arg(_filename));
                }
            }

            // Scale tetrahedra a bit
            obj->meshNode()->set_scaling(0.8);
        }


    }
#endif // ENABLE_OPENVOLUMEMESH_TETRAHEDRAL_SUPPORT
    else {

        emit addEmptyObject(DATA_POLYHEDRAL_MESH, id);
        PolyhedralMeshObject* obj(0);

        if (PluginFunctions::getObject(id, obj)) {
            baseObj = obj;

            if(compatibility_mode) {

                loadMesh((const char*) _filename.toLatin1(), *(obj->mesh()), compatibility_mode,
                         topology_checks);

            } else {
                if(!fileManager_.readFile(_filename.toStdString(), *(obj->mesh()),
                                          topology_checks,true)) {
                    emit log(LOGERR, QString("Could not open file %1!").arg(_filename));
                }
            }

            // Scale polyhedra a bit
            obj->meshNode()->set_scaling(0.8);

        }
    }

    if (baseObj)
    {
      baseObj->setFromFileName(_filename);
      baseObj->setName(baseObj->filename());

      // Go into solid flat shaded mode
      baseObj->setObjectDrawMode(ACG::SceneGraph::DrawModes::getDrawMode("Cells (flat shaded)"));

      // Compute face normals
      emit updatedObject(baseObj->id(), UPDATE_ALL);

      emit openedFile(baseObj->id());
    }

    return id;
}

//----------------------------------------------------------------------------


bool FileOpenVolumeMeshPlugin::saveObject(int _id, QString _filename) {

    BaseObjectData* obj(0);
    if (PluginFunctions::getObject(_id, obj)) {

        PolyhedralMeshObject* mesh_obj = PluginFunctions::polyhedralMeshObject(obj);
        HexahedralMeshObject* hex_mesh_obj = PluginFunctions::hexahedralMeshObject(obj);
#ifdef ENABLE_OPENVOLUMEMESH_TETRAHEDRAL_SUPPORT
        TetrahedralMeshObject* tet_mesh_obj = PluginFunctions::tetrahedralMeshObject(obj);
#endif
        if (mesh_obj) {

          obj->setFromFileName(_filename);
          obj->setName(obj->filename());
          if(!fileManager_.writeFile(_filename.toStdString(), *(mesh_obj->mesh()))) {
            emit log(LOGERR, tr("Unable to save ") + _filename);
            return false;
          }
        }
        else if (hex_mesh_obj) {

          obj->setFromFileName(_filename);
          obj->setName(obj->filename());
          if (!fileManager_.writeFile(_filename.toStdString(), *(hex_mesh_obj->mesh()))) {
            emit log(LOGERR, tr("Unable to save ") + _filename);
            return false;
          }
        }
#ifdef ENABLE_OPENVOLUMEMESH_TETRAHEDRAL_SUPPORT
        else if (tet_mesh_obj) {

          obj->setFromFileName(_filename);
          obj->setName(obj->filename());
          if (!fileManager_.writeFile(_filename.toStdString(), *(tet_mesh_obj->mesh()))) {
            emit log(LOGERR, tr("Unable to save ") + _filename);
            return false;
          }
        }
#endif // ENABLE_OPENVOLUMEMESH_TETRAHEDRAL_SUPPORT

        return true;

    } else {
      emit log(LOGERR, tr("saveObject : cannot get object id %1 for save name %2").arg(_id).arg(_filename) );
      return false;
    }


}

//----------------------------------------------------------------------------


void FileOpenVolumeMeshPlugin::loadIniFileLast(INIFile& _ini, int _id) {

    BaseObjectData* baseObject;
    if (!PluginFunctions::getObject(_id, baseObject)) {
        emit log(LOGERR, tr("Cannot find object for id %1 in saveFile!").arg(_id));
        return;
    }

    PolyhedralMeshObject* object = PluginFunctions::polyhedralMeshObject(baseObject);

    if (object) {
        ACG::Vec4f col(0.0, 0.0, 0.0, 0.0);

        if (_ini.get_entryVecf(col, object->name(), "BaseColor"))
            object->materialNode()->set_base_color(col);
    }

}

//----------------------------------------------------------------------------

void FileOpenVolumeMeshPlugin::saveIniFile(INIFile& _ini, int _id) {

    BaseObjectData* baseObject;
    if (!PluginFunctions::getObject(_id, baseObject)) {
        emit log(LOGERR, tr("Cannot find object for id %1 in saveFile!").arg(_id));
        return;
    }

    PolyhedralMeshObject* object = PluginFunctions::polyhedralMeshObject(baseObject);

    if (object) {
        _ini.add_entryVec(object->name(), "BaseColor", object->materialNode()->base_color());
    }
}

//----------------------------------------------------------------------------

QWidget* FileOpenVolumeMeshPlugin::saveOptionsWidget(QString _currentFilter) {

    return saveOptions_;
}

//----------------------------------------------------------------------------

QWidget* FileOpenVolumeMeshPlugin::loadOptionsWidget(QString _currentFilter) {

    return loadOptions_;
}
#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2(fileopenvolumemeshplugin, FileOpenVolumeMeshPlugin)
#endif

