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

/**
 * \file RPCWrappersHelper.hh
 * private helper classes for the RPCWrappers *
 * Usage is described in \ref RPCInterfacePage
 */

#ifndef RPCWRAPPERSHELPER_HH
#define RPCWRAPPERSHELPER_HH

#include <QtScript>
#include <QObject>
#include <OpenFlipper/common/Types.hh>

namespace RPC
{
  class DLLEXPORT RPCHelper : public QObject
  {
    Q_OBJECT
  public:
    RPCHelper();
    ~RPCHelper();

  public slots:
    /** \brief call a function provided by a plugin
     *
     * @param _plugin Plugin name ( Scripting name )
     * @param _functionName Name of the remote function
     */
    QScriptValue callFunction(QScriptEngine* _engine, const QString& _plugin,const QString& _functionName);

    /** \brief Call a function provided by a plugin getting multiple parameters
     *
     * This function gets the parameters which are converted to a QScriptValue on your own.
     *
     * @param _plugin Plugin name ( Scripting name of the plugin )
     * @param _functionName Name of the remote function
     * @param _parameters vector of scriptvalues containing the functions parameters in the right order
     */
    QScriptValue callFunction(QScriptEngine* _engine, const QString& _plugin,const QString& _functionName , const std::vector< QScriptValue >& _parameters);
  };
}

#endif
