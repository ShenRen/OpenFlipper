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



//=============================================================================
//
//  CLASS CoreWidget - IMPLEMENTATION
//
//=============================================================================


//== INCLUDES =================================================================

// -------------------- mview
#include "CoreWidget.hh"

//== IMPLEMENTATION ==========================================================

StackWidgetInfo::StackWidgetInfo(bool _editable,QString _name,QWidget* _widget) :
    editable(_editable),
    name(_name),
    widget(_widget)
{
}

void CoreWidget::slotGetStackWidget( QString _name, QWidget*& _widget) {
  
  for ( uint i = 0 ; i < stackWidgetList_.size(); ++i) {
    if ( stackWidgetList_[i].name == _name ) {
      _widget = stackWidgetList_[i].widget;
      return; 
    }
  }
  
  _widget = 0;
}

void CoreWidget::slotAddStackWidget( QString _name, QWidget* _widget ) {
  QWidget* widget;
  
  slotGetStackWidget( _name, widget );
  
  if ( widget ) {
    emit log(LOGERR,tr("Name already existing"));
    return; 
  }
  
  stackedWidget_->addWidget(_widget);
  stackWidgetList_.push_back( StackWidgetInfo( true, _name , _widget ) );
  
  slotUpdateStackMenu();
}


void CoreWidget::slotUpdateStackWidget( QString _name, QWidget* _widget ) {
   
  QWidget* oldWidget = 0;      
  uint index = 0;
  for ( uint i = 0 ; i < stackWidgetList_.size(); ++i) {
    if ( stackWidgetList_[i].name == _name ) {
      oldWidget = stackWidgetList_[i].widget;
      index = i;
      break;
    }
  }
  
  if ( oldWidget == 0 ) {
    emit log(LOGERR,tr("Did not find widget to update stackwidget"));
    return; 
  }
  
  stackedWidget_->removeWidget( oldWidget );
  stackWidgetList_.erase( stackWidgetList_.begin() + index );
  
  stackedWidget_->addWidget(_widget);
  stackWidgetList_.push_back( StackWidgetInfo( true, _name , _widget ) );
  
  slotUpdateStackMenu();
}

void CoreWidget::slotViewMenuAction( QAction * _action) {
  
  // Get the object Name from the action
  QString objectName = _action->text();
  
  QWidget* widget = 0;      
  for ( uint i = 0 ; i < stackWidgetList_.size(); ++i) {
    if ( stackWidgetList_[i].name == objectName ) {
      widget = stackWidgetList_[i].widget;
      break;
    }
  }
  
  if ( widget == 0 ) {
    emit log(LOGERR,tr("Cannot set Widget"));
    return; 
  }
  
  stackedWidget_->setCurrentWidget ( widget ); 
}

void CoreWidget::slotUpdateStackMenu() {
  // Only create menu if there are more then two widgets
  if ( stackWidgetList_.size() > 1 ) {
    
    if ( stackMenu_ == 0 ) {
      stackMenu_ = new QMenu(tr("Views"));
      menuBar()->addMenu(stackMenu_ );
    }
  
    
    if ( ! stackMenu_->isEmpty() )
      stackMenu_->clear();
  
    // Create an action group for every view
    QActionGroup* actionGroup = new QActionGroup( stackMenu_ );
    actionGroup->setExclusive( true );
    
    for ( uint i = 0 ; i < stackWidgetList_.size() ; ++i ) {
      QAction* newAction = new QAction(stackWidgetList_[i].name, actionGroup);  
      newAction->setText(stackWidgetList_[i].name);
    }
    
    stackMenu_->addActions(actionGroup->actions());   
    
    // Connect this actiongoup to a slot
    connect( actionGroup, SIGNAL( triggered( QAction * ) ), this, SLOT( slotViewMenuAction( QAction * ) ) );
  
    stackMenu_->show();
  } else {
    // Only One widget left, so delete the switching menu
    if ( stackMenu_ != 0 ) {
      
      // remove menu from menuBar
      menuBar()->removeAction( stackMenu_->menuAction() );
      delete stackMenu_;
      
      stackMenu_ = 0;
    }
  }
}


//=============================================================================
