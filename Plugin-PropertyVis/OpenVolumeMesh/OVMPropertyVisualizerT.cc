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

#define OVM_PROPERTY_VISUALIZER_CC

#ifdef ENABLE_OPENVOLUMEMESH_POLYHEDRAL_SUPPORT
#include <ObjectTypes/PolyhedralMesh/PolyhedralMesh.hh>
#endif
#ifdef ENABLE_OPENVOLUMEMESH_HEXAHEDRAL_SUPPORT
#include <ObjectTypes/HexahedralMesh/HexahedralMesh.hh>
#endif

#include "OVMPropertyVisualizer.hh"

template <typename MeshT>
template <typename InnerType>
QString OVMPropertyVisualizer<MeshT>::getPropertyText_(unsigned int index)
{
    if (PropertyVisualizer::propertyInfo.isCellProp())
    {
        OpenVolumeMesh::CellPropertyT<InnerType> prop = OVMPropertyVisualizer<MeshT>::mesh->template request_cell_property<InnerType>(OVMPropertyVisualizer<MeshT>::propertyInfo.propName());
        return PropertyVisualizer::toStr(prop[OpenVolumeMesh::CellHandle(index)]);
    }
    else if (PropertyVisualizer::propertyInfo.isFaceProp())
    {
        OpenVolumeMesh::FacePropertyT<InnerType> prop = OVMPropertyVisualizer<MeshT>::mesh->template request_face_property<InnerType>(OVMPropertyVisualizer<MeshT>::propertyInfo.propName());
        return PropertyVisualizer::toStr(prop[OpenVolumeMesh::FaceHandle(index)]);
    }
    else if (PropertyVisualizer::propertyInfo.isHalffaceProp())
    {
        OpenVolumeMesh::HalfFacePropertyT<InnerType> prop = OVMPropertyVisualizer<MeshT>::mesh->template request_halfface_property<InnerType>(OVMPropertyVisualizer<MeshT>::propertyInfo.propName());
        return PropertyVisualizer::toStr(prop[OpenVolumeMesh::HalfFaceHandle(index)]);
    }
    else if (PropertyVisualizer::propertyInfo.isEdgeProp())
    {
        OpenVolumeMesh::EdgePropertyT<InnerType> prop = OVMPropertyVisualizer<MeshT>::mesh->template request_edge_property<InnerType>(OVMPropertyVisualizer<MeshT>::propertyInfo.propName());
        return PropertyVisualizer::toStr(prop[OpenVolumeMesh::EdgeHandle(index)]);
    }
    else if (PropertyVisualizer::propertyInfo.isHalfedgeProp())
    {
        OpenVolumeMesh::HalfEdgePropertyT<InnerType> prop = OVMPropertyVisualizer<MeshT>::mesh->template request_halfedge_property<InnerType>(OVMPropertyVisualizer<MeshT>::propertyInfo.propName());
        return PropertyVisualizer::toStr(prop[OpenVolumeMesh::HalfEdgeHandle(index)]);
    }
    else //if (propertyInfo.isVertexProp())
    {
        OpenVolumeMesh::VertexPropertyT<InnerType> prop = OVMPropertyVisualizer<MeshT>::mesh->template request_vertex_property<InnerType>(OVMPropertyVisualizer<MeshT>::propertyInfo.propName());
        return PropertyVisualizer::toStr(prop[OpenVolumeMesh::VertexHandle(index)]);
    }
}

template <typename MeshT>
void OVMPropertyVisualizer<MeshT>::setPropertyFromText(unsigned int index, QString text)
{
    if (propertyInfo.isCellProp())
        setCellPropertyFromText(index, text);
    else if (propertyInfo.isFaceProp())
        setFacePropertyFromText(index, text);
    else if (propertyInfo.isHalffaceProp())
        setHalffacePropertyFromText(index, text);
    else if (propertyInfo.isEdgeProp())
        setEdgePropertyFromText(index, text);
    else if (propertyInfo.isHalfedgeProp())
        setHalfedgePropertyFromText(index, text);
    else //if (propertyInfo.isVertexProp())
        setVertexPropertyFromText(index, text);
}

template <typename MeshT>
int OVMPropertyVisualizer<MeshT>::getEntityCount()
{
    if (propertyInfo.isCellProp())
        return mesh->n_cells();
    if (propertyInfo.isFaceProp())
        return mesh->n_faces();
    if (propertyInfo.isHalffaceProp())
        return mesh->n_halffaces();
    else if (propertyInfo.isEdgeProp())
        return mesh->n_edges();
    else if (propertyInfo.isHalfedgeProp())
        return mesh->n_halfedges();
    else //if (propertyInfo.isVertexProp())
        return mesh->n_vertices();
}

template <typename MeshT>
QString OVMPropertyVisualizer<MeshT>::getHeader()
{
    //Header: headerVersion, numberOfEntities, typeOfEntites, typeOfProperty, propertyName

    QString header = QObject::tr("1"); //version
    header.append(QObject::tr(", %1").arg(getEntityCount())); //number of entities
    header.append(QObject::tr(", %1").arg(propertyInfo.entityType())); //type of entities
    header.append(", ").append(propertyInfo.friendlyTypeName()); //type of property
    header.append(", ").append(propertyInfo.propName().c_str()); // name of property
    return header;
}

template <typename MeshT>
unsigned int OVMPropertyVisualizer<MeshT>::getClosestPrimitiveId(unsigned int _face, ACG::Vec3d& _hitPoint)
{
    if (propertyInfo.isHalffaceProp())
        return getClosestHalffaceId(_face, _hitPoint);
    else// if (propertyInfo.isHalfedgeProp())
        return getClosestHalfedgeId(_face, _hitPoint);
}

template <typename MeshT>
unsigned int OVMPropertyVisualizer<MeshT>::getClosestHalffaceId(unsigned int _face, ACG::Vec3d& _hitPoint)
{
    ACG::Vec3d direction = PluginFunctions::viewingDirection();

    OpenVolumeMesh::HalfFaceHandle hfh = mesh->halfface_handle(OpenVolumeMesh::FaceHandle(_face), 0);
    OpenVolumeMesh::HalfFaceVertexIter hfv_it = mesh->hfv_iter(hfh);
    ACG::Vec3d p1 = mesh->vertex(*(hfv_it+0));
    ACG::Vec3d p2 = mesh->vertex(*(hfv_it+1));
    ACG::Vec3d p3 = mesh->vertex(*(hfv_it+2));

    ACG::Vec3d normal = (p2-p1)%(p3-p1);

    if ((direction | normal) < 0)
        return hfh.idx();
    else
        return mesh->halfface_handle(OpenVolumeMesh::FaceHandle(_face), 1).idx();
}

template <typename MeshT>
unsigned int OVMPropertyVisualizer<MeshT>::getClosestHalfedgeId(unsigned int _face, ACG::Vec3d& _hitPoint)
{
    unsigned int halfface = getClosestHalffaceId(_face, _hitPoint);

    OpenVolumeMesh::OpenVolumeMeshFace face = mesh->halfface(halfface);

    const std::vector<OpenVolumeMesh::HalfEdgeHandle> & halfedges = face.halfedges();

    double min_distance = DBL_MAX;
    OpenVolumeMesh::HalfEdgeHandle closestHalfEdgeHandle;

    for (std::vector<OpenVolumeMesh::HalfEdgeHandle>::const_iterator he_it = halfedges.begin(); he_it != halfedges.end(); ++he_it)
    {
        OpenVolumeMesh::OpenVolumeMeshEdge edge = OVMPropertyVisualizer<MeshT>::mesh->halfedge(*he_it);
        ACG::Vec3d v1 = mesh->vertex(edge.from_vertex());
        ACG::Vec3d v2 = mesh->vertex(edge.to_vertex());
        ACG::Vec3d p = 0.5 * (v1+v2);
        double distance = (p-_hitPoint).length();
        if (distance < min_distance)
        {
            min_distance = distance;
            closestHalfEdgeHandle = *he_it;
        }

    }

    return closestHalfEdgeHandle.idx();
}


template <typename MeshT>
void OVMPropertyVisualizer<MeshT>::visualize(bool _setDrawMode, QWidget* _widget)
{
    QWidget* tmp;
    if (_widget)
    {
        tmp = widget;
        widget = _widget;
    }

    if (propertyInfo.isCellProp())
        visualizeCellProp(_setDrawMode);
    else if (propertyInfo.isFaceProp())
        visualizeFaceProp(_setDrawMode);
    else if (propertyInfo.isHalffaceProp())
        visualizeHalffaceProp(_setDrawMode);
    else if (propertyInfo.isEdgeProp())
        visualizeEdgeProp(_setDrawMode);
    else if (propertyInfo.isHalfedgeProp())
        visualizeHalfedgeProp(_setDrawMode);
    else if (propertyInfo.isVertexProp())
        visualizeVertexProp(_setDrawMode);

    if (_widget)
    {
        widget = tmp;
    }
}

template <typename MeshT>
OpenMesh::Vec4f OVMPropertyVisualizer<MeshT>::convertColor(const QColor _color){

  OpenMesh::Vec4f color;

  color[0] = _color.redF();
  color[1] = _color.greenF();
  color[2] = _color.blueF();
  color[3] = _color.alphaF();

  return color;
}

template <typename MeshT>
void OVMPropertyVisualizer<MeshT>::visualizeFaceProp(bool /*_setDrawMode*/)
{
    emit log(LOGERR, "Visualizing FaceProp not implemented");
}

template <typename MeshT>
void OVMPropertyVisualizer<MeshT>::visualizeEdgeProp(bool /*_setDrawMode*/)
{
    emit log(LOGERR, "Visualizing EdgeProp not implemented");
}

template <typename MeshT>
void OVMPropertyVisualizer<MeshT>::visualizeHalfedgeProp(bool /*_setDrawMode*/)
{
    emit log(LOGERR, "Visualizing HalfedgeProp not implemented");
}

template <typename MeshT>
void OVMPropertyVisualizer<MeshT>::visualizeVertexProp(bool /*_setDrawMode*/)
{
    emit log(LOGERR, "Visualizing VertexProp not implemented");
}

template <typename MeshT>
void OVMPropertyVisualizer<MeshT>::visualizeCellProp(bool /*_setDrawMode*/)
{
    emit log(LOGERR, "Visualizing CellProp not implemented");
}

template <typename MeshT>
void OVMPropertyVisualizer<MeshT>::visualizeHalffaceProp(bool /*_setDrawMode*/)
{
    emit log(LOGERR, "Visualizing HalffaceProp not implemented");
}

template<typename MeshT>
template<typename PropType>
inline void OVMPropertyVisualizer<MeshT>::duplicateProperty_stage1() {
    std::string newPropertyName;
    for (int i = 1;; ++i) {
        std::ostringstream oss;
        oss << propertyInfo.propName() << " Copy " << i;
        newPropertyName = oss.str();

        if (propertyInfo.isCellProp())
        {
            if(!mesh->template cell_property_exists<PropType>(newPropertyName)) break;
        }
        else if (propertyInfo.isFaceProp())
        {
            if(!mesh->template face_property_exists<PropType>(newPropertyName)) break;
        }
        else if (propertyInfo.isHalffaceProp())
        {
            if(!mesh->template halfface_property_exists<PropType>(newPropertyName)) break;
        }
        else if (propertyInfo.isEdgeProp())
        {
            if(!mesh->template edge_property_exists<PropType>(newPropertyName)) break;
        }
        else if (propertyInfo.isHalfedgeProp())
        {
            if(!mesh->template halfedge_property_exists<PropType>(newPropertyName)) break;
        }
        else if (propertyInfo.isVertexProp())
        {
            if(!mesh->template vertex_property_exists<PropType>(newPropertyName)) break;
        }
    }

    if (propertyInfo.isCellProp())
    {
        OpenVolumeMesh::CellPropertyT<PropType> prop = mesh->template request_cell_property<PropType>(OVMPropertyVisualizer<MeshT>::propertyInfo.propName());
        OpenVolumeMesh::CellPropertyT<PropType> newProp = mesh->template request_cell_property< PropType >(newPropertyName);
        mesh->set_persistent(newProp, true);
        std::for_each(mesh->cells_begin(), mesh->cells_end(), CopyProperty<OpenVolumeMesh::CellPropertyT<PropType> >(newProp, prop, mesh));
    }
    else if (propertyInfo.isFaceProp())
    {
        OpenVolumeMesh::FacePropertyT<PropType> prop = mesh->template request_face_property<PropType>(OVMPropertyVisualizer<MeshT>::propertyInfo.propName());
        OpenVolumeMesh::FacePropertyT<PropType> newProp = mesh->template request_face_property< PropType >(newPropertyName);
        mesh->set_persistent(newProp, true);
        std::for_each(mesh->faces_begin(), mesh->faces_end(), CopyProperty<OpenVolumeMesh::FacePropertyT<PropType> >(newProp, prop, mesh));
    }
    else if (propertyInfo.isHalffaceProp())
    {
        OpenVolumeMesh::HalfFacePropertyT<PropType> prop = mesh->template request_halfface_property<PropType>(OVMPropertyVisualizer<MeshT>::propertyInfo.propName());
        OpenVolumeMesh::HalfFacePropertyT<PropType> newProp = mesh->template request_halfface_property< PropType >(newPropertyName);
        mesh->set_persistent(newProp, true);
        std::for_each(mesh->halffaces_begin(), mesh->halffaces_end(), CopyProperty<OpenVolumeMesh::HalfFacePropertyT<PropType> >(newProp, prop, mesh));
    }
    else if (propertyInfo.isEdgeProp())
    {
        OpenVolumeMesh::EdgePropertyT<PropType> prop = mesh->template request_edge_property<PropType>(OVMPropertyVisualizer<MeshT>::propertyInfo.propName());
        OpenVolumeMesh::EdgePropertyT<PropType> newProp = mesh->template request_edge_property< PropType >(newPropertyName);
        mesh->set_persistent(newProp, true);
        std::for_each(mesh->edges_begin(), mesh->edges_end(), CopyProperty<OpenVolumeMesh::EdgePropertyT<PropType> >(newProp, prop, mesh));
    }
    else if (propertyInfo.isHalfedgeProp())
    {
        OpenVolumeMesh::HalfEdgePropertyT<PropType> prop = mesh->template request_halfedge_property<PropType>(OVMPropertyVisualizer<MeshT>::propertyInfo.propName());
        OpenVolumeMesh::HalfEdgePropertyT<PropType> newProp = mesh->template request_halfedge_property< PropType >(newPropertyName);
        mesh->set_persistent(newProp, true);
        std::for_each(mesh->halfedges_begin(), mesh->halfedges_end(), CopyProperty<OpenVolumeMesh::HalfEdgePropertyT<PropType> >(newProp, prop, mesh));
    }
    else if (propertyInfo.isVertexProp())
    {
        OpenVolumeMesh::VertexPropertyT<PropType> prop = mesh->template request_vertex_property<PropType>(OVMPropertyVisualizer<MeshT>::propertyInfo.propName());
        OpenVolumeMesh::VertexPropertyT<PropType> newProp = mesh->template request_vertex_property< PropType >(newPropertyName);
        mesh->set_persistent(newProp, true);
        std::for_each(mesh->vertices_begin(), mesh->vertices_end(), CopyProperty<OpenVolumeMesh::VertexPropertyT<PropType> >(newProp, prop, mesh));
    }
}

template <typename MeshT>
void OVMPropertyVisualizer<MeshT>::clear()
{
    VolumeMeshObject<MeshT>* object;
    PluginFunctions::getObject(OVMPropertyVisualizer<MeshT>::mObjectID, object);

    if (propertyInfo.isCellProp())
        object->colors().clear_cell_colors();
    else if (propertyInfo.isFaceProp())
        object->colors().clear_face_colors();
    else if (propertyInfo.isHalffaceProp())
        object->colors().clear_halfface_colors();
    else if (propertyInfo.isEdgeProp())
        object->colors().clear_edge_colors();
    else if (propertyInfo.isHalfedgeProp())
        object->colors().clear_halfedge_colors();
    else if (propertyInfo.isVertexProp())
        object->colors().clear_vertex_colors();

    object->setObjectDrawMode(drawModes.cellsFlatShaded);
}

template <typename MeshT>
void OVMPropertyVisualizer<MeshT>::setCellPropertyFromText(unsigned int /*index*/, QString /*text*/)
{
    emit log(LOGERR, "Setting CellProp not implemented for this property type");
}

template <typename MeshT>
void OVMPropertyVisualizer<MeshT>::setFacePropertyFromText(unsigned int /*index*/, QString /*text*/)
{
    emit log(LOGERR, "Setting FaceProp not implemented for this property type");
}

template <typename MeshT>
void OVMPropertyVisualizer<MeshT>::setHalffacePropertyFromText(unsigned int /*index*/, QString /*text*/)
{
    emit log(LOGERR, "Setting HalffaceProp not implemented for this property type");
}

template <typename MeshT>
void OVMPropertyVisualizer<MeshT>::setEdgePropertyFromText(unsigned int /*index*/, QString /*text*/)
{
    emit log(LOGERR, "Setting EdgeProp not implemented for this property type");
}

template <typename MeshT>
void OVMPropertyVisualizer<MeshT>::setHalfedgePropertyFromText(unsigned int /*index*/, QString /*text*/)
{
    emit log(LOGERR, "Setting HalfedgeProp not implemented for this property type");
}

template <typename MeshT>
void OVMPropertyVisualizer<MeshT>::setVertexPropertyFromText(unsigned int /*index*/, QString /*text*/)
{
    emit log(LOGERR, "Setting VertexProp not implemented for this property type");
}

#endif /* ENABLE_OPENVOLUMEMESH_SUPPORT */
