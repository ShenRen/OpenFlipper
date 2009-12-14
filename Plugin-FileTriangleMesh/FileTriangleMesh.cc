/*===========================================================================*\
 *                                                                           *
 *                              OpenFlipper                                  *
 *      Copyright (C) 2001-2009 by Computer Graphics Group, RWTH Aachen      *
 *                           www.openflipper.org                             *
 *                                                                           *
 *---------------------------------------------------------------------------*
 *  This file is part of OpenFlipper.                                        *
 *                                                                           *
 *  OpenFlipper is free software: you can redistribute it and/or modify      *
 *  it under the terms of the GNU Lesser General Public License as           *
 *  published by the Free Software Foundation, either version 3 of           *
 *  the License, or (at your option) any later version with the              *
 *  following exceptions:                                                    *
 *                                                                           *
 *  If other files instantiate templates or use macros                       *
 *  or inline functions from this file, or you compile this file and         *
 *  link it with other files to produce an executable, this file does        *
 *  not by itself cause the resulting executable to be covered by the        *
 *  GNU Lesser General Public License. This exception does not however       *
 *  invalidate any other reasons why the executable file might be            *
 *  covered by the GNU Lesser General Public License.                        *
 *                                                                           *
 *  OpenFlipper is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *  GNU Lesser General Public License for more details.                      *
 *                                                                           *
 *  You should have received a copy of the GNU LesserGeneral Public          *
 *  License along with OpenFlipper. If not,                                  *
 *  see <http://www.gnu.org/licenses/>.                                      *
 *                                                                           *
\*===========================================================================*/

/*===========================================================================*\
 *                                                                           *
 *   $Revision$                                                       *
 *   $Author$                                                      *
 *   $Date$                   *
 *                                                                           *
\*===========================================================================*/





#include <QtGui>
#include <QFileInfo>

#include "FileTriangleMesh.hh"

#include <iostream>
#include <ACG/GL/GLState.hh>

#include "OpenFlipper/common/GlobalOptions.hh"
#include "OpenFlipper/BasePlugin/PluginFunctions.hh"

#include <OpenMesh/Core/IO/IOManager.hh>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/IO/Options.hh>

#include <OpenFlipper/ACGHelper/DrawModeConverter.hh>


FileTriangleMeshPlugin::FileTriangleMeshPlugin()
: saveOptions_(0),
  loadOptions_(0){
}

/// load a triangle-mesh with given filename
int FileTriangleMeshPlugin::loadObject(QString _filename) {

    int id = -1;
    emit addEmptyObject(DATA_TRIANGLE_MESH, id);
    
    TriMeshObject* object(0);
    if(PluginFunctions::getObject( id, object)) {
        
        if ( PluginFunctions::objectCount() == 1 )
            object->target(true);
        
        object->setFromFileName(_filename);
        
        // call the local function to update names
        QFileInfo f(_filename);
        object->setName( f.fileName() );
        
        std::string filename = std::string( _filename.toUtf8() );
        
        //set options
        OpenMesh::IO::Options opt = OpenMesh::IO::Options::Default;
        
        if ( !OpenFlipper::Options::loadingSettings() &&
            !OpenFlipper::Options::loadingRecentFile() && loadOptions_ != 0){
            
            if (loadVertexColor_->isChecked())
                opt += OpenMesh::IO::Options::VertexColor;
            
            if (loadFaceColor_->isChecked())
                opt += OpenMesh::IO::Options::FaceColor;
            
            //ColorAlpha is only checked when loading binary off's
            if (loadAlpha_->isChecked())
                opt += OpenMesh::IO::Options::ColorAlpha;
            
            if (loadNormals_->isChecked())
                opt += OpenMesh::IO::Options::VertexNormal;
            
            if (loadTexCoords_->isChecked())
                opt += OpenMesh::IO::Options::VertexTexCoord;
            
        } else {
            //let openmesh try to read everything it can
            opt += OpenMesh::IO::Options::VertexColor;
            opt += OpenMesh::IO::Options::FaceColor;
            opt += OpenMesh::IO::Options::VertexNormal;
            opt += OpenMesh::IO::Options::VertexTexCoord;
        }
           
        // load file
        bool ok = OpenMesh::IO::read_mesh( (*object->mesh()) , filename, opt );
        if (!ok)
        {
            std::cerr << "Plugin FileTriangleMesh : Read error for Triangle Mesh\n";
            emit deleteObject( object->id() );
            return -1;
        }
        
        object->mesh()->update_normals();
        
        object->update();
        
        object->show();
        
        emit log(LOGINFO,object->getObjectinfo());
        
        emit openedFile( object->id() );
        
        return object->id();
        
    } else {
        emit log(LOGERR,"Error : Could not create new triangle mesh object.");
        return -1;
    }
}

/// Save object with given id
bool FileTriangleMeshPlugin::saveObject(int _id, QString _filename){
  BaseObjectData* object;
  PluginFunctions::getObject(_id,object);

  if (object  == 0){
      emit log(LOGERR, tr("Unable to save (Could not get object)"));
      return false;
  }

  std::string filename = std::string( _filename.toUtf8() );

  if ( object->dataType( DATA_TRIANGLE_MESH ) ) {
    object->setName(_filename.section(OpenFlipper::Options::dirSeparator(),-1));
    object->path(_filename.section(OpenFlipper::Options::dirSeparator(),0,-2) );

    TriMeshObject* triObj = dynamic_cast<TriMeshObject* >( object );

    OpenMesh::IO::Options opt = OpenMesh::IO::Options::Default;

    if ( !OpenFlipper::Options::savingSettings() && saveOptions_ != 0){

      if (saveBinary_->isChecked())
        opt += OpenMesh::IO::Options::Binary;

      if (saveVertexColor_->isChecked()){
        opt += OpenMesh::IO::Options::VertexColor;
      }

      if (saveFaceColor_->isChecked()){
        opt += OpenMesh::IO::Options::FaceColor;
      }

      if (saveAlpha_->isChecked()){
        opt += OpenMesh::IO::Options::ColorAlpha;
      }

      if (saveNormals_->isChecked())
        opt += OpenMesh::IO::Options::VertexNormal;

      if (saveTexCoords_->isChecked())
        opt += OpenMesh::IO::Options::VertexTexCoord;

    }

    if (OpenMesh::IO::write_mesh(*triObj->mesh(), filename.c_str(),opt) ){
      emit log(LOGINFO, tr("Saved object to ") + object->path() + OpenFlipper::Options::dirSeparator() + object->name() );
      return true;
    }else{
      emit log(LOGERR, tr("Unable to save ") + object->path() + OpenFlipper::Options::dirSeparator() + object->name());
      return false;
    }
  }else{

    emit log(LOGERR, tr("Unable to save (object isn't a triangle mesh)"));
    return false;
  }
}

/// Save object with given id
bool FileTriangleMeshPlugin::saveObject(int _id, QString _filename, bool _binary){

  BaseObjectData* object;
  PluginFunctions::getObject(_id,object);

  if (object  == 0){
      emit log(LOGERR, tr("Unable to save (Could not get object)"));
      return false;
  }

  std::string filename = std::string( _filename.toUtf8() );

  if ( object->dataType( DATA_TRIANGLE_MESH ) ) {
    object->setName(_filename.section(OpenFlipper::Options::dirSeparator(),-1));
    object->path(_filename.section(OpenFlipper::Options::dirSeparator(),0,-2) );

    TriMeshObject* triObj = dynamic_cast<TriMeshObject* >( object );

    OpenMesh::IO::Options opt = OpenMesh::IO::Options::Default;

    if ( _binary )
      opt += OpenMesh::IO::Options::Binary;

    if (OpenMesh::IO::write_mesh(*triObj->mesh(), filename.c_str(),opt) ){
      emit log(LOGINFO, tr("Saved object to ") + object->path() + OpenFlipper::Options::dirSeparator() + object->name() );
      return true;
    }else{
      emit log(LOGERR, tr("Unable to save ") + object->path() + OpenFlipper::Options::dirSeparator() + object->name());
      return false;
    }
  }else{
    emit log(LOGERR, tr("Unable to save (object isn't a triangle mesh)"));
    return false;
  }
}

///return file-extension filters for loading objects
QString FileTriangleMeshPlugin::getLoadFilters() {
  
  QString filters = QString( OpenMesh::IO::IOManager().qt_read_filters().c_str() );
  
  // Remove support for OFF-files since they are now handled by a separate plugin
  filters.replace(";;Object File Format ( *.off )", "");
  filters.replace("*.off", "");
  
  return filters;
};

///return file-extension filters for saving objects
QString FileTriangleMeshPlugin::getSaveFilters() {
  
  QString filters = QString( OpenMesh::IO::IOManager().qt_write_filters().c_str() );
  
  // Remove support for OFF-files since they are now handled by a separate plugin
  filters.replace(";;no description ( *.off )", "");
  filters.replace("*.off", "");
  
  return filters;
};

///return supported DataType
DataType FileTriangleMeshPlugin::supportedType() {
  DataType type = DATA_TRIANGLE_MESH;
  return type;
}

void FileTriangleMeshPlugin::loadIniFileLast( INIFile& _ini ,int _id ) {
  BaseObjectData* baseObject;
  if ( !PluginFunctions::getObject(_id,baseObject) ) {
    emit log(LOGERR,tr("Cannot find object for id %1 in saveFile").arg(_id) );
    return;
  }

  TriMeshObject* object = PluginFunctions::triMeshObject(baseObject);

  if ( object ) {
    std::vector< QString > draw_modes;
    if( _ini.get_entry(draw_modes, object->name(), "MaterialDrawMode") )
      object->materialNode()->drawMode( ListToDrawMode(draw_modes) );

    if( _ini.get_entry(draw_modes, object->name(), "MeshDrawMode") )
      object->meshNode()->drawMode( ListToDrawMode(draw_modes) );

    ACG::Vec4f col(0.0,0.0,0.0,0.0);

    if ( _ini.get_entryVecf( col, object->name() , "BaseColor" ) )
      object->materialNode()->set_base_color(col);

    if ( _ini.get_entryVecf( col, object->name() , "AmbientColor" ) )
      object->materialNode()->set_ambient_color(col);

    if ( _ini.get_entryVecf( col, object->name() , "DiffuseColor" ) )
      object->materialNode()->set_diffuse_color(col);

    if ( _ini.get_entryVecf( col, object->name() , "SpecularColor" ) )
      object->materialNode()->set_specular_color(col);

    int size = 1;
    if ( _ini.get_entry( size, object->name() , "PointSize" ) )
      object->materialNode()->set_point_size(size);
    if ( _ini.get_entry( size, object->name() , "LineWidth" ) )
      object->materialNode()->set_line_width(size);
  }

}

void FileTriangleMeshPlugin::saveIniFile( INIFile& _ini ,int _id) {
  BaseObjectData* baseObject;
  if ( !PluginFunctions::getObject(_id,baseObject) ) {
    emit log(LOGERR,tr("Cannot find object for id %1 in saveFile!").arg(_id) );
    return;
  }

  TriMeshObject* object = PluginFunctions::triMeshObject(baseObject);

  if ( object ) {
    _ini.add_entry( object->name() ,
                    "MaterialDrawMode" ,
                    drawModeToList( object->materialNode()->drawMode()) );
    _ini.add_entry( object->name() ,
                    "MeshDrawMode" ,
                    drawModeToList( object->meshNode()->drawMode()) );
    _ini.add_entryVec( object->name() ,
                      "BaseColor" ,
                      object->materialNode()->base_color()) ;
    _ini.add_entryVec( object->name() ,
                      "AmbientColor" ,
                      object->materialNode()->ambient_color()) ;
    _ini.add_entryVec( object->name() ,
                      "DiffuseColor" ,
                      object->materialNode()->diffuse_color());
    _ini.add_entryVec( object->name() ,
                      "SpecularColor" ,
                      object->materialNode()->specular_color());

    _ini.add_entry( object->name() ,
                    "PointSize" ,
                    object->materialNode()->point_size());
    _ini.add_entry( object->name() ,
                    "LineWidth" ,
                    object->materialNode()->line_width());
  }
}

QWidget* FileTriangleMeshPlugin::saveOptionsWidget(QString _currentFilter) {

  if (saveOptions_ == 0){
    //generate widget
    saveOptions_ = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignTop);

    if( ! _currentFilter.contains("Wavefront") ){ //dont add 'save binary' for obj
      saveBinary_ = new QCheckBox("Save Binary");
      layout->addWidget(saveBinary_);
    }

    saveVertexColor_ = new QCheckBox("Save Vertex Colors");
    layout->addWidget(saveVertexColor_);

    saveFaceColor_ = new QCheckBox("Save Face Colors");
    layout->addWidget(saveFaceColor_);

    saveAlpha_ = new QCheckBox("Save Color Alpha");
    layout->addWidget(saveAlpha_);

    saveNormals_ = new QCheckBox("Save Normals");
    layout->addWidget(saveNormals_);

    saveTexCoords_ = new QCheckBox("Save TexCoords");
    layout->addWidget(saveTexCoords_);

    saveOptions_->setLayout(layout);
  }else{
    //adjust widget
    if( _currentFilter.contains("Wavefront") ) //dont add 'save binary' for obj
      saveBinary_->setVisible( false );
    else
      saveBinary_->setVisible( true );
  }

  saveBinary_->setChecked( Qt::Unchecked );
  saveVertexColor_->setChecked( Qt::Unchecked );
  saveFaceColor_->setChecked( Qt::Unchecked );
  saveAlpha_->setChecked( Qt::Unchecked );
  saveNormals_->setChecked( Qt::Unchecked );
  saveTexCoords_->setChecked( Qt::Unchecked );

  return saveOptions_;
}

QWidget* FileTriangleMeshPlugin::loadOptionsWidget(QString /*_currentFilter*/) {

  if (loadOptions_ == 0){
    //generate widget
    loadOptions_ = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignTop);

    loadVertexColor_ = new QCheckBox("Load Vertex Colors");
    layout->addWidget(loadVertexColor_);

    loadFaceColor_ = new QCheckBox("Load Face Colors");
    layout->addWidget(loadFaceColor_);

    loadAlpha_ = new QCheckBox("Load Color Alpha");
    layout->addWidget(loadAlpha_);

    loadNormals_ = new QCheckBox("Load Normals");
    layout->addWidget(loadNormals_);

    loadTexCoords_ = new QCheckBox("Load TexCoords");
    layout->addWidget(loadTexCoords_);

    loadOptions_->setLayout(layout);

  ///TODO load face normals checkbox except for off
  }

  loadVertexColor_->setChecked( Qt::Checked );
  loadFaceColor_->setChecked( Qt::Checked );
  loadAlpha_->setChecked( Qt::Checked );
  loadNormals_->setChecked( Qt::Checked );
  loadTexCoords_->setChecked( Qt::Checked );

  return loadOptions_;
}

Q_EXPORT_PLUGIN2( filetrianglemeshplugin , FileTriangleMeshPlugin );

