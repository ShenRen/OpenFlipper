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

#ifndef BACKUPPLUGININTERFACE_HH 
#define BACKUPPLUGININTERFACE_HH 


#include <OpenFlipper/common/Types.hh>

#if QT_VERSION >= 0x050000 
  #include <QtWidgets>
#else
  #include <QtGui>
#endif
 
 /** \file BackupInterface.hh
*
* Interface class for backup handling.\ref backupInterfacePage
*/

/** \brief Interface class for backup handling.
  *
  * \ref backupInterfacePage "Detailed description"
  * \n
  *
  * This interface defines functions to implement backup and restore
  * for objects or object groups.
 */
class BackupInterface {
  
  //===========================================================================
  /** @name Interface definition for general Plugins
  * @{ */
  //===========================================================================
  
  signals:

    /** \brief Tell Backup Plugin to create a backup
    *
    * Plugins which supports backups can call this function if they want to create backups.\n
    * A Backup control Plugin will do the rest.
    *
    * @param _objectid Identifier of the object to create the backup
    * @param _name     Name of the Backup, to show the user what can be recovered
    * @param _type     The type of the backup (e.g. UPDATE_SELECTION)
    */
    virtual void createBackup( int _objectid, QString _name, UpdateType _type = UPDATE_ALL){};

    /** \brief Tell Backup Plugin to create a group backup
    *
    * Plugins which supports backups can call this function if they want to create group backups.\n
    * The backups specified here will be grouped together. They can only be reverted as one block and
    * not one by one. They combine backups on multiple objects to a single backup set.
    *
    * A Backup control Plugin will do the rest.
    *
    * @param _objectids Identifier of the object to create the backup
    * @param _name      Name of the Backup, to show the user what can be recovered
    * @param _types     The types of the backups (e.g. UPDATE_SELECTION)
    */
    virtual void createBackup( IdList _objectids, QString _name, std::vector<UpdateType> _types){};

    /** \brief Tell Backup Plugin to undo the last action of an object
    *
    * Plugins which supports backups can call this function if they want to restore backups.\n
    * A Backup control Plugin will do the rest.
    * @param _objectid   Identifier of the object to restore
    */
    virtual void undo(int _objectid) {};

    /** \brief Tell Backup Plugin to redo the last action on an object
    *
    * Plugins which supports backups can call this function if they want to restore backups.\n
    * A Backup control Plugin will do the rest.
    * @param _objectid   Identifier of the object to restore
    */
    virtual void redo(int _objectid) {};

    /** \brief Tell Backup Plugin to undo the last action
    *
    * Plugins which supports backups can call this function if they want to restore backups.\n
    * A Backup control Plugin will do the rest.
    */
    virtual void undo(){};

    /** \brief Tell Backup Plugin to redo the last action
    *
    * Plugins which supports backups can call this function if they want to restore backups.\n
    * A Backup control Plugin will do the rest.
    */
    virtual void redo(){};

  private slots:

    /** \brief Backup for an object requested
    * 
    * This function will be called if a plugin requests a backup. You can
    * also react on this event if you reimplement this function in your plugin.
    *
    * @param _objectid Identifier of the object to create the backup
    * @param _name Name of the Backup, to show the user what can be recovered
    * @param _type What has been updated (This can be used to restrict the backup to certain parts of the object)
    */
    virtual void slotCreateBackup( int _objectid , QString _name , UpdateType _type = UPDATE_ALL) {};

    /** \brief Backup for an object requested
    * 
    * This function will be called if a plugin requests a backup. You can
    * also react on this event if you reimplement this function in your plugin.
    *
    * @param _objectids Identifiers of the object to create the backup
    * @param _name Name of the Backup, to show the user what can be recovered
    * @param _types What has been updated (This can be used to restrict the backup to certain parts of the objects)
    */
    virtual void slotCreateBackup( IdList _objectids , QString _name , std::vector<UpdateType> _types) {};

    /** \brief A given object will be restored.
    *
    * This function is called before an object is restored from a backup.
    * perObjectDatas and the object will be reset to the backup state
    * after this function is called for all plugins.
    *
    * If you have any pointers or references to the given object you have to
    * clean them up here.
    *
    * @param _objectid         Identifier of the object which is about to be restored
    */
    virtual void slotAboutToRestore( int _objectid ) {};

    /** \brief Object fully restored
    *
    * This function is called after an object and all data from other plugins
    * is restored from a backup.
    *
    * perObjectDatas and the object have been reset to the backup state.
    *
    * @param _objectid   Identifier of the object which is about to be restored
    */
    virtual void slotRestored( int _objectid ) {};

   public:

    /// Destructor
    virtual ~BackupInterface() {};

  /** @} */

  //===========================================================================
  /** @name Interface definition for Backup Plugins
  *
  *  These signals and slots have to be implemented if you create a plugin managing 
  *  Backups ( A Backup plugin is already provided by OpenFlipper so you don't need
  *  to use these funcions).
  * @{ */
  //===========================================================================     
  
  signals: 
    /** \brief Backup Plugin tells other Plugins that a restore will happen
    *
    */
    virtual void aboutToRestore(int _objectid ) {};

    /** \brief Backup Plugin tells other Plugins that a restore has happened
    *
    */
    virtual void restored( int _objectid ) {};
    
    
  private slots:
    
    
    /** \brief Undo the last action of an object
    *
    * This function has to be implemented in the backup management plugin. Normally
    * this function is provided by the default backup plugin and should not be used!
    * To restore data in your plugin use the slotRestore above.
    *
    * @param _objectid   Identifier of the object to restore
    */
    virtual void slotUndo(int _objectid) {};

    /** \brief Redo the last action on an object
    *
    * This function has to be implemented in the backup management plugin. Normally
    * this function is provided by the default backup plugin and should not be used!
    * To restore data in your plugin use the slotRestore above.
    *
    * @param _objectid   Identifier of the object to restore
    */
    virtual void slotRedo(int _objectid) {};

    /** \brief Undo the last action
    *
    * This function has to be implemented in the backup management plugin. Normally
    * this function is provided by the default backup plugin and should not be used!
    * To restore data in your plugin use the slotRestore above.
    *
    */
    virtual void slotUndo(){};

    /** \brief Redo the last action
    *
    * This function has to be implemented in the backup management plugin. Normally
    * this function is provided by the default backup plugin and should not be used!
    * To restore data in your plugin use the slotRestore above.
    *
    */
    virtual void slotRedo(){};

  signals:

    /** \brief This signal is emitted by a BackupPlugin and tells a TypePlugin to generate a backup
     *
     * @param _id   Id of the added object
     * @param _name Name of the backup to generate
     * @param _type the type of backup that needs to be done
     */
    virtual void generateBackup( int _id, QString _name, UpdateType _type ) {};

  /** @} */

};

/** \page backupInterfacePage Backup Interface
 * \n
\image html BackupInterface.png
\n

\section BackupInterfacePage_Overview Overview
The Backup interface can be used to manage backups within OpenFlipper. It provides abstractions for backup and
redo operations which get managed by the backup plugin.

\section backupInterfacePage_generatingBackups Generating Backups
To generate a backup of a specific object, you just have to derive from the BackupInterface and emit the
BackupInterface::createBackup() signal. A simple example looks like this:

\code
emit createBackup(object->id(),"Smoothing", UPDATE_GEOMETRY );
\endcode

We create a backup of the object with the given id. The Backup will be named "Smoothing" and the backup
system is informed that only the geometry has changed.

It is also possible to create backup groups. This is required, if you change several objects at once and
they should be restored only together.

\section backupInterfacePage_restoringBackups Restoring Backups
Backups can be restored and re-applied. You can simply emit the BackupInterface::undo() signal and the
last backup will be restored. BackupInterface::redo() will re apply the last operation stored. It is
also possible to restore backups of a specific object via BackupInterface::undo(int) and
BackupInterface::redo(int), if they are not blocked by a group backup which can only be reverted as one
operation.\n

The other plugins will be informed about a restore operation by BackupInterface::slotAboutToRestore(int)
and BackupInterface::slotRestored(int) which will be emitted directly before and after the restore operation.


\section backupInterfacePage_usage Usage
To use the BackupInterface:
<ul>
<li> include BackupInterface.hh in your plugins header file
<li> derive your plugin from the class BackupInterface
<li> add Q_INTERFACES(BackupInterface) to your plugin class
<li> And implement the required slots and functions
</ul>

*/


Q_DECLARE_INTERFACE(BackupInterface,"GUI.BackupInterface/1.1")
      
#endif // BACKUPPLUGININTERFACE_HH
