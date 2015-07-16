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

#ifndef MESHREPAIRPLUGIN_HH
#define MESHREPAIRPLUGIN_HH

#include <QObject>
#include <QMenuBar>

#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>
#include <OpenFlipper/BasePlugin/ToolboxInterface.hh>
#include <OpenFlipper/BasePlugin/BackupInterface.hh>
#include <OpenFlipper/BasePlugin/ScriptInterface.hh>
#include <OpenFlipper/BasePlugin/RPCInterface.hh>
#include <OpenFlipper/common/Types.hh>
#include <ObjectTypes/PolyMesh/PolyMesh.hh>
#include <ObjectTypes/TriangleMesh/TriangleMesh.hh>

#include "MeshRepairToolbar.hh"

class MeshRepairPlugin : public QObject, BaseInterface , ToolboxInterface, BackupInterface, LoggingInterface, ScriptInterface, RPCInterface
{
    Q_OBJECT
    Q_INTERFACES(BaseInterface)
    Q_INTERFACES(ToolboxInterface)
    Q_INTERFACES(BackupInterface)
    Q_INTERFACES(LoggingInterface)
    Q_INTERFACES(ScriptInterface)
    Q_INTERFACES(RPCInterface)

#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "org.OpenFlipper.Plugins.Plugin-MeshRepair")
#endif

signals:

    void updateView();

    // Base Interface
    void updatedObject(int _identifier, const UpdateType& _type);
    void setSlotDescription(QString     _slotName,   QString     _slotDescription,
                            QStringList _parameters, QStringList _descriptions);

    // Logging interface
    void log(Logtype _type, QString _message);
    void log(QString _message);

    // ToolboxInterface
    void addToolbox( QString _name  , QWidget* _widget, QIcon* _icon );

    // Backup Interface
    void createBackup( int _objectid, QString _name, UpdateType _type = UPDATE_ALL);

    // RPC Interface
    void pluginExists( QString _pluginName , bool& _exists  ) ;
    void functionExists( QString _pluginName , QString _functionName , bool& _exists  );

    // ScriptInterface
    void scriptInfo( QString _functionName );

private slots:

    void initializePlugin();
    void pluginsInitialized();

public :

    MeshRepairPlugin();

    ~MeshRepairPlugin() {};

    QString name() {
        return (QString("Mesh Repair"));
    };
    QString description( ) {
        return (QString("Functionality to repair meshes"));
    };

private :

    /// Widget for Toolbox
    MeshRepairToolbarWidget* tool_;
    QIcon* toolIcon_;

private slots:

    /// Button slot
    void slotRemoveSelectedVal3Vertices();

    /// Button slot
    void slotRemoveSelectedEdges();

    /// Button slot
    void slotDetectSkinnyTriangleByAngle();

    /// Button slot
    void slotRemoveSkinnyTriangleByAngle();

    /// Button slot
    void slotDetectFoldover();

    /// Button slot
    void slotDetectTriangleAspect();

    /// Button slot
    void slotFlipOrientation();

    /// Button slot
    void slotFixMesh();

    /// Button slot
    void slotUpdateVertexNormals();

    /// Button slot
    void slotUpdateFaceNormals();

    /// Button slot
    void slotUpdateHalfedgeNormals();

    /// Button slot
    void slotUpdateNormals();

    /// Button Slot
    void slotDetectEdgesLonger();

    /// Button Slot
    void slotDetectEdgesShorter();

    /// Button slot
    void slotDetectFlatValence3Vertices();

    /// Button slot
    void slotSnapBoundary();

    /// Button slot
    void slotFixNonManifoldVertices();


    //===========================================================================
    /** @name Scripting functions
    * @{ */
    //===========================================================================

public slots:

    // ==================================================
    // Vertex operations
    // ==================================================

    /** \brief Detect valence 3 vertices with faces that lie in the plane of their adjacent triangles
     *
     */
    void detectFlatValence3Vertices(int _objectId, double _angle);

    /// Remove all selected valence 3 vertices
    void removeSelectedVal3Vertices(int _objectId);

    // ==================================================
    // Edge operations
    // ==================================================

    /** \brief Selects all edges of an object which are shorter than the given length
     *
     * @param _objectId Id of the object
     * @param _length   maximal edge length to select
     */
    void selectEdgesShorterThan(int _objectId,double _length);

    /** \brief Selects all edges of an object which are larger than the given length
     *
     * @param _objectId Id of the object
     * @param _length   Minimal edge length to select
     */
    void selectEdgesLongerThan(int _objectId,double _length);

    /** \brief Removes all selected edges
     *
     * @param _objectId Id of the object
     */
    void removeSelectedEdges(int _objectId);

    /** \brief  Detect/Remove edges where neighboring faces form angle > _angle degrees
     *
     * @param _objectId Id of the object
     * @param _angle    minimal angle to detect
     * @param _remove   Detect them or also remove them?
     */
    void detectSkinnyTriangleByAngle(int _objectId, double _angle, bool _remove);

    /** \brief Detect folded-over configurations by the dihedral angle
     *
     * @param _objectId Id of the object
     * @param _angle    Minimal dihedral angle that will be selected
     */
    void detectFoldover(int _objectId, float _angle);


    // ==================================================
    // Face operations
    // ==================================================

    /** \brief Detect triangles with aspect ratio greater than _aspect and select them
     *
     * @param _objectId Id of the object
     * @param _aspect   Minimal aspect ratio to select
     */
    void detectTriangleAspect(int _objectId, float _aspect);

    /** \brief Flips the normals of all selected faces by changing the vertex order
     *
     * @param _objectId Id of the object
     */
    void flipOrientation(int _objectId);

    // ==================================================
    // Normal recomputations
    // ==================================================

    /** \brief  Recomputes the face normals of an object
     *
     * @param _objectId Id of the object
     */
    void updateFaceNormals(int _objectId);

    /** \brief Recomputes the halfedge normals of an object
     *
     * @param _objectId Id of the object
     */
    void updateHalfedgeNormals(int _objectId);

    /** \brief Recomputes the vertex normals of an object
     *
     * @param _objectId Id of the object
     */
    void updateVertexNormals(int _objectId);

    /** \brief  Recomputes the face and vertex normals of an object
     *
     * @param _objectId Id of the object
     */
    void updateNormals(int _objectId);

    // ==================================================
    // General
    // ==================================================

    /** \brief Snaps selected vertices at boundaries
     *
     * Snaps selected boundary vertices together if they are closer than the given
     * distance. No new vertices will be introduced on either edge, so they are just
     * snapped to existing ones.
     *
     * @param _objectId Id of the object
     * @param _eps      Distance of vertices to snap
     */
    void snapBoundary(int _objectId, double _eps);

    /** \brief remove non-manifold vertices by duplicating them
     *
     * @param _objectId Id of the mesh to fix
     */
    void fixNonManifoldVertices(int _objectId);

    /** \brief Fix a mesh
     *
     * Degenerated faces will be removed and all vertices which are closer than the given distance
     * will be collapsed. Non-manifold configurations at vertices will be removed and all faces of
     * each component will be updated to have the same orientation.
     *
     * @param _objectId Id of the object to fix
     * @param _epsilon  Snapping distance
     */
    void fixMesh(int _objectId, double _epsilon);

    /** @} */

private:
    /** \brief select edges based on length
    *
    * Selects edges if they are larger or shorter than a given length,
    * @param _objectId Id of the object to select
    * @param _length Length
    * @param _larger true : select larger edges, false select smaller edges
    */
    void selectionEdgeLength(int _objectId, double _length, bool _larger);

    /** \brief flip orientation of selected faces.
     *
     * flips the orientation of selected faces or if nothing is selected, the whole mesh is flipped
     * for correct topology, some vertices may be duplicated
     *
     * @param _mesh target mesh
     *
     */
    template<typename MeshT>
    void flipOrientation(MeshT& _mesh);


public slots:
    QString version() { return QString("1.3"); };
};

#if defined(INCLUDE_TEMPLATES) && !defined(MESHREPAIRPLUGINT_CC)
#define MESHREPAIRPLUGIN_TEMPLATES
#include "MeshRepairPluginT.cc"
#endif

#endif //MESHREPAIRPLUGIN_HH
