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

#ifndef SCRIPTINTERFACE_HH
#define SCRIPTINTERFACE_HH

#include <QtScript/QScriptEngine>

/** \file ScriptInterface.hh
*
* Interface for controlling OpenFlippers scripting system. \ref scriptInterfacePage
*/

/** \brief Interface for all Plugins which provide scriptable Functions.
 *
 * \ref scriptInterfacePage "Detailed description"
 * \n
 *
 *
*/
class ScriptInterface {

   public:
      /// Destructor
      virtual ~ScriptInterface() {};


  //===========================================================================
  /** @name Script execution
   * @{ */
  //===========================================================================

  signals:
    /** Emit this signal to execute a script
     *
     * @param _script The script to execute
     */
    virtual void executeScript( QString _script  ) {};

    /** Emit this signal to execute a script from a file
      * @param _filename Filename of the script to execute
     */
    virtual void executeFileScript( QString _filename  ) {};

  /** @} */

  //===========================================================================
  /** @name Function Information
  * @{ */
  //===========================================================================

  signals:

    /** With this signal you can get a list of all available scripting functions
      * @param _functions All available functions
     */
    virtual void getAvailableFunctions(QStringList& _functions) { };

    /** With this signal you can get descriptions about a given function if available
     *
     *   @param _function name of the function (e.g. plugin.function)
     *   @param _description a description for the function
     *   @param _parameters list of names for the parameters
     *   @param _descriptions list of descriptions for the parameters (_descriptions[i] corresponds to _parameters[i])
     */
    virtual void getDescription(QString _function, QString& _description, QStringList& _parameters, QStringList& _descriptions) { };

  /** @} */

  //===========================================================================
  /** @name Scripting History
  * @{ */
  //===========================================================================
  signals:

       /** \brief Emit this signal if a scriptable function is executed
         *
         * e.g. selectAllVertices( ObjectId )
         *
         * If you execute this function, you can append your functions to the script
         * execution history in OpenFlipper. In this history OpenFlipper records operations
         * that are scriptable. Later this list can be executed again and therefore
         * reruns the recorded operations.
         *
         * The following example code shows a function called translate in a plugin,
         * that gets an ObjectId as a parameter. The plugin which emits this function
         * will automatically be added in the history ( e.g. \code move.translate( ObjectId , Vector(1 , 0 , 0 ) ) \endcode )
         * \code
         *   emit scriptInfo( "translate( ObjectId , Vector(1 , 0 , 0 ) )" );
         * \endcode
         *
         * @param _functionWithParameters The called function with all parameters
         */
      virtual void scriptInfo( QString _functionWithParameters  ) {};


  /** @} */

  //===========================================================================
  /** @name Others
  * @{ */
  //===========================================================================

  signals:
      /** Emit this signal to get a reference to the core scripting engine
        *
        * @param _engine The scripting Engine
        */
      virtual void getScriptingEngine( QScriptEngine*& _engine  ) {};

  /** @} */


  //===========================================================================
  /** @name Slots for a scripting control plugin like Plugin-Script
    * @{ */
  //===========================================================================

  private slots:

    /** Slot for a scripting plugin. Gets the Script and executes it.
     *
     * @param _pluginName Name of the plugin that executed a scriptable function
     * @param _functionWithParameters Name with parameters (values!) of the function to call.
     */
    virtual void slotScriptInfo( QString _pluginName , QString _functionWithParameters  ) {};


    /** Slot for a scripting plugin. Gets the Script as QString and executes it.
     *
     * @param _script Script to execute as a QString
     */
    virtual void slotExecuteScript( QString _script ) {};


    /** Slot for a scripting plugin. Gets the Script as a filename, opens and executes it.
     */
    virtual void slotExecuteFileScript( QString _filename ) {};

  /** @} */

};

/** \page scriptInterfacePage Scripting Interface
\n
\image html ScriptInterface.png
\n

OpenFlipper uses QTs scripting system to provide scripting functions to the user and to the plugins.
It also includes a batch mode where OpenFlipper is started without an user interface. All plugins
which support this mode can than be controlled by command line supplied batch scripts without any user
interaction.

The ScriptingInterface has several functions to support scripting.

<ul>
<li> Execute scripts which are provided in a simple QString (ScriptInterface::executeScript()).
     \code
       emit executeScript( "translate( 5 , Vector(1 , 0 , 0 ) )" );
     \endcode
     This will translate The object with id 6 by the given vector.
<li> Execute scripts which are provided in a file (ScriptInterface::executeFileScript()).
     \code
       emit executeFileScript( "/home/user/script.ofs" );
     \endcode
<li> If a scriptable slot is executed, it can add an entry to OpenFlippers script history. This history can
     later be used to rerun a process in batch mode ( ScriptInterface::scriptInfo() ).

     The following example code shows a function called translate in a plugin,
     that gets an ObjectId as a parameter. The plugin which emits this function
     will automatically be added in the history ( e.g. <tt> move.translate( ObjectId , Vector(1 , 0 , 0 ) ) </tt> )

     \code
       emit scriptInfo( "translate( ObjectId , Vector(1 , 0 , 0 ) )" );
     \endcode
<li> Get Information about scriptable functions ( ScriptInterface::getAvailableFunctions(),ScriptInterface::getDescription() ).

     The first function ( ScriptInterface::getAvailableFunctions() ) can be used to get a QStringlist of all available
     functions. Each string is of the form <tt>pluginname.functionname</tt>
     \code
       // Update list of available functions
       QStringList completeList;
       emit getAvailableFunctions( completeList  );
     \endcode

     The second function ( ScriptInterface::getDescription() ) can be used to get information about a function, if it was
     provided by the developer.
</ul>


To use the ScriptInterface:
<ul>
<li> include ScriptInterface.hh in your plugins header file
<li> derive your plugin from the class ScriptInterface
<li> add Q_INTERFACES(ScriptInterface) to your plugin class
<li> And add the signals or slots you want to use to your plugin class (You don't need to implement all of them)
</ul>

An easier interface to call functions is available by the \ref RPCInterfacePage "RPC Interface". Additionally it is
possible to connect signals and slots across plugins via the  \ref pluginConnectionInterfacePage "Plugin Connection Interface"

*/


Q_DECLARE_INTERFACE(ScriptInterface,"OpenFlipper.ScriptInterface/1.1")

#endif // SCRIPTINTERFACE_HH
