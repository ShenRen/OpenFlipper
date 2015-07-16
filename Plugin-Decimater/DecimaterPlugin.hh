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

#ifndef DECIMATERPLUGIN_HH
#define DECIMATERPLUGIN_HH

//== INCLUDES =================================================================

#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/ToolboxInterface.hh>
#include <OpenFlipper/BasePlugin/BackupInterface.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>
#include <OpenFlipper/BasePlugin/ScriptInterface.hh>
#include <OpenFlipper/BasePlugin/RPCInterface.hh>
#include <OpenFlipper/BasePlugin/LoadSaveInterface.hh>
#include <OpenFlipper/BasePlugin/ProcessInterface.hh>

#include <OpenFlipper/common/Types.hh>

#include <ObjectTypes/TriangleMesh/TriangleMesh.hh>

#include "DecimaterInfo.hh"

#include "DecimaterToolbarWidget.hh"

#include "DecimateThread.hh"

#include <OpenMesh/Tools/Decimater/DecimaterT.hh>
#include <OpenMesh/Tools/Decimater/McDecimaterT.hh>
#include <OpenMesh/Tools/Decimater/MixedDecimaterT.hh>
#include <OpenMesh/Tools/Decimater/ModQuadricT.hh>
#include <OpenMesh/Tools/Decimater/ModNormalFlippingT.hh>
#include <OpenMesh/Tools/Decimater/ModHausdorffT.hh>
#include <OpenMesh/Tools/Decimater/ModNormalDeviationT.hh>
#include <OpenMesh/Tools/Decimater/ModEdgeLengthT.hh>
#include <OpenMesh/Tools/Decimater/ModAspectRatioT.hh>
#include <OpenMesh/Tools/Decimater/ModRoundnessT.hh>
#include <OpenMesh/Tools/Decimater/ModIndependentSetsT.hh>

#include <ObjectTypes/TriangleMesh/TriangleMesh.hh>

#include <ACG/Utils/SmartPointer.hh>
#include <vector>

#include <QObject>
#include <QMenuBar>
#include <QTimer>


//== CLASS DEFINITION =========================================================

/** Plugin for Decimater Support
 */
class DecimaterPlugin : public QObject, BaseInterface, ToolboxInterface, LoggingInterface, ScriptInterface, BackupInterface, RPCInterface, LoadSaveInterface, ProcessInterface
{
  Q_OBJECT
  Q_INTERFACES(BaseInterface)
  Q_INTERFACES(ToolboxInterface)
  Q_INTERFACES(BackupInterface)
  Q_INTERFACES(LoggingInterface)
  Q_INTERFACES(ScriptInterface)
  Q_INTERFACES(RPCInterface)
  Q_INTERFACES(LoadSaveInterface)
  Q_INTERFACES(ProcessInterface)

#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "org.OpenFlipper.Plugins.Plugin-Decimater")
#endif
signals:

  // BaseInterface
  void updateView();
  void updatedObject(int _id, const UpdateType& _type);
  void setSlotDescription(QString     _slotName,   QString     _slotDescription,
                          QStringList _parameters, QStringList _descriptions);

  // LoggingInterface
  void log(Logtype _type, QString _message);
  void log(QString _message);

  // RPC Interface
  void pluginExists( QString _pluginName , bool& _exists  ) ;
  void functionExists( QString _pluginName , QString _functionName , bool& _exists  );

  // ToolboxInterface
  void addToolbox( QString _name  , QWidget* _widget, QIcon* _icon );

  // ScriptInterface
  void scriptInfo( QString _functionName );

  // BackupInterface
  void createBackup( int _id , QString _name, UpdateType _type = UPDATE_ALL );

  // ProcessInterface
  void startJob( QString _jobId, QString _description , int _min , int _max , bool _blocking  = false);
  void cancelJob(QString _jobId );
  void finishJob(QString _jobId );
  void setJobState(QString, int);
  void setJobName(QString _jobId, QString _name );
  void setJobDescription(QString _jobId, QString _text );

  //self defined signals
  void jobCanceled(QString _jobId);

private slots:
    // BaseInterface
    void initializePlugin();
    void pluginsInitialized();

    void slotObjectUpdated( int _identifier , const UpdateType& _type ); // BaseInterface
    void slotObjectSelectionChanged( int _identifier ); // BaseInterface

    void objectDeleted(int _id); //LoadSaveInterface

    void slotAboutToRestore(int _id); //BackupInterface

    // Tell system that this plugin runs without ui
    void noguiSupported( ) {} ;

    // ProcessInterface
    void canceledJob (QString _job );

public :

  /// Default constructor
  DecimaterPlugin();

  /// Default destructor
  ~DecimaterPlugin() {};

  /// Name of the Plugin
  QString name(){ return (QString("Decimater")); };

  /// Description of the Plugin
  QString description() { return (QString(tr("Mesh Decimation ..."))); };


private :
  typedef DecimateThread::BaseDecimaterType             BaseDecimaterType;
  typedef DecimateThread::DecimaterType                 DecimaterType;
  typedef DecimateThread::McDecimaterType               McDecimaterType;
  typedef DecimateThread::MixedDecimaterType            MixedDecimaterType;

  typedef OpenMesh::Decimater::ModAspectRatioT< TriMesh >::Handle     ModAspectRatioH;
  typedef OpenMesh::Decimater::ModEdgeLengthT< TriMesh >::Handle      ModEdgeLengthH;
  typedef OpenMesh::Decimater::ModHausdorffT< TriMesh >::Handle       ModHausdorffH;
  typedef OpenMesh::Decimater::ModIndependentSetsT< TriMesh >::Handle ModIndependentH;
  typedef OpenMesh::Decimater::ModNormalDeviationT< TriMesh >::Handle ModNormalDeviationH;
  typedef OpenMesh::Decimater::ModNormalFlippingT< TriMesh >::Handle  ModNormalFlippingH;
  typedef OpenMesh::Decimater::ModQuadricT< TriMesh >::Handle         ModQuadricH;
  typedef OpenMesh::Decimater::ModRoundnessT< TriMesh >::Handle       ModRoundnessH;

  /// Widget for Toolbox
  DecimaterToolbarWidget* tool_;

  //saves the decimater and the object id
  struct DecimaterInit
  {
    ptr::shared_ptr<BaseDecimaterType> decimater;
    int objId;
    ModAspectRatioH     hModAspectRatio;
    ModEdgeLengthH      hModEdgeLength;
    ModHausdorffH       hModHausdorff;
    ModIndependentH     hModIndependent;
    ModNormalDeviationH hModNormalDeviation;
    ModNormalFlippingH  hModNormalFlipping;
    ModQuadricH         hModQuadric;
    ModRoundnessH       hModRoundness;
  };
  std::vector< ptr::shared_ptr<DecimaterInit> > decimater_objects_;
  QIcon* toolIcon_;

  int runningJobs_;

private slots:

  /// decimating called from button in toolbox
  void slot_decimate();
  void slot_decimate_finished(QString _jobId); //postprocess after threads finished

  /// init called from button in toolbox
  void slot_initialize();
  void slot_initialize_object(int obj_id, bool clear = false);

private slots:
  /// roundness slider - spinbox sync
  void slotUpdateRoundness(int    _value);
  void slotUpdateRoundness(double _value);
  void slotUpdateAspectRatio(int _value);
  void slotUpdateAspectRatio(double _value);

  /// slider / spinbox updates
  void slotUpdateDistance ();
  void slotUpdateNormalDev ();
  void slotUpdateVertices ();
  void slotUpdateTriangles ();
  void slotUpdateEdgeLength ();

  /// update number of vertices information
  void slotUpdateNumVertices();
  void slotUpdateNumTriangles();

  void slotMixedCounterValueChanged(double);
  void slotMixedSliderValueChanged(int);
  void slotDisableDecimation();

private:
  void initialize_object(BaseObjectData *obj);

//===========================================================================
/** @name Scripting Functions
  * @{ */
//===========================================================================

public slots:

  /** \brief decimate an object
   *
   * @param _objID       Id of the object that should be decimated
   * @param _constraints List of constraints for the decimater
   *
   */
  void decimate(int _objID, QVariantMap _constraints);

  /** @} */

public slots:
   QString version() { return QString("1.0"); };

};


#endif //DECIMATERPLUGIN_HH
