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


#ifndef RPCINTERFACE_HH
#define RPCINTERFACE_HH

#include <OpenFlipper/BasePlugin/RPCWrappers.hh>

/** \file RPCInterface.hh
*
* Interface for calling functions across plugins. \ref RPCInterfacePage
*/

/** \brief Interface to call functions across plugins.
 *
 * \ref RPCInterfacePage "Detailed description"
 * \n
 * \n
 *
 * Interface for all Plugins which do remote procedure calls ( and cross plugin calls).\n
 * The functions provided in the interface are only used to verify that certain plugins or
 * functions are available. The actual calls are in the RPCWrappers.hh file.
*/
class RPCInterface {

   public:

      /// Destructor
      virtual ~RPCInterface() {};

   signals :

     /** Check if the plugin exists in the current Environment \n
       *
       * @param _pluginName Name of the Plugin (has to be the clean version .. no Spaces etc)
       * @param _exists found or not
       */
    virtual void pluginExists( QString _pluginName , bool& _exists  ) {};


    /** Check if a plugin exists and provides the given function \n
       * @param _pluginName Name of the Plugin (has to be the clean version .. no Spaces etc)
       * @param _functionName requested function
       * @param _exists found or not
       */
    virtual void functionExists( QString _pluginName , QString _functionName , bool& _exists  ) {};

};

/** \page RPCInterfacePage RPC Interface
\image html RPCInterface.png
\n
The RPCInterface can be used by plugins to call functions from other OpenFlipper plugins. The calls are
internally transformed into scripting calls and passed to the other plugins.

You can directly connect signals and slots via the \ref pluginConnectionInterfacePage .

The interface itself contains two basic functions which can be used to check if another plugin exists (RPCInterface::pluginExists())
or if a specific function exists in another plugin (RPCInterface::functionExists()). Both functions  get the clean name
of the plugin to check. To get that name, just open the script editor in OpenFlipper which lists all plugins and their
available functions.

Example Code for plugins:
\code
  bool isPluginAvailable;
  emit pluginExists ("scripting", isPluginAvailable);
\endcode

Example Code for functions:
\code
  bool isFunctionAvailable;
  emit functionExists ("scripting", "showScriptInEditor(QString)", isFunctionAvailable);
\endcode

In addition to these basic functions there is a set of template functions in RPCWrappers.hh which can be used to directly call
a function from another plugin via the scripting system. This requires the parameters to be registered to the QT MetaObject system which
is already done for the most common types of variables.

For calling a function you can use RPC::callFunction() or RPC::callFunctionValue() if the called function returns a value.
These functions get the pluginName and the function name and call the requested function via scripting.
It is possible to supply additional parameters to these functions that will be passed to the called function.

Example code:
\code

  // call without getting return value
  RPC::callFunction ("pluginName", "FunctionName");

  // call without getting return value and suplying a parameter
  int parameter1
  RPC::callFunction ("pluginName", "FunctionName", parameter1);

  // call with getting return value which is defined as the template parameter
  QString parameter2
  bool value = RPC::callFunctionValue<bool> ("pluginName", "FunctionName", parameter1, parameter2);

\endcode


To use the RPCInterface:
<ul>
<li> include RPCInterface.hh in your plugins header file
<li> derive your plugin from the class RPCInterface
<li> add Q_INTERFACES(RPCInterface) to your plugin class
<li> And add the signals or slots you want to use to your plugin class (You don't need to implement all of them)
</ul>

\note Multithreading: You can call any script function from any thread, but the script will be executed on the main thread.

*/


Q_DECLARE_INTERFACE(RPCInterface,"OpenFlipper.RPCInterface/1.0")

#endif // RPCINTERFACE_HH
