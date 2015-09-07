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

#ifndef MULTI_OBJECT_PROPERTY_MODEL_H
#define MULTI_OBJECT_PROPERTY_MODEL_H

#include "PropertyModel.hh"
#include "OpenFlipper/common/DataTypes.hh"

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
    const DataType datatypes;
    std::vector<QString> propNames;
    std::vector<PropertyInfo> propInfos;
    std::vector<QWidget*> propWidgets;
    QWidget* widget;
};

#endif /* MULTI_OBJECT_PROPERTY_MODEL_H */
