/*===========================================================================*\
 *                                                                           *
 *                              OpenFlipper                                  *
 *      Copyright (C) 2001-2014 by Computer Graphics Group, RWTH Aachen      *
 *                           www.openflipper.org                             *
 *                                                                           *
 *---------------------------------------------------------------------------*
 *  This file is part of OpenFlipper.                                        *
 *                                                                           *
 *  OpenFlipper is free software: you can redistribute it and/or modify      *
 *  it under the terms of the GNU Lesser General Public License as           *
 *  published by the Free Software Foundation, either version 3 of           *
 *  the License, or (at your option) any later version with the              *
 *  following exceptions:                                                    *
 *                                                                           *
 *  If other files instantiate templates or use macros                       *
 *  or inline functions from this file, or you compile this file and         *
 *  link it with other files to produce an executable, this file does        *
 *  not by itself cause the resulting executable to be covered by the        *
 *  GNU Lesser General Public License. This exception does not however       *
 *  invalidate any other reasons why the executable file might be            *
 *  covered by the GNU Lesser General Public License.                        *
 *                                                                           *
 *  OpenFlipper is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *  GNU Lesser General Public License for more details.                      *
 *                                                                           *
 *  You should have received a copy of the GNU LesserGeneral Public          *
 *  License along with OpenFlipper. If not,                                  *
 *  see <http://www.gnu.org/licenses/>.                                      *
 *                                                                           *
\*===========================================================================*/

/*===========================================================================*\
 *                                                                           *
 *   $Revision: 18130 $                                                      *
 *   $Author: moebius $                                                      *
 *   $Date: 2014-02-05 10:41:16 +0100 (Wed, 05 Feb 2014) $                    *
 *                                                                           *
\*===========================================================================*/


#ifndef ACG_BSPLINEBASIS_HH_
#define ACG_BSPLINEBASIS_HH_

#include <vector>
#include <ACG/Math/VectorT.hh>

namespace ACG {



template<typename Scalar>
Vec2i
bsplineSpan(Scalar _t,
  int _degree,
  const std::vector<Scalar>& _knots);



/// compute derivatives of the basis functions in a given span
template<typename Scalar>
void
bsplineBasisFunctions( std::vector<Scalar>& _N,
  const Vec2i& _span,
  Scalar _t,
  const std::vector<Scalar>& _knots);



/// compute derivatives of the basis functions in a given span
template<typename Scalar>
void
bsplineBasisDerivatives( std::vector<Scalar>& _ders,
  const Vec2i& _span,
  Scalar _t,
  int _der,
  const std::vector<Scalar>& _knots,
  std::vector<Scalar>* _functionVals);


template<typename Scalar>
Scalar
bsplineBasisFunction(int _i,
  int _degree,
  Scalar _t,
  const std::vector<Scalar>& _knots);


template<typename Scalar>
Scalar
bsplineBasisDerivative(int _i,
  int _degree,
  Scalar _t,
  int _der,
  const std::vector<Scalar>& _knots);



} /* namespace ACG */

#if defined(INCLUDE_TEMPLATES) && !defined(ACG_BSPLINEBASIS_C)
#define ACG_BSPLINEBASIS_TEMPLATES
#include "BSplineBasis.cc"
#endif

#endif /* ACG_BSPLINEBASIS_HH_ */