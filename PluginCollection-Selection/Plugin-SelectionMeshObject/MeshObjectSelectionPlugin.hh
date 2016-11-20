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

#ifndef MESHOBJECTSELECTIONPLUGIN_HH
#define MESHOBJECTSELECTIONPLUGIN_HH

#include <QObject>

#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/KeyInterface.hh>
#include <OpenFlipper/BasePlugin/MouseInterface.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>
#include <OpenFlipper/BasePlugin/ToolboxInterface.hh>
#include <OpenFlipper/BasePlugin/ToolbarInterface.hh>
#include <OpenFlipper/BasePlugin/LoadSaveInterface.hh>
#include <OpenFlipper/BasePlugin/BackupInterface.hh>
#include <OpenFlipper/BasePlugin/INIInterface.hh>
#include <OpenFlipper/BasePlugin/ScriptInterface.hh>
#include <OpenFlipper/BasePlugin/SelectionInterface.hh>
#include <OpenFlipper/BasePlugin/OptionsInterface.hh>

#include <OpenFlipper/common/Types.hh>
#include <OpenFlipper/BasePlugin/PluginFunctions.hh>
#include <OpenFlipper/INIFile/INIFile.hh>

#include <ObjectTypes/PolyMesh/PolyMesh.hh>
#include <ObjectTypes/TriangleMesh/TriangleMesh.hh>

#include <ACG/QtWidgets/QtColorChooserButton.hh>

#include "ConversionDialog.hh"

class MeshObjectSelectionPlugin : public QObject, BaseInterface, KeyInterface, LoadSaveInterface,
            INIInterface, BackupInterface, ScriptInterface, LoggingInterface, SelectionInterface,
            MouseInterface, OptionsInterface
{
    Q_OBJECT
    Q_INTERFACES(BaseInterface)
    Q_INTERFACES(KeyInterface)
    Q_INTERFACES(MouseInterface)
    Q_INTERFACES(INIInterface)
    Q_INTERFACES(BackupInterface)
    Q_INTERFACES(ScriptInterface)
    Q_INTERFACES(LoggingInterface)
    Q_INTERFACES(LoadSaveInterface)
    Q_INTERFACES(SelectionInterface)
    Q_INTERFACES(OptionsInterface)

#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "org.OpenFlipper.Plugins.Plugin-SelectionMeshObject")
#endif

public:
    /// Default constructor
    MeshObjectSelectionPlugin();

    /// Default destructor
    ~MeshObjectSelectionPlugin();
    
    friend class SelectVolumeAction;

signals:

    // BaseInterface
    void updateView();
    void updatedObject(int, const UpdateType&);
    void nodeVisibilityChanged(int _identifier);
    void setSlotDescription(QString     _slotName,   QString     _slotDescription,
                            QStringList _parameters, QStringList _descriptions);

    // BackupInterface
    void createBackup( int _objectid, QString _name, UpdateType _type = UPDATE_ALL);

    // LoggingInterface
    void log(Logtype _type, QString _message);
    void log(QString _message);

    // SelectionInterface
    void addSelectionEnvironment(QString _modeName, QString _description, QString _icon, QString& _handleName);
    void registerType(QString _handleName, DataType _type);
    void addPrimitiveType(QString _handleName, QString _name, QString _icon, SelectionInterface::PrimitiveType& _typeHandle);
    void addSelectionOperations(QString _handleName, QStringList _operationsList, QString _category, SelectionInterface::PrimitiveType _type = 0u);
    
    void showToggleSelectionMode(QString _handleName, bool _show, SelectionInterface::PrimitiveType _associatedTypes);

    void showLassoSelectionMode(QString _handleName, bool _show, SelectionInterface::PrimitiveType _associatedTypes);
    void showVolumeLassoSelectionMode(QString _handleName, bool _show, SelectionInterface::PrimitiveType _associatedTypes);
    void showSphereSelectionMode(QString _handleName, bool _show, SelectionInterface::PrimitiveType _associatedTypes);
    void showClosestBoundarySelectionMode(QString _handleName, bool _show, SelectionInterface::PrimitiveType _associatedTypes);
    void showFloodFillSelectionMode(QString _handleName, bool _show, SelectionInterface::PrimitiveType _associatedTypes);
    void showComponentsSelectionMode(QString _handleName, bool _show, SelectionInterface::PrimitiveType _associatedTypes);
    
    void getActiveDataTypes(SelectionInterface::TypeList& _types);
    void getActivePrimitiveType(SelectionInterface::PrimitiveType& _type);
    void targetObjectsOnly(bool& _targetsOnly);
    
    void registerKeyShortcut(int _key, Qt::KeyboardModifiers _modifiers = Qt::NoModifier);
    
    // LoadSaveInterface
    void deleteObject(int _objectId);
    void addEmptyObject( DataType _type, int& _id);
    
    // ScriptInterface
    void scriptInfo(QString _functionName);

public slots:

    // SelectionInterface
    void loadSelection(int _objId, const QString& _filename);

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

    void slotLassoSelection(QMouseEvent* _event, SelectionInterface::PrimitiveType _currentType, bool _deselect);
    void slotVolumeLassoSelection(QMouseEvent* _event, SelectionInterface::PrimitiveType _currentType, bool _deselect);
    void slotSphereSelection(QMouseEvent* _event, double _radius, SelectionInterface::PrimitiveType _currentType, bool _deselect);
    void slotClosestBoundarySelection(QMouseEvent* _event, SelectionInterface::PrimitiveType _currentType, bool _deselect);
    void slotFloodFillSelection(QMouseEvent* _event, double _maxAngle, SelectionInterface::PrimitiveType _currentType, bool _deselect);
    void slotComponentsSelection(QMouseEvent* _event, SelectionInterface::PrimitiveType _currentType, bool _deselect);

    void slotLoadSelection(const INIFile& _file);
    void slotSaveSelection(INIFile& _file);
    
    void slotKeyShortcutEvent(int _key, Qt::KeyboardModifiers _modifiers);

    // MouseInterface
    void slotMouseWheelEvent(QWheelEvent* event, std::string const& mode);

    // LoadSaveInterface
    void addedEmptyObject( int _id );

public:

    // OptionsInterface
    bool initializeOptionsWidget(QWidget*& _widget);

private slots:
    void applyOptions();

    // BaseInterface
    QString name() {
        return (QString(tr("Mesh Object Selection")));
    };
    
    QString description() {
        return (QString(tr("Allows to select parts of Mesh Objects")));
    };

    //===========================================================================
    /** @name Private methods
    * @{ */
    //===========================================================================
private:
    
    // update and saves new color values for area node/selecion noe etc.
    void updateColorValues();

    /// Set descriptions for local public slots
    void updateSlotDescriptions();
    
    /// Set color for selection
    void setColorForSelection(const int _objectId, const PrimitiveType _primitiveType);
    
    /** @} */
    
    //===========================================================================
    /** @name Private slots
    * @{ */
    //===========================================================================
private slots:
    
    /// Show selection conversion dialog
    void conversionRequested();

    /// sets the default color values for selection/handle/region/feature nodes for all objects of this type
    void setDefaultColorValues();

    /** @} */
    
public slots:

    QString version() {
        return QString("1.0");
    };

    // Is vertex type active? (for use in plugins that need mesh selection)
    bool vertexTypeActive() {
        SelectionInterface::PrimitiveType t = 0u;
        emit getActivePrimitiveType(t);
        return (t & vertexType_) > 0;
    }

    // Is vertex type active? (for use in plugins that need mesh selection)
    bool edgeTypeActive() {
        SelectionInterface::PrimitiveType t = 0u;
        emit getActivePrimitiveType(t);
        return (t & edgeType_) > 0;
    }

    // Is face type active? (for use in plugins that need mesh selection)
    bool faceTypeActive() {
        SelectionInterface::PrimitiveType t = 0u;
        emit getActivePrimitiveType(t);
        return (t & faceType_) > 0;
    }

    //===========================================================================
    /** @name Scriptable slots
    * @{ */
    //===========================================================================
public slots:

    //==========================================
    // VERTEX OPERATIONS
    //==========================================

    /// select given vertices
    void selectVertices(int objectId, IdList _vertexList);

    /// unselect given vertices
    void unselectVertices(int objectId, IdList _vertexList);

    /// Select all Vertices
    void selectAllVertices(int _objectId);

    /// Unselect all vertices
    void clearVertexSelection(int _objectId);

    /// Invert the current vertex selection
    void invertVertexSelection(int _objectId);

    /// Select all boundary vertices of the given object
    void selectBoundaryVertices(int _objectId);

    /// Select all vertices of the boundary close to the given vertex
    void selectClosestBoundaryVertices(int _objectId, int _vertexId);

    /// Shrink the current vertex selection
    void shrinkVertexSelection(int _objectId);

    /// Grow the current vertex selection
    void growVertexSelection(int _objectId);

    /// Return a list of all selected vertices
    IdList getVertexSelection(int _objectId);

    /// Delete vertices and faces that are currently selected
    void deleteVertexSelection(int _objectId);

    int createMeshFromVertexSelection( int _objectId);

    /// Colorize the vertex selection
    void colorizeVertexSelection(int _objectId, int _r, int _g, int _b, int a);

    //==========================================

    /// Set vertices to be part of the handle area
    void selectHandleVertices(int objectId, IdList _vertexList);

    /// Remove vertices from handle area
    void unselectHandleVertices(int objectId, IdList _vertexList);

    /// Clear handle Area
    void clearHandleVertices(int objectId);

    /// Set all vertices to be part of the handle area
    void setAllHandleVertices(int objectId);

    /// Get a list of all handle vertices
    IdList getHandleVertices(int objectId);

    /// Load a selection from an Flipper selection file for the given object
    void loadFlipperModelingSelection(int _objectId, QString _filename);

    /// Save a selection in Flipper Selection Format
    void saveFlipperModelingSelection(int _objectId, QString _filename);

    //==========================================

    /// Set vertices to be part of the modeling area
    void selectModelingVertices(int objectId, IdList _vertexList);

    /// Remove vertices from modeling area
    void unselectModelingVertices(int objectId, IdList _vertexList);

    /// Select vertices by their value
    void selectVerticesByValue(int _objectId, QString _component, bool _greater, double _value );

    /// Clear Modeling Area
    void clearModelingVertices(int objectId);

    /// Set all vertices to be part of the modeling area
    void setAllModelingVertices(int objectId);

    /// Get a list of all modeling vertices
    IdList getModelingVertices(int objectId);

    //==========================================
    // EDGE OPERATIONS
    //==========================================

    /// Select given Edges
    void selectEdges(int objectId, IdList _edgeList, const double _dihedral_angle_threshold = 0.0);

    /// Unselect given Edges
    void unselectEdges(int objectId, IdList _edgeList);

    /// Select all Edges
    void selectAllEdges(int objectId);

    /// Unselect all Edges
    void invertEdgeSelection(int objectId);

    /// Invert the current edge selection
    void clearEdgeSelection(int objectId);

    /// select boundary edges
    void selectBoundaryEdges(int objectId);

    /// Delete edges that are currently selected
    void deleteEdgeSelection(int _objectId);

    /// Return a list of all selected edges
    IdList getEdgeSelection(int objectId);

    /// Convert edge ids to vertex pairs
    IdList convertEdgesToVertexPairs(int _id, const IdList& _edges);

    /// Inverse of function above
    IdList convertVertexPairsToEdges(int _id, const IdList& _vertices);

    /// Create a mesh containing the face selection of the given mesh
    int createMeshFromEdgeSelection( int _objectId);

    /// Colorize the edge selection
    void colorizeEdgeSelection(int objectId, int r, int g, int b, int a);

    /// Trace Edge Path
    void traceEdgePath(int objectId, double threshold);

    //==========================================
    // HALFEDGE OPERATIONS
    //==========================================

    /// Select given Halfedges
    void selectHalfedges(int objectId, IdList _vertexList);

    /// Unselect given Halfedges
    void unselectHalfedges(int objectId, IdList _vertexList);

    /// Select all Halfedges
    void selectAllHalfedges(int objectId);

    /// Unselect all Halfedges
    void invertHalfedgeSelection(int objectId);

    /// Invert the current edge selection
    void clearHalfedgeSelection(int objectId);

    /// Select boundary edges
    void selectBoundaryHalfedges(int objectId);

    /// Return a list of all selected edges
    IdList getHalfedgeSelection(int objectId);

    /// Convert halfedge ids to vertex pairs
    IdList convertHalfedgesToVertexPairs(int _id, const IdList& _halfedges);

    /// Inverse of function above
    IdList convertVertexPairsToHalfedges(int _id, const IdList& _vertices);

    /// Colorize the edge selection
    void colorizeHalfedgeSelection(int objectId, int r, int g, int b, int a);

    //==========================================
    // FACE OPERATIONS
    //==========================================

    /// Select given faces
    void selectFaces(int objectId, IdList _facesList);

    /// Unselect given faces
    void unselectFaces(int objectId, IdList _facesList);

    /// Select all faces
    void selectAllFaces(int objectId);

    /// Unselect all faces
    void clearFaceSelection(int objectId);

    /// Invert the current face selection
    void invertFaceSelection(int objectId);

    /// Delete face that are currently selected
    void deleteFaceSelection(int _objectId);

    /// Select all boundary faces of the given object
    void selectBoundaryFaces(int objectId);

    /// Shrink the current face selection
    void shrinkFaceSelection(int objectId);

    /// Grow the current face selection
    void growFaceSelection(int objectId);

    /// Return a list of all selected faces
    IdList getFaceSelection(int objectId);

    /// Create a mesh containing the face selection of the given mesh
    int createMeshFromFaceSelection( int _objectId);

    /// Colorize the face selection
    void colorizeFaceSelection(int objectId, int r, int g, int b, int a);
    
    //===========================================================================

    /// Lasso selection tool
    void lassoSelect(QRegion& _region, PrimitiveType _primitiveType, bool _deselection);

    /// Convert the selection on one object
    void convertSelection(const int& _objectId ,const QString& _from, const QString& _to, bool _deselect);

    /// Convert the selection on all target objects
    void conversion(const QString& _from, const QString& _to, bool _deselect);


public:
    /// set dihedral angle threshold for edge selection
    void   set_dihedral_angle_threshold(const double _a);
    /// get dihedral angle threshold for edge selection
    double get_dihedral_angle_threshold();

private:
    void update_dihedral_angle_threshold_from_ui();

    /** @} */

    //===========================================================================
    /** @name Template Functions
    * @{ */
    //===========================================================================

private:

    /// Delete all selected elements of a mesh
    template<typename MeshT>
    bool deleteSelection(MeshT* _mesh, PrimitiveType _primitiveType);

    /// Update face selection to correspond to the vertex selection
    template<typename MeshT>
    void update_regions(MeshT* _mesh);

    /// Toggle mesh selection
    template<typename MeshT>
    void toggleMeshSelection(int _objectId, MeshT* _mesh, uint _fh, ACG::Vec3d& _hit_point, PrimitiveType _primitiveType);

    /// Use the event to paint selection with a sphere
    template<typename MeshT>
    void paintSphereSelection(MeshT* _mesh, int _objectId, int _target_idx,
            typename MeshT::Point _hitpoint, double _radius,
            PrimitiveType _primitiveTypes, bool _deselection);
    
    /// Surface volume selection tool
    template<class MeshT>
    bool volumeSelection(MeshT* _mesh, int _objectId, ACG::GLState& _state,
            QRegion *_region, PrimitiveType _primitiveTypes, bool _deselection);

    /// Select all entities that are incident to closest boundary
    template<class MeshT>
    void closestBoundarySelection(MeshT* _mesh, int _vh,
            PrimitiveType _primitiveTypes, bool _deselection);
    
    /// Select all entities that are connected (and do not exceed the maximum dihedral angle)
    template<class MeshT>
    void floodFillSelection(MeshT* _mesh, int _objectId, uint _fh,
            double _maxAngle, PrimitiveType _primitiveTypes, bool _deselection);

    /// Connected component mesh selection
    template<typename MeshT>
    void componentsMeshSelection(MeshT* _mesh, int _objectId, uint _fh,
            ACG::Vec3d& _hit_point, PrimitiveType _primitiveType);
                            
    /// Colorize the selection
    template<class MeshT>
    void colorizeSelection(MeshT* _mesh, PrimitiveType _primitiveTypes, int _red, int _green, int _blue, int _alpha);

    /// Create a new mesh from the selection
    template< class MeshT >
    void createMeshFromSelection( MeshT& _mesh, MeshT& _newMesh, PrimitiveType _primitiveType);

    /// Select vertices by value
    template< class MeshT >
    void selectVerticesByValue(MeshT* _mesh, QString _component, bool _greater, double _value);

    /** @} */

    //===========================================================================
    /** @name Private helper function
     * @{ */
    //===========================================================================
private:

    /// Create a mesh containing the selection of the given mesh
    int createMeshFromSelection( int _objectId , PrimitiveType _primitiveType);

/** @} */

    //===========================================================================
    /** @name Member variables
    * @{ */
    //===========================================================================

private:

    /// Handle to selection environment
    QString environmentHandle_;

    /// Primitive type handles:
    SelectionInterface::PrimitiveType vertexType_;
    SelectionInterface::PrimitiveType edgeType_;
    SelectionInterface::PrimitiveType halfedgeType_;
    SelectionInterface::PrimitiveType faceType_;
    
    SelectionInterface::PrimitiveType allSupportedTypes_;

    /// Used for lasso selection tool
    QPolygon lasso_2Dpoints_;
    
    /// Used for volume lasso tool
    QVector<QPoint> volumeLassoPoints_;

    ConversionDialog* conversionDialog_;

    /// Options
    QtColorChooserButton*       colorButtonSelection_;
    QtColorChooserButton*       colorButtonArea_;
    QtColorChooserButton*       colorButtonHandle_;
    QtColorChooserButton*       colorButtonFeature_;

    ACG::Vec4f statusColor_;
    ACG::Vec4f areaColor_;
    ACG::Vec4f handleColor_;
    ACG::Vec4f featureColor_;

    double dihedral_angle_threshold_;

    /** @} */

};

/// Traverse the scenegraph and call the selection function for all mesh nodes
class SelectVolumeAction
{
public:

    SelectVolumeAction(QRegion&_region, MeshObjectSelectionPlugin* _plugin,
                       unsigned int _type, bool _deselection, ACG::GLState& _state) :
            state_(_state), region_(_region), plugin_(_plugin), type_(_type), deselection_(_deselection) {};

    void enter(BaseNode* /*_node*/) {};

    void leave(BaseNode* /*_node*/) {};

    bool operator()(BaseNode* _node);

private:
    ACG::GLState&               state_;
    QRegion&                    region_;
    MeshObjectSelectionPlugin*  plugin_;
    unsigned int                type_;
    bool                        deselection_;
};
//=============================================================================
#if defined(INCLUDE_TEMPLATES) && !defined(MESHOBJECTSELECTIONPLUGINT_CC)
#define MESHOBJECTSELECTIONPLUGINT_TEMPLATES
#include "MeshObjectSelectionPluginT.cc"
#endif

//=============================================================================
#endif // MESHOBJECTSELECTIONPLUGIN_HH defined
//=============================================================================
