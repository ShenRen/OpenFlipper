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


#include "Core.hh"

#include <ObjectTypes/Light/Light.hh>

/// Save Settings (slot is called from CoreWidget's File-Menu)
void Core::saveSettings(){

  // ========================================================================================
  // generate the saveSettings-Dialog
  // ========================================================================================

  QFileDialog fileDialog( coreWidget_,
                          tr("Save Settings"),
                          OpenFlipperSettings().value("Core/CurrentDir").toString(),
                          tr("INI files (*.ini);;OBJ files (*.obj )") );

  fileDialog.setOption (QFileDialog::DontUseNativeDialog, true);
  fileDialog.setAcceptMode ( QFileDialog::AcceptSave );
  fileDialog.setFileMode ( QFileDialog::AnyFile );

  QGridLayout *layout = (QGridLayout*)fileDialog.layout();

  QGroupBox* optionsBox = new QGroupBox( &fileDialog ) ;
  optionsBox->setSizePolicy( QSizePolicy ( QSizePolicy::Expanding , QSizePolicy::Preferred ) );
  optionsBox->setTitle(tr("Options"));
  layout->addWidget( optionsBox, layout->rowCount() , 0 , 1,layout->columnCount() );

  QCheckBox *saveProgramSettings = new QCheckBox(optionsBox);
  saveProgramSettings->setText(tr("Save program settings"));
  saveProgramSettings->setToolTip(tr("Save all current program settings to the file ( This will include view settings, colors,...) "));
  saveProgramSettings->setCheckState( Qt::Unchecked );

  QCheckBox *savePluginSettings = new QCheckBox(optionsBox);
  savePluginSettings->setText(tr("Save per Plugin Settings"));
  savePluginSettings->setToolTip(tr("Plugins should add their current global settings to the file"));
  savePluginSettings->setCheckState( Qt::Checked );

  QCheckBox *saveObjectInfo = new QCheckBox(optionsBox);
  saveObjectInfo->setText(tr("Save open object information to the file"));
  saveObjectInfo->setToolTip(tr("Save all open Objects and add them to the settings file ( they will be loaded if opening the settings file"));
  saveObjectInfo->setCheckState( Qt::Checked );

  QCheckBox *saveAllBox = new QCheckBox(optionsBox);
  saveAllBox->setText(tr("Save everything to same folder"));
  saveAllBox->setToolTip(tr("Save all open files to the same folder as the ini file"));
  saveAllBox->setCheckState( Qt::Checked );

  QCheckBox *askOverwrite = new QCheckBox(optionsBox);
  askOverwrite->setText(tr("Ask before overwriting files"));
  askOverwrite->setToolTip(tr("If a file exists you will get asked what to do"));
  askOverwrite->setCheckState( Qt::Checked );

  QCheckBox *targetOnly = new QCheckBox(optionsBox);
  targetOnly->setText(tr("Save only target objects"));
  targetOnly->setToolTip(tr("Only objects with target flag will be handled"));
  targetOnly->setCheckState( Qt::Unchecked );

  QBoxLayout* frameLayout = new QBoxLayout(QBoxLayout::TopToBottom,optionsBox);
  frameLayout->addWidget( saveProgramSettings , 0 , 0);
  frameLayout->addWidget( savePluginSettings  , 1 , 0);
  frameLayout->addWidget( saveObjectInfo      , 2 , 0);
  frameLayout->addWidget( saveAllBox          , 3 , 0);
  frameLayout->addWidget( askOverwrite        , 4 , 0);
  frameLayout->addWidget( targetOnly          , 5 , 0);
  frameLayout->addStretch();

  fileDialog.resize(550 ,600);

  // ========================================================================================
  // show the saveSettings-Dialog and get the target file
  // ========================================================================================
  QStringList fileNames;
  if (fileDialog.exec()) {
    fileNames = fileDialog.selectedFiles();
  } else {
    return;
  }

  if ( fileNames.size() > 1 ) {
    std::cerr << "Too many save filenames selected" << std::endl;
    return;
  }

  QString complete_name = fileNames[0];

  //check the extension if its a known one
  if ( !complete_name.endsWith(".ini", Qt::CaseInsensitive) && !complete_name.endsWith(".obj", Qt::CaseInsensitive) ){

    // If its unknown, get the type from the currently selected filter and add this extension to the filename
    if ( fileDialog.selectedNameFilter().contains(tr("INI files (*.ini)")) )
      complete_name += ".ini";
    else
      complete_name += ".obj";

  }

  bool is_saveObjectInfo = saveObjectInfo->isChecked();
  bool is_targetOnly = targetOnly->isChecked();
  bool is_saveAll = saveAllBox->isChecked();
  bool is_askOverwrite = askOverwrite->isChecked();
  bool is_saveProgramSettings = saveProgramSettings->isChecked();
  bool is_savePluginSettings = savePluginSettings->isChecked();

  saveSettings(complete_name, is_saveObjectInfo, is_targetOnly, is_saveAll, is_askOverwrite, is_saveProgramSettings, is_savePluginSettings);
}

void Core::saveSettings(QString complete_name, bool is_saveObjectInfo, bool is_targetOnly, bool is_saveAll,
                        bool is_askOverwrite, bool is_saveProgramSettings, bool is_savePluginSettings){
  // Get the chosen directory and remember it.
  QFileInfo fileInfo(complete_name);
  OpenFlipperSettings().setValue("Core/CurrentDir", fileInfo.absolutePath() );

  // ========================================================================================
  // update status information
  // ========================================================================================
  OpenFlipper::Options::savingSettings(true);

  if ( OpenFlipper::Options::gui() ) {
    coreWidget_->statusMessage( tr("Saving Settings to ") + complete_name + " ...");
    coreWidget_->setStatus(ApplicationStatus::BLOCKED );
  }

  // ========================================================================================
  // Save the objects itself
  // ========================================================================================
  // Depending on the checkbox iterate over all objects or only the selected ones.

  // Memorize saved files new file names
  std::map<int,QString> savedFiles;

  if ( is_saveObjectInfo ) {

    PluginFunctions::IteratorRestriction restriction;
    if ( is_targetOnly )
      restriction = PluginFunctions::TARGET_OBJECTS;
    else
      restriction = PluginFunctions::ALL_OBJECTS;

    // Store saved file's original names (in order to get number of duplicates)
    std::multiset<QString> originalFiles;

    // Store default extensions per type
    std::map<DataType,QString> defaultExtensions;
    // get the supported extensions for when no extension is given
    QMultiMap<DataType,QString> allFilters; // type -> supported extension
    const std::vector<fileTypes>& types = supportedTypes();
    for (int i=0; i < (int)types.size(); i++) {
      QString filters = types[i].saveFilters;

      // only take the actual extensions
      filters = filters.section("(",1).section(")",0,0);
      if (filters.trimmed() == "")
        continue;

      QStringList separateFilters = filters.split(" ");
      bool found = false;
      for ( int filterId = 0 ; filterId < separateFilters.size(); ++filterId ) {
        if (separateFilters[filterId].trimmed() == "")
          continue;

        found = true;
        allFilters.insert(types[i].type,separateFilters[filterId]);
      }

      if (!found)
        allFilters.insert(types[i].type,filters);
    }

    // create a dialog to set extensions if none are given once
    QDialog extensionDialog(coreWidget_, Qt::Dialog);
    QGridLayout extensionLayout;
    const QString extensionCheckBoxPrefixString = "Apply extension to all Objects without preset extensions with DataType: ";
    QCheckBox extensionCheckBox;
    QComboBox extensionComboBox;
    QDialogButtonBox extensionButtons(QDialogButtonBox::Ok);
    QDialogButtonBox::connect(&extensionButtons, SIGNAL(accepted()), &extensionDialog, SLOT(accept()));
    //extensionComboBox.addItems(allFilters);
    extensionLayout.addWidget(&extensionComboBox);
    extensionLayout.addWidget(&extensionCheckBox);
    extensionLayout.addWidget(&extensionButtons);
    extensionDialog.setLayout(&extensionLayout);

    //Iterate over opened objects and save them
    for ( PluginFunctions::ObjectIterator o_it(restriction);
                                          o_it != PluginFunctions::objectsEnd(); ++o_it)
    {
      QString filename;

      if ( is_saveAll )
      {
        // Use path of settings file for all objects
        filename = fileInfo.absolutePath() + OpenFlipper::Options::dirSeparator() + o_it->name();
      }
      else
      {
        // Use objects own path if it has one. Otherwise also use path of settings file
        filename = o_it->path() + OpenFlipper::Options::dirSeparator() + o_it->name();

        // handle the case that the object was created in current session and not loaded from disk
        if (o_it->path() == ".") {
          filename = fileInfo.absolutePath() + OpenFlipper::Options::dirSeparator() + o_it->name();
          std::cerr << "newpath : " << fileInfo.absolutePath().toStdString() << std::endl;
          std::cerr << "name  : " << o_it->name().toStdString() << std::endl;
        }
      }

      // enforce that all files end with obj extension if its an obj-settings file
      if ( complete_name.endsWith("obj") )
      {
        if (!filename.endsWith("obj"))
        {
          // remove old extension
          int pos = filename.lastIndexOf(".");
          filename.remove(pos+1, filename.length() - pos);
          // add obj extension
          filename += "obj";
        }
      }
      
      // Don't save default light source objects
      LightObject* light = 0;
      PluginFunctions::getObject( o_it->id(), light );
      if(light != 0) {
          if(light->defaultLight()) continue;
      }

      // Store original file name
      originalFiles.insert(filename);

      // If a file with the same name already has been saved,
      // rename it.
      size_t c = originalFiles.count(filename);
      if(c > 1) {
          QFileInfo finfo(filename);
          filename = finfo.absolutePath() + OpenFlipper::Options::dirSeparator();
          filename += finfo.baseName() + QString("_%1").arg(c-1) + ".";
          filename += finfo.completeSuffix();
      }

      // check if the name of the object specifies already the extension
      bool extInName = false;
      for (QMultiMap<DataType,QString>::const_iterator e_it = allFilters.begin(); e_it != allFilters.end(); ++e_it)
      {
        // suffix is the same as one extension and
        extInName = e_it.key().contains(o_it->dataType()) && e_it.value() == QString("*.")+QFileInfo(filename).suffix();
        if (extInName)
          break;
      }

      if (!extInName)
      {
        // search for the default data type
        std::map<DataType,QString>::const_iterator defaultExtIt = defaultExtensions.find(o_it->dataType());
        bool useDefault = defaultExtIt != defaultExtensions.end();
        QString extension = (useDefault) ? defaultExtIt->second : "";

        // if no default extension for the datatype was given, request one
        if (!useDefault)
        {
          // present only those filters, which support the type
          QStringList supportedFilters;
          for (QMultiMap<DataType,QString>::const_iterator it = allFilters.begin(); it != allFilters.end() ; ++it)
          {
            if (it.key().contains(o_it->dataType()))
              supportedFilters.append(it.value());
          }

          extensionComboBox.clear();
          extensionComboBox.addItems(supportedFilters);
          extensionDialog.setWindowTitle("Please specify a file extension for " + o_it->name());
          extensionCheckBox.setText(extensionCheckBoxPrefixString + typeName(o_it->dataType()));
          extensionDialog.move(coreWidget_->width()/2 - extensionDialog.width(),
                        coreWidget_->height()/2 - extensionDialog.height());

          if (extensionDialog.exec() && !supportedFilters.isEmpty())
          {
            extension = extensionComboBox.currentText();
            extension = QFileInfo(extension).suffix();
            filename += "." + extension;
            if (extensionCheckBox.isChecked())
              defaultExtensions[o_it->dataType()] = extension;

          } else
          {
            emit log(LOGERR, tr("Unabel to save %1. No extension specified.").arg(o_it->name()));
            continue;
          }
        } else
        {
          filename += "." + extension;
        }
      }


      // decide whether to use saveObject or saveObjectTo
      if ( !QFile(filename).exists() || !is_askOverwrite )
        saveObject( o_it->id(), filename);
      else
        saveObjectTo(o_it->id(), filename);

      // Store saved file's name
      savedFiles.insert(std::pair<int,QString>(o_it->id(),filename));

    }
  }


  // ========================================================================================
  // Finally save all Settings
  // ========================================================================================
  if ( complete_name.endsWith("obj") ) {

    //write to obj
    writeObjFile(complete_name, is_saveAll, is_targetOnly, savedFiles);

  } else {
      // write to ini
      writeIniFile( complete_name,
                    is_saveAll,
                    is_targetOnly,
                    is_saveProgramSettings,
                    is_savePluginSettings,
                    is_saveObjectInfo,
                    savedFiles);
  }

  // update status
  OpenFlipper::Options::savingSettings(false);

  if ( OpenFlipper::Options::gui() ) {
    coreWidget_->statusMessage( tr("Saving Settings to ") + complete_name + tr(" ... Done"), 4000);
    coreWidget_->setStatus(ApplicationStatus::READY );
  }

  //add to recent files
  if ( OpenFlipper::Options::gui() )
    coreWidget_->addRecent( complete_name, DATA_UNKNOWN );

}
