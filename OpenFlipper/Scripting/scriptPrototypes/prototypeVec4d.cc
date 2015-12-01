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
*   $Revision: 14819 $                                                       *
*   $LastChangedBy: moebius $                                                *
*   $Date: 2012-06-29 13:42:49 +0200 (Fr, 29 Jun 2012) $                     *
*                                                                            *
\*===========================================================================*/




//
// C++ Interface: prototypeVec4d
//
// Description: 
//
//
// Author: Jan Möbius <moebius@cs.rwth-aachen.de>, (C) 2007
//

#define PROTOTYPEVEC4D_C

#include "prototypeVec4d.hh"

prototypeVec4d::prototypeVec4d(QObject *parent ) :
    QObject(parent) 
{
  
}

QString prototypeVec4d::toString() const {
  return "Vector : ( " + 
          thisObject().property("x").toString() + "," +
          thisObject().property("y").toString() + "," +
          thisObject().property("z").toString() + "," +
          thisObject().property("w").toString() + " )";
}
      
void prototypeVec4d::multiply(QScriptValue _scalar) {
  thisObject().setProperty("x", QScriptValue(thisObject().property("x").toNumber() * _scalar.toNumber()));
  thisObject().setProperty("y", QScriptValue(thisObject().property("y").toNumber() * _scalar.toNumber()));
  thisObject().setProperty("z", QScriptValue(thisObject().property("z").toNumber() * _scalar.toNumber()));
  thisObject().setProperty("w", QScriptValue(thisObject().property("w").toNumber() * _scalar.toNumber()));
}

void prototypeVec4d::add(QScriptValue _vector) {
  thisObject().setProperty("x", QScriptValue(_vector.property("x").toNumber() + thisObject().property("x").toNumber()));
  thisObject().setProperty("y", QScriptValue(_vector.property("y").toNumber() + thisObject().property("y").toNumber()));
  thisObject().setProperty("z", QScriptValue(_vector.property("z").toNumber() + thisObject().property("z").toNumber()));
  thisObject().setProperty("w", QScriptValue(_vector.property("w").toNumber() + thisObject().property("w").toNumber()));
}


void prototypeVec4d::sub(QScriptValue _vector) {
  thisObject().setProperty("x", QScriptValue(thisObject().property("x").toNumber() - _vector.property("x").toNumber()));
  thisObject().setProperty("y", QScriptValue(thisObject().property("y").toNumber() - _vector.property("y").toNumber()));
  thisObject().setProperty("z", QScriptValue(thisObject().property("z").toNumber() - _vector.property("z").toNumber()));
  thisObject().setProperty("w", QScriptValue(thisObject().property("w").toNumber() - _vector.property("w").toNumber()));
}

void prototypeVec4d::zero() {
  thisObject().setProperty("x", QScriptValue(0.0));
  thisObject().setProperty("y", QScriptValue(0.0));
  thisObject().setProperty("z", QScriptValue(0.0));
  thisObject().setProperty("w", QScriptValue(0.0));
}

QScriptValue prototypeVec4d::sprod(QScriptValue _vector) {
  return QScriptValue( thisObject().property("x").toNumber() * _vector.property("x").toNumber() +
                       thisObject().property("y").toNumber() * _vector.property("y").toNumber() +
                       thisObject().property("z").toNumber() * _vector.property("z").toNumber() +
                       thisObject().property("w").toNumber() * _vector.property("w").toNumber() );

}

QScriptValue prototypeVec4d::norm() {
  return QScriptValue( sqrt( thisObject().property("x").toNumber() * thisObject().property("x").toNumber() +
                             thisObject().property("y").toNumber() * thisObject().property("y").toNumber() +
                             thisObject().property("z").toNumber() * thisObject().property("z").toNumber() +
                             thisObject().property("w").toNumber() * thisObject().property("w").toNumber() ) );
}

QScriptValue prototypeVec4d::sqrnorm() {
  return QScriptValue( thisObject().property("x").toNumber() * thisObject().property("x").toNumber() +
                       thisObject().property("y").toNumber() * thisObject().property("y").toNumber() +
                       thisObject().property("z").toNumber() * thisObject().property("z").toNumber() +
                       thisObject().property("w").toNumber() * thisObject().property("w").toNumber() );
}

void prototypeVec4d::normalize() {
  double length = sqrt( thisObject().property("x").toNumber() * thisObject().property("x").toNumber() +
                        thisObject().property("y").toNumber() * thisObject().property("y").toNumber() +
                        thisObject().property("z").toNumber() * thisObject().property("z").toNumber() +
                        thisObject().property("w").toNumber() * thisObject().property("w").toNumber() );

  if ( length != 0.0 ) {
    thisObject().setProperty("x", ( thisObject().property("x").toNumber() / length ) );
    thisObject().setProperty("y", ( thisObject().property("y").toNumber() / length ) );
    thisObject().setProperty("z", ( thisObject().property("z").toNumber() / length ) );
    thisObject().setProperty("w", ( thisObject().property("w").toNumber() / length ) );
  }


}

