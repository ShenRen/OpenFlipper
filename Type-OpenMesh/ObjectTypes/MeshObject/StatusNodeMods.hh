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

#ifndef STATUSNODEMODS_HH
#define STATUSNODEMODS_HH

//=============================================================================
//
//  Mesh Attribute bits for the Status Nodes
//
//=============================================================================

/** \brief Additional Node status Bits for Mesh
*
* These status bits define special Attributes on the mesh. The are used to define the handle area
* and modeling areas. 
*
*/
enum StatusBits {
  /// Modifyable area
  AREA      = OpenMesh::Attributes::UNUSED << 1,
  /// Handle Area
  HANDLEAREA = OpenMesh::Attributes::UNUSED << 2
};

//=============================================================================
//
//  Mods for the Status Nodes
//
//=============================================================================

/** Structure used in the Status Nodes to show if an area is marked as a modeling area
 */
template< class MeshT >
struct AreaNodeMod
{
  /// tell status node if the vertex is marked as modeling area
  static inline bool is_area(const MeshT& _m, typename MeshT::VertexHandle _vh)
  { return _m.status(_vh).is_bit_set(AREA); }

  /// tell status node if the face is marked as modeling area
  static inline bool is_area(const MeshT& _m, typename MeshT::FaceHandle _fh)
  {
    for (typename MeshT::ConstFaceVertexIter cfv_it = _m.cfv_iter(_fh); cfv_it.is_valid(); ++cfv_it)
      if (_m.status(*cfv_it).is_bit_set(AREA))
        return true;

    return false;
  }

  /// tell status node if the vertex is marked as modeling area
  static bool is_vertex_selected(const MeshT& _mesh, typename MeshT::VertexHandle _vh)
  {
    if (_mesh.is_isolated(_vh))
      return is_area(_mesh, _vh);
    else
      return false;
  }


  /// default to false
  static bool is_edge_selected(const MeshT& /*_mesh */, typename MeshT::EdgeHandle /* _eh */ )
  {
    return false;
  }


  /// default to false
  static bool is_halfedge_selected(const MeshT& /*_mesh */, typename MeshT::HalfedgeHandle /* _eh */ )
  {
    return false;
  }

  /// tell status node if the face is marked as modeling area
  static bool is_face_selected(const MeshT& _mesh, typename MeshT::FaceHandle _fh)
  {
    return is_area(_mesh, _fh);
  }
};

/** Structure used in the Status Nodes to show if an area is marked as a handle region
 */
template< class MeshT >
    struct HandleNodeMod
{

  /// tell status node if the vertex is marked as handle area
  static inline bool is_handle(const MeshT& _m, typename MeshT::VertexHandle _vh)
  { return _m.status(_vh).is_bit_set(HANDLEAREA); }

  /// tell status node if the face is marked as handle area
  static inline bool is_handle(const MeshT& _m, typename MeshT::FaceHandle _fh)
  {
      bool isArea = true;
      for(typename MeshT::ConstFaceVertexIter cfv_it = _m.cfv_iter(_fh); cfv_it.is_valid(); ++cfv_it) {
          if(!_m.status(*cfv_it).is_bit_set(HANDLEAREA)) {
              isArea = false;
              break;
          }
      }
      return isArea;
  }


  /// tell status node if the vertex is marked as handle area
  static bool is_vertex_selected(const MeshT& _mesh, typename MeshT::VertexHandle _vh)
  {
    if (!is_handle(_mesh, _vh))
      return false;

    for (typename MeshT::CVFIter vf_it(_mesh.cvf_iter(_vh)); vf_it.is_valid(); ++vf_it)
      if (is_handle(_mesh, *vf_it))
        return false;

    return true;
  }

  /// tell status node if the edge is marked as handle area
  static bool is_edge_selected(const MeshT& _mesh, typename MeshT::EdgeHandle _eh)
  {
    typename MeshT::HalfedgeHandle hh;
    typename MeshT::FaceHandle     fh;

    hh = _mesh.halfedge_handle(_eh, 0);
    if (!is_handle(_mesh, _mesh.to_vertex_handle(hh))) return false;

    fh = _mesh.face_handle(hh);
    if (fh.is_valid() && is_handle(_mesh, fh)) return false;

    hh = _mesh.halfedge_handle(_eh, 1);
    if (!is_handle(_mesh, _mesh.to_vertex_handle(hh))) return false;

    fh = _mesh.face_handle(hh);
    if (fh.is_valid() && is_handle(_mesh, fh)) return false;

    return true;
  }


  /// default to false
  static bool is_halfedge_selected(const MeshT& /*_mesh */, typename MeshT::HalfedgeHandle /* _eh */ )
  {
    return false;
  }


  /// tell status node if the face is marked as handle area
  static bool is_face_selected(const MeshT& _mesh, typename MeshT::FaceHandle _fh)
  {
    return is_handle(_mesh, _fh);
  }
};

/** Structure used in the Status Nodes to show if an primitive is marked as a feature
 */
template< class MeshT >
    struct FeatureNodeMod
{

  /// tell status node if the vertex is marked as handle area
  static bool is_vertex_selected(const MeshT& _mesh, typename MeshT::VertexHandle _vh)
  {
    return _mesh.status(_vh).feature();
  }

  /// tell status node if the edge is marked as handle area
  static bool is_edge_selected(const MeshT& _mesh, typename MeshT::EdgeHandle _eh)
  {
    return _mesh.status(_eh).feature();
  }


  /// default to false
  static bool is_halfedge_selected(const MeshT& _mesh, typename MeshT::HalfedgeHandle _heh)
  {
    return _mesh.status(_heh).feature();
  }

  /// tell status node if the face is marked as feature
  static bool is_face_selected(const MeshT& _mesh, typename MeshT::FaceHandle _fh)
  {
    return _mesh.status(_fh).feature();
  }

};

#endif

