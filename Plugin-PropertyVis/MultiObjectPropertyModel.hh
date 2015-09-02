#ifndef MULTI_OBJECT_PROPERTY_MODEL_H
#define MULTI_OBJECT_PROPERTY_MODEL_H

#include "PropertyModel.hh"

class PropertyVisualizer;

class MultiObjectPropertyModel: public PropertyModel
{
    Q_OBJECT

public:
	MultiObjectPropertyModel(const QStringList& res, QObject *parent = 0);
	virtual ~MultiObjectPropertyModel();

	virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    /// Revisualizes visualized properties.
    virtual void objectUpdated();

    /// Visualizes the selected properties.
    virtual void visualize(QModelIndexList selectedIndices, QWidgetList widgets = QWidgetList());

    /// Removes the selected properties.
    virtual void removeProperty(QModelIndexList selectedIndices);

    /// Duplicates the selected properties.
    virtual void duplicateProperty(QModelIndexList selectedIndices);

    /// Searches for properties and creates PropertyVisualizers.
    virtual void gatherProperties();

    /// Clears the selected property visualization.
    virtual void clear(QModelIndexList selectedIndices);

    /// Hides the widget.
    virtual void hideWidget();

    /// Returns the widget.
    virtual QWidget* getWidget();

    /// Updates the widget
    virtual void updateWidget(const QModelIndexList& selectedIndices);

    /// Connects the PropertyVisualizer log signals with the log slot.
    virtual void connectLogs(PropertyVisualizer* propViz);

	/// Returns the property info for the property with the given index.
	virtual PropertyInfo getPropertyInfo(const QModelIndex index) const;

private:
	QWidget* createWidgetForType(const TypeInfoWrapper& info) const;
	void setRange(const PropertyInfo& info, QWidget* widget) const;

private:
	const QStringList restriction;
    std::vector<QString> propNames;
	std::vector<PropertyInfo> propInfos;
	std::vector<QWidget*> propWidgets;
	QWidget* widget;
};

#endif /* MULTI_OBJECT_PROPERTY_MODEL_H */
