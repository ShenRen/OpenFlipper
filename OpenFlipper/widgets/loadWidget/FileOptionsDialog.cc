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

#include "FileOptionsDialog.hh"

#include <OpenFlipper/common/GlobalOptions.hh>

#include <QVBoxLayout>
#include <QGridLayout>

FileOptionsDialog::FileOptionsDialog(std::vector<fileTypes>& _supportedTypes, QStringList _extensions, bool _loadMode, QWidget *parent)
  : QDialog(parent),
    ext_(_extensions),
    supportedTypes_(_supportedTypes),
    loadMode_(_loadMode),
    defaultPluginBox_(NULL)
{

  ext_.removeDuplicates();
  
  QGridLayout* grid = new QGridLayout;
  
  QVector< int > usefulPlugins;
  
  //check if more than one plugin is able to handle this extension
  for (int i=0; i < ext_.size(); i++){
    int count = 0;
    QStringList names;
    
    for (unsigned int t=0; t < supportedTypes_.size(); t++){
      
      QString filters;
      
      if (loadMode_)
        filters = supportedTypes_[t].loadFilters;
      else
        filters = supportedTypes_[t].saveFilters;

      // Only take the parts inside the brackets
      filters = filters.section("(",1).section(")",0,0);

      // Split into blocks
      QStringList separateFilters = filters.split(" ");

      for ( int filterId = 0 ; filterId < separateFilters.size(); ++filterId ) {
        separateFilters[filterId] = separateFilters[filterId].trimmed();

        if (separateFilters[filterId].endsWith("." + ext_[i],Qt::CaseInsensitive)){
          count++;
          names.push_back( supportedTypes_[t].name );
          usefulPlugins.push_back( t );
          continue;
        }

      }

    }
    
    //more than one plugin can handle the extension
    if (count > 1){
      QLabel*  label = new QLabel( tr("For *.%1 use plugin ").arg(ext_[i]) );
      QComboBox* box = new QComboBox();

      defaultPluginBox_ = new QCheckBox(tr("Make this plugin the default"));
      
      box->addItems(names);
      box->setAccessibleName(ext_[i]);

      currentName_ = box->currentText();
      currentExtension_ = ext_[i];

      grid->addWidget(label, grid->rowCount(), 0);
      grid->addWidget(box,   grid->rowCount()-1, 1);
      grid->addWidget(defaultPluginBox_, grid->rowCount()+1, 0);
      
      connect(box, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotPluginChanged(QString)) );
      connect(defaultPluginBox_, SIGNAL(stateChanged(int)), this, SLOT(slotPluginDefault(int)) );
      boxes_.push_back(box);
    }
  }
  
  //force TriangleMesh as initial value if available
  for (int i=0; i < boxes_.count(); i++){
    
    for (int t=0; t < (boxes_[i])->count(); t++)
      if ( (boxes_[i])->itemText(t).contains("TriangleMesh") ){
        (boxes_[i])->setCurrentIndex(t);
        break;
      }
  }
  
  QGroupBox* group = new QGroupBox(tr("Extensions with multiple plugins"));
  group->setLayout(grid);
  
  if (boxes_.count() == 0)
    group->setVisible(false);

  
  //add option widgets from all fileplugins
  for (unsigned int t=0; t < supportedTypes_.size(); t++){
    
    if ( !usefulPlugins.contains(t) ) 
      continue;
    
    QWidget* widget;

    if (loadMode_)
      widget = supportedTypes_[t].plugin->loadOptionsWidget("");
    else
      widget = supportedTypes_[t].plugin->saveOptionsWidget("");

    if (widget != 0)
      tabs_.addTab(widget, supportedTypes_[t].name);
  }

  //add buttons at bottom
  QPushButton* cancel = new QPushButton(tr("&Cancel"));
  QPushButton* ok = new QPushButton(tr("&Ok"));
  
  QHBoxLayout* buttonLayout = new QHBoxLayout;
  
  buttonLayout->addWidget(cancel);
  buttonLayout->addStretch();
  buttonLayout->addWidget(ok);

  QVBoxLayout* layout = new QVBoxLayout;
  layout->addWidget(group);
  layout->addWidget(&tabs_);
  layout->addLayout(buttonLayout);
  
  setLayout(layout);
  
  connect( cancel, SIGNAL(clicked()), this, SLOT(reject()) );
  connect(     ok, SIGNAL(clicked()), this, SLOT(accept()) );
  
}

FileOptionsDialog::~FileOptionsDialog()
{
  //remove tabs
  for (int i=tabs_.count()-1; i >= 0; i--)
    tabs_.removeTab(i);
  
  //and set parent of widgets to NULL
  for (unsigned int t=0; t < supportedTypes_.size(); t++){
    
    QWidget* widget;
    
    if (loadMode_)
      widget = supportedTypes_[t].plugin->loadOptionsWidget("");
    else
      widget = supportedTypes_[t].plugin->saveOptionsWidget("");

    if (widget != 0)
      widget->setParent(0);
  }
}

int FileOptionsDialog::exec(){
  
  if ( tabs_.count() == 0 && boxes_.count() == 0 )
    return QDialog::Accepted;
  else {
    return QDialog::exec();
  }
}

bool FileOptionsDialog::makePluginDefault() {
  if (!defaultPluginBox_)
    return false;

  return defaultPluginBox_->isChecked();
}

void FileOptionsDialog::slotPluginChanged(QString _name){
  
  QComboBox* box = dynamic_cast<QComboBox*>(QObject::sender());

  for (unsigned int t=0; t < supportedTypes_.size(); t++)
    if ( supportedTypes_[t].name == _name ){

      currentName_ = _name;
      currentExtension_ = box->accessibleName();

      if (makePluginDefault()) {
        OpenFlipperSettings().setValue(QString("Core/File/DefaultLoader/").append(currentExtension_), currentName_);
      }

      emit setPluginForExtension(box->accessibleName(), t ); //accessibleName contains the extension
      break;
    }
}

void FileOptionsDialog::slotPluginDefault(int _state) {
  // store the name of the default plugin for loading
  if (_state == Qt::Checked) {
    OpenFlipperSettings().setValue(QString("Core/File/DefaultLoader/").append(currentExtension_), currentName_);
  } else {
    OpenFlipperSettings().setValue(QString("Core/File/DefaultLoader/").append(currentExtension_), "");
  }
}

