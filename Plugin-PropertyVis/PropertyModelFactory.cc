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

#include "PropertyModelFactory.hh"

#include <OpenFlipper/BasePlugin/PluginFunctions.hh>

#include "OpenMesh/OMPropertyModel.hh"
#include <ObjectTypes/TriangleMesh/TriangleMesh.hh>
#include <ObjectTypes/PolyMesh/PolyMesh.hh>

#ifdef ENABLE_OPENVOLUMEMESH_SUPPORT
    #include "OpenVolumeMesh/OVMPropertyModel.hh"
#endif /* ENABLE_OPENVOLUMEMESH_SUPPORT */

#ifdef ENABLE_OPENVOLUMEMESH_POLYHEDRAL_SUPPORT
    #include <ObjectTypes/PolyhedralMesh/PolyhedralMesh.hh>
#endif /* ENABLE_OPENVOLUMEMESH_POLYHEDRAL_SUPPORT */

#ifdef ENABLE_OPENVOLUMEMESH_HEXAHEDRAL_SUPPORT
    #include <ObjectTypes/HexahedralMesh/HexahedralMesh.hh>
#endif /* ENABLE_OPENVOLUMEMESH_HEXAHEDRAL_SUPPORT */


/**
 * @brief Returns the PropertyModel
 *
 * @param objectID The index of the object for which we want to get the
 * property model
 * @return The PropertyModel.
 *
 * For the given objectID this method finds out which type the object has
 * and creates and return the PropertyModel for that type of object.
 *
 * Currently supported are TriMesh, PolyMesh and - if found - also
 * PolyhedralMesh and HexahedralMesh.
 * For an unrecognized object a PropertyModel with no functionality will
 * be returned.
 */
PropertyModel* __PropertyModelFactory::getModel(int objectID)
{
    PropertyModelMap::iterator it = propertyModelMap.find(objectID);
    if (it != propertyModelMap.end())
        return it->second;

    BaseObjectData* object = 0;

    PluginFunctions::getObject( objectID, object );

    PropertyModel* propertyModel;

    if (object == 0) {
        return 0;
    }

    if ( object->dataType(DATA_TRIANGLE_MESH) )
    {
        TriMesh* mesh = PluginFunctions::triMesh(object);
        propertyModel = new OMPropertyModel<TriMesh>(mesh, objectID);
    }
    else if ( object->dataType(DATA_POLY_MESH) )
    {
        PolyMesh* mesh = PluginFunctions::polyMesh(object);
        propertyModel = new OMPropertyModel<PolyMesh>(mesh, objectID);
    }
#ifdef ENABLE_OPENVOLUMEMESH_POLYHEDRAL_SUPPORT
    else if ( object->dataType(DATA_POLYHEDRAL_MESH) )
    {
        PolyhedralMesh* mesh = PluginFunctions::polyhedralMesh(object);
        propertyModel = new OVMPropertyModel<PolyhedralMesh>(mesh, objectID);
    }
#endif /* ENABLE_OPENVOLUMEMESH_POLYHEDRAL_SUPPORT */
#ifdef ENABLE_OPENVOLUMEMESH_HEXAHEDRAL_SUPPORT
    else if ( object->dataType(DATA_HEXAHEDRAL_MESH) )
    {
        HexahedralMesh* mesh = PluginFunctions::hexahedralMesh(object);
        propertyModel = new OVMPropertyModel<HexahedralMesh>(mesh, objectID);
    }
#endif /* ENABLE_OPENVOLUMEMESH_HEXAHEDRAL_SUPPORT */
    else
    {
        return 0;
    }

    propertyModelMap.insert(std::pair<int, PropertyModel*>(objectID, propertyModel));
    return propertyModel;
}

void __PropertyModelFactory::deleteModel(int objectID)
{
    if (propertyModelMap.find(objectID) != propertyModelMap.end())
    {
        delete getModel(objectID);
        propertyModelMap.erase(objectID);
    }
}
