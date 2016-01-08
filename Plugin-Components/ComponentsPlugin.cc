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

#include "ComponentsPlugin.hh"

#if QT_VERSION >= 0x050000
#else
#include <QtGui>
#endif

const char *SPLIT_COMPONENTS = "SplitComponents";
const char *BIGGEST_COMPONENT = "ComponentsPluginBiggestComponent";

//------------------------------------------------------------------------------

/** \brief Constructor
 *
 */
ComponentsPlugin::ComponentsPlugin():
    splitAction_(NULL),
    biggestAction_(NULL){}

/*******************************************************************************
        BaseInterface implementation
 *******************************************************************************/

/** \brief Initialize the plugin
 *
 */
void ComponentsPlugin::initializePlugin() {
}


//------------------------------------------------------------------------------

/** \brief Second initialization phase
 *
 */
void ComponentsPlugin::pluginsInitialized()
{

  if ( ! OpenFlipper::Options::gui())
    return;

  setDescriptions();

  emit addPickMode( SPLIT_COMPONENTS );
  emit addPickMode( BIGGEST_COMPONENT );
  
  // Add a scissor Toolbar
  QToolBar* toolbar = new QToolBar("Components Toolbar");
  //Split components
  splitAction_ = new QAction(tr("&Split into components"), this);
  splitAction_->setCheckable( true );
  splitAction_->setStatusTip(tr("Clicked objects are splitted into components"));
  splitAction_->setIcon(QIcon(OpenFlipper::Options::iconDirStr()+OpenFlipper::Options::dirSeparator()+"components_split_components.png") );
  connect(splitAction_, SIGNAL(triggered()), this, SLOT(slotSplitComponentsButton()) );
  toolbar->addAction(splitAction_);
  
  biggestAction_ = new QAction(tr("&Get biggest component"), this);
  biggestAction_->setCheckable( true );
  biggestAction_->setStatusTip(tr("Get the biggest component of the clicked object and delete the other components."));
  biggestAction_->setIcon(QIcon(OpenFlipper::Options::iconDirStr()+OpenFlipper::Options::dirSeparator()+"components_biggest_component.png") );
  connect(biggestAction_, SIGNAL(triggered()), this, SLOT(slotBiggestComponentButton()) );
  toolbar->addAction(biggestAction_);

  emit addToolbar( toolbar );
}

/*******************************************************************************
    PickingInterface implementation
 *******************************************************************************/

/** \brief the pickMode changed
 *
 * @param _mode the new pickMode
 */
void ComponentsPlugin::slotPickModeChanged( const std::string& _mode){
  splitAction_->setChecked( _mode == SPLIT_COMPONENTS );
  biggestAction_->setChecked( _mode == BIGGEST_COMPONENT );
}

/*******************************************************************************
    MouseInterface implementation
 *******************************************************************************/

/** \brief a mouse event occured
 *
 * @param _event the event that occured
 */
void ComponentsPlugin::slotMouseEvent( QMouseEvent* _event )
{
  if( PluginFunctions::pickMode() == SPLIT_COMPONENTS )
    splitComponents( _event );
  if (PluginFunctions::pickMode() == BIGGEST_COMPONENT)
    biggestComponent( _event );
}

/*******************************************************************************
         ComponentsPlugin Implementation
 *******************************************************************************/

/** \brief Split into Components Button was hit
 *
 */
void ComponentsPlugin::slotSplitComponentsButton( )
{
  PluginFunctions::actionMode( Viewer::PickingMode );
  PluginFunctions::pickMode( SPLIT_COMPONENTS );
}


//------------------------------------------------------------------------------

/** \brief Split components (triggered by pickMode)
 *
 * @param _event the mouse event that occured
 */
void ComponentsPlugin::splitComponents(QMouseEvent * _event)
{
  if (_event->type() == QEvent::MouseButtonPress )
  {
    unsigned int     node_idx, target_idx;
    ACG::Vec3d*      sourcePoint3D = 0;

    if (PluginFunctions::scenegraphPick(ACG::SceneGraph::PICK_FACE,
                                                    _event->pos(),
                                                    node_idx,
                                                    target_idx,
                                                    sourcePoint3D))
    {
      BaseObjectData *obj;
      PluginFunctions::getPickedObject(node_idx, obj);
      splitComponents( obj->id() );
    }
  }
}


void ComponentsPlugin::slotBiggestComponentButton()
{
  PluginFunctions::actionMode( Viewer::PickingMode );
  PluginFunctions::pickMode( BIGGEST_COMPONENT );
}

void ComponentsPlugin::biggestComponent(QMouseEvent* _event)
{
  if (_event->type() == QEvent::MouseButtonPress )
  {
    unsigned int     node_idx, target_idx;
    ACG::Vec3d*      sourcePoint3D = 0;

    if (PluginFunctions::scenegraphPick(ACG::SceneGraph::PICK_FACE,
        _event->pos(),
        node_idx,
        target_idx,
        sourcePoint3D))
    {
      BaseObjectData *obj;
      PluginFunctions::getPickedObject(node_idx, obj);

      if (!obj)
      {
        emit log(LOGERR,tr("Unable to pick Object."));
        return;
      }

      biggestComponent(obj->id());

      emit createBackup(obj->id(),"GetBiggestComponents",UPDATE_ALL);
      emit updatedObject( obj->id() , UPDATE_ALL );
    }
  }
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2( componentsplugin, ComponentsPlugin );
#endif
