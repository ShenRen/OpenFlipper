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


#ifndef MOUSEINTERFACE_HH
#define MOUSEINTERFACE_HH


#if QT_VERSION >= 0x050000 
  #include <QtWidgets>
#else
  #include <QtGui>
#endif


/** \file MouseInterface.hh
*
* Interface for registering types in OpenFlipper. \ref mouseInterfacePage
*/


 /** \brief Interface class for receiving mouse events
 *
 * \n
 * \ref mouseInterfacePage "Detailed description"
 * \n
 *
 */
class MouseInterface {

  private slots :

    /**  \brief Wheel Event from main application
      *
      *  This slot is called if a wheel event occurred in the Viewer and is extended by
      *  the current Mode set in the Viewer. You can add additional Picking Modes to
      *  the Viewer by using the picking interface.
      *  This slot will only get called in pickingMode.
      *  @param _event Mouse event
      *  @param _mode Name of the current Picking Mode.
    */
    virtual void slotMouseWheelEvent(QWheelEvent * _event, const std::string & _mode) {};

    /**  \brief Mouse Event from main application ( Picking Mode )
      *
      *  This slot is called if a mouse event occurred in the Viewer
      *  This slot will only get called in pickingMode.
      *  Right button clicks will not be passed to the plugins as this is reserved for
      *  the context Menu.
      *
      *  @param _event Mouse event
    */
    virtual void slotMouseEvent( QMouseEvent* _event ) {};
    
    /**  \brief Mouse Event from main application ( Light Mode )
      *
      *  This slot is called if a mouse event occurred in the Viewer
      *  This slot will only get called in LightMode.
      *  Right button clicks will not be passed to the plugins as this is reserved for
      *  the context Menu.
      *
      *  @param _event Mouse event
    */
    virtual void slotMouseEventLight( QMouseEvent*  _event ) {};

  public :

    /// Destructor
    virtual ~MouseInterface() {};

};

/** \page mouseInterfacePage Mouse Interface
\n
\image html mouseInterface.png
\n


The mouse interface can be used to receive mouse events which occur in the glViewer.

There are 4 main viewer modes:
<ul>
<li> <b>Move Mode</b> events are never passed to one of the plugins and is only used for scene navigation
inside the viewers.</li>
<li> <b>Picking Mode</b> events where passed through the MouseInterface::slotMouseEvent() and MouseInterface::slotMouseWheelEvent() to the plugins. This is usually the mode, that plugins use to implement mouse interaction.</li>
<li> <b>Light Mode</b> events where passed through the MouseInterface::slotMouseEventLight(). Plugins modifying light sources use that mode to get mouse events. </li>
<li> <b>Info Mode</b> events are passed through the Information interface \ref informationInterfacePage. This mode is used to provide information about clicked objects only.</li>
</ul>

You can add handlers for these mouse events in your plugin. Remember that all plugins receive these signals.\n

The picking action is only a global mode which is divided into several other picking modes that can be managed
through the PickingInterface. If you react on mouse events, you should check if the current
picking mode is yours and of course define such a mode for your plugin.
\code
void MousePlugin::slotMouseEvent(QMouseEvent* _event) {

  // Check if your pick mode is currently active
  if ( PluginFunctions::pickMode() == "YourPickMode" && PluginFunctions::actionMode() == Viewer::PickingMode ) {

    // If double click has been performed
    if (_event->type() == QEvent::MouseButtonDblClick) {
      unsigned int node_idx, target_idx;
      OpenMesh::Vec3d hitPoint;

      // Get picked object's identifier by picking in scenegraph
      if ( PluginFunctions::scenegraphPick(ACG::SceneGraph::PICK_ANYTHING,_event->pos(), node_idx, target_idx, &hitPoint) ) {

        BaseObjectData* object;

        // Get picked object
        if ( PluginFunctions::getPickedObject(node_idx, object) ) {

          // Do something with the object

        }
      }
    }
  }


}
\endcode

See our tutorial \ref ex3 for an example of how to use mouse and keyboard events
within a plugin.

To use the MouseInterface:
<ul>
<li> include MouseInterface.hh in your plugins header file
<li> derive your plugin from the class MouseInterface
<li> add Q_INTERFACES(MouseInterface) to your plugin class
<li> And add the signals or slots you want to use to your plugin class (You don't need to implement all of them)
</ul>


*/

Q_DECLARE_INTERFACE(MouseInterface,"OpenFlipper.MouseInterface/1.0")

#endif // MOUSEINTERFACE_HH
