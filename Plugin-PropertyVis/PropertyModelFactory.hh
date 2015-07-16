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

#ifndef PROPERTY_MODEL_FACTORY_HH
#define PROPERTY_MODEL_FACTORY_HH

#include <OpenMesh/Core/Utils/SingletonT.hh>

#include "PropertyModel.hh"


class PropertyVisPlugin;

/*! \class __PropertyModelFactory
 *  \brief This class manages the creation of PropertyModels.
 *
 * This class is used to create the correct PropertyModel for a given object.
 * Using this factory the PropertyVisPlugin does not need to know about different
 * object types such as TriMesh, PolyMesh, PolyhedralMesh etc. If in future a new
 * type of object should be visualized only the factory needs to be updated in
 * order to recognize the new type and create the correct PropertyModel.
 *
 * A map of PropertyModels is kept so that for each object only one
 * PropertyModel is created.
 *
 * Note that this class is transformed to a Singleton and accessed via
 * PropertyModelFactory::Instance().
 */

class __PropertyModelFactory
{
    friend class OpenMesh::SingletonT<__PropertyModelFactory>;

public:
    /// Returns the PropertyModel.
    PropertyModel* getModel(int objectID);

    /// Deletes the PropertyModel.
    void deleteModel(int objectID);

private:
    __PropertyModelFactory(){}
    __PropertyModelFactory(__PropertyModelFactory&){}
    ~__PropertyModelFactory()
    {
        for (PropertyModelMap::iterator it = propertyModelMap.begin(); it != propertyModelMap.end(); ++it)
            delete it->second;
        propertyModelMap.erase(propertyModelMap.begin(), propertyModelMap.end()); //is this necessary?
    }

    typedef std::map<int, PropertyModel*> PropertyModelMap;
    PropertyModelMap propertyModelMap;
};

typedef OpenMesh::SingletonT<__PropertyModelFactory> PropertyModelFactory;

#endif /* PROPERTY_VISUALISER_FACTORY_HH */
