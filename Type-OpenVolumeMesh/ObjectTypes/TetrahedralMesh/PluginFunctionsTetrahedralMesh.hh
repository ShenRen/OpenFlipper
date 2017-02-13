/*===========================================================================*\
*                                                                            *
 *                              OpenFlipper                                   *
 *      Copyright (C) 2001-2014 by Computer Graphics Group, RWTH Aachen       *
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
 *                                                                           *
 *   $Revision$                                                          *
 *   $Date$                   *
 *   $LastChangedBy$                                                  *
 *                                                                           *
\*===========================================================================*/

/**
 * \file PluginFunctionsTetrahedralMesh.hh
 * This file contains functions which can be used by plugins to access
 * TetrahedralMeshes in the framework.
 */

#ifndef PLUGINFUNCTIONSTETRAHEDRALMESH_HH
#define PLUGINFUNCTIONSTETRAHEDRALMESH_HH

#include <ObjectTypes/VolumeMeshObject/VolumeMeshObject.hh>
#include <ObjectTypes/TetrahedralMesh/TetrahedralMesh.hh>
#include <OpenFlipper/common/Types.hh>
#include <OpenFlipper/common/ObjectTypeDLLDefines.hh>

//== NAMESPACES ===============================================================

/** The Namespace PluginFunctions contains functions for all plugins. */
namespace PluginFunctions {


/** \brief Get a pointer to every TetrahedralMesh which is marked as a source.
 *
 * @param _meshes ( vector returning the source hexahedral meshes )
 * @return false, if no hexahedral mesh is selected as source
*/
OBJECTTYPEDLLEXPORT
bool getSourceMeshes( std::vector<TetrahedralMesh*>& _meshes  );

/** \brief Get a pointer to every TetrahedralMesh which is marked as a target.
 *
 * @param _meshes ( vector returning the target hexahedral meshes )
 * @return false, if no hexahedral mesh is selected as target
*/
OBJECTTYPEDLLEXPORT
bool getTargetMeshes( std::vector<TetrahedralMesh*>& _meshes  );


/** This functions returns the object with the given id if it is an TetrahedralMeshObject.
 * See get_object(  int _identifier , BaseObject*& _object ) for more details.
 */
OBJECTTYPEDLLEXPORT
bool getObject( int _identifier, TetrahedralMeshObject*& _object );

/** \brief Get an TetrahedralMesh from an object.
 *
 * @param _object The object should be of type BaseDataObject. If the content is
 * a hexahedral mesh, a hexahedral mesh will be returned. Otherwise a NULL pointer is returned.
 */
OBJECTTYPEDLLEXPORT
TetrahedralMesh* tetrahedralMesh( BaseObjectData* _object );

/** \brief Get a TetrahedralMesh from an object id.
 *
 * @param _identifier Identifier of the object. If its a TetrahedralMesh mesh, the function will return the pointer to the mesh
 *                    otherwise 0
 */
OBJECTTYPEDLLEXPORT
TetrahedralMesh* tetrahedralMesh( int _identifier );

/** \brief Cast an BaseObject to an TetrahedralMeshObject if possible
 *
 * @param _object The object should be of type BaseDataObject. If the content is
 * an TetrahedralMesh, an TetrahedralMeshObject is returned. Otherwise a NULL pointer is returned.
 */
OBJECTTYPEDLLEXPORT
TetrahedralMeshObject* tetrahedralMeshObject( BaseObjectData* _object );


/** \brief Cast an int to an TetrahedralMeshObject if possible
 *
 * @param _identifier
 */

OBJECTTYPEDLLEXPORT
TetrahedralMeshObject* tetrahedralMeshObject( int _identifier );

}

#endif // PLUGINFUNCTIONSTETRAHEDRALMESH_HH
