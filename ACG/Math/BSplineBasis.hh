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
 *   $Revision$                                                      *
 *   $Author$                                                      *
 *   $Date$                    *
 *                                                                           *
\*===========================================================================*/


#ifndef ACG_BSPLINEBASIS_HH_
#define ACG_BSPLINEBASIS_HH_

#include <vector>
#include <ACG/Math/VectorT.hh>

namespace ACG {


/** \brief Find the span of a parameter value.
  *
  * @param _t parameter value
  * @param _degree spline degree
  * @param _knots knotvector
*/
template<typename Scalar>
Vec2i
bsplineSpan(Scalar _t,
  int _degree,
  const std::vector<Scalar>& _knots);


/** \brief Evaluate basis functions in a span
  *
  * Uses a fast algorithm to compute all basis functions in a span.
  * The output vector _N must have enough space to hold _span[1] - _span[0] values.
  *
  * @param _N output basis function values
  * @param _span span of the parameter _t
  * @param _t parameter value
  * @param _knots knotvector
*/
template<typename Scalar>
void
bsplineBasisFunctions( std::vector<Scalar>& _N,
  const Vec2i& _span,
  Scalar _t,
  const std::vector<Scalar>& _knots);



/** \brief Compute derivatives of basis functions in a span
  *
  * Uses a fast algorithm to compute all derivatives of basis functions in a span.
  * The output vector _ders must have enough space to hold _span[1] - _span[0] values.
  *
  * @param _ders output derivative values of all basis functions in the span
  * @param _span span of the parameter _t
  * @param _t parameter value
  * @param _der order of derivative (ie. first, second, third order...)
  * @param _knots knotvector
  * @param _functionVals output basis function values, null is accepted (optional)
  */
template<typename Scalar>
void
bsplineBasisDerivatives( std::vector<Scalar>& _ders,
  const Vec2i& _span,
  Scalar _t,
  int _der,
  const std::vector<Scalar>& _knots,
  std::vector<Scalar>* _functionVals);


/** \brief Evaluate a single basis function
  *
  * Uses the recursive definition of the basis function.
  * This is slower than using bsplineBasisFunctions() for all values in a span.
  *
  * @param _i index of basis function
  * @param _degree spline degree
  * @param _t parameter value
  * @param _knots knotvector
  * @return basis function value
*/
template<typename Scalar>
Scalar
bsplineBasisFunction(int _i,
  int _degree,
  Scalar _t,
  const std::vector<Scalar>& _knots);

/** \brief Compute derivative of a single basis function
  *
  * Uses the recursive definition of the basis function.
  * This is slower than using bsplineBasisDerivatives() for all values in a span.
  *
  * @param _i index of basis function
  * @param _degree spline degree
  * @param _t parameter value
  * @param _der order of derivative
  * @param _knots knotvector
  * @return derivative of basis function value
*/
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