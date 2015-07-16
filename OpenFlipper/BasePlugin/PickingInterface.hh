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

#ifndef PICKINGINTERFACE_HH
#define PICKINGINTERFACE_HH

 #include <OpenFlipper/common/Types.hh>

#if QT_VERSION >= 0x050000 
  #include <QtWidgets>
#else
  #include <QtGui>
#endif

 #include <QMenuBar>




/** \file PickingInterface.hh
*
* Interface Class which allows access to picking functions. \ref pickingInterfacePage
*/

/** \brief Allow access to picking functions.
  *
  * \ref pickingInterfacePage "Detailed description"
  * \n
  *
  * Using this interface you can add different pick modes to OpenFlipper and
  * separate different mouse Interactions depending on the current mode.
  *
  * Read tutorial \ref ex3 for an example of how to use mouse picking.
 */
class PickingInterface {
   signals :
      /** \brief Add a new picking mode to the examiner
       *
       * @param _mode Identifier of Picking mode or "Separator" to add a Separator
      */
      virtual void addPickMode( const std::string& _mode) {};

      /** \brief Add a new picking mode to the examiner which will be invisible
       *
       * The added PickMode will not appear in the context menus Picking menu.
       * You have to provide a button or menu entry yourself if you want to switch to
       * the picking mode provided here.
       *
       * @param _mode Identifier of Picking mode or "Separator" to add a Separator
      */
      virtual void addHiddenPickMode( const std::string& _mode ) {};

      /** \brief Set the cursor of the given PickMode
       *
       * Set the cursor that should be used inside the pickMode
       *
       * @param _mode Identifier of Picking mode
       * @param _cursor the new cursor
      */
      virtual void setPickModeCursor( const std::string& _mode , QCursor _cursor ) {};

      /** \brief Set mouse tracking for the given PickMode
       *
       * Enable mouseTracking for the given PickMode. When MouseTracking is enabled
       * you receive mouseMove events also when no button is pressed (via MouseInterface)
       *
       * @param _mode Identifier of Picking mode
       * @param _mouseTracking new state of mouseTracking
      */
      virtual void setPickModeMouseTracking( const std::string& _mode , bool _mouseTracking ) {};

      /** \brief Set the additional toolbar of the given PickMode
       *
       * Set the additional toolbar that should be shown in the pickMode
       *
       * @param _mode Identifier of Picking mode
       * @param _toolbar the toolbar
      */
      virtual void setPickModeToolbar( const std::string& _mode , QToolBar * _toolbar ) {};

      /** \brief Removes the additional toolbar of the given PickMode
       *
       * Set the additional toolbar that should be shown in the pickMode
       *
       * @param _mode Identifier of Picking mode
      */
      virtual void removePickModeToolbar( const std::string& _mode ) {};

     
  private slots:
      /**  \brief The pickingMode has changed
       *
       * This slot is called if the user changes the current picking mode
       * @param _mode Identifier of Picking mode
      */
      virtual void slotPickModeChanged( const std::string& _mode) {};

   public :

      /// Destructor
      virtual ~PickingInterface() {};

};

/** \page pickingInterfacePage Picking Interface/Mouse Picking/Pick Mode Toolbars
\n
\image html PickingInterface.png
\n


\section pickingInterface_functionality Functionality
OpenFlipper uses several different ActionModes. These include, Light interaction (Changing lights), Move interaction (navigating
through the scene) and the picking interaction. The picking interaction is separated into different pick modes. These modes
are usually defined by plugins and are used to restrict the mouse interaction to one plugin. E.g. if the current picking mode is
"vertex selection", all other plugins that react on mouse events, but are not responsible for the "vertex selection" will ignore
these events.

\section pickingInterface_managing Managing Pick Modes
Pick modes can be created in your plugin initialization. You can add them as visible pick modes (visible in the context menus
of objects) or as hidden pick modes PickingInterface::addHiddenPickMode() ( This should be preferred! ). You can also set
a special cursor that will be used when your pick mode is active via PickingInterface::setPickModeCursor(). If you need
to enable mouse tracking in your mode, you can use PickingInterface::setPickModeMouseTracking(). This will result in mouse events
even if no button is pressed.

\code
// When the plugin gets initialized
void ExamplePlugin::pluginsInitialized() {

 // Add pick mode
 emit addPickMode("ExamplePlugin Pick Mode");

}

// Triggered when the pick mode got changed.
void ExamplePlugin::slotPickModeChanged(const std::string& _mode) {

  // Enable a button depending on the current pick mode
  button_->setEnabled(_mode == "ExamplePlugin Pick Mode");

}

// From MouseInterface
void ExamplePlugin::slotMouseEvent(QMouseEvent* _event) {

  // Check if your pick mode is currently active
  if ( PluginFunctions::pickMode() == "ExamplePlugin Pick Mode" && PluginFunctions::actionMode() == Viewer::PickingMode ) {

    // Do something

  }

}
\endcode

A change of the current pick mode can be detected by the PickingInterface::slotPickModeChanged() function.

\section pickingInterface_toolbars PickMode Toolbars
\n
\image html PickModeToolbar.png
\n
Additionally it is possible to show a special toolbar in OpenFlippers viewer when your pick mode is active. This is especially
Useful, when you can change the interaction type by buttons in the bar, e.g. select vertices or faces. Every time your
pick mode is activated, your toolbar will be visible at the top of the viewer. Use the functions PickingInterface::setPickModeToolbar()
and PickingInterface::removePickModeToolbar() for controlling these toolbars. The embedding is fully transparent so that
you can manage your toolbar and connect signals and slots as usual.

\code
// When the plugin gets initialized
void ExamplePlugin::pluginsInitialized() {

  // Global variable QToolBar* pickToolbar_
  pickToolbar_ = new QToolBar(tr("Example Pickmode Toolbar"));
  pickToolbar_->setAttribute(Qt::WA_AlwaysShowToolTips, true);

  QActionGroup* pickToolBarActions = new QActionGroup(pickToolbar_);

  QAction* action = new QAction(tr("Action"),pickToolBarActions);
  action->setStatusTip(tr("Action description"));
  action->setIcon(QIcon(OpenFlipper::Options::iconDirStr()+OpenFlipper::Options::dirSeparator()+"icon.png") );
  action->setCheckable(true);
  pickToolbar_->addAction(action);

  // Connect to a local slot. The embedding is transparent, so you can connect all your actions and toolbars as usual.
  connect(pickToolBarActions, SIGNAL(triggered(QAction*)), this, SLOT(slotPickToolbarAction(QAction*)) );

  emit setPickModeToolbar ("ExamplePlugin Pick Mode", pickToolbar_);
}
\endcode


\section pickingInterface_usage Usage
To use the PickingInterface:
<ul>
<li> include PickingInterface.hh in your plugins header file
<li> derive your plugin from the class PickingInterface
<li> add Q_INTERFACES(PickingInterface) to your plugin class
<li> And add the signals or slots you want to use to your plugin class (You don't need to implement all of them)
</ul>

*/






Q_DECLARE_INTERFACE(PickingInterface,"OpenFlipper.PickingInterface/1.1")

#endif // PICKINGINTERFACE_HH
