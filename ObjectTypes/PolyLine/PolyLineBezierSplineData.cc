/*===========================================================================*\
 *                                                                           *
 *                              OpenFlipper                                  *
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
 *                                                                           *
\*===========================================================================*/

/*===========================================================================*\
 *                                                                           *
 *   $Revision$                                                       *
 *   $Author$                                                      *
 *   $Date$                   *
 *                                                                           *
\*===========================================================================*/

//=============================================================================
//
//  CLASS PolyLineCircleData - IMPLEMENTATION
//
//=============================================================================


//== INCLUDES =================================================================

#include "PolyLineBezierSplineData.hh"

PolyLineBezierSplineData::PolyLineBezierSplineData(unsigned int _meshIndex)
: meshIndex_(_meshIndex)
{
}

void PolyLineBezierSplineData::addInterpolatePoint(ACG::Vec3d _position, ACG::Vec3d _normal)
{
	InterpolatePoint p;
	p.position = _position;
	p.normal   = _normal;

	points_.push_back(p);
}

bool PolyLineBezierSplineData::finishSpline()
{
    if(points_.size() < 2)
        return false;

	handles_.clear();

	for(unsigned int i = 0; i + 1 < points_.size(); i++) {

		const ACG::Vec3d firstPoint = points_[i].position, sndPoint = points_[i + 1].position;
		double r = (firstPoint - sndPoint).norm() / 4.0;
		const ACG::Vec3d dir = sndPoint - firstPoint;
		const ACG::Vec3d ort0 = dir % points_[i].normal, ort1 = dir % points_[i + 1].normal;
		ACG::Vec3d f0 = ort0 % points_[i].normal, f1 = ort1 % points_[i + 1].normal;

		ACG::Vec3d near = firstPoint - f0.normalize() * r,
				       far  = sndPoint   + f1.normalize() * r;

		handles_.push_back(near);
		handles_.push_back(far);

	}

	//handles will be degenerate up to now
	for(unsigned int i = 1; i + 1 < handles_.size(); i += 2) {

		const ACG::Vec3d dir = (handles_[i + 1] - handles_[i]) / 2.0;

		InterpolatePoint& p = getInterpolatePoint(i);
		handles_[i + 1]     = p.position + dir;
		handles_[i]         = p.position - dir;
	}

	return true;
}

PolyLineBezierSplineData::InterpolatePoint& PolyLineBezierSplineData::getInterpolatePoint(unsigned int _handleIndex)
{
	if(!_handleIndex)
		return points_[0];
	else if(_handleIndex == handles_.size())
		return points_.back();
	else return points_[(_handleIndex - 1) / 2 + 1];
}
