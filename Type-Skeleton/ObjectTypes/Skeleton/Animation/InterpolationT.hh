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

#ifndef SKELETON_INTERPOLATIONT_HH
#define SKELETON_INTERPOLATIONT_HH

#include <utility>
#include <vector>


template<typename Scalar>
class InterpolationT
{
  public:
    typedef std::vector<Scalar>         Target;
    typedef std::pair<Scalar, Target>   Point;
    
    InterpolationT<Scalar>(Point _P0, Point _P1) : P0_(_P0), P1_(_P1) {}
    
    virtual Target getValue(Scalar _atInput) = 0;
    virtual Scalar getMaxInput() = 0;
    virtual Scalar getMinInput() = 0;
    
  protected:
    Point P0_;
    Point P1_;
    
  private:
    
    
};

template<typename Scalar>
class BezierInterpolationT : public InterpolationT<Scalar>
{
  public:
    typedef std::vector<Scalar>         Target;
    typedef std::pair<Scalar, Target>   Point;
    
    BezierInterpolationT<Scalar>(Point _P0, Point _P1)
        : InterpolationT<Scalar>(_P0, _P1) {}
    BezierInterpolationT<Scalar>(Point _P0, Point _P1, Point _C0, Point _C1)
        : InterpolationT<Scalar>(_P0, _P1), C0_(_C0), C1_(_C1) {}
    
    Target getValue(Scalar atX);
    Scalar getMaxInput();
    Scalar getMinInput();
    
  protected:
    using InterpolationT<Scalar>::P0_;
    using InterpolationT<Scalar>::P1_;
    Point C0_;
    Point C1_;
    
    
  public:
    Point& P0();
    Point& C0();
    Point& C1();
    Point& P1();
};

//=============================================================================
#if defined(INCLUDE_TEMPLATES) && !defined(INTERPOLATIONT_CC)
#define INTERPOLATIONT_TEMPLATES
#include "BezierInterpolationT.cc"
#endif
//=============================================================================

#endif //SKELETON_INTERPOLATIONT_HH