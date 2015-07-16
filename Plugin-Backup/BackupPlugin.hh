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

#ifndef BACKUPPLUGIN_HH
#define BACKUPPLUGIN_HH

#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/KeyInterface.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>
#include <OpenFlipper/BasePlugin/MenuInterface.hh>
#include <OpenFlipper/BasePlugin/BackupInterface.hh>
#include <OpenFlipper/BasePlugin/ToolbarInterface.hh>
#include <OpenFlipper/BasePlugin/LoadSaveInterface.hh>
#include <OpenFlipper/BasePlugin/ContextMenuInterface.hh>
#include <OpenFlipper/BasePlugin/OptionsInterface.hh>
#include <OpenFlipper/common/Types.hh>

#include <QObject>
#include <QMenuBar>

#include "GroupData.hh"

class BackupPlugin : public QObject, BaseInterface , KeyInterface, MenuInterface, BackupInterface, LoggingInterface, ToolbarInterface, LoadSaveInterface, ContextMenuInterface, OptionsInterface
{
Q_OBJECT
Q_INTERFACES(BaseInterface)
Q_INTERFACES(KeyInterface)
Q_INTERFACES(MenuInterface)
Q_INTERFACES(BackupInterface)
Q_INTERFACES(LoggingInterface)
Q_INTERFACES(ToolbarInterface)
Q_INTERFACES(LoadSaveInterface)
Q_INTERFACES(ContextMenuInterface)
Q_INTERFACES(OptionsInterface)

#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "org.OpenFlipper.Plugins.Plugin-Backup")
#endif

signals:
  // BaseInterface
  void updateView();
  void updatedObject(int _identifier, const UpdateType& _type);
  void setSlotDescription(QString     _slotName    ,   QString     _slotDescription,
                                        QStringList _parameters  , QStringList _descriptions);
  
  //  LoggingInterface
  void log(Logtype _type, QString _message);
  void log(QString _message);

  // MenuInterface
  void getMenubarMenu (QString _name, QMenu *& _menu, bool _create);
  
  // BackupInterface
  void undo(int _objectid);
  void undo();
  void redo(int _objectid);
  void redo();
  void aboutToRestore(int _objectid);
  void restored(int _objectid);
  void generateBackup( int _id, QString _name, UpdateType _type);
  
  // ToolBarInterface
  void addToolbar(QToolBar* _toolbar);
  void getToolBar( QString _name, QToolBar*& _toolbar);
  
  //KeyInterface
  void registerKey(int _key, Qt::KeyboardModifiers _modifiers, QString _description, bool _multiUse = false);

  //ContextMenuInterface
  void addContextMenuItem(QAction* _action , DataType _objectType, ContextMenuType _type);
  
private slots:

  // BaseInterface
  void initializePlugin();
  void pluginsInitialized();
  
  void slotAllCleared();
  
  // KeyInterface
  void slotKeyEvent( QKeyEvent* _event );
  
  // LoadSaveInterface
  void objectDeleted (int _objectid);
   
  // Backup Interface
  void slotCreateBackup( int _objectid, QString _name, UpdateType _type = UPDATE_ALL);
  void slotCreateBackup( IdList _objectids , QString _name, std::vector<UpdateType> _types);
  void slotUndo(int _objectid);
  void slotRedo(int _objectid);
  void slotUndo();
  void slotRedo();

  //ContextMenuInterface
  void slotUpdateContextMenu( int _objectId );

  // OptionsInterface
  bool initializeOptionsWidget(QWidget*& _widget);
  void applyOptions();

private slots:
  
  void updateButtons();
  
  /// Backups enabled or disabled checkbox
  void slotEnableDisableBackups();

  void slotObjectUndo();
  void slotObjectRedo();

public slots:
  QString version() { return QString("0.1"); };  
  
  void createBackup(int _objectId, QString _name, UpdateType _type= UPDATE_ALL);

public:
  ~BackupPlugin() {};
  BackupPlugin();
  
  QString name() { return (QString("Backup")); };
  QString description( ) { return (QString("Creates Backups of objects when supported by plugins")); };

private :
  GroupData globalBackup_;

  /// The backup Menu
  QMenu*  backupMenu_;

  QAction* backupsEnabledAction_;

  QAction* undoMenuAction_;
  QAction* redoMenuAction_;

  QAction* undoToolAction_;
  QAction* redoToolAction_;
  
  QAction* undoContextAction_;
  QAction* redoContextAction_;

  QSpinBox* maxBackupSpinBox_;
};

#endif //BACKUPPLUGIN_HH
