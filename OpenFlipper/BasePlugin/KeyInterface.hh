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


#ifndef KEYINTERFACE_HH 
#define KEYINTERFACE_HH 


#if QT_VERSION >= 0x050000 
  #include <QtWidgets>
#else
  #include <QtGui>
#endif

 


/** \file KeyInterface.hh
*
* Interface for receiving keyboard events inside your plugin.\ref keyInterfacePage
*/

/** \brief Keyboard Event Interface
 *
 * \ref keyInterfacePage "Detailed description"
 * \n
 *
 * Using this interface you can react on key events.
 */
class KeyInterface {
   signals:
      /**  \brief Register a key-combination for your plugin
       * 
       * To obtain key events from the viewer for a certain key-combination, you have
       * to register that combination first
       *
       * @param _key the key that should be registered
       * @param _modifiers the keyboard modifiers
       * @param _description a short description about the functionality (Will be used in the Options Widget)
       * @param _multiUse can the key additionally be registered by another plugin (You could make a key exclusive by this)
       */
      virtual void registerKey(int _key, Qt::KeyboardModifiers _modifiers, QString _description, bool _multiUse = false) {};

   public slots :
      
      /**  \brief Key Event from Main App
       * 
       * This slot is called if a key event occurred in the Viewer
       * @param _event Key event
      */
      virtual void slotKeyEvent( QKeyEvent* _event ) {};
      
      /**  \brief Key Release Event from Main App
       * 
       * This slot is called if a key release event occurred in the Viewer
       * @param _event Key event
      */
      virtual void slotKeyReleaseEvent( QKeyEvent* _event ) {};
      
   public : 
       
      /// Destructor
      virtual ~KeyInterface() {};
      
};


/** \page keyInterfacePage Key Interface
\image html keyInterface.png
\n

Using this interface you can react on key events. You first have to register
a key (or key combination) with KeyInterface::registerKey().
Note that the log widget will contain a warning if you register
a key twice or it is occupied by another plugin.

For each registered key your KeyInterface::slotKeyEvent() will be triggered when
the key was pressed and the slot KeyInterface::slotKeyReleaseEvent() when the key
was released. Both slots get the QKeyEvent and will contain all the modifiers
that were active when the event occurred.

In OpenFlippers options tab you can view all registered keys and reassign them.
The events will be mapped, such that the plugins still receive the right combination.
Therefore the mapping will be completely invisible to them.

See our tutorial \ref ex3 for an example of how to use mouse and keyboard events
within a plugin.

To use the KeyInterface:
<ul>
<li> include KeyInterface.hh in your plugins header file
<li> derive your plugin from the class KeyInterface
<li> add Q_INTERFACES(KeyInterface) to your plugin class
<li> Implement the required functions.
</ul>

\code

// In your plugin initialization register the keys you want to get.
void ExamplePlugin::initializePlugin() {

  // Register keys
  emit registerKey(Qt::Key_W,     Qt::NoModifier, "Rotate object down");
  emit registerKey(Qt::Key_A,     Qt::NoModifier, "Rotate object left");
  emit registerKey(Qt::Key_D,     Qt::NoModifier, "Rotate object right");
  emit registerKey(Qt::Key_S,     Qt::NoModifier, "Rotate object up");

}

// Called when one of the registered Keys is pressed.
void MouseAndKeyPlugin::slotKeyEvent( QKeyEvent* _event ) {

  // Check if it's really the right key
  if ( _event->modifiers() == Qt::NoModifier ) {

    // Switch pressed keys
    switch (_event->key()) {
      case Qt::Key_W:
        // Move forward
        break;
      case Qt::Key_A:
        // Move left
        break;
      case Qt::Key_D:
        // Move right
        break;
      case Qt::Key_S:
        // Move backward
        break;
      default:
        break;
    }

  }
}
\endcode
*/

Q_DECLARE_INTERFACE(KeyInterface,"OpenFlipper.KeyInterface/1.1")
      
#endif // KEYINTERFACE_HH
