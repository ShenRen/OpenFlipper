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

#include "FileOfv.hh"

#include <iostream>


void FileViewPlugin::initializePlugin() {
}

QString FileViewPlugin::getLoadFilters() {
  return QString( tr("View definition files ( *.ofv )") );
}

QString FileViewPlugin::getSaveFilters() {
  return QString( tr("View definition files ( *.ofv )") );
}

DataType  FileViewPlugin::supportedType() {
  return DataType();
}

int FileViewPlugin::loadObject(QString _filename) {

    // Declare variables
    ACG::Vec3d  eye(1.0,1.0,1.0);
    ACG::Vec3d  center(0.0,0.0,0.0);
    ACG::Vec3d  up(1.0,0.0,0.0);
    ACG::Vec4f  background;
    
    bool e_eye = false;
    bool e_center = false;
    bool e_up = false;
    bool e_background = false;
    
    QSettings settings(_filename, QSettings::IniFormat);
    settings.beginGroup("VIEW");

    if(settings.contains("Width") && settings.contains("Height")) {
        const int width = settings.value("Width").toInt();
        const int height = settings.value("Height").toInt();
        std::cerr << "Setting new viewport to " << width << "x" << height << std::endl;
    }
    
    if(settings.contains("EyeX")) {
        eye[0] = settings.value("EyeX").toDouble();
        eye[1] = settings.value("EyeY").toDouble();
        eye[2] = settings.value("EyeZ").toDouble();
        std::cerr << "Setting new eye position to " << eye << std::endl;
        e_eye = true;
    }
    
    if(settings.contains("CenterX")) {
        center[0] = settings.value("CenterX").toDouble();
        center[1] = settings.value("CenterY").toDouble();
        center[2] = settings.value("CenterZ").toDouble();
        std::cerr << "Setting new scene center to " << center << std::endl;
        e_center = true;
    }
    
    if(settings.contains("UpX")) {
        up[0] = settings.value("UpX").toDouble();
        up[1] = settings.value("UpY").toDouble();
        up[2] = settings.value("UpZ").toDouble();
        std::cerr << "Setting new up vector to " << up << std::endl;
        e_up = true;
    }
    
    
    if(settings.contains("BackgroundR")) {
        background[0] = settings.value("BackgroundR").toDouble();
        background[1] = settings.value("BackgroundG").toDouble();
        background[2] = settings.value("BackgroundB").toDouble();
        background[3] = settings.value("BackgroundA").toDouble();
        std::cerr << "Setting new background color to " << background << std::endl;
        e_background = true;
    }

    settings.endGroup();
    
    // Now set new projection and view
       
    // LookAt
    ACG::Vec3d new_eye = e_eye ? eye : PluginFunctions::eyePos();
    ACG::Vec3d new_center = e_center ? center : PluginFunctions::sceneCenter();
    ACG::Vec3d new_up = e_up ? up : PluginFunctions::upVector();
        
    PluginFunctions::lookAt(new_eye, new_center, new_up);
        
    // Background color
    if(e_background) {
        PluginFunctions::setBackColor(background);
     }
    
    emit updateView();
  
    return 0;
}

bool FileViewPlugin::saveObject(int /*_id*/, QString /*_filename*/) {
  
  return true;
}

#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2( fileviewplugin , FileViewPlugin );
#endif


