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

#include "FileLgt.hh"
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

void FileLightPlugin::initializePlugin() {
}

QString FileLightPlugin::getLoadFilters() {
  return QString( tr("Light definition files ( *.lgt )") );
};

QString FileLightPlugin::getSaveFilters() {
  return QString( tr("Light definition files ( *.lgt )") );
};

DataType  FileLightPlugin::supportedType() {
  DataType type = DATA_LIGHT;
  return type;
}

int FileLightPlugin::loadObject(QString _filename)
{
  int id = -1;
  emit addEmptyObject( DATA_LIGHT, id );

  LightObject* lightObject = 0;
  if(PluginFunctions::getObject( id, lightObject))
  {    
    if( lightObject )
    {

      QFileInfo fi(_filename);

      if ( fi.exists() ){
        LightSource* light = PluginFunctions::lightSource(lightObject);

        QString name = lightObject->name();

        QSettings settings(_filename, QSettings::IniFormat);
        settings.beginGroup("LIGHT");

        if ( settings.contains("Name") ) {
          
          QString name = settings.value("Name").toString();
          
          lightObject->setName( name );
        } else {
          lightObject->setFromFileName(_filename);
        }        
        
        if (settings.contains("PositionX")) {
          ACG::Vec3d position;
          position[0] = settings.value("PositionX").toDouble();
          position[1] = settings.value("PositionY").toDouble();
          position[2] = settings.value("PositionZ").toDouble();

          light->position(position);
        }

        if (settings.contains("DirectionX")) {
          ACG::Vec3d direction;
          direction[0] = settings.value("DirectionX").toDouble();
          direction[1] = settings.value("DirectionY").toDouble();
          direction[2] = settings.value("DirectionZ").toDouble();

          light->direction(direction);
        }

        if (settings.contains("SpotDirectionX")) {
          ACG::Vec3d spotDirection;
          spotDirection[0] = settings.value("SpotDirectionX").toDouble();
          spotDirection[1] = settings.value("SpotDirectionY").toDouble();
          spotDirection[2] = settings.value("SpotDirectionZ").toDouble();

          light->spotDirection(spotDirection);
        }

        if (settings.contains("AmbientColorR")) {
          ACG::Vec4f ambientColor;
          ambientColor[0] = settings.value("AmbientColorR").toDouble();
          ambientColor[1] = settings.value("AmbientColorG").toDouble();
          ambientColor[2] = settings.value("AmbientColorB").toDouble();
          if (settings.contains("AmbientColorA"))
            ambientColor[3] = settings.value("AmbientColorA").toDouble();
          else
            ambientColor[3] = 1.0f;

          light->ambientColor(ambientColor);
        }

        if (settings.contains("DiffuseColorR")) {
          ACG::Vec4f diffuseColor;
          diffuseColor[0] = settings.value("DiffuseColorR").toDouble();
          diffuseColor[1] = settings.value("DiffuseColorG").toDouble();
          diffuseColor[2] = settings.value("DiffuseColorB").toDouble();
          if (settings.contains("DiffuseColorA"))
            diffuseColor[3] = settings.value("DiffuseColorA").toDouble();
          else
            diffuseColor[3] = 1.0f;

          light->diffuseColor(diffuseColor);
        }

        if (settings.contains("SpecularColorR")) {
          ACG::Vec4f specularColor;
          specularColor[0] = settings.value("SpecularColorR").toDouble();
          specularColor[1] = settings.value("SpecularColorG").toDouble();
          specularColor[2] = settings.value("SpecularColorB").toDouble();
          if (settings.contains("SpecularColorA"))
            specularColor[3] = settings.value("SpecularColorA").toDouble();
          else
            specularColor[3] = 1.0f;

          light->specularColor(specularColor);
        }

        if (settings.contains("FixedPosition")) {

          bool fixedPosition = settings.value("FixedPosition").toBool();

          light->fixedPosition(fixedPosition);
        }

        if (settings.contains("SpotExponent")) {

          float spotExponent = settings.value("SpotExponent").toDouble();

          light->spotExponent(spotExponent);
        }

        if (settings.contains("SpotCutOff")) {

          float spotCutOff = settings.value("SpotCutOff").toDouble();

          light->spotCutoff(spotCutOff);
        }

        if (settings.contains("ConstantAttenuation")) {

          float cAttenuation = settings.value("ConstantAttenuation").toDouble();

          light->constantAttenuation(cAttenuation);
        }

        if (settings.contains("LinearAttenuation")) {

          float lAttenuation = settings.value("LinearAttenuation").toDouble();

          light->linearAttenuation(lAttenuation);
        }

        if (settings.contains("QuadraticAttenuation")) {

          float qAttenuation = settings.value("QuadraticAttenuation").toDouble();

          light->quadraticAttenuation(qAttenuation);
        }

        if (settings.contains("Radius")) {

          float radius = settings.value("Radius").toDouble();

          light->radius(radius);
        }

        if (settings.contains("Enabled")) {

          bool enabled = settings.value("Enabled").toBool();

          enabled ? light->enable() : light->disable();
        }
        
        settings.endGroup();
      }

      emit updatedObject( lightObject->id(), UPDATE_ALL );
      emit openedFile( lightObject->id() );

    }

  }

  return id;
};

bool FileLightPlugin::saveObject(int _id, QString _filename)
{

  BaseObjectData*     obj(0);
  if(PluginFunctions::getObject( _id, obj))
  {
    LightObject* lightObject = PluginFunctions::lightObject(obj);
    if( lightObject )
    {
        LightSource* light = PluginFunctions::lightSource(lightObject);

        QSettings settings(_filename, QSettings::IniFormat);
        settings.beginGroup("LIGHT");

        settings.setValue("Name",lightObject->name());
        
        if(!light->directional()) {
            settings.setValue("PositionX", light->position()[0]);
            settings.setValue("PositionY", light->position()[1]);
            settings.setValue("PositionZ", light->position()[2]);
        } else {
            settings.setValue("DirectionX", light->direction()[0]);
            settings.setValue("DirectionY", light->direction()[1]);
            settings.setValue("DirectionZ", light->direction()[2]);
        }

        settings.setValue("SpotDirectionX", light->spotDirection()[0]);
        settings.setValue("SpotDirectionY", light->spotDirection()[1]);
        settings.setValue("SpotDirectionZ", light->spotDirection()[2]);
        
        settings.setValue("AmbientColorR", light->ambientColor()[0]);
        settings.setValue("AmbientColorG", light->ambientColor()[1]);
        settings.setValue("AmbientColorB", light->ambientColor()[2]);
        settings.setValue("AmbientColorA", light->ambientColor()[3]);
        
        settings.setValue("DiffuseColorR", light->diffuseColor()[0]);
        settings.setValue("DiffuseColorG", light->diffuseColor()[1]);
        settings.setValue("DiffuseColorB", light->diffuseColor()[2]);
        settings.setValue("DiffuseColorA", light->diffuseColor()[3]);
        
        settings.setValue("SpecularColorR", light->specularColor()[0]);
        settings.setValue("SpecularColorG", light->specularColor()[1]);
        settings.setValue("SpecularColorB", light->specularColor()[2]);
        settings.setValue("SpecularColorA", light->specularColor()[3]);
        
        settings.setValue("FixedPosition", light->fixedPosition());
        
        settings.setValue("SpotExponent", light->spotExponent());
        
        settings.setValue("SpotCutOff", light->spotCutoff());
        
        settings.setValue("ConstantAttenuation", light->constantAttenuation());
        
        settings.setValue("LinearAttenuation", light->linearAttenuation());
        
        settings.setValue("QuadraticAttenuation", light->quadraticAttenuation());
        
        settings.setValue("Radius", light->radius());
        
        settings.setValue("Enabled", light->enabled());
                
        settings.endGroup();
        
        obj->setFromFileName(_filename);
        obj->setName(obj->filename());
    }
  }

  return true;
}
#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2( filelightplugin , FileLightPlugin );
#endif


