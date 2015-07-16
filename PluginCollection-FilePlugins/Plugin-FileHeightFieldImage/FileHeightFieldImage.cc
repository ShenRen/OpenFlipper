/*===========================================================================*\
 *                                                                           *
 *                             OpenFlipper                                   *
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
*                                                                            *
*   $Revision$                                                       *
*   $LastChangedBy$                                                *
*   $Date$                     *
*                                                                            *
\*===========================================================================*/

#include "FileHeightFieldImage.hh"
#include "ImageDialog.hh"

#if QT_VERSION >= 0x050000 
  #include <QtWidgets>
#else
  #include <QtGui>
#endif

#include <QFileInfo>
#include <QSettings>
#include <OpenFlipper/BasePlugin/PluginFunctions.hh>

void FileHeightFieldPNGPlugin::initializePlugin() {
}

QString FileHeightFieldPNGPlugin::getLoadFilters() {
  QList<QByteArray> imageFormats = QImageReader::supportedImageFormats();
  QString formats;

  if ( imageFormats.contains("jpeg")) {
    formats += "*.jpeg *.jpg ";
  }

  if ( imageFormats.contains("tiff")) {
    formats += "*.tiff ";
  }

  if ( imageFormats.contains("tif")) {
    formats += "*.tif ";
  }

  if ( imageFormats.contains("png")) {
    formats += "*.png";
  }

  return QString( tr("HeightField PNG files ( ")+ formats + ")" );
};

QString FileHeightFieldPNGPlugin::getSaveFilters() {
  return QString( "" );
};

DataType  FileHeightFieldPNGPlugin::supportedType() {
  DataType type = DATA_TRIANGLE_MESH;
  return type;
}

bool FileHeightFieldPNGPlugin::showImageDialog(const QImage& _image, int* _minX, int* _maxX, int* _minY, int* _maxY, double* _height)
{
  ImageDialog imageDialog(_image);
  imageDialog.heightValue->setValue(*_height);

  int result = imageDialog.exec();

  if ( result == QDialog::Accepted) {
    *_minX   = imageDialog.minX->value();
    *_maxX   = imageDialog.maxX->value();
    *_minY   = imageDialog.minY->value();
    *_maxY   = imageDialog.maxY->value();
    *_height = imageDialog.heightValue->value();
  } else {
    return false;
  }
  return true;
}


int FileHeightFieldPNGPlugin::loadObject(QString _filename)
{

  QFile file(_filename);
  if ( !file.exists() ) {
    emit log(LOGERR,tr("Unable to load file: ") + _filename);
    return -1;
  }

  QImage image(_filename);

  // Prepare for taking specific regions from the file (defaults to full size)
  int minX = 0;
  int maxX = image.width();

  int minY = 0;
  int maxY = image.height();

  // Try to calculate a reasonable height
  double height = image.height() / 100;

  if ( OpenFlipper::Options::gui() ) {

    bool ret = false;
    QMetaObject::invokeMethod(this,"showImageDialog",
        // execute widget in main thread
        (QThread::currentThread() != QApplication::instance()->thread()) ? Qt::BlockingQueuedConnection: Qt::DirectConnection,
        Q_RETURN_ARG(bool, ret),
        Q_ARG(const QImage&, image),
        Q_ARG(int*,&minX),
        Q_ARG(int*,&maxX),
        Q_ARG(int*,&minY),
        Q_ARG(int*,&maxY),
        Q_ARG(double*,&height));
    if (!ret)
      return -1;
  }


  int id = -1;
  emit addEmptyObject( DATA_TRIANGLE_MESH, id );

  TriMeshObject* object = 0;
  if(PluginFunctions::getObject( id, object))
  {
    TriMesh* mesh = object->mesh();

    if ( mesh ) {


      // Load the data into the mesh.
      loadImageAsTriangleMesh(image,mesh,minX ,maxX , minY ,maxY, height);

      // Make sure everything is ready
      emit updatedObject(object->id(), UPDATE_ALL);

      // Tell core about update
      emit openedFile( id );

    }
  }

  return id;
}

void FileHeightFieldPNGPlugin::loadImageAsTriangleMesh(QImage& _image,TriMesh* _mesh,int _minX , int _maxX , int _minY , int _maxY, double _height) {

  const int mWidth  = _maxY - _minY;
  const int mHeigth = _maxX - _minX;

  // Reserve to get reasonable memory usage
  _mesh->reserve( mWidth*mHeigth, mWidth*mHeigth * 4, mWidth*mHeigth * 2  );

  for ( int i = _minX ; i < _maxX ; ++i ) {
    for ( int j = _minY ; j < _maxY ; ++j ) {
      const QColor currentColor = _image.pixel(i,j);
      double value = std::max(currentColor.redF(),currentColor.blueF());
      value = std::max(currentColor.greenF(),value);
      TriMesh::VertexHandle vh = _mesh->add_vertex(TriMesh::Point(i,j,-value * _height));
      _mesh->set_color(vh,TriMesh::Color(currentColor.redF(),currentColor.greenF(),currentColor.blueF(),1.0f));
    }
  }

    // Triangulate
    for ( int i = 0 ; i < mHeigth - 1 ; ++i ) {
      const int upperStart = mWidth * i;
      const int lowerStart = mWidth * (i + 1);

      for ( int j = 0 ; j < mWidth -1 ; ++j ) {
        std::vector<TriMesh::VertexHandle> handles;
        handles.push_back( _mesh->vertex_handle(upperStart + j) );
        handles.push_back( _mesh->vertex_handle(lowerStart + j + 1) );
        handles.push_back( _mesh->vertex_handle(lowerStart + j) );


        _mesh->add_face(handles);

        handles.clear();

        handles.push_back( _mesh->vertex_handle(upperStart + j) );
        handles.push_back( _mesh->vertex_handle(upperStart + j + 1) );
        handles.push_back( _mesh->vertex_handle(lowerStart + j + 1) );

        _mesh->add_face(handles);
      }
    }

  //Calculate some normals
  _mesh->update_normals();
}


bool FileHeightFieldPNGPlugin::saveObject(int _id, QString _filename) {
  return false;
}

#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2( fileheightfieldPpngplugin , FileHeightFieldPNGPlugin );
#endif


