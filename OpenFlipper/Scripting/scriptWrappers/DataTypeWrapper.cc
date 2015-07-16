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

#include "DataTypeWrapper.hh"

//== IMPLEMENTATION ==========================================================

QScriptValue toScriptValueDataType(QScriptEngine *engine, const DataType &s)
{
  QScriptValue obj = engine->newObject();
  obj.setProperty("type", QScriptValue(engine, s.value() ));
  return obj;
}

void fromScriptValueDataType(const QScriptValue &obj, DataType &s)
{
  s = obj.property("type").toNumber();
}

QScriptValue createDataType(QScriptContext *context, QScriptEngine *engine)
{
  DataType s;
  
  QScriptValue callee = context->callee();
  
  // If arguments are given, use them for initialization otherwise
  // initialize with 0
  if (context->argumentCount() == 1) {     
    QVariant argument = context->argument(0).toVariant();
    bool ok;
    
    // Try if we can convert a given uint here
    s = argument.toUInt(&ok);
    
    // If not, we get a String and should therefore try to resolve it via the Typename
    if (! ok )
      s = typeId(argument.toString());

  } else {
    // Basic construction without an Argument! Set to Unknown
    s = DATA_UNKNOWN;
  }

  return engine->toScriptValue( s );
}

QScriptValue DataTypeToString(QScriptContext *context, QScriptEngine *engine)
{
  DataType s = context->thisObject().property("type").toNumber();
  
  return QScriptValue(engine, typeName(s) );
}

//=============================================================================

