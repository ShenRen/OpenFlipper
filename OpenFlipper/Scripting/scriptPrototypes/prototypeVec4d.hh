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
// Author: Jan Möbius <moebius@cs.rwth-aachen.de>, (C) 2013
//

/** \page scripting_vector_type_4d Vector4d data type for scripting
 *
 *   The vector data type is corresponding to the Vec4d type in C++. The implementation of the
 *   scripting type is done in prototypeVec3d.
 *
 *   You can use the following functions:
 *   \code
 *      var vec = Vector4(1,2,3,4)
 *      var vec2 = Vector4(10,20,30,4)
 *
 *      vec.multiply(0.5)           // Multiply vec with 0.5
 *      vec.add(vec2)               // Add vec2 to vec
 *      vec.sub(vec2)               // Subtract vec2 from vec
 *
 *      var norm    = vec.norm()    // Calculate the length of the vector
 *      var sqrnorm = vec.sqrnorm() // Calculate the squared length of the vector
 *
 *      var sprod = vec.sprod(vec)  //Compute scalar product with the vector and itself
 *
 *      vec.normalize()             // Normalize vector
 *
 *      vec.zero()                  // Set all components of the vector to zero
 *
 *      print(vec)                  // Output vector to the console
 *
 *   \endcode
 *
 */

#ifndef PROTOTYPEVEC4D_HH
#define PROTOTYPEVEC4D_HH


#include <QtCore/QObject>
#include <QtScript/QtScript>
 
/** \class prototypeVec4d
 *
 * This class wraps the Vec4d to the scripting language
 *
 * \ref scripting_vector_type
 *
 */
class prototypeVec4d : public QObject , public QScriptable
{
     Q_OBJECT
 
 public:
     prototypeVec4d(QObject *parent = 0);

 public slots:
     /** \brief Multiplies the given vector with the scalar
      *
      * @param _scalar Scalar value that gets multiplied
      */
     void multiply(QScriptValue _scalar);

     /** \brief Adds another vector to this vector
      *
      * @param _vector Vector that should be added
      */
     void add(QScriptValue _vector);

     /** \brief Subtracts another vector from this vector
      *
      * @param _vector Vector that should be subtracted
      */
     void sub(QScriptValue _vector);

     /** \brief Resets all components of the vector to zero
      *
      */
     void zero();


     /** \brief Calculate scalar product
      *
      * @param _vector Second vector for scalar product (Can be the vector itself)
      * @return Scalar product value
      */
     QScriptValue sprod(QScriptValue _vector);

     /** \brief Calculate the Euclidean norm of the vector
      *
      * @return Norm of the vector
      */
     QScriptValue norm();

     /** \brief Calculate the squared Euclidean norm of the vector
      *
      * @return Squared norm of the vector
      */
     QScriptValue sqrnorm();


     /** \brief Normalize the vector
      */
     void normalize();

 public Q_SLOTS:
     QString toString() const;

};
 
      
#endif // PROTOTYPEVEC4D_HH

