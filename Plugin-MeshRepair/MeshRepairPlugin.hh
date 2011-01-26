/*===========================================================================*\
*                                                                            *
*                              OpenFlipper                                   *
*      Copyright (C) 2001-2011 by Computer Graphics Group, RWTH Aachen       *
*                           www.openflipper.org                              *
*                                                                            *
*--------------------------------------------------------------------------- *
*  This file is part of OpenFlipper.                                         *
*                                                                            *
*  OpenFlipper is free software: you can redistribute it and/or modify       *
*  it under the terms of the GNU Lesser General Public License as            *
*  published by the Free Software Foundation, either version 3 of            *
*  the License, or (at your option) any later version with the               *
*  following exceptions:                                                     *
*                                                                            *
*  If other files instantiate templates or use macros                        *
*  or inline functions from this file, or you compile this file and          *
*  link it with other files to produce an executable, this file does         *
*  not by itself cause the resulting executable to be covered by the         *
*  GNU Lesser General Public License. This exception does not however        *
*  invalidate any other reasons why the executable file might be             *
*  covered by the GNU Lesser General Public License.                         *
*                                                                            *
*  OpenFlipper is distributed in the hope that it will be useful,            *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of            *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
*  GNU Lesser General Public License for more details.                       *
*                                                                            *
*  You should have received a copy of the GNU LesserGeneral Public           *
*  License along with OpenFlipper. If not,                                   *
*  see <http://www.gnu.org/licenses/>.                                       *
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
#include <OpenFlipper/common/Types.hh>
#include <ObjectTypes/PolyMesh/PolyMesh.hh>
#include <ObjectTypes/TriangleMesh/TriangleMesh.hh>

#include "MeshRepairToolbar.hh"

class MeshRepairPlugin : public QObject, BaseInterface , ToolboxInterface, BackupInterface, LoggingInterface
{
    Q_OBJECT
    Q_INTERFACES(BaseInterface)
    Q_INTERFACES(ToolboxInterface)
    Q_INTERFACES(BackupInterface)
    Q_INTERFACES(LoggingInterface)

signals:

    void updateView();

    // Base Interface
    void updatedObject(int _identifier, const UpdateType _type);

    void log(Logtype _type, QString _message);
    void log(QString _message);

    // ToolboxInterface
    void addToolbox( QString _name  , QWidget* _widget, QIcon* _icon );

    // Backup Interface
    void createBackup( int _objectid, QString _name);

private slots:

    void initializePlugin();
    void slotActiveObjectChanged() {};

public :

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
    void slotDetectFlatTriangle();
    
    /// Button slot
    void slotRemoveSelectedVal3Vertices();

    /// Button Slot
    void slotDetectEdgesLarger();

    /// Button Slot
    void slotDetectEdgesShorter();

    /// Button slot
    void slotRemoveSelectedEdges();

    /// Button slot
    void slotFlipOrientation();

    /// Button slot
    void slotDetectCaps();

    /// Button slot
    void slotRemoveCaps();

    /// Button slot
    void slotDetectFoldover();

    /// Button slot
    void slotDetectTriangleAspect();

    //Scripting functions:
public slots:

    /// Selects all edges of an oobject which are shorter than the given length
    void selectEdgesShorterThan(int _objectId,double _length);

    /// Selects all edges of an oobject which are larger than the given length
    void selectEdgesLargerThan(int _objectId,double _length);

    /// Removes all selected edges
    void removeSelectedEdges(int _objectId);

    /// Flips the normals of all faces by changing the vertex order
    void flipOrientation(int _objectId);

    /// Detect/Remove edges where neighbouring faces form angle > _angle degrees
    void detectCaps(int _objectId, double _angle, bool _remove);

private:
    /** \brief select edges based on length
    *
    * Selects edges if they are larger or shorter than a given length,
    * @param _objectId Id of the object to select
    * @param _length Length
    * @param _larger true : select larger edges, false select smaller edges
    */
    void selectionEdgeLength(int _objectId, double _length, bool _larger);

private:

    typedef ACG::Vec3uc Color;

public slots:
    QString version() {
        return QString("1.0");
    };
};

#endif //MESHREPAIRPLUGIN_HH
