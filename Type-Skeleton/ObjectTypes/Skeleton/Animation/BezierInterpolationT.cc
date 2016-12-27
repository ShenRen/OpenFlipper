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

#include "AnimationHelper.hh"
#include <utility>

//-----------------------------------------------------------------------------------------------------

template<typename Scalar>
std::vector<Scalar> BezierInterpolationT<Scalar>::getValue(Scalar _atX) {
  float bezierParam = AnimationHelper::approximateCubicBezierParameter<Scalar>(_atX, P0_.first, P1_.first, C0_.first, C1_.first);
  return AnimationHelper::evaluateBezier<Scalar>(bezierParam, P0_.second, P1_.second, C0_.second, C1_.second);
}

//-----------------------------------------------------------------------------------------------------

template<typename Scalar>
Scalar BezierInterpolationT<Scalar>::getMaxInput() {
  typedef std::vector<Scalar> Scalars;
  
  Scalars p0_x; p0_x.push_back(P0_.first);
  Scalars p1_x; p1_x.push_back(P1_.first);
  Scalars c0_x; c0_x.push_back(C0_.first);
  Scalars c1_x; c1_x.push_back(C1_.first);
  
  return AnimationHelper::evaluateBezier<Scalar>(1.0f, p0_x, p1_x, c0_x, c1_x)[0];
}

//-----------------------------------------------------------------------------------------------------

template<typename Scalar>
Scalar BezierInterpolationT<Scalar>::getMinInput() {
  typedef std::vector<Scalar> Scalars;
  
  Scalars p0_x; p0_x.push_back(P0_.first);
  Scalars p1_x; p1_x.push_back(P1_.first);
  Scalars c0_x; c0_x.push_back(C0_.first);
  Scalars c1_x; c1_x.push_back(C1_.first);
  
  return AnimationHelper::evaluateBezier<Scalar>(0.0f, p0_x, p1_x, c0_x, c1_x)[0];
}

//-----------------------------------------------------------------------------------------------------

template<typename Scalar>
typename BezierInterpolationT<Scalar>::Point&
BezierInterpolationT<Scalar>::P0(){
  return P0_;
}

//-----------------------------------------------------------------------------------------------------

template<typename Scalar>
typename BezierInterpolationT<Scalar>::Point&
BezierInterpolationT<Scalar>::C0(){
  return C0_;
}

//-----------------------------------------------------------------------------------------------------

template<typename Scalar>
typename BezierInterpolationT<Scalar>::Point&
BezierInterpolationT<Scalar>::C1(){
  return C1_;
}

//-----------------------------------------------------------------------------------------------------

template<typename Scalar>
typename BezierInterpolationT<Scalar>::Point&
BezierInterpolationT<Scalar>::P1(){
  return P1_;
}

//-----------------------------------------------------------------------------------------------------
