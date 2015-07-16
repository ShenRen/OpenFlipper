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

//=============================================================================
//
// SmootherPlugin
//
//=============================================================================

#ifndef SMOOTHERPLUGIN_HH
#define SMOOTHERPLUGIN_HH

//== INCLUDES =================================================================

#include <QObject>
#include <QMenuBar>
#include <QTimer>

#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/ToolboxInterface.hh>
#include <OpenFlipper/BasePlugin/BackupInterface.hh>
#include <OpenFlipper/BasePlugin/ScriptInterface.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>

#include <OpenFlipper/common/Types.hh>
#include <ObjectTypes/PolyMesh/PolyMesh.hh>
#include <ObjectTypes/TriangleMesh/TriangleMesh.hh>

#include <OpenMesh/Tools/Smoother/JacobiLaplaceSmootherT.hh>

#include "SmootherToolbarWidget.hh"


//== CLASS DEFINITION =========================================================


/** Plugin for Smoother Support
 */
class SmootherPlugin : public QObject, BaseInterface, ToolboxInterface, LoggingInterface, BackupInterface, ScriptInterface
{
  Q_OBJECT
  Q_INTERFACES(BaseInterface)
  Q_INTERFACES(ToolboxInterface)
  Q_INTERFACES(BackupInterface)
  Q_INTERFACES(LoggingInterface)
  Q_INTERFACES(ScriptInterface)

#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "org.OpenFlipper.Plugins.Plugin-Smoother")
#endif

signals:

  // BaseInterface
  void updateView();

  void updatedObject(int, const UpdateType&);

  void setSlotDescription(QString     _slotName,   QString     _slotDescription,
                          QStringList _parameters, QStringList _descriptions);

  // LoggingInterface
  void log(Logtype _type, QString _message);
  void log(QString _message);
  
  // ToolboxInterface
  void addToolbox( QString _name  , QWidget* _widget, QIcon* _icon );
  
  // BackupInterface
  void createBackup( int _id , QString _name, UpdateType _type = UPDATE_ALL );

  // ScriptInterface
  void scriptInfo(QString _functionName);
  
public :

  /// default constructor
  SmootherPlugin();

  /// default destructor
  ~SmootherPlugin() {};

  /// Name of the Plugin
  QString name(){ return (QString("Smoother")); };

  /// Description of the Plugin
  QString description() { return (QString("Mesh Smoothing ...")); };

private slots:

  void initializePlugin(); // BaseInterface
  
  /// Second initialization stage
  void pluginsInitialized();

  /// Tell system that this plugin runs without ui
  void noguiSupported( ) {} ;

private :

  /// Widget for Toolbox
  SmootherToolbarWidget* tool_;
  QIcon* toolIcon_;

private slots:

  /// Slot connected to the smooth button in the toolbox
  void slot_smooth();

//===========================================================================
/** @name Scripting Functions
  * @{ */
//===========================================================================
public slots:

   /** \brief smooth an object
    *
    * @param _objectId        Id of the object that should be smoothed
    * @param _iterations      Number of iterations
    * @param _direction       Tangential or normal?
    * @param _continuity      Continuity selection
    * @param _maxDistance     Maximal distance after smoothing
    * @param _respectFeatures Keep features
    *
    */
   void smooth(int _objectId , int _iterations , QString _direction , QString _continuity, double _maxDistance = -1.0, bool _respectFeatures = true);

   /** @} */

public slots:
   QString version() { return QString("1.0"); };



private:
   typedef OpenMesh::Smoother::JacobiLaplaceSmootherT< TriMesh > SmootherType;
};

#endif //SMOOTHERPLUGIN_HH
