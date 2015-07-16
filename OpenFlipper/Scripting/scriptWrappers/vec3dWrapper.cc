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
//  Wrapper for Vector ( Vec3d ) - IMPLEMENTATION
//
//=============================================================================


//== INCLUDES =================================================================

#include "vec3dWrapper.hh"

//== IMPLEMENTATION ========================================================== 

//=== Vector ( = Vec3d) =======================================================

QScriptValue toScriptValueVector(QScriptEngine *engine, const Vector &s)
{
  QScriptValue obj = engine->newObject();
  obj.setProperty("x", QScriptValue(engine, s[0]));
  obj.setProperty("y", QScriptValue(engine, s[1]));
  obj.setProperty("z", QScriptValue(engine, s[2]));
//   obj.setProperty("toString", engine->newFunction(VectorToString) );
  return obj;
}

void fromScriptValueVector(const QScriptValue &obj, Vector &s)
{
    if (obj.isObject()) {
        s[0] = obj.property("x").toNumber();
        s[1] = obj.property("y").toNumber();
        s[2] = obj.property("z").toNumber();
        return;
    }

    QString _from = obj.toString();
    if (_from.startsWith ("Vector ("))
        _from.remove (0, 8);
    else if (_from.startsWith ("Vector : ( "))
        _from.remove (0, 11);
    if (_from.endsWith (")"))
        _from.remove (_from.length () - 1, 1);

    QStringList sl = _from.split (',');

    float v[3];
    bool ok = true;

    if (sl.length () == 3) {
        for (int i = 0; i < 3 && ok; i++)
            v[i] = sl[i].toFloat (&ok);

        if (ok)
            for (int i = 0; i < 3; i++)
                s[i] = v[i];
        else {
            for (int i = 0; i < 3; i++)
                s[i] = 0;
            std::cerr << "String to Vec3D conversion failed!" << std::endl;
        }
    }
}

QScriptValue createVector(QScriptContext *context, QScriptEngine *engine)
{     
  Vector s;
  
  QScriptValue callee = context->callee();
  
  // If arguments are given, use them for initialization otherwise
  // initialize with 0
  if (context->argumentCount() == 3) {     
    s[0] = context->argument(0).toNumber();
    s[1] = context->argument(1).toNumber();
    s[2] = context->argument(2).toNumber(); 
  } else {
    s[0] = 0.0;
    s[1] = 0.0;
    s[2] = 0.0;  
  }
  
  return engine->toScriptValue(s);
}

QScriptValue VectorToString(QScriptContext *context, QScriptEngine *engine)
{
  QString result = "Vector : ( " + 
                   context->thisObject().property("x").toString() + "," +
                   context->thisObject().property("y").toString() + "," +
                   context->thisObject().property("z").toString() + " )";
  return QScriptValue(engine, result);
}

//=============================================================================

