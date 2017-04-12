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
//  CLASS PropertyVisPlugin
//
//=============================================================================


#ifndef PROPERTYVISPLUGIN_HH
#define PROPERTYVISPLUGIN_HH


//== INCLUDES =================================================================

#include <QObject>
#include <QMenuBar>
#include <QSpinBox>
#include <QFile>
#include <QTextStream>
#include <vector>

#include <OpenFlipper/common/Types.hh>
#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/LoadSaveInterface.hh>
#include <OpenFlipper/BasePlugin/ToolboxInterface.hh>
#include <OpenFlipper/BasePlugin/KeyInterface.hh>
#include <OpenFlipper/BasePlugin/MouseInterface.hh>
#include <OpenFlipper/BasePlugin/PickingInterface.hh>
#include <OpenFlipper/BasePlugin/ScriptInterface.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>

#include "PropertyVisToolbar.hh"
#include "ObjectListItemModel.hh"

#include "PropertyModel.hh"

//== CLASS DEFINITION =========================================================

class PropertyVisPlugin : public QObject, BaseInterface, LoadSaveInterface, ToolboxInterface, KeyInterface, ScriptInterface, MouseInterface, PickingInterface, LoggingInterface
{
  Q_OBJECT
  Q_INTERFACES(BaseInterface)
  Q_INTERFACES(LoadSaveInterface)
  Q_INTERFACES(ToolboxInterface)
  Q_INTERFACES(KeyInterface)
  Q_INTERFACES(ScriptInterface)
  Q_INTERFACES(MouseInterface)
  Q_INTERFACES(PickingInterface)
  Q_INTERFACES(LoggingInterface)

#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "org.OpenFlipper.Plugins.Plugin-PropertyVis")
#endif

signals:
  void updateView();
  void updatedObject(int, const UpdateType&);

  void log(Logtype, QString);
  void log(QString);
  
  // ToolboxInterface
  void addToolbox( QString  _name  , QWidget* _widget ,QIcon* _icon);

  //PickingInterface
  void addHiddenPickMode( const std::string& _mode );

  // Script Interface
  void getScriptingEngine( QScriptEngine*& _engine  ) override;

private slots:

  // BaseInterface
  void slotObjectUpdated( int _identifier, const UpdateType& _type );
  void slotAllCleared();
  
  // LoadSaveInterface
  void objectDeleted( int _id );

  // initialization functions
  void initializePlugin();
  void pluginsInitialized();

  // toolbox slots
  void slotMeshChanged(int _index = -1);
  void slotVisualize();
  
  // MouseInterface
  void slotMouseEvent( QMouseEvent* _event );
  
  //PickingInterface
  void slotPickModeChanged( const std::string& _mode);
  
  void updateGUI();

  /// Called when user selects a property.
  void propertySelectionChanged();

  /// Duplicates the selected properties.
  void slotDuplicateProperty();

  /// Removes the selected properties.
  void slotRemoveProperty();

  /// Receives log messages from PropertyModels and emits them
  void slotLog(Logtype _type, QString _message){ emit log(_type, _message); }
  void slotLog(QString _message){ emit log(_message); }


public slots:
  void slotVisualizeProperty( int _id, const QString& _propname );
  QScriptValue getPropertyVisualizer( int _id, const QString& _propname );


public :
  PropertyVisPlugin();
  ~PropertyVisPlugin() {}


  QString name() { return (QString("Property Visualization")); }
  QString description( ) { return (QString("Computes the PropertyVis of the the active Mesh")); }


private :
  // Widget for Toolbox
  PropertyVisToolbar* tool_;

private:

  /// Exchanges the PropertyModel after the user selected a different object.
  void setNewPropertyModel(int id);


private:
  ObjectListItemModel objectListItemModel_;

  PropertyModel* propertyModel_;
};

#if defined(INCLUDE_TEMPLATES) && !defined(PROPERTYVISPLUGIN_CC)
#define PROPERTYVISPLUGIN_TEMPLATES
#include "PropertyVisPluginT.cc"
#endif
//=============================================================================
#endif // PROPERTYVISPLUGIN_HH defined
//=============================================================================

