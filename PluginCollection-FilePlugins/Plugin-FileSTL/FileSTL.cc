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

#include "FileSTL.hh"


#if QT_VERSION >= 0x050000 
  #include <QtWidgets>
#else
  #include <QtGui>
#endif

/// Constructor
FileSTLPlugin::FileSTLPlugin() :
        saveOptions_(0),
        loadOptions_(0),
        saveBinary_(0),
        savePrecisionLabel_(0),
        savePrecision_(0),
        loadFaceNormal_(0),
        saveDefaultButton_(0),
        loadDefaultButton_(0)

{
}

//-----------------------------------------------------------------------------------------------------

void FileSTLPlugin::initializePlugin() {
}

//-----------------------------------------------------------------------------------------------------

QString FileSTLPlugin::getLoadFilters() {
    return QString( tr("Stereolithography files ( *.stl *.stla *.stlb )") );
};

//-----------------------------------------------------------------------------------------------------

QString FileSTLPlugin::getSaveFilters() {
    return QString( tr("Stereolithography files ( *.stl *.stla *.stlb )") );
};

//-----------------------------------------------------------------------------------------------------

DataType  FileSTLPlugin::supportedType() {
    DataType type = DATA_TRIANGLE_MESH;
    return type;
}

//-----------------------------------------------------------------------------------------------------

int FileSTLPlugin::loadObject(QString _filename) {

    int id = -1;
    emit addEmptyObject(DATA_TRIANGLE_MESH, id);

    TriMeshObject* object(0);
    if(PluginFunctions::getObject( id, object)) {

        if (PluginFunctions::objectCount() == 1 )
            object->target(true);

        object->setFromFileName(_filename);
        object->setName(object->filename());

        std::string filename = std::string( _filename.toUtf8() );

        object->mesh()->request_face_normals();

        bool loadNormals( (loadFaceNormal_ && loadFaceNormal_->isChecked()) ||
                          (!loadFaceNormal_ && OpenFlipperSettings().value("FileSTL/Load/FaceNormal", true).toBool())
            );

        // load file
        OpenMesh::IO::Options opt;
        // load face normals from the stl file if requested
        if (loadNormals) {
          opt += OpenMesh::IO::Options::FaceNormal;
        }

        bool ok = OpenMesh::IO::read_mesh( (*object->mesh()) , filename, opt );
        if (!ok)
        {
            std::cerr << "Plugin FileSTL : Read error for stl mesh.\n";
            emit deleteObject( object->id() );
            return -1;

        }

        // only calculate the face normals if they are not read from the file
        if (!loadNormals || !opt.face_has_normal())
          object->mesh()->update_normals();
        else {
          if (object->mesh()->has_vertex_normals())
            object->mesh()->update_vertex_normals();
          if (object->mesh()->has_halfedge_normals())
            object->mesh()->update_halfedge_normals();
        }

        emit updatedObject(object->id(), UPDATE_ALL);

        emit openedFile( object->id() );

        return object->id();

    } else {
        emit log(LOGERR,"Error : Could not create new triangle mesh object.");
        return -1;
    }
};

//-----------------------------------------------------------------------------------------------------

bool FileSTLPlugin::saveObject(int _id, QString _filename)
{
    BaseObjectData* object;
    if ( !PluginFunctions::getObject(_id,object) ) {
      emit log(LOGERR, tr("saveObject : cannot get object id %1 for save name %2").arg(_id).arg(_filename) );
      return false;
    }

    std::string filename = std::string( _filename.toUtf8() );

    if ( object->dataType( DATA_TRIANGLE_MESH ) ) {

        object->setFromFileName(_filename);
        object->setName(object->filename());

        TriMeshObject* triObj = dynamic_cast<TriMeshObject* >( object );

        OpenMesh::IO::Options opt = OpenMesh::IO::Options::Default;

        std::streamsize precision = 6;
        if ( !OpenFlipper::Options::savingSettings() && saveOptions_ != 0){

            if (!OpenFlipper::Options::nogui() && saveBinary_->isChecked())
                opt += OpenMesh::IO::Options::Binary;

            if (!saveBinary_->isChecked())
                precision = savePrecision_->value();

        }

        if (OpenMesh::IO::write_mesh(*triObj->mesh(), filename.c_str(), opt, precision) ) {
            emit log(LOGINFO, tr("Saved object to ") + _filename );
            return true;
        } else {
            emit log(LOGERR, tr("Unable to save ") + _filename );
            return false;
        }
    } else {
        emit log(LOGERR, tr("Unable to save (object is not a triangle mesh type)"));
        return false;
    }
}

//-----------------------------------------------------------------------------------------------------

QWidget* FileSTLPlugin::saveOptionsWidget(QString /*_currentFilter*/) {

    if (saveOptions_ == 0){
        //generate widget
        saveOptions_ = new QWidget();
        QVBoxLayout* layout = new QVBoxLayout();
        layout->setAlignment(Qt::AlignTop);

        saveBinary_ = new QCheckBox("Save Binary");
        layout->addWidget(saveBinary_);

        savePrecisionLabel_ = new QLabel("Writer Precision");
        layout->addWidget(savePrecisionLabel_);

        savePrecision_ = new QSpinBox();
        savePrecision_->setMinimum(1);
        savePrecision_->setMaximum(12);
        savePrecision_->setValue(6);
        layout->addWidget(savePrecision_);

        saveDefaultButton_ = new QPushButton("Make Default");
        layout->addWidget(saveDefaultButton_);

        saveOptions_->setLayout(layout);

        saveBinary_->setChecked( OpenFlipperSettings().value( "FileSTL/Save/Binary", false ).toBool() );

        connect(saveBinary_, SIGNAL(clicked(bool)), savePrecision_, SLOT(setDisabled(bool)));
        connect(saveDefaultButton_, SIGNAL(clicked()), this, SLOT(slotSaveDefault()));

    }

    return saveOptions_;
}

//-----------------------------------------------------------------------------------------------------

QWidget* FileSTLPlugin::loadOptionsWidget(QString /*_currentFilter*/) {

    if (loadOptions_ == 0){
        //generate widget
        loadOptions_ = new QWidget();
        QVBoxLayout* layout = new QVBoxLayout();
        layout->setAlignment(Qt::AlignTop);

        loadFaceNormal_ = new QCheckBox("Load Face Normals");
        layout->addWidget(loadFaceNormal_);

        loadFaceNormal_->setChecked( OpenFlipperSettings().value("FileSTL/Load/FaceNormal",true).toBool()  );

        loadDefaultButton_ = new QPushButton("Make Default");
        layout->addWidget(loadDefaultButton_);

        loadOptions_->setLayout(layout);

        connect(loadDefaultButton_, SIGNAL(clicked()), this, SLOT(slotLoadDefault()));
    }

    return loadOptions_;
}

//-----------------------------------------------------------------------------------------------------

void FileSTLPlugin::slotLoadDefault() {
  OpenFlipperSettings().setValue( "FileSTL/Load/FaceNormal",   loadFaceNormal_->isChecked()  );
  OpenFlipperSettings().setValue( "Core/File/UseLoadDefaults", true );
}

//-----------------------------------------------------------------------------------------------------

void FileSTLPlugin::slotSaveDefault() {
    OpenFlipperSettings().setValue( "FileSTL/Save/Binary",      saveBinary_->isChecked()  );
}
#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2( filestlplugin , FileSTLPlugin );
#endif


