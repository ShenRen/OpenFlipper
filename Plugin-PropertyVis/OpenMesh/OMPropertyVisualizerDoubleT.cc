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

#define OM_PROPERTY_VISUALIZER_DOUBLE_CC

#include <ACG/Utils/ColorConversion.hh>
#include "OMPropertyVisualizerDouble.hh"

template <typename MeshT>
OMPropertyVisualizerDouble<MeshT>::OMPropertyVisualizerDouble(MeshT* _mesh, PropertyInfo _propertyInfo)
    : OMPropertyVisualizer<MeshT>(_mesh, _propertyInfo)
{
    if (PropertyVisualizer::widget) delete PropertyVisualizer::widget;
    DoubleWidget* w = new DoubleWidget();
    w->paramDouble->setTitle(QString("Double Parameters of ").append(PropertyVisualizer::propertyInfo.propName().c_str()));
    PropertyVisualizer::widget = w;
#ifdef ENABLE_PROPVIS_HISTOGRAMS
    this->connect(w->computeHistogramButton, &QPushButton::clicked,
                  [this, w](){this->template showHistogram<double>(w->histogram);});
#endif
}

template <typename MeshT>
void OMPropertyVisualizerDouble<MeshT>::visualizeFaceProp(bool _setDrawMode)
{
    DoubleWidget* doubleWidget = static_cast<DoubleWidget*>(PropertyVisualizer::widget);

    typename MeshT::Color colorMin = ACG::to_Vec4f(doubleWidget->doubleMin->color());

    // color coder in [0,1]
    ACG::IColorCoder *cc = doubleWidget->buildColorCoder();

    OpenMesh::FPropHandleT< double > prop;

    if ( !OMPropertyVisualizer<MeshT>::mesh->get_property_handle(prop, OMPropertyVisualizer<MeshT>::propertyInfo.propName() ) )
        return;

    double min, max;

    if ( doubleWidget->doubleAbsolute->isChecked() ){
        min = FLT_MAX;
        max = 0.0;
    } else {
        min = FLT_MAX;
        max = FLT_MIN;
    }

    for (typename MeshT::FaceIter f_it = OMPropertyVisualizer<MeshT>::mesh->faces_begin() ; f_it != OMPropertyVisualizer<MeshT>::mesh->faces_end() ; ++f_it){
        if ( doubleWidget->doubleAbsolute->isChecked() ){
            min = std::min( min, fabs(OMPropertyVisualizer<MeshT>::mesh->property(prop, *f_it)));
            max = std::max( max, fabs(OMPropertyVisualizer<MeshT>::mesh->property(prop, *f_it)));
        } else {
            min = std::min( min, OMPropertyVisualizer<MeshT>::mesh->property(prop, *f_it));
            max = std::max( max, OMPropertyVisualizer<MeshT>::mesh->property(prop, *f_it));
        }
    }

    // fixed range?
    if( doubleWidget->doubleFixedRange->isChecked())
    {
        min = doubleWidget->doubleFixedRangeMin->value();
        max = doubleWidget->doubleFixedRangeMax->value();
    }
    else
    {
        doubleWidget->doubleFixedRangeMin->setValue(min);
        doubleWidget->doubleFixedRangeMax->setValue(max);
    }

    double range = max - min;

    if ( ! OMPropertyVisualizer<MeshT>::mesh->has_face_colors() )
        OMPropertyVisualizer<MeshT>::mesh->request_face_colors();

    for (typename MeshT::FaceIter f_it = OMPropertyVisualizer<MeshT>::mesh->faces_begin() ; f_it != OMPropertyVisualizer<MeshT>::mesh->faces_end() ; ++f_it){

        if (range == 0.0)
          OMPropertyVisualizer<MeshT>::mesh->set_color(*f_it, colorMin);
        else
        {
            double v = OMPropertyVisualizer<MeshT>::mesh->property(prop, *f_it);

            // absolut value?
            if ( doubleWidget->doubleAbsolute->isChecked())
                v = fabs(v);

            // clamping
            v = std::max(min,v);
            v = std::min(max,v);

            double t = (v-min)/range;
            typename MeshT::Color color = cc->color_float4(t);

              // set color
            OMPropertyVisualizer<MeshT>::mesh->set_color(*f_it, color);
        }
    }
    delete cc;

    if (_setDrawMode)
        PluginFunctions::setDrawMode(ACG::SceneGraph::DrawModes::SOLID_FACES_COLORED);
}

template <typename MeshT>
void OMPropertyVisualizerDouble<MeshT>::visualizeEdgeProp(bool _setDrawMode)
{
    DoubleWidget* doubleWidget = static_cast<DoubleWidget*>(PropertyVisualizer::widget);
    typename MeshT::Color colorMin;

    colorMin = ACG::to_Vec4f(doubleWidget->doubleMin->color());

    // color coder in [0,1]
    ACG::IColorCoder *cc = doubleWidget->buildColorCoder();

    //TODO check if this also works if the property is Vec3d
    OpenMesh::EPropHandleT< double > prop;

    if ( !OMPropertyVisualizer<MeshT>::mesh->get_property_handle(prop, OMPropertyVisualizer<MeshT>::propertyInfo.propName() ) )
        return;

    double min, max;

    if ( doubleWidget->doubleAbsolute->isChecked() ){
        min = FLT_MAX;
        max = 0.0;
    } else {
        min = FLT_MAX;
        max = FLT_MIN;
    }

    for (typename MeshT::EdgeIter e_it = OMPropertyVisualizer<MeshT>::mesh->edges_begin() ; e_it != OMPropertyVisualizer<MeshT>::mesh->edges_end() ; ++e_it){
        if ( doubleWidget->doubleAbsolute->isChecked() ){
            min = std::min( min, fabs(OMPropertyVisualizer<MeshT>::mesh->property(prop, *e_it)));
            max = std::max( max, fabs(OMPropertyVisualizer<MeshT>::mesh->property(prop, *e_it)));
        } else {
            min = std::min( min, OMPropertyVisualizer<MeshT>::mesh->property(prop, *e_it));
            max = std::max( max, OMPropertyVisualizer<MeshT>::mesh->property(prop, *e_it));
        }
    }

    // fixed range?
    if( doubleWidget->doubleFixedRange->isChecked())
    {
        min = doubleWidget->doubleFixedRangeMin->value();
        max = doubleWidget->doubleFixedRangeMax->value();
    }
    else
    {
        doubleWidget->doubleFixedRangeMin->setValue(min);
        doubleWidget->doubleFixedRangeMax->setValue(max);
    }


    double range = max - min;

    if ( ! OMPropertyVisualizer<MeshT>::mesh->has_edge_colors() )
        OMPropertyVisualizer<MeshT>::mesh->request_edge_colors();

    for (typename MeshT::EdgeIter e_it = OMPropertyVisualizer<MeshT>::mesh->edges_begin() ; e_it != OMPropertyVisualizer<MeshT>::mesh->edges_end() ; ++e_it){

        if (range == 0.0)
            OMPropertyVisualizer<MeshT>::mesh->set_color(*e_it, colorMin);
        else {

            double v = OMPropertyVisualizer<MeshT>::mesh->property(prop, *e_it);

            // absolut value?
            if ( doubleWidget->doubleAbsolute->isChecked())
            v = fabs(v);

            // clamping
            v = std::max(min,v);
            v = std::min(max,v);

            double t = (v-min)/range;
            typename MeshT::Color color = cc->color_float4(t);

            // set color
            OMPropertyVisualizer<MeshT>::mesh->set_color(*e_it, color);
        }
    }
    delete cc;

    if (_setDrawMode)
        PluginFunctions::setDrawMode(ACG::SceneGraph::DrawModes::EDGES_COLORED);

}

template <typename MeshT>
void OMPropertyVisualizerDouble<MeshT>::visualizeHalfedgeProp(bool _setDrawMode)
{
    DoubleWidget* doubleWidget = static_cast<DoubleWidget*>(PropertyVisualizer::widget);

    typename MeshT::Color colorMin = ACG::to_Vec4f(doubleWidget->doubleMin->color());

    // color coder in [0,1]
    ACG::IColorCoder *cc = doubleWidget->buildColorCoder();

    //TODO check if this also works if the property is Vec3d
    OpenMesh::HPropHandleT< double > prop;

    if ( !OMPropertyVisualizer<MeshT>::mesh->get_property_handle(prop, OMPropertyVisualizer<MeshT>::propertyInfo.propName() ) )
        return;

    double min, max;

    if ( doubleWidget->doubleAbsolute->isChecked() ){
        min = FLT_MAX;
        max = 0.0;
    } else {
        min = FLT_MAX;
        max = FLT_MIN;
    }

    for (typename MeshT::HalfedgeIter he_it = OMPropertyVisualizer<MeshT>::mesh->halfedges_begin() ; he_it != OMPropertyVisualizer<MeshT>::mesh->halfedges_end() ; ++he_it){
        if ( doubleWidget->doubleAbsolute->isChecked() ){
            min = std::min( min, fabs(OMPropertyVisualizer<MeshT>::mesh->property(prop, *he_it)));
            max = std::max( max, fabs(OMPropertyVisualizer<MeshT>::mesh->property(prop, *he_it)));
        } else {
            min = std::min( min, OMPropertyVisualizer<MeshT>::mesh->property(prop, *he_it));
            max = std::max( max, OMPropertyVisualizer<MeshT>::mesh->property(prop, *he_it));
        }
    }

    // fixed range?
    if( doubleWidget->doubleFixedRange->isChecked())
    {
        min = doubleWidget->doubleFixedRangeMin->value();
        max = doubleWidget->doubleFixedRangeMax->value();
    }
    else
    {
        doubleWidget->doubleFixedRangeMin->setValue(min);
        doubleWidget->doubleFixedRangeMax->setValue(max);
    }

    double range = max - min;

    if ( ! OMPropertyVisualizer<MeshT>::mesh->has_halfedge_colors() )
        OMPropertyVisualizer<MeshT>::mesh->request_halfedge_colors();

    for (typename MeshT::HalfedgeIter he_it = OMPropertyVisualizer<MeshT>::mesh->halfedges_begin() ; he_it != OMPropertyVisualizer<MeshT>::mesh->halfedges_end() ; ++he_it){

        if (range == 0.0)
            OMPropertyVisualizer<MeshT>::mesh->set_color(*he_it, colorMin);
        else {

            double v = OMPropertyVisualizer<MeshT>::mesh->property(prop, *he_it);

            // absolut value?
            if ( doubleWidget->doubleAbsolute->isChecked())
            v = fabs(v);

            // clamping
            v = std::max(min,v);
            v = std::min(max,v);

            double t = (v-min)/range;
            typename MeshT::Color color = cc->color_float4(t);

            // set color
            OMPropertyVisualizer<MeshT>::mesh->set_color(*he_it, color);
        }
    }
    if (_setDrawMode)
        PluginFunctions::setDrawMode(ACG::SceneGraph::DrawModes::HALFEDGES_COLORED);
}

template <typename MeshT>
void OMPropertyVisualizerDouble<MeshT>::visualizeVertexProp(bool _setDrawMode)
{
    DoubleWidget* doubleWidget = static_cast<DoubleWidget*>(PropertyVisualizer::widget);

    typename MeshT::Color colorMin = ACG::to_Vec4f(doubleWidget->doubleMin->color());

    // color coder in [0,1]
    ACG::IColorCoder *cc = doubleWidget->buildColorCoder();

    //TODO check if this also works if the property is Vec3d
    OpenMesh::VPropHandleT< double > prop;

    if ( !OMPropertyVisualizer<MeshT>::mesh->get_property_handle(prop, OMPropertyVisualizer<MeshT>::propertyInfo.propName() ) )
        return;

    double min, max;

    if ( doubleWidget->doubleAbsolute->isChecked() ){
        min = FLT_MAX;
        max = 0.0;
    } else {
        min = FLT_MAX;
        max = FLT_MIN;
    }

    for (typename MeshT::VertexIter v_it = OMPropertyVisualizer<MeshT>::mesh->vertices_begin() ; v_it != OMPropertyVisualizer<MeshT>::mesh->vertices_end() ; ++v_it){
        if ( doubleWidget->doubleAbsolute->isChecked() ){
            min = std::min( min, fabs(OMPropertyVisualizer<MeshT>::mesh->property(prop, *v_it)));
            max = std::max( max, fabs(OMPropertyVisualizer<MeshT>::mesh->property(prop, *v_it)));
        } else {
            min = std::min( min, OMPropertyVisualizer<MeshT>::mesh->property(prop, *v_it));
            max = std::max( max, OMPropertyVisualizer<MeshT>::mesh->property(prop, *v_it));
        }
    }

    // fixed range?
    if( doubleWidget->doubleFixedRange->isChecked())
    {
        min = doubleWidget->doubleFixedRangeMin->value();
        max = doubleWidget->doubleFixedRangeMax->value();
    }
    else
    {
        doubleWidget->doubleFixedRangeMin->setValue(min);
        doubleWidget->doubleFixedRangeMax->setValue(max);
    }

    const double range = max - min;

    if ( ! OMPropertyVisualizer<MeshT>::mesh->has_vertex_colors() )
        OMPropertyVisualizer<MeshT>::mesh->request_vertex_colors();

    for (typename MeshT::VertexIter v_it = OMPropertyVisualizer<MeshT>::mesh->vertices_begin() ; v_it != OMPropertyVisualizer<MeshT>::mesh->vertices_end() ; ++v_it){

        if (range == 0.0)
            OMPropertyVisualizer<MeshT>::mesh->set_color(*v_it, colorMin);
        else {

            double v = OMPropertyVisualizer<MeshT>::mesh->property(prop, *v_it);

            // absolut value?
            if ( doubleWidget->doubleAbsolute->isChecked())
            v = fabs(v);

            // clamping
            v = std::max(min,v);
            v = std::min(max,v);

            double t = (v-min)/range;
            typename MeshT::Color color = cc->color_float4(t);

            // set color
            OMPropertyVisualizer<MeshT>::mesh->set_color(*v_it, color);
        }
    }

    if (_setDrawMode)
        PluginFunctions::setDrawMode(ACG::SceneGraph::DrawModes::SOLID_POINTS_COLORED);
}

template <typename MeshT>
void OMPropertyVisualizerDouble<MeshT>::setFacePropertyFromText(unsigned int index, QString text)
{
    OpenMesh::FPropHandleT< double > prop;
    MeshT* mesh = OMPropertyVisualizer<MeshT>::mesh;

    if ( !mesh->get_property_handle(prop, PropertyVisualizer::propertyInfo.propName() ) )
        emit this->log(LOGERR, QObject::tr("Error: No property with name ").append(PropertyVisualizer::propertyInfo.propName().c_str()));


    typename MeshT::FaceHandle fh = mesh->face_handle(index);

    mesh->property(prop, fh) = this->strToDouble(text);
}

template <typename MeshT>
void OMPropertyVisualizerDouble<MeshT>::setEdgePropertyFromText(unsigned int index, QString text)
{
    OpenMesh::EPropHandleT< double > prop;
    MeshT* mesh = OMPropertyVisualizer<MeshT>::mesh;

    if ( !mesh->get_property_handle(prop, PropertyVisualizer::propertyInfo.propName() ) )
        emit this->log(LOGERR, QObject::tr("Error: No property with name ").append(PropertyVisualizer::propertyInfo.propName().c_str()));


    typename MeshT::EdgeHandle eh = mesh->edge_handle(index);

    mesh->property(prop, eh) = this->strToDouble(text);
}

template <typename MeshT>
void OMPropertyVisualizerDouble<MeshT>::setHalfedgePropertyFromText(unsigned int index, QString text)
{
    OpenMesh::HPropHandleT< double > prop;
    MeshT* mesh = OMPropertyVisualizer<MeshT>::mesh;

    if ( !mesh->get_property_handle(prop, PropertyVisualizer::propertyInfo.propName() ) )
        emit this->log(LOGERR, QObject::tr("Error: No property with name ").append(PropertyVisualizer::propertyInfo.propName().c_str()));


    typename MeshT::HalfedgeHandle heh = mesh->halfedge_handle(index);

    mesh->property(prop, heh) = this->strToDouble(text);
}

template <typename MeshT>
void OMPropertyVisualizerDouble<MeshT>::setVertexPropertyFromText(unsigned int index, QString text)
{
    OpenMesh::VPropHandleT< double > prop;
    MeshT* mesh = OMPropertyVisualizer<MeshT>::mesh;

    if ( !mesh->get_property_handle(prop, PropertyVisualizer::propertyInfo.propName() ) )
        emit this->log(LOGERR, QObject::tr("Error: No property with name ").append(PropertyVisualizer::propertyInfo.propName().c_str()));


    typename MeshT::VertexHandle vh = mesh->vertex_handle(index);

    mesh->property(prop, vh) = this->strToDouble(text);
}

template<typename MeshT>
ACG::IColorCoder *OMPropertyVisualizerDouble<MeshT>::buildColorCoder()
{
    DoubleWidget* doubleWidget = static_cast<DoubleWidget*>(PropertyVisualizer::widget);
    return doubleWidget->buildColorCoder();
}


template<typename MeshT>
void OMPropertyVisualizerDouble<MeshT>::removeProperty()
{
    OMPropertyVisualizer<MeshT>::template removeProperty_stage1<double>();
}

template<typename MeshT>
void OMPropertyVisualizerDouble<MeshT>::duplicateProperty()
{
    OMPropertyVisualizer<MeshT>::template duplicateProperty_stage1<double>();
}

