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
*   $Revision: 11127 $                                                       *
*   $LastChangedBy: moebius $                                                *
*   $Date: 2011-03-15 16:18:28 +0100 (Di, 15 Mär 2011) $                     *
*                                                                            *
\*===========================================================================*/

#define HOLEINFO_C

#include "HoleInfoT.hh"

#include <MeshTools/MeshSelectionT.hh>

/// Constructor
template< class MeshT >
HoleInfo< MeshT >::HoleInfo(MeshT * _mesh)
  : mesh_(_mesh), filler_(0)
{
}

/// Destructor
template< class MeshT >
HoleInfo< MeshT >::~HoleInfo()
{
  if (filler_ != 0)
    delete filler_;
}

/// get all holes and store them internally
template< class MeshT >
void HoleInfo< MeshT >::getHoles()
{

  // Property for the active mesh to mark already visited edges
  OpenMesh::EPropHandleT< bool > boundary_search;

  // Add a property to the mesh to store original vertex positions
  mesh_->add_property( boundary_search, "Boundary search" );

  // Initialize Property
  typename MeshT::EdgeIter e_it, e_end=mesh_->edges_end();
  for (e_it=mesh_->edges_begin(); e_it!=e_end; ++e_it) {
    mesh_->property(  boundary_search , *e_it ) = false;
  }

  holes_.clear();

  for (e_it=mesh_->edges_begin(); e_it!=e_end; ++e_it) {

    // Skip already visited edges
    if ( mesh_->property(  boundary_search , *e_it ) )
      continue;

    // Check only boundary edges
    if ( !mesh_->is_boundary(*e_it))
      continue;

    // Get boundary halfedge
    typename MeshT::HalfedgeHandle hh = mesh_->halfedge_handle( *e_it, 0 );
    if ( ! mesh_->is_boundary( hh ) )
      hh = mesh_->opposite_halfedge_handle( hh );


    typename MeshT::Point center(0,0,0);

    Hole currentHole;

    // Collect boundary edges
    typename MeshT::HalfedgeHandle  ch = hh;
    do {
      currentHole.push_back( mesh_->edge_handle(ch) );

      center += mesh_->point( mesh_->from_vertex_handle(ch) );

      mesh_->property(  boundary_search , mesh_->edge_handle(ch) ) = true;

      //check number of outgoing boundary HEH's at Vertex
      int c = 0;
      typename MeshT::VertexHandle vh = mesh_->to_vertex_handle(ch);

      for ( typename MeshT::VertexOHalfedgeIter voh_it(*mesh_,vh); voh_it.is_valid(); ++voh_it)
        if ( mesh_->is_boundary( *voh_it ) )
          c++;

      if ( c >= 2){
        typename MeshT::HalfedgeHandle  op = mesh_->opposite_halfedge_handle( ch );
        typename MeshT::VertexOHalfedgeIter voh_it(*mesh_,op);

        ch = *(++voh_it);

      }else
        ch = mesh_->next_halfedge_handle( ch );

    } while ( ch != hh );


    center /= currentHole.size();

    bool isHole = true;

    int err = 0;

    for (unsigned int i=0; i < currentHole.size(); i++){
      typename MeshT::HalfedgeHandle hh = mesh_->halfedge_handle( currentHole[i], 0 );
      
      if ( ! mesh_->is_boundary( hh ) )
        hh = mesh_->opposite_halfedge_handle( hh );

        typename MeshT::VertexHandle vh = mesh_->from_vertex_handle(hh);

        typename MeshT::Normal n = mesh_->normal( vh );

        typename MeshT::Point p = mesh_->point( vh );

        if ( (p - center).norm() < (p + n - center).norm()  ){
//           isHole = false;
//           break;
          err++;
        }
    }

//   std::cerr << "Errors " << err << " Size " << hole.count << std::endl; 
    if (isHole)
      holes_.push_back(currentHole);
  }

  mesh_->remove_property( boundary_search);

}

/// fill hole with given index 
template< class MeshT >
void HoleInfo< MeshT >::fillHole(int _index, int _stages)
{

  if ( (uint) _index > holes_.size()){
    std::cerr << "Cannot fill hole. Index invalid." << std::endl;
    return;
  }

  if (filler_ == 0)
    filler_ = new HoleFiller< MeshT >(*mesh_);

  filler_->fill_hole( holes_[_index][0], _stages );

  mesh_->garbage_collection();

  MeshSelection::clearEdgeSelection(mesh_);

  mesh_->update_normals();
}

/// fill hole with given edgeHandle on the boundary 
template< class MeshT >
void HoleInfo< MeshT >::fillHole(typename MeshT::EdgeHandle _eh, int _stages)
{
  if (filler_ == 0)
    filler_ = new HoleFiller< MeshT >(*mesh_);

  filler_->fill_hole( _eh, _stages );

  mesh_->garbage_collection();

  MeshSelection::clearEdgeSelection(mesh_);

  mesh_->update_normals();
}

/// fill all holes of the mesh
template< class MeshT >
void HoleInfo< MeshT >::fillAllHoles(int _stages)
{

  if (filler_ == 0)
    filler_ = new HoleFiller< MeshT >(*mesh_);

  filler_->fill_all_holes( _stages );

}

/// select a hole with given index
template< class MeshT >
void HoleInfo< MeshT >::selectHole(int _index)
{

  if ( (uint) _index > holes_.size()){
    std::cerr << "Cannot select hole. Index invalid." << std::endl;
    return;
  }

  for ( uint i = 0 ; i < (holes_[_index]).size() ; ++i ) {
     mesh_->status( (holes_[_index])[i] ).set_selected(true);
  }

}

/// get the holes vector
template< class MeshT >
std::vector< std::vector< typename MeshT::EdgeHandle > >* HoleInfo< MeshT >::holes()
{
  return &holes_;
}


