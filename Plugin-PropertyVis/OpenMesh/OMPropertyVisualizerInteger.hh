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

#ifndef OM_PROPERTY_VISUALIZER_INTEGER_HH
#define OM_PROPERTY_VISUALIZER_INTEGER_HH

#include "OMPropertyVisualizer.hh"

#include "Widgets/IntegerWidget.hh"

#include <ACG/Utils/HuePartitioningColors.hh>

#include <iostream>

template <typename MeshT, typename T>
class OMPropertyVisualizerInteger: public OMPropertyVisualizer<MeshT>{

public:
    OMPropertyVisualizerInteger(MeshT* _mesh, PropertyInfo _propertyInfo, bool isUnsigned);
    virtual ~OMPropertyVisualizerInteger(){}

protected:
    virtual void visualizeFaceProp(bool _setDrawMode = true);
    virtual void visualizeEdgeProp(bool _setDrawMode = true);
    virtual void visualizeHalfedgeProp(bool _setDrawMode = true);
    virtual void visualizeVertexProp(bool _setDrawMode = true);

    virtual void removeProperty();
    virtual void duplicateProperty();

    virtual T getValue(OpenMesh::FPropHandleT< T > prop, typename MeshT::FaceIter iter)     { return OMPropertyVisualizer<MeshT>::mesh->property(prop, *iter) ; }
    virtual T getValue(OpenMesh::EPropHandleT< T > prop, typename MeshT::EdgeIter iter)     { return OMPropertyVisualizer<MeshT>::mesh->property(prop, *iter) ; }
    virtual T getValue(OpenMesh::HPropHandleT< T > prop, typename MeshT::HalfedgeIter iter) { return OMPropertyVisualizer<MeshT>::mesh->property(prop, *iter) ; }
    virtual T getValue(OpenMesh::VPropHandleT< T > prop, typename MeshT::VertexIter iter)  { return OMPropertyVisualizer<MeshT>::mesh->property(prop, *iter) ; }

    virtual QString getPropertyText(unsigned int index);

    virtual void setFacePropertyFromText(unsigned int index, QString text);
    virtual void setEdgePropertyFromText(unsigned int index, QString text);
    virtual void setHalfedgePropertyFromText(unsigned int index, QString text);
    virtual void setVertexPropertyFromText(unsigned int index, QString text);

    inline int             strToT   (QString str, int)             { return this->strToInt(str);  }
    inline unsigned int    strToT   (QString str, unsigned int)    { return this->strToUInt(str); }
    inline QString         TToStr   (int i)                        { return this->intToStr(i);    }
    inline QString         TToStr   (unsigned int i)               { return this->uintToStr(i);   }



    ACG::HuePartitioningColors mColorGenerator;

};


#if defined(INCLUDE_TEMPLATES) && !defined(OM_PROPERTY_VISUALIZER_INTEGER_CC)
#include "OMPropertyVisualizerIntegerT.cc"
#endif

#endif /* OM_PROPERTY_VISUALIZER_INTEGER_HH */
