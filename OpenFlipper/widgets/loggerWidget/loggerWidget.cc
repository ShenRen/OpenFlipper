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




#include "loggerWidget.hh"

#include <OpenFlipper/common/GlobalOptions.hh>
#include <iostream>
#include <cmath>

#include <OpenMesh/Core/System/omstream.hh>

LoggerWidget::LoggerWidget( QWidget *parent)
  : QWidget(parent),
  newData_(true)
{
  // Don't delete this widget on close actions
  // since it may be embedded in different widget
  // containers at the same time
  setAttribute(Qt::WA_DeleteOnClose, false);

  QVBoxLayout* vlayout  = new QVBoxLayout();
  QHBoxLayout* hlayout  = new QHBoxLayout();
  
  list_ = new QListWidget();
  
  list_->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  list_->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  list_->setFocusPolicy(Qt::NoFocus);
  list_->setSelectionMode(QAbstractItemView::ExtendedSelection);
  list_->setUniformItemSizes(true);

  QString path = OpenFlipper::Options::iconDirStr()+OpenFlipper::Options::dirSeparator();

  // ============================
  // Context Menu
  // ============================
  context_ = new QMenu(tr("Log Viewer"));
  
  QAction* copyAction  = new QAction(QIcon(path + "edit-copy.png"), tr("Copy"),0);
  copyAction->setShortcut( QKeySequence("Ctrl+C") );
  QAction* selectAction = new QAction(tr("Select All"),0);
  selectAction->setShortcut( QKeySequence("Ctrl+A") );
  
  connect (copyAction, SIGNAL(triggered()), this, SLOT(copySelected()));
  connect (selectAction, SIGNAL(triggered()), list_, SLOT(selectAll()));
  
  context_->addAction(copyAction);
  context_->addSeparator();
  context_->addAction(selectAction);
  
  
  // ============================
  // Filters Menu
  // ============================
  filterMenu_ = new QMenu(tr("Log Viewer"));
  
  openMeshFilterAction_  = new QAction(QIcon(path + "edit-copy.png"), tr("Enable OpenMesh error messages"),0);
  openMeshFilterAction_->setCheckable(true);
  

  if ( OpenFlipperSettings().value("Core/Gui/LogWindow/OpenMeshErrors",true).toBool() ) {
    openMeshFilterAction_->setChecked( true );
    omerr().enable();
  } else {
    openMeshFilterAction_->setChecked( false );
    omerr().disable();
  }
  
  filterMenu_->addAction(openMeshFilterAction_);
  
  // ============================
  // Scrollbar
  // ============================
  blockNext_ = false;
  
  connect (&loggerUpdateTimer_, SIGNAL(timeout ()), this, SLOT(slotScrollUpdate()));
  
  // Single shot timer every 500 msecs
  loggerUpdateTimer_.setSingleShot(true);
  loggerUpdateTimer_.setInterval(500);
  
  allButton_   = new QPushButton(QIcon(path + "status_all.png"),tr("All Messages"));
  allButton_->setCheckable(true);
  allButton_->setAutoExclusive(true);
  infoButton_  = new QPushButton(QIcon(path + "status_green.png"),tr("Informations"));
  infoButton_->setCheckable(true);
  infoButton_->setAutoExclusive(true);
  warnButton_  = new QPushButton(QIcon(path + "status_yellow.png"),tr("Warnings"));
  warnButton_->setCheckable(true);
  warnButton_->setAutoExclusive(true);
  errorButton_ = new QPushButton(QIcon(path + "status_red.png"),tr("Errors"));
  errorButton_->setCheckable(true);
  errorButton_->setAutoExclusive(true);

  filterButton_ = new QPushButton(QIcon(path + "status_filter.png"),tr("Set Filters"));
  filterButton_->setCheckable(false);

  allButton_->setChecked(true);

  connect(allButton_,   SIGNAL(clicked()), this, SLOT(updateList()));
  connect(infoButton_,  SIGNAL(clicked()), this, SLOT(updateList()));
  connect(warnButton_,  SIGNAL(clicked()), this, SLOT(updateList()));
  connect(errorButton_, SIGNAL(clicked()), this, SLOT(updateList()));
  connect(filterButton_,SIGNAL(clicked()), this, SLOT(slotFilterMenu()));

  clearButton_ = new QPushButton(QIcon(path + "edit-clear.png"),tr("Clear Messages"));
  connect(clearButton_, SIGNAL(clicked()), list_, SLOT(clear()));
  
  hlayout->addWidget( allButton_ );
  hlayout->addWidget( infoButton_ );
  hlayout->addWidget( warnButton_ );
  hlayout->addWidget( errorButton_ );
  hlayout->addStretch();
  hlayout->addWidget( filterButton_ );
  hlayout->addStretch();
  hlayout->addWidget( clearButton_ );
  
  hlayout->setSpacing(0);
  hlayout->setContentsMargins (0,0,0,0);
  vlayout->setSpacing(0);
  vlayout->setContentsMargins (0,0,0,0);
  
  vlayout->addWidget(list_);
  vlayout->addLayout( hlayout );
  
  setLayout( vlayout );
}

LoggerWidget::~LoggerWidget()
{
  delete clearButton_;
  delete errorButton_;
  delete warnButton_;
  delete infoButton_;
  delete allButton_;
  delete context_;
  delete list_;
}


//-------------------------------------------------------------------------------------

/// Append a new logmessage to log viewer
void LoggerWidget::append(const QString& _text, Logtype _type){
 
  list_->addItem(_text);

  QListWidgetItem* item = list_->item( list_->count()-1 );
  
  if ( allButton_->isChecked() )
    item->setHidden(false);
  else
    item->setHidden(true);
  
  switch (_type) {
    case LOGINFO:
      item->setForeground( QBrush(QColor(Qt::darkGreen)) );
      item->setBackground( QBrush(QColor(225,255,225), Qt::Dense4Pattern) );
      
      if ( infoButton_->isChecked() )
        item->setHidden(false);
      break;
    case LOGOUT:
      item->setForeground( QBrush(QColor(Qt::black)) );
      break;
    case LOGWARN:
      item->setForeground( QBrush(QColor(160,160,0)) );
      item->setBackground( QBrush(QColor(255,240,200),Qt::Dense4Pattern) );
      
      if ( warnButton_->isChecked() )
        item->setHidden(false);
      break;
    case LOGERR:
      item->setForeground( QBrush(QColor(Qt::red)) );
      item->setBackground( QBrush(QColor(255,225,225),Qt::Dense4Pattern) );
      
      if ( errorButton_->isChecked() )
        item->setHidden(false);
      break;
    case LOGSTATUS:
      item->setForeground( QBrush(QColor(Qt::blue)) );
      item->setBackground( QBrush(QColor(255,225,225),Qt::Dense4Pattern) );

      if ( errorButton_->isChecked() )
        item->setHidden(false);
      break;
  }
  
  // If the logger is hidden, we just ignore the update ... done by showEvent later
  if ( isHidden() )
    return;

  // Remember that we have new logs to show
  newData_ = true;
  
  // Check if we already have a running timer.
  // If so, the timeout of that timer will trigger the redraw.
  // Otherwise, we redraw and start the timer to block concurrent redraws.
  // Only if new data is available, the redraw at the timers timeout will be done.
  if ( ! loggerUpdateTimer_.isActive() ) {
    // Update the logger
    list_->scrollToBottom();
    
    // Remember that there is no new data now.
    // This might change again on a call to this function, while the timer is active.
    newData_ = false;
    
    // start the timer
    loggerUpdateTimer_.start();
  }
  
}

//-------------------------------------------------------------------------------------

void LoggerWidget::slotScrollUpdate() {
  
  // If there is data to show, do it.
  if ( newData_ ) {
    list_->scrollToBottom();
    newData_ = false;
  }
  
}

//-------------------------------------------------------------------------------------

/// update the list if a button was pressed
void LoggerWidget::updateList(){
  
  QColor color;
  
  if ( infoButton_->isChecked() )
    color = QColor(Qt::darkGreen);
  else if ( warnButton_->isChecked() )
    color = QColor(160,160,0);
  else if ( errorButton_->isChecked() )
    color = QColor(Qt::red);
  else
    color = QColor(Qt::black);
  
  if (color == QColor(Qt::black)){

    for (int i=0; i < list_->count(); i++)
      list_->item( i )->setHidden(false);
    
  } else {
    
    for (int i=0; i < list_->count(); i++)
      if ( list_->item(i)->foreground().color() == color )
        list_->item( i )->setHidden(false);
      else
        list_->item( i )->setHidden(true);
  }
  
  list_->scrollToBottom();
}

//-------------------------------------------------------------------------------------
void LoggerWidget::showEvent ( QShowEvent * /*event*/ ) {
  list_->scrollToBottom();
}

//-------------------------------------------------------------------------------------

/// handle keyPressEvents
void LoggerWidget::keyPressEvent (QKeyEvent * _event ) {
  // Return key event to parent if not one of the standard key combinations ( ... Core )
  if ( (_event->modifiers() & Qt::ControlModifier ) && ( _event->key() == Qt::Key_C ) )
    copySelected();
 
  else if ( (_event->modifiers() & Qt::ControlModifier ) && ( _event->key() == Qt::Key_A ) )
    list_->selectAll();
  
  else
    _event->ignore();
}

//-------------------------------------------------------------------------------------

/// show context menu
void LoggerWidget::contextMenuEvent ( QContextMenuEvent * event ){
 
  QPoint p = list_->mapToGlobal( event->pos() );
  
  context_->popup( p );
  
}

//-------------------------------------------------------------------------------------

///copy Selected rows to clipboard
void LoggerWidget::copySelected (){
  
  QString str = "";
  
  for (int i=0; i < list_->selectedItems().count(); i++)
    str += (list_->selectedItems()[i])->text() + "\n";
  
 QClipboard *clipboard = QApplication::clipboard();

 clipboard->setText(str);
}

//-------------------------------------------------------------------------------------

void LoggerWidget::slotFilterMenu() {
  filterMenu_->popup( list_->mapToGlobal(filterButton_->pos()) );
}

