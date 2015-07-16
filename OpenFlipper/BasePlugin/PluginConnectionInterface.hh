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


#ifndef PLUGINCONNECTIONINTERFACE_HH
#define PLUGINCONNECTIONINTERFACE_HH

#include <QString>

/** \file PluginConnectionInterface.hh
*
* Interface for connecting signals and slots across plugins. \ref pluginConnectionInterfacePage
*/

/**
  * \brief Allow to connect slots between plugins
  *
  * \ref pluginConnectionInterfacePage "Detailed description"
  * \n
  *
  * Interface Class which allows to connect signals and slots across plugins
  *
  * \n
  *  <b> WARNING! Never connect SIGNALS and SLOTS which are already defined in the Interfaces!! WARNING! </b>
  * \n
  *
  * This might result in endless loops and breaks OpenFlippers control loops. You have to keep track
  * of your connections yourself. If you call slots which call updatedOject which in turn call your original
  * slot you get a loop and OpenFlipper is not responsible for your endless loops! Be careful!
 */
class PluginConnectionInterface {
  
   signals :
     /** \brief connect signals and slots of plugins across plugins
     *
     * Connect a signal of one plugin to another plugin by its scripting name.
     * _pluginName1 is the scripting name of the plugin containing the signal ( Use script editor to check that name)
     * _signal is the signal name. Use it like with regular connect ( <tt>SIGNAL(...)</tt> ) macro
     * _pluginName2 is the scripting name of the plugin containing a slot
     * _slot is the slot name. Use it like with regular connect ( <tt>SLOT(...)</tt> ) macro
     *
     */
     virtual void crossPluginConnect( QString /*_pluginName1*/, const char* /*_signal*/, QString /*_pluginName2*/, const char* /*_slot*/) {};


   public :

      /// Destructor
      virtual ~PluginConnectionInterface() {};

};

/** \page pluginConnectionInterfacePage Plugin Connection Interface
\n
\image html PluginConnectionInterface.png
\n

The PluginConnectionInterface allows to connect signals and slots across different OpenFlipper plugins.

Usually you should implement the BaseInterface::pluginsInitialized() function from BaseInterface. In this function you can setup
your connections. Don't try to setup your connections earlier as the plugins you try to connect may not exist yet.

\n
 <b> WARNING! Never connect SIGNALS and SLOTS which are already defined in other Interfaces!! WARNING! </b>
\n
This might result in endless loops and breaks OpenFlippers control loops. You have to keep track
of your connections yourself. If you call slots which call updatedOject which in turn call your original
slot you get a loop and OpenFlipper is not responsible for your endless loops! Be careful!

The following code shows a simple example to connect signals and slots. For the signal and slot definition
you have to use the common QT macros SIGNAL and SLOT.
\code
void ExamplePlugin::pluginsInitialized()
{
 // Use the QT macros to generate the signatures
 emit crossPluginConnect(pluginA,SIGNAL(signalA(QString)),pluginB,SLOT(slotB(QString)));
}
\endcode

To use the ScriptInterface:
<ul>
<li> include PluginConnectionInterface.hh in your plugins header file
<li> derive your plugin from the class PluginConnectionInterface
<li> add Q_INTERFACES(PluginConnectionInterface) to your plugin class
<li> And add the signals or slots you want to use to your plugin class (You don't need to implement all of them)
</ul>

*/


Q_DECLARE_INTERFACE(PluginConnectionInterface,"OpenFlipper.PluginConnectionInterface/1.0")

#endif // PLUGINCONNECTIONINTERFACE_HH
