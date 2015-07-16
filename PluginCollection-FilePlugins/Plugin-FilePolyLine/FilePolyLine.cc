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

#include "FilePolyLine.hh"
#include <ACG/GL/GLState.hh>

#include <OpenFlipper/BasePlugin/PluginFunctions.hh>
#include <OpenFlipper/common/GlobalOptions.hh>

#include <OpenMesh/Core/IO/IOManager.hh>

#include <OpenFlipper/ACGHelper/DrawModeConverter.hh>

#include <ObjectTypes/PolyLine/PolyLineObjectSerializer.hh>

#if QT_VERSION >= 0x050000 
  #include <QtWidgets>
#else
  #include <QtGui>
#endif

#include <iostream>

//-----------------------------------------------------------------------------

void FilePolyLinePlugin::initializePlugin() {
  // Data type has already been added by polyline type plugin
}

QString FilePolyLinePlugin::getLoadFilters() {
  return QString( tr("Poly-Line files ( *.pol )" ));
};

QString FilePolyLinePlugin::getSaveFilters() {
  return QString( tr("Poly-Line files ( *.pol )" ));
};

DataType  FilePolyLinePlugin::supportedType() {
  DataType type = DATA_POLY_LINE;
  return type;
}

int FilePolyLinePlugin::loadObject(QString _filename)
{
  int id = -1;
  emit addEmptyObject( DATA_POLY_LINE, id );

  PolyLineObject* pol(0);
  if(PluginFunctions::getObject( id, pol))
  {
      std::ifstream fin(_filename.toLatin1(), std::ios::in);
      PolyLineObjectSerializer serializer(pol);
      serializer.deserialize(fin);
      fin.close();

      pol->lineNode()->drawMode(ACG::SceneGraph::DrawModes::WIREFRAME |
              ACG::SceneGraph::DrawModes::POINTS);
      pol->setFromFileName(_filename);
      pol->setName(QFileInfo(_filename).fileName());
  }

  emit openedFile( pol->id() );

  return id;
};

bool FilePolyLinePlugin::saveObject(int _id, QString _filename)
{
  BaseObjectData*     obj(0);
  if(PluginFunctions::getObject( _id, obj))
  {
    PolyLineObject* pol = PluginFunctions::polyLineObject(obj);
    if( pol)
    {
      std::cerr << "call Line Function\n";
      std::cerr << (const char*) _filename.toLatin1() << std::endl;

      obj->setName(_filename.section(OpenFlipper::Options::dirSeparator(),-1));
      obj->setPath(_filename.section(OpenFlipper::Options::dirSeparator(),0,-2) );

      std::ofstream fout(_filename.toLatin1(), std::ios::out);
      PolyLineObjectSerializer serializer(pol);
      serializer.serialize(fout);
      fout.close();
      return true;
    } else {
      return false;
    }
  } else {
    emit log(LOGERR, tr("saveObject : cannot get object id %1 for save name %2").arg(_id).arg(_filename) );
    return false;
  }

  return true;
}

void FilePolyLinePlugin::loadIniFileLast( INIFile& _ini ,int _id ) {
  BaseObjectData* baseObject;
  if ( !PluginFunctions::getObject(_id,baseObject) ) {
    emit log(LOGERR,tr("Cannot find object for id %1 in saveFile!").arg(_id));
    return;
  }

  PolyLineObject* object = PluginFunctions::polyLineObject(baseObject);

  if ( object ) {
     ACG::Vec4f col(0.0,0.0,0.0,0.0);

    if ( _ini.get_entryVecf( col, object->name() , "BaseColor" ) )
      object->materialNode()->set_base_color(col);
  }

}

void FilePolyLinePlugin::saveIniFile( INIFile& _ini ,int _id) {
  BaseObjectData* baseObject;
  if ( !PluginFunctions::getObject(_id,baseObject) ) {
    emit log(LOGERR,tr("Cannot find object for id %1 in saveFile!").arg(_id) );
    return;
  }

  PolyLineObject* object = PluginFunctions::polyLineObject(baseObject);

  if ( object ) {
      _ini.add_entryVec( object->name() ,
                         "BaseColor" ,
                         object->materialNode()->base_color() ) ;
  }
}
#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2( filepolylineplugin , FilePolyLinePlugin );
#endif


