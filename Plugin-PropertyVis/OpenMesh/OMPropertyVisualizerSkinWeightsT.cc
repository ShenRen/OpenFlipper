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


#ifdef ENABLE_SKELETON_SUPPORT

#define OM_PROPERTY_VISUALIZER_SKIN_WEIGHTS_CC

#include "OMPropertyVisualizerSkinWeights.hh"

template <typename MeshT>
OMPropertyVisualizerSkinWeights<MeshT>::OMPropertyVisualizerSkinWeights(MeshT* _mesh, int _objectID, PropertyInfo _propertyInfo)
    : OMPropertyVisualizer<MeshT>(_mesh, _objectID, _propertyInfo)
{
    if (PropertyVisualizer::widget) delete PropertyVisualizer::widget;
    SkinWeightsWidget* w = new SkinWeightsWidget();
    w->paramSkinWeights->setTitle(QString("SkinWeights Parameters of ").append(PropertyVisualizer::propertyInfo.propName().c_str()));
    PropertyVisualizer::widget = w;
}

template <typename MeshT>
QString OMPropertyVisualizerSkinWeights<MeshT>::getPropertyText(unsigned int index)
{
    return OMPropertyVisualizer<MeshT>::template getPropertyText_<BaseSkin::SkinWeights>(index);
}

template <typename MeshT>
void OMPropertyVisualizerSkinWeights<MeshT>::visualizeFaceProp(bool /*_setDrawMode*/)
{
    OMPropertyVisualizer<MeshT>::log("Visualizing SkinWeights for faces not supported.");
}

template <typename MeshT>
void OMPropertyVisualizerSkinWeights<MeshT>::visualizeEdgeProp(bool /*_setDrawMode*/)
{
    OMPropertyVisualizer<MeshT>::log("Visualizing SkinWeights for edges not supported.");
}

template <typename MeshT>
void OMPropertyVisualizerSkinWeights<MeshT>::visualizeHalfedgeProp(bool /*_setDrawMode*/)
{
    OMPropertyVisualizer<MeshT>::log("Visualizing SkinWeights for halfedges not supported.");
}

template <typename MeshT>
void OMPropertyVisualizerSkinWeights<MeshT>::visualizeVertexProp(bool _setDrawMode)
{
    SkinWeightsWidget* skinWeightsWidget = static_cast<SkinWeightsWidget*>(PropertyVisualizer::widget);

    OpenMesh::VPropHandleT< BaseSkin::SkinWeights  > prop;

    if ( !OMPropertyVisualizer<MeshT>::mesh->get_property_handle(prop, OMPropertyVisualizer<MeshT>::propertyInfo.propName() ) )
        return;

    // Request vertex colors if required
    if ( !OMPropertyVisualizer<MeshT>::mesh->has_vertex_colors() )
        OMPropertyVisualizer<MeshT>::mesh->request_vertex_colors();

    for (typename MeshT::VertexIter v_it = OMPropertyVisualizer<MeshT>::mesh->vertices_begin() ; v_it != OMPropertyVisualizer<MeshT>::mesh->vertices_end() ; ++v_it){

        BaseSkin::SkinWeights weight = OMPropertyVisualizer<MeshT>::mesh->property(prop, *v_it);

        double value = 0.0;
        if ( weight.find( skinWeightsWidget->boneId->value() ) != weight.end() ) {
            value = weight[skinWeightsWidget->boneId->value()];
        }


        typename MeshT::Color color(value,1.0-value,0.0,1.0);

        // set color
        OMPropertyVisualizer<MeshT>::mesh->set_color(*v_it, color);
    }

    if (_setDrawMode)
        PluginFunctions::setDrawMode(ACG::SceneGraph::DrawModes::SOLID_POINTS_COLORED);
}

template<typename MeshT>
void OMPropertyVisualizerSkinWeights<MeshT>::removeProperty()
{
    OMPropertyVisualizer<MeshT>::template removeProperty_stage1<BaseSkin::SkinWeights>();
}


template<typename MeshT>
void OMPropertyVisualizerSkinWeights<MeshT>::duplicateProperty()
{
    OMPropertyVisualizer<MeshT>::template duplicateProperty_stage1<BaseSkin::SkinWeights>();
}

template<typename MeshT>
void OMPropertyVisualizerSkinWeights<MeshT>::setFacePropertyFromText(unsigned int index, QString text)
{
    OMPropertyVisualizer<MeshT>::log("Setting SkinWeights for faces not supported.");
}

template<typename MeshT>
void OMPropertyVisualizerSkinWeights<MeshT>::setEdgePropertyFromText(unsigned int index, QString text)
{
    OMPropertyVisualizer<MeshT>::log("Setting SkinWeights for edges not supported.");
}

template<typename MeshT>
void OMPropertyVisualizerSkinWeights<MeshT>::setHalfedgePropertyFromText(unsigned int index, QString text)
{
    OMPropertyVisualizer<MeshT>::log("Setting SkinWeights for halfedges not supported.");
}

template<typename MeshT>
void OMPropertyVisualizerSkinWeights<MeshT>::setVertexPropertyFromText(unsigned int index, QString text)
{
    OpenMesh::VPropHandleT< BaseSkin::SkinWeights > prop;
    MeshT* mesh = OMPropertyVisualizer<MeshT>::mesh;

    if ( !mesh->get_property_handle(prop, PropertyVisualizer::propertyInfo.propName() ) )
        emit this->log(LOGERR, QObject::tr("Error: No property with name ").append(PropertyVisualizer::propertyInfo.propName().c_str()));

    typename MeshT::VertexHandle vh = mesh->vertex_handle(index);

    BaseSkin::SkinWeights sw;

    text.remove(0,1);
    text.remove(text.length()-1, 1);

    QStringList slAll = text.split(")(");

    for (QStringList::iterator it = slAll.begin(); it != slAll.end(); ++it)
    {
        QStringList slCurrent = it->split(",");
        sw.insert(std::pair<int, double>(slCurrent.at(0).toInt(), slCurrent.at(1).toDouble()));
    }

    mesh->property(prop, vh) = sw;
}

#endif /* ENABLE_SKELETON_SUPPORT */
