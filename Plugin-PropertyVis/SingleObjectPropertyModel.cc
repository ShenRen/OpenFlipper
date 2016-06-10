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

#include "SingleObjectPropertyModel.hh"

SingleObjectPropertyModel::SingleObjectPropertyModel(QObject *parent)
    : PropertyModel(parent),
      widgets(0),
      currentlySelectedIndices(),
      currentlyVisualizedIndices()
{
    gatherProperties();
    QVBoxLayout* layout = new QVBoxLayout();
    widgets = new QWidget();
    widgets->setLayout(layout);
}

SingleObjectPropertyModel::~SingleObjectPropertyModel()
{
    for (unsigned int i = 0; i < propertyVisualizers.size(); i++)
        delete propertyVisualizers[i];
}

/**
 * @brief Visualizes the selected properties.
 * @param selectedIndices The
 */
void SingleObjectPropertyModel::visualize(QModelIndexList selectedIndices, QWidgetList widgets)
{
	for (int i = 0; i < selectedIndices.size(); ++i)
	{
		const int row = selectedIndices[i].row();

		if (widgets.empty())
		{
			propertyVisualizers[row]->visualize(true, 0);
		}
		else
		{
			propertyVisualizers[row]->visualize(true, widgets[i]);
		}

        //delete index and reinsert so it is the last element.
        std::vector<unsigned int>& vec = currentlyVisualizedIndices;
        vec.erase(std::remove(vec.begin(), vec.end(), row), vec.end());
        vec.push_back(row);
	}
}

void SingleObjectPropertyModel::removeProperty(QModelIndexList selectedIndices)
{
    std::vector<unsigned int> deleteIndices;

    for (QModelIndexList::Iterator it = selectedIndices.begin(); it != selectedIndices.end(); ++it)
    {
        propertyVisualizers[it->row()]->removeProperty();
        QWidget* w = propertyVisualizers[it->row()]->getWidget();
        widgets->layout()->removeWidget(w);
        delete propertyVisualizers[it->row()];
        propertyVisualizers[it->row()] = 0;
        deleteIndices.push_back(it->row());
    }

    std::sort(deleteIndices.begin(), deleteIndices.end());

    for (int i = deleteIndices.size()-1; i >= 0; i--)
    {
        for (int j = currentlyVisualizedIndices.size()-1; j >= 0; j--)
        {
            if (currentlyVisualizedIndices[j] == deleteIndices[i])
                //erase so the deleted property will not be revisualized on updateObject
                currentlyVisualizedIndices.erase(currentlyVisualizedIndices.begin() + j);
            else if (currentlyVisualizedIndices[j] > deleteIndices[i])
                //decrease index by one since the index of all property visualizers in propertyVisualizers
                //shifts by one for each property visualizer that gets deleted in front of them
                currentlyVisualizedIndices[j]--;
        }
    }

    for (int i = deleteIndices.size()-1; i >= 0; i--){
        propertyVisualizers.erase(propertyVisualizers.begin() + deleteIndices[i]);
    }
}

void SingleObjectPropertyModel::duplicateProperty(QModelIndexList selectedIndices)
{
    for (QModelIndexList::Iterator it = selectedIndices.begin(); it != selectedIndices.end(); ++it)
        propertyVisualizers[it->row()]->duplicateProperty();
}

void SingleObjectPropertyModel::clear(QModelIndexList selectedIndices) {
    for (QModelIndexList::Iterator it = selectedIndices.begin(); it != selectedIndices.end(); ++it)
    {
        propertyVisualizers[it->row()]->clear();
        std::vector<unsigned int>& vec = currentlyVisualizedIndices;
        vec.erase(std::remove(vec.begin(), vec.end(), it->row()), vec.end());
    }
}

void SingleObjectPropertyModel::updateWidget(const QModelIndexList& selectedIndices)
{
    QLayout* layout = widgets->layout();

    currentlySelectedIndices = selectedIndices;

    for (unsigned int i = 0; i < propertyVisualizers.size(); i++)
    {
        propertyVisualizers[i]->getWidget()->hide();
    }

    for (QModelIndexList::const_iterator it = selectedIndices.begin(), it_end = selectedIndices.end();
                it != it_end; ++it) {
        QWidget* widget = propertyVisualizers[it->row()]->getWidget();
        layout->addWidget(widget);
        widget->show();
    }
    widgets->setLayout(layout);
}

void SingleObjectPropertyModel::connectLogs(PropertyVisualizer* propViz)
{
    connect(propViz, SIGNAL(log(QString)), this, SLOT(slotLog(QString)));
    connect(propViz, SIGNAL(log(Logtype, QString)), this, SLOT(slotLog(Logtype, QString)));
}

int SingleObjectPropertyModel::rowCount(const QModelIndex & parent) const {
    return propertyVisualizers.size();
}

QVariant SingleObjectPropertyModel::data(const QModelIndex & index, int role) const {
    switch (role) {
        case Qt::DisplayRole:
            return propertyVisualizers[index.row()]->getName();
        default:
            return QVariant::Invalid;
    }
}

QVariant SingleObjectPropertyModel::headerData(int section, Qt::Orientation orientation, int role) const {
    switch (role) {
        case Qt::DisplayRole:
            return tr("Some header. %1 %2").arg(section).arg(orientation);
            break;
        default:
            return QAbstractListModel::headerData(section, orientation, role);
    }
}

void SingleObjectPropertyModel::objectUpdated()
{
    for (unsigned int i = 0; i < currentlyVisualizedIndices.size(); i++)
    {
        propertyVisualizers[currentlyVisualizedIndices[i]]->visualize(false, 0);
    }
}

QString SingleObjectPropertyModel::getLoadFilename()
{
  QString filter = getLoadFilenameFilter();
  QString selected_filter = tr("All Files (*)");

  QString fileName = QFileDialog::getOpenFileName(0, tr("Load Property"), QString(), filter, &selected_filter);

  return fileName;
}

QString SingleObjectPropertyModel::getLoadFilenameFilter()
{
    return tr("");
}

QString SingleObjectPropertyModel::getSaveFilename(unsigned int propId)
{
  QString filter(getSaveFilenameFilter(propId));
  QString defaultName = getDefaultSaveFilename(propId);

  QString fileName = QFileDialog::getSaveFileName(0, tr("Save Property"), defaultName, filter);

  return fileName;
}

QString SingleObjectPropertyModel::getSaveFilenameFilter(unsigned int propId)
{
    QString filter= tr("All Files (*)");

    return filter;
}

QString SingleObjectPropertyModel::getDefaultSaveFilename(unsigned int propId)
{
    PropertyVisualizer* propViz = propertyVisualizers[propId];

    QString name = tr(propViz->getPropertyInfo().propName().c_str());

    if (propViz->getPropertyInfo().isVertexProp())
        name += tr(".vprop");
    else if (propViz->getPropertyInfo().isHalfedgeProp())
        name += tr(".hprop");
    else if (propViz->getPropertyInfo().isEdgeProp())
        name += tr(".eprop");
    else if (propViz->getPropertyInfo().isFaceProp())
        name += tr(".fprop");
    else if (propViz->getPropertyInfo().isHalffaceProp())
        name += tr(".hfprop");
    else if (propViz->getPropertyInfo().isCellProp())
        name += tr(".cprop");

    return name;
}

void SingleObjectPropertyModel::openFile(QString _filename, QFile& file_, QTextStream*& file_stream_)
{
  closeFile(file_, file_stream_);
  file_.setFileName(_filename);
  if(file_.open(QIODevice::ReadWrite | QIODevice::Text))
    file_stream_ = new QTextStream(&file_);
}

void SingleObjectPropertyModel::closeFile(QFile& file_, QTextStream*& file_stream_)
{
  if( file_stream_)
  {
    delete file_stream_;
    file_stream_ = 0;
  }
  if(file_.exists())
  {
      file_.close();
  }
}

QString SingleObjectPropertyModel::readLine(QTextStream* file_stream_)
{
    if(file_stream_)
    {
      QString s;
      s = file_stream_->readLine();
      return s;
    }
    else return QString("");
}

void SingleObjectPropertyModel::writeLine(QString _s, QTextStream*& file_stream_)
{
  if(file_stream_)
  {
    (*file_stream_) << _s << '\n';
  }
  else std::cerr << "Warning: filestream not available...\n";
}

void SingleObjectPropertyModel::saveProperty(unsigned int propId)
{
    PropertyVisualizer* propVis = propertyVisualizers[propId];

    QString filename = getSaveFilename(propId);
    if (filename == "") return;

    QFile file_;
    QTextStream* file_stream_ = 0;

    openFile(filename, file_, file_stream_);

    QString header = propVis->getHeader();
    writeLine(header, file_stream_);

    int n = propVis->getEntityCount();

    for (int i = 0; i < n; ++i)
    {
        QString propertyText = propVis->getPropertyText(i);
        writeLine(propertyText, file_stream_);
    }

    closeFile(file_, file_stream_);
}

void SingleObjectPropertyModel::loadProperty()
{

    QString filename = getLoadFilename();
    if (filename == "") return;

    QFile file_;
    QTextStream* file_stream_ = 0;

    openFile(filename, file_, file_stream_);

    QString header = readLine(file_stream_);

    PropertyVisualizer* propVis;
    unsigned int n;


    if (parseHeader(header, propVis, n))
    {
        setPropertyFromFile(file_stream_, n, propVis);
    }
    else
    {
        emit log("Property could not be loaded.");
    }

    closeFile(file_, file_stream_);
}

void SingleObjectPropertyModel::setPropertyFromFile(QTextStream*& file_stream_, unsigned int n, PropertyVisualizer* propVis)
{
    for (unsigned int i = 0; i < n; ++i)
    {
        QString propertyText = readLine(file_stream_);
        propVis->setPropertyFromText(i, propertyText);
    }
}

PropertyVisualizer* SingleObjectPropertyModel::getPropertyVisualizer(QString propName, PropertyInfo::ENTITY_FILTER filter, TypeInfoWrapper typeInfo)
{
    for (unsigned int i = 0; i < propertyVisualizers.size(); ++i)
    {
        const PropertyInfo& propInfo =  propertyVisualizers[i]->getPropertyInfo();
        if ((propInfo.entityType() == filter) && (QString::compare(tr(propInfo.propName().c_str()), propName)==0) && (propInfo.typeinfo() == typeInfo))
            return propertyVisualizers[i];
    }
    return 0;
}

bool SingleObjectPropertyModel::isPropertyFree(QString propName, PropertyInfo::ENTITY_FILTER filter, TypeInfoWrapper typeInfo)
{
    return getPropertyVisualizer(propName, filter, typeInfo) == 0;
}

PropertyInfo SingleObjectPropertyModel::getPropertyInfo(const QModelIndex index) const
{
	return propertyVisualizers[index.row()]->getPropertyInfo();
}
