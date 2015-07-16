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

#ifndef STATUSBARINTERFACE_HH
#define STATUSBARINTERFACE_HH

#include <OpenFlipper/common/Types.hh>

#if QT_VERSION >= 0x050000 
  #include <QtWidgets>
#else
  #include <QtGui>
#endif

#include <QStatusBar>


/** \file StatusbarInterface.hh
*
* Interface for controlling OpenFlippers statusbar .\ref statusbarInterfacePage
*/

/** Namespace for Application status enum
 *
 */
namespace ApplicationStatus {
  
  /** \brief Enum for the statusBar Status Icon
   *
   * This enum is used to controll the small activity icon of OpenFlipper.
   * See \ref statusbarInterfacePage
   */
  enum applicationStatus {
    READY,     //!< Status is ready (green light)
    PROCESSING,//!< Status is processing but system will allow interaction (yellow light)
    BLOCKED    //!< Status is processing and blocked system will not allow interaction (red light)
  };

}


 /** \brief Control OpenFlippers status bar
 *
 * \ref statusbarInterfacePage "Detailed description"
 * \n
 *
 * This interface can be used to send messages to OpenFlippers statusbar.
 * It is also possible to add your own widget to the bar.
 */
class StatusbarInterface {

   private slots :

   public :

      /// Destructor
     virtual ~StatusbarInterface() {};

    signals :

      /** \brief Show a message in the status bar
       * 
       * You can specify a timeout if the message should be removed after
       * _timeout milliseconds
       */
      virtual void showStatusMessage(QString _message, int _timeout = 0) {};
      
      
      /** \brief Sets the status icon of the StatusBar
       *
       * OpenFlipper statusbar contains a small icon in the statusbar which can be used to indicate
       * the current status of the application. The valid values are defined in the enum ApplicationStatus::applicationStatus.
       * 
       * @param _status New status
       */
      void setStatus( ApplicationStatus::applicationStatus _status) {};
      
      /** \brief Removes the current message from the statusBar
       * 
       */
      void clearStatusMessage() {};
      
      /** \brief Add a widget on the right side of the statusBar
       * 
       * Use this function if you want to add an additional widget to the statusbar.
       * It will be added on the right side.
       *
       * @param _widget The widget that should be added to the statusbar
       */
      void addWidgetToStatusbar(QWidget* _widget) {};
      
};


/** \page statusbarInterfacePage Statusbar Interface
\image html StatusbarInterface.png
\n
The StatusbarInterface can be used by plugins to send messages to OpenFlippers
statusbar. The statusbar is located below the GL viewer.

Messages can be controlled via StatusbarInterface::setStatus(),StatusbarInterface::clearStatusMessage()

Additionally it is possible to add small widgets to it ( StatusbarInterface::addWidgetToStatusbar).

A small icon can be controlled to indicate OpenFlippers status via StatusbarInterface::setStatus().

To use the StatusbarInterface:
<ul>
<li> include StatusbarInterface.hh in your plugins header file
<li> derive your plugin from the class StatusbarInterface
<li> add Q_INTERFACES(StatusbarInterface) to your plugin class
<li> And add the signals or slots you want to use to your plugin class (You don't need to implement all of them)
</ul>

The following code shows a simple example to send a message to the statusbar.
\code
void ExamplePlugin::function()
{
  ...

  // Print the text "Your Status Message" for 4 seconds in the statusBar
  emit showStatusMessage( tr("Your Status Message"), 4000 );

  ...
}
\endcode

The following code shows a simple example to add a widget to the statusbar.
Usually you should implement the BaseInterface::pluginsInitialized() function from BaseInterface.
In this function you can setup your widget for the status bar.
\code
void ExamplePlugin::pluginsInitialized()
{

  // Create the widget
  QWidget* infoBar = new QWidget();

  // Setup buttons or anything else in your widget
  ...

  // Add the new widget to the statusBar
  emit addWidgetToStatusbar(infoBar);

}
\endcode

*/



Q_DECLARE_INTERFACE(StatusbarInterface,"OpenFlipper.StatusbarInterface/1.0")

#endif // STATUSBARINTERFACE_HH
