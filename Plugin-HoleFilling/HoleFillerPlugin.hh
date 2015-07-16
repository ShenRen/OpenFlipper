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

#ifndef HOLEFILLINGPLUGIN_HH
#define HOLEFILLINGPLUGIN_HH

#include <QObject>
#include <QMenuBar>

#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/MouseInterface.hh>
#include <OpenFlipper/BasePlugin/PickingInterface.hh>
#include <OpenFlipper/BasePlugin/ToolboxInterface.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>
#include <OpenFlipper/BasePlugin/ScriptInterface.hh>
#include <OpenFlipper/BasePlugin/BackupInterface.hh>
#include <OpenFlipper/common/Types.hh>
#include <ObjectTypes/PolyMesh/PolyMesh.hh>
#include <ObjectTypes/TriangleMesh/TriangleMesh.hh>

#include "holefillerToolbar.hh"

#include <vector>
#include <utility>

class HoleFillerPlugin : public QObject, BaseInterface, MouseInterface, PickingInterface, ToolboxInterface, LoggingInterface, ScriptInterface, BackupInterface
{
  Q_OBJECT
  Q_INTERFACES(BaseInterface)
  Q_INTERFACES(MouseInterface)
  Q_INTERFACES(PickingInterface)
  Q_INTERFACES(ToolboxInterface)
  Q_INTERFACES(LoggingInterface)
  Q_INTERFACES(ScriptInterface)
  Q_INTERFACES(BackupInterface)
#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "org.OpenFlipper.Plugins.Plugin-HoleFilling")
#endif

  signals:
    // BaseInterface
    void updateView();
    void updatedObject(int _id, const UpdateType& _type);

    void addPickMode( const std::string& _mode );

    void log(Logtype _type, QString _message);
    void log(QString _message);

    void createBackup( int _objectid, QString _name, UpdateType _type = UPDATE_ALL);

    void scriptInfo( QString _functionName  );
    
    void setSlotDescription(QString     _slotName    ,   QString     _slotDescription,
                                          QStringList _parameters  , QStringList _descriptions);

    // ToolboxInterface
    void addToolbox( QString _name  , QWidget* _widget, QIcon* _icon );


  private slots:
    void slotObjectUpdated(int _identifier);

    void slotMouseWheelEvent(QWheelEvent * /*_event*/, const std::string & /*_mode*/){};

    void slotMouseEvent( QMouseEvent* /*_event*/ ){};

    // BaseInterface
    void initializePlugin();
    void pluginsInitialized() ;

    void detectButton( );

  public :

    ~HoleFillerPlugin() {};

    HoleFillerPlugin();

    QString name() { return (QString("Hole Filler")); };
    QString description( ) { return (QString("Fill Holes or connect meshes")); };

  private slots:
    void slotItemSelectionChanged();
    
    void slotCellDoubleClicked(int _row , int _col);
    void slotFillSelection();
  private :

    void update_menu();

    /// get a map from objectID to (selected) holeIDs
    void getSelectedHoles(std::vector<int>& _holeIds, std::vector<int>& _objIds);

    /// Widget for Toolbox
    HoleFillerToolbarWidget* tool_;

    /// map from the index in the table to (object-id, hole-id)
    std::vector< std::pair< int , int> > holeMapping_;

  //Scripting slots
  public slots:

    void fillAllHoles(int _objectID);

    void fillHole(int _objectID, int _edgeHandle);

  public slots:

    QString version() { return QString("1.3"); };
};

#endif //HOLEFILLINGPLUGIN_HH
