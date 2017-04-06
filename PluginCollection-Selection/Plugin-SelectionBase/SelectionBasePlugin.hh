/*===========================================================================*\
 *                                                                           *
 *                              OpenFlipper                                  *
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
 *                                                                           *
 *   $Revision$                                                       *
 *   $Author$                                                       *
 *   $Date$                   *
 *                                                                           *
\*===========================================================================*/

#ifndef SELECTIONBASEPLUGIN_HH
#define SELECTIONBASEPLUGIN_HH

#include <QObject>
#include <QGroupBox>
#include <QMenuBar>

#include <set>

#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/MouseInterface.hh>
#include <OpenFlipper/BasePlugin/KeyInterface.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>
#include <OpenFlipper/BasePlugin/PickingInterface.hh>
#include <OpenFlipper/BasePlugin/ToolboxInterface.hh>
#include <OpenFlipper/BasePlugin/ToolbarInterface.hh>
#include <OpenFlipper/BasePlugin/LoadSaveInterface.hh>
#include <OpenFlipper/BasePlugin/INIInterface.hh>
#include <OpenFlipper/BasePlugin/ScriptInterface.hh>
#include <OpenFlipper/BasePlugin/ContextMenuInterface.hh>
#include <OpenFlipper/BasePlugin/SelectionInterface.hh>

#include <ACG/Scenegraph/GlutPrimitiveNode.hh>
#include <ACG/Scenegraph/LineNode.hh>

#include <OpenFlipper/common/Types.hh>
#include <OpenFlipper/BasePlugin/PluginFunctions.hh>

#include "widgets/SelectionBaseToolbox.hh"
#include "widgets/SelectionTypeFrame.hh"

#include <OpenFlipper/INIFile/INIFile.hh>

#include "HelperClasses.hh"

class SelectionBasePlugin : public QObject, BaseInterface , MouseInterface, KeyInterface, PickingInterface, ToolboxInterface, INIInterface,
    LoggingInterface, ScriptInterface, ContextMenuInterface, ToolbarInterface, LoadSaveInterface, SelectionInterface {
Q_OBJECT
Q_INTERFACES(BaseInterface)
Q_INTERFACES(MouseInterface)
Q_INTERFACES(KeyInterface)
Q_INTERFACES(PickingInterface)
Q_INTERFACES(ToolboxInterface)
Q_INTERFACES(INIInterface)
Q_INTERFACES(LoggingInterface)
Q_INTERFACES(ScriptInterface)
Q_INTERFACES(ContextMenuInterface)
Q_INTERFACES(ToolbarInterface)
Q_INTERFACES(LoadSaveInterface)
Q_INTERFACES(SelectionInterface)

#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "org.OpenFlipper.Plugins.Plugin-SelectionBase")
#endif

    public:

        /// Default constructor
        SelectionBasePlugin();

        // BaseInterface
        QString name() { return (QString(tr("SelectionBase"))); };
        QString description( ) { return (QString(tr("Provides basic selection functions"))); };

    signals:

        // BaseInterface
        void updateView();

        // LoggingInterface
        void log(Logtype _type, QString _message);
        void log(QString _message);

        // ToolbarInterface
        void addToolbar(QToolBar* _toolbar);

        // SelectionInterface
        void toggleSelection(QMouseEvent* _event, SelectionInterface::PrimitiveType _currentType, bool _deselect);
        void lassoSelection(QMouseEvent* _event, SelectionInterface::PrimitiveType _currentType, bool _deselect);
        void volumeLassoSelection(QMouseEvent* _event, SelectionInterface::PrimitiveType _currentType, bool _deselect);
        void surfaceLassoSelection(QMouseEvent* _event, SelectionInterface::PrimitiveType _currentType, bool _deselect);
        void sphereSelection(QMouseEvent* _event, double _radius, SelectionInterface::PrimitiveType _currentType, bool _deselect);
        void closestBoundarySelection(QMouseEvent* _event, SelectionInterface::PrimitiveType _currentType, bool _deselect);
        void floodFillSelection(QMouseEvent* _event, double _maxAngle, SelectionInterface::PrimitiveType _currentType, bool _deselect);
        void componentsSelection(QMouseEvent* _event, SelectionInterface::PrimitiveType _currentType, bool _deselect);
        void customSelection(QMouseEvent* _event, SelectionInterface::PrimitiveType _currentType, QString _customIdentifier, bool _deselect);

        void selectionOperation(QString _operation);
        void loadSelection(const INIFile& _file);
        void saveSelection(INIFile& _file);

        void keyShortcutEvent(int _key, Qt::KeyboardModifiers _modifiers = Qt::NoModifier);

        // PickingInterface
        void addPickMode(const std::string& _mode);
        void setPickModeMouseTracking(const std::string& _mode, bool _mouseTracking);
        void setPickModeCursor(const std::string& _mode, QCursor _cursor);
        void setPickModeToolbar (const std::string& _mode, QToolBar * _toolbar);

        // KeyInterface
        void registerKey(int _key, Qt::KeyboardModifiers _modifiers, QString _description, bool _multiUse = false);

        // ToolboxInterface
        void addToolbox(QString _name, QWidget* _widget, QIcon* _icon);

    private slots:

        // BaseInterface
        void initializePlugin();
        void pluginsInitialized();

        //MouseInterface
        void slotMouseWheelEvent(QWheelEvent* _event, const std::string& _mode);
        void slotMouseEvent(QMouseEvent* _event);

        // SelectionInterface
        void slotAddSelectionEnvironment(QString _modeName, QString _description, QString _icon, QString& _handleName);
        void slotRegisterType(QString _handleName, DataType _type);
        void slotAddPrimitiveType(QString _handleName, QString _name, QString _icon, SelectionInterface::PrimitiveType& _typeHandle);
        void slotAddCustomSelectionMode(QString _handleName, QString _modeName, QString _description, QString _icon,
                                        SelectionInterface::PrimitiveType _associatedTypes, QString& _customIdentifier);
        void slotAddCustomSelectionMode(QString _handleName, QString _modeName, QString _description, QString _icon,
                                        SelectionInterface::PrimitiveType _associatedTypes, QString& _customIdentifier,
                                        DataType _objectTypeRestriction);

        void slotAddSelectionOperations(QString _handleName, QStringList _operationsList, QString _category, SelectionInterface::PrimitiveType _type = 0u);
        void slotAddSelectionParameters(QString _handleName, QWidget* _widget, QString _category, SelectionInterface::PrimitiveType _type = 0u);

        void slotShowToggleSelectionMode(QString _handleName, bool _show, SelectionInterface::PrimitiveType _associatedTypes);
        void slotShowLassoSelectionMode(QString _handleName, bool _show, SelectionInterface::PrimitiveType _associatedTypes);
        void slotShowVolumeLassoSelectionMode(QString _handleName, bool _show, SelectionInterface::PrimitiveType _associatedTypes);
        void slotShowSurfaceLassoSelectionMode(QString _handleName, bool _show, SelectionInterface::PrimitiveType _associatedTypes);
        void slotShowSphereSelectionMode(QString _handleName, bool _show, SelectionInterface::PrimitiveType _associatedTypes);
        void slotShowClosestBoundarySelectionMode(QString _handleName, bool _show, SelectionInterface::PrimitiveType _associatedTypes);
        void slotShowFloodFillSelectionMode(QString _handleName, bool _show, SelectionInterface::PrimitiveType _associatedTypes);
        void slotShowComponentsSelectionMode(QString _handleName, bool _show, SelectionInterface::PrimitiveType _associatedTypes);

        void slotGetActiveDataTypes(SelectionInterface::TypeList& _types);
        void slotGetActivePrimitiveType(SelectionInterface::PrimitiveType& _type);

        void slotTargetObjectsOnly(bool& _targetsOnly);
        void slotRegisterKeyShortcut(int _key, Qt::KeyboardModifiers _modifiers);

        // LoadSaveInterface
        void addedEmptyObject (int _id);
        void objectDeleted (int _id);

        // PickingInterface
        void slotPickModeChanged (const std::string& _pickmode);

        // KeyInterface
        void slotKeyEvent(QKeyEvent* _event);
        void slotKeyReleaseEvent(QKeyEvent* _event);

        // ============== Selection Actions ===============

        void slotMouseToggleSelection(QMouseEvent* _event);
        void slotMouseLassoSelection(QMouseEvent* _event);
        void slotMouseVolumeLassoSelection(QMouseEvent* _event);
        void slotMouseSurfaceLassoSelection(QMouseEvent* _event);
        void slotMouseSphereSelection(QMouseEvent* _event);
        void slotMouseBoundarySelection(QMouseEvent* _event);
        void slotMouseFloodFillSelection(QMouseEvent* _event);
        void slotMouseComponentsSelection(QMouseEvent* _event);
        void slotMouseCustomSelection(QMouseEvent* _event);

        void slotLoadSelectionButton();
        void slotSaveSelectionButton();

        void slotOperationRequested();
        void slotShowAndHideOperations();
        void slotShowAndHideParameters();

        // ==================== GUI =======================

        void slotSelectionEnvironmentRequested(bool _checked);

        void slotEnterSelectionMode(bool _checked);

        void updateActivePrimitiveTypes(bool _checked);

    public slots:
        QString version() { return QString("1.0"); };

        /** \brief Enables setting the selection metaphor via scripting
          * The default selection metaphors are:
          * - "sb_toggle"
          * - "sb_lasso"
          * - "sb_volumelasso"
          * - "sb_surfacelasso"
          * - "sb_sphere"
          * - "sb_closestboundary"
          * - "sb_floodfill"
          * - "sb_components"
          *
          * Note that furhter custom methaphors can be defined and used
          */
        void setSelectionMetaphor(QString _metaphor);

        /** \brief Enables setting the selection primitive via scripting
          * The default primitives for OpenMesh are:
          * - "Select Vertices"
          * - "Select Halfedges"
          * - "Select Edges"
          * - "Select Faces"
          * The default primitives for OpenVolumeMesh are:
          * - "Select Volume Mesh Vertices"
          * - "Select Volume Mesh Edges"
          * - "Select Volume Mesh Faces"
          * - "Select Volume Mesh Cells"
          * - "Column Selection"
          * - "Sheet Selection"
          *
          * Note that further custom primitive types can be defined and used
          */
        void setSelectionPrimitiveType(QString _primitive);

    private:

        struct SelectionEnvironment {
            // Name of the environment
            QString name;
            // List of supported types
            std::vector<DataType> types;
            // List of all default selection modes
            std::set<HandleAction*> defaultSelectionModes;
            // List of all default selection modes
            std::set<HandleAction*> customSelectionModes;
            // Selection environments handle
            QString handle;
            // Handle to tab widget's primitive's bar
            QHBoxLayout* primitivesBar;
            // Handle to tab widget's operations widget
            QVBoxLayout* operationsBar;
            // Keep track of operation categories
            std::map<QString,std::pair<FillingLayout*,QGroupBox*> > categories;
            // Action group for primitives
            QActionGroup* primitiveActions;
            // Store the supported primitive types (bitmask)
            unsigned int primitiveTypes;
            // Map that stores for each primitive type a set of
            // operations that are supported on it
            std::multimap<PrimitiveType, QPushButton*> operations;
            // Map that stores for each primitive type a widget
            // that provides parameters
            std::multimap<PrimitiveType, QWidget*> parameters;
            // Keep name of tab for selection environment
            QWidget* tabWidget;
        };

        /// Get a unique pickmode name
        QString getUniqueIdentifierName(QString _name, int _num = 0);

        /// Get a unique handle name
        QString getUniqueHandleName(QString _name, int _num = 0);

        /// Get a selectionEnvironment by a given name
        bool getSelectionEnvironment(SelectionEnvironment*& env, const QString& _handleName);

        /// Test if at least one object of type _type is in the scene graph
        bool typeExists(DataType _type, int _excludeId = -1);

        /// Caches the number of available elements of a certain data type for the typeExists function
        QMap<DataType,int> typeCounter_;

        /// Show standard selection mode
        /// Use this method to show/hide standard selection actions (lasso, floodfill, sphere, etc.)
        void showSelectionMode(QString _mode, QString _icon, QString _desc,
                               QString _handleName, bool _show, SelectionInterface::PrimitiveType _associatedTypes,
                               QString& _customIdentifier, bool _custom = false, DataType _objectTypeRestriction = DATA_ALL);

        /// helper function for showSelectionMode
        void selectionModeShowSwitch(bool _show, SelectionEnvironment*& env, HandleAction* toggleSelectionAction_,
                                                          SelectionInterface::PrimitiveType& _associatedTypes);

        /// helper function to find a baseObjectData and selection environment given a specific id
        bool findObjectType(BaseObjectData*& obj, bool& found, SelectionEnvironment*& env, int _id);

        /// Create new type frame for tabs widget
        SelectionTypeFrameWidget* createNewTypeFrame(SelectionEnvironment& _env);

        /// Update the pickmode toolbar
        void updatePickModeToolBar();

        // Update the tabs widgets' order
        void updateTabsOrder();

        // Map of registered selection environments (handle, environment)
        std::map<QString,SelectionEnvironment> selectionEnvironments_;

        //================== GUI ======================

        // List of primitive type buttons (primitivetype, button)
        std::map<PrimitiveType,QAction*> primitiveTypeButtons_;

        // Toolbar in OpenFlipper that contains the selection button
        QToolBar* toolBar_;

        // Widget for Toolbox
        SelectionBaseToolboxWidget* tool_;

        // Toolbar that displays ALL primitive types of all selection environments
        QActionGroup* primitivesBarGroup_;
        QToolBar* pickModeToolBar_;

        QActionGroup* selectionModesGroup_;

        // Actions for default selection modes
        HandleAction* toggleSelectionAction_;
        HandleAction* lassoSelectionAction_;
        HandleAction* volumeLassoSelectionAction_;
        HandleAction* surfaceLassoSelectionAction_;
        HandleAction* sphereSelectionAction_;
        HandleAction* boundarySelectionAction_;
        HandleAction* floodFillSelectionAction_;
        HandleAction* componentsSelectionAction_;

        std::set<std::pair<int,Qt::KeyboardModifiers> > registeredKeys_;

        //================== Status ===================

        // Keep the current pick mode
        QString currentPickMode_;

        // Keep the last used pick mode
        QString lastPickMode_;

        // Keep the current selection mode
        QString currentSelectionMode_;

        // Bitmask for currently selected primitive types
        SelectionInterface::PrimitiveType currentPrimitiveType_;

        // Next free primitive handle
        unsigned int nextFreePrimitiveType_;

        // Indicates whether we want to deselect primitives
        bool deselection_;

        //=============== Rendering ====================

        // Sphere Nodes material
        ACG::SceneGraph::MaterialNode* sphere_mat_node_;

        // Sphere Node
        ACG::SceneGraph::GlutPrimitiveNode* sphere_node_;

        // Sphere radius
        double sphere_radius_;

        // Line Node
        ACG::SceneGraph::LineNode* line_node_;

        // Indicates if lasso selection is currently active
        bool lassoSelection_;

        // List of points for line node
        std::vector<ACG::Vec3d> linePoints_;

        QIcon* toolIcon_;

        // Keep track of all data types in the scene
        DataType availableObjectTypes_;
};

#endif // SELECTIONBASEPLUGIN_HH
