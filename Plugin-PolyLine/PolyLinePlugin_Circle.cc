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
//  CLASS PolyLinePlugin - IMPLEMENTATION
//
//=============================================================================

//== INCLUDES =================================================================
#include "PolyLinePlugin.hh"

namespace {

struct Onb {

        ACG::Vec3d x, y, z;

        Onb(PolyLineCircleData* _circleData) {
            x = _circleData->circleMainAxis_;
            z = _circleData->circleSideAxis_;
            y = _circleData->circleNormal_;
        }
        ACG::Vec3d toWorld(ACG::Vec3d v) const {
            return ACG::Vec3d(
                    x[0] * v[0] + y[0] * v[1] + z[0] * v[2],
                    x[1] * v[0] + y[1] * v[1] + z[1] * v[2],
                    x[2] * v[0] + y[2] * v[1] + z[2] * v[2]);
        }
};

}

//-----------------------------------------------------------------------------

bool PolyLinePlugin::createCircle_getPointOnMesh(TriMeshObject* _triMeshObject, ACG::Vec3d _center,
        ACG::Vec3d _pOnPlane, ACG::Vec3d _n, ACG::Vec3d* _pOut) {
    OpenMeshTriangleBSPT<TriMesh>* bsp = _triMeshObject->requestTriangleBsp();
    OpenMeshTriangleBSPT<TriMesh>::RayCollision rayInt = bsp->raycollision(_pOnPlane, _n);

    if (rayInt.empty())
        return false;
    int i = -1;
    double smDist = 10e10;

    for (unsigned int j = 0; j < rayInt.size(); j++) {
        ACG::Vec3d p = _pOnPlane + _n * rayInt[j].second, dir = _center - p;
        const double dist = dir.norm();
        if (dist < smDist) {
            smDist = dist;
            if (_pOut)
                *_pOut = p; //+ norAtInt * 0.002;//that would fix lines below the surface, but what is the correct scale?
            i = j;
        }
    }

    return i != -1;
}

//-----------------------------------------------------------------------------

bool PolyLinePlugin::createCircle_getHitInfo(PolyLineCircleData* _circleData, ACG::Vec3d _hit_Point, ACG::Vec3d* _pOut,
        double* r, ACG::Vec3d* _onPlaneO) {
    ACG::Vec3d n = _circleData->circleNormal_, x0 = _circleData->circleCenter_;
    double t = ((n | x0) - (n | _hit_Point)) / n.sqrnorm();
    ACG::Vec3d onPlane = _hit_Point + t * n, d = onPlane - x0;
    if (r)
        *r = d.norm();
    if (_onPlaneO)
        *_onPlaneO = onPlane;
    TriMeshObject* mesh;
    if (PluginFunctions::getObject(_circleData->circleMeshIndex_, mesh))
        return createCircle_getPointOnMesh(mesh, x0, onPlane, n, _pOut);
    else
        return false;
}

//-----------------------------------------------------------------------------

ACG::Vec3d PolyLinePlugin::createCircle_getHit(PolyLineCircleData* _circleData, ACG::Vec3d _hit_point) {
    ACG::Vec3d pOnMesh, pOnPlane;
    double r;
    if (!createCircle_getHitInfo(_circleData, _hit_point, &pOnMesh, &r, &pOnPlane))
        return pOnPlane; //no point on the mesh was found...
    else
        return pOnMesh;
}

//-----------------------------------------------------------------------------

void PolyLinePlugin::updatePolyEllipse(PolyLineObject* _lineObject, unsigned int _pointCount) {
    PolyLineCircleData* lineData = dynamic_cast<PolyLineCircleData*>(_lineObject->objectData(CIRCLE_DATA));

    TriMeshObject* mesh;
    if(!PluginFunctions::getObject(lineData->circleMeshIndex_, mesh))
        return;//if there is no mesh available do not try to recreate the circle

    const double theta = 2.0 * M_PI / double(_pointCount);
    //   _pointCount += tool_->rb_CloseCircle->isChecked() ? 0 : -1;
    const double r2 = lineData->circleMainRadius_, r1 = lineData->circleSideRadius_;
    const Onb basis(lineData);

    _lineObject->line()->clear();
    _lineObject->line()->set_closed(tool_->rb_CloseCircle->isChecked());

    for (unsigned int i = 0; i <= _pointCount; i++) {
        const double tanTheta_i = tan(theta * i);
        double x = (r1 * r2) / sqrt(r2 * r2 + r1 * r1 * tanTheta_i * tanTheta_i);
        x = ((theta * i) <= M_PI_2 || (theta * i) > (3.0 * M_PI_2)) ? x : -x;
        const double y = tanTheta_i * x;
        ACG::Vec3d pOnMesh;
        const ACG::Vec3d pOnPlane(y, 0, x), //this is in the local coord system!
                         pInWorld = basis.toWorld(pOnPlane) + lineData->circleCenter_;
        if(moveCircle_IsFloating)
            _lineObject->line()->add_point(pInWorld);
        else if(createCircle_getHitInfo(lineData, pInWorld, &pOnMesh))
            _lineObject->line()->add_point(pOnMesh);
    }

    emit updatedObject(_lineObject->id(), UPDATE_GEOMETRY | UPDATE_TOPOLOGY);
}

//-----------------------------------------------------------------------------

