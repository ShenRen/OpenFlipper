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

//================================================================
//
//  CLASS TypeSplatCloudPlugin
//
//    This class represents an OpenFlipper plugin for a new node named SplatCloud.
//
//================================================================


#ifndef TYPESPLATCLOUDPLUGIN_HH
#define TYPESPLATCLOUDPLUGIN_HH


//== INCLUDES ====================================================


#include <QObject>

#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/LoadSaveInterface.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>
#include <OpenFlipper/BasePlugin/TypeInterface.hh>
#include <OpenFlipper/BasePlugin/RPCInterface.hh>

#include <OpenFlipper/common/Types.hh>

#include <ObjectTypes/SplatCloud/SplatCloud.hh>
#include <ObjectTypes/Camera/Camera.hh>
#include <ObjectTypes/PolyMesh/PolyMesh.hh>


//== CLASS DEFINITION ============================================


class TypeSplatCloudPlugin : public QObject, BaseInterface, LoadSaveInterface, LoggingInterface, TypeInterface, RPCInterface
{
  Q_OBJECT
  Q_INTERFACES( BaseInterface     )
  Q_INTERFACES( LoadSaveInterface )
  Q_INTERFACES( LoggingInterface  )
  Q_INTERFACES( TypeInterface     )
  Q_INTERFACES( RPCInterface      )

#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "org.OpenFlipper.Plugins.Plugin-TypeSplatCloud")
#endif

signals:

  //-- Type Interface --
  void emptyObjectAdded( int _objectId );

  //-- LoadSave Interface --
  void addEmptyObject( DataType _type, int &_objectId );
  void deleteObject( int _objectId );

  //-- Logging Interface --
  void log(                QString _message );
  void log( Logtype _type, QString _message );

public slots:

  //-- Base Interface --
  QString version() { return QString( "1.0" ); };

  //-- Type Interface --
  int addEmpty();
  DataType supportedType() { return DATA_SPLATCLOUD; }; 
  void generateBackup( int _objectId, QString _name, UpdateType _type );

private slots:

  // -- Base Inferface --
  void noguiSupported() { }
  void slotViewChanged();
  void slotObjectPropertiesChanged( int _objectId );
  void slotObjectUpdated          ( int _objectId, const UpdateType &_updateType );

  //-- LoadSave Interface --
  void objectDeleted( int _objectId );

public:

  // standard constructor/destructor
  TypeSplatCloudPlugin();
  ~TypeSplatCloudPlugin() { }

  //-- Base Interface --
  QString name()        { return QString(     "TypeSplatCloud"             ); };
  QString description() { return QString( tr( "Register SplatCloud type" ) ); };

  // -- Type Interface --
  bool registerType();

private:

  // ungroup group-object
  void ungroupGroupObject( GroupObject *_groupObject );

  // get group-objects
  GroupObject *getCamerasGroupObject ( const SplatCloud_CameraManager  &_cameraManager  );
  GroupObject *getClustersGroupObject( const SplatCloud_ClusterManager &_clusterManager );

  // ungroup objects
  void ungroupCameraObjects ( const SplatCloud_CameraManager  &_cameraManager  );
  void ungroupClusterObjects( const SplatCloud_ClusterManager &_clusterManager );

  // group objects
  void groupCameraObjects ( const SplatCloud_CameraManager  &_cameraManager  );
  void groupClusterObjects( const SplatCloud_ClusterManager &_clusterManager );

  // add objects
  void addCameraObjects  ( SplatCloudObject *_splatCloudObject );
  void addClusterObjects ( SplatCloudObject *_splatCloudObject );

  // free objects
  void freeCameraObjects ( SplatCloudObject *_splatCloudObject );
  void freeClusterObjects( SplatCloudObject *_splatCloudObject );

  // erase camera
  void eraseCamera( CameraObject *_cameraObject );

  ACG::SceneGraph::DrawModes::DrawMode splatsDrawMode_;
  ACG::SceneGraph::DrawModes::DrawMode dotsDrawMode_;
};


//================================================================


#endif // TYPESPLATCLOUDPLUGIN_HH
