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

#include "FilePLY.hh"

#if QT_VERSION >= 0x050000
  #include <QtWidgets>
#else
  #include <QtGui>
#endif


// Defines for the type handling drop down box
#define TYPEAUTODETECT 0
#define TYPEASK        1
#define TYPEPOLY       2
#define TYPETRIANGLE   3

/// Constructor
FilePLYPlugin::FilePLYPlugin()
: loadOptions_(0),
  saveOptions_(0),
  saveBinary_(0),
  saveVertexNormal_(0),
  saveVertexColor_(0),
  saveVertexTexCoord_(0),
  saveFaceNormal_(0),
  saveFaceColor_(0),
  savePrecisionLabel_(0),
  savePrecision_(0),
  saveDefaultButton_(0),
  triMeshHandling_(0),
  loadVertexNormal_(0),
  loadVertexColor_(0),
  loadVertexTexCoord_(0),
  loadFaceNormal_(0),
  loadFaceColor_(0),
  loadDefaultButton_(0),
  trimeshOptions_(0) {
}

//-----------------------------------------------------------------------------------------------------

void FilePLYPlugin::initializePlugin() {
}

//-----------------------------------------------------------------------------------------------------

QString FilePLYPlugin::getLoadFilters() {
    return QString( tr("Polygon File Format files ( *.ply )") );
};

//-----------------------------------------------------------------------------------------------------

QString FilePLYPlugin::getSaveFilters() {
    return QString( tr("Polygon File Format files ( *.ply )") );
};

//-----------------------------------------------------------------------------------------------------

DataType  FilePLYPlugin::supportedType() {
    DataType type = DATA_POLY_MESH | DATA_TRIANGLE_MESH;
    return type;
}

//-----------------------------------------------------------------------------------------------------
int FilePLYPlugin::loadObject(QString _filename, DataType _type) {

  int objectId = -1;

  bool gui = OpenFlipper::Options::gui() && (loadVertexNormal_ != 0) /*buttons initialized*/;
  // If in no gui mode -> request as much as possible
  bool vNormals   =  ((gui && loadVertexNormal_->isChecked()) ||
      (!gui && OpenFlipperSettings().value("FilePLY/Load/Normals",true).toBool()));
  bool vColors    =  ((gui && loadVertexColor_->isChecked()) ||
      (!gui && OpenFlipperSettings().value("FilePLY/Load/VertexColor",true).toBool()));
  bool vTexCoords =  ((gui && loadVertexTexCoord_->isChecked()) ||
      (!gui && OpenFlipperSettings().value("FilePLY/Load/TexCoords",true).toBool()));
  bool fNormals   =  ((gui && loadFaceNormal_->isChecked()) ||
      (!gui && OpenFlipperSettings().value("FilePLY/Load/FaceNormal",true).toBool()));
  bool fColors    =  ((gui && loadFaceColor_->isChecked()) ||
      (!gui && OpenFlipperSettings().value("FilePLY/Load/FaceColor",true).toBool()));

  OpenMesh::IO::Options opt;
  if (vNormals)
    opt += OpenMesh::IO::Options::VertexNormal;
  if (vColors)
    opt += OpenMesh::IO::Options::VertexColor;
  if (vTexCoords)
    opt += OpenMesh::IO::Options::VertexTexCoord;
  if (fNormals)
    opt += OpenMesh::IO::Options::FaceNormal;
  if (fColors)
    opt += OpenMesh::IO::Options::FaceColor;
  opt += OpenMesh::IO::Options::Custom;

  // Forced polymesh read
  if ( _type == DATA_POLY_MESH ) {
    objectId = loadPolyMeshObject(_filename, opt);

    PolyMeshObject* object(0);
    if(PluginFunctions::getObject( objectId, object )) {
      emit updatedObject(objectId, UPDATE_ALL);
      emit openedFile( objectId );
    }


    return objectId;
  } else if ( _type == DATA_TRIANGLE_MESH) {
    // If always open as TriMesh is selected

    objectId = loadTriMeshObject(_filename, opt);

    TriMeshObject* object(0);
    if(PluginFunctions::getObject( objectId, object )) {

      emit updatedObject(objectId, UPDATE_ALL);
      emit openedFile( objectId );
    }


    return objectId;
  } else {
    emit log( LOGERR, tr("FilePLYPlugin::loadObject(): Tried loading with unknown forced data type"));
  }

  return -1;
}

//-----------------------------------------------------------------------------------------------------

int FilePLYPlugin::loadObject(QString _filename) {

    int triMeshControl = TYPEAUTODETECT; // 0 == Auto-Detect

    bool gui = OpenFlipper::Options::gui() && (loadVertexNormal_ != 0) /*buttons initialized*/;
    // If in no gui mode -> request as much as possible
    bool vNormals   =  ((gui && loadVertexNormal_->isChecked()) ||
        (!gui && OpenFlipperSettings().value("FilePLY/Load/Normals",true).toBool()));
    bool vColors    =  ((gui && loadVertexColor_->isChecked()) ||
        (!gui && OpenFlipperSettings().value("FilePLY/Load/VertexColor",true).toBool()));
    bool vTexCoords =  ((gui && loadVertexTexCoord_->isChecked()) ||
        (!gui && OpenFlipperSettings().value("FilePLY/Load/TexCoords",true).toBool()));
    bool fNormals   =  ((gui && loadFaceNormal_->isChecked()) ||
        (!gui && OpenFlipperSettings().value("FilePLY/Load/FaceNormal",true).toBool()));
    bool fColors    =  ((gui && loadFaceColor_->isChecked()) ||
        (!gui && OpenFlipperSettings().value("FilePLY/Load/FaceColor",true).toBool()));

    OpenMesh::IO::Options opt;
    if (vNormals)
      opt += OpenMesh::IO::Options::VertexNormal;
    if (vColors)
      opt += OpenMesh::IO::Options::VertexColor;
    if (vTexCoords)
      opt += OpenMesh::IO::Options::VertexTexCoord;
    if (fNormals)
      opt += OpenMesh::IO::Options::FaceNormal;
    if (fColors)
      opt += OpenMesh::IO::Options::FaceColor;
    opt += OpenMesh::IO::Options::Custom;

    if ( OpenFlipper::Options::gui() ){
        if ( triMeshHandling_ != 0 ){
          triMeshControl = triMeshHandling_->currentIndex();
        } else {
          triMeshControl = TYPEAUTODETECT;
        }
    }

    int objectId = -1;


    if(triMeshControl == TYPEAUTODETECT || triMeshControl == TYPEASK) {

      objectId = loadPolyMeshObject(_filename, opt);

      PolyMeshObject *object = 0;
      if(!PluginFunctions::getObject(objectId, object))
        return -1;

      bool isTriMesh = true;
      for ( PolyMesh::FaceIter f_it = object->mesh()->faces_begin(); f_it != object->mesh()->faces_end() && isTriMesh; ++f_it) {

        // Count number of vertices for the current face
        unsigned count = 0;
        for ( PolyMesh::FaceVertexIter fv_it( *(object->mesh()),*f_it); fv_it.is_valid(); ++fv_it )
          ++count;

        // Check if it is a triangle. If not, this is really a poly mesh
        isTriMesh = isTriMesh && (count == 3);
      }

      if (triMeshControl == TYPEAUTODETECT)
      {
        if(!isTriMesh) {
            PolyMeshObject* object(0);
            if(PluginFunctions::getObject( objectId, object )) {
                emit updatedObject(objectId, UPDATE_ALL);
                emit openedFile( objectId );
            }
            return objectId;
        }
      }
      else if (triMeshControl == TYPEASK) {

        // If Ask is selected -> show dialog
        QMetaObject::invokeMethod(this,"handleTrimeshDialog",Qt::BlockingQueuedConnection);
        if ((trimeshOptions_ == TYPEPOLY) ||
            (trimeshOptions_ == TYPEASK && !isTriMesh)) {

          PolyMeshObject* object(0);
          if(PluginFunctions::getObject( objectId, object )) {
            emit updatedObject(object->id(), UPDATE_ALL);
            emit openedFile( objectId );
          }

          return objectId;
        }

      }
    } else if (triMeshControl == TYPEPOLY) {
        // If always open as PolyMesh is selected

        objectId = loadPolyMeshObject(_filename, opt);

        PolyMeshObject* object(0);
        if(PluginFunctions::getObject( objectId, object )) {
          emit updatedObject(object->id(), UPDATE_ALL);
          emit openedFile( objectId );
        }

        return objectId;
    } else {
        // If always open as TriMesh is selected

        objectId = loadTriMeshObject(_filename, opt);

        TriMeshObject* object(0);
        if(PluginFunctions::getObject( objectId, object )) {
          emit updatedObject(object->id(), UPDATE_ALL);
          emit openedFile( objectId );
        }


        return objectId;
    }

    // Load object as triangle mesh

    if(objectId != -1) emit deleteObject(objectId);
    objectId = loadTriMeshObject(_filename, opt);

    TriMeshObject* object(0);
    if(PluginFunctions::getObject( objectId, object )) {
      emit updatedObject(object->id(), UPDATE_ALL);
      emit openedFile( objectId );
    }

    return objectId;
};

void FilePLYPlugin::handleTrimeshDialog()
{
   QMessageBox msgBox;
   QPushButton *detectButton = msgBox.addButton(tr("Auto-Detect"), QMessageBox::ActionRole);
   QPushButton *triButton    = msgBox.addButton(tr("Open as triangle mesh"), QMessageBox::ActionRole);
   QPushButton *polyButton   = msgBox.addButton(tr("Open as poly mesh"), QMessageBox::ActionRole);
   msgBox.setWindowTitle( tr("Mesh types in file") );
   msgBox.setText( tr("You are about to open a file containing one or more mesh types. \n\n Which mesh type should be used?") );
   msgBox.setDefaultButton( detectButton );
   msgBox.exec();


   if (msgBox.clickedButton() == triButton)
    trimeshOptions_ =  TYPETRIANGLE ;
   else if (msgBox.clickedButton() == polyButton)
    trimeshOptions_ = TYPEPOLY ;
   else
     trimeshOptions_ = TYPEASK;
}

//-----------------------------------------------------------------------------------------------------

/// load a triangle-mesh with given filename
int FilePLYPlugin::loadTriMeshObject(QString _filename, OpenMesh::IO::Options& _opt){

    int id = -1;
    emit addEmptyObject(DATA_TRIANGLE_MESH, id);

    TriMeshObject* object(0);
    if(PluginFunctions::getObject( id, object)) {

        if ( PluginFunctions::objectCount() == 1 )
            object->target(true);

        object->setFromFileName(_filename);
        object->setName(object->filename());

        // Get mesh
        TriMesh* mesh = object->mesh();

        bool hadVNormals = mesh->has_vertex_normals();
        bool hadVColors = mesh->has_vertex_colors();
        bool hadVTexCoords = mesh->has_vertex_texcoords2D();
        bool hadFNormals = mesh->has_face_normals();
        bool hadFColors = mesh->has_face_colors();

        if(_opt.vertex_has_normal())
          mesh->request_vertex_normals();
        if(_opt.vertex_has_color())
          mesh->request_vertex_colors();
        if(_opt.vertex_has_texcoord())
          mesh->request_vertex_texcoords2D();
        if(_opt.face_has_normal())
          mesh->request_face_normals();
        if(_opt.face_has_color())
          mesh->request_face_colors();

        bool ok = OpenMesh::IO::read_mesh( *mesh , _filename.toStdString(), _opt);
        if (!ok)
        {
          emit log(LOGERR, "Error while reading PLY file!");
          emit deleteObject(id);
          return -1;
        }

        //update normals if they aren't read
        if (!_opt.vertex_has_normal() || !_opt.face_has_normal())
          mesh->update_normals();

        //cleanup mesh if selected option could not be loaded
        if (!hadVColors && !_opt.vertex_has_color() && mesh->has_vertex_colors())
          mesh->release_vertex_colors();
        if (!hadVTexCoords && !_opt.vertex_has_texcoord() && mesh->has_vertex_texcoords2D())
          mesh->release_vertex_texcoords2D();
        if (!hadFColors && !_opt.face_has_color() && mesh->has_face_colors())
          mesh->release_face_colors();

        // Switch to point mode if mesh does not contain one single face
        if(mesh->n_faces() == 0)
            PluginFunctions::setDrawMode(ACG::SceneGraph::DrawModes::POINTS);

        backupTextureCoordinates(*mesh);

        return object->id();

    } else {
        emit log(LOGERR,"Error : Could not create new triangle mesh object.");
        return -1;
    }
}

//-----------------------------------------------------------------------------------------------------

/// load a poly-mesh with given filename
int FilePLYPlugin::loadPolyMeshObject(QString _filename, OpenMesh::IO::Options& _opt){

    int id = -1;
    emit addEmptyObject(DATA_POLY_MESH, id);

    PolyMeshObject* object(0);
    if(PluginFunctions::getObject(id, object)) {

        if (PluginFunctions::objectCount() == 1 )
            object->target(true);

        object->setFromFileName(_filename);
        object->setName(object->filename());

        // Get mesh
        PolyMesh* mesh = object->mesh();

        bool hadVNormals = mesh->has_vertex_normals();
        bool hadVColors = mesh->has_vertex_colors();
        bool hadVTexCoords = mesh->has_vertex_texcoords2D();
        bool hadFNormals = mesh->has_face_normals();
        bool hadFColors = mesh->has_face_colors();

        if(_opt.vertex_has_normal())
          mesh->request_vertex_normals();
        if(_opt.vertex_has_color())
          mesh->request_vertex_colors();
        if(_opt.vertex_has_texcoord())
          mesh->request_vertex_texcoords2D();
        if(_opt.face_has_normal())
          mesh->request_face_normals();
        if(_opt.face_has_color())
          mesh->request_face_colors();

        bool ok = OpenMesh::IO::read_mesh( *mesh , _filename.toStdString(), _opt);
        if (!ok)
        {
          emit log(LOGERR, "Error while reading PLY file!");
          emit deleteObject(id);
          return -1;
        }

        //update normals if they aren't read
        if (!_opt.vertex_has_normal() || !_opt.face_has_normal())
          mesh->update_normals();

        //cleanup mesh if selected option could not be loaded
        if (!hadVColors && !_opt.vertex_has_color() && mesh->has_vertex_colors())
          mesh->release_vertex_colors();
        if (!hadVTexCoords && !_opt.vertex_has_texcoord() && mesh->has_vertex_texcoords2D())
          mesh->release_vertex_texcoords2D();
        if (!hadFColors && !_opt.face_has_color() && mesh->has_face_colors())
          mesh->release_face_colors();


        // Switch to point mode if mesh does not contain one single face
        if(mesh->n_faces() == 0)
            PluginFunctions::setDrawMode(ACG::SceneGraph::DrawModes::POINTS);

        backupTextureCoordinates(*mesh);

        return object->id();

    } else {
        emit log(LOGERR,"Error : Could not create new poly mesh object.");
        return -1;
    }

    return id;
}

//-----------------------------------------------------------------------------------------------------

bool FilePLYPlugin::saveObject(int _id, QString _filename)
{
    BaseObjectData* object;
    if ( !PluginFunctions::getObject(_id,object) ) {
      emit log(LOGERR, tr("saveObject : cannot get object id %1 for save name %2").arg(_id).arg(_filename) );
      return false;
    }

    object->setFromFileName(_filename);
    object->setName(object->filename());


    OpenMesh::IO::Options opt;
    bool gui = OpenFlipper::Options::gui() && (saveBinary_ != 0) /*buttons initialized?*/;
    bool binary = ((gui && saveBinary_->isChecked()) ||
                    OpenFlipperSettings().value("FilePLY/Save/Binary",false).toBool());

    if (binary)
      opt += OpenMesh::IO::Options::Binary;

    bool vNormals   = ((gui && saveVertexNormal_->isChecked())) ||
        (!gui && OpenFlipperSettings().value("FilePLY/Save/Normals",true).toBool());
    bool vColors    = ((gui && saveVertexColor_->isChecked())) ||
        (!gui && OpenFlipperSettings().value("FilePLY/Save/VertexColor",true).toBool());
    bool vTexCoords = ((gui && saveVertexTexCoord_->isChecked())) ||
        (!gui && OpenFlipperSettings().value("FilePLY/Save/TexCoords",true).toBool());
    bool fNormals   = ((gui && saveFaceNormal_->isChecked())) ||
        (!gui && OpenFlipperSettings().value("FilePLY/Save/FaceNormal",true).toBool());
    bool fColors    = ((gui && saveFaceColor_->isChecked())) ||
        (!gui && OpenFlipperSettings().value("FilePLY/Save/FaceColor",true).toBool());


    bool ok = false;

    if ( object->dataType(DATA_POLY_MESH) ) {

        PolyMeshObject* polyObj = dynamic_cast<PolyMeshObject*>(object);
        PolyMesh* mesh = polyObj->mesh();

        if (vNormals && mesh->has_vertex_normals())
          opt += OpenMesh::IO::Options::VertexNormal;
        if (vColors && mesh->has_vertex_colors())
          opt += OpenMesh::IO::Options::VertexColor;
        if (vTexCoords && mesh->has_vertex_texcoords2D())
          opt += OpenMesh::IO::Options::VertexTexCoord;
        if (fNormals && mesh->has_face_normals())
          opt += OpenMesh::IO::Options::FaceNormal;
        if (fColors && mesh->has_face_colors())
          opt += OpenMesh::IO::Options::FaceColor;

        ok = OpenMesh::IO::write_mesh(*mesh, _filename.toStdString() ,opt);

    } else if ( object->dataType( DATA_TRIANGLE_MESH ) ) {

        TriMeshObject* triObj = dynamic_cast<TriMeshObject*>(object);
        TriMesh* mesh = triObj->mesh();

        if (vNormals && mesh->has_vertex_normals())
          opt += OpenMesh::IO::Options::VertexNormal;
        if (vColors && mesh->has_vertex_colors())
          opt += OpenMesh::IO::Options::VertexColor;
        if (vTexCoords && mesh->has_vertex_texcoords2D())
          opt += OpenMesh::IO::Options::VertexTexCoord;
        if (fNormals && mesh->has_face_normals())
          opt += OpenMesh::IO::Options::FaceNormal;
        if (fColors && mesh->has_face_colors())
          opt += OpenMesh::IO::Options::FaceColor;

        ok = OpenMesh::IO::write_mesh(*mesh, _filename.toStdString() ,opt);
    }

    if(!ok) {
      emit log(LOGERR, tr("Unable to save ") + _filename);
      return false;
    }

    emit log(LOGINFO, tr("Saved object to ") + _filename );
    return true;
}

//-----------------------------------------------------------------------------------------------------

QWidget* FilePLYPlugin::saveOptionsWidget(QString /*_currentFilter*/) {

    if (saveOptions_ == 0){
        //generate widget
        saveOptions_ = new QWidget();
        QVBoxLayout* layout = new QVBoxLayout();
        layout->setAlignment(Qt::AlignTop);

        saveBinary_ = new QCheckBox("Save Binary");
        layout->addWidget(saveBinary_);

        saveVertexNormal_ = new QCheckBox("Save Vertex Normals");
        layout->addWidget(saveVertexNormal_);

        saveVertexColor_ = new QCheckBox("Save Vertex Colors");
        layout->addWidget(saveVertexColor_);

        saveVertexTexCoord_ = new QCheckBox("Save Vertex TexCoords");
        layout->addWidget(saveVertexTexCoord_);

        saveFaceNormal_ = new QCheckBox("Save Face Normals");
        layout->addWidget(saveFaceNormal_);

        saveFaceColor_ = new QCheckBox("Save Face Colors");
        layout->addWidget(saveFaceColor_);

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

        connect(saveBinary_, SIGNAL(clicked(bool)), savePrecision_, SLOT(setDisabled(bool)));
        connect(saveDefaultButton_, SIGNAL(clicked()), this, SLOT(slotSaveDefault()));

        saveBinary_->setChecked( OpenFlipperSettings().value("FilePLY/Save/Binary",true).toBool() );
        savePrecision_->setDisabled(true);
        saveVertexNormal_->setChecked( OpenFlipperSettings().value("FilePLY/Save/Normals",true).toBool() );
        saveVertexColor_->setChecked( OpenFlipperSettings().value("FilePLY/Save/VertexColor",true).toBool() );
        saveVertexTexCoord_->setChecked( OpenFlipperSettings().value("FilePLY/Save/TexCoords",true).toBool() );
        saveFaceNormal_->setChecked( OpenFlipperSettings().value("FilePLY/Save/FaceNormal",true).toBool() );
        saveFaceColor_->setChecked( OpenFlipperSettings().value("FilePLY/Save/FaceColor",true).toBool() );
    }

    return saveOptions_;
}

//-----------------------------------------------------------------------------------------------------

QWidget* FilePLYPlugin::loadOptionsWidget(QString /*_currentFilter*/) {

    if (loadOptions_ == 0){
        //generate widget
        loadOptions_ = new QWidget();
        QVBoxLayout* layout = new QVBoxLayout();
        layout->setAlignment(Qt::AlignTop);

        QLabel* label = new QLabel(tr("If PolyMesh is a Triangle Mesh:"));

        layout->addWidget(label);

        triMeshHandling_ = new QComboBox();
        triMeshHandling_->addItem( tr("Auto-Detect") );
        triMeshHandling_->addItem( tr("Ask") );
        triMeshHandling_->addItem( tr("Always open as PolyMesh") );
        triMeshHandling_->addItem( tr("Always open as TriangleMesh") );

        layout->addWidget(triMeshHandling_);

        loadVertexNormal_ = new QCheckBox("Load Vertex Normals");
        layout->addWidget(loadVertexNormal_);

        loadVertexColor_ = new QCheckBox("Load Vertex Colors");
        layout->addWidget(loadVertexColor_);

        loadVertexTexCoord_ = new QCheckBox("Load Vertex TexCoords");
        layout->addWidget(loadVertexTexCoord_);

        loadFaceNormal_ = new QCheckBox("Load Face Normals");
        layout->addWidget(loadFaceNormal_);

        loadFaceColor_ = new QCheckBox("Load Face Colors");
        layout->addWidget(loadFaceColor_);

        loadDefaultButton_ = new QPushButton("Make Default");
        layout->addWidget(loadDefaultButton_);

        loadOptions_->setLayout(layout);

        connect(loadDefaultButton_, SIGNAL(clicked()), this, SLOT(slotLoadDefault()));


        triMeshHandling_->setCurrentIndex(OpenFlipperSettings().value("FilePLY/Load/TriMeshHandling",TYPEAUTODETECT).toInt() );

        loadVertexNormal_->setChecked( OpenFlipperSettings().value("FilePLY/Load/Normals",true).toBool()  );
        loadVertexColor_->setChecked( OpenFlipperSettings().value("FilePLY/Load/VertexColor",true).toBool() );
        loadVertexTexCoord_->setChecked( OpenFlipperSettings().value("FilePLY/Load/TexCoords",true).toBool()  );
        loadFaceNormal_->setChecked( OpenFlipperSettings().value("FilePLY/Load/FaceNormal",true).toBool()  );
        loadFaceColor_->setChecked( OpenFlipperSettings().value("FilePLY/Load/FaceColor",true).toBool()  );
    }

    return loadOptions_;
}

void FilePLYPlugin::slotLoadDefault() {

    OpenFlipperSettings().setValue( "FilePLY/Load/Normals",     loadVertexNormal_->isChecked()  );
    OpenFlipperSettings().setValue( "FilePLY/Load/VertexColor", loadVertexColor_->isChecked()  );
    OpenFlipperSettings().setValue( "FilePLY/Load/TexCoords",   loadVertexTexCoord_->isChecked()  );
    OpenFlipperSettings().setValue( "FilePLY/Load/FaceNormal",  loadFaceNormal_->isChecked()  );
    OpenFlipperSettings().setValue( "FilePLY/Load/FaceColor",   loadFaceColor_->isChecked()  );

    OpenFlipperSettings().setValue( "FilePLY/Load/TriMeshHandling", triMeshHandling_->currentIndex() );

    OpenFlipperSettings().setValue( "Core/File/UseLoadDefaults", true );
}


void FilePLYPlugin::slotSaveDefault() {

    OpenFlipperSettings().setValue( "FilePLY/Save/Binary",      saveBinary_->isChecked()  );
    OpenFlipperSettings().setValue( "FilePLY/Save/TexCoords",   saveVertexTexCoord_->isChecked()  );
    OpenFlipperSettings().setValue( "FilePLY/Save/Normals",     saveVertexNormal_->isChecked()  );
    OpenFlipperSettings().setValue( "FilePLY/Save/VertexColor", saveVertexColor_->isChecked()  );
    OpenFlipperSettings().setValue( "FilePLY/Save/FaceNormal",  saveFaceNormal_->isChecked()  );
    OpenFlipperSettings().setValue( "FilePLY/Save/FaceColor",   saveFaceColor_->isChecked()  );
}
#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2( fileplyplugin , FilePLYPlugin );
#endif

