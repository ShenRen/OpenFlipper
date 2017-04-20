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

#ifndef OM_PROPERTY_VISUALIZER_VECTOR_FIELD_DIFFERENCE_HH
#define OM_PROPERTY_VISUALIZER_VECTOR_FIELD_DIFFERENCE_HH

#include "OMPropertyVisualizer.hh"

#include <ACG/Scenegraph/LineNode.hh>
#include <OpenFlipper/BasePlugin/PluginFunctions.hh>

#include "Widgets/VectorFieldDifferenceWidget.hh"

#include <iostream>

#include <cmath>


template <typename MeshT>
class OMPropertyVisualizerVectorFieldDifference: public OMPropertyVisualizer<MeshT>{

public:
    OMPropertyVisualizerVectorFieldDifference(MeshT* _mesh, int _objectID, PropertyInfo _propertyInfo1, PropertyInfo _propertyInfo2);
    virtual ~OMPropertyVisualizerVectorFieldDifference(){}

    virtual QString getName() { return QObject::tr("Combination of %1 and %2").arg(PropertyVisualizer::propertyInfo.propName().c_str()).arg(propertyInfo2.propName().c_str()); }


protected:

    virtual void visualizeFaceProp(bool _setDrawMode = true);
    virtual void visualizeEdgeProp(bool _setDrawMode = true);
    virtual void visualizeHalfedgeProp(bool _setDrawMode = true);
    virtual void visualizeVertexProp(bool _setDrawMode = true);

    virtual void removeProperty(){} //no need to delete a property
    virtual void duplciateProperty(){ emit OMPropertyVisualizer<MeshT>::log("combined properties cannot be duplicated");}


    virtual QString getPropertyText(unsigned int index);

private:
    PropertyInfo propertyInfo2;

};


#if defined(INCLUDE_TEMPLATES) && !defined(OM_PROPERTY_VISUALIZER_VECTOR_FIELD_DIFFERENCE_CC)
#include "OMPropertyVisualizerVectorFieldDifferenceT.cc"
#endif
#endif /* OM_PROPERTY_VISUALIZER_VECTOR_FIELD_DIFFERENCE_HH */
