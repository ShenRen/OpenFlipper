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

#define ANIMATIONHELPER_CC

#include "AnimationHelper.hh"
#include <cmath>
#include <cassert>

#define APPROXIMATION_EPSILON 1.0e-09
#define VERYSMALL 1.0e-20
#define MAXIMUM_ITERATIONS 1000

template<typename Scalar>
Scalar AnimationHelper::clampTo01Interval(Scalar _value) {
  if (_value < 0.0)
    return 0.0;
  else if (_value > 1.0)
    return 1.0;
  else
    return _value;
}

template<typename Scalar>
Scalar AnimationHelper::abs(Scalar _value) {
  if (_value < 0.0)
    return _value * -1.0;
  else
    return _value;
}

template<typename Scalar>
float AnimationHelper::approximateCubicBezierParameter(Scalar _atX, Scalar _P0X, Scalar _P1X, Scalar _C0X, Scalar _C1X) {
   if (_atX - _P0X < VERYSMALL)
      return 0.0;
   
   if (_P1X - _atX < VERYSMALL) 
      return 1.0;
   
   long iterationStep = 0;
   
   float u = 0.0f; float v = 1.0f;
   
   //iteratively apply subdivision to approach value atX
   while (iterationStep < MAXIMUM_ITERATIONS) {
      
      // de Casteljau Subdivision.
      Scalar a = (_P0X + _C0X)*0.5;
      Scalar b = (_C0X + _C1X)*0.5;
      Scalar c = (_C1X + _P1X)*0.5;
      Scalar d = (a + b)*0.5;
      Scalar e = (b + c)*0.5;
      Scalar f = (d + e)*0.5; //this one is on the curve!
      
      //The curve point is close enough to our wanted atX
      if (abs<Scalar>(f - _atX) < APPROXIMATION_EPSILON)
         return clampTo01Interval<Scalar>((u + v)*0.5f);
      
      //dichotomy
      if (f < _atX) {
         _P0X = f;
         _C0X = e;
         _C1X = c;
         u = (u + v)*0.5f;
      } else {
         _C0X = a; _C1X = d; _P1X = f; v = (u + v)*0.5f;
      }
      
      iterationStep++;
   }
   
   return clampTo01Interval<Scalar>((u + v)*0.5f); 
}

template<typename Scalar>
std::vector<Scalar> AnimationHelper::evaluateBezier(float at, std::vector<Scalar> _P0, std::vector<Scalar> _P1, std::vector<Scalar> _C0, std::vector<Scalar> _C1) {
  unsigned int size = _P0.size();
  assert(size == _P1.size() && size == _C0.size() && size == _C1.size());
  
  float s = at;
  float sinv = (1-s);

  std::vector<Scalar> result;
  
  for (unsigned int i=0;i<size;++i) {
    result.push_back( _P0[i]*sinv*sinv*sinv + 3*_C0[i]*s*sinv*sinv + 3*_C1[i]*s*s*sinv + _P1[i]*s*s*s );
  }
  
  return result;
}
