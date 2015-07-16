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

#include "RPCWrappersHelper.hh"

namespace RPC
{

RPCHelper::RPCHelper()
{
}

RPCHelper::~RPCHelper()
{
}

QScriptValue RPCHelper::callFunction(QScriptEngine* _engine, const QString& _plugin, const QString& _functionName)
{
  QString command = _plugin+"."+_functionName+ "()";

  QScriptValue returnValue = _engine->evaluate(command);
  if ( returnValue.isError() ) {
    QString error = returnValue.toString();
    std::cerr << "Error : " << error.toStdString() << std::endl;
  }
  return returnValue;
}


QScriptValue RPCHelper::callFunction(QScriptEngine* _engine, const QString& _plugin, const QString& _functionName , const std::vector< QScriptValue >& _parameters)
{
  QString command = _plugin+"."+_functionName+ "(";
  // Make the parameters available in the scripting environment
  for ( uint i = 0 ; i < _parameters.size(); ++i ) {
    _engine->globalObject().setProperty("ParameterData" + QString::number(i) , _parameters[i] );
    command += "ParameterData" + QString::number(i);
    if ( (i + 1) < _parameters.size() )
      command+=",";
  }
  command += ")";

  QScriptValue returnValue = _engine->evaluate(command);
  if ( returnValue.isError() ) {
    QString error = returnValue.toString();
    std::cerr << "Error : " << error.toStdString() << std::endl;
  }
  return returnValue;
}

}
