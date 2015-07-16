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


#define OM_PROPERTY_VISUALIZER_VECTOR_FIELD_DIFFERENCE_CC

#include "OMPropertyVisualizerVectorFieldDifference.hh"

template <typename MeshT>
OMPropertyVisualizerVectorFieldDifference<MeshT>::OMPropertyVisualizerVectorFieldDifference(MeshT* _mesh, PropertyInfo _propertyInfo1, PropertyInfo _propertyInfo2)
    : OMPropertyVisualizer<MeshT>(_mesh, _propertyInfo1),
      propertyInfo2(_propertyInfo2)
{
    if (PropertyVisualizer::widget) delete PropertyVisualizer::widget;
    VectorFieldDifferenceWidget* w = new VectorFieldDifferenceWidget();
    w->paramVectorFieldDifference->setTitle(QString("3D Vector Field Difference Parameters of ").append(PropertyVisualizer::propertyInfo.propName().c_str()));
    PropertyVisualizer::widget = w;
}

template <typename MeshT>
QString OMPropertyVisualizerVectorFieldDifference<MeshT>::getPropertyText(unsigned int index)
{
    return "";
}

namespace OMPVVFD{ //OMPVVFD = OpenMeshProeprtyVisualizerVectorFieldDifference

template<typename MeshT>
float scalarFn_norm_of_diff(const typename MeshT::Point &a, const typename MeshT::Point &b) {
    return (a - b).norm();
}

template<typename MeshT>
float scalarFn_diff_of_norms(const typename MeshT::Point &a, const typename MeshT::Point &b) {
    return std::fabs(a.norm() - b.norm());
}

template<typename MeshT>
float scalarFn_4_symm_diff(const typename MeshT::Point &a, const typename MeshT::Point &b) {
    double alpha = std::acos((a|b) / a.norm() / b.norm());
    alpha -= std::floor((alpha + M_PI_4) / M_PI_2) * M_PI_2;
    return std::fabs(alpha);
}


template<typename Mesh, typename Prop1, typename Prop2, float (*ScalarFn)(const typename Prop1::value_type &, const typename Prop2::value_type &)>
class ScalarAssigner {
    public:
        ScalarAssigner(const Mesh &mesh, const Prop1 &prop1, const Prop2 &prop2) :
            mesh(mesh), prop1(prop1), prop2(prop2) { }

        template<typename Handle>
        float operator() (const Handle &handle) const {
            return ScalarFn(mesh.property(prop1, handle), mesh.property(prop2, handle));
        }

    protected:
        const Mesh &mesh;
        const Prop1 &prop1;
        const Prop2 &prop2;
};

template<typename MeshT, typename IteratorT, typename PropHandleType, float (*ScalarFn)(const typename MeshT::Point &, const typename MeshT::Point &)>
void colorElements(MeshT *mesh, const PropertyInfo &p1,
                   const PropertyInfo &p2,
                   IteratorT primitivesBegin, IteratorT primitivesEnd) {
    PropHandleType prop1, prop2;
    if (!mesh->get_property_handle(prop1, p1.propName())) return;
    if (!mesh->get_property_handle(prop2, p2.propName())) return;

    std::vector<float> scalars;
    std::transform(primitivesBegin, primitivesEnd, std::back_inserter(scalars),
                   ScalarAssigner<MeshT, PropHandleType, PropHandleType, ScalarFn >(*mesh, prop1, prop2));

    const float min = *std::min_element(scalars.begin(), scalars.end());
    const float max = *std::max_element(scalars.begin(), scalars.end());

    ACG::ColorCoder colCod(min,max,false);
    for (std::vector<float>::iterator i = scalars.begin(); i != scalars.end(); ++i)
        mesh->set_color(*(primitivesBegin++),colCod(*i));
  }

}

template <typename MeshT>
void OMPropertyVisualizerVectorFieldDifference<MeshT>::visualizeFaceProp(bool _setDrawMode)
{
    MeshT* mesh = OMPropertyVisualizer<MeshT>::mesh;
    const PropertyInfo p1 = PropertyVisualizer::propertyInfo;
    const PropertyInfo p2 = propertyInfo2;
    VectorFieldDifferenceWidget* w = static_cast<VectorFieldDifferenceWidget*>(PropertyVisualizer::widget);
    if (!mesh->has_face_colors())
        mesh->request_face_colors();

    if (w->vecFieldDiff_4symm_rb->isChecked())
        OMPVVFD::colorElements<MeshT, typename MeshT::FaceIter, OpenMesh::FPropHandleT<typename MeshT::Point>, OMPVVFD::scalarFn_4_symm_diff<MeshT> >(mesh, p1, p2, mesh->faces_begin(), mesh->faces_end());
    if (w->vecFieldDiff_diff_norm_rb->isChecked())
        OMPVVFD::colorElements<MeshT, typename MeshT::FaceIter, OpenMesh::FPropHandleT<typename MeshT::Point>, OMPVVFD::scalarFn_diff_of_norms<MeshT> >(mesh, p1, p2, mesh->faces_begin(), mesh->faces_end());
    if (w->vecFieldDiff_norm_diff_rb->isChecked())
        OMPVVFD::colorElements<MeshT, typename MeshT::FaceIter, OpenMesh::FPropHandleT<typename MeshT::Point>, OMPVVFD::scalarFn_norm_of_diff<MeshT> >(mesh, p1, p2, mesh->faces_begin(), mesh->faces_end());

    if (_setDrawMode)
        PluginFunctions::setDrawMode(ACG::SceneGraph::DrawModes::SOLID_FACES_COLORED);
}

template <typename MeshT>
void OMPropertyVisualizerVectorFieldDifference<MeshT>::visualizeEdgeProp(bool _setDrawMode)
{
    MeshT* mesh = OMPropertyVisualizer<MeshT>::mesh;
    const PropertyInfo p1 = PropertyVisualizer::propertyInfo;
    const PropertyInfo p2 = propertyInfo2;
    VectorFieldDifferenceWidget* w = static_cast<VectorFieldDifferenceWidget*>(PropertyVisualizer::widget);
    if (!mesh->has_edge_colors())
        mesh->request_edge_colors();

    if (w->vecFieldDiff_4symm_rb->isChecked())
        OMPVVFD::colorElements<MeshT, typename MeshT::EdgeIter, OpenMesh::EPropHandleT<typename MeshT::Point>, OMPVVFD::scalarFn_4_symm_diff<MeshT> >(mesh, p1, p2, mesh->edges_begin(), mesh->edges_end());
    if (w->vecFieldDiff_diff_norm_rb->isChecked())
        OMPVVFD::colorElements<MeshT, typename MeshT::EdgeIter, OpenMesh::EPropHandleT<typename MeshT::Point>, OMPVVFD::scalarFn_diff_of_norms<MeshT> >(mesh, p1, p2, mesh->edges_begin(), mesh->edges_end());
    if (w->vecFieldDiff_norm_diff_rb->isChecked())
        OMPVVFD::colorElements<MeshT, typename MeshT::EdgeIter, OpenMesh::EPropHandleT<typename MeshT::Point>, OMPVVFD::scalarFn_norm_of_diff<MeshT> >(mesh, p1, p2, mesh->edges_begin(), mesh->edges_end());

    if (_setDrawMode)
        PluginFunctions::setDrawMode(ACG::SceneGraph::DrawModes::EDGES_COLORED);
}

template <typename MeshT>
void OMPropertyVisualizerVectorFieldDifference<MeshT>::visualizeHalfedgeProp(bool _setDrawMode)
{
    MeshT* mesh = OMPropertyVisualizer<MeshT>::mesh;
    const PropertyInfo p1 = PropertyVisualizer::propertyInfo;
    const PropertyInfo p2 = propertyInfo2;
    VectorFieldDifferenceWidget* w = static_cast<VectorFieldDifferenceWidget*>(PropertyVisualizer::widget);
    if (!mesh->has_halfedge_colors())
        mesh->request_halfedge_colors();

    if (w->vecFieldDiff_4symm_rb->isChecked())
        OMPVVFD::colorElements<MeshT, typename MeshT::HalfedgeIter, OpenMesh::HPropHandleT<typename MeshT::Point>, OMPVVFD::scalarFn_4_symm_diff<MeshT> >(mesh, p1, p2, mesh->halfedges_begin(), mesh->halfedges_end());
    if (w->vecFieldDiff_diff_norm_rb->isChecked())
        OMPVVFD::colorElements<MeshT, typename MeshT::HalfedgeIter, OpenMesh::HPropHandleT<typename MeshT::Point>, OMPVVFD::scalarFn_diff_of_norms<MeshT> >(mesh, p1, p2, mesh->halfedges_begin(), mesh->halfedges_end());
    if (w->vecFieldDiff_norm_diff_rb->isChecked())
        OMPVVFD::colorElements<MeshT, typename MeshT::HalfedgeIter, OpenMesh::HPropHandleT<typename MeshT::Point>, OMPVVFD::scalarFn_norm_of_diff<MeshT> >(mesh, p1, p2, mesh->halfedges_begin(), mesh->halfedges_end());

    if (_setDrawMode)
        PluginFunctions::setDrawMode(ACG::SceneGraph::DrawModes::HALFEDGES_COLORED);
}

template <typename MeshT>
void OMPropertyVisualizerVectorFieldDifference<MeshT>::visualizeVertexProp(bool _setDrawMode)
{
    MeshT* mesh = OMPropertyVisualizer<MeshT>::mesh;
    const PropertyInfo p1 = PropertyVisualizer::propertyInfo;
    const PropertyInfo p2 = propertyInfo2;
    VectorFieldDifferenceWidget* w = static_cast<VectorFieldDifferenceWidget*>(PropertyVisualizer::widget);
    if (!mesh->has_vertex_colors())
        mesh->request_vertex_colors();

    if (w->vecFieldDiff_4symm_rb->isChecked())
        OMPVVFD::colorElements<MeshT, typename MeshT::VertexIter, OpenMesh::VPropHandleT<typename MeshT::Point>, OMPVVFD::scalarFn_4_symm_diff<MeshT> >(mesh, p1, p2, mesh->vertices_begin(), mesh->vertices_end());
    if (w->vecFieldDiff_diff_norm_rb->isChecked())
        OMPVVFD::colorElements<MeshT, typename MeshT::VertexIter, OpenMesh::VPropHandleT<typename MeshT::Point>, OMPVVFD::scalarFn_diff_of_norms<MeshT> >(mesh, p1, p2, mesh->vertices_begin(), mesh->vertices_end());
    if (w->vecFieldDiff_norm_diff_rb->isChecked())
        OMPVVFD::colorElements<MeshT, typename MeshT::VertexIter, OpenMesh::VPropHandleT<typename MeshT::Point>, OMPVVFD::scalarFn_norm_of_diff<MeshT> >(mesh, p1, p2, mesh->vertices_begin(), mesh->vertices_end());

    if (_setDrawMode)
        PluginFunctions::setDrawMode(ACG::SceneGraph::DrawModes::SOLID_POINTS_COLORED);
}
