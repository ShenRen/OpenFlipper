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


#include <OpenFlipper/BasePlugin/RPCWrappers.hh>
#include <iostream>
#include "RPCWrappersHelper.hh"

#include <QApplication>

namespace RPC {

/** Internal pointer to the script engine. Don't use it directly!
 *
 */
static QScriptEngine* engine_;

QScriptValue callFunction( QString _plugin, QString _functionName , std::vector< QScriptValue > _parameters ) {
  RPCHelper h;

  Qt::ConnectionType connection = Qt::DirectConnection;
  if (h.thread() != QThread::currentThread())
  {
    h.moveToThread(QApplication::instance()->thread());
    connection = Qt::QueuedConnection;
  }

  QScriptValue retVal;
  if (!QMetaType::type("std::vector<QScriptValue>"))
    qRegisterMetaType< std::vector< QScriptValue > >("ScriptParameters");

  //call _functionName in main thread. blocks, if our function runs in another thread
  QMetaObject::invokeMethod(&h, "callFunction", connection ,
      Q_RETURN_ARG(QScriptValue, retVal),
      Q_ARG(QScriptEngine*, engine_),
      Q_ARG(QString, _plugin),
      Q_ARG(QString, _functionName),
      Q_ARG(std::vector< QScriptValue >, _parameters));

  return retVal;

}

QScriptValue callFunction( QString _plugin, QString _functionName ) {
  RPCHelper h;

  Qt::ConnectionType connection = Qt::DirectConnection;
  if (h.thread() != QThread::currentThread())
  {
    h.moveToThread(QApplication::instance()->thread());
    connection = Qt::QueuedConnection;
  }

  QScriptValue retVal;

  //call _functionName in main thread. blocks, if our function runs in another thread
  QMetaObject::invokeMethod(&h, "callFunction", connection ,
      Q_RETURN_ARG(QScriptValue, retVal),
      Q_ARG(QScriptEngine*, engine_),
      Q_ARG(QString, _plugin),
      Q_ARG(QString, _functionName));

  return retVal;
}


void setScriptEngine( QScriptEngine* _engine ) {
  engine_ = _engine;
}

QScriptEngine* getScriptEngine() {
  return engine_;
}

}

