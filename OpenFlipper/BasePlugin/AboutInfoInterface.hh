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
*   $Revision: 11127 $                                                       *
*   $LastChangedBy: moebius $                                                *
*   $Date: 2011-03-15 16:18:28 +0100 (Di, 15 Mär 2011) $                     *
*                                                                            *
\*===========================================================================*/


#pragma once


#if QT_VERSION >= 0x050000 
  #include <QtWidgets>
#else
  #include <QtGui>
#endif


/** \file AboutInfoInterface.hh
*
* Interface for adding a widget to OpenFlippers About dialog .\ref AboutInfoInterfacePage
*/

/** \brief About Info interface
 *
 * \ref AboutInfoInterfacePage "Detailed description"
 * \n
 *
 * This interface can be used to add a widget to OpenFlippers About dialog to display license Information or other data.
 */
class AboutInfoInterface {

public:
  /// Destructor
  virtual ~AboutInfoInterface() {};

signals :
  /**   \brief Adds a widget to OpenFlippers about dialog
   *
   *
   * @param _text    Text for the tab (Supports html tags)
   * @param _tabName Name of the tab to be added
   */
   virtual void addAboutInfo(QString _text, QString _tabName ) {};
};


/** \page AboutInfoInterfacePage About Info interface
\image html AboutInfoInterface.png
\n
The AboutInfoInterface can be used by plugins to add an additional tab to OpenFlippers about dialog.
This is especially useful, if you use an external library which requires you to publish the license information
in the application.

You can add multiple tabs by additional calls to this interface

To use the AboutInfoInterface:
<ul>
<li> include AboutInfoInterface.hh in your plugins header file
<li> derive your plugin from the class AboutInfoInterface
<li> add Q_INTERFACES(AboutInfoInterface) to your plugin class
<li> Implement the signal of this interface
</ul>

\code
void ExamplePlugin::pluginsInitialized(){
  // Simply emit the signal with the text
  emit addAboutInfo("Info Text","Example Plugin License");
}
\endcode
*/

Q_DECLARE_INTERFACE(AboutInfoInterface,"OpenFlipper.AboutInfoInterface/1.0")

