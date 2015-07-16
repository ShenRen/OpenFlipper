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
*   $Date$                    *
*                                                                            *
\*===========================================================================*/

#ifndef ADVANCEDREMESHERPLUGIN_HH
#define ADVANCEDREMESHERPLUGIN_HH

#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/BackupInterface.hh>
#include <OpenFlipper/BasePlugin/ToolboxInterface.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>
#include <OpenFlipper/BasePlugin/RPCInterface.hh>
#include <OpenFlipper/BasePlugin/ProcessInterface.hh>
#include <OpenFlipper/BasePlugin/ScriptInterface.hh>
#include <OpenFlipper/common/Types.hh>
#include <ObjectTypes/TriangleMesh/TriangleMesh.hh>
#include <ObjectTypes/PolyMesh/PolyMesh.hh>

#include "RemesherToolbox.hh"
#include "ProgressEmitter.hh"



class RemesherPlugin : public QObject, BaseInterface, BackupInterface, ToolboxInterface, LoggingInterface, RPCInterface, ProcessInterface, ScriptInterface
{
Q_OBJECT
Q_INTERFACES(BaseInterface)
Q_INTERFACES(BackupInterface)
Q_INTERFACES(ToolboxInterface)
Q_INTERFACES(LoggingInterface)
Q_INTERFACES(RPCInterface)
Q_INTERFACES(ProcessInterface)
Q_INTERFACES(ScriptInterface)

#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "org.OpenFlipper.Plugins.Plugin-Remesher")
#endif

signals:
  
  //BaseInterface
  void updateView();
  void updatedObject(int _id, const UpdateType& _type);

  void setSlotDescription(QString     _slotName,   QString     _slotDescription,
                            QStringList _parameters, QStringList _descriptions);

  //LoggingInterface:
  void log( Logtype _type, QString _message );
  void log( QString _message );

  // RPC Interface
  void pluginExists( QString _pluginName , bool& _exists  ) ;
  void functionExists( QString _pluginName , QString _functionName , bool& _exists  );

  // ToolboxInterface
  void addToolbox( QString _name  , QWidget* _widget, QIcon* _icon );

  // ProcessInterface
  void startJob( QString _jobId, QString _description, int _min, int _max, bool _blocking = false);
  void setJobState(QString _jobId, int _value);
  void setJobName(QString _jobId, QString _name);
  void finishJob(QString _jobId);
  void setJobDescription(QString _jobId, QString _description);

  // BackupInterface
  void createBackup(int _id , QString _name, UpdateType _type = UPDATE_ALL);

  // ScriptInterface
  void scriptInfo(QString _functionName);

private slots:

  // BaseInterface
  void initializePlugin();
  void pluginsInitialized(); // BaseInterface

public :

  RemesherPlugin();

  ~RemesherPlugin();

  QString name() { return (QString("Remesher")); };
  QString description( ) { return (QString("Remeshing plugin with isotropic and adaptive remesher.")); };

//GUI
private :
  RemesherToolBox* tool_;

  ProgressEmitter* progress_;

  QIcon* toolIcon_;

  enum RemeshingOperation { REMESH_UNIFORM, REMESH_ADAPTIVE };

  RemeshingOperation operation_;

private slots:
    
  //---- Adaptive Remeshing ------------
    
  void adaptiveRemeshingButtonClicked();

  void adaptiveRemeshing();

  //---- Adaptive Remeshing ------------

  void uniformRemeshingButtonClicked();

  void uniformRemeshing();

  //------------------------------------

  void threadFinished(QString _jobId);

  //------------------------------------

  /// Compute mean edge length and set values
  void computeInitValues();

  void slotAdaptiveRemeshing(int             _objectId,
                               double        _error,
                               double        _min_edge_length,
                               double        _max_edge_length,
                               unsigned int  _iters,
                               bool          _use_projection = true,
                               bool          _vertex_selection = true);

  void slotUniformRemeshing(int           _objectId,
                            double        _edge_length,
                            unsigned int  _iters,
                            unsigned int  _area_iters,
                            bool          _use_projection = true,
                            bool          _vertex_selection = true);


//scripting functions
public slots:
    
  void adaptiveRemeshing(int           _objectId,
                         double        _error,
                         double        _min_edge_length,
                         double        _max_edge_length,
                         unsigned int  _iters,
                         bool          _use_projection = true);

  void adaptiveRemeshingFaceSelection(int           _objectId,
                                      double        _error,
                                      double        _min_edge_length,
                                      double        _max_edge_length,
                                      unsigned int  _iters,
                                      bool          _use_projection = true);

  void uniformRemeshing(int           _objectId,
                        double        _edge_length,
                        unsigned int  _iters,
                        unsigned int  _area_iters,
                        bool          _use_projection = true);

  void uniformRemeshingFaceSelection(int           _objectId,
                                     double        _edge_length,
                                     unsigned int  _iters,
                                     unsigned int  _area_iters,
                                     bool          _use_projection = true);

public slots:
  QString version() { return QString("1.0"); };

  void noguiSupported( ) {} ;
};

#endif //ADVANCEDREMESHERPLUGIN_HH
