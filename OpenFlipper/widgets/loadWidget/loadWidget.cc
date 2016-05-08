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




#include "loadWidget.hh"

#include "FileOptionsDialog.hh"

#include <QFileInfo>

LoadWidget::LoadWidget(std::vector<fileTypes>& _supportedTypes , QWidget *parent)
  : QFileDialog(parent),
    loadMode_(true),
    supportedTypes_(_supportedTypes)
{
  setOption (QFileDialog::DontUseNativeDialog, true);

  // Get our layout
  QGridLayout *gridLayout = (QGridLayout*)layout();

  //supported Types
  optionsBox_ = new QCheckBox(tr("use defaults"), this);
  optionsBox_->setChecked( OpenFlipperSettings().value("Core/File/UseLoadDefaults",false).toBool() );
  
  // add the options box to the bottom
  gridLayout->addWidget( optionsBox_, gridLayout->rowCount() , 1 );

  // Add a very nice label for it
  QLabel* typeLabel = new QLabel(tr("Options:") , this);
  gridLayout->addWidget( typeLabel, gridLayout->rowCount() -1 , 0 );

  //overwrite dialog shouldn't be handled by the qfiledialog
  setConfirmOverwrite(false);

  setDirectory( OpenFlipperSettings().value("Core/CurrentDir").toString() );
}

/// Destructor
LoadWidget::~LoadWidget()
{

}

/// adjust load-filters to current datatype
void LoadWidget::slotSetLoadFilters(){

  QStringList allFilters;

  //TODO All files filter
  
  for (int i=0; i < (int)supportedTypes_.size(); i++){
    QStringList filters = supportedTypes_[i].loadFilters.split(";;");
    for (int f=filters.size()-1; f >= 0;  f--){
      if (filters[f].trimmed() == "") { filters.removeAt(f); continue; }
      if (filters[f].contains( tr("All files") ) ) filters.removeAt(f);
    }

    allFilters.append( filters );
  }

  allFilters.removeDuplicates();
  allFilters.sort();

  QStringList allExt;
  
  //get all possible extensions
  for (int i=0; i < allFilters.size(); i++){
     QString ext = allFilters[i].section("(",1).section(")",0,0);
     allExt.append( ext.split(" ") );
  }

  for (int f=allExt.size()-1; f >= 0;  f--)
    if (allExt[f].trimmed() == "") allExt.removeAt(f);
  
  allExt.removeDuplicates();
  allExt.sort();
  
  QString allFiles = tr("All Files (");
  
  for (int i=0; i < allExt.size(); i++)
    allFiles += " " + allExt[i];
  
  allFiles += " )";

  allFilters.push_front(allFiles);
  
  setNameFilters(allFilters);
}

/// adjust save-filters to current datatype
void LoadWidget::slotSetSaveFilters(DataType _type){

  QStringList allFilters;

  bool multipleFiles = (ids_.size() > 1);
  
  for (int i=0; i < (int)supportedTypes_.size(); i++)
     if ( supportedTypes_[i].type.contains(_type) ){
       
       if ( multipleFiles && !supportedTypes_[i].saveMultipleObjects)
         continue;

      QStringList filters = supportedTypes_[i].saveFilters.split(";;");
      for (int f=filters.size()-1; f >= 0;  f--){
        if (filters[f].trimmed() == "") { filters.removeAt(f); continue; }
        if (filters[f].contains( tr("All files") ) ) filters.removeAt(f);
      }
      
      allFilters.append( filters );
    }

  allFilters.removeDuplicates();
  allFilters.sort();

  QStringList allExt;
  
  //get all possible extensions
  for (int i=0; i < allFilters.size(); i++){
     QString ext = allFilters[i].section("(",1).section(")",0,0);
     allExt.append( ext.split(" ") );
  }

  for (int f=allExt.size()-1; f >= 0;  f--)
    if (allExt[f].trimmed() == "") allExt.removeAt(f);
  
  allExt.removeDuplicates();
  allExt.sort();
  
  QString allFiles = tr("All Files (");
  
  for (int i=0; i < allExt.size(); i++)
    allFiles += " " + allExt[i];
  
  allFiles += " )";

  allFilters.push_front(allFiles);
  
  setNameFilters(allFilters);
}


/// find suitable plugin for loading current file
void LoadWidget::loadFile(){
  
  //get selection
  QStringList files = selectedFiles();

  //get all extensions
  QStringList ext;

  for (int i=0; i < files.size(); i++)
    ext.push_back( QFileInfo(files[i]).suffix() );
  
   
  //find plugins that can handle the current extensions
  pluginForExtension_.clear();
  
  for (int i=0; i < ext.size(); i++){

    for (uint t=0; t < supportedTypes_.size(); t++){
      
      QString filters = supportedTypes_[t].loadFilters;  

      // Take only part inside brackets
      filters = filters.section("(",1).section(")",0,0);

      QStringList separateFilters = filters.split(" ");

      bool found = false;

      for ( int filterId = 0 ; filterId < separateFilters.size(); ++filterId ) {

        if (separateFilters[filterId].endsWith(ext[i],Qt::CaseInsensitive)){
          pluginForExtension_[ ext[i] ] = t;
          found = true;
        }

      }

      if ( found )
        break;

    }
  }

  // display options for all dataTypes
  if ( !optionsBox_->isChecked() ){

    FileOptionsDialog options(supportedTypes_,ext, loadMode_);

    connect(&options, SIGNAL(setPluginForExtension(QString,int)), this, SLOT(slotSetPluginForExtension(QString,int)) );

    if ( !options.exec() )
      return;
  }


  //load the selected files
  QStringList loadableFiles;
  IdList pluginIds;
  for (int i=0; i < files.size(); i++){
    
    QFileInfo fi(files[i]);
    QString filename = fi.absoluteFilePath();
    OpenFlipperSettings().setValue("Core/CurrentDir", fi.absolutePath());
    QFile file(filename);

    if (fi.isDir() || !file.exists()) continue; //do nothing if its a not a valid file
    QString ext = fi.suffix();

    // if the default options should be used find the default plugin
    if (optionsBox_->isChecked()) {
      QString pluginName = OpenFlipperSettings().value(QString("Core/File/DefaultLoader/").append(ext)).toString();

      // find the id of the plugin
      bool found = false;
      unsigned int j;
      for (j = 0; j < supportedTypes_.size(); ++j) {
        if (supportedTypes_[j].name == pluginName) {
          found = true;
          break;
        }
      }

      if (found)
        pluginForExtension_[ ext ] = j;
    }

    //emit load signal
    if ( pluginForExtension_.find( ext ) != pluginForExtension_.end() ){
      emit load(filename, pluginForExtension_[ ext ]);
      loadableFiles.push_back(filename);
      pluginIds.push_back(pluginForExtension_[ext]);
    }
  }
  emit loadFiles(loadableFiles, pluginIds);
}

/// find suitable plugin for saving current file
void LoadWidget::saveFile(){
  //get selection
  QStringList files = selectedFiles();

  if ( files.size() != 1 ) {
    std::cerr << "Error: zero or multiple save files selected!" << std::endl;
    return;
  }

  QString filename = files[0];

  if (QFileInfo(filename).completeSuffix().isEmpty()) {

    int s = selectedNameFilter().indexOf("*")+1;
    int e = selectedNameFilter().indexOf(" ", s);
    int e2 = selectedNameFilter().indexOf(")", s);
    if (e == -1 || e2 < e) e = e2;

    QString ext = selectedNameFilter().mid(s,e-s);
    filename += ext;
  }

  QFile      f(filename);
  QFileInfo fi(filename);
   
  //find plugin that can handle the current extension
  pluginForExtension_.clear();
  
  for (uint t=0; t < supportedTypes_.size(); t++){

    QString filters = supportedTypes_[t].saveFilters;  

    // Take only part inside brackets
    filters = filters.section("(",1).section(")",0,0);

    QStringList separateFilters = filters.split(" ");

    bool found = false;

    for ( int filterId = 0 ; filterId < separateFilters.size(); ++filterId ) {

      if (separateFilters[filterId].endsWith(fi.suffix(),Qt::CaseInsensitive)){
        pluginForExtension_[ fi.suffix() ] = t;
        found = true;
      }

    }

    if ( found )
      break;

  }
  
  // display options for all dataTypes
  if ( !optionsBox_->isChecked() ){

    FileOptionsDialog options(supportedTypes_,QStringList(fi.suffix()), loadMode_);

    connect(&options, SIGNAL(setPluginForExtension(QString,int)), this, SLOT(slotSetPluginForExtension(QString,int)) );

    if ( !options.exec() )
      return;
  }

  if (f.exists()){ //check for extension
    int ret = QMessageBox::warning(this, tr("File exists"),tr("This file already exists.\n"
        "Do you want to overwrite the file?"),QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
    if (ret == QMessageBox::No)
      return; //abort if users doesn't want to overwrite
  }

  const std::map< QString, int >::iterator saving_plugin =
          pluginForExtension_.find( fi.suffix() );
  if ( saving_plugin != pluginForExtension_.end() ){
    if (ids_.size() == 1)
      emit save(ids_[0],filename, saving_plugin->second);
    else
      emit save(ids_   ,filename, saving_plugin->second);
  }

  OpenFlipperSettings().setValue("Core/CurrentDir", fi.absolutePath() );
}

/// show Widget for loading Files
int LoadWidget::showLoad(){
  setAcceptMode ( QFileDialog::AcceptOpen );
  setWindowTitle(tr("Load Object"));
  loadMode_ = true;

  setFileMode(QFileDialog::ExistingFiles);

  // Set directory to last Directory used for Opening Files
  setDirectory(OpenFlipperSettings().value("Core/CurrentDir").toString() );

  slotSetLoadFilters();

  return this->exec();
}

/// show Widget for saving Files
int LoadWidget::showSave(int _id, QString _filename){
  setAcceptMode ( QFileDialog::AcceptSave );
  setFileMode( QFileDialog::AnyFile );
  setWindowTitle(tr("Save Object"));
  loadMode_ = false;

  ids_.clear();
  ids_.push_back(_id);

  //set dataType
  BaseObjectData* object;
  PluginFunctions::getObject(_id,object);

  //check if we can save this dataType
  bool typeFound = false;
  
  for (int i=0; i < (int)supportedTypes_.size(); i++)
    if ( object->dataType( supportedTypes_[i].type ) )
      typeFound = true;


  if (!typeFound){
    std::cerr << "No suitable plugin for saving this dataType." << std::endl;
    return QDialog::Rejected;
  }

  slotSetSaveFilters( object->dataType() );

  //display correct path/name
  QFileInfo fi(_filename);
  QFile file(_filename);
  
  setDirectory(OpenFlipperSettings().value("Core/CurrentDir").toString() );
  selectFile ( fi.fileName() );

  //try to select the best fitting name filter
  for (int i=0; i < nameFilters().count(); i++){
    int s = nameFilters()[i].indexOf("*")+2;
    int e = nameFilters()[i].indexOf(" ", s);
    int e2 = nameFilters()[i].indexOf(")", s);
    if (e == -1 || e2 < e) e = e2;

    QString ext = nameFilters()[i].mid(s,e-s);

    if (ext == fi.completeSuffix()){
      selectNameFilter(nameFilters()[i]);
      break;
    }
  }

  return this->exec();
}

/// show Widget for saving Files
int LoadWidget::showSave(IdList _ids, QString _filename){
  setAcceptMode ( QFileDialog::AcceptSave );
  setFileMode( QFileDialog::AnyFile );
  setWindowTitle(tr("Save Objects"));
  loadMode_ = false;

  ids_ = _ids;

  DataType types = 0;
  
  for (uint i=0; i < _ids.size(); i++){
    
    BaseObjectData* object;
    PluginFunctions::getObject(_ids[i], object);
    
    types |= object->dataType();
  }
  
  //check if we can save this dataType
  bool typeFound = false;
  
  for (int i=0; i < (int)supportedTypes_.size(); i++)
    if ( supportedTypes_[i].type.contains(types) )
      typeFound = true;


  if (!typeFound){
    std::cerr << "No suitable plugin for saving this dataType." << std::endl;
    return QDialog::Rejected;
  }

  slotSetSaveFilters( types );

  //display correct path/name
  QFileInfo fi(_filename);
  QFile file(_filename);

  setDirectory(OpenFlipperSettings().value("Core/CurrentDir").toString() );
  selectFile ( fi.fileName() );

  //try to select the best fitting name filter
  for (int i=0; i < nameFilters().count(); i++){
    int s = nameFilters()[i].indexOf("*")+2;
    int e = nameFilters()[i].indexOf(" ", s);
    int e2 = nameFilters()[i].indexOf(")", s);
    if (e == -1 || e2 < e) e = e2;

    QString ext = nameFilters()[i].mid(s,e-s);

    if (ext == fi.completeSuffix()){
      selectNameFilter(nameFilters()[i]);
      break;
    }
  }

  return this->exec();
}

bool LoadWidget::validFilename() {
    
  // Only proceed if selected file is REALLY a file
  // Consider two cases:
  // Case 1: Filename is neither a valid file nor a directory -> continue and wait for valid file
  // Case 2: Entered filename is a directory -> Change to directory and wait for valid file
  QString firstEntered = selectedFiles()[0];
  
  // Test if directory exists
  QDir testdir(firstEntered);
  if(testdir.exists()) {
      setDirectory(testdir);
      return false;
  }
  
  // Test if file exists
  QFile file(firstEntered);
  if(!file.exists()) return false;
  
  return true;
}

bool LoadWidget::dirExists()
{
  QFileInfo firstEntered = selectedFiles()[0];
  // Test if directory exists
  return QDir(firstEntered.dir()).exists();
}

void LoadWidget::accept() {
    
  if( (!loadMode_) || validFilename() ) {
    
    if ( loadMode_ )
      loadFile();
    else
    {
      if (!dirExists()) //if dir does not exist, don't accept
        return;
      saveFile();
    }

    QFileDialog::accept();
  }
}

void LoadWidget::slotSetPluginForExtension(QString _extension, int _pluginId ){
  pluginForExtension_[ _extension ] = _pluginId;
}


