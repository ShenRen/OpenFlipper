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

#ifndef ISOTROPICREMESHER_HH
#define ISOTROPICREMESHER_HH
#include "ProgressEmitter.hh"


template< class MeshT >
class IsotropicRemesher{

public:

  IsotropicRemesher(ProgressEmitter* _prgEmt = NULL) : prgEmt_(_prgEmt)  {};
  ~IsotropicRemesher() {};

  void remesh( MeshT& _mesh, const double _targetEdgeLength );

private:

  void splitLongEdges( MeshT& _mesh, const double _maxEdgeLength );

  void collapseShortEdges( MeshT& _mesh, const double _minEdgeLength, const double _maxEdgeLength );

  void equalizeValences( MeshT& _mesh );

  inline
  int targetValence( MeshT& _mesh, const typename MeshT::VertexHandle& _vh );

  void tangentialRelaxation( MeshT& _mesh );

  template <class SpatialSearchT>
  typename MeshT::Point
  findNearestPoint(const MeshT&                   _mesh,
                   const typename MeshT::Point&   _point,
                   typename MeshT::FaceHandle&    _fh,
                   SpatialSearchT*                _ssearch,
                   double*                        _dbest);

  OpenMeshTriangleBSPT< MeshT >* getTriangleBSP(MeshT& _mesh);

  template< class SpatialSearchT >
  void projectToSurface( MeshT& _mesh, MeshT& _original, SpatialSearchT* _ssearch );

  inline
  bool isBoundary( MeshT& _mesh, const typename MeshT::VertexHandle& _vh);
  
  inline
  bool isFeature( MeshT& _mesh, const typename MeshT::VertexHandle& _vh);
  
  private:
    ProgressEmitter* prgEmt_;
};

//=============================================================================
#if defined(INCLUDE_TEMPLATES) && !defined(ISOTROPICREMESHER_C)
#define ISOTROPICREMESHER_TEMPLATES
#include "IsotropicRemesherT.cc"
#endif
//=============================================================================
#endif // ISOTROPICREMESHER_HH defined
//=============================================================================
