/*===========================================================================*\
 *                                                                           *
 *                              OpenFlipper                                  *
 *      Copyright (C) 2001-2009 by Computer Graphics Group, RWTH Aachen      *
 *                           www.openflipper.org                             *
 *                                                                           *
 *---------------------------------------------------------------------------*
 *  This file is part of OpenFlipper.                                        *
 *                                                                           *
 *  OpenFlipper is free software: you can redistribute it and/or modify      *
 *  it under the terms of the GNU Lesser General Public License as           *
 *  published by the Free Software Foundation, either version 3 of           *
 *  the License, or (at your option) any later version with the              *
 *  following exceptions:                                                    *
 *                                                                           *
 *  If other files instantiate templates or use macros                       *
 *  or inline functions from this file, or you compile this file and         *
 *  link it with other files to produce an executable, this file does        *
 *  not by itself cause the resulting executable to be covered by the        *
 *  GNU Lesser General Public License. This exception does not however       *
 *  invalidate any other reasons why the executable file might be            *
 *  covered by the GNU Lesser General Public License.                        *
 *                                                                           *
 *  OpenFlipper is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *  GNU Lesser General Public License for more details.                      *
 *                                                                           *
 *  You should have received a copy of the GNU LesserGeneral Public          *
 *  License along with OpenFlipper. If not,                                  *
 *  see <http://www.gnu.org/licenses/>.                                      *
 *                                                                           *
\*===========================================================================*/

/*===========================================================================*\
 *                                                                           *
 *   $Revision$                                                         *
 *   $Author$                                                      *
 *   $Date$                   *
 *                                                                           *
\*===========================================================================*/




#include "viewModeWidget.hh"
#include <OpenFlipper/widgets/coreWidget/CoreWidget.hh>
#include <OpenFlipper/common/GlobalOptions.hh>

#include <QInputDialog>
#include <QMessageBox>

/// Constructor
viewModeWidget::viewModeWidget(const QVector< ViewMode* >& _modes, QWidget *_parent)
  : QDialog(_parent),
    modes_(_modes)
{
  setupUi(this);



  connect(viewModeList ,SIGNAL(customContextMenuRequested ( const QPoint &  )  ),
          this,SLOT(slotModeContextMenu ( const QPoint & ) ));
  connect(toolboxList ,SIGNAL(customContextMenuRequested ( const QPoint &  )  ),
          this,SLOT(slotToolContextMenu ( const QPoint & ) ));

  viewModeList->setContextMenuPolicy(Qt::CustomContextMenu);
  toolboxList->setContextMenuPolicy(Qt::CustomContextMenu);

  connect(viewModeList, SIGNAL(itemSelectionChanged()), this, SLOT(slotSetToolWidgets()));

  connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
  connect(okButton, SIGNAL(clicked()), this, SLOT(slotChangeView()));
  connect(saveButton, SIGNAL(clicked()), this, SLOT(slotSaveMode()));
  connect(removeButton, SIGNAL(clicked()), this, SLOT(slotRemoveMode()));
  connect(viewModeList, SIGNAL(currentTextChanged (QString)), this, SLOT(slotModeChanged(QString)) );
  connect(viewModeList, SIGNAL(clicked (QModelIndex)), this, SLOT(slotModeClicked(QModelIndex)) );
  connect(upButton, SIGNAL(clicked()), this, SLOT(slotMoveUp()) );
  connect(downButton, SIGNAL(clicked()), this, SLOT(slotMoveDown()) );
  connect(removeWidgetButton, SIGNAL(clicked()), this, SLOT(slotRemoveWidget()) );

  // load icons for tool buttons
  upButton->setIcon( QIcon(OpenFlipper::Options::iconDirStr() + OpenFlipper::Options::dirSeparator() + "arrow-up.png" ) );
  removeWidgetButton->setIcon( QIcon(OpenFlipper::Options::iconDirStr() + OpenFlipper::Options::dirSeparator() + "edit-delete.png" ) );
  downButton->setIcon( QIcon(OpenFlipper::Options::iconDirStr() + OpenFlipper::Options::dirSeparator() + "arrow-down.png" ) );
}

/// Move Widget up in the list
void viewModeWidget::slotMoveUp(){
  if (toolboxList->selectedItems().count() == 1){
    if (toolboxList->currentRow() == 0) return;
    //extract a list of all items
    QString item = toolboxList->currentItem()->text();
    int oldRow = toolboxList->currentRow();
    QStringList widgets;
    for (int i=0; i < toolboxList->count(); i++)
      widgets << toolboxList->item(i)->text();

    //reorder items
    QString last = widgets[0];
    for (int i=1; i < widgets.size(); i++){
      if (widgets[i] == item){
        widgets[i] = last;
        widgets[i-1] = item;
      }
      last = widgets[i];
    }
    // set new list as elements for the toolboxList
    toolboxList->clear();
    toolboxList->addItems(widgets);
    //highlight active item
    toolboxList->setCurrentRow(oldRow-1);
  }
}

/// Move widget do in the list
void viewModeWidget::slotMoveDown(){
  if (toolboxList->selectedItems().count() == 1){
    if (toolboxList->currentRow() == toolboxList->count()-1) return;
    //extract a list of all items
    QString item = toolboxList->currentItem()->text();
    int oldRow = toolboxList->currentRow();
    QStringList widgets;
    for (int i=0; i < toolboxList->count(); i++)
      widgets << toolboxList->item(i)->text();

    //reorder items
    QString last = widgets[widgets.size()-1];
    for (int i=widgets.size()-2; i >= 0; i--){
      if (widgets[i] == item){
        widgets[i] = last;
        widgets[i+1] = item;
      }
      last = widgets[i];
    }
    // set new list as elements for the toolboxList
    toolboxList->clear();
    toolboxList->addItems(widgets);
    //highlight active item
    toolboxList->setCurrentRow(oldRow+1);
  }
}

/// remove Widget form the list
void viewModeWidget::slotRemoveWidget(){
  qDeleteAll(toolboxList->selectedItems());
}

/// Slot for updating removeButton when new mode is selected
void viewModeWidget::slotModeClicked(QModelIndex /*_id*/){
  slotModeChanged(QString());
}

/// Slot for updating removeButton when new mode is selected
void viewModeWidget::slotModeChanged(QString /*_mode*/){
  //check if mode is custom e.g. that it can be removed
  if (viewModeList->selectedItems().count() > 0){
    for (int i=0; i < modes_.size(); i++)
      if (modes_[i]->name == viewModeList->currentItem()->text()){
        removeButton->setEnabled(modes_[i]->custom);
        break;
      }
  }
}

/// opens custom context menu in Mode-List
void viewModeWidget::slotModeContextMenu ( const QPoint & _pos ){

  if (viewModeList->itemAt(_pos)){

    QMenu menu(0);

    menu.addAction(tr("Remove Mode"), this, SLOT ( slotRemoveMode() ));

    //check if mode is custom e.g. that it can be removed
    for (int i=0; i < modes_.size(); i++)
      if (modes_[i]->name == viewModeList->currentItem()->text()){
          menu.actions()[0]->setEnabled(modes_[i]->custom);
        break;
      }

    menu.exec(viewModeList->mapToGlobal( _pos) );
  }
}

/// opens custom context menu in tool-List
void viewModeWidget::slotToolContextMenu ( const QPoint & _pos ){

  if (toolboxList->itemAt(_pos)){

    QMenu menu(0);

    if (toolboxList->selectedItems().count() == 1){
      menu.addAction(tr("Move up"), this, SLOT ( slotMoveUp() ));
      menu.addAction(tr("Move down"), this, SLOT ( slotMoveDown() ));
      menu.addSeparator();
    }

    menu.addAction(tr("Remove Widget"), this, SLOT ( slotRemoveWidget() ));

    menu.exec(toolboxList->mapToGlobal( _pos) );
  }
}

/// Slot for removing custom modes
void viewModeWidget::slotRemoveMode(){
  emit removeMode( viewModeList->currentItem()->text() );
  QListWidgetItem* item =  viewModeList->takeItem( viewModeList->currentRow() );
  delete item;
}

/// Slot for updating ToolWidget-List depending on the selected Mode
void viewModeWidget::slotSetToolWidgets(){
  toolboxList->clear();
  //iterate over all selected modes
  for (int m=0; m < viewModeList->selectedItems().size(); m++)
    // find mode in modeVector modes_
    for (int i=0; i < modes_.size(); i++)
      if ( modes_[i]->name == (viewModeList->selectedItems()[m])->text() )
        toolboxList->addItems(modes_[i]->visibleToolboxes); //add corresponding widgets
}

/// Slot for changing View and closing widget
void viewModeWidget::slotChangeView(){
  //get widgets
  QStringList widgets;
  for (int i=0; i < toolboxList->count(); i++)
    widgets << toolboxList->item(i)->text();
  //get mode
  QString mode = "";
  if (viewModeList->selectedItems().size() > 0)
    mode = viewModeList->selectedItems()[0]->text();
  emit changeView(mode,widgets);
  close();
}

/// Slot for saving current List of Widgets as custom mode
void viewModeWidget::slotSaveMode(){
  if (toolboxList->count() == 0) return;
  //ask for a name for the new viewmode
  bool ok;
  QString name = QInputDialog::getText(this, tr("Change View Mode"),
                                       tr("Enter a name for the new ViewMode:"), QLineEdit::Normal,
                                          "", &ok);
  //Remove Spaces from name
  name.remove(" ");

  if (!ok || name.isEmpty()) return;

  //check if name already exists
  for (int i=0; i < modes_.size(); i++)
    if (modes_[i]->name == name){
      QMessageBox::warning(this, tr("Change View Mode"), tr("Cannot Save ViewMode. Name already taken by a different mode."),
                                        QMessageBox::Ok);
      return;
    }

  QStringList widgets;
  for (int i=0; i < toolboxList->count(); i++)
    widgets << toolboxList->item(i)->text();
  emit saveMode(name, true, widgets);
  show(name);
}

/// overloaded show function
void viewModeWidget::show(QString _lastMode){
  QDialog::show();

  //fill viewModeList
  viewModeList->clear();
  for (int i=0; i < modes_.size(); i++){
    QListWidgetItem *item = new QListWidgetItem(viewModeList);
    item->setTextAlignment(Qt::AlignHCenter);
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    item->setIcon(QIcon(OpenFlipper::Options::iconDirStr() + QDir::separator () + "Unknown.png"));
    item->setText(modes_[i]->name);

    if (modes_[i]->custom)
      viewModeList->item(i)->setForeground( QBrush(QColor(0,0,150) ) );
    else
      viewModeList->item(i)->setForeground( QBrush(QColor(0,0,0) ) );
  }

  //select given mode
  viewModeList->setCurrentRow(0);

  for (int i=0; i < viewModeList->count(); i++)
    if (viewModeList->item(i)->text() == _lastMode)
      viewModeList->setCurrentRow(i);

  removeButton->setEnabled(false);
}

