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

#define OM_PROPERTY_VISUALIZER_INTEGER_CC

#include "OMPropertyVisualizerInteger.hh"

template <typename MeshT, typename T>
OMPropertyVisualizerInteger<MeshT, T>::OMPropertyVisualizerInteger(MeshT* _mesh, PropertyInfo _propertyInfo, bool isUnsigned)
    : OMPropertyVisualizer<MeshT>(_mesh, _propertyInfo)
{
    if (PropertyVisualizer::widget) delete PropertyVisualizer::widget;
    IntegerWidget* w = new IntegerWidget();
    w->paramInt->setTitle(QString("Integer Parameters of ").append(PropertyVisualizer::propertyInfo.propName().c_str()));
    PropertyVisualizer::widget = w;

    if (isUnsigned)
    {
        w->intAbsolute->setChecked(false); //because we already have unsigned integers wo don't have to calculate their absolute value
        w->intAbsolute->setCheckable(false);
    }
}

template <typename MeshT,typename T>
QString OMPropertyVisualizerInteger<MeshT, T>::getPropertyText(unsigned int index)
{
    return OMPropertyVisualizer<MeshT>::template getPropertyText_<int>(index);
}

template <typename MeshT, typename T>
void OMPropertyVisualizerInteger<MeshT, T>::visualizeFaceProp(bool _setDrawMode)
{
    IntegerWidget* integerWidget = static_cast<IntegerWidget*>(PropertyVisualizer::widget);
    typename MeshT::Color colorMin, colorMax;

    colorMin = OMPropertyVisualizer<MeshT>::convertColor(integerWidget->intMin->color());
    colorMax = OMPropertyVisualizer<MeshT>::convertColor(integerWidget->intMax->color());

    // color coder in [0,1]
    ACG::ColorCoder cc;

    std::map< int, typename MeshT::Color> randomColor;

    if ( integerWidget->intRandom->isChecked() && integerWidget->intMapBlack->isChecked() )
        randomColor[ integerWidget->intMapBlackValue->value() ] = typename MeshT::Color(0.0, 0.0, 0.0,  1.0);

    //TODO check if this also works if the property is Vec3d
    OpenMesh::FPropHandleT< T > prop;

    if ( !OMPropertyVisualizer<MeshT>::mesh->get_property_handle(prop, OMPropertyVisualizer<MeshT>::propertyInfo.propName() ) )
        return;

    T max = std::numeric_limits<T>::min();
    T min = std::numeric_limits<T>::max();

    for (typename MeshT::FaceIter f_it = OMPropertyVisualizer<MeshT>::mesh->faces_begin() ; f_it != OMPropertyVisualizer<MeshT>::mesh->faces_end() ; ++f_it){
        min = std::min( min, getValue(prop, f_it));
        max = std::max( max, getValue(prop, f_it));
    }

    // fixed range?
    if( integerWidget->intFixedRange->isChecked())
    {
        min = integerWidget->intFixedRangeMin->value();
        max = integerWidget->intFixedRangeMax->value();
    }
    else
    {
        integerWidget->intFixedRangeMin->setValue(min);
        integerWidget->intFixedRangeMax->setValue(max);
    }

    unsigned int range = max - min;

    if ( ! OMPropertyVisualizer<MeshT>::mesh->has_face_colors() )
        OMPropertyVisualizer<MeshT>::mesh->request_face_colors();

    for (typename MeshT::FaceIter f_it = OMPropertyVisualizer<MeshT>::mesh->faces_begin() ; f_it != OMPropertyVisualizer<MeshT>::mesh->faces_end() ; ++f_it){

        if (range == 0)
            OMPropertyVisualizer<MeshT>::mesh->set_color(*f_it, colorMin);
        else {

            double pos = (getValue(prop, f_it) - min) / (double) range;

            typename MeshT::Color color;

            if (integerWidget->intColorCoder->isChecked())
            {
                color = cc.color_float4(pos);
            }
            else if ( !integerWidget->intRandom->isChecked() ){

                color[0] = colorMin[0] * (1-pos) + pos * colorMax[0];
                color[1] = colorMin[1] * (1-pos) + pos * colorMax[1];
                color[2] = colorMin[2] * (1-pos) + pos * colorMax[2];
                color[3] = 1.0;

            } else {

                if ( randomColor.find( getValue(prop, f_it) ) == randomColor.end() ){

                    color = mColorGenerator.generateNextColor();
                    color[3] = 1.0;

                    randomColor[ getValue(prop, f_it) ] = color;
                }

                color = randomColor[ getValue(prop, f_it) ];
            }

            OMPropertyVisualizer<MeshT>::mesh->set_color(*f_it, color);
        }
    }

    if (_setDrawMode)
        PluginFunctions::setDrawMode(ACG::SceneGraph::DrawModes::SOLID_FACES_COLORED);
}

template <typename MeshT, typename T>
void OMPropertyVisualizerInteger<MeshT, T>::visualizeEdgeProp(bool _setDrawMode)
{
    IntegerWidget* integerWidget = static_cast<IntegerWidget*>(PropertyVisualizer::widget);
    typename MeshT::Color colorMin, colorMax;

    colorMin = OMPropertyVisualizer<MeshT>::convertColor(integerWidget->intMin->color());
    colorMax = OMPropertyVisualizer<MeshT>::convertColor(integerWidget->intMax->color());

    // color coder in [0,1]
    ACG::ColorCoder cc;

    std::map< int, typename MeshT::Color> randomColor;

    if ( integerWidget->intRandom->isChecked() && integerWidget->intMapBlack->isChecked() )
        randomColor[ integerWidget->intMapBlackValue->value() ] = typename MeshT::Color(0.0, 0.0, 0.0,  1.0);

    //TODO check if this also works if the property is Vec3d
    OpenMesh::EPropHandleT< T > prop;

    if ( !OMPropertyVisualizer<MeshT>::mesh->get_property_handle(prop, OMPropertyVisualizer<MeshT>::propertyInfo.propName() ) )
        return;

    T max = std::numeric_limits<T>::min();
    T min = std::numeric_limits<T>::max();

    for (typename MeshT::EdgeIter e_it = OMPropertyVisualizer<MeshT>::mesh->edges_begin() ; e_it != OMPropertyVisualizer<MeshT>::mesh->edges_end() ; ++e_it){
        min = std::min( min, getValue(prop, e_it));
        max = std::max( max, getValue(prop, e_it));
    }

    // fixed range?
    if( integerWidget->intFixedRange->isChecked())
    {
        min = integerWidget->intFixedRangeMin->value();
        max = integerWidget->intFixedRangeMax->value();
    }
    else
    {
        integerWidget->intFixedRangeMin->setValue(min);
        integerWidget->intFixedRangeMax->setValue(max);
    }

    unsigned int range = max - min;

    if ( ! OMPropertyVisualizer<MeshT>::mesh->has_edge_colors() )
        OMPropertyVisualizer<MeshT>::mesh->request_edge_colors();

    for (typename MeshT::EdgeIter e_it = OMPropertyVisualizer<MeshT>::mesh->edges_begin() ; e_it != OMPropertyVisualizer<MeshT>::mesh->edges_end() ; ++e_it){

        if (range == 0)
            OMPropertyVisualizer<MeshT>::mesh->set_color(*e_it, colorMin);
        else {

            double pos = (getValue(prop, e_it) - min) / (double) range;

            typename MeshT::Color color;

            if (integerWidget->intColorCoder->isChecked())
            {
                color = cc.color_float4(pos);
            }
            else if ( !integerWidget->intRandom->isChecked() ){

                color[0] = colorMin[0] * (1-pos) + pos * colorMax[0];
                color[1] = colorMin[1] * (1-pos) + pos * colorMax[1];
                color[2] = colorMin[2] * (1-pos) + pos * colorMax[2];
                color[3] = 1.0;

            } else {

                if ( randomColor.find( getValue(prop, e_it) ) == randomColor.end() ){

                    color = mColorGenerator.generateNextColor();
                    color[3] = 1.0;

                    randomColor[ getValue(prop, e_it) ] = color;
                }

                color = randomColor[ getValue(prop, e_it) ];
            }

            OMPropertyVisualizer<MeshT>::mesh->set_color(*e_it, color);
        }
    }

    if (_setDrawMode)
        PluginFunctions::setDrawMode(ACG::SceneGraph::DrawModes::EDGES_COLORED);

}

template <typename MeshT, typename T>
void OMPropertyVisualizerInteger<MeshT, T>::visualizeHalfedgeProp(bool _setDrawMode)
{
    IntegerWidget* integerWidget = static_cast<IntegerWidget*>(PropertyVisualizer::widget);
    typename MeshT::Color colorMin, colorMax;

    colorMin = OMPropertyVisualizer<MeshT>::convertColor(integerWidget->intMin->color());
    colorMax = OMPropertyVisualizer<MeshT>::convertColor(integerWidget->intMax->color());

    // color coder in [0,1]
    ACG::ColorCoder cc;

    std::map< int, typename MeshT::Color> randomColor;

    if ( integerWidget->intRandom->isChecked() && integerWidget->intMapBlack->isChecked() )
        randomColor[ integerWidget->intMapBlackValue->value() ] = typename MeshT::Color(0.0, 0.0, 0.0,  1.0);

    //TODO check if this also works if the property is Vec3d
    OpenMesh::HPropHandleT< T > prop;

    if ( !OMPropertyVisualizer<MeshT>::mesh->get_property_handle(prop, OMPropertyVisualizer<MeshT>::propertyInfo.propName() ) )
        return;

    T max = std::numeric_limits<T>::min();
    T min = std::numeric_limits<T>::max();

    for (typename MeshT::HalfedgeIter he_it = OMPropertyVisualizer<MeshT>::mesh->halfedges_begin() ; he_it != OMPropertyVisualizer<MeshT>::mesh->halfedges_end() ; ++he_it){
        min = std::min( min, getValue(prop, he_it));
        max = std::max( max, getValue(prop, he_it));
    }

    // fixed range?
    if( integerWidget->intFixedRange->isChecked())
    {
        min = integerWidget->intFixedRangeMin->value();
        max = integerWidget->intFixedRangeMax->value();
    }
    else
    {
        integerWidget->intFixedRangeMin->setValue(min);
        integerWidget->intFixedRangeMax->setValue(max);
    }

    unsigned int range = max - min;

    if ( ! OMPropertyVisualizer<MeshT>::mesh->has_halfedge_colors() )
        OMPropertyVisualizer<MeshT>::mesh->request_halfedge_colors();

    for (typename MeshT::HalfedgeIter he_it = OMPropertyVisualizer<MeshT>::mesh->halfedges_begin() ; he_it != OMPropertyVisualizer<MeshT>::mesh->halfedges_end() ; ++he_it){

        if (range == 0)
            OMPropertyVisualizer<MeshT>::mesh->set_color(*he_it, colorMin);
        else {

            double pos = (getValue(prop, he_it) - min) / (double) range;

            typename MeshT::Color color;

            if (integerWidget->intColorCoder->isChecked())
            {
                color = cc.color_float4(pos);
            }
            else if ( !integerWidget->intRandom->isChecked() ){

                color[0] = colorMin[0] * (1-pos) + pos * colorMax[0];
                color[1] = colorMin[1] * (1-pos) + pos * colorMax[1];
                color[2] = colorMin[2] * (1-pos) + pos * colorMax[2];
                color[3] = 1.0;

            } else {

                if ( randomColor.find( getValue(prop, he_it) ) == randomColor.end() ){

                    color = mColorGenerator.generateNextColor();
                    color[3] = 1.0;

                    randomColor[ getValue(prop, he_it) ] = color;
                }

                color = randomColor[ getValue(prop, he_it) ];
            }

            OMPropertyVisualizer<MeshT>::mesh->set_color(*he_it, color);
        }
    }

    if (_setDrawMode)
        PluginFunctions::setDrawMode(ACG::SceneGraph::DrawModes::HALFEDGES_COLORED);
}

template <typename MeshT, typename T>
void OMPropertyVisualizerInteger<MeshT, T>::visualizeVertexProp(bool _setDrawMode)
{
    IntegerWidget* integerWidget = static_cast<IntegerWidget*>(PropertyVisualizer::widget);
    typename MeshT::Color colorMin, colorMax;

    colorMin = OMPropertyVisualizer<MeshT>::convertColor(integerWidget->intMin->color());
    colorMax = OMPropertyVisualizer<MeshT>::convertColor(integerWidget->intMax->color());

    // color coder in [0,1]
    ACG::ColorCoder cc;

    std::map< int, typename MeshT::Color> randomColor;

    if ( integerWidget->intRandom->isChecked() && integerWidget->intMapBlack->isChecked() )
        randomColor[ integerWidget->intMapBlackValue->value() ] = typename MeshT::Color(0.0, 0.0, 0.0,  1.0);

    //TODO check if this also works if the property is Vec3d
    OpenMesh::VPropHandleT< T > prop;

    if ( !OMPropertyVisualizer<MeshT>::mesh->get_property_handle(prop, OMPropertyVisualizer<MeshT>::propertyInfo.propName() ) )
        return;

    T max = std::numeric_limits<T>::min();
    T min = std::numeric_limits<T>::max();

    for (typename MeshT::VertexIter v_it = OMPropertyVisualizer<MeshT>::mesh->vertices_begin() ; v_it != OMPropertyVisualizer<MeshT>::mesh->vertices_end() ; ++v_it){
        min = std::min( min, getValue(prop, v_it));
        max = std::max( max, getValue(prop, v_it));
    }

    // fixed range?
    if( integerWidget->intFixedRange->isChecked())
    {
        min = integerWidget->intFixedRangeMin->value();
        max = integerWidget->intFixedRangeMax->value();
    }
    else
    {
        integerWidget->intFixedRangeMin->setValue(min);
        integerWidget->intFixedRangeMax->setValue(max);
    }

    unsigned int range = max - min;

    if ( ! OMPropertyVisualizer<MeshT>::mesh->has_vertex_colors() )
        OMPropertyVisualizer<MeshT>::mesh->request_vertex_colors();

    for (typename MeshT::VertexIter v_it = OMPropertyVisualizer<MeshT>::mesh->vertices_begin() ; v_it != OMPropertyVisualizer<MeshT>::mesh->vertices_end() ; ++v_it){

        if (range == 0)
            OMPropertyVisualizer<MeshT>::mesh->set_color(*v_it, colorMin);
        else {

            double pos = (getValue(prop, v_it) - min) / (double) range;

            typename MeshT::Color color;

            if (integerWidget->intColorCoder->isChecked())
            {
                color = cc.color_float4(pos);
            }
            else if ( !integerWidget->intRandom->isChecked() ){

                color[0] = colorMin[0] * (1-pos) + pos * colorMax[0];
                color[1] = colorMin[1] * (1-pos) + pos * colorMax[1];
                color[2] = colorMin[2] * (1-pos) + pos * colorMax[2];
                color[3] = 1.0;

            }
            else
            {

                if ( randomColor.find( getValue(prop, v_it) ) == randomColor.end() ){

                    color = mColorGenerator.generateNextColor();
                    color[3] = 1.0;

                    randomColor[ getValue(prop, v_it) ] = color;
                }

                color = randomColor[ getValue(prop, v_it) ];
            }

            OMPropertyVisualizer<MeshT>::mesh->set_color(*v_it, color);
        }
    }

    if (_setDrawMode)
        PluginFunctions::setDrawMode(ACG::SceneGraph::DrawModes::SOLID_POINTS_COLORED);

}

template <typename MeshT, typename T>
void OMPropertyVisualizerInteger<MeshT, T>::setFacePropertyFromText(unsigned int index, QString text)
{
    OpenMesh::FPropHandleT< int > prop;
    MeshT* mesh = OMPropertyVisualizer<MeshT>::mesh;

    if ( !mesh->get_property_handle(prop, PropertyVisualizer::propertyInfo.propName() ) )
        emit this->log(LOGERR, QObject::tr("Error: No property with name ").append(PropertyVisualizer::propertyInfo.propName().c_str()));


    typename MeshT::FaceHandle fh = mesh->face_handle(index);

    T dummy = 0;
    mesh->property(prop, fh) = this->strToT(text, dummy);
}

template <typename MeshT, typename T>
void OMPropertyVisualizerInteger<MeshT, T>::setEdgePropertyFromText(unsigned int index, QString text)
{
    OpenMesh::EPropHandleT< int > prop;
    MeshT* mesh = OMPropertyVisualizer<MeshT>::mesh;

    if ( !mesh->get_property_handle(prop, PropertyVisualizer::propertyInfo.propName() ) )
        emit this->log(LOGERR, QObject::tr("Error: No property with name ").append(PropertyVisualizer::propertyInfo.propName().c_str()));

    typename MeshT::EdgeHandle eh = mesh->edge_handle(index);

    T dummy = 0;
    mesh->property(prop, eh) = this->strToT(text, dummy);
}

template <typename MeshT, typename T>
void OMPropertyVisualizerInteger<MeshT, T>::setHalfedgePropertyFromText(unsigned int index, QString text)
{
    OpenMesh::HPropHandleT< int > prop;
    MeshT* mesh = OMPropertyVisualizer<MeshT>::mesh;

    if ( !mesh->get_property_handle(prop, PropertyVisualizer::propertyInfo.propName() ) )
        emit this->log(LOGERR, QObject::tr("Error: No property with name ").append(PropertyVisualizer::propertyInfo.propName().c_str()));


    typename MeshT::HalfedgeHandle heh = mesh->halfedge_handle(index);

    T dummy = 0;
    mesh->property(prop, heh) = this->strToT(text, dummy);
}

template <typename MeshT, typename T>
void OMPropertyVisualizerInteger<MeshT, T>::setVertexPropertyFromText(unsigned int index, QString text)
{
    OpenMesh::VPropHandleT< int > prop;
    MeshT* mesh = OMPropertyVisualizer<MeshT>::mesh;

    if ( !mesh->get_property_handle(prop, PropertyVisualizer::propertyInfo.propName() ) )
        emit this->log(LOGERR, QObject::tr("Error: No property with name ").append(PropertyVisualizer::propertyInfo.propName().c_str()));


    typename MeshT::VertexHandle vh = mesh->vertex_handle(index);

    T dummy = 0;
    mesh->property(prop, vh) = this->strToT(text, dummy);
}

template<typename MeshT, typename T>
void OMPropertyVisualizerInteger<MeshT, T>::removeProperty()
{
    OMPropertyVisualizer<MeshT>::template removeProperty_stage1<T>();
}

template<typename MeshT, typename T>
void OMPropertyVisualizerInteger<MeshT, T>::duplicateProperty()
{
    OMPropertyVisualizer<MeshT>::template duplicateProperty_stage1<T>();
}
