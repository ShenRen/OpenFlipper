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




//
// C++ Interface: prototypeVec3d
//
// Description: 
//
//
// Author: Jan Möbius <moebius@cs.rwth-aachen.de>, (C) 2007
//

#define PROTOTYPEMATRIX4X4_C 

#include "prototypeMatrix4x4.hh"

prototypeMatrix4x4::prototypeMatrix4x4(QObject *parent ) : 
    QObject(parent) 
{
  
}

QString prototypeMatrix4x4::toString() const { 
  
  QString result = "Matrix4x4 : \n";
  
  for ( uint i = 0 ; i < 4 ; ++i ) {
    for ( uint j = 0 ; j < 4 ; ++j ) {
      result = result + thisObject().property(QString::number(i) + QString::number(j)).toString() + "  ";
    }
    
    // Don't add a newline at the end of the Matrix
    if ( i != 3 )
      result += "\n";
  }
    
  return result;
}

QScriptValue prototypeMatrix4x4::transform_vector(QScriptValue _vector )
{

  // Convert from this objects properties to the matrix representation
  Matrix4x4 matrix;
  for ( uint i = 0 ; i < 4 ; ++i )
    for ( uint j = 0 ; j < 4 ; ++j )
      matrix(i,j) = thisObject().property(QString::number(i) + QString::number(j)).toNumber();

  // Convert the vector from scripting representation to C++
  ACG::Vec3d vector(_vector.property("x").toNumber(),_vector.property("y").toNumber(),_vector.property("z").toNumber());

  // Calculate result vector
  ACG::Vec3d result = matrix.transform_vector(vector);

  // Transform back to scriptable vector
  return QScriptValue( engine()->toScriptValue(result) );

}

QScriptValue prototypeMatrix4x4::transform_point(QScriptValue _vector )
{

  // Convert from this objects properties to the matrix representation
  Matrix4x4 matrix;
  for ( uint i = 0 ; i < 4 ; ++i )
    for ( uint j = 0 ; j < 4 ; ++j )
      matrix(i,j) = thisObject().property(QString::number(i) + QString::number(j)).toNumber();

  // Convert the vector from scripting representation to C++
  ACG::Vec3d vector(_vector.property("x").toNumber(),_vector.property("y").toNumber(),_vector.property("z").toNumber());

  // Calculate result vector
  const ACG::Vec3d result = matrix.transform_point(vector);

  // Transform back to scriptable vector
  return QScriptValue( engine()->toScriptValue(result)  );

}
      
