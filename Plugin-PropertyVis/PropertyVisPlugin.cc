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

//=============================================================================
//
//  CLASS PropertyVisPlugin - IMPLEMENTATION
//
//=============================================================================

//== INCLUDES =================================================================

#include "PropertyVisPlugin.hh"

#include "PropertyModelFactory.hh"
#include "SingleObjectPropertyModel.hh"

#ifdef ENABLE_POLYHEDRALMESH_SUPPORT
    #include <ObjectTypes/PolyhedralMesh/PolyhedralMesh.hh>
#endif
#ifdef ENABLE_HEXAHEDRALMESH_SUPPORT
    #include <ObjectTypes/HexahedralMesh/HexahedralMesh.hh>
#endif
#ifdef ENABLE_TETRAHEDRALMESH_SUPPORT
    #include <ObjectTypes/TetrahedralMesh/TetrahedralMesh.hh>
#endif

//== IMPLEMENTATION ==========================================================

#define PROP_VIS "PropertyVisualization"

PropertyVisPlugin::PropertyVisPlugin() :
tool_(0),
propertyModel_(0)
{
}

void PropertyVisPlugin::initializePlugin()
{
	if ( OpenFlipper::Options::gui() ) {
	  tool_ = new PropertyVisToolbar();

	  QSize size(300,300);
	  tool_->resize(size);

	  tool_->meshNames->setModel(&objectListItemModel_);

	  emit addHiddenPickMode( PROP_VIS );

	  QIcon* toolIcon = new QIcon(OpenFlipper::Options::iconDirStr()+OpenFlipper::Options::dirSeparator()+"PropertyVisIcon.png");

	  emit addToolbox( tr("Property Visualization") , tool_, toolIcon );
	}
}

//-----------------------------------------------------------------------------

void PropertyVisPlugin::pluginsInitialized()
{
	if ( OpenFlipper::Options::gui() ) {

		// connect toolbox elements
		connect(tool_->meshNames,       SIGNAL( currentIndexChanged(int) ), this, SLOT( slotMeshChanged(int) ) );

		connect(tool_->visualizeButton, SIGNAL( clicked() ), this, SLOT( slotVisualize() ) );
		connect(tool_->clearButton,     SIGNAL( clicked() ), this, SLOT( slotAllCleared() ) );

		connect(tool_->refresh_property_names_tb, SIGNAL( clicked() ), this, SLOT( slotMeshChanged() ) );
		connect(tool_->duplicate_property_tb, SIGNAL( clicked() ), this, SLOT( slotDuplicateProperty() ) );
		connect(tool_->remove_property_tb, SIGNAL( clicked() ), this, SLOT( slotRemoveProperty() ) );

		connect(tool_, SIGNAL( widgetShown() ), this, SLOT( updateGUI() ) );

		setNewPropertyModel(-1);
	}
}

//-----------------------------------------------------------------------------

void PropertyVisPlugin::slotVisualizeProperty( int _id, const QString& _propname )
{
	PropertyModel* model = PropertyModelFactory::Instance().getModel(_id);

	if (model != 0)
	{
		model->gatherProperties();
		QModelIndex idx = model->indexFromPlainPropName(_propname);

		if (idx.isValid())
		{
			QModelIndexList list;
			list.append(idx);

			model->visualize(list);

			
			emit updateView();
			emit updatedObject( _id, UPDATE_COLOR );
		}
    }
}

QScriptValue PropertyVisPlugin::getPropertyVisualizer(int _id, const QString &_propname)
{
    PropertyModel* model = PropertyModelFactory::Instance().getModel(_id);

    if (model == nullptr) { return QScriptValue::SpecialValue::NullValue; }

    model->gatherProperties();
    QModelIndex idx = model->indexFromPlainPropName(_propname);
    if (!idx.isValid()) { return QScriptValue::SpecialValue::NullValue; }

    QScriptEngine *engine;
    emit getScriptingEngine (engine);
    if (engine == nullptr) { return QScriptValue::SpecialValue::NullValue; }

    QScriptContext *ctx = engine->currentContext();
    if (ctx == nullptr) { return QScriptValue::SpecialValue::NullValue; }

    auto sopm = dynamic_cast<SingleObjectPropertyModel*>(model);
    if (!sopm) { return QScriptValue::SpecialValue::NullValue; }

    return sopm->getScriptObject(idx, ctx);
}

//-----------------------------------------------------------------------------

void PropertyVisPlugin::slotPickModeChanged( const std::string& _mode)
{
	if (propertyModel_ != 0)
		propertyModel_->pickModeChanged(_mode);
}

//-----------------------------------------------------------------------------

void PropertyVisPlugin::slotAllCleared()
{
	using namespace PluginFunctions;

	if (propertyModel_ != 0)
	{
		QModelIndexList selectedIndices = tool_->propertyName_lv->selectionModel()->selectedIndexes();
		propertyModel_->clear(selectedIndices);
		propertyModel_->objectUpdated();
		emit updateView();
	}
}

//-----------------------------------------------------------------------------

void PropertyVisPlugin::objectDeleted(int _id)
{
	if( OpenFlipper::Options::gui() )
		objectListItemModel_.removeObject(_id);
	PropertyModelFactory::Instance().deleteModel(_id);
}

//-----------------------------------------------------------------------------

void PropertyVisPlugin::slotObjectUpdated( int _identifier, const UpdateType& _type )
{
	if( OpenFlipper::Options::gui() )
	{
		if ( tool_->isVisible() )
			updateGUI();
		PropertyModel* propertyModel = PropertyModelFactory::Instance().getModel(_identifier);
		if (propertyModel)
		{
			if (_type == UPDATE_ALL)
				propertyModel->gatherProperties();
			if (_type == (UPDATE_ALL | UPDATE_GEOMETRY))
				propertyModel->objectUpdated();
		}
	}
}

void PropertyVisPlugin::updateGUI()
{
	DataType datatype = DataType(DATA_TRIANGLE_MESH | DATA_POLY_MESH);
#ifdef ENABLE_POLYHEDRALMESH_SUPPORT
	datatype |= DataType(DATA_POLYHEDRAL_MESH);
#endif
#ifdef ENABLE_HEXAHEDRALMESH_SUPPORT
        datatype |= DataType(DATA_HEXAHEDRAL_MESH);
#endif
#ifdef ENABLE_TETRAHEDRALMESH_SUPPORT
        datatype |= DataType(DATA_TETRAHEDRAL_MESH);
#endif
        objectListItemModel_.refresh(datatype);
}

//-----------------------------------------------------------------------------

void PropertyVisPlugin::propertySelectionChanged()
{
	if (propertyModel_ != 0)
	{
		QModelIndexList selectedIndices = tool_->propertyName_lv->selectionModel()->selectedIndexes();
		propertyModel_->updateWidget(selectedIndices);
	}
}

//-----------------------------------------------------------------------------


/*! \brief Exchanges the PropertyModel after the user selected a different object.
 *
 * This method exchanges the PropertyModel after the user selects a different object
 * for visualization. The widget of the old model needs to be hidden and the one of
 * the new model is shown. Also some signals need to be disconnected and connected.
 *
 * \param id The id of the selected object
 */
void PropertyVisPlugin::setNewPropertyModel(int id)
{
	if (propertyModel_)
	{
		propertyModel_->hideWidget();
		disconnect(propertyModel_, SIGNAL(log(Logtype,QString)), this, SLOT(slotLog(Logtype,QString)));
		disconnect(propertyModel_, SIGNAL(log(QString)), this, SLOT(slotLog(QString)));
	}
	propertyModel_ = PropertyModelFactory::Instance().getModel(id);
	if (propertyModel_ != 0)
	{

		tool_->propertyName_lv->setModel(propertyModel_);
		connect(propertyModel_, SIGNAL( modelReset() ), this, SLOT( propertySelectionChanged() ));
		connect(tool_->propertyName_lv->selectionModel(),
				SIGNAL( selectionChanged(const QItemSelection &, const QItemSelection &) ),
				this,
				SLOT( propertySelectionChanged() ));
		QWidget* widget = propertyModel_->getWidget();
		tool_->propertyWidgets->addWidget(widget);
		widget->show();
		propertyModel_->gatherProperties();
		connect(propertyModel_, SIGNAL(log(Logtype,QString)), this, SLOT(slotLog(Logtype,QString)));
		connect(propertyModel_, SIGNAL(log(QString)), this, SLOT(slotLog(QString)));
	}
	else
	{
		tool_->propertyName_lv->setModel(0);
	}
}

//-----------------------------------------------------------------------------

void PropertyVisPlugin::slotMeshChanged(int /*_index*/)
{
	int id = tool_->meshNames->itemData( tool_->meshNames->currentIndex() ).toInt();
	setNewPropertyModel(id);
}

//-----------------------------------------------------------------------------

void PropertyVisPlugin::slotVisualize()
{
	using namespace PluginFunctions;

	// return if nothing is selected
	if (propertyModel_ == 0) return;

	int selectedId = tool_->meshNames->itemData( tool_->meshNames->currentIndex() ).toInt();
	QModelIndexList selectedIndices = tool_->propertyName_lv->selectionModel()->selectedIndexes();

	// visualize property
	propertyModel_->visualize(selectedIndices);

	// emit updates
	emit updateView();

	if (selectedId >= 0)
	{
		emit updatedObject( selectedId, UPDATE_COLOR );
	}
	else
	{
		ObjectIterator o_it(ALL_OBJECTS, supportedDataTypes());
		while (o_it != objectsEnd())
		{
			emit updatedObject( o_it->id(), UPDATE_COLOR );
			++o_it;
		}
	}
}

//-----------------------------------------------------------------------------

void PropertyVisPlugin::slotMouseEvent( QMouseEvent* _event ) {
	if (propertyModel_ != 0)
		propertyModel_->mouseEvent(_event);
}

//-----------------------------------------------------------------------------

void PropertyVisPlugin::slotDuplicateProperty()
{
	using namespace PluginFunctions;

	if (propertyModel_ != 0)
	{
		QModelIndexList selectedIndices = tool_->propertyName_lv->selectionModel()->selectedIndexes();
		propertyModel_->duplicateProperty(selectedIndices);

		emit updateView();
		int id = tool_->meshNames->itemData( tool_->meshNames->currentIndex() ).toInt();
		slotMeshChanged();

		if (id >= 0)
		{
			emit updatedObject( id, UPDATE_ALL );
		}
		else
		{
			ObjectIterator o_it(ALL_OBJECTS, supportedDataTypes());
			while (o_it != objectsEnd())
			{
				emit updatedObject( o_it->id(), UPDATE_ALL );
				++o_it;
			}
		}
	}
}

void PropertyVisPlugin::slotRemoveProperty()
{
	using namespace PluginFunctions;

	if (propertyModel_ != 0)
	{
		QModelIndexList selectedIndices = tool_->propertyName_lv->selectionModel()->selectedIndexes();
		propertyModel_->removeProperty(selectedIndices);

		emit updateView();
		int id = tool_->meshNames->itemData( tool_->meshNames->currentIndex() ).toInt();

		if (id >= 0)
		{
			emit updatedObject( id, UPDATE_ALL );
		}
		else
		{
			ObjectIterator o_it(ALL_OBJECTS, supportedDataTypes());
			while (o_it != objectsEnd())
			{
				emit updatedObject( o_it->id(), UPDATE_ALL );
				++o_it;
			}
		}
	}
}

#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2( propertyvisplugin , PropertyVisPlugin );
#endif

