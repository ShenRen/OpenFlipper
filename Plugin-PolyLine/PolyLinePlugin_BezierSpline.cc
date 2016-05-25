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
 *   $Revision: 17392 $                                                       *
 *   $Author: moebius $                                                      *
 *   $Date: 2013-08-26 15:03:53 +0200 (Mon, 26 Aug 2013) $                   *
 *                                                                           *
\*===========================================================================*/

//=============================================================================
//
//  CLASS PolyLinePlugin - IMPLEMENTATION
//
//=============================================================================


//== INCLUDES =================================================================

#include "PolyLinePlugin.hh"

ACG::Vec3d
PolyLinePlugin::
getPointOnMesh(PolyLineBezierSplineData* _SplineData, ACG::Vec3d _point, ACG::Vec3d* _nor)
{
	TriMeshObject* mesh;
	if(!PluginFunctions::getObject(_SplineData->meshIndex_, mesh))
		return _point;
	OpenMeshTriangleBSPT<TriMesh>* bsp = mesh->requestTriangleBsp();
	OpenMeshTriangleBSPT<TriMesh>::NearestNeighbor neigh = bsp->nearest(_point);
	ACG::Vec3d nor = mesh->mesh()->normal(neigh.handle);
	if(_nor)
		*_nor = nor;
    OpenMeshTriangleBSPT<TriMesh>::RayCollision rayInt;
	rayInt = bsp->raycollision(_point, nor);
	if(rayInt.size())
		return _point + nor * rayInt[0].second;
	return _point + nor.normalize() * neigh.dist;
}

void
PolyLinePlugin::
updatePolyBezierHandles(PolyLineObject* _lineObject, ACG::SceneGraph::LineNode* _line)
{
	PolyLineBezierSplineData* splineData = dynamic_cast<PolyLineBezierSplineData*>(_lineObject->objectData(BEZSPLINE_DATA));
	if(!splineData)
		return;
	_line->clear();
	for(unsigned int i = 0; i < splineData->handles_.size(); i++) {
		ACG::SceneGraph::GlutPrimitiveNode* node = 0;
		if(_lineObject->getAdditionalNode(node, name(), "handle", i))
			node->set_position(splineData->handles_[i]);
		const PolyLineBezierSplineData::InterpolatePoint& control = splineData->getInterpolatePoint(i);
		_line->add_line(control.position, splineData->handles_[i]);
		_line->add_color(ACG::Vec4f(1,0,0,1));
	}
	for(unsigned int i = 0; i < splineData->points_.size(); i++) {
		ACG::SceneGraph::GlutPrimitiveNode* node = 0;
		if(_lineObject->getAdditionalNode(node, name(), "control", i))
			node->set_position(splineData->points_[i].position);
	}
}

void
PolyLinePlugin::
updatePolyBezierSpline(PolyLineObject* _lineObject, unsigned int _pointCount)
{
	PolyLineBezierSplineData* splineData = dynamic_cast<PolyLineBezierSplineData*>(_lineObject->objectData(BEZSPLINE_DATA));
	TriMeshObject* mesh;
	if(!splineData || !PluginFunctions::getObject(splineData->meshIndex_, mesh)) {
		return;//no mesh -> do nothing
	}
	_lineObject->line()->clear();
	unsigned int segCount = (splineData->points_.size() + splineData->handles_.size() - 1) / 3;
	for(unsigned int s = 0; s < segCount; s++) {
		const ACG::Vec3d a = splineData->points_[s].position, d = splineData->points_[s + 1].position,
						 b = splineData->handles_[s * 2], c = splineData->handles_[s * 2 + 1];
		//the last segment will get one more point to close the spline
		unsigned int n = (s != segCount - 1) ? _pointCount : (_pointCount + 1);
		for(unsigned int i = 0; i < n; i++) {
			float alpha = float(i) / float(_pointCount);
			const ACG::Vec3d e = a + (b - a) * alpha, f = c + (d - c) * alpha;
			ACG::Vec3d g = e + (f - e) * alpha;
			g = getPointOnMesh(splineData, g);
			_lineObject->line()->add_point(g);
		}
	}
	emit updatedObject(_lineObject->id(), UPDATE_GEOMETRY | UPDATE_TOPOLOGY);
}
