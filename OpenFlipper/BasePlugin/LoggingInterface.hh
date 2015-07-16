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



#ifndef LOGGINGINTERFACE_HH 
#define LOGGINGINTERFACE_HH 

#include <QMetaType>

/** \file LoggingInterface.hh
*
* Interface for sending log messages to the log widget. \ref loggingInterfacePage
*/

/** \page loggingInterfacePage Logging Interface
\image html LoggingInterface.png
The Logging interface can be used by plugins to print messages in OpenFlippers log widget below 
the gl viewer (See image). The log widget can apply filters to visualize only the messages
of a certain importance. The widget has 3 different modes, which can be toggled by pressing Ctrl + L:
<ul>
<li> Docked automatic mode: In this mode the widget is at the bottom and will hide itself if not used.
<li> Docked mode: The widget is always visible and docked at the bottom
<li> Undocked: The widget is undocked into a separate window
</ul>

The message importance level can be specified by the Logtype enum. There are four different levels which 
describe the importance and are represented by different colors in the log widget.
<ul>
<li> Out:    Simple output messages in black
<li> Info:   Information messages printed in green
<li> Warn:   Warnings in yellow
<li> Errors: Error messages printed in red
</ul>

To use the LoggingInterface:
<ul>
<li> include LoggingInterface.hh in your plugins header file
<li> derive your plugin from the class LoggingInterface
<li> add Q_INTERFACES(LoggingInterface) to your plugin class 
<li> And add the signals or slots you want to use to your plugin class (You don't need to implement all of them)
</ul>

If you use the interface, all messages printed from that plugin will be prepended with the name of the plugin
to identify the origin of the message.\n
The following code sends a log message
\code 
  emit log(LOGERR,tr("Error message"));
\endcode

*/

/** \brief Log types for Message Window 
*
* Use this enum to specify the importance of log messages.
*/
enum Logtype {  LOGSTATUS , /*!< Status log messages. Will be printed in blue in the logwidget */
                LOGOUT    , /*!< Standard log messages. Will be printed in black in the logwidget */
                LOGINFO   , /*!< Info log messages. Will be printed in green in the logwidget */
                LOGWARN   , /*!< Warning messages. Will be printed in yellow in the logwidget */
                LOGERR      /*!< Error messages. Will be printed in red in the logwidget */
             };
Q_DECLARE_METATYPE(Logtype)

/** \brief Interface for all Plugins which do logging to the logging window of the framework
  * 
  * \ref loggingInterfacePage "Detailed description" 
  * \n
  * By emitting the given signals you can log information to the main logger window of the core.
  * To simplify debugging, the core will prepend the plugin name to every log message. You dont
  * have to do this yourself!\n
  * The log message will either be black or will be colored depending on the Logtype you specified.
  *
  * A more detailed description of the LoggingInterface can be found \ref loggingInterfacePage "here" or
  * read our tutorials \ref ex2 and \ref ex3 for an example of how to use logging output.
 */ 
class LoggingInterface {
  
   signals :
      /** Send a log message to the mainwindow of the widget \n
       * This signal can be called from any thread.\n
       *
       * @param _type Message type (LOGINFO,LOGOUT,LOGWARN,LOGERR)
       * @param _message Message to be displayed
       */
      virtual void log(Logtype _type, QString _message) = 0;
      
      /** Send a log message to the mainwindow of the widget \n
       * defaults to LOGOUT message type \n
       * This signal can be called from any thread.\n
       *
       * @param _message Message to be displayed
       */
      virtual void log(QString _message) = 0;
      
   private slots:
     
      /** Through this slot you can receive all logging information emitted by OpenFlipper
       * or one of its plugins
       *
       * @param _type    Message type
       * @param _message Message
       */
      virtual void logOutput( Logtype _type , QString _message ) {};

   public:

      /// Destructor
      virtual ~LoggingInterface() {};
};

Q_DECLARE_INTERFACE(LoggingInterface,"OpenFlipper.LoggingInterface/1.0")
      
#endif // LOGGINGINTERFACE_HH
