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

#ifndef TOOLBARINTERFACE_HH
#define TOOLBARINTERFACE_HH

 #include <OpenFlipper/common/Types.hh>
 
#if QT_VERSION >= 0x050000 
  #include <QtWidgets>
#else
  #include <QtGui>
#endif

 #include <QMenuBar>
 #include <QStringList>
 #include <QToolBar>

/** \file ToolbarInterface.hh
*
* Interface for adding per plugin toolbars to OpenFlippers UI.\ref toolbarInterfacePage
*/


/** \brief Add a toolbox to OpenFlipper
 *
 * \ref toolbarInterfacePage "Detailed description"
 * \n
 *
 * This Interface should be used by plugins which will provide a toolbar.
 * Each Plugin can create own toolbars.
*/
class ToolbarInterface {

   private slots :

   public :

      /// Destructor
      virtual ~ToolbarInterface() {};

    signals :

      /** \brief Adds a Toolbar to the main widget
      *
      * To create your own Toolbar, emit this signal with a pointer to your Toolbar.
      * @param _toolbar Your Toolbar
      */
      virtual void addToolbar(QToolBar* _toolbar) {};


      /** \brief Remove a Toolbar from the main widget
      *
      * @param _toolbar Toolbar to be removed
      */
      virtual void removeToolbar(QToolBar* _toolbar) {};


      /** \brief Get a pointer to a Toolbar of the given name or 0 if it does not exist.
      *
      * @param _name Name of the Toolbar
      * @param _toolbar requested Toolbar or 0
      */
      virtual void getToolBar( QString _name, QToolBar*& _toolbar ) {};


};



/** \page toolbarInterfacePage Toolbar Interface
\image html ToolbarInterface.png
\n
The ToolbarInterface can be used by plugins to add toolbars to OpenFlippers
UI. The toolbars are located above the GL viewer (See image).


To use the ToolbarInterface:
<ul>
<li> include ToolbarInterface in your plugins header file
<li> derive your plugin from the class ToolbarInterface
<li> add Q_INTERFACES(ToolbarInterface) to your plugin class
<li> And add the signals or slots you want to use to your plugin class (You don't need to implement all of them)
</ul>

Usually you should implement the BaseInterface::pluginsInitialized() function from BaseInterface. In this function you can setup
your toolbars.

The following code shows a simple example to create a simple toolbar.
\code
void ExamplePlugin::pluginsInitialized()
{

  // Create your toolbar
  QToolBar* toolbar = new QToolBar(tr("Example Toolbar"));

  // Create an action for the toolbar
  QAction* exampleAction = new QAction(tr("&Toolbar Button"), this);

  // Create an icon which is shown for the action
  exampleAction->setIcon(QIcon(OpenFlipper::Options::iconDirStr()+OpenFlipper::Options::dirSeparator()+"exampleIcon.png"));

  // Add the action to the toolbar
  toolbar->addAction(exampleAction);

  // Integrate the new toolbar into OpenFlipper
  emit addToolbar( toolbar );
}
\endcode

Signals and slots of your toolbar (e.g. from an action inside it) can be directly connected to signals and slots in
your plugin. Therefore the embedding of the toolbar is fully transparent.

*/



Q_DECLARE_INTERFACE(ToolbarInterface,"OpenFlipper.ToolbarInterface/1.1")

#endif // TOOLBARINTERFACE_HH
