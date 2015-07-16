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


//=============================================================================
//
//  CLASS BSplineCurveT - IMPLEMENTATION
//  Author:  Ellen Dekkers <dekkers@cs.rwth-aachen.de>
//
//=============================================================================

#define BSPLINECURVE_BSPLINECURVET_C

//== INCLUDES =================================================================

#include <OpenMesh/Core/Geometry/VectorT.hh>

#include <iostream>
#include <fstream>

#include "BSplineCurve.hh"

#include <cfloat>
#include <ACG/Geometry/Algorithms.hh>
#include <ACG/Math/BSplineBasis.hh>

//== NAMESPACES ===============================================================

namespace ACG {

//== IMPLEMENTATION ==========================================================


template <class PointT>
BSplineCurveT<PointT>::
BSplineCurveT( unsigned int _degree )
: degree_(_degree),
  autocompute_knotvector_(true),
  fix_number_control_points_(false),
  ref_count_cpselections_(0),
  ref_count_eselections_(0)
{
}

//-----------------------------------------------------------------------------

template <class PointT>
BSplineCurveT<PointT>::
BSplineCurveT( const BSplineCurveT& _curve )
{
  //copy control points
  control_polygon_ = _curve.control_polygon_;

  //copy knotvector
  knotvector_ = _curve.knotvector_;

  degree_                    = _curve.degree_;
  autocompute_knotvector_    = _curve.autocompute_knotvector_;
  fix_number_control_points_ = _curve.fix_number_control_points_;

  // copy properties
  cpselections_ = _curve.cpselections_;
  eselections_ = _curve.eselections_;

  // copy property reference counter
  ref_count_cpselections_ = _curve.ref_count_cpselections_;
  ref_count_eselections_ = _curve.ref_count_eselections_;

}

//-----------------------------------------------------------------------------

template <class PointT>
template <class PropT>
void
BSplineCurveT<PointT>::
request_prop( unsigned int& _ref_count, PropT& _prop)
{
  if(_ref_count == 0)
  {
    _ref_count = 1;
    // always use vertex size!!!
    _prop.resize(n_control_points());
  }
  else ++_ref_count;
}

//-----------------------------------------------------------------------------

template <class PointT>
template <class PropT>
void
BSplineCurveT<PointT>::
release_prop( unsigned int& _ref_count, PropT& _prop)
{
  if( _ref_count <= 1)
  {
    _ref_count = 0;
    _prop.clear();
  }
  else --_ref_count;
}

//-----------------------------------------------------------------------------

template <class PointT>
void
BSplineCurveT<PointT>::
add_control_point(const Point& _cp)
{
  if (fix_number_control_points_)
    return;

  // add new point
  control_polygon_.push_back(_cp);

  // update knot vector
  if (autocompute_knotvector_)
    knotvector_.createKnots(degree_, control_polygon_.size());

  // add available properties
  if( controlpoint_selections_available())
    cpselections_.push_back( false);

  if( edge_selections_available())
    eselections_.push_back(false);
}

//-----------------------------------------------------------------------------

template <class PointT>
void
BSplineCurveT<PointT>::
insert_control_point(int _idx, const Point& _cp)
{
  if (fix_number_control_points_)
    return;

  assert(_idx < (int)control_polygon_.size());
  control_polygon_.insert(control_polygon_.begin()+_idx, _cp);

  // update knot vector
  if (autocompute_knotvector_)
    knotvector_.createKnots(degree_, control_polygon_.size());
  else{
     // compute knot in between its wo neighbors
     double knot = ( knotvector_(_idx-1) + knotvector_(_idx+1) ) / 2.0;
     knotvector_.insertKnot(_idx, knot);
  }

  // add available properties
  if( controlpoint_selections_available())
    cpselections_.insert(cpselections_.begin()+_idx, false);

  if( edge_selections_available())
    eselections_.insert(eselections_.begin()+_idx, false);
}


//-----------------------------------------------------------------------------

template <class PointT>
void
BSplineCurveT<PointT>::
delete_control_point(int _idx)
{
  if (fix_number_control_points_)
    return;

  assert(_idx < (int)control_polygon_.size());
  control_polygon_.erase(control_polygon_.begin()+_idx);

  // update knot vector
  if (autocompute_knotvector_)
    knotvector_.createKnots(degree_, control_polygon_.size());
  else{
    // delete knot at given index
    knotvector_.deleteKnot(_idx);
  }

  // add available properties
  if( controlpoint_selections_available())
    cpselections_.erase(cpselections_.begin()+_idx);

  if( edge_selections_available())
    eselections_.erase(eselections_.begin()+_idx);
}

//-----------------------------------------------------------------------------

template <class PointT>
void
BSplineCurveT<PointT>::
set_control_point(int _idx, const Point& _cp)
{
  assert(_idx < (int)control_polygon_.size());
  control_polygon_[_idx] = _cp;
}

//-----------------------------------------------------------------------------

template <class PointT>
void
BSplineCurveT<PointT>::
set_control_polygon(std::vector< Point> & _control_polygon)
{
  control_polygon_.clear();

  for (unsigned int i = 0; i < _control_polygon.size(); ++i)
    control_polygon_.push_back(_control_polygon[i]);

  cpselections_.clear();
  if( controlpoint_selections_available())
    cpselections_.resize(control_polygon_.size(), false);

  eselections_.clear();
  if( edge_selections_available())
    eselections_.resize(control_polygon_.size(), false);
}

//-----------------------------------------------------------------------------

template <class PointT>
void
BSplineCurveT<PointT>::
reset_control_polygon()
{
  control_polygon_.clear();

  // reset properties
  cpselections_.clear();
  eselections_.clear();
}

//-----------------------------------------------------------------------------

template <class PointT>
PointT
BSplineCurveT<PointT>::
curvePoint(Scalar _u)
{
  Scalar epsilon = 0.0000001;

  if (_u > upper() && _u < upper()+epsilon)
    _u = upper();

  assert(_u >= lower() && _u <= upper());

  int p = degree();           // spline degree

  Vec2i span = ACG::bsplineSpan(_u, p, knotvector_.getKnotvector());

  // eval non-zero basis functions
  std::vector<Scalar> N(p+1);
  ACG::bsplineBasisFunctions(N, span, _u, knotvector_.getKnotvector());

  Point point = Point(0.0, 0.0, 0.0);

  for (int i = 0; i <= p; ++i)
    point += get_control_point(span[0] + i) * N[i];

  return point;
}

//-----------------------------------------------------------------------------

template <class PointT>
PointT
BSplineCurveT<PointT>::
derivativeCurvePoint(Scalar _u, unsigned int _der)
{
  assert(_u >= lower() && _u <= upper());

  int p = degree();           // spline degree

  Vec2i span = ACG::bsplineSpan(_u, p, knotvector_.getKnotvector());

  // eval non-zero basis functions
  std::vector<Scalar> dNdu(p+1);
  ACG::bsplineBasisDerivatives(dNdu, span, _u, 1, knotvector_.getKnotvector(), 0);

  Point point = Point(0.0, 0.0, 0.0);

  for (int i = 0; i <= p; ++i)
    point += get_control_point(i + span[0]) * dNdu[i];

  return point;
}

//-----------------------------------------------------------------------------

template <class PointT>
typename BSplineCurveT<PointT>::Scalar
BSplineCurveT<PointT>::
basisFunction(int _i, int _n, Scalar _t)
{
  int m = knotvector_.size() - 1;
  
  // Mansfield Cox deBoor recursion
  if ((_i==0 && _t== knotvector_(0)) || (_i==m-_n-1 && _t==knotvector_(m)))
    return 1.0;

  if (_n == 0){
    if (_t >= knotvector_(_i) && _t < knotvector_(_i+1))
      return 1.0;
    else
      return 0.0;
  }

  typename BSplineCurveT<PointT>::Scalar Nin1 = basisFunction(_i,   _n-1, _t);
  typename BSplineCurveT<PointT>::Scalar Nin2 = basisFunction(_i+1, _n-1, _t);

  Scalar fac1 = 0;
  if ((knotvector_(_i+_n)-knotvector_(_i)) !=0 )
    fac1 = (_t - knotvector_(_i)) / (knotvector_(_i+_n) - knotvector_(_i)) ;

  Scalar fac2 = 0;
  if ( (knotvector_(_i+1+_n)-knotvector_(_i+1)) !=0 )
    fac2 = (knotvector_(_i+1+_n) - _t)/ (knotvector_(_i+1+_n) - knotvector_(_i+1));

//   std::cout << "N "   << _i   << " " << _n-1 << " = " << Nip1 << ", fac1 = " << fac1
//             << ", N " << _i+1 << " " << _n-1 << " = " << Nip2 << ", fac2 = " << fac2
//             << ", result = " << (fac1*Nip1 - fac2*Nip2)
//             << std::endl;

  return (fac1*Nin1 + fac2*Nin2);
}

//-----------------------------------------------------------------------------

template <class PointT>
typename BSplineCurveT<PointT>::Scalar
BSplineCurveT<PointT>::
derivativeBasisFunction(int _i, int _n, Scalar _t, int _der)
{
  if (_der == 0)
    return basisFunction(_i, _n, _t);

  typename BSplineCurveT<PointT>::Scalar Nin1 = derivativeBasisFunction(_i,   _n-1, _t, _der-1);
  typename BSplineCurveT<PointT>::Scalar Nin2 = derivativeBasisFunction(_i+1, _n-1, _t, _der-1);
//   std::cout << "der = " << _der << ", i = " << _i << ", n = " << _n << ", t = " << _t << "   ===>   " << Nin1 << " , " << Nin2 << std::endl;
//   std::cout << "Nin1 = " << Nin1 << ", Nin2 = " << Nin2 << std::endl;

  double fac1 = 0;
  if ( (knotvector_(_i+_n)-knotvector_(_i)) !=0 )
    fac1 = double(_n) / (knotvector_(_i+_n)-knotvector_(_i));

  double fac2 = 0;
  if ( (knotvector_(_i+_n+1)-knotvector_(_i+1)) !=0 )
    fac2 = double(_n) / (knotvector_(_i+_n+1)-knotvector_(_i+1));

//   std::cout << "fac1 = " << fac1 << ", fac2 = " << fac2 << std::endl;
  return (fac1*Nin1 - fac2*Nin2);
}

//-----------------------------------------------------------------------------

template <class PointT>
std::vector<PointT>
BSplineCurveT<PointT>::
deBoorAlgorithm( double _u)
{
//   std::cout << "\n Evaluating via deBoor algorithm at " << _u << std::endl;
  assert(_u >= lower() && _u <= upper());

  int n = degree();

  Point point = Point(0.0, 0.0, 0.0);

  ACG::Vec2i span_u = span(_u);

  std::vector<Point> allPoints;

  // control points in current iteration
  std::vector<Point> controlPoints_r;

  // control points in last iteration
  std::vector<Point> controlPoints_r1;
  for (int i = span_u[0]; i <= span_u[1]; ++i)
    controlPoints_r1.push_back(control_polygon_[i]);

  for (int r = 1; r <= n; ++r)
  {
    controlPoints_r.clear();

    for (int i = r; i <= n; ++i)
    {
      // compute alpha _i ^ n-r
      double alpha = (_u - knotvector_(span_u[0]+i)) / (knotvector_(span_u[0]+i + n + 1 - r) - knotvector_(span_u[0]+i));
      Point c = controlPoints_r1[i-r] * (1.0 - alpha) + controlPoints_r1[i-r+1] * alpha;

      // save
      controlPoints_r.push_back(c);
      allPoints.push_back(c);
    }

    controlPoints_r1 = controlPoints_r;
  }

  return allPoints;
}

//-----------------------------------------------------------------------------

template <class PointT>
typename BSplineCurveT<PointT>::Scalar
BSplineCurveT<PointT>::
lower() const
{
  return knotvector_(degree());
}

//-----------------------------------------------------------------------------

template <class PointT>
typename BSplineCurveT<PointT>::Scalar
BSplineCurveT<PointT>::
upper() const
{
  return knotvector_(knotvector_.size() - 1 - degree());
}

//-----------------------------------------------------------------------------

template <class PointT>
ACG::Vec2i
BSplineCurveT<PointT>::
span(double _t)
{
  unsigned int i(0);

  if ( _t >= upper())
    i = n_control_points() - 1;
  else  {
    while (_t >= knotvector_(i)) i++;
    while (_t <  knotvector_(i)) i--;
  }

  return Vec2i(i-degree(), i);
}

//-----------------------------------------------------------------------------

template <class PointT>
ACG::Vec2i
BSplineCurveT<PointT>::
interval(double _t)
{
  Vec2i interval = Vec2i(-1, -1);
  
  unsigned int i(0);
  while (_t >= knotvector_(i)) i++;
  while (_t <  knotvector_(i)) i--;
  
  return Vec2i(i, i+1);
}

//-----------------------------------------------------------------------------

template <class PointT>
void
BSplineCurveT<PointT>::
print() const
{
  std::cerr << "****** BSplineInfo ******\n";

  std::cerr << "#control_points: " << control_polygon_.size() << std::endl;
  for (unsigned int i = 0; i < control_polygon_.size(); ++i)
    std::cerr << get_control_point(i) << std::endl;
}

//-----------------------------------------------------------------------------

template <class PointT>
void
BSplineCurveT<PointT>::
insertKnot(double _u)
{
  // insert a knot at parameter u and recompute controlpoint s.t. curve stays the same

//   std::cout << "Lower: " << lower() << ", upper: " << upper() << std::endl;
  assert(_u >= lower() && _u <= upper());

  ACG::Vec2i span_u = span(_u);

  std::vector<Point> updateControlPoints;

  // keep control points that are not affected by knot insertion
  for (int i = 0; i <= span_u[0]; ++i)
    updateControlPoints.push_back(get_control_point(i));

  // add control points in second column of triangular array
  std::vector<Vec3d> cp = deBoorAlgorithm(_u);
  for (unsigned int i = 0; i < degree_; ++i)
    updateControlPoints.push_back(cp[i]);

  // add remaining control points not affected by knot insertion
  for (unsigned int i = span_u[1]; i < n_control_points(); ++i)
    updateControlPoints.push_back(get_control_point(i));


  // find out where to insert the new knot
  int index = 0;
  for (unsigned int i = 0; i < knotvector_.size(); ++i)
    if (_u > knotvector_(i))
      ++index;

  // update knot vector
  knotvector_.insertKnot(index, _u);

  // update control points
  set_control_polygon(updateControlPoints);
}

//-----------------------------------------------------------------------------

template <class PointT>
void
BSplineCurveT<PointT>::
set_knots(std::vector< Scalar > _knots)
{
  autocompute_knotvector(false);

  // set the knotvector
  knotvector_.setKnotvector(_knots);
}

//-----------------------------------------------------------------------------

template <class PointT>
void
BSplineCurveT<PointT>::
reverse()
{
  // reverse vector of controlpoints
  std::vector<Point> reversed_control_polygon;
  for (int i = n_control_points()-1; i > -1; --i)
    reversed_control_polygon.push_back( get_control_point(i) );

  // compute new knot vector
  int num_knots = n_knots();
  std::vector< Scalar > reversed_knotvector(num_knots) ;

  int m = num_knots-1;
  int p = degree();

  double a = get_knot(0);
  double b = get_knot(num_knots-1);

  for (int i = 0; i <= p; ++i)
    reversed_knotvector[i] = get_knot(i);

  for (int i = m-p-1; i <= m; ++i)
    reversed_knotvector[i] = get_knot(i);

  for (int i = 1; i < m - 2*p; ++i)
    reversed_knotvector[m - p - i] = - get_knot(p+i) + a + b;

  // reset control polygon
  set_control_polygon(reversed_control_polygon);

  // reset knot vector
  set_knots(reversed_knotvector);
}

//-----------------------------------------------------------------------------

//=============================================================================
} // namespace ACG
//=============================================================================
