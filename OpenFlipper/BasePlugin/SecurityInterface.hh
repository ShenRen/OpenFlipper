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

#ifndef SECURITYINTERFACE_HH
#define SECURITYINTERFACE_HH

#include <OpenFlipper/common/Types.hh>

#if QT_VERSION >= 0x050000 
  #include <QtWidgets>
#else
  #include <QtGui>
#endif


/** \file SecurityInterface.hh
*
* This interface is used to add copy protection to plugins. \ref securityInterfacePage
* Usually you don't include this file but OpenFlipper/LicenseManager/LicenseManager.hh
*/


/** \brief Interface class for adding copy protection and license management to a plugin.

\n
\ref securityInterfacePage "Detailed description"
\n

This interface is used to add copy protection to plugins.
*/
class SecurityInterface {

  public:

    virtual ~SecurityInterface(){};
    SecurityInterface(){};

  public slots:
    /// Function starting the authentication process
    bool    authenticate();
    
    /// If authenticate returns fails, this string will contain the license info
    QString licenseError();

  private :
    bool authenticated();

};

/** \page securityInterfacePage Security/License Interface
\image html SecurityInterface.png
\n

\section Functionality
The security Interface can be used to add a license management to your plugin. When it is used,
plugins will only start, if a valid license is available. The license is bound to a machine
(Processor,network,...) and expires after a specified date.

\section security_interface_usage Usage
To use this interface do the following steps:

\subsection license_includes Includes
Add the LicenseManager include to your plugin header file.

\code
#include <OpenFlipper/LicenseManager/LicenseManager.hh>
\endcode

\subsection license_licensemanager_integration QObject Replacement
Don't derive your Plugin from QObject anymore but from LicenseManager
which has to be the first one in your class definition.

\code
// Before:
class ExamplePlugin : public QObject, BaseInterface, MenuInterface, ScriptInterface, ToolbarInterface, StatusbarInterface

// After:
class ExamplePlugin : public LicenseManager, BaseInterface, MenuInterface, ScriptInterface, ToolbarInterface, StatusbarInterface
\endcode

\subsection cmake_license_changes Cmake Change
Additionally add the option LICENSEMANAGER to your CMakeLists generator e.g.:

\code
openflipper_plugin (LICENSEMANAGER ) )
\endcode

\subsection salt_file_setup Salt File
Copy the example sat file from OpenFlipper/LicenseManager/salt.hh.example to your plugin directory and rename it
to salt.hh ( e.g. Plugin-Example/salt.hh ). Modify the contents of your salt file to match your plugin:

The contact line will be presented to the user when a license has to be retrieved. The user gets the option
to start the mail program with this destination address and the license request.
\code
#define CONTACTMAIL QString("contact@openflipper.org")
\endcode

Afterwards modify the ADD_SALT_PRE and ADD_SALT_POST macros. Set the characters to some numbers you want.
These salts will basically define a password that will be added to the hashes. It has to be kept !!secret!! as
everybody who knows these salts, can generate licenses for your plugin. So don't give away your salt file. It is
only used when compiling your plugin and the corresponding license manager.

The last thing to change is the ADD_PLUGIN_FILENAME macro. Specify the filename of your plugin here, omitting the ending
as this will be added platform dependent by OpenFlipper.


\section license_genration License generation
When the plugin is now build, an additional LicenseManager is generated which shows
a widget on execution. It will be in your build directory in the subdirectory LicenseManagement.

To generate a license, copy the license request you received into the widget. Remove all text lines.
The manager will split the request into its separate strings. Enter a date, until the license should be valid
and generate it.

The generated license has to be copied into OpenFlippers License subdirectory.

*/

Q_DECLARE_INTERFACE(SecurityInterface,"OpenFlipper.SecurityInterface/1.0")

#endif // SECURITYINTERFACE_HH
