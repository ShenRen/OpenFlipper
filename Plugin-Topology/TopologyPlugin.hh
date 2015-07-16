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

#ifndef TOPOLOGYPLUGIN_HH
#define TOPOLOGYPLUGIN_HH

#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/MouseInterface.hh>
#include <OpenFlipper/BasePlugin/KeyInterface.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>
#include <OpenFlipper/BasePlugin/PickingInterface.hh>
#include <OpenFlipper/BasePlugin/BackupInterface.hh>
#include <OpenFlipper/BasePlugin/ToolbarInterface.hh>
#include <OpenFlipper/common/Types.hh>
#include <ObjectTypes/PolyMesh/PolyMesh.hh>
#include <ObjectTypes/TriangleMesh/TriangleMesh.hh>

class TopologyPlugin : public QObject, BaseInterface , MouseInterface, KeyInterface, PickingInterface, BackupInterface, LoggingInterface, ToolbarInterface
{
  Q_OBJECT
  Q_INTERFACES(BaseInterface)
  Q_INTERFACES(MouseInterface)
  Q_INTERFACES(KeyInterface)
  Q_INTERFACES(PickingInterface)
  Q_INTERFACES(BackupInterface)
  Q_INTERFACES(LoggingInterface)
  Q_INTERFACES(ToolbarInterface)

#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "org.OpenFlipper.Plugins.Plugin-Topology")
#endif

  signals:
    // BaseInterface
    void updateView();
    void updatedObject(int _id, const UpdateType& _type);

    // PickingInterface
    void addHiddenPickMode( const std::string& _mode );

    // BackupInterface
    void createBackup( int _objectid, QString _name, UpdateType _type = UPDATE_ALL);

    // LoggingInterface
    void log(Logtype _type, QString _message);
    void log(QString _message);

    // ToolbarInterface
    void addToolbar(QToolBar* _toolbar);

  private slots:

    // BaseInterface
    void pluginsInitialized();

    //PickingInterface
    void slotPickModeChanged( const std::string& _mode);

    // MouseInterface
    void slotMouseEvent( QMouseEvent* _event );

  private slots:

    /// called when an action on the toolbar was triggered
    void toolBarTriggered(QAction* _action);

  public :

    ///Constructor
    TopologyPlugin();

    /// Destructor
    ~TopologyPlugin() {};


    QString name() { return (QString("Topology")); };
    QString description( ) { return (QString("Allows to control Mesh topology (Edge Flips,...)")); };

  private :

    /// clear the add face vector
    void clearAddFaceVertices();

    /// Add a face
    void add_face(QMouseEvent* _event);

    /// Split a face at the current hit point
    void split_face(QMouseEvent* _event);

    /// Delete a face at the current hit point
    void delete_face(QMouseEvent* _event);

    /// Flip edge
    void flip_edge(QMouseEvent* _event);

    /// Collapse edge
    void collapse_edge(QMouseEvent* _event);

    /// Split Edge
    void split_edge(QMouseEvent* _event);

  private:

    std::vector< std::pair<int,int> > addFaceVertices_;

    QToolBar* toolbar_;

    QAction* edgeFlipAction_;
    QAction* edgeSplitAction_;
    QAction* edgeCollapseAction_;

    QAction* faceAddAction_;
    QAction* faceDeleteAction_;
    QAction* faceSplitAction_;


   public slots:
      QString version() { return QString("1.0"); };
};

#endif //TOPOLOGYPLUGIN_HH
