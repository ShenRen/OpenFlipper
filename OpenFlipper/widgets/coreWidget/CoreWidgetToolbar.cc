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
//  CLASS MViewWidget - IMPLEMENTATION
//
//=============================================================================


//== INCLUDES =================================================================

// -------------------- mview
#include "CoreWidget.hh"
// -------------------- ACG

// -------------------- Qt

//== IMPLEMENTATION ==========================================================

void CoreWidget::slotAddToolbar(QToolBar* _toolbar) {
  
  int id = -1;
  
  // Find the plugin which added this Toolbox
  for ( unsigned int i = 0 ; i < plugins_.size(); ++i ) {
    if ( plugins_[i].plugin == sender() ) {
      id = i;
      break;
    }
  }
  
  // Find the scripting plugin because we assign this toolBox to it as we did not find the original sender
  if ( id == -1 ) {
    for ( unsigned int i = 0 ; i < plugins_.size(); ++i ) {
      if ( plugins_[i].name == "Scripting" ) {
        id = i;
        break;
      }
    }
    
  }

  // Check if a toolbar with the same name is already registered
  for ( unsigned int i = 0 ; i < toolbars_.size(); ++i ) {
    if ( toolbars_[i]->windowTitle() == _toolbar->windowTitle() ) {
      emit log(LOGERR,tr("slotAddToolbar: Toolbar already added to system: ") + _toolbar->windowTitle() );
      return;
    }
  }

  // Correctly set the object ame
  _toolbar->setObjectName( _toolbar->windowTitle() );

  // Store in internal vector
  toolbars_.push_back( _toolbar );

  // Add to main ui
  addToolBar( _toolbar );
  
  // Remember which plugin this toolbar belongs to
  if ( id != -1 )
    plugins_[id].toolbars.push_back( std::pair< QString,QToolBar* >( _toolbar->windowTitle() , _toolbar) );
  
  // add widget name to viewMode 'all'
  if ( !viewModes_[0]->visibleToolbars.contains( _toolbar->windowTitle() ) ){
        viewModes_[0]->visibleToolbars << _toolbar->windowTitle();
        viewModes_[0]->visibleToolbars.sort();
  }      
  
}

void CoreWidget::getToolBar( QString _name, QToolBar*& _toolbar) {

  for ( unsigned int i = 0 ; i < toolbars_.size(); ++i ) {

    if ( toolbars_[i]->windowTitle() == _name ) {
      _toolbar = toolbars_[i];
      return;
    }

  }

  _toolbar = 0;

  emit log(LOGERR,tr("getToolBar: Toolbar \"%1\" not found.").arg(_name) );
}

void CoreWidget::slotRemoveToolbar(QToolBar* _toolbar) {
  for ( unsigned int i = 0 ; i < toolbars_.size(); ++i ) {

    if ( toolbars_[i]->windowTitle() == _toolbar->windowTitle() ) {
      std::cerr << "Todo : erase Toolbar from list" << std::endl;
      removeToolBar( _toolbar );
      return;
    }

  }

  emit log(LOGERR,tr("Remove Toolbar: Toolbar not found.") );
}

QSize CoreWidget::defaultIconSize(){
  return defaultIconSize_;
}

//=============================================================================
