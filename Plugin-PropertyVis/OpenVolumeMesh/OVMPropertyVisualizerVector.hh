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

#ifndef OVM_PROPERTY_VISUALIZER_VECTOR_HH
#define OVM_PROPERTY_VISUALIZER_VECTOR_HH

#include "OVMPropertyVisualizer.hh"

#include <ACG/Scenegraph/LineNode.hh>
#include <OpenFlipper/BasePlugin/PluginFunctions.hh>

#include <OpenVolumeMesh/Core/BaseEntities.hh>
#include <OpenVolumeMesh/Core/BaseProperty.hh>

#include "Widgets/VectorWidget.hh"

#include <iostream>

template <typename MeshT>
class OVMPropertyVisualizerVector: public OVMPropertyVisualizer<MeshT>{

public:
    OVMPropertyVisualizerVector(MeshT* _mesh, int objectID, PropertyInfo _propertyInfo);
    virtual ~OVMPropertyVisualizerVector(){ clear(); }

    virtual void clear();

protected:

    template<typename PropType, typename EntityIterator>
    void visualizeVectorAsColorForEntity(PropType prop, EntityIterator e_begin, EntityIterator e_end);
    template<typename PropType, typename EntityIterator>
    void visualizeVectorLengthAsColorForEntity(PropType prop, EntityIterator e_begin, EntityIterator e_end);
    virtual void duplicateProperty();

    virtual void visualizeFacePropAsStrokes();
    virtual void visualizeEdgePropAsStrokes();
    virtual void visualizeHalfedgePropAsStrokes();
    virtual void visualizeVertexPropAsStrokes();
    virtual void visualizeCellPropAsStrokes();
    virtual void visualizeHalffacePropAsStrokes();

    virtual void visualizeFaceProp(bool _setDrawMode = true);
    virtual void visualizeEdgeProp(bool _setDrawMode = true);
    virtual void visualizeHalfedgeProp(bool _setDrawMode = true);
    virtual void visualizeVertexProp(bool _setDrawMode = true);
    virtual void visualizeCellProp(bool _setDrawMode = true);
    virtual void visualizeHalffaceProp(bool _setDrawMode = true);

    virtual QString getPropertyText(unsigned int index);

    virtual void setCellPropertyFromText(unsigned int index, QString text);
    virtual void setFacePropertyFromText(unsigned int index, QString text);
    virtual void setHalffacePropertyFromText(unsigned int index, QString text);
    virtual void setEdgePropertyFromText(unsigned int index, QString text);
    virtual void setHalfedgePropertyFromText(unsigned int index, QString text);
    virtual void setVertexPropertyFromText(unsigned int index, QString text);

    ACG::SceneGraph::LineNode*  lineNode;

};

#if defined(INCLUDE_TEMPLATES) && !defined(OVM_PROPERTY_VISUALIZER_VECTOR_CC)
#include "OVMPropertyVisualizerVectorT.cc"
#endif

#endif /* OVM_PROPERTY_VISUALIZER_VECTOR_HH */

