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
 *   $Revision: 18604 $                                                       *
 *   $Author: akathrein $                                                      *
 *   $Date: 2014-05-05 16:29:36 +0200 (Mon, 05 May 2014) $                   *
 *                                                                           *
\*===========================================================================*/




//=============================================================================
//
//  CLASS Geometry - IMPLEMENTATION
//
//=============================================================================

#define ACG_BSPLINEBASIS_C

//== INCLUDES =================================================================

#include "BSplineBasis.hh"
#include <assert.h>
#include <cmath>

//----------------------------------------------------------------------------


namespace ACG {


//== IMPLEMENTATION ========================================================== 



template<typename Scalar>
Vec2i 
bsplineSpan(Scalar _t, 
  int _degree,
  const std::vector<Scalar>& _knots)
{
  // binary search

  int lo = _degree;
  int hi = _knots.size() - 1 - _degree;

  Scalar upperBound = _knots[hi];

  if (_t >= upperBound)
    return hi - 1;

  int mid = (lo + hi) >> 1;

  while (_t < _knots[mid] || _t >= _knots[mid + 1])
  {
    if (_t < _knots[mid])
      hi = mid;
    else
      lo = mid;

    mid = (lo + hi) >> 1;
  }

  return Vec2i(mid - _degree, mid);


  
  // linear search:
//   
//   int i = 0;
// 
//   if (_t >= upperBound)
//     i = _knots.size() - _degree - 2;
//   else
//   {
//     while (_t >= _knots[i]) i++;
//     while (_t < _knots[i])  i--;
//   }
// 
//   return Vec2i(i - _degree, i);
}


template<typename Scalar>
void
bsplineBasisFunctions( std::vector<Scalar>& _N,
    const Vec2i& _span,
    Scalar _t,
    const std::vector<Scalar>& _knots)
{
  // inverted triangular scheme
  // "The NURBS Book" : Algorithm A2.2 p70


  // degree
  int p = _span[1] - _span[0];

  int i = _span[1];


  _N[0] = 1.0;

  // alloc temp buffer
  static std::vector<Scalar> left(p+1);
  static std::vector<Scalar> right(p+1);

  if (left.size() < size_t(p+1))
  {
    left.resize(p+1);
    right.resize(p+1);
  }

  // compute
  for (int j = 1; j <= p; ++j)
  {
    left[j] = _t - _knots[i + 1 - j];
    right[j] = _knots[i + j] - _t;

    Scalar saved = 0.0;

    for (int r = 0; r < j; ++r)
    {
      Scalar tmp = _N[r] / (right[r + 1] + left[j - r]);
      _N[r] = saved + right[r + 1] * tmp;
      saved = left[j - r] * tmp;
    }
    _N[j] = saved;
  }
}


template<typename Scalar>
void 
bsplineBasisDerivatives( std::vector<Scalar>& _ders,
    const Vec2i& _span,
    Scalar _t, 
    int _der, 
    const std::vector<Scalar>& _knots,
    std::vector<Scalar>* _functionVals )
{
  // The NURBS Book  p72  algorithm A2.3

  int p = _span[1] - _span[0];
  int p1 = p+1;
  int i = _span[1];


  // alloc temp arrays
  static std::vector< std::vector<Scalar> > ndu(p1);
  static std::vector<Scalar> left(p1);
  static std::vector<Scalar> right(p1);
  static std::vector<Scalar> a(2*p1);

  if (ndu[0].size() < size_t(p1))
  {
    ndu.resize(p1);
    for (int i = 0; i < p1; ++i)
      ndu[i].resize(p1);

    left.resize(p1);
    right.resize(p1);

    a.resize(2*p1);
  }




  ndu[0][0] = 1.0;

  for (int j = 1; j <= p; ++j)
  {
    left[j]= _t - _knots[i + 1 - j];
    right[j]= _knots[i + j] - _t;
    Scalar saved = 0.0;

    for (int r = 0; r < j; ++r)
    {
      // lower triangle
      ndu[j][r] = right[r+1] + left[j-r];
      Scalar tmp = ndu[r][j-1] / ndu[j][r];

      ndu[r][j] = saved + right[r+1] * tmp;
      saved = left[j-r] * tmp;
    }
    ndu[j][j] = saved;
  }

  // load the basis functions
  if (_functionVals)
  {
    for (int j = 0; j <= p; ++j)
      (*_functionVals)[j] = ndu[j][p];
  }

  // compute derivatives



  for (int r = 0; r <= p; ++r)
  {
    int s1 = 0, s2 = p1; // s1, s2: row offsets of linearized 2d array a[2][p+1]
    a[0] = 1.0;

    for (int k = 1; k <= _der; ++k)
    {
      Scalar d = 0.0;
      int rk = r - k, pk = p - k;

      if (r >= k)
      {
        a[s2 + 0] = a[s1 + 0] / ndu[pk+1][rk];
        d = a[s2] * ndu[rk][pk];
      }

      int j1 = (rk >= -1) ? 1 : -rk;
      int j2 = (r - 1 <= pk) ? k-1 : p-r;

      for (int j = j1; j <= j2; ++j)
      {
        a[s2 + j] = (a[s1 + j] - a[s1 + j-1]) / ndu[pk+1][rk+j];
        d += a[s2 + j] * ndu[rk+j][pk];
      }

      if (r <= pk)
      {
        a[s2 + k] = -a[s1 + k-1] / ndu[pk+1][r];
        d += a[s2 + k] * ndu[r][pk];
      }

      if (k == _der)
        _ders[r] = d;

      std::swap(s1, s2); // switch rows
    }
  }

  // correct factors

  int r = p;

  for (int k = 1; k <= _der; ++k)
  {
    Scalar rf = Scalar(r);
    for (int j = 0; j <= p; ++j)
      _ders[j] *= rf;

    r *= (p - k);
  }
}



template<typename Scalar>
Scalar 
bsplineBasisFunction(int _i,
  int _degree, 
  Scalar _t, 
  const std::vector<Scalar>& _knots)
{
  int m = _knots.size() - 1;

  // Mansfield Cox deBoor recursion
  if ((_i == 0 && _t == _knots[0]) || (_i == m - _degree - 1 && _t == _knots[m]))
    return 1.0;

  if (_degree == 0) {
    if (_t >= _knots[_i] && _t < _knots[_i + 1])
      return 1.0;
    else
      return 0.0;
  }

  double Nin1 = basisFunction(_i, _degree - 1, _t, _knots);
  double Nin2 = basisFunction(_i + 1, _degree - 1, _t, _knots);

  double fac1 = 0;
  //   if ((_knotvector(_i+_n) - _knotvector(_i)) > 0.000001 )
  if ((_knots[_i + _degree] - _knots[_i]) != 0)
    fac1 = (_t - _knots[_i]) / (_knots[_i + _degree] - _knots[_i]);

  double fac2 = 0;
  //   if ( (_knotvector(_i+1+_n) - _knotvector(_i+1)) > 0.000001 )
  if ((_knots[_i + 1 + _degree] - _knots[_i + 1]) != 0)
    fac2 = (_knots[_i + 1 + _degree] - _t) / (_knots[_i + 1 + _degree] - _knots[_i + 1]);

  //   std::cout << "Nin1 = " << Nin1 << ", Nin2 = " << Nin2 << ", fac1 = " << fac1 << ", fac2 = " << fac2 << std::endl;

  return (fac1*Nin1 + fac2*Nin2);
}


template<typename Scalar>
Scalar
bsplineBasisDerivative(int _i,
  int _degree,
  Scalar _t, 
  int _der, 
  const std::vector<Scalar>& _knots)
{
  assert(_degree >= 0);
  assert(_i >= 0);


  if (_der == 0)
    return basisFunction(_i, _degree, _t, _knots);

  Scalar Nin1 = derivativeBasisFunction(_i, _degree - 1, _t, _der - 1, _knots);
  Scalar Nin2 = derivativeBasisFunction(_i + 1, _degree - 1, _t, _der - 1, _knots);

  Scalar fac1 = 0;
  if (std::abs(_knots[_i + _degree] - _knots[_i]) > 1e-6)
    fac1 = Scalar(_degree) / (_knots[_i + _degree] - _knots[_i]);

  Scalar fac2 = 0;
  if (std::abs(_knots[_i + _degree + 1] - _knots[_i + 1]) > 1e-6)
    fac2 = Scalar(_degree) / (_knots[_i + _degree + 1] - _knots[_i + 1]);

  return (fac1*Nin1 - fac2*Nin2);
}

//=============================================================================
} // namespace ACG
//=============================================================================
