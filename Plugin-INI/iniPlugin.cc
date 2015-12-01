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

// Includes
#include "iniPlugin.hh"

#include <ObjectTypes/PolyMesh/PolyMesh.hh>
#include <ObjectTypes/Light/Light.hh>
#include <ObjectTypes/TriangleMesh/TriangleMesh.hh>

/// Constructor
INIPlugin::INIPlugin() {
}


template <>
void INIPlugin::parseIniFileT(INIFile& _ini, LightObject* _object) {
  
  if ( _object ) {
    
    std::vector< QString > draw_modes;
    if( _ini.get_entry(draw_modes, _object->name(), "MaterialDrawMode") )
      _object->materialNode()->drawMode( listToDrawMode(draw_modes) );
    
  }
}


template <>
void INIPlugin::saveIniFileT(INIFile& _ini, LightObject* _object) {
  
  if ( _object ) {
    
    _ini.add_entry( _object->name() ,
                    "MaterialDrawMode" ,
                    drawModeToList( _object->materialNode()->drawMode()) );    
                    
  }
}

void INIPlugin::parseIniFile(INIFile& _ini, BaseObjectData* _object) {
  
  if ( _object ) {
    std::vector< QString > draw_modes;
    if( _ini.get_entry(draw_modes, _object->name(), "MaterialDrawMode") )
      _object->materialNode()->drawMode( listToDrawMode(draw_modes) );
    
    ACG::Vec4f col(0.0,0.0,0.0,0.0);
    
    if ( _ini.get_entryVecf( col, _object->name() , "BaseColor" ) )
      _object->materialNode()->set_base_color(col);
    
    if ( _ini.get_entryVecf( col, _object->name() , "AmbientColor" ) )
      _object->materialNode()->set_ambient_color(col);
    
    if ( _ini.get_entryVecf( col, _object->name() , "DiffuseColor" ) )
      _object->materialNode()->set_diffuse_color(col);
    
    if ( _ini.get_entryVecf( col, _object->name() , "SpecularColor" ) )
      _object->materialNode()->set_specular_color(col);
    
    if ( _ini.get_entryVecf( col, _object->name() , "OverlayColor" ) )
      _object->materialNode()->set_overlay_color(col);

    double shininess;
    if ( _ini.get_entry( shininess, _object->name() , "Shininess" ) )
      _object->materialNode()->set_shininess(shininess);
    
    double reflectance;
    if ( _ini.get_entry( reflectance, _object->name() , "Reflectance" ) ) {
      _object->materialNode()->set_reflectance(reflectance);        
    }
    
    bool visible;
    if ( _ini.get_entry( visible, _object->name() , "Visible" ) ) {
      _object->visible(visible);        
    }
    
    int size = 1;
    if ( _ini.get_entry( size, _object->name() , "PointSize" ) )
      _object->materialNode()->set_point_size(size);
    if ( _ini.get_entry( size, _object->name() , "LineWidth" ) )
      _object->materialNode()->set_line_width(size);
  }
}


void INIPlugin::saveIniFile(INIFile& _ini, BaseObjectData* _object) {
  
  if ( _object ) {
        _ini.add_entry( _object->name() ,
            "MaterialDrawMode" ,
            drawModeToList( _object->materialNode()->drawMode()) );
        _ini.add_entryVec( _object->name() ,
            "BaseColor" ,
            _object->materialNode()->base_color()) ;
        _ini.add_entryVec( _object->name() ,
            "AmbientColor" ,
            _object->materialNode()->ambient_color()) ;
        _ini.add_entryVec( _object->name() ,
            "DiffuseColor" ,
            _object->materialNode()->diffuse_color());
        _ini.add_entryVec( _object->name() ,
            "SpecularColor" ,
            _object->materialNode()->specular_color());
        _ini.add_entryVec( _object->name() ,
            "OverlayColor" ,
            _object->materialNode()->overlay_color());
        _ini.add_entry( _object->name() ,
            "Shininess" ,
            _object->materialNode()->shininess());
        _ini.add_entry( _object->name() ,
            "Reflectance" ,
            _object->materialNode()->reflectance());       
        _ini.add_entry( _object->name() ,
            "Visible" ,
            _object->visible() );     
        _ini.add_entry( _object->name() ,
            "PointSize" ,
            _object->materialNode()->point_size());
        _ini.add_entry( _object->name() ,
            "LineWidth" ,
            _object->materialNode()->line_width());
    }
    
}

void INIPlugin::loadIniFile( INIFile& _ini, int _id ) {
    
  BaseObjectData* baseObject;
  if (!PluginFunctions::getObject(_id, baseObject)) {
    emit log(LOGERR, tr("Cannot find object for id ") + QString::number(_id) + tr(" in saveFile"));
    return;
  }

  // Load all data from baseobjectdata part
  parseIniFile(_ini, baseObject);

  if (baseObject->dataType() == DATA_POLY_MESH) {
    PolyMeshObject* polyObject = PluginFunctions::polyMeshObject(baseObject);
    parseIniFileT(_ini, polyObject);
  } else if (baseObject->dataType() == DATA_TRIANGLE_MESH) {
    TriMeshObject* triObject = PluginFunctions::triMeshObject(baseObject);
    parseIniFileT(_ini, triObject);
  } else if (baseObject->dataType() == DATA_LIGHT) {
    LightObject* lightObject = PluginFunctions::lightObject(baseObject);
    parseIniFileT(_ini, lightObject);
  } else {
    // Unhandled data type
    emit log(LOGERR, tr("The specified data type is not supported, yet. Aborting!"));
  }
}

void INIPlugin::saveIniFile( INIFile& _ini, int _id) {

  BaseObjectData* baseObject;
  if (!PluginFunctions::getObject(_id, baseObject)) {
    emit log(LOGERR, tr("Cannot find object for id ") + QString::number(_id) + tr(" in saveFile"));
    return;
  }

  // Save all data from baseobjectdata part
  saveIniFile(_ini, baseObject);

  if (baseObject->dataType() == DATA_POLY_MESH) {
    PolyMeshObject* polyObject = PluginFunctions::polyMeshObject(baseObject);
    saveIniFileT(_ini, polyObject);
  } else if (baseObject->dataType() == DATA_TRIANGLE_MESH) {
    TriMeshObject* triObject = PluginFunctions::triMeshObject(baseObject);
    saveIniFileT(_ini, triObject);
  } else if (baseObject->dataType() == DATA_LIGHT) {
    LightObject* lightObject = PluginFunctions::lightObject(baseObject);
    saveIniFileT(_ini, lightObject);
  } else {
    // Unhandled data type
    emit log(LOGERR, tr("The specified data type is not supported, yet. Aborting!"));
  }

}


#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2( iniplugin , INIPlugin );
#endif

