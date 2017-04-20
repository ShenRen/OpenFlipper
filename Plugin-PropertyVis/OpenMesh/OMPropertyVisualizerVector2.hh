/* ========================================================================= *
 *                                                                           *
 *                               OpenFlipper                                 *
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
 *                                                                           *
 * ========================================================================= */

/*===========================================================================*\
*                                                                            *
*   $Revision: 13620 $                                                       *
*   $LastChangedBy: moebius $                                                *
*   $Date: 2012-02-01 14:51:25 +0100 (Mi, 01 Feb 2012) $                     *
*                                                                            *
\*===========================================================================*/

#ifndef OM_PROPERTY_VISUALIZER_VECTOR2_HH
#define OM_PROPERTY_VISUALIZER_VECTOR2_HH

#include "OMPropertyVisualizer.hh"

#include <ACG/Scenegraph/LineNode.hh>
#include <OpenFlipper/BasePlugin/PluginFunctions.hh>
#include <OpenMesh/Core/Utils/PropertyManager.hh>

#include "Widgets/VectorWidget.hh"

#include <iostream>


#include <cmath>

template <typename MeshT, typename VectorType>
class OMPropertyVisualizerVector2 : public OMPropertyVisualizer<MeshT>{

public:
        OMPropertyVisualizerVector2(MeshT* _mesh, int _objectID, PropertyInfo _propertyInfo);
    virtual ~OMPropertyVisualizerVector2(){clear();}

    virtual void clear();

protected:

    virtual void visualizeFaceProp(bool _setDrawMode = true);
    virtual void visualizeEdgeProp(bool _setDrawMode = true);
    virtual void visualizeHalfedgeProp(bool _setDrawMode = true);
    virtual void visualizeVertexProp(bool _setDrawMode = true);

    virtual void visualizeFacePropOnEdges();

    virtual void visualizeFacePropAsStrokes();
    virtual void visualizeEdgePropAsStrokes();
    virtual void visualizeHalfedgePropAsStrokes();
    virtual void visualizeVertexPropAsStrokes();

    virtual void removeProperty();
    virtual void duplicateProperty();

    virtual QString getPropertyText(unsigned int index);

    virtual void setFacePropertyFromText(unsigned int index, QString text);
    virtual void setEdgePropertyFromText(unsigned int index, QString text);
    virtual void setHalfedgePropertyFromText(unsigned int index, QString text);
    virtual void setVertexPropertyFromText(unsigned int index, QString text);

    ACG::SceneGraph::LineNode*  lineNode;
};


#if defined(INCLUDE_TEMPLATES) && !defined(OM_PROPERTY_VISUALIZER_VECTOR2_CC)
#include "OMPropertyVisualizerVector2T.cc"
#endif

#endif /* OM_PROPERTY_VISUALIZER_VECTOR2_HH */
