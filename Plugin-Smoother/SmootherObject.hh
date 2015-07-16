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

#include <OpenMesh/Tools/Smoother/JacobiLaplaceSmootherT.hh>
#include <OpenFlipper/common/perObjectData.hh>
#include <OpenFlipper/common/Types.hh>
#include <ObjectTypes/PolyMesh/PolyMesh.hh>
#include <ObjectTypes/TriangleMesh/TriangleMesh.hh>


class SmootherObject : public PerObjectData
{
  public:
    
    SmootherObject();
    ~SmootherObject();
    
    // Copy function
    PerObjectData* copyPerObjectData();
    
    private:
        
        typedef OpenMesh::Smoother::SmootherT< TriMesh >::Component     SmoothingComponent;
        typedef OpenMesh::Smoother::SmootherT< TriMesh >::Continuity    SmoothingContinuity;
    
    public:
    
    // Get/Set methods
    void component(SmoothingComponent _comp)    { component_ = _comp; }
    void continuity(SmoothingContinuity _cont)  { continuity_ = _cont; }
    void distance(float _distance)             { distance_ = _distance; }
    void features(bool _features)               { respectFeatures_ = _features; }
    void iterations(uint _iterations)           { iterations_ = _iterations; }
    void initialized(bool _initialized)         { initialized_ = _initialized; }
    
    SmoothingComponent component()      { return component_; }
    SmoothingContinuity continuity()    { return continuity_; }
    float distance()                   { return distance_; }
    bool features()                     { return respectFeatures_; }
    uint iterations()                   { return iterations_; }
    bool initialized()                  { return initialized_;}
    
  private:
    // Smoothing attributes
    // Component:
    SmoothingComponent component_;
    
    // Continuity
    SmoothingContinuity continuity_;
    
    // Constraints
    float distance_;
    bool respectFeatures_;
    
    // Iterations
    uint iterations_;

    // initialization necessary?
    bool initialized_;
};

