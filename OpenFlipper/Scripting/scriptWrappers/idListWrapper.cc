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



//=============================================================================
//
//  Wrapper for IdList ( std::vector< int > ) - IMPLEMENTATION
//
//=============================================================================


//== INCLUDES =================================================================

#include "idListWrapper.hh"

//== IMPLEMENTATION ==========================================================

QScriptValue toScriptValueIdList(QScriptEngine *engine, const IdList &s)
{
  QScriptValue obj = engine->newObject();
  obj.setProperty("size", QScriptValue(engine, (int)s.size() ));
  for (uint i=0; i < s.size(); i++)
    obj.setProperty(QString::number(i), QScriptValue(engine, s[i]));

  return obj;
}

void fromScriptValueIdList(const QScriptValue &obj, IdList &s)
{
  int size = obj.property("size").toNumber();

  for (int i=0; i < size; i++)
    s.push_back( obj.property( QString::number(i) ).toNumber() );
}

QScriptValue createIdList(QScriptContext *context, QScriptEngine *engine)
{
  IdList s;

  QScriptValue callee = context->callee();

  // If arguments are given, use them for initialization otherwise
  // initialize with 0
  for (int i=0; i < context->argumentCount(); i++)
    s.push_back( context->argument(i).toNumber() );

  return engine->toScriptValue(s);
}

QScriptValue idListToString(QScriptContext *context, QScriptEngine *engine)
{
  int size = context->thisObject().property("size").toNumber();

  QString result;

  if (size == 0)
    result = "[]";
  else{
    result = "[" + context->thisObject().property("0").toString();

    for (int i = 1; i < size; i++)
      result += "," + context->thisObject().property( QString::number(i) ).toString();

    result += "]";
  }

  return QScriptValue(engine, result);
}

//=============================================================================

