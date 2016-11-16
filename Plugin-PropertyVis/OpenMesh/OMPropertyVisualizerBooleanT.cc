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

#define OM_PROPERTY_VISUALIZER_BOOLEAN_CC

#include "OMPropertyVisualizerBoolean.hh"
#include <ACG/Utils/ColorConversion.hh>

template <typename MeshT>
OMPropertyVisualizerBoolean<MeshT>::OMPropertyVisualizerBoolean(MeshT* _mesh, PropertyInfo _propertyInfo)
    : OMPropertyVisualizer<MeshT>(_mesh, _propertyInfo)
{
    if (PropertyVisualizer::widget) delete PropertyVisualizer::widget;
    BooleanWidget* w = new BooleanWidget();
    w->paramBool->setTitle(QString("Boolean Parameters of ").append(PropertyVisualizer::propertyInfo.propName().c_str()));
    PropertyVisualizer::widget = w;
}

template <typename MeshT>
QString OMPropertyVisualizerBoolean<MeshT>::getPropertyText(unsigned int index)
{
    return OMPropertyVisualizer<MeshT>::template getPropertyText_<bool>(index);
}

template <typename MeshT>
void OMPropertyVisualizerBoolean<MeshT>::visualizeFaceProp(bool _setDrawMode)
{
    BooleanWidget* booleanWidget = static_cast<BooleanWidget*>(PropertyVisualizer::widget);
    MeshT* mesh = OMPropertyVisualizer<MeshT>::mesh;
    typename MeshT::Color colorTrue, colorFalse;

    colorTrue  = ACG::to_Vec4f(booleanWidget->colorTrue->color());
    colorFalse = ACG::to_Vec4f(booleanWidget->colorFalse->color());

    OpenMesh::FPropHandleT< bool > prop;

    if ( !mesh->get_property_handle(prop, PropertyVisualizer::propertyInfo.propName() ) )
        return;

    if ( !mesh->has_face_colors() )
        OMPropertyVisualizer<MeshT>::mesh->request_face_colors();

    for (typename MeshT::FaceIter f_it = mesh->faces_begin() ; f_it != mesh->faces_end() ; ++f_it)
        if ( mesh->property(prop, *f_it) )
            mesh->set_color(*f_it, colorTrue);
        else
            mesh->set_color(*f_it, colorFalse);

    if (_setDrawMode)
        PluginFunctions::setDrawMode(ACG::SceneGraph::DrawModes::SOLID_FACES_COLORED);

}

template <typename MeshT>
void OMPropertyVisualizerBoolean<MeshT>::visualizeEdgeProp(bool _setDrawMode)
{
    BooleanWidget* booleanWidget = static_cast<BooleanWidget*>(PropertyVisualizer::widget);
    MeshT* mesh = OMPropertyVisualizer<MeshT>::mesh;
    typename MeshT::Color colorTrue, colorFalse;

    colorTrue  = ACG::to_Vec4f(booleanWidget->colorTrue->color());
    colorFalse = ACG::to_Vec4f(booleanWidget->colorFalse->color());

    OpenMesh::EPropHandleT< bool > prop;

    if ( !mesh->get_property_handle(prop, PropertyVisualizer::propertyInfo.propName() ) )
        return;

    if ( !mesh->has_edge_colors() )
        mesh->request_edge_colors();

    for (typename MeshT::EdgeIter e_it = mesh->edges_begin() ; e_it != mesh->edges_end() ; ++e_it)
        if ( mesh->property(prop, *e_it) )
            mesh->set_color(*e_it, colorTrue);
        else
            mesh->set_color(*e_it, colorFalse);

    if (_setDrawMode)
        PluginFunctions::setDrawMode(ACG::SceneGraph::DrawModes::EDGES_COLORED);
}

template <typename MeshT>
void OMPropertyVisualizerBoolean<MeshT>::visualizeHalfedgeProp(bool _setDrawMode)
{
    BooleanWidget* booleanWidget = static_cast<BooleanWidget*>(PropertyVisualizer::widget);
    MeshT* mesh = OMPropertyVisualizer<MeshT>::mesh;
    typename MeshT::Color colorTrue, colorFalse;

    colorTrue  = ACG::to_Vec4f(booleanWidget->colorTrue->color());
    colorFalse = ACG::to_Vec4f(booleanWidget->colorFalse->color());

    OpenMesh::HPropHandleT< bool > prop;

    if ( !mesh->get_property_handle(prop, PropertyVisualizer::propertyInfo.propName() ) )
        return;

    if ( ! mesh->has_halfedge_colors() )
        mesh->request_halfedge_colors();

    for (typename MeshT::HalfedgeIter he_it = mesh->halfedges_begin() ; he_it != mesh->halfedges_end() ; ++he_it)
        if ( mesh->property(prop, *he_it) )
            mesh->set_color(*he_it, colorTrue);
        else
            mesh->set_color(*he_it, colorFalse);

    if (_setDrawMode)
        PluginFunctions::setDrawMode(ACG::SceneGraph::DrawModes::HALFEDGES_COLORED);

}

template <typename MeshT>
void OMPropertyVisualizerBoolean<MeshT>::visualizeVertexProp(bool _setDrawMode)
{
    BooleanWidget* booleanWidget = static_cast<BooleanWidget*>(PropertyVisualizer::widget);
    MeshT* mesh = OMPropertyVisualizer<MeshT>::mesh;
    typename MeshT::Color colorTrue, colorFalse;

    colorTrue  = ACG::to_Vec4f(booleanWidget->colorTrue->color());
    colorFalse = ACG::to_Vec4f(booleanWidget->colorFalse->color());

    OpenMesh::VPropHandleT< bool > prop;

    if ( !mesh->get_property_handle(prop, PropertyVisualizer::propertyInfo.propName() ) )
        return;

    if ( ! mesh->has_vertex_colors() )
        mesh->request_vertex_colors();

    for (typename MeshT::VertexIter v_it = mesh->vertices_begin() ; v_it != mesh->vertices_end() ; ++v_it)
        if ( mesh->property(prop, *v_it) )
            mesh->set_color(*v_it, colorTrue);
        else
            mesh->set_color(*v_it, colorFalse);

    if (_setDrawMode)
        PluginFunctions::setDrawMode(ACG::SceneGraph::DrawModes::SOLID_POINTS_COLORED);

}

template <typename MeshT>
void OMPropertyVisualizerBoolean<MeshT>::setFacePropertyFromText(unsigned int index, QString text)
{
    OpenMesh::FPropHandleT< bool > prop;
    MeshT* mesh = OMPropertyVisualizer<MeshT>::mesh;

    if ( !mesh->get_property_handle(prop, PropertyVisualizer::propertyInfo.propName() ) )
        emit this->log(LOGERR, QObject::tr("Error: No property with name ").append(PropertyVisualizer::propertyInfo.propName().c_str()));


    typename MeshT::FaceHandle fh = mesh->face_handle(index);

    mesh->property(prop, fh) = this->strToBool(text);
}

template <typename MeshT>
void OMPropertyVisualizerBoolean<MeshT>::setEdgePropertyFromText(unsigned int index, QString text)
{
    OpenMesh::EPropHandleT< bool > prop;
    MeshT* mesh = OMPropertyVisualizer<MeshT>::mesh;

    if ( !mesh->get_property_handle(prop, PropertyVisualizer::propertyInfo.propName() ) )
        emit this->log(LOGERR, QObject::tr("Error: No property with name ").append(PropertyVisualizer::propertyInfo.propName().c_str()));


    typename MeshT::EdgeHandle eh = mesh->edge_handle(index);

    mesh->property(prop, eh) = this->strToBool(text);
}

template <typename MeshT>
void OMPropertyVisualizerBoolean<MeshT>::setHalfedgePropertyFromText(unsigned int index, QString text)
{
    OpenMesh::HPropHandleT< bool > prop;
    MeshT* mesh = OMPropertyVisualizer<MeshT>::mesh;

    if ( !mesh->get_property_handle(prop, PropertyVisualizer::propertyInfo.propName() ) )
        emit this->log(LOGERR, QObject::tr("Error: No property with name ").append(PropertyVisualizer::propertyInfo.propName().c_str()));


    typename MeshT::HalfedgeHandle heh = mesh->halfedge_handle(index);

    mesh->property(prop, heh) = this->strToBool(text);
}

template <typename MeshT>
void OMPropertyVisualizerBoolean<MeshT>::setVertexPropertyFromText(unsigned int index, QString text)
{
    OpenMesh::VPropHandleT< bool > prop;
    MeshT* mesh = OMPropertyVisualizer<MeshT>::mesh;

    if ( !mesh->get_property_handle(prop, PropertyVisualizer::propertyInfo.propName() ) )
        emit this->log(LOGERR, QObject::tr("Error: No property with name ").append(PropertyVisualizer::propertyInfo.propName().c_str()));


    typename MeshT::VertexHandle vh = mesh->vertex_handle(index);

    mesh->property(prop, vh) = this->strToBool(text);
}

template<typename MeshT>
void OMPropertyVisualizerBoolean<MeshT>::removeProperty()
{
    OMPropertyVisualizer<MeshT>::template removeProperty_stage1<bool>();
}

template<typename MeshT>
void OMPropertyVisualizerBoolean<MeshT>::duplicateProperty()
{
    OMPropertyVisualizer<MeshT>::template duplicateProperty_stage1<bool>();
}
