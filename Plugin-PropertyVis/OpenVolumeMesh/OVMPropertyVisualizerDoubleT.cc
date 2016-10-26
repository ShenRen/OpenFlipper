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

#ifdef ENABLE_OPENVOLUMEMESH_SUPPORT

#define OVM_PROPERTY_VISUALIZER_DOUBLE_CC

#include "OVMPropertyVisualizerDouble.hh"

#include <ACG/Utils/IColorCoder.hh>
#include <ACG/Utils/LinearTwoColorCoder.hh>
#include <ACG/Utils/ColorConversion.hh>

#include <QObject>

template <typename MeshT>
OVMPropertyVisualizerDouble<MeshT>::OVMPropertyVisualizerDouble(MeshT* _mesh, int objectID, PropertyInfo _propertyInfo)
    : OVMPropertyVisualizer<MeshT>(_mesh, objectID, _propertyInfo)
{
    if (PropertyVisualizer::widget) delete PropertyVisualizer::widget;
    DoubleWidget* w = new DoubleWidget();
    w->paramDouble->setTitle(QString("Double Parameters of ").append(PropertyVisualizer::propertyInfo.propName().c_str()));
    PropertyVisualizer::widget = w;

    this->connect(w->computeHistogramButton, &QPushButton::clicked,
                  [this, w](){this->template showHistogram<double>(w->histogram);});

}

template <typename MeshT>
template <typename PropType, typename EntityIterator>
void OVMPropertyVisualizerDouble<MeshT>::visualizeProp(PropType prop, EntityIterator e_begin, EntityIterator e_end)
{
    if (!prop) return;

    DoubleWidget* doubleWidget = static_cast<DoubleWidget*>(PropertyVisualizer::widget);
    ACG::Vec4f colorMin = ACG::to_Vec4f(doubleWidget->doubleMin->color());

    ACG::IColorCoder *cc = doubleWidget->buildColorCoder();
    double min, max;

    if ( doubleWidget->doubleAbsolute->isChecked() ){
        min = FLT_MAX;
        max = 0.0;
    } else {
        min = FLT_MAX;
        max = FLT_MIN;
    }

    for (EntityIterator e_it = e_begin; e_it != e_end; ++e_it){
        double value = prop[*e_it];
        if ( doubleWidget->doubleAbsolute->isChecked() ){
            min = std::min( min, fabs(value));
            max = std::max( max, fabs(value));
        } else {
            min = std::min( min, value);
            max = std::max( max, value);
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

    VolumeMeshObject<MeshT>* object;
    PluginFunctions::getObject(OVMPropertyVisualizer<MeshT>::mObjectID, object);
    for (EntityIterator e_it = e_begin; e_it != e_end; ++e_it){

        if (range == 0.0)
            object->colors()[*e_it] = colorMin;
        else {

            double value = prop[*e_it];

            // absolut value?
            if ( doubleWidget->doubleAbsolute->isChecked())
                value = fabs(value);

            // clamping
            value = std::max(min,value);
            value = std::min(max,value);

            double t = (value-min)/range;

            ACG::Vec4f color = cc->color_float4(t);

            if (doubleWidget->doubleMapOutsideRange->isChecked()) {
              if (prop[*e_it] < min || prop[*e_it] > max)
                color[3] = 0.f;
            }

            // set color
            object->colors()[*e_it] = color;
        }
    }
    delete cc;
}
CALLS_TO_VISUALIZE_PROP(OVMPropertyVisualizerDouble<MeshT>, typename MeshT, double)

template <typename MeshT>
void OVMPropertyVisualizerDouble<MeshT>::duplicateProperty()
{
    OVMPropertyVisualizer<MeshT>::template duplicateProperty_stage1<double>();
}

template <typename MeshT>
QString OVMPropertyVisualizerDouble<MeshT>::getPropertyText(unsigned int index)
{
    return OVMPropertyVisualizer<MeshT>::template getPropertyText_<double>(index);
}

template <typename MeshT>
void OVMPropertyVisualizerDouble<MeshT>::setCellPropertyFromText(unsigned int index, QString text)
{
    MeshT* mesh = OVMPropertyVisualizer<MeshT>::mesh;

    OpenVolumeMesh::CellPropertyT<double> prop = mesh->template request_cell_property<double>(OVMPropertyVisualizer<MeshT>::propertyInfo.propName());
    if ( !prop )
    {
        emit this->log(LOGERR, QObject::tr("Error: No property with name ").append(PropertyVisualizer::propertyInfo.propName().c_str()));
        return;
    }

    OpenVolumeMesh::CellHandle ch(index);

    prop[ch] = this->strToDouble(text);
}

template <typename MeshT>
void OVMPropertyVisualizerDouble<MeshT>::setFacePropertyFromText(unsigned int index, QString text)
{
    MeshT* mesh = OVMPropertyVisualizer<MeshT>::mesh;

    OpenVolumeMesh::FacePropertyT<double> prop = mesh->template request_face_property<double>(OVMPropertyVisualizer<MeshT>::propertyInfo.propName());
    if ( !prop )
    {
        emit this->log(LOGERR, QObject::tr("Error: No property with name ").append(PropertyVisualizer::propertyInfo.propName().c_str()));
        return;
    }

    OpenVolumeMesh::FaceHandle fh(index);

    prop[fh] = this->strToDouble(text);
}

template <typename MeshT>
void OVMPropertyVisualizerDouble<MeshT>::setHalffacePropertyFromText(unsigned int index, QString text)
{
    MeshT* mesh = OVMPropertyVisualizer<MeshT>::mesh;

    OpenVolumeMesh::HalfFacePropertyT<double> prop = mesh->template request_halfface_property<double>(OVMPropertyVisualizer<MeshT>::propertyInfo.propName());
    if ( !prop )
    {
        emit this->log(LOGERR, QObject::tr("Error: No property with name ").append(PropertyVisualizer::propertyInfo.propName().c_str()));
        return;
    }

    OpenVolumeMesh::HalfFaceHandle hfh(index);

    prop[hfh] = this->strToDouble(text);
}

template <typename MeshT>
void OVMPropertyVisualizerDouble<MeshT>::setEdgePropertyFromText(unsigned int index, QString text)
{
    MeshT* mesh = OVMPropertyVisualizer<MeshT>::mesh;

    OpenVolumeMesh::EdgePropertyT<double> prop = mesh->template request_edge_property<double>(OVMPropertyVisualizer<MeshT>::propertyInfo.propName());
    if ( !prop )
    {
        emit this->log(LOGERR, QObject::tr("Error: No property with name ").append(PropertyVisualizer::propertyInfo.propName().c_str()));
        return;
    }

    OpenVolumeMesh::EdgeHandle eh(index);

    prop[eh] = this->strToDouble(text);
}

template <typename MeshT>
void OVMPropertyVisualizerDouble<MeshT>::setHalfedgePropertyFromText(unsigned int index, QString text)
{
    MeshT* mesh = OVMPropertyVisualizer<MeshT>::mesh;

    OpenVolumeMesh::HalfEdgePropertyT<double> prop = mesh->template request_halfedge_property<double>(OVMPropertyVisualizer<MeshT>::propertyInfo.propName());
    if ( !prop )
    {
        emit this->log(LOGERR, QObject::tr("Error: No property with name ").append(PropertyVisualizer::propertyInfo.propName().c_str()));
        return;
    }

    OpenVolumeMesh::HalfEdgeHandle heh(index);

    prop[heh] = this->strToDouble(text);
}

template <typename MeshT>
void OVMPropertyVisualizerDouble<MeshT>::setVertexPropertyFromText(unsigned int index, QString text)
{
    MeshT* mesh = OVMPropertyVisualizer<MeshT>::mesh;

    OpenVolumeMesh::VertexPropertyT<double> prop = mesh->template request_vertex_property<double>(OVMPropertyVisualizer<MeshT>::propertyInfo.propName());
    if ( !prop )
    {
        emit this->log(LOGERR, QObject::tr("Error: No property with name ").append(PropertyVisualizer::propertyInfo.propName().c_str()));
        return;
    }

    OpenVolumeMesh::VertexHandle vh(index);

    prop[vh] = this->strToDouble(text);
}

template <typename MeshT>
ACG::IColorCoder *OVMPropertyVisualizerDouble<MeshT>::buildColorCoder()
{
    DoubleWidget* doubleWidget = static_cast<DoubleWidget*>(PropertyVisualizer::widget);
    return doubleWidget->buildColorCoder();
}

#endif /* ENABLE_OPENVOLUMEMESH_SUPPORT */
