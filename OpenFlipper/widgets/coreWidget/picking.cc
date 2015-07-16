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

#include "CoreWidget.hh"

#include <OpenFlipper/BasePlugin/PluginFunctions.hh>
#include <OpenFlipper/common/GlobalOptions.hh>
#include <OpenFlipper/widgets/glWidget/CursorPainter.hh>
#include <ACG/Scenegraph/SceneGraphAnalysis.hh>

//-----------------------------------------------------------------------------

void CoreWidget::setActionMode(const Viewer::ActionMode _am){

  // update Buttons
  moveButton_->setChecked(false);
  pickButton_->setChecked(false);
  questionButton_->setChecked(false);

  switch (_am)
  {
  case Viewer::ExamineMode:
    moveButton_->setChecked(true);
    break;
  case Viewer::LightMode:
    break;
  case Viewer::PickingMode:
    pickButton_->setChecked(true);
    break;
  case Viewer::QuestionMode:
    questionButton_->setChecked(true);
    break;
  }

  if (_am != actionMode_) {
    lastActionMode_ = actionMode_;
    actionMode_ = _am;

    // update cursor
    switch ( _am )
    {
    case Viewer::ExamineMode:
      cursorPainter_->setCursor(QCursor( QPixmap( OpenFlipper::Options::iconDirStr() + QDir::separator() + "cursor_move.png"  ) ,0,0 ));
      break;
    case Viewer::LightMode:
      cursorPainter_->setCursor(QCursor( QPixmap( OpenFlipper::Options::iconDirStr() + QDir::separator() + "cursor_light.png"  ) ,0,0 ));
      break;
    case Viewer::PickingMode:
      cursorPainter_->setCursor(QCursor( QPixmap( OpenFlipper::Options::iconDirStr() + QDir::separator() + "cursor_arrow.png"  ) ,0,0 ));
      if (pick_mode_idx_ != -1) {
        cursorPainter_->setCursor(pick_modes_[pick_mode_idx_].cursor() );
      }
      break;
    case Viewer::QuestionMode:
      cursorPainter_->setCursor(QCursor( QPixmap( OpenFlipper::Options::iconDirStr() + QDir::separator() + "cursor_whatsthis.png"  ) ,0,0 ));
      break;
    }

    // Update pickmode toolbar
    switch ( _am ) {
    case Viewer::PickingMode:
      // Show the pickMode Toolbar for this picking mode if it is set
      if (pick_mode_idx_ != -1) {
        if (pick_modes_[pick_mode_idx_].toolbar() )
          setActivePickToolBar(pick_modes_[pick_mode_idx_].toolbar());
        else
          hidePickToolBar();
      }
      break;

    default:
      hidePickToolBar();
      break;
    }

    unsigned int maxPases = 1;
    ACG::Vec3d bbmin,bbmax;
    ACG::SceneGraph::analyzeSceneGraph(PluginFunctions::getSceneGraphRootNode(),maxPases,bbmin,bbmax);

    //update Viewers
    for ( unsigned int i = 0 ; i < OpenFlipper::Options::examinerWidgets() ; ++i ) {

      examiner_widgets_[i]->sceneGraph( PluginFunctions::getSceneGraphRootNode(), maxPases,bbmin,bbmax );
      examiner_widgets_[i]->trackMouse(false);

      if(_am == Viewer::PickingMode) {
        if (pick_mode_idx_ != -1) {
          examiner_widgets_[i]->trackMouse(pick_modes_[pick_mode_idx_].tracking() );
        }
      }
    }

    //emit pickmodeChanged with either the name of the current pickmode or an empty string
    if( pickingMode() )
      emit(signalPickModeChanged(pick_mode_name_));
    else
      emit(signalPickModeChanged(""));
  }
}

//-----------------------------------------------------------------------------

void CoreWidget::getActionMode(Viewer::ActionMode& _am){
  _am = actionMode_;
}

//-----------------------------------------------------------------------------

void CoreWidget::setPickMode(const std::string& _mode){

  for (unsigned int i=0; i<pick_modes_.size(); ++i)
  {
    if (pick_modes_[i].name() == _mode)
    {
      pickMode( i );
      updatePickMenu();
      return;
    }
  }
}

//-----------------------------------------------------------------------------

void CoreWidget::getPickMode(std::string& _mode){
  _mode = pick_mode_name_;
}

//-----------------------------------------------------------------------------

void CoreWidget::setActivePickToolBar(QToolBar* _tool) {

    if(_tool != 0) {

      // Hide all picking toolbars
      hidePickToolBar();

      if ( OpenFlipperSettings().value("Core/Gui/ToolBars/PickToolbarInScene",true).toBool() ) {

        // Try to find toolbar in local map ( if it is in the gl scene
        PickToolBarMap::iterator ret = curPickingToolbarItems_.find(_tool);
        if(ret == curPickingToolbarItems_.end()) {

          // Set horizontal orientation
          _tool->setOrientation(Qt::Horizontal);

          // Update size as the orientation changed
          _tool->adjustSize();

          // Add widget to the gl scene
          QGraphicsProxyWidget* item = glScene_->addWidget(_tool);

          // Put it into center of the screen
          int midP = (glScene_->width() / 2) - (int)(_tool->width() / 2);
          item->setPos(midP, 3);

          item->show();

          // Remember it as being part of the scene
          curPickingToolbarItems_.insert(std::pair<QToolBar*,QGraphicsProxyWidget*>(_tool,item));
        } else {
          // Widget has already been added once, so just show it
          ret->second->show();
        }
      } else {

        // Try to find toolbar in local map
        // If its in there, we need to remove it from the graphics scene before adding it
        // to the side toolbar.
        PickToolBarMap::iterator ret = curPickingToolbarItems_.find(_tool);
        if(ret != curPickingToolbarItems_.end()) {

          glScene_->removeItem(ret->second);
          ret->first->setParent(0);

          // remove from list of widgets in glScene
          curPickingToolbarItems_.erase(ret);
        }

        pickToolBarExternal_ = _tool;

        // Check whether this toolbar has been displayed before
        if(registeredToolbars_.count(_tool) == 0) {

          /*
           * This code makes sure that the orientation and size
           * of a toolbar is only set once (the first time it is added).
           * In some cases, the user wants to move the toolbar somewhere
           * else and thus this check makes sure that it does not
           * loose its position and orientation after a pickmode change.
           */

          // Adjust its size and orientation
          pickToolBarExternal_->setOrientation(Qt::Vertical);
          pickToolBarExternal_->adjustSize();

          addToolBar(Qt::LeftToolBarArea,_tool);

          // Add to registered toolbars
          registeredToolbars_.insert(_tool);
        }

        _tool->show();
      }
    } else {
        hidePickToolBar();
    }
}

//-----------------------------------------------------------------------------

void CoreWidget::hidePickToolBar() {

  // Hide all picking toolbars
  for(PickToolBarMap::iterator it = curPickingToolbarItems_.begin();
      it != curPickingToolbarItems_.end(); ++it) {
    it->second->hide();
  }

  // if a toolbar is in the global scene, we remove it here.
  if ( pickToolBarExternal_ != 0 ) {
    pickToolBarExternal_->hide();
    pickToolBarExternal_ = NULL;
  }




}

//-----------------------------------------------------------------------------

void CoreWidget::updatePickMenu()
{
  if (pickMenu_ != 0) {
    pickMenu_->clear();
  } else {
    pickMenu_ = new QMenu( 0 );
    connect( pickMenu_, SIGNAL( aboutToHide() ),
         this, SLOT( hidePopupMenus() ) );
  }

  QActionGroup * ag = new QActionGroup( pickMenu_ );
  ag->setExclusive( true );

  for (unsigned int i=0; i<pick_modes_.size(); ++i) {
    if ( !pick_modes_[i].visible() )
      continue;

    if (pick_modes_[i].name() == "Separator")
    {
      if ((i > 0) && (i<pick_modes_.size()-1)) // not first, not last
        pickMenu_->addSeparator();
    }
    else
    {
      QAction* ac = new QAction( pick_modes_[i].name().c_str(), ag );
      ac->setData( QVariant( i ) );
      ac->setCheckable( true );

      if ((int)i == pick_mode_idx_)
        ac->setChecked( true );

      pickMenu_->addAction( ac );
    }
  }

  connect( ag, SIGNAL( triggered( QAction * ) ),
     this, SLOT( actionPickMenu( QAction * ) ));
}

//-----------------------------------------------------------------------------


void CoreWidget::actionPickMenu( QAction * _action )
{
  int _id = _action->data().toInt();
  if (_id < (int) pick_modes_.size() )
  {
    pickMode( _id );
  }

  setPickingMode();

  hidePopupMenus();
}

//-----------------------------------------------------------------------------


void CoreWidget::hidePopupMenus()
{

  if ( pickMenu_ )
  {
    pickMenu_->blockSignals(true);
    pickMenu_->hide();
    pickMenu_->blockSignals(false);
  }
}

//-----------------------------------------------------------------------------


void CoreWidget::pickMode( int _id )
{
  if (_id < (int) pick_modes_.size() )
  {
    pick_mode_idx_  = _id;
    pick_mode_name_ = pick_modes_[pick_mode_idx_].name();

    if (pick_modes_[pick_mode_idx_].toolbar() )
      setActivePickToolBar(pick_modes_[pick_mode_idx_].toolbar());
    else
      hidePickToolBar();

    // adjust mouse tracking
    if ( pickingMode() )
      for ( unsigned int i = 0 ; i < OpenFlipper::Options::examinerWidgets() ; ++i )
        examiner_widgets_[i]->trackMouse(pick_modes_[pick_mode_idx_].tracking() );

    // adjust Cursor
    if ( pickingMode() )
        cursorPainter_->setCursor( pick_modes_[pick_mode_idx_].cursor() );

    // emit signal
    emit(signalPickModeChanged(pick_mode_name_));
  }
}


//-----------------------------------------------------------------------------


void CoreWidget::addPickMode(const std::string& _name,
                               bool _tracking,
                               int  _pos,
                               bool _visible,
                               QCursor _cursor)
{

  if ((unsigned int)_pos < pick_modes_.size())
  {
    std::vector<PickMode>::iterator it = pick_modes_.begin();
    it += _pos+1;
    pick_modes_.insert(it, PickMode(_name, _tracking, _visible, _cursor));
  }
  else
    pick_modes_.push_back(PickMode(_name, _tracking, _visible, _cursor));

  updatePickMenu();
}

//-----------------------------------------------------------------------------

void CoreWidget::setPickModeCursor(const std::string& _name, QCursor _cursor)
{
  for (uint i=0; i < pick_modes_.size(); i++)
    if ( pick_modes_[i].name() == _name ){
      pick_modes_[i].cursor( _cursor );

      //switch cursor if pickMode is active
      if (pick_mode_name_ == _name && pickingMode() )
          cursorPainter_->setCursor(_cursor);
      break;
    }
}

//-----------------------------------------------------------------------------

void CoreWidget::setPickModeMouseTracking(const std::string& _name, bool _mouseTracking)
{
  for (uint i=0; i < pick_modes_.size(); i++)
    if ( pick_modes_[i].name() == _name ){

      pick_modes_[i].tracking(_mouseTracking );

      //switch cursor if pickMode is active
      if (pick_mode_name_ == _name && pickingMode() )
        for ( unsigned int i = 0 ; i < OpenFlipper::Options::examinerWidgets() ; ++i )
          examiner_widgets_[i]->trackMouse(_mouseTracking);
      break;
    }
}

//-----------------------------------------------------------------------------

void CoreWidget::setPickModeToolbar( const std::string& _mode , QToolBar * _toolbar )
{
  // Get the pickmode that belongs to the given name
  for (uint i=0; i < pick_modes_.size(); i++)
    if ( pick_modes_[i].name() == _mode ){

      // Set the new toolbar for that mode
      pick_modes_[i].toolbar( _toolbar );

      // Activate the toolbar if this mode is currently active
      if (pick_mode_name_ == _mode && pickingMode() )
        setActivePickToolBar(_toolbar);

      break;
    }
}

//-----------------------------------------------------------------------------

void CoreWidget::removePickModeToolbar( const std::string& _mode )
{
  for (uint i=0; i < pick_modes_.size(); i++)
    if ( pick_modes_[i].name() == _mode ){

      pick_modes_[i].toolbar(0);

      if (pick_mode_name_ == _mode && pickingMode() )
        hidePickToolBar();
      break;
    }
}

//-----------------------------------------------------------------------------

void CoreWidget::clearPickModes()
{
  pick_modes_.clear();
  pick_mode_idx_  = -1;
  pick_mode_name_ = "";
  updatePickMenu();
}


//-----------------------------------------------------------------------------

void CoreWidget::expandToolBoxWidget(QWidget *widget, bool expand) {
    toolBox_->expand(widget, expand);
}


const std::string& CoreWidget::pickMode() const
{
  return pick_mode_name_;
}



