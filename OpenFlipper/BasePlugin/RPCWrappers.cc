/*===========================================================================*\
*                                                                            *
*                              OpenFlipper                                   *
*      Copyright (C) 2001-2014 by Computer Graphics Group, RWTH Aachen       *
*                           www.openflipper.org                              *
*                                                                            *
*--------------------------------------------------------------------------- *
*  This file is part of OpenFlipper.                                         *
*                                                                            *
*  OpenFlipper is free software: you can redistribute it and/or modify       *
*  it under the terms of the GNU Lesser General Public License as            *
*  published by the Free Software Foundation, either version 3 of            *
*  the License, or (at your option) any later version with the               *
*  following exceptions:                                                     *
*                                                                            *
*  If other files instantiate templates or use macros                        *
*  or inline functions from this file, or you compile this file and          *
*  link it with other files to produce an executable, this file does         *
*  not by itself cause the resulting executable to be covered by the         *
*  GNU Lesser General Public License. This exception does not however        *
*  invalidate any other reasons why the executable file might be             *
*  covered by the GNU Lesser General Public License.                         *
*                                                                            *
*  OpenFlipper is distributed in the hope that it will be useful,            *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of            *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
*  GNU Lesser General Public License for more details.                       *
*                                                                            *
*  You should have received a copy of the GNU LesserGeneral Public           *
*  License along with OpenFlipper. If not,                                   *
*  see <http://www.gnu.org/licenses/>.                                       *
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

