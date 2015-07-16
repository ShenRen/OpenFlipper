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




#include "viewModeChangeWidget.hh"
#include <OpenFlipper/widgets/coreWidget/CoreWidget.hh>
#include <OpenFlipper/common/GlobalOptions.hh>

#include <QInputDialog>
#include <QMessageBox>

/// Constructor
viewModeChangeWidget::viewModeChangeWidget(const QVector< ViewMode* >& _modes, QWidget *_parent)
  : QDialog(_parent),
    modes_(_modes)
{
  setupUi(this);


  connect(viewModeList, SIGNAL(clicked (QModelIndex)), this, SLOT(slotModeClicked(QModelIndex)) );
  

}

// =======================================================================================================
// External communication
// =======================================================================================================

/// Slot for changing View and closing widget
void viewModeChangeWidget::slotModeClicked(QModelIndex /*_idx*/ ){
  
  // Search for current mode 
  int id = -1;
  if ( viewModeList->selectedItems().count() > 0) 
    for (int i=0; i < modes_.size(); i++)
      if (modes_[i]->name == viewModeList->currentItem()->text()){
        id = i;
        break;
      }
      
  if ( id == -1 ) {
    std::cerr << "Currently selected Mode not found?!" << std::endl; 
    return;
  }

  
  emit changeView(modes_[id]->name,modes_[id]->visibleToolboxes,modes_[id]->visibleToolbars,modes_[id]->visibleContextMenus);
  close();
}


/// overloaded show function
void viewModeChangeWidget::show(QString _lastMode){
  QDialog::show();
  
  //fill viewModeList
  viewModeList->clear();
  for (int i=0; i < modes_.size(); i++){
    QListWidgetItem *item = new QListWidgetItem(viewModeList);
    item->setTextAlignment(Qt::AlignHCenter);
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    
    QFile iconFile( OpenFlipper::Options::iconDirStr() + QDir::separator () + modes_[i]->icon  );
    
    if ( iconFile.exists() )
      item->setIcon( QIcon(iconFile.fileName()) );
    else {
      iconFile.setFileName( OpenFlipper::Options::configDirStr() + QDir::separator() + "Icons" + QDir::separator() + modes_[i]->icon );
      if ( iconFile.exists() )
        item->setIcon( QIcon(iconFile.fileName()) );
      else {
        item->setIcon( QIcon(OpenFlipper::Options::iconDirStr() + QDir::separator () + "Unknown.png")  );
        std::cerr << "Unable to find icon file! " << iconFile.fileName().toStdString() <<  std::endl;
      }
    }
    
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
    
}

