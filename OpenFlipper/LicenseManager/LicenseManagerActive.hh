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

#ifndef LICENSEMANAGER_HH
#define LICENSEMANAGER_HH

#include <OpenFlipper/BasePlugin/SecurityInterface.hh>

/** \file LicenseManager.hh
*
* This interface is used to add copy protection to plugins. \ref securityInterfacePage
*
*/

/* The salt file has to be provided for each plugin. It can be the same
  for all plugins. See example for details on how this file has to be setup
*/
#include "salt.hh"

/** \brief License management base class
 *
 * See \ref securityInterfacePage for Details on how to use it.
 *
 * The class is used by plugins to integrate license management. It will check the license,
 * generate license requests and prevent the plugin from loading if an invalid license or no license
 * is found.
 */
class LicenseManager : public QObject, SecurityInterface  {

Q_OBJECT
Q_INTERFACES(SecurityInterface)

  public:

    virtual ~LicenseManager();
    LicenseManager();

  public :

    /** This function is overloaded and will not allow to unblock signals
        if the plugin is not authenticated.
    */
    void blockSignals( bool _state);
    
    /** Return if the plugin has successfully passed the authentication.
    */
    bool authenticated();

  public slots:
    /** Call this function for plugin authentication. If it returns true,
        the authentication has been successful. Otherwise the core will
        stop loading the plugin. Additionally the plugin will make itself
        unusable by not allowing any signal slot connections.

    */
    bool authenticate();
    
    /** if authenticate returns false, this string will contain the license information required
     *  to generate a license request and the error that caused the failure.
    */
    QString licenseError();

  private:


    bool timestampOk();

    /** This is used to get the plugins Name from derived classes
        The plugin name is the usual name of the plugin
    */
    virtual QString name() = 0;

    /** This function is special to the LicenseManager. It is used to
        find the plugin when checking its hash value.
    */
    virtual QString pluginFileName();

    /// This flag is true if authentication was successful
    bool authenticated_;
    
    /// License information string
    QString authstring_;


  protected :
    /** This function is overloaded in the security interface. If the
        plugin is not authenticated, all connections will be automatically
        removed again.
    */
    void connectNotify ( const char * signal );

};

#endif // LICENSEMANAGER_HH
