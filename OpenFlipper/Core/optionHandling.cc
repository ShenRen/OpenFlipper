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




//=============================================================================
//
//  CLASS Core - IMPLEMENTATION of Path and File Handling
//
//=============================================================================


//== INCLUDES =================================================================

// -------------------- mview
#include "Core.hh"
// -------------------- ACG


//== IMPLEMENTATION ==========================================================

void Core::applyOptions(){

  if ( OpenFlipper::Options::gui() ) {

    //Init ViewModes
    coreWidget_->initViewModes();

    //Set default Viewmode (note: This always resets all toolboxes and should be skipped
    // here. Setting the initial view mode is instead done when initializing the core
    // and reading the ini-files.)
    //if (OpenFlipper::Options::currentViewMode() != "")
    //  coreWidget_->slotChangeView(OpenFlipper::Options::currentViewMode(), QStringList(), QStringList(), QStringList());
      
    //Set Fullscreen
    if ( OpenFlipperSettings().value("Core/Gui/fullscreen",false).toBool() )
      coreWidget_->setWindowState( coreWidget_->windowState() | Qt::WindowFullScreen);
    else
      coreWidget_->setWindowState( (coreWidget_->windowState() | Qt::WindowFullScreen) ^ Qt::WindowFullScreen);

    // Logger
    coreWidget_->showLogger( OpenFlipper::Options::loggerState() );

    // Toolbar Size
    switch ( OpenFlipperSettings().value("Core/Toolbar/iconSize",0).toInt() ){
      case 1  : coreWidget_->setIconSize( QSize(16,16) );break;
      case 2  : coreWidget_->setIconSize( QSize(32,32) );break;
      default : coreWidget_->setIconSize( coreWidget_->defaultIconSize() );
    }

    // gl mouse cursor
    coreWidget_->setForceNativeCursor( OpenFlipperSettings().value("Core/Gui/glViewer/nativeMouse",false).toBool() );
    
    //set viewer properties
    for (int i=0; i < PluginFunctions::viewers(); i++){

      // PluginFunctions::setDrawMode( OpenFlipper::Options::defaultDrawMode(i), i );
      PluginFunctions::setFixedView(OpenFlipper::Options::defaultViewingDirection(i), i );
      PluginFunctions::allowRotation(!OpenFlipper::Options::defaultLockRotation(i),i);

      //only switch projection here if an object is opened
      //this prevents problems when applying options on app start
      if ( PluginFunctions::objectCount() > 0 ){
        if ( OpenFlipper::Options::defaultPerspectiveProjectionMode(i)  )
          PluginFunctions::perspectiveProjection(i);
        else
          PluginFunctions::orthographicProjection(i);
      }
    }

    if ( OpenFlipperSettings().value("Core/Gui/glViewer/useMultipleViewers",true).toBool() )
      coreWidget_->setViewerLayout( OpenFlipper::Options::defaultViewerLayout() );


    // toolbox orientation
    coreWidget_->setToolBoxOrientationOnTheRight(OpenFlipperSettings().value("Core/Gui/ToolBoxes/ToolBoxOnTheRight",true).toBool());

    for ( unsigned int i = 0 ; i < OpenFlipper::Options::examinerWidgets() ; ++i ) {
      //wheel zoom factor
      PluginFunctions::viewerProperties(i).wheelZoomFactor( OpenFlipperSettings().value("Core/Mouse/Wheel/ZoomFactor",1).toDouble() );
      PluginFunctions::viewerProperties(i).wheelZoomFactorShift( OpenFlipperSettings().value("Core/Mouse/Wheel/ZoomFactorShift",0.2).toDouble() );
      PluginFunctions::viewerProperties(i).wheelInvert( OpenFlipperSettings().value("Core/Mouse/Wheel/Invert",false).toBool() );
    }

    //hideToolbox
    if ( OpenFlipperSettings().value("Core/Gui/ToolBoxes/hidden",false).toBool() ) 
      coreWidget_->showToolbox (false);

    //setup logFile
    if (logFile_ != 0){

      if (  OpenFlipperSettings().value("Core/Log/logFile").toString() != logFile_->fileName() ){
        logFile_->close();
        delete logFile_;
        logFile_ = 0;
        if (logStream_ != 0){
          delete logStream_;
          logStream_ = 0;
        }
      }
    }

    updateView();

  }
}

void Core::saveOptions(){
  QString inifile = OpenFlipper::Options::configDirStr() +  "OpenFlipper.ini";

  INIFile ini;
  if ( ! ini.connect( inifile ,false) ) {
    emit log(LOGERR,tr("Failed to connect to users ini file"));

    if ( ! ini.connect( inifile,true) ) {
      emit log(LOGERR,tr("Can not create user ini file"));
    } else {
      writeApplicationOptions(ini);
      ini.disconnect();
    }
  } else {
    writeApplicationOptions(ini);
    ini.disconnect();
  }
}

void Core::setupOptions() {

  // initialize the Settings:
  OpenFlipper::Options::initializeSettings();

  QStringList optionFiles;

  // ==============================================================
  // Global ini file in the application directory
  // ==============================================================
  QFile globalIni(OpenFlipper::Options::applicationDir().absolutePath() + OpenFlipper::Options::dirSeparator() + "OpenFlipper.ini");
  if ( globalIni.exists() )
    optionFiles.push_back(OpenFlipper::Options::applicationDir().absolutePath() + OpenFlipper::Options::dirSeparator() + "OpenFlipper.ini");

  // ==============================================================
  // Local ini file in the users home directory
  // ==============================================================
  QFile localIni(OpenFlipper::Options::configDirStr() + OpenFlipper::Options::dirSeparator() + "OpenFlipper.ini");
  if ( localIni.exists() )
    optionFiles.push_back(OpenFlipper::Options::configDirStr() + OpenFlipper::Options::dirSeparator() + "OpenFlipper.ini");

  // Set the previously generated option files
  OpenFlipper::Options::optionFiles(optionFiles);

  // ==============================================================
  // Default File open directories
  // ==============================================================
  // Default to OpenFlippers Texture dir
  OpenFlipper::Options::currentTextureDir(OpenFlipper::Options::textureDirStr());

  // Default to home directory
  OpenFlipperSettings().setValue("Core/CurrentDir", QDir::homePath() );

  // ==============================================================
  // Load Application options from all files available
  // ==============================================================

  for ( int i = 0 ; i < (int)optionFiles.size(); ++i) {

    INIFile _ini;

    if ( ! _ini.connect(optionFiles[i],false) ) {
      emit log(LOGERR,tr("Failed to connect to _ini file") + optionFiles[i]);
      continue;
    }

    readApplicationOptions(_ini);

    _ini.disconnect();
  }

}

/// restore key Assignments that were saved in config files
void Core::restoreKeyBindings(){

  QStringList optionFiles;

  // ==============================================================
  // Global ini file in the application directory
  // ==============================================================
  QFile globalIni(OpenFlipper::Options::applicationDir().absolutePath() + OpenFlipper::Options::dirSeparator() + "OpenFlipper.ini");
  if ( globalIni.exists() )
    optionFiles.push_back(OpenFlipper::Options::applicationDir().absolutePath() + OpenFlipper::Options::dirSeparator() + "OpenFlipper.ini");

  // ==============================================================
  // Local ini file in the users home directory
  // ==============================================================
  QFile localIni(OpenFlipper::Options::configDirStr() + OpenFlipper::Options::dirSeparator() + "OpenFlipper.ini");
  if ( localIni.exists() )
    optionFiles.push_back(OpenFlipper::Options::configDirStr() + OpenFlipper::Options::dirSeparator() + "OpenFlipper.ini");

  // Set the previously generated Optionfiles
  OpenFlipper::Options::optionFiles(optionFiles);


  // ==============================================================
  // Load Application options from all files available
  // ==============================================================

  for ( int i = 0 ; i < (int)optionFiles.size(); ++i) {

    INIFile _ini;

    if ( ! _ini.connect(optionFiles[i],false) ) {
      emit log(LOGERR,tr("Failed to connect to _ini file") + optionFiles[i]);
      continue;
    }

    if ( OpenFlipper::Options::gui() )
      coreWidget_->loadKeyBindings(_ini);

    _ini.disconnect();
  }
}

//=============================================================================
