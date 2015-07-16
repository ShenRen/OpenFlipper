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

#define OVM_PROPERTY_VISUALIZER_BOOLEAN_CC

#include "OVMPropertyVisualizerBoolean.hh"

template <typename MeshT>
OVMPropertyVisualizerBoolean<MeshT>::OVMPropertyVisualizerBoolean(MeshT* _mesh, int objectID, PropertyInfo _propertyInfo)
    : OVMPropertyVisualizer<MeshT>(_mesh, objectID, _propertyInfo)
{
    if (PropertyVisualizer::widget) delete PropertyVisualizer::widget;
    BooleanWidget* w = new BooleanWidget();
    w->paramBool->setTitle(QString("Boolean Parameters of ").append(PropertyVisualizer::propertyInfo.propName().c_str()));
    PropertyVisualizer::widget = w;

}

template <typename MeshT>
template <typename PropType, typename EntityIterator>
void OVMPropertyVisualizerBoolean<MeshT>::visualizeProp(PropType prop, EntityIterator e_begin, EntityIterator e_end)
{
    if (!prop)
        return;

    BooleanWidget* booleanWidget = static_cast<BooleanWidget*>(PropertyVisualizer::widget);
    ACG::Vec4f colorTrue, colorFalse;

    colorTrue  = OVMPropertyVisualizer<MeshT>::convertColor(booleanWidget->colorTrue->color());
    colorFalse = OVMPropertyVisualizer<MeshT>::convertColor(booleanWidget->colorFalse->color());

    VolumeMeshObject<MeshT>* object;
    PluginFunctions::getObject(OVMPropertyVisualizer<MeshT>::mObjectID, object);

    for (EntityIterator e_it = e_begin ; e_it != e_end; ++e_it)
        if ( prop[*e_it] )
            object->colors()[*e_it] = colorTrue;
        else
            object->colors()[*e_it] = colorFalse;
}
CALLS_TO_VISUALIZE_PROP(OVMPropertyVisualizerBoolean<MeshT>, typename MeshT, bool)

template <typename MeshT>
void OVMPropertyVisualizerBoolean<MeshT>::duplicateProperty()
{
    OVMPropertyVisualizer<MeshT>::template duplicateProperty_stage1<bool>();
}

template <typename MeshT>
void OVMPropertyVisualizerBoolean<MeshT>::setCellPropertyFromText(unsigned int index, QString text)
{
    MeshT* mesh = OVMPropertyVisualizer<MeshT>::mesh;

    OpenVolumeMesh::CellPropertyT<bool> prop = mesh->template request_cell_property<bool>(OVMPropertyVisualizer<MeshT>::propertyInfo.propName());
    if ( !prop )
    {
        emit this->log(LOGERR, QObject::tr("Error: No property with name ").append(PropertyVisualizer::propertyInfo.propName().c_str()));
        return;
    }

    OpenVolumeMesh::CellHandle ch(index);

    prop[ch] = this->strToBool(text);
}

template <typename MeshT>
void OVMPropertyVisualizerBoolean<MeshT>::setFacePropertyFromText(unsigned int index, QString text)
{
    MeshT* mesh = OVMPropertyVisualizer<MeshT>::mesh;

    OpenVolumeMesh::FacePropertyT<bool> prop = mesh->template request_face_property<bool>(OVMPropertyVisualizer<MeshT>::propertyInfo.propName());
    if ( !prop )
    {
        emit this->log(LOGERR, QObject::tr("Error: No property with name ").append(PropertyVisualizer::propertyInfo.propName().c_str()));
        return;
    }

    OpenVolumeMesh::FaceHandle fh(index);

    prop[fh] = this->strToBool(text);
}

template <typename MeshT>
void OVMPropertyVisualizerBoolean<MeshT>::setHalffacePropertyFromText(unsigned int index, QString text)
{
    MeshT* mesh = OVMPropertyVisualizer<MeshT>::mesh;

    OpenVolumeMesh::HalfFacePropertyT<bool> prop = mesh->template request_halfface_property<bool>(OVMPropertyVisualizer<MeshT>::propertyInfo.propName());
    if ( !prop )
    {
        emit this->log(LOGERR, QObject::tr("Error: No property with name ").append(PropertyVisualizer::propertyInfo.propName().c_str()));
        return;
    }

    OpenVolumeMesh::HalfFaceHandle hfh(index);

    prop[hfh] = this->strToBool(text);
}

template <typename MeshT>
void OVMPropertyVisualizerBoolean<MeshT>::setEdgePropertyFromText(unsigned int index, QString text)
{
    MeshT* mesh = OVMPropertyVisualizer<MeshT>::mesh;

    OpenVolumeMesh::EdgePropertyT<bool> prop = mesh->template request_edge_property<bool>(OVMPropertyVisualizer<MeshT>::propertyInfo.propName());
    if ( !prop )
    {
        emit this->log(LOGERR, QObject::tr("Error: No property with name ").append(PropertyVisualizer::propertyInfo.propName().c_str()));
        return;
    }

    OpenVolumeMesh::EdgeHandle eh(index);

    prop[eh] = this->strToBool(text);
}

template <typename MeshT>
void OVMPropertyVisualizerBoolean<MeshT>::setHalfedgePropertyFromText(unsigned int index, QString text)
{
    MeshT* mesh = OVMPropertyVisualizer<MeshT>::mesh;

    OpenVolumeMesh::HalfEdgePropertyT<bool> prop = mesh->template request_halfedge_property<bool>(OVMPropertyVisualizer<MeshT>::propertyInfo.propName());
    if ( !prop )
    {
        emit this->log(LOGERR, QObject::tr("Error: No property with name ").append(PropertyVisualizer::propertyInfo.propName().c_str()));
        return;
    }

    OpenVolumeMesh::HalfEdgeHandle heh(index);

    prop[heh] = this->strToBool(text);
}

template <typename MeshT>
void OVMPropertyVisualizerBoolean<MeshT>::setVertexPropertyFromText(unsigned int index, QString text)
{
    MeshT* mesh = OVMPropertyVisualizer<MeshT>::mesh;

    OpenVolumeMesh::VertexPropertyT<bool> prop = mesh->template request_vertex_property<bool>(OVMPropertyVisualizer<MeshT>::propertyInfo.propName());
    if ( !prop )
    {
        emit this->log(LOGERR, QObject::tr("Error: No property with name ").append(PropertyVisualizer::propertyInfo.propName().c_str()));
        return;
    }

    OpenVolumeMesh::VertexHandle vh(index);

    prop[vh] = this->strToBool(text);
}


template <typename MeshT>
QString OVMPropertyVisualizerBoolean<MeshT>::getPropertyText(unsigned int index)
{
    return OVMPropertyVisualizer<MeshT>::template getPropertyText_<bool>(index);
}

#endif /* ENABLE_OPENVOLUMEMESH_SUPPORT */
