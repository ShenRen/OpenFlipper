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


#ifndef SUBDIVIDERPLUGIN_HH
#define SUBDIVIDERPLUGIN_HH

#include <QObject>
#include <QMenuBar>

#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>
#include <OpenFlipper/BasePlugin/ToolboxInterface.hh>
#include <OpenFlipper/BasePlugin/BackupInterface.hh>
#include <OpenFlipper/BasePlugin/ScriptInterface.hh>
#include <OpenFlipper/BasePlugin/LoadSaveInterface.hh>
#include <OpenFlipper/BasePlugin/BackupInterface.hh>
#include <OpenFlipper/common/Types.hh>
#include <ObjectTypes/TriangleMesh/TriangleMesh.hh>
#include <ObjectTypes/PolyMesh/PolyMesh.hh>

#include "subdividerToolbar.hh"

class SubdividerPlugin : public QObject, BaseInterface , ToolboxInterface, LoggingInterface, LoadSaveInterface, ScriptInterface, BackupInterface
{
  Q_OBJECT
  Q_INTERFACES(BaseInterface)
  Q_INTERFACES(ToolboxInterface)
  Q_INTERFACES(LoggingInterface)
  Q_INTERFACES(LoadSaveInterface)
  Q_INTERFACES(ScriptInterface)
  Q_INTERFACES(BackupInterface)

#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "org.OpenFlipper.Plugins.Plugin-Subdivider")
#endif

signals:
  void updateView();
  void updatedObject(int _id, const UpdateType& _type);

  void addPickMode( const std::string& _mode );

  void log(Logtype _type, QString _message);
  void log(QString _message);

  void addEmptyObject( DataType _type, int& _id );

  void defineViewMode(QString _mode, QStringList _usedWidgets);

  // ToolboxInterface
  void addToolbox( QString _name  , QWidget* _widget, QIcon* _icon );
  
  // BackupInterface
  void createBackup( int _id , QString _name, UpdateType _type = UPDATE_ALL );
  
  // ScriptInterface
  void setSlotDescription(QString     _slotName,   QString     _slotDescription,
                          QStringList _parameters, QStringList _descriptions);

private slots:

  void initializePlugin();
  
  void pluginsInitialized();
  
  /// Make plugin available in no-GUI mode
  void noguiSupported( ) {} ;
  
  void slotSubdivideUniformButton();

  void slotSimpleSubdivideButton();

public slots:

  /** \brief Scripting slot for subdivision
   *
   * Supported algorithm are:
   *
   * Polymeshes:
   * - catmullClark
   *
   * Triangle Meshes:
   * - loop
   * - sqrt3
   * - interpolating_sqrt3
   * - modifiedButterfly
   *
   * @param _objectId      Object to work on
   * @param _algorithm     String describing the algorithm to use
   * @param _steps         Number of iterations
   * @param _update_points Update original point locations
   */
  void subdivide(int _objectId, QString _algorithm , int _steps, bool _update_points = true);
  
  /** \brief Scripting slot for the simple subdivision algorithms
   *
   * Supported algorithm are:
   *
   * Triangle meshes:
   * - longest (Split all edges in descending order until all edges are shorter than given maximal edge length)
   *
   * @param _objectId      Object to work on
   * @param _algorithm     String describing the algorithm to use
   * @param _steps         Number of iterations
   * @param _parameter     Additional parameter depending on the algorithm (usually the maximal edge length in the final mesh)
   * @param _update_points Update original point locations
   */
  void simpleSubdivide(int _objectId, QString _algorithm , int _steps, double _parameter, bool _update_points = true);


public :
  SubdividerPlugin();
  ~SubdividerPlugin() {};

  QString name() { return (QString("Subdivider")); };
  QString description( ) { return (QString("Subdivide a mesh")); };

private :
  /// Widget for Toolbox
  subdividerToolbarWidget* tool_;
  QIcon* toolIcon_;

private:

  bool test_trimesh_object(int _identifier, BaseObjectData*& _object);


public slots:
   QString version() { return QString("1.0"); };
};

#endif //SUBDIVIDERPLUGIN_HH
