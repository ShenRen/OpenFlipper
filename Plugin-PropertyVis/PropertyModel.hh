#ifndef PROPERTY_MODEL_H
#define PROPERTY_MODEL_H

#include "Utils.hh"
#include <QAbstractListModel>

class QMouseEvent;
class PropertyVisualizer;

class PropertyModel: public QAbstractListModel
{
    Q_OBJECT

public:
	virtual void pickModeChanged(const std::string& _mode) {}
    virtual void mouseEvent(QMouseEvent* _event) {}

	PropertyModel(QObject *parent = 0) : QAbstractListModel(parent) {}
	virtual ~PropertyModel() {}

    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const = 0;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const = 0;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const = 0;

    /// Revisualizes visualized properties.
    virtual void objectUpdated() = 0;

    /// Visualizes the selected properties.
    virtual void visualize(QModelIndexList selectedIndices, QWidgetList widgets = QWidgetList()) = 0;

    /// Removes the selected properties.
    virtual void removeProperty(QModelIndexList selectedIndices) = 0;

    /// Duplicates the selected properties.
    virtual void duplicateProperty(QModelIndexList selectedIndices) = 0;

    /// Searches for properties and creates PropertyVisualizers.
    virtual void gatherProperties() = 0;

    /// Clears the selected property visualization.
    virtual void clear(QModelIndexList selectedIndices) = 0;

    /// Hides the widget.
    virtual void hideWidget() = 0;

    /// Returns the widget.
    virtual QWidget* getWidget() = 0;

    /// Updates the widget
    virtual void updateWidget(const QModelIndexList& selectedIndices) = 0;

    /// Connects the PropertyVisualizer log signals with the log slot.
    virtual void connectLogs(PropertyVisualizer* propViz) = 0;

	/// Returns the property info for the property with the given index.
	virtual PropertyInfo getPropertyInfo(const QModelIndex index) const = 0;

	/// Returns the index of the property with the given name.
	QModelIndex indexFromPropName(const QString& propName) const
	{
		for (int i = 0; i < rowCount(); ++i) {
			const QModelIndex idx = index(i, 0);
			const QString name = idx.data().toString();
			if (name == propName) return idx;
		}
		return QModelIndex();
	}
};

#endif /* PROPERTY_MODEL_H */
