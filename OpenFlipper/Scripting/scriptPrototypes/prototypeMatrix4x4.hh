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
// C++ Interface: prototypeMatrix4x4
//
// Description: 
//
//
// Author: Jan Möbius <moebius@cs.rwth-aachen.de>, (C) 2007
//

#ifndef PROTOTYPEVMATRIX4X4_HH 
#define PROTOTYPEVMATRIX4X4_HH 

/** \page scripting_matrix_type Matrix data type used for scripting
 *
 *   The matrix type Matrix4x4 is used in the scripting language to handle matrix operations.
 *   The implementation of the type for scripting can be found in prototypeMatrix4x4 .
 *
 *   You can use the following functions:
 *   \code
 *   var matrix = Matrix4x4(1,0,0,1,
 *                          0,1,0,2,
 *                          0,0,1,3,
 *                          0,0,0,1 )  // Create a matrix that contains only a translation (1,2,3).
 *
 *   var vec = Vector(1,2,3)  // Create a vector
 *
 *   var vec2 = matrix.transform_point(vec);  // Transforms a point by the given matrix (translation is included)
 *   var vec3 = matrix.transform_vector(vec); // Transforms a vector by the given matrix (translation is omitted)
 *
 *   \endcode
 *
 */

#include <QtCore/QObject>
#include <QtScript/QtScript>
#include "OpenFlipper/common/Types.hh"
 
class prototypeMatrix4x4 : public QObject , public QScriptable
{

  Q_OBJECT

  public:
    prototypeMatrix4x4(QObject *parent = 0);

  public slots:

    /** \brief Matrix vector product
     *
     * Transforms a vector by the given matrix (translation is omitted)
     */
    QScriptValue transform_vector(QScriptValue _vector);

    /** \brief Matrix vector product
     *
     * Transforms a point by the given matrix (translation is included)
     */
    QScriptValue transform_point(QScriptValue _vector);

  public Q_SLOTS:
    QString toString() const;

};
 
      
#endif // PROTOTYPEVMATRIX4X4_HH
