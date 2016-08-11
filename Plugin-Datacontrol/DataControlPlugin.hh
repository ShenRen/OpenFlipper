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




#ifndef DATACONTROLPLUGIN_HH
#define DATACONTROLPLUGIN_HH


#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/ToolboxInterface.hh>
#include <OpenFlipper/BasePlugin/KeyInterface.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>
#include <OpenFlipper/BasePlugin/ContextMenuInterface.hh>
#include <OpenFlipper/BasePlugin/ScriptInterface.hh>
#include <OpenFlipper/BasePlugin/INIInterface.hh>
#include <OpenFlipper/BasePlugin/LoadSaveInterface.hh>
#include <OpenFlipper/common/Types.hh>
#include <ObjectTypes/Light/Light.hh>
#include "Toolbox.hh"

#include "TreeModel.hh"
#include "DefaultObjectMarker.hh"


/** Plugin for controlling the Object data structures (Show,hide Object, Target, Source selection
*/
class DataControlPlugin : public QObject, BaseInterface, ToolboxInterface, KeyInterface , LoggingInterface, INIInterface, ContextMenuInterface, ScriptInterface, LoadSaveInterface
{
  Q_OBJECT
  Q_INTERFACES(BaseInterface)
  Q_INTERFACES(ToolboxInterface)
  Q_INTERFACES(KeyInterface)
  Q_INTERFACES(LoggingInterface)
  Q_INTERFACES(INIInterface)
  Q_INTERFACES(ContextMenuInterface)
  Q_INTERFACES(ScriptInterface)
  Q_INTERFACES(LoadSaveInterface)

#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "org.OpenFlipper.Plugins.Plugin-DataControl")
#endif

  signals:
    // BaseInterface
    void updateView();
    void updatedObject(int _it, const UpdateType& _type);

    void setSlotDescription(QString     _slotName,   QString     _slotDescription,
                            QStringList _parameters, QStringList _descriptions);

    // LoggingInterface
    void log(Logtype _type, QString _message);
    void log(QString _message);
    
    // ContextMenuInterface
    void addContextMenuItem(QAction* _action , ContextMenuType _type);
    void addContextMenuItem(QAction* _action , DataType _objectType , ContextMenuType _type );
    
    // LoadSaveInterface
    void deleteObject( int _id );
    void emptyObjectAdded( int _id );
    void copyObject( int _oldId, int& _newId);
    
    // ToolboxInterface
    void addToolbox( QString _name  , QWidget* _widget, QIcon* _icon, QWidget *_headerAreaWidget);

    void objectsGrouped(IdList);

  private slots :
    // BaseInterface
    void initializePlugin();
    void pluginsInitialized();
    void slotVisibilityChanged      ( int _identifier );
    void slotObjectSelectionChanged ( int _identifier );
    void slotObjectPropertiesChanged( int _identifier );
    void slotObjectUpdated( int _identifier, const UpdateType& _type );

    // KeyInterface
    void slotKeyEvent( QKeyEvent* _event );

    // INIInterface
    void loadIniFileOptionsLast( INIFile& _ini );
    void saveIniFileOptions( INIFile& _ini );
    void saveOnExit(INIFile& _ini );

    // LoadSaveInterface
    void fileOpened(int _id);
    void addedEmptyObject(int _id);
    void objectDeleted(int _id);

    // Tell system that this plugin runs without ui
    void noguiSupported( ) {} ;

    void slotObjectsGrouped(IdList _lst);

  public :

    /// Constructor
    DataControlPlugin();

    /// Destructor
    ~DataControlPlugin() {};

    /// Name of the Plugin
    QString name(){ return (QString("DataControl")); };

    /// Description of the Plugin
    QString description() { return (QString(tr("Manages Data Objects"))); };

  private :
    /// Widget for Toolbox
    DatacontrolToolboxWidget* tool_;
    QIcon* toolIcon_;

    /// Layout for Toolbox
    QGridLayout* MeshDialogLayout_;

    /// Table containing Mesh information and status
    QTableWidget* objectList_;

    /// Flag set to true if meshlist is updated, preventing signal for list udate to be called
    bool locked;

    /// The Treemodel organizing the data
    TreeModel* model_;

    ///Tree view
    QTreeView* view_;

    /// Pointer to the header to the view widget
    QHeaderView * viewHeader_;

    /// Default marker to visualize "source" and "target" object flags
    DefaultObjectMarker objectMarker;


//===========================================================================
/** @name Slots which are called if data changed
* @{ */
//===========================================================================

  private slots:

    /// Gets called when the data in the table has changed
    void slotDataChanged (int _id, int _column, const QVariant& _value);

    /// Gets called when an object was moved via drag n drop
    void slotMoveBaseObject (int _id, int _newParentId);

  private:
    void propagateDownwards(BaseObject* _obj, int _column );
    void propagateUpwards(BaseObject* _obj, int _column );

    //variables to restrict propagation direction
    int onlyDown_;
    int onlyUp_;
    //defines, which selection column needs an update if not -1
    // 2: source 3: target
    int columnFromGUI_;

/** @} */

//===========================================================================
/** @name Popup Menu related
* @{ */
//===========================================================================
      private slots:
        /// Display a custom context window for the TreeView
        void slotCustomContextMenuRequested ( const QPoint & _pos );

        /// Called by the popup menu to remove an object/group
        void slotPopupRemove( );

        /// Called by the popup menu to ungroup items
        void slotUngroup ( );

        /// Called by the popup menu to group items
        void slotGroup ( );

        /// Called by the popup menu to copy items
        void slotCopy();

        ///Called by the popup menu to rename items
        void slotRename ( );

        ///Called by the popup menu to zoom to an object
        void slotZoomTo();
        
        /// Called by the popup menu in order to turn lights on/off
        void slotSwitchLight();
        
        /// Called by the popup menu in order to edit lights
        void slotEditLight();

        ///Called by the popup menu to set material properties
        void slotMaterialProperties();

        ///Called by the popup menu to copy material properties
        void slotCopyMaterialToTargeted();

        ///Called by the popup menu to copy material properties to clipboard
        void slotCopyMaterialToClipboard();

        ///Called by the popup menu to paste material properties from clipboard
        void slotPasteMaterialFromClipboard();

        /** \brief Called when the material properties were changed inside the material dialog
         *
         * @param _node unused
         */
        void slotNodeChanged( ACG::SceneGraph::BaseNode* _node );
        
        /// Hide/Show all light sources if checkbox has been checked
        void slotShowLightSources( bool _state );
        void slotShowLightSources();

      private :
        /// Index where a popup has been opened
        QModelIndex popupIndex_;

      private slots:

        /// Display a custom context window for the TreeViews header
        void slotHeaderCustomContextMenuRequested ( const QPoint & _pos );

      private:
        /// Defines the section of the Popup
        int headerPopupType_;

/** @} */

//===========================================================================
/** @name GL Area Context Menu related slots
* @{ */
//===========================================================================

      private slots:

        /// Hide an object
        void slotContextMenuHide( );

        /// Target Selection
        void slotContextMenuTarget( );

        /// Source Selection
        void slotContextMenuSource( );
        
        /// Remove Selected Item
        void slotContextMenuRemove( );

        /// Update the context Menus for the given id
        void slotUpdateContextMenu( int _objectId);

      private :
        QAction* targetAction_;
        QAction* sourceAction_;
        QAction* removeAction_;

        QToolButton *advancedSettingsBtn_;

/** @} */

//===========================================================================
/** @name Bounding box related slots/functions
* @{ */
//===========================================================================

      private slots:

        /// Bounding box selection changed
        void slotBoundingBoxChange( );

     private:

        /// Updates bounding box
        void updateBoundingBox(BaseObjectData* _obj);

/** @} */

//===========================================================================
/** @name Scripting functions
* @{ */
//===========================================================================

      private slots:

        /** This function sets the slot descriptions for this plugin
         *  which will be visible in the script editor
         */
        void setDescriptions();

      public slots:
        /// Get the Objects name from the id
        QString getObjectName( int objectId );

        /// Delete the given object
        void objectDelete(int objectId);

        /// Creates an object copy
        int copyObject( int objectId );

        /// Get the id of an object by its name
        int getObject( QString _name );

        /// Get the DataType of a given object
        DataType dataType( int objectId );

        /// Hide the given Object
        void hideObject( int objectId );

        /// Show the given Object
        void showObject( int objectId );

        /// set the given Object as target
        void setTarget( int objectId, bool _target );

        /// set the given Object as source
        void setSource( int objectId, bool _source );

        /// set the name of the given object
        void setObjectName( int objectId, QString _name );

        /// Makes all available objects target
        void setAllTarget();

        /// Makes all available objects source
        void setAllSource();

        /// Unselect all objects
        void clearAllTarget();

        /// remove source selection from all objects
        void clearAllSource();

        /// Shows all objects
        void showAll();

        /// Hides all objects
        void hideAll();

		/** Create and add an empty group.
		 *
		 * @param _groupName Name of the new group. If it is an empty string,
		 *        a default name based on group id will be generated.
		 * @param _parentGroupId Parent object under which the new group is
		 *        created. If _parentGroupId is zero, the corresponding object
		 *        does not exist or it is not a group, the new group is created
		 *        as toplevel.
		 */
		int addEmptyGroup(QString _groupName = "", int _parentGroupId = 0);

        /** Group objects together
        *
        * @return object id of new group or -1 if failed
        */
        int groupObjects(IdList _objectIDs, QString _groupName = "");
        
        /// add an object to an existing group
        bool addObjectToGroup(int _objectId, int _groupId);

        /** remove an object from its group and place it at the toplevel group
        *
        * @return successfull
        */
        bool unGroupObject(int _id);
        
        /// Get all target objects of given type
        IdList getTargetObjects(DataType _type);
        
        /// Get all source objects of given type
        IdList getSourceObjects(DataType _type);

        /// Returns the number of groups
        unsigned int groupCount() const;

        /// Returns a list of all available DataTypes
        QStringList availableDataTypeNames() const;

        /// Print information about all open objects to the console
        void printObjectInfoToLog();

        /// Get all elements of the given group
        IdList getGroupElements(int _groupId);

        void showReducedUi(bool reduced);

/** @} */

  public slots:
    QString version() { return QString("1.03"); };
};

#endif //DATACONTROLPLUGIN_HH
