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
 *   $Revision: 11942 $                                                       *
 *   $Author: kremer $                                                       *
 *   $Date: 2011-07-06 15:10:17 +0200 (Wed, 06 Jul 2011) $                   *
 *                                                                           *
\*===========================================================================*/

#ifndef VOLUMEMESHSELECTIONPLUGIN_HH
#define VOLUMEMESHSELECTIONPLUGIN_HH

#include <QObject>

#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/KeyInterface.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>
#include <OpenFlipper/BasePlugin/ToolboxInterface.hh>
#include <OpenFlipper/BasePlugin/ToolbarInterface.hh>
#include <OpenFlipper/BasePlugin/LoadSaveInterface.hh>
#include <OpenFlipper/BasePlugin/BackupInterface.hh>
#include <OpenFlipper/BasePlugin/INIInterface.hh>
#include <OpenFlipper/BasePlugin/ScriptInterface.hh>
#include <OpenFlipper/BasePlugin/SelectionInterface.hh>
#include <OpenFlipper/common/Types.hh>
#include <OpenFlipper/BasePlugin/PluginFunctions.hh>
#include <OpenFlipper/INIFile/INIFile.hh>

#include <ObjectTypes/PolyhedralMesh/PolyhedralMesh.hh>
#include <ObjectTypes/HexahedralMesh/HexahedralMesh.hh>
#ifdef ENABLE_TETRAHEDRALMESH_SUPPORT
#include <ObjectTypes/TetrahedralMesh/TetrahedralMesh.hh>
#endif

class ParameterWidget;

class VolumeMeshSelectionPlugin : public QObject, BaseInterface, KeyInterface,
            INIInterface, BackupInterface, LoggingInterface, ScriptInterface, SelectionInterface
{
    Q_OBJECT
    Q_INTERFACES(BaseInterface)
    Q_INTERFACES(KeyInterface)
    Q_INTERFACES(INIInterface)
    Q_INTERFACES(BackupInterface)
    Q_INTERFACES(LoggingInterface)
    Q_INTERFACES(ScriptInterface)    
    Q_INTERFACES(SelectionInterface)

#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "org.OpenFlipper.Plugins.Plugin-SelectionVolumeMesh")
#endif

public:
    
    /// Default constructor
    VolumeMeshSelectionPlugin();

    /// Default destructor
    ~VolumeMeshSelectionPlugin();
    
    friend class SelectVolumeAction;

    /// set max angle for flood fill selection
    void   set_max_angle(const double _a);
    /// get max angle for flood fill selection
    double get_max_angle();

signals:

    // BaseInterface
    void updateView();
    void updatedObject(int, const UpdateType&);
    void nodeVisibilityChanged(int _identifier);
    void setSlotDescription(QString     _slotName,   QString     _slotDescription,
                            QStringList _parameters, QStringList _descriptions);

    // LoggingInterface
    void log(Logtype _type, QString _message);
    void log(QString _message);

    // ScriptInterface
    void scriptInfo(QString _functionName);

    // SelectionInterface
    void addSelectionEnvironment(QString _modeName, QString _description, QString _icon, QString& _handleName);
    void registerType(QString _handleName, DataType _type);
    void addPrimitiveType(QString _handleName, QString _name, QString _icon, SelectionInterface::PrimitiveType& _typeHandle);
    void addSelectionOperations(QString _handleName, QStringList _operationsList, QString _category, SelectionInterface::PrimitiveType _type = 0u);
    void addSelectionParameters(QString _handleName, QWidget* _widget, QString _category, SelectionInterface::PrimitiveType _type = 0u);

    void showToggleSelectionMode(QString _handleName, bool _show, SelectionInterface::PrimitiveType _associatedTypes);

    void showVolumeLassoSelectionMode(QString _handleName, bool _show, SelectionInterface::PrimitiveType _associatedTypes);
    void showFloodFillSelectionMode(QString _handleName, bool _show, SelectionInterface::PrimitiveType _associatedTypes);
    void addCustomSelectionMode(QString _handleName, QString _modeName, QString _description, QString _icon,
           SelectionInterface::PrimitiveType _associatedTypes, QString& _customIdentifier);

    void getActiveDataTypes(SelectionInterface::TypeList& _types);
    void getActivePrimitiveType(SelectionInterface::PrimitiveType& _type);
    void targetObjectsOnly(bool& _targetsOnly);
    
    void registerKeyShortcut(int _key, Qt::KeyboardModifiers _modifiers = Qt::NoModifier);

private slots:

    // INIInterface
    void loadIniFile(INIFile& _ini, int _id);
    void saveIniFile(INIFile& _ini, int _id);

    // BaseInterface
    void initializePlugin();
    void pluginsInitialized();
    void noguiSupported() {};

    // SelectionInterface
    void slotSelectionOperation(QString _operation);
    void slotToggleSelection(QMouseEvent* _event, SelectionInterface::PrimitiveType _currentType, bool _deselect);

    void slotVolumeLassoSelection(QMouseEvent* _event, SelectionInterface::PrimitiveType _currentType, bool _deselect);
    void slotFloodFillSelection(QMouseEvent* _event, SelectionInterface::PrimitiveType _currentType, bool _deselect);
    void slotCustomSelection(QMouseEvent *_event, SelectionInterface::PrimitiveType _currentType, QString _customIdentifier, bool _deselect);

    void slotLoadSelection(const INIFile& _file);
    void slotSaveSelection(INIFile& _file);
    
    void slotKeyShortcutEvent(int _key, Qt::KeyboardModifiers _modifiers);

public slots:

    // SelectionInterface
    void loadSelection(int _objId, const QString& _filename);

public:

    // BaseInterface
    QString name() {
        return (QString(tr("VolumeMesh Selection")));
    };
    
    QString description() {
        return (QString(tr("Allows to select VolumeMeshes")));
    };

    //===========================================================================
    /** @name Private methods
    * @{ */
    //===========================================================================
private:
    
    /// Set slot descriptions for scripting functions
    void updateSlotDescriptions();
    
    /** @} */
    
public slots:

    QString version() {
        return QString("1.0");
    };

    //===========================================================================
    /** @name Scriptable slots
    * @{ */
    //===========================================================================

    /// Is vertex type active? (for use in plugins that need mesh selection)
    bool vertexTypeActive();

    /// Is vertex type active? (for use in plugins that need mesh selection)
    bool edgeTypeActive();

    /// Is face type active? (for use in plugins that need mesh selection)
    bool faceTypeActive();

    /// Is cell type active? (for use in plugins that need mesh selection)
    bool cellTypeActive();

    //===========================================================================
    
    /// Select all vertices of a volume mesh
    void selectAllVertices(int _objectId);

    /// Deselect all vertices of a volume mesh
    void deselectAllVertices(int _objectId);

    /// Invert vertex selection
    void invertVertexSelection(int _objectId);

    /// Select specific vertices of a volume mesh
    void selectVertices(int _objectId, const IdList& _ids, bool _deselect = false);

    /// Get current vertex selection
    IdList getVertexSelection(int _objectId);

    /// Delete selected vertices from mesh
    void deleteSelectedVertices(int _objectId, bool _preserveManifoldness = true);

    //===========================================================================

    /// Select all edges of a volume mesh
    void selectAllEdges(int _objectId);

    /// Deselect all edges of a volume mesh
    void deselectAllEdges(int _objectId);

    /// Invert edge selection
    void invertEdgeSelection(int _objectId);

    /// Select specific edges of a volume mesh
    void selectEdges(int _objectId, const IdList& _ids, bool _deselect = false);

    /// Get current edge selection
    IdList getEdgeSelection(int _objectId);

    /// Delete selected edges from mesh
    void deleteSelectedEdges(int _objectId, bool _preserveManifoldness = true);

    //===========================================================================

    /// Select all half-edges of a volume mesh
    void selectAllHalfEdges(int _objectId);

    /// Deselect all half-edges of a volume mesh
    void deselectAllHalfEdges(int _objectId);

    /// Invert half-edge selection
    void invertHalfEdgeSelection(int _objectId);

    /// Select specific half-edges of a volume mesh
    void selectHalfEdges(int _objectId, const IdList& _ids, bool _deselect = false);

    /// Get current half-edge selection
    IdList getHalfEdgeSelection(int _objectId);

    //===========================================================================

    /// Select all faces of a volume mesh
    void selectAllFaces(int _objectId);

    /// Deselect all faces of a volume mesh
    void deselectAllFaces(int _objectId);

    /// Invert face selection
    void invertFaceSelection(int _objectId);

    /// Select specific faces of a volume mesh
    void selectFaces(int _objectId, const IdList& _ids, bool _deselect = false);

    /// Get current face selection
    IdList getFaceSelection(int _objectId);

    /// Delete selected faces from mesh
    void deleteSelectedFaces(int _objectId, bool _preserveManifoldness = true);

    //===========================================================================

    /// Select all half-faces of a volume mesh
    void selectAllHalfFaces(int _objectId);

    /// Deselect all half-faces of a volume mesh
    void deselectAllHalfFaces(int _objectId);

    /// Invert half-face selection
    void invertHalfFaceSelection(int _objectId);

    /// Select specific half-faces of a volume mesh
    void selectHalfFaces(int _objectId, const IdList& _ids, bool _deselect = false);

    /// Get current half-face selection
    IdList getHalfFaceSelection(int _objectId);

    //===========================================================================

    /// Select all cells of a volume mesh
    void selectAllCells(int _objectId);

    /// Deselect all cells of a volume mesh
    void deselectAllCells(int _objectId);

    /// Invert cell selection
    void invertCellSelection(int _objectId);

    /// Select specific cells of a volume mesh
    void selectCells(int _objectId, const IdList& _ids, bool _deselect = false);

    /// Get current cell selection
    IdList getCellSelection(int _objectId);

    /// Delete selected cells from mesh
    void deleteSelectedCells(int _objectId, bool _preserveManifoldness = true);

    /** @} */

    //===========================================================================
    /** @name Member variables
    * @{ */
    //===========================================================================

private:
    /// Surface volume selection tool
    template<class MeshT>
    bool volumeSelection(MeshT* _mesh, ACG::GLState& _state, QRegion *_region,
                         PrimitiveType _primitiveTypes, bool _deselection);

    /// Select all entities that are connected (and do not exceed the maximum dihedral angle)
    template<class MeshT>
    void floodFillSelection(MeshT* _mesh, uint _fh, double _maxAngle,
                            PrimitiveType _primitiveTypes, bool _deselection);

    // Get orthogonal orientation of neighboring cell
    unsigned char getOrthogonalOrientationOfNeighborCell(const OpenVolumeMesh::CellHandle& _ch1,
            const OpenVolumeMesh::CellHandle& _ch2, unsigned char _firstOrthDirection, const HexahedralMesh* _mesh) const;

    // Get face shared by two adjacent cells
    typedef std::pair<OpenVolumeMesh::HalfFaceHandle,OpenVolumeMesh::HalfFaceHandle> HFPair;
    HFPair getCommonFace(const OpenVolumeMesh::CellHandle& _ch1,
            const OpenVolumeMesh::CellHandle& _ch2, const HexahedralMesh* _mesh) const;


    OpenVolumeMesh::StatusAttrib* getStatus(int _objectId);
    OpenVolumeMesh::StatusAttrib* getStatus(BaseObjectData* object);

    /// Handle to selection environment
    QString environmentHandle_;

    QString columnSelectionHandle_;
    QString sheetSelectionHandle_;

    /// Primitive type handles:
    PrimitiveType vertexType_;
    PrimitiveType edgeType_;
    PrimitiveType faceType_;
    PrimitiveType cellType_;
    
    PrimitiveType allSupportedTypes_;
    PrimitiveType floodFillSupportedTypes_;

    ParameterWidget* parameterWidget_;

    double max_angle_;
    
    /// Keep volume lasso points
    QVector<QPoint> volumeLassoPoints_;

    // Store last picked cell for sheet selection
    OpenVolumeMesh::CellHandle lastPickedCell_;
    unsigned char lastPickedOrientation_;
    std::map<OpenVolumeMesh::CellHandle, unsigned char> orientationMap_;

    /** @} */

};

/// Traverse the scenegraph and call the selection function for all mesh nodes
class SelectVolumeAction
{
public:
    SelectVolumeAction(QRegion&_region, VolumeMeshSelectionPlugin* _plugin,
            unsigned int _type, bool _deselection, ACG::GLState& _state)
        : state_(_state)
        , region_(_region)
        , plugin_(_plugin)
        , type_(_type)
        , deselection_(_deselection)
    {
    }

    void enter(BaseNode* /*_node*/) {}
    void leave(BaseNode* /*_node*/) {}

    bool operator()(BaseNode* _node);

private:
    ACG::GLState&               state_;
    QRegion&                    region_;
    VolumeMeshSelectionPlugin*  plugin_;
    unsigned int                type_;
    bool                        deselection_;
};
//=============================================================================
#if defined(INCLUDE_TEMPLATES) && !defined(VOLUMEMESHSELECTIONPLUGINT_CC)
#define VOLUMEMESHSELECTIONPLUGINT_TEMPLATES
#include "VolumeMeshSelectionPluginT.cc"
#endif

//=============================================================================
#endif // VOLUMEMESHSELECTIONPLUGIN_HH defined
//=============================================================================
