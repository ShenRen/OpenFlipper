#include "MultiObjectPropertyModel.hh"

#include "PropertyModelFactory.hh"
#include "OpenMesh/OMPropertyModel.hh"
#include "OpenVolumeMesh/OVMPropertyModel.hh"
#include "OpenVolumeMesh/Mesh/HexahedralMesh.hh"

MultiObjectPropertyModel::MultiObjectPropertyModel(const QStringList& res, QObject *parent) :
	PropertyModel(parent), restriction(res), widget(0)
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

	for (ObjectIterator o_it(restriction, DATA_TRIANGLE_MESH | DATA_POLY_MESH); o_it != objectsEnd(); ++o_it)
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

	for (ObjectIterator o_it(restriction, DATA_TRIANGLE_MESH | DATA_POLY_MESH); o_it != objectsEnd(); ++o_it)
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
			const QModelIndex idx = model->indexFromPropName(name);
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

	for (ObjectIterator o_it(restriction, DATA_TRIANGLE_MESH | DATA_POLY_MESH); o_it != objectsEnd(); ++o_it)
	{
		PropertyModel* model = PropertyModelFactory::Instance().getModel(o_it->id());
		if (model == 0) continue;
		
		QModelIndexList indexList;
		
		for (int i = 0; i < selectedIndices.size(); ++i)
		{
			const QString name = selectedIndices[i].data().toString();
			
			// skip this property if it does not exist
			const QModelIndex idx = model->indexFromPropName(name);
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

	for (ObjectIterator o_it(restriction, DATA_TRIANGLE_MESH | DATA_POLY_MESH); o_it != objectsEnd(); ++o_it)
	{
		PropertyModel* model = PropertyModelFactory::Instance().getModel(o_it->id());
		if (model == 0) continue;
		
		QModelIndexList indexList;
		
		for (int i = 0; i < selectedIndices.size(); ++i)
		{
			const QString name = selectedIndices[i].data().toString();
			
			// skip this property if it does not exist
			const QModelIndex idx = model->indexFromPropName(name);
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

	for (ObjectIterator o_it(restriction, DATA_TRIANGLE_MESH | DATA_POLY_MESH); o_it != objectsEnd(); ++o_it)
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

	for (ObjectIterator o_it(restriction, DATA_TRIANGLE_MESH | DATA_POLY_MESH); o_it != objectsEnd(); ++o_it)
	{
		PropertyModel* model = PropertyModelFactory::Instance().getModel(o_it->id());
		if (model == 0) continue;

		QModelIndexList indexList;
		
		for (int i = 0; i < selectedIndices.size(); ++i)
		{
			const QString name = selectedIndices[i].data().toString();
			
			// skip this property if it does not exist
			const QModelIndex idx = model->indexFromPropName(name);
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


	// Other
	// ----------------------------------------

	return new QWidget();
}

template <typename ItemHandle, typename PropHandle, typename T>
void range2(const OpenMesh::BaseKernel* mesh, unsigned int n, const std::string& name, T& min, T& max)
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

template <typename T>
void range1(const BaseObject* obj, const PropertyInfo& info, T& min, T& max)
{
	using namespace PluginFunctions;

	OpenMesh::BaseKernel* mesh = 0;
	if (obj->dataType(DATA_TRIANGLE_MESH))
		mesh = triMesh(obj->id());
	if (obj->dataType(DATA_POLY_MESH))
		mesh = polyMesh(obj->id());

	if (mesh == 0) return;

	if (info.isVertexProp())
		range2<OpenMesh::VertexHandle, OpenMesh::VPropHandleT<T>, T>
			(mesh, mesh->n_vertices(), info.propName(), min, max);
	if (info.isHalfedgeProp())
		range2<OpenMesh::HalfedgeHandle, OpenMesh::HPropHandleT<T>, T>
			(mesh, mesh->n_halfedges(), info.propName(), min, max);
	if (info.isEdgeProp())
		range2<OpenMesh::EdgeHandle, OpenMesh::EPropHandleT<T>, T>
			(mesh, mesh->n_edges(), info.propName(), min, max);
	if (info.isFaceProp())
		range2<OpenMesh::FaceHandle, OpenMesh::FPropHandleT<T>, T>
			(mesh, mesh->n_faces(), info.propName(), min, max);
}

void MultiObjectPropertyModel::setRange(const PropertyInfo& info, QWidget* widget) const
{
	using namespace PluginFunctions;

	if (info.typeinfo() == OMPropertyModel<TriMesh>::proptype_double)
	{
		double min = +DBL_MAX;
		double max = -DBL_MAX;

		for (ObjectIterator o_it(restriction, DATA_TRIANGLE_MESH | DATA_POLY_MESH); o_it != objectsEnd(); ++o_it)
		{
			range1<double>(*o_it, info, min, max);
		}

		DoubleWidget* w = static_cast<DoubleWidget*>(widget);
		w->doubleFixedRange->toggle();
		w->doubleFixedRangeMin->setValue(min);
		w->doubleFixedRangeMax->setValue(max);
	}

	if (info.typeinfo() == OMPropertyModel<TriMesh>::proptype_int)
	{
		int min = +INT_MAX;
		int max = -INT_MAX;

		for (ObjectIterator o_it(restriction, DATA_TRIANGLE_MESH | DATA_POLY_MESH); o_it != objectsEnd(); ++o_it)
		{
			range1<int>(*o_it, info, min, max);
		}

		IntegerWidget* w = static_cast<IntegerWidget*>(widget);
		w->intFixedRange->toggle();
		w->intFixedRangeMin->setValue(min);
		w->intFixedRangeMax->setValue(max);
	}
}
