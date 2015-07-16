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

#ifndef COMPONENTSPLUGIN_HH
#define COMPONENTSPLUGIN_HH

#include <QObject>

#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/MouseInterface.hh>
#include <OpenFlipper/BasePlugin/PickingInterface.hh>
#include <OpenFlipper/BasePlugin/BackupInterface.hh>
#include <OpenFlipper/BasePlugin/LoadSaveInterface.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>
#include <OpenFlipper/BasePlugin/ScriptInterface.hh>
#include <OpenFlipper/BasePlugin/ToolbarInterface.hh>
#include <OpenFlipper/common/Types.hh>
#include <ObjectTypes/PolyMesh/PolyMesh.hh>
#include <ObjectTypes/TriangleMesh/TriangleMesh.hh>

class ComponentsPlugin : public QObject, BaseInterface, MouseInterface, PickingInterface, LoggingInterface, BackupInterface, LoadSaveInterface, ToolbarInterface,ScriptInterface
{
   Q_OBJECT
   Q_INTERFACES( BaseInterface )
   Q_INTERFACES( MouseInterface )
   Q_INTERFACES( PickingInterface )
   Q_INTERFACES( LoggingInterface )
   Q_INTERFACES( BackupInterface )
   Q_INTERFACES( LoadSaveInterface )
   Q_INTERFACES( ToolbarInterface )
   Q_INTERFACES( ScriptInterface )


#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "org.OpenFlipper.Plugins.Plugin-Components")
#endif

  signals:
    // BaseInterface
    void updateView();
    void updatedObject(int _identifier, const UpdateType& _type);
    void setSlotDescription(QString     _slotName,   QString     _slotDescription,
                            QStringList _parameters, QStringList _descriptions);

    // LoadSaveInterface
    void addEmptyObject( DataType _type, int& _id);
    void copyObject( int _oldId, int& _newId);

    // ToolBarInterface
    void addToolbar(QToolBar* _toolbar);
    void getToolBar( QString _name, QToolBar*& _toolbar);

    // PickingInterface
    void addPickMode( const std::string& _mode);

    //ScriptInterface
    void scriptInfo( QString _functionName );

    // LoggingInterface
    void log( Logtype _type, QString _message );
    void log( QString _message );

    // BackupInterface
    void createBackup( int _objectid, QString _name, UpdateType _type);

    // LoadSaveInterface
    void deleteObject( int _id );

  private slots:

    // BaseInterface
    void initializePlugin();
    void noguiSupported(){};
    void pluginsInitialized();

    // PickingInterface
    void slotPickModeChanged( const std::string& _mode);

    // MouseInterface:
    void slotMouseEvent( QMouseEvent* _event );

  public:
    ComponentsPlugin();
    ~ComponentsPlugin() { }

    void exit() {}

    QString name() { return (QString("Components")); };
    QString description( ) { return (QString("Handle mesh components")); };


//===========================================================================
/** @name GUI Related
  * @{ */
//===========================================================================

  private slots:

    /// Split into Components Button was hit
    void slotSplitComponentsButton();
    
    void slotBiggestComponentButton();

    /// Split Components of picked object
    void splitComponents(QMouseEvent* _event);

    void biggestComponent(QMouseEvent* _event);

  private :
    //toolbar
    QAction* splitAction_;
    QAction* biggestAction_;

/** @} */

//===========================================================================
/** @name Template Functions
  * @{ */
//===========================================================================

  /** \brief Split mesh into components
   *
   * This function will split an arbitrary component out of a mesh.
   * It takes a copy of the original mesh.
   *
   * @param _mesh the original mesh
   * @param _copy original mesh copy with identical topology and geometry
   *              the copy will contain one component of the original mesh.
   */
  template< class MeshT >
  void splitComponent( MeshT*   _mesh, MeshT* _copy);

  /** \brief Select the biggest component of the mesh
   *
   * This function will select the component of the mesh which has the most faces
   *
   * @param _mesh the mesh to work on
   */
  template< class MeshT >
  void selectBiggestComponent( MeshT*   _mesh);

  /** \brief Deletes all faces of a mesh that are not selected
   *
   * Use selectBiggestcomponent first to select the biggest component.
   * Than use this function to remove all other parts of the mesh.
   *
   * @param _mesh the mesh to work on
   */
  template< class MeshT >
  void deleteUnselectedFaces( MeshT*   _mesh);

/** @} */

//===========================================================================
/** @name Scripting Functions
  * @{ */
//===========================================================================

   public slots:


     /** \brief Splits the object into components
      *
      * i.e. creates ( number of components) new meshes and deletes the old mesh
      *
      * @param _objectId id of the target object
      */
      IdList splitComponents( int _objectId  );


      /** \brief Gets the biggest connected component of a mesh
       *
       * i.e. deletes all components except the one with the maximal number of faces
       *
       * @param _objId id of the target object
       */
      void biggestComponent(int _objId);

/** @} */

   public slots:
      QString version() { return QString("1.0"); };

   private:
      /// set scripting slot descriptions
      void setDescriptions();
};

#if defined(INCLUDE_TEMPLATES) && !defined(COMPONENTSPLUGIN_CC)
#define COMPONENTSPLUGIN_TEMPLATES
#include "ComponentsPluginT.cc"
#endif
//=============================================================================
#endif  //CUTPLUGIN_HH
//=============================================================================
