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
//  Wrapper for Matrix4x4 ( ACG::Matrix4x4d ) - IMPLEMENTATION
//
//=============================================================================


//== INCLUDES =================================================================

#include "matrix4x4Wrapper.hh"

//== IMPLEMENTATION ========================================================== 

//=== Matrix4x4 ( = Vec3d) =======================================================

QScriptValue toScriptValueMatrix4x4(QScriptEngine *engine, const Matrix4x4 &s)
{
  QScriptValue obj = engine->newObject();
  for ( uint i = 0 ; i < 4 ; ++i )
    for ( uint j = 0 ; j < 4 ; ++j )
      obj.setProperty(QString::number(i) + QString::number(j),QScriptValue(engine, s(i,j)) );
  return obj;
}

void fromScriptValueMatrix4x4(const QScriptValue &obj, Matrix4x4 &s)
{
    if (obj.isObject()) {
        for ( uint i = 0 ; i < 4 ; ++i )
            for ( uint j = 0 ; j < 4 ; ++j )
                s(i,j) = obj.property(QString::number(i) + QString::number(j)).toNumber();
        return;
    }

    QString _from = obj.toString();
    if (_from.startsWith ("Matrix4x4 ("))
        _from.remove (0, 11);
    else if (_from.startsWith ("Matrix4x4 : ("))
        _from.remove (0, 14);
    if (_from.endsWith (")"))
        _from.remove (_from.length () - 1, 1);

    QStringList sl = _from.split (',');

    float v[16];
    bool ok = true;

    if (sl.length () == 16)
    {
        for (int i = 0; i < 16 && ok; i++)
            v[i] = sl[i].toFloat (&ok);

        if (ok)
            for (int i = 0; i < 16; i++)
                s(i/4,i%4) = v[i];
        else
        {
            for (int i = 0; i < 16; i++)
                s(i/4,i%4) = 0;
            std::cerr << "String to Matrix4x4 conversion failed!" << std::endl;
        }
    }
}

QScriptValue createMatrix4x4(QScriptContext *context, QScriptEngine *engine)
{     
  Matrix4x4 s;
  
  QScriptValue callee = context->callee();
  
  // If arguments are given, use them for initialization otherwise
  // initialize with 0
  if (context->argumentCount() == 16) {     
    for ( uint i = 0 ; i < 4 ; ++i )
      for ( uint j = 0 ; j < 4 ; ++j )
        s(i,j) = context->argument(i*4+j).toNumber();
  } else {
    for ( uint i = 0 ; i < 4 ; ++i )
      for ( uint j = 0 ; j < 4 ; ++j )
        s(i,j) = 0;
  }
  
  return engine->toScriptValue(s);
}

QScriptValue Matrix4x4ToString(QScriptContext *context, QScriptEngine *engine)
{
  QString result = "Matrix4x4 : ( ";
  
  for ( uint i = 0 ; i < 4 ; ++i )
    for ( uint j = 0 ; j < 4 ; ++j ) {
      if ( i == 3 && j == 3  )
        break;
      result = result + context->thisObject().property(QString::number(i) + QString::number(j)).toString() + " , ";
    }
    
  result = result + context->thisObject().property("33").toString() + " ) ";
  
  return QScriptValue(engine, result);
}

//=============================================================================

