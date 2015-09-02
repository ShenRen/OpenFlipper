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
*   $Revision: 21016 $                                                       *
*   $LastChangedBy: schultz $                                                *
*   $Date: 2015-07-16 16:48:42 +0200 (Thu, 16 Jul 2015) $                     *
*                                                                            *
\*===========================================================================*/

#ifndef SINGLE_OBJECT_PROPERTY_MODEL_H
#define SINGLE_OBJECT_PROPERTY_MODEL_H

#include "PropertyModel.hh"
#include "PropertyVisualizer.hh"

#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/PickingInterface.hh>
#include <OpenFlipper/BasePlugin/MouseInterface.hh>

#include <vector>
#include <iostream>
#include <set>
#include <typeinfo>

#include <QPushButton>

#include <Widgets/VectorWidget.hh>


/*! \class SingleObjectPropertyModel
 *  \brief This class manages the visualizers for a single object.
 *
 * For each mesh object a SingleObjectPropertyModel is created. It searches for properties and
 * creates a PropertyVisualizer for each of them. It provides the GUI for all selected properties.
 * Also loading and saving of properties is handled here.
 *
 * Note that for each type of object (OpenMesh, OpenVolumeMesh and others that might follow)
 * a subclass should be derived from this class.
 */
class SingleObjectPropertyModel: public PropertyModel
{
    Q_OBJECT

signals:
    void log(Logtype _type, QString _message);
    void log(QString _message);

private slots:
    void slotLog(Logtype _type, QString _message){ emit log(_type, _message); }
    void slotLog(QString _message){ emit log(_message);}

public:
    virtual void pickModeChanged(const std::string& _mode) {}
    virtual void mouseEvent(QMouseEvent* _event) {}

    SingleObjectPropertyModel(QObject *parent = 0);
    virtual ~SingleObjectPropertyModel();

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
    virtual void gatherProperties(){ /* implemented by subclass */ }

    /// Clears the selected property visualization.
    virtual void clear(QModelIndexList selectedIndices);

    /// Hides the widget.
    inline void hideWidget()    { widgets->hide(); }

    /// Returns the widget.
    inline QWidget* getWidget() { return widgets;  }

    /// Updates the widget
    virtual void updateWidget(const QModelIndexList& selectedIndices);

    /// Connects the PropertyVisualizer log signals with the log slot.
    void connectLogs(PropertyVisualizer* propViz);

	/// Returns the property info for the property with the given index.
	virtual PropertyInfo getPropertyInfo(const QModelIndex index) const;


protected:

    /// Returns a PropertyVisualizer.
    PropertyVisualizer* getPropertyVisualizer(QString propName, PropertyInfo::ENTITY_FILTER filter, TypeInfoWrapper typeInfo);

    /// Checks if the property name is still available.
    bool isPropertyFree(QString propName, PropertyInfo::ENTITY_FILTER filter, TypeInfoWrapper typeInfo);


    /// Asks the user for a file to load.
    QString getLoadFilename();

    /// Returns the filename filter for loading.
    virtual QString getLoadFilenameFilter();

    /// Asks the user for a file to load.
    QString getSaveFilename(unsigned int propId);

    /// Returns the filename filter for saving.
    virtual QString getSaveFilenameFilter(unsigned int propId);

    /// Returns the default file name
    virtual QString getDefaultSaveFilename(unsigned int propId);

    /// Opens a file.
    void openFile(QString _filename, QFile &file_, QTextStream *&file_stream_);

    /// Closes a file.
    void closeFile(QFile& file_, QTextStream*& file_stream_);

    /// Read line from a file.
    QString readLine(QTextStream *file_stream_);

    /// Writes a line to a file.
    void writeLine(QString _s, QTextStream *&file_stream_);


    /// Saves property.
    void saveProperty(unsigned int propId);

    /// Loads property.
    void loadProperty();

    /// Sets the property values from a given file.
    virtual void setPropertyFromFile(QTextStream*& file_stream_, unsigned int n, PropertyVisualizer *propVis);

    /**
     * @brief Parses the property file header.
     *
     * @param header The header.
     * @param[out] propVis The PropertyVisualizer that will visualize the new property.
     * @param[out] n The number of values stored in the file
     * @return True if parsing was successfull, False if not.
     *
     * When loading a property from a file this method parses the header placed in the file's first line. If parsing
     * was successfull a new property and a PropertyVisualizer will be created.
     */
    virtual bool parseHeader(QString header, PropertyVisualizer*& propVis, unsigned int& n){ return false; }

protected:
    std::vector<PropertyVisualizer*> propertyVisualizers;
    QWidget* widgets;
    QModelIndexList currentlySelectedIndices;
    std::vector<unsigned int> currentlyVisualizedIndices;
};

#endif /* SINGLE_OBJECT_PROPERTY_MODEL_H */
