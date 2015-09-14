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
*   $Revision: 21177 $                                                       *
*   $LastChangedBy: moebius $                                                *
*   $Date: 2015-09-07 08:56:47 +0200 (Mo, 07 Sep 2015) $                     *
*                                                                            *
\*===========================================================================*/

#include "MultiObjectPropertyModel.hh"

#include "PropertyModelFactory.hh"
#include "OpenMesh/OMPropertyModel.hh"
#include "OpenVolumeMesh/OVMPropertyModel.hh"
#include "OpenVolumeMesh/Mesh/HexahedralMesh.hh"

#include "Utils.hh"

MultiObjectPropertyModel::MultiObjectPropertyModel(const QStringList& res, QObject *parent) :
    PropertyModel(parent), restriction(res), datatypes(supportedDataTypes()), widget(0)
{
    QVBoxLayout* layout = new QVBoxLayout();
    widget = new QWidget();
    widget->setLayout(layout);
}

MultiObjectPropertyModel::~MultiObjectPropertyModel()
{
    for (size_t i = 0; i < propWidgets.size(); ++i)
    {
        delete propWidgets[i];
    }
    delete widget;
}

QVariant MultiObjectPropertyModel::data(const QModelIndex & index, int role) const
{
    switch (role) {
        case Qt::DisplayRole:
            return QVariant(propNames[index.row()]);
        default:
            return QVariant::Invalid;
    }
}

int MultiObjectPropertyModel::rowCount(const QModelIndex & parent) const
{
    return propNames.size();
}

QVariant MultiObjectPropertyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant::Invalid;
}

void MultiObjectPropertyModel::objectUpdated()
{
    using namespace PluginFunctions;

    for (ObjectIterator o_it(restriction, datatypes); o_it != objectsEnd(); ++o_it)
    {
        // get the property model and call objectUpdated
        PropertyModel* model = PropertyModelFactory::Instance().getModel(o_it->id());
        if (model == 0) continue;
        model->objectUpdated();
    }
}

void MultiObjectPropertyModel::visualize(QModelIndexList selectedIndices, QWidgetList widgets)
{
    using namespace PluginFunctions;

    // return if nothing is selected
    if (selectedIndices.size() < 1) return;

    for (ObjectIterator o_it(restriction, datatypes); o_it != objectsEnd(); ++o_it)
    {
        // get the property model and update it
        PropertyModel* model = PropertyModelFactory::Instance().getModel(o_it->id());
        if (model == 0) continue;
        model->gatherProperties();

        QModelIndexList indexList;
        QWidgetList widgetList;

        for (int i = 0; i < selectedIndices.size(); ++i)
        {
            const QString name = selectedIndices[i].data().toString();

            // skip this property if it does not exist
            const QModelIndex idx = model->indexFromFancyPropName(name);
            if (!idx.isValid()) continue;

            // insert items into lists
            indexList.append(idx);
            widgetList.append(propWidgets[selectedIndices[i].row()]);
        }

        // visualize the property
        model->visualize(indexList, widgetList);
    }
}

void MultiObjectPropertyModel::removeProperty(QModelIndexList selectedIndices)
{
    using namespace PluginFunctions;

    // return if nothing is selected
    if (selectedIndices.size() < 1) return;

    for (ObjectIterator o_it(restriction, datatypes); o_it != objectsEnd(); ++o_it)
    {
        PropertyModel* model = PropertyModelFactory::Instance().getModel(o_it->id());
        if (model == 0) continue;

        QModelIndexList indexList;

        for (int i = 0; i < selectedIndices.size(); ++i)
        {
            const QString name = selectedIndices[i].data().toString();

            // skip this property if it does not exist
            const QModelIndex idx = model->indexFromFancyPropName(name);
            if (!idx.isValid()) continue;

            // insert item into list
            indexList.append(idx);
        }

        model->removeProperty(indexList);
    }

    // update this model
    gatherProperties();
}

void MultiObjectPropertyModel::duplicateProperty(QModelIndexList selectedIndices)
{
    using namespace PluginFunctions;

    // return if nothing is selected
    if (selectedIndices.size() < 1) return;

    for (ObjectIterator o_it(restriction, datatypes); o_it != objectsEnd(); ++o_it)
    {
        PropertyModel* model = PropertyModelFactory::Instance().getModel(o_it->id());
        if (model == 0) continue;

        QModelIndexList indexList;

        for (int i = 0; i < selectedIndices.size(); ++i)
        {
            const QString name = selectedIndices[i].data().toString();

            // skip this property if it does not exist
            const QModelIndex idx = model->indexFromFancyPropName(name);
            if (!idx.isValid()) continue;

            // insert item into list
            indexList.append(idx);
        }

        model->duplicateProperty(indexList);
    }

    // update this model
    gatherProperties();
}

void MultiObjectPropertyModel::gatherProperties()
{
    using namespace PluginFunctions;

    beginResetModel();
    propNames.clear();
    propInfos.clear();
    for (size_t i = 0; i < propWidgets.size(); ++i)
    {
        delete propWidgets[i];
    }
    propWidgets.clear();
    endResetModel();

    for (ObjectIterator o_it(restriction, datatypes); o_it != objectsEnd(); ++o_it)
    {
        PropertyModel* model = PropertyModelFactory::Instance().getModel(o_it->id());
        if (model == 0) continue;
        model->gatherProperties();

        for (int i = 0; i < model->rowCount(); ++i)
        {
            const QModelIndex idx = model->index(i, 0);
            const QString name = idx.data().toString();

            // add property
            if (std::find(propNames.begin(), propNames.end(), name) == propNames.end())
            {
                PropertyInfo info = model->getPropertyInfo(idx);
                QWidget* widget = createWidgetForType(info.typeinfo());
                setRange(info, widget);

                propNames.push_back(name);
                propInfos.push_back(info);
                propWidgets.push_back(widget);
            }
        }
    }
}

void MultiObjectPropertyModel::clear(QModelIndexList selectedIndices)
{
    using namespace PluginFunctions;

    // return if nothing is selected
    if (selectedIndices.size() < 1) return;

    for (ObjectIterator o_it(restriction, datatypes); o_it != objectsEnd(); ++o_it)
    {
        PropertyModel* model = PropertyModelFactory::Instance().getModel(o_it->id());
        if (model == 0) continue;

        QModelIndexList indexList;

        for (int i = 0; i < selectedIndices.size(); ++i)
        {
            const QString name = selectedIndices[i].data().toString();

            // skip this property if it does not exist
            const QModelIndex idx = model->indexFromFancyPropName(name);
            if (!idx.isValid()) continue;

            // insert item into list
            indexList.append(idx);
        }

        model->clear(indexList);
    }
}

void MultiObjectPropertyModel::hideWidget()
{
    widget->hide();
}

QWidget* MultiObjectPropertyModel::getWidget()
{
    return widget;
}

void MultiObjectPropertyModel::updateWidget(const QModelIndexList& selectedIndices)
{
    QLayout* layout = widget->layout();

    for (unsigned int i = 0; i < propWidgets.size(); ++i)
    {
        propWidgets[i]->hide();
    }

    for (int i = 0; i < selectedIndices.size(); ++i)
    {
        const int row = selectedIndices[i].row();
        QWidget* w = propWidgets[row];
        layout->addWidget(w);
        w->show();
    }

    widget->setLayout(layout);
}

void MultiObjectPropertyModel::connectLogs(PropertyVisualizer* propViz)
{

}

PropertyInfo MultiObjectPropertyModel::getPropertyInfo(const QModelIndex index) const
{
    return propInfos[index.row()];
}

QWidget* MultiObjectPropertyModel::createWidgetForType(const TypeInfoWrapper& info) const
{
    // OpenMesh
    // ----------------------------------------

    if (info == OMPropertyModel<TriMesh>::proptype_bool)
        return new BooleanWidget();
    if (info == OMPropertyModel<TriMesh>::proptype_int)
        return new IntegerWidget();
    if (info == OMPropertyModel<TriMesh>::proptype_uint)
        return new IntegerWidget();
    if (info == OMPropertyModel<TriMesh>::proptype_double)
        return new DoubleWidget();
    if (info == OMPropertyModel<TriMesh>::proptype_Vec3d)
        return new VectorWidget;
    if (info == OMPropertyModel<TriMesh>::proptype_Vec3f)
        return new VectorWidget;
    if (info == OMPropertyModel<TriMesh>::proptype_Vec2d)
        return new VectorWidget;
    if (info == OMPropertyModel<TriMesh>::proptype_Vec2f)
        return new VectorWidget;

    #ifdef ENABLE_SKELETON_SUPPORT
    if (info == OMPropertyModel<TriMesh>::proptype_SkinWeights)
        return new SkinWeightsWidget;
    #endif


    // OpenVolumeMesh
    // ----------------------------------------

    #ifdef ENABLE_OPENVOLUMEMESH_SUPPORT
    if (OVMPropertyModel<HexahedralMesh>::isBoolType(info))
        return new BooleanWidget();
    if (OVMPropertyModel<HexahedralMesh>::isIntType(info))
        return new IntegerWidget();
    if (OVMPropertyModel<HexahedralMesh>::isUnsignedIntType(info))
        return new IntegerWidget();
    if (OVMPropertyModel<HexahedralMesh>::isDoubleType(info))
        return new DoubleWidget();
    if (OVMPropertyModel<HexahedralMesh>::isVec3dType(info))
        return new VectorWidget();
    if (OVMPropertyModel<HexahedralMesh>::isVec3fType(info))
        return new VectorWidget();
    #endif


    // Other
    // ----------------------------------------

    return new QWidget();
}

template <typename ItemHandle, typename PropHandle, typename T>
void range3_om(const OpenMesh::BaseKernel* mesh, unsigned int n, const std::string& name, T& min, T& max)
{
    PropHandle ph;
    mesh->get_property_handle(ph, name);
    if (!ph.is_valid()) return;

    for (unsigned int i = 0; i < n; ++i)
    {
        const ItemHandle ih(i);
        min = std::min(min, mesh->property(ph, ih));
        max = std::max(max, mesh->property(ph, ih));
    }
}

template <typename ItemHandle, typename Property, typename T>
void range3_ovm(Property& prop, unsigned int n, T& min, T& max)
{
    for (unsigned int i = 0; i < n; ++i)
    {
        const ItemHandle ih(i);
        min = std::min(min, prop[ih]);
        max = std::max(max, prop[ih]);
    }
}

template <typename Mesh, typename T>
void range2_om(const Mesh* mesh, const PropertyInfo& info, T& min, T&max)
{
    if (mesh == 0) return;

    if (info.isVertexProp())
        range3_om<OpenMesh::VertexHandle, OpenMesh::VPropHandleT<T>, T>
            (mesh, mesh->n_vertices(), info.propName(), min, max);
    if (info.isHalfedgeProp())
        range3_om<OpenMesh::HalfedgeHandle, OpenMesh::HPropHandleT<T>, T>
            (mesh, mesh->n_halfedges(), info.propName(), min, max);
    if (info.isEdgeProp())
        range3_om<OpenMesh::EdgeHandle, OpenMesh::EPropHandleT<T>, T>
            (mesh, mesh->n_edges(), info.propName(), min, max);
    if (info.isFaceProp())
        range3_om<OpenMesh::FaceHandle, OpenMesh::FPropHandleT<T>, T>
            (mesh, mesh->n_faces(), info.propName(), min, max);
}

template <typename Mesh, typename T>
void range2_ovm(Mesh* mesh, const PropertyInfo& info, T& min, T&max)
{
    if (mesh == 0) return;

    if (info.isCellProp() && mesh->template cell_property_exists<T>(info.propName())) {
        OpenVolumeMesh::CellPropertyT<T> prop = mesh->template request_cell_property<T>(info.propName());
        range3_ovm<OpenVolumeMesh::CellHandle, OpenVolumeMesh::CellPropertyT<T>, T>
            (prop, mesh->n_cells(), min, max);
    }
    if (info.isEdgeProp() && mesh->template edge_property_exists<T>(info.propName())) {
        OpenVolumeMesh::EdgePropertyT<T> prop = mesh->template request_edge_property<T>(info.propName());
        range3_ovm<OpenVolumeMesh::EdgeHandle, OpenVolumeMesh::EdgePropertyT<T>, T>
            (prop, mesh->n_edges(), min, max);
    }
    if (info.isFaceProp() && mesh->template face_property_exists<T>(info.propName())) {
        OpenVolumeMesh::FacePropertyT<T> prop = mesh->template request_face_property<T>(info.propName());
        range3_ovm<OpenVolumeMesh::FaceHandle, OpenVolumeMesh::FacePropertyT<T>, T>
            (prop, mesh->n_faces(), min, max);
    }
    if (info.isHalfedgeProp() && mesh->template halfedge_property_exists<T>(info.propName())) {
        OpenVolumeMesh::HalfEdgePropertyT<T> prop = mesh->template request_halfedge_property<T>(info.propName());
        range3_ovm<OpenVolumeMesh::HalfEdgeHandle, OpenVolumeMesh::HalfEdgePropertyT<T>, T>
            (prop, mesh->n_halfedges(), min, max);
    }
    if (info.isHalffaceProp() && mesh->template halfface_property_exists<T>(info.propName())) {
        OpenVolumeMesh::HalfFacePropertyT<T> prop = mesh->template request_halfface_property<T>(info.propName());
        range3_ovm<OpenVolumeMesh::HalfFaceHandle, OpenVolumeMesh::HalfFacePropertyT<T>, T>
            (prop, mesh->n_halffaces(), min, max);
    }
    if (info.isVertexProp() && mesh->template vertex_property_exists<T>(info.propName())) {
        OpenVolumeMesh::VertexPropertyT<T> prop = mesh->template request_vertex_property<T>(info.propName());
        range3_ovm<OpenVolumeMesh::VertexHandle, OpenVolumeMesh::VertexPropertyT<T>, T>
            (prop, mesh->n_vertices(), min, max);
    }
}

template <typename T>
void range1(const BaseObject* obj, const PropertyInfo& info, T& min, T& max)
{
    using namespace PluginFunctions;

    if (obj->dataType(DATA_TRIANGLE_MESH))
        range2_om(triMesh(obj->id()), info, min, max);
    if (obj->dataType(DATA_POLY_MESH))
        range2_om(polyMesh(obj->id()), info, min, max);

    #ifdef ENABLE_OPENVOLUMEMESH_POLYHEDRAL_SUPPORT
    if (obj->dataType(DATA_POLYHEDRAL_MESH))
        range2_ovm(polyhedralMesh(obj->id()), info, min, max);
    #endif

    #ifdef ENABLE_OPENVOLUMEMESH_HEXAHEDRAL_SUPPORT
    if (obj->dataType(DATA_HEXAHEDRAL_MESH))
        range2_ovm(hexahedralMesh(obj->id()), info, min, max);
    #endif
}

void MultiObjectPropertyModel::setRange(const PropertyInfo& info, QWidget* widget) const
{
    using namespace PluginFunctions;

    bool isDoubleType = info.typeinfo() == OMPropertyModel<TriMesh>::proptype_double;

    #ifdef ENABLE_OPENVOLUMEMESH_HEXAHEDRAL_SUPPORT
      isDoubleType |= OVMPropertyModel<HexahedralMesh>::isDoubleType(info.typeinfo());
    #endif

    bool isIntType = info.typeinfo() == OMPropertyModel<TriMesh>::proptype_int;

    #ifdef ENABLE_OPENVOLUMEMESH_HEXAHEDRAL_SUPPORT
      isIntType |= OVMPropertyModel<HexahedralMesh>::isIntType(info.typeinfo());
    #endif

    if (isDoubleType)
    {
        double min = +DBL_MAX;
        double max = -DBL_MAX;

        for (ObjectIterator o_it(restriction, datatypes); o_it != objectsEnd(); ++o_it)
        {
            range1<double>(*o_it, info, min, max);
        }

        DoubleWidget* w = static_cast<DoubleWidget*>(widget);
        w->doubleFixedRange->toggle();
        w->doubleFixedRangeMin->setValue(min);
        w->doubleFixedRangeMax->setValue(max);
    }

    if (isIntType)
    {
        int min = +INT_MAX;
        int max = -INT_MAX;

        for (ObjectIterator o_it(restriction, datatypes); o_it != objectsEnd(); ++o_it)
        {
            range1<int>(*o_it, info, min, max);
        }

        IntegerWidget* w = static_cast<IntegerWidget*>(widget);
        w->intFixedRange->toggle();
        w->intFixedRangeMin->setValue(min);
        w->intFixedRangeMax->setValue(max);
    }
}
