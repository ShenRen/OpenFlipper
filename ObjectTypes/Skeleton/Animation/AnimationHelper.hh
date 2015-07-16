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

#ifndef SKELETON_ANIMATIONHELPER_HH
#define SKELETON_ANIMATIONHELPER_HH
#include <vector>

class AnimationHelper
{
  public:
        
    /** \brief Approximates the parameter value for a Bezier curve to get a certain x value
    * Does an iterative DeCasteljau search for the correct value.
    *
    *  ^
    *  |     C0----------------------------C1       
    *  |    /                               \                   
    *  |   /                                 \           
    *  |  /                                   \        
    *  | P0                                   P1
    *  |             
    *  +------------------atX------------------------------------>
    *
    * @param _atX The X value that the parameter value should be found for
    * @param _P0X The X value of the first key point
    * @param _P1X The X value of the second key point
    * @param _C0X The X value of the first control point
    * @param _C1X The X value of the second control point
    * @return The parameter value that has to be used to get the specified X value
    */
    template<typename Scalar>
    static float approximateCubicBezierParameter(Scalar _atX, Scalar _P0X, Scalar _P1X, Scalar _C0X, Scalar _C1X);
    
    /**
    * \brief Evaluates the cubic Bezier curve parameterized by P0, P1, C0 and C1 at the parameter value "at"
    */
    template<typename Scalar>
    static std::vector<Scalar> evaluateBezier(float at, std::vector<Scalar> _P0, std::vector<Scalar> _P1, std::vector<Scalar> _C0, std::vector<Scalar> _C1);
    
  private:
    template<typename Scalar>
    static Scalar clampTo01Interval(Scalar _value);
    
    template<typename Scalar>
    static Scalar abs(Scalar _value);
};

//=============================================================================
#if defined(INCLUDE_TEMPLATES) && !defined(ANIMATIONHELPER_CC)
#define ANIMATIONHELPER_TEMPLATES
#include "AnimationHelper.cc"
#endif
//=============================================================================

#endif //SKELETON_ANIMATIONHELPER_HH