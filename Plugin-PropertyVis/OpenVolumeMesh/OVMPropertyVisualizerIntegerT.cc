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

#define OVM_PROPERTY_VISUALIZER_ITEGER_CC

#include "OVMPropertyVisualizerInteger.hh"
#include <ACG/Utils/ColorConversion.hh>
#include <ACG/Utils/LinearTwoColorCoder.hh>

template <typename MeshT, typename T>
OVMPropertyVisualizerInteger<MeshT,T>::OVMPropertyVisualizerInteger(MeshT* _mesh, int objectID,  PropertyInfo _propertyInfo, bool isUnsigned)
    : OVMPropertyVisualizer<MeshT>(_mesh, objectID, _propertyInfo)
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


    mNumericLimitMax = std::numeric_limits<T>::max();
    mNumericLimitMin = std::numeric_limits<T>::min();

}

template <typename MeshT, typename T>
template <typename PropType, typename EntityIterator>
void OVMPropertyVisualizerInteger<MeshT, T>::visualizeProp(PropType prop, EntityIterator e_begin, EntityIterator e_end)
{
    if (!prop) return;

    IntegerWidget* integerWidget = static_cast<IntegerWidget*>(PropertyVisualizer::widget);

    ACG::Vec4f colorMin = ACG::to_Vec4f(integerWidget->intMin->color());

    std::map< int, ACG::Vec4f> randomColor;

    if ( integerWidget->intRandom->isChecked() && integerWidget->intMapBlack->isChecked() )
        randomColor[ integerWidget->intMapBlackValue->value() ] = ACG::Vec4f(0.0, 0.0, 0.0, 0.0);

    T min = mNumericLimitMax;
    T max = mNumericLimitMin;

    for (EntityIterator e_it = e_begin; e_it != e_end; ++e_it)
    {
        T value = prop[*e_it];
        min = std::min( min, value);
        max = std::max( max, value);
    }

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

    auto cc = integerWidget->buildColorCoder();

    unsigned int range = max - min;
    VolumeMeshObject<MeshT>* object;
    PluginFunctions::getObject(OVMPropertyVisualizer<MeshT>::mObjectID, object);

    for (EntityIterator e_it = e_begin; e_it != e_end; ++e_it)
    {
        if (range == 0)
            object->colors()[*e_it] = colorMin;
        else {
            T value = prop[*e_it];
            double pos = (value - min) / (double) range;
            ACG::Vec4f color;
            if ( integerWidget->intRandom->isChecked() )
            {
                // TODO: build appropriate subclass of IColorCoder for this purpose
                if ( randomColor.find( value ) == randomColor.end() )
                {
                    color = mColorGenerator.generateNextColor();
                    color[3] = 1.0;
                    randomColor[ value ] = color;
                }
                color = randomColor[ value ];
            }
            else
            {
                color = cc->color_float4(pos);
            }

            object->colors()[*e_it] = color;
        }
    }
}
#define KOMMA ,
CALLS_TO_VISUALIZE_PROP(OVMPropertyVisualizerInteger<MeshT KOMMA T>, typename MeshT KOMMA typename T, T)
#undef KOMMA

template <typename MeshT, typename T>
void OVMPropertyVisualizerInteger<MeshT, T>::duplicateProperty()
{
    OVMPropertyVisualizer<MeshT>::template duplicateProperty_stage1<T>();
}

template <typename MeshT, typename T>
QString OVMPropertyVisualizerInteger<MeshT, T>::getPropertyText(unsigned int index)
{
    return OVMPropertyVisualizer<MeshT>::template getPropertyText_<T>(index);
}

template <typename MeshT, typename T>
void OVMPropertyVisualizerInteger<MeshT, T>::setCellPropertyFromText(unsigned int index, QString text)
{
    MeshT* mesh = OVMPropertyVisualizer<MeshT>::mesh;

    OpenVolumeMesh::CellPropertyT<int> prop = mesh->template request_cell_property<int>(OVMPropertyVisualizer<MeshT>::propertyInfo.propName());
    if ( !prop )
    {
        emit this->log(LOGERR, QObject::tr("Error: No property with name ").append(PropertyVisualizer::propertyInfo.propName().c_str()));
        return;
    }

    OpenVolumeMesh::CellHandle ch(index);

    prop[ch] = this->strToInt(text);
}

template <typename MeshT, typename T>
void OVMPropertyVisualizerInteger<MeshT, T>::setFacePropertyFromText(unsigned int index, QString text)
{
    MeshT* mesh = OVMPropertyVisualizer<MeshT>::mesh;

    OpenVolumeMesh::FacePropertyT<int> prop = mesh->template request_face_property<int>(OVMPropertyVisualizer<MeshT>::propertyInfo.propName());
    if ( !prop )
    {
        emit this->log(LOGERR, QObject::tr("Error: No property with name ").append(PropertyVisualizer::propertyInfo.propName().c_str()));
        return;
    }

    OpenVolumeMesh::FaceHandle fh(index);

    prop[fh] = this->strToInt(text);
}

template <typename MeshT, typename T>
void OVMPropertyVisualizerInteger<MeshT, T>::setHalffacePropertyFromText(unsigned int index, QString text)
{
    MeshT* mesh = OVMPropertyVisualizer<MeshT>::mesh;

    OpenVolumeMesh::HalfFacePropertyT<int> prop = mesh->template request_halfface_property<int>(OVMPropertyVisualizer<MeshT>::propertyInfo.propName());
    if ( !prop )
    {
        emit this->log(LOGERR, QObject::tr("Error: No property with name ").append(PropertyVisualizer::propertyInfo.propName().c_str()));
        return;
    }

    OpenVolumeMesh::HalfFaceHandle hfh(index);

    prop[hfh] = this->strToInt(text);
}

template <typename MeshT, typename T>
void OVMPropertyVisualizerInteger<MeshT, T>::setEdgePropertyFromText(unsigned int index, QString text)
{
    MeshT* mesh = OVMPropertyVisualizer<MeshT>::mesh;

    OpenVolumeMesh::EdgePropertyT<int> prop = mesh->template request_edge_property<int>(OVMPropertyVisualizer<MeshT>::propertyInfo.propName());
    if ( !prop )
    {
        emit this->log(LOGERR, QObject::tr("Error: No property with name ").append(PropertyVisualizer::propertyInfo.propName().c_str()));
        return;
    }

    OpenVolumeMesh::EdgeHandle eh(index);

    prop[eh] = this->strToInt(text);
}

template <typename MeshT, typename T>
void OVMPropertyVisualizerInteger<MeshT, T>::setHalfedgePropertyFromText(unsigned int index, QString text)
{
    MeshT* mesh = OVMPropertyVisualizer<MeshT>::mesh;

    OpenVolumeMesh::HalfEdgePropertyT<int> prop = mesh->template request_halfedge_property<int>(OVMPropertyVisualizer<MeshT>::propertyInfo.propName());
    if ( !prop )
    {
        emit this->log(LOGERR, QObject::tr("Error: No property with name ").append(PropertyVisualizer::propertyInfo.propName().c_str()));
        return;
    }

    OpenVolumeMesh::HalfEdgeHandle heh(index);

    prop[heh] = this->strToInt(text);
}

template <typename MeshT, typename T>
void OVMPropertyVisualizerInteger<MeshT, T>::setVertexPropertyFromText(unsigned int index, QString text)
{
    MeshT* mesh = OVMPropertyVisualizer<MeshT>::mesh;

    OpenVolumeMesh::VertexPropertyT<int> prop = mesh->template request_vertex_property<int>(OVMPropertyVisualizer<MeshT>::propertyInfo.propName());
    if ( !prop )
    {
        emit this->log(LOGERR, QObject::tr("Error: No property with name ").append(PropertyVisualizer::propertyInfo.propName().c_str()));
        return;
    }

    OpenVolumeMesh::VertexHandle vh(index);

    prop[vh] = this->strToInt(text);
}

template <typename MeshT, typename T>
std::unique_ptr<ACG::IColorCoder> OVMPropertyVisualizerInteger<MeshT, T>::buildColorCoder()
{
    IntegerWidget* integerWidget = static_cast<IntegerWidget*>(PropertyVisualizer::widget);
    return integerWidget->buildColorCoder();
}

