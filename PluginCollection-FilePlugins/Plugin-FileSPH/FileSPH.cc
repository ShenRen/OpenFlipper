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

#include "FileSPH.hh"
#include <ACG/GL/GLState.hh>

#include <OpenFlipper/BasePlugin/PluginFunctions.hh>
#include <OpenFlipper/common/GlobalOptions.hh>

#include <OpenMesh/Core/IO/IOManager.hh>

#include <OpenFlipper/ACGHelper/DrawModeConverter.hh>

#if QT_VERSION >= 0x050000 
  #include <QtWidgets>
#else
  #include <QtGui>
#endif

#include <QFileInfo>
#include <QSettings>

#include <iostream>



void FileSPHPlugin::initializePlugin() {
}

QString FileSPHPlugin::getLoadFilters() {
  return QString( tr("Sphere definition files ( *.sph )") );
};

QString FileSPHPlugin::getSaveFilters() {
  return QString( tr("Sphere definition files ( *.sph )") );
};

DataType  FileSPHPlugin::supportedType() {
  DataType type = DATA_SPHERE;
  return type;
}

int FileSPHPlugin::loadObject(QString _filename)
{
  int id = -1;
  emit addEmptyObject( DATA_SPHERE, id );

  SphereObject* sphereObject = 0;
  if(PluginFunctions::getObject( id, sphereObject))
  {    
    if( sphereObject )
    {

      QFileInfo fi(_filename);

      if ( fi.exists() ){
        SphereNode* sphere = PluginFunctions::sphereNode(sphereObject);

        QString name = sphereObject->name();
        
        QSettings settings(_filename, QSettings::IniFormat);
        settings.beginGroup("SPHERE");
        
        if ( settings.contains("Name") ) {
          
          QString name = settings.value("Name").toString();
          
          sphereObject->setName( name );
        } else {
          sphereObject->setFromFileName(_filename);
        }        
        

        
        if ( settings.contains("PositionX") ) {

          ACG::Vec3d position;
          position[0] = settings.value("PositionX").toDouble();
          position[1] = settings.value("PositionY").toDouble();
          position[2] = settings.value("PositionZ").toDouble();
          
          sphere->get_primitive(0).position = position;
        }
        
        if ( settings.contains("Size") ){
          
          double size = settings.value("Size").toDouble();
          
          sphere->get_primitive(0).size = size;
        }
        
        if ( settings.contains("Slices") ){
          
          int slices = settings.value("Slices").toDouble();
          
          sphere->get_primitive(0).slices = slices;
        }
        
        if ( settings.contains("Stacks") ){
          
          int stacks = settings.value("Stacks").toDouble();
          
          sphere->get_primitive(0).stacks = stacks;
        }
        
        settings.endGroup();
      }

      emit updatedObject( sphereObject->id(), UPDATE_ALL );
      emit openedFile( sphereObject->id() );

    }


  }

  return id;
};


bool FileSPHPlugin::saveObject(int _id, QString _filename)
{

  BaseObjectData*     obj(0);
  if(PluginFunctions::getObject( _id, obj))
  {
    SphereObject* sphereObject = PluginFunctions::sphereObject(obj);
    if( sphereObject )
    {
      SphereNode* sphere = PluginFunctions::sphereNode(sphereObject);

      QSettings settings(_filename, QSettings::IniFormat);
      settings.beginGroup("SPHERE");

      settings.setValue("Name",sphereObject->name());
      
      settings.setValue("PositionX", sphere->get_primitive(0).position[0]);
      settings.setValue("PositionY", sphere->get_primitive(0).position[1]);
      settings.setValue("PositionZ", sphere->get_primitive(0).position[2]);

      settings.setValue("Size",   sphere->get_primitive(0).size);
      settings.setValue("Slices", sphere->get_primitive(0).slices);
      settings.setValue("Stacks", sphere->get_primitive(0).stacks);

      settings.endGroup();
      
      obj->setFromFileName(_filename);
      obj->setName(obj->filename());
    }
  } else {
    emit log(LOGERR, tr("saveObject : cannot get object id %1 for save name %2").arg(_id).arg(_filename) );
    return false;
  }

  return true;
}

#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2( filesphplugin , FileSPHPlugin );
#endif


