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


#ifndef PLUGININFO_HH
#define PLUGININFO_HH

#include <QString>
#include <QObject>
#include <QStringList>
#include <QList>
#include <QDockWidget>
#include <QToolBar>

struct KeyBinding{
  int                   key;
  Qt::KeyboardModifiers modifiers;
  QString               description;
  bool                  multiUse;
  bool                  slot;
};

struct SlotInfo{
  QString slotName;
  QString slotDescription;
  QStringList parameters;
  QStringList descriptions;
};

/** Type defining a currently loaded Plugin */
class PluginInfo{

  public :

  PluginInfo() :
  plugin(0),
  name(""),
  description(""),
  version(""),
  path(""),
  rpcName(""),
  optionsWidget(0),
  buildIn(true),
  status(FAILED)
  {
    rpcFunctions.clear();
    slotInfos.clear();
    keys.clear();
    toolboxWidgets.clear();
    headerAreaWidgets.clear();
    toolboxIcons.clear();
    toolbars.clear();
    contextMenus.clear();
  }



  PluginInfo( const PluginInfo& _i) :
      plugin(_i.plugin),
      name(_i.name),
      description(_i.description),
      version(_i.version),
      path(_i.path),
      rpcName(_i.rpcName),
      rpcFunctions(_i.rpcFunctions),
      slotInfos(_i.slotInfos),
      keys(_i.keys),
      toolboxWidgets(_i.toolboxWidgets),
      headerAreaWidgets(_i.headerAreaWidgets),
      toolboxIcons(_i.toolboxIcons),
      toolbars(_i.toolbars),
      contextMenus(_i.contextMenus),
      optionsWidget(_i.optionsWidget),
      buildIn(_i.buildIn),
      status(_i.status)
  {

  }

  /// Pointer to the loaded plugin (Already casted when loading it)
  QObject*    plugin;

  /// Name of the plugin ( requested from the plugin on load)
  QString     name;

  /// Description of the plugin ( requested from the plugin on load)
  QString     description;

  /// Version of the plugin
  QString     version;

  /// Path to the plugin ( set on load )
  QString     path;

  /// Clean rpc name of the plugin
  QString     rpcName;

  /// List of exported rpc slots
  QStringList rpcFunctions;

  /// This list contains Descriptions about public slots if available
  QList< SlotInfo > slotInfos;

  /// List of registered keys with description
  QList< KeyBinding > keys;

  /// Pointer to plugins toolbox widget (if available)
  std::vector< std::pair< QString , QWidget* > > toolboxWidgets;

  /// Pointer to plugins header area widgets (if available)
  std::vector< std::pair< QString , QWidget* > > headerAreaWidgets;

  /// Pointer to plugins toolbox widget icons (if available)
  std::vector< QIcon* > toolboxIcons;
  
  /// Pointer to plugins toolbars (if available)
  std::vector< std::pair< QString , QToolBar* > > toolbars;
  
  /// Pointer to plugins context menus (if available)
  std::vector< std::pair< QString , QAction* > > contextMenus;

  /// Pointer to plugins options widget (if available)
  QWidget* optionsWidget;

  /// Indicates, if the plugin is a built in Plugin (in Plugin directory)
  bool buildIn;

  /// current status of the plugin
  enum StatusType
  {
    LOADED = 0,
    BLOCKED,
    UNLOADED,
    FAILED
  }status;


};

#endif //PLUGININFO_HH
