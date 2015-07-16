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

#include <OpenFlipper/BasePlugin/BaseInterface.hh>

#include "optionsWidget.hh"

#include <OpenFlipper/common/GlobalOptions.hh>

#include "ShortcutButton.hh"

#include <QMessageBox>

/// get the pluginInfo object corresponding to the given pluginName
int OptionsWidget::getPluginInfo(QString pluginName){
  for (uint i=0; i < plugins_.size(); i++)
    if (plugins_[i].name == pluginName)
      return i;

  return -1;
}

/// doubleclick in the keyTree
void OptionsWidget::keyTreeDoubleClicked(QTreeWidgetItem* _item, int /*col*/){

std::cerr << "set focus\n";

  if (_item == 0 || _item->parent() == 0 || _item->columnCount() < 7)
    return;

  shortcutButton->setFocus(Qt::TabFocusReason);
}

/// user selects a different item in the tree
void OptionsWidget::keyTreeItemChanged( QTreeWidgetItem* current, QTreeWidgetItem* /*previous*/){

  if (current == 0 || current->parent() == 0 || current->columnCount() < 7){
    shortcutBox->setEnabled(false);
    shortcutButton->setText("");
    defaultShortcut->setText("");
  }else{
    shortcutBox->setEnabled(true);

    shortcutButton->setCurrentShortcut( current->text(4).toInt(), (Qt::KeyboardModifiers) current->text(5).toInt() );
    defaultShortcut->setText( current->text(2) );
  }

}

///check if the shortcut exists and add it if not
void OptionsWidget::updateShortcut(){

  if ( keyTree->currentItem() == 0)
    return;

  bool myMultiUse = (bool) keyTree->currentItem()->text(6).toInt();
  QString myNewKey = shortcutButton->text();

   //check if the shortcut already exists
  for (int i=0; i < keyTree->topLevelItemCount(); i++)
    for (int j=0; j < keyTree->topLevelItem(i)->childCount(); j++){

      QTreeWidgetItem* item = keyTree->topLevelItem(i)->child(j);
      QString key = item->text(1);
      bool multiUse = (bool) item->text(6).toInt();

      if (keyTree->currentItem() == item)
        continue;

      if (key == myNewKey && ( !multiUse || !myMultiUse ) ){
        QMessageBox::warning(this, tr("OpenFlipper"), tr("Could not add Shortcut. Shortcut already assigned."), QMessageBox::Ok);
        return;
      }
    }

  //update the item
  if ( keyTree->currentItem() != 0){
    keyTree->currentItem()->setText( 1, shortcutButton->text() );
    keyTree->currentItem()->setText( 4, QString::number( shortcutButton->key() ) );
    keyTree->currentItem()->setText( 5, QString::number( shortcutButton->modifiers() ) );
  }

  keyTree->setFocus(Qt::TabFocusReason);
  keyTreeItemChanged(keyTree->currentItem(), 0);
}

///check which of the shortcuts changed and inform the core about the change
void OptionsWidget::applyShortcuts(){

  //check all shortcuts
  for (int i=0; i < keyTree->topLevelItemCount(); i++)
    for (int j=0; j < keyTree->topLevelItem(i)->childCount(); j++){

      //check wether the shortcut changed
      QTreeWidgetItem* item = keyTree->topLevelItem(i)->child(j);

      if ( item->text(1) != item->text(7) ){

        QString pluginName = item->parent()->text(0);

        QObject* plugin = 0;

        if (pluginName != "Core"){

          //get the plugin object
          int index = getPluginInfo(pluginName);

          if (index == -1) //if pluginInfo was not found ->skip
            continue;

          plugin = plugins_[index].plugin;
        }

        int bindingID = item->text(3).toInt();
        int key = item->text(4).toInt();

        Qt::KeyboardModifiers modi = (Qt::KeyboardModifiers) item->text(5).toInt();

        emit addKeyMapping(key, modi, plugin, bindingID);
      }
    }
}

void OptionsWidget::restoreKeyPresets(){

   //check if the shortcut already exists
  for (int i=0; i < keyTree->topLevelItemCount(); i++)
    for (int j=0; j < keyTree->topLevelItem(i)->childCount(); j++){

      QTreeWidgetItem* item = keyTree->topLevelItem(i)->child(j);
      QString key = item->text(1);

      QString pluginName = item->parent()->text(0);
      int bindingID = item->text(3).toInt();

      if (pluginName == "Core"){

        item->setText( 1, item->text(2) );
        item->setText( 4, QString::number(coreKeys_[ bindingID ].key) );
        item->setText( 5, QString::number(coreKeys_[ bindingID ].modifiers) );

      } else {

        //get the plugin object
        int index = getPluginInfo(pluginName);

        if (index == -1) //if pluginInfo was not found ->skip
          continue;

        item->setText( 1, item->text(2) );
        item->setText( 4, QString::number(plugins_[index].keys[ bindingID ].key) );
        item->setText( 5, QString::number(plugins_[index].keys[ bindingID ].modifiers) );
      }
    }

  keyTree->setFocus(Qt::TabFocusReason);
  keyTreeItemChanged(keyTree->currentItem(), 0);
}

/// init the TreeWidget containing the keyBindings
void OptionsWidget::initKeyTree(){

  keyTree->clear();

  keyTree->setEditTriggers(QAbstractItemView::NoEditTriggers);

  keyTree->setColumnCount ( 8 );

  QStringList headerdata;
  headerdata << "Action" << "Shortcut" << "Default" << "keyIndex" << "currentKey" << "currentModi" << "MultiUse" << "initShortcut";
  keyTree->setHeaderLabels(headerdata);

  //fill the keyTree
  std::map< QString, QTreeWidgetItem* > parentMap;

  InverseKeyMap::iterator it;
  for (it=keys_.begin(); it != keys_.end(); ++it){

    QObject* plugin = (*it).first.first;
    int bindingID = (*it).first.second;
    int key = (*it).second.first;
    Qt::KeyboardModifiers modifiers = (*it).second.second;

    //get plugin name
    QString name;

    if (plugin == 0) //parent is the core
      name = "Core";
    else{

      BaseInterface* basePlugin = qobject_cast< BaseInterface * >(plugin);

      if (basePlugin)
        name = basePlugin->name();
      else{
        //name not found so skip it
        continue;
      }
    }

    //get corresponding the pluginInfo object
    int i = -1;

    if (name != "Core"){
      i = getPluginInfo(name);

      if (i == -1) //if pluginInfo was not found ->skip
        continue;
    }

    //is the toplevel item already there?
    if ( parentMap.find(name) == parentMap.end() ){
      parentMap[name] = new QTreeWidgetItem(keyTree, QStringList(name));
      keyTree->addTopLevelItem( parentMap[name] );
    }

    QTreeWidgetItem* parent = parentMap[name];

    //get the default settings
    QString               description;
    bool                  multiUse;
    int                   defKey;
    Qt::KeyboardModifiers defModi;

    if (name == "Core"){

      description = coreKeys_[bindingID].description;
      multiUse = coreKeys_[bindingID].multiUse;
      defKey = coreKeys_[bindingID].key;
      defModi = coreKeys_[bindingID].modifiers;

    } else {

      description = plugins_[i].keys[bindingID].description;
      multiUse = plugins_[i].keys[bindingID].multiUse;
      defKey = plugins_[i].keys[bindingID].key;
      defModi = plugins_[i].keys[bindingID].modifiers;
    }

    QStringList rows;

    //setup the strings for the shortcuts
    QString keyString;

    if (key == -1 && modifiers == 0){
      keyString = "unassigned";
    }else if (key == Qt::Key_AltGr || key == Qt::Key_Alt || key == Qt::Key_Control || key == Qt::Key_Shift || key == Qt::Key_Meta){
      keyString = QKeySequence( modifiers ).toString();
      keyString = keyString.left(keyString.size()-1);
    }else
      keyString = QKeySequence( key + modifiers ).toString();

    QString defaultStr;

    if (defKey == -1 && defModi == 0){
      defaultStr = "unassigned";
    }else if (defKey == Qt::Key_AltGr || defKey == Qt::Key_Alt || defKey == Qt::Key_Control
           || defKey == Qt::Key_Shift || defKey == Qt::Key_Meta){
      defaultStr = QKeySequence( defModi ).toString();
      defaultStr = defaultStr.left(defaultStr.size()-1);
    }else
      defaultStr = QKeySequence( defKey + defModi ).toString();

    //and add the row
    rows << description << keyString << defaultStr << QString::number(bindingID) << QString::number(key)
         << QString::number(modifiers) << QString::number(multiUse) << keyString;

    QTreeWidgetItem* keyItem = new QTreeWidgetItem(parent, rows);

    parent->addChild( keyItem );
  }


  keyTree->setColumnWidth(0,350);
  keyTree->setColumnHidden(3, true);
  keyTree->setColumnHidden(4, true);
  keyTree->setColumnHidden(5, true);
  keyTree->setColumnHidden(6, true);
  keyTree->setColumnHidden(7, true);
}
