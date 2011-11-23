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

#ifndef PRIMITIVESGENERATORPLUGIN_HH
#define PRIMITIVESGENERATORPLUGIN_HH

#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>
#include <OpenFlipper/BasePlugin/MenuInterface.hh>
#include <OpenFlipper/BasePlugin/LoadSaveInterface.hh>
#include <OpenFlipper/common/Types.hh>

#include <ObjectTypes/TriangleMesh/TriangleMesh.hh>

class PrimitivesGeneratorPlugin: public QObject, BaseInterface, LoggingInterface, MenuInterface, LoadSaveInterface {
  Q_OBJECT
  Q_INTERFACES(BaseInterface)
  Q_INTERFACES(LoggingInterface)
  Q_INTERFACES(MenuInterface)
  Q_INTERFACES(LoadSaveInterface)

  signals:

  //BaseInterface
  void updateView();
  void updatedObject(int _id, const UpdateType _type);
  void setSlotDescription(QString     _slotName,   QString     _slotDescription,
                          QStringList _parameters, QStringList _descriptions);

  //LoggingInterface
  void log(Logtype _type, QString _message);
  void log(QString _message);

  // Menu Interface
  void getMenubarMenu (QString _name, QMenu *& _menu, bool _create);

  // LoadSaveInterface
  void addEmptyObject( DataType _type, int& _id);

public:

  // BaseInterface
  QString name() { return (QString("Primitives Generator Plugin"));  } ;
  QString description() {  return (QString("Plugin used for generating basic primitives")); } ;

private slots:

  /// BaseInterface
  void initializePlugin();
  void pluginsInitialized();

  /// Tell system that this plugin runs without ui
  void noguiSupported( ) {} ;

public slots:

  QString version() { return QString("1.0"); };

  int addSphere();
  int addTetrahedron();
  int addPyramid();
  int addTriangulatedCube();
  int addIcosahedron();

private:
  int addTriMesh();

  inline void add_face( int _vh1 , int _vh2, int _vh3 );

  std::vector<TriMesh::VertexHandle> vhandles_;

  TriMesh* triMesh_;
};

#endif //PRIMITIVESGENERATORPLUGIN_HH
