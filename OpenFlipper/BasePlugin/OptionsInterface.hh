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


#ifndef OPTIONSINTERFACE_HH
#define OPTIONSINTERFACE_HH


#if QT_VERSION >= 0x050000 
  #include <QtWidgets>
#else
  #include <QtGui>
#endif


/** \file OptionsInterface.hh
*
* Interface for adding a widget to OpenFlippers Options dialog .\ref optionsInterfacePage
*/

/** \brief Options Dialog interface
 *
 * \ref optionsInterfacePage "Detailed description"
 * \n
 *
 * This interface can be used to by plugins to add their own widget to OpenFlippers Options Dialog.
 */
class OptionsInterface {

   public :

      /// Destructor
      virtual ~OptionsInterface() {};

      /**   \brief Initialize the Options Widget
       *
       *  Initialize the options widget (create a widget and return a pointer to it ) \n
       * \n
       *   Example : \n
       *  _widget = new QWidget();  \n
       * \n
       * In the widget you can add anything you want.\n
       * \n 
       * use the slot applyOptions() to save the values of options currently entered
       * in your widget.
       * \n
       * @param _widget Pointer to the new Widget
       * @return Return true if a widget has been created
      */
      virtual bool initializeOptionsWidget(QWidget*& _widget) = 0;

       /** \brief Apply changes to options
       *
       * Called, when the user hits the apply/ok button in the options window
       * of Openflipper.\n
       * 
       * use this slot to save the values of options currently entered
       * in your options widget.
       */
      virtual void applyOptions() = 0;

};


/** \page optionsInterfacePage Options Widget Interface
\image html OptionsInterface.png
\n
The OptionsInterface can be used by plugins to add an own widget to OpenFlippers
Options dialog.

This Interface should be used by plugins which will provide their own options widget in OpenFlippers Options Dialog.
For your options to show up in OpenFlippers Options window implement the initializeOptionsWidget() function.

The widget you return with this slot will be integrated into the options window and when
the user hits the apply- or ok-button in the options window the slot applyOptions() is called and
thus enables you to store the changes in your options.

To use the OptionsInterface:
<ul>
<li> include OptionsInterface.hh in your plugins header file
<li> derive your plugin from the class OptionsInterface
<li> add Q_INTERFACES(OptionsInterface) to your plugin class
<li> Implement all functions from this interface
</ul>

\code
bool ExamplePlugin::initializeOptionsWidget(QWidget*& _widget){

  // Global variable .. in header file: QWidget* optionswidget_;
  optionsWidget_ = new QWidget();

  // Setup widget here

  //connect the signals
  connect(optionsWidget_->setA,      SIGNAL( clicked() ), this, SLOT( slotSetA() ) );

  _widget = optionsWidget_;

  return true;
}

void ExamplePlugin::applyOptions(){

  // Get some value from the widget
  int value = optionsWidget_->getValue();

  //
  ...

}

\endcode

*/

Q_DECLARE_INTERFACE(OptionsInterface,"OpenFlipper.OptionsInterface/1.0")

#endif // OPTIONSINTERFACE_HH
