#define POLYLINEPLUGIN_CC

#include "PolyLinePlugin.hh"
#include <queue>

//------------------------------------------------------------------------------


template< class MeshT >
bool cutted(MeshT& _mesh, typename MeshT::HalfedgeHandle _he, const ACG::Vec3d _planeNormal, const ACG::Vec3d _planePoint, ACG::Vec3d* _point = 0) {

  //get intersection point with plane
  typename MeshT::Point p0 = _mesh.point( _mesh.from_vertex_handle(_he) );
  typename MeshT::Point p1 = _mesh.point( _mesh.to_vertex_handle(_he)   );

  typename MeshT::Point u = p1 - p0;
  typename MeshT::Point w = p0 - _planePoint;

  double D =   (_planeNormal | u);
  double N = - (_planeNormal | w);

  // compute intersect parameter
  double sI = N / D;

  if ( _point ) {
    *_point = p0 + sI * u;
  }
  return (sI >= 0.0 && sI <= 1.0 );
}

//------------------------------------------------------------------------------

/**
*
* @param _mesh the mesh
* @param _fh a starting face that is intersected
* @param _planeNormal normal of the cut plane
* @param _planePoint point on the cut plane
* @param _closed TODO find out what it's good for
* @return a list of intersection points
*/
template< class MeshT >
std::vector< ACG::Vec3d > getIntersectionLoop( MeshT* _mesh,
                                                                 uint _fh,
                                                                 ACG::Vec3d _planeNormal,
                                                                 ACG::Vec3d _planePoint,
                                                                 bool& _closed ) {

  OpenMesh::HPropHandleT< bool > cut;
  _mesh->get_property_handle(cut,"Plane Cut Property" );

  typename MeshT::FaceHandle fh ( _fh );

  typename MeshT::FaceHandle current_face = typename MeshT::FaceHandle(_fh);

  bool stop = false;
  bool nothingFound = true;
  int  expansionLevel = 0;
  bool flip_dir = false;
  _closed = true;

  std::vector< ACG::Vec3d > linePoints;

  std::vector< typename MeshT::FaceHandle > startCandidates;
  std::vector< typename MeshT::FaceHandle > expandable;
  expandable.push_back( fh );

  while (!stop) {
    stop = true;

    // First check the face we are in
    for ( typename MeshT::FaceHalfedgeIter fhe_it( *_mesh, current_face ); fhe_it.is_valid(); ++fhe_it){
      if ( _mesh->property(cut,*fhe_it) )
        continue;

      typename MeshT::Point p0 = _mesh->point( _mesh->from_vertex_handle(*fhe_it) );
      typename MeshT::Point p1 = _mesh->point( _mesh->to_vertex_handle(*fhe_it)   );

      typename MeshT::Point u = p1 - p0;
      typename MeshT::Point w = p0 - _planePoint;

      double D =   (_planeNormal | u);
      double N = - (_planeNormal | w);

      // compute intersect param
      double sI = N / D;
      if (sI < 0.0 || sI > 1.0 ) // intersection on ray, but not within line segment
        continue;

      nothingFound = false;

      stop = false;
      _mesh->property(cut,*fhe_it) = true;
      _mesh->property(cut,_mesh->opposite_halfedge_handle(*fhe_it)) = true;
      current_face = _mesh->face_handle(_mesh->opposite_halfedge_handle(*fhe_it));

      if (!current_face.is_valid())
        stop = true;

      typename MeshT::Point cutPoint = p0 + sI * u;

      // add new point
      if ( !flip_dir )
        linePoints.push_back(cutPoint);
      else {
        linePoints.insert( linePoints.begin() , cutPoint );
        _closed = false;
      }

      break;
    }

    if ( stop ){
      if ( nothingFound ){
        if ( startCandidates.empty() ){

          if (expansionLevel > 3 )
            std::cerr << "Expanded" << expansionLevel << "rings but still nothing found!" << std::endl;
          else{

            //add the "expansionLevel"-ring of the start-face to the start candidates
            for (uint i=0; i < expandable.size(); i++)
              for( typename MeshT::FaceFaceIter ff_it(*_mesh, expandable[i]); ff_it.is_valid(); ++ff_it )
                startCandidates.push_back( *ff_it );

            expandable.clear();
            expansionLevel++;
          }
        }

        if ( !startCandidates.empty() ){
          fh = startCandidates.back();
          expandable.push_back( fh );
          startCandidates.pop_back();
          stop = false;
        }

      }else if (! flip_dir ){
        flip_dir = true;
        stop = false;
      }

      current_face = fh;
    }
  }

  return linePoints;

}

//------------------------------------------------------------------------------

/**
*
* @param _mesh the mesh
* @param _fh a starting face that is intersected
* @param _planeNormal normal of the cut plane
* @param _planePoint point on the cut plane
* @param _closed TODO find out what it's good for
* @return a list of intersection points
*/
template< class MeshT >
std::vector< ACG::Vec3d > PolyLinePlugin::getIntersectionPoints( MeshT* _mesh,
                                                                 uint _fh,
                                                                 ACG::Vec3d _planeNormal,
                                                                 ACG::Vec3d _planePoint,
                                                                 bool& _closed ) {
    OpenMesh::HPropHandleT< bool > cut;
    _mesh->add_property(cut,"Plane Cut Property" );

    typename MeshT::HalfedgeIter e_it, e_end = _mesh->halfedges_end();
    for( e_it = _mesh->halfedges_begin(); e_it != e_end; ++e_it )
        _mesh->property( cut, *e_it ) = false;

    std::vector< ACG::Vec3d > linePoints = getIntersectionLoop(_mesh,_fh,_planeNormal,_planePoint,_closed);
    
    _mesh->remove_property( cut );
    
    return linePoints;
}

//------------------------------------------------------------------------------

/** \brief get an edge of the mesh that is cut by the plane
*
* @param _mesh the mesh
* @param _planeNormal normal of the cut plane
* @param _planePoint point on the cut plane
* @return the edge that is intersected and closest to _planePoint
*/
template< class MeshT >
typename MeshT::EdgeHandle
PolyLinePlugin::getCuttedEdge(MeshT& _mesh, ACG::Vec3d& _planeNormal, ACG::Vec3d& _planePoint) {
    
    typename MeshT::EdgeIter e_it;
    typename MeshT::EdgeIter e_end = _mesh.edges_end();
    
    typename MeshT::Scalar minDistance = FLT_MAX;
    typename MeshT::EdgeHandle minEdge(-1);
    
    for (e_it = _mesh.edges_begin(); e_it != e_end; ++e_it){
        
        typename MeshT::HalfedgeHandle hh = _mesh.halfedge_handle(*e_it, 0);
        
        //get intersection point with plane
        typename MeshT::Point p0 = _mesh.point( _mesh.from_vertex_handle(hh) );
        typename MeshT::Point p1 = _mesh.point( _mesh.to_vertex_handle(hh)   );
        
        typename MeshT::Point u = p1 - p0;
        typename MeshT::Point w = p0 - _planePoint;
        
        double D =   (_planeNormal | u);
        double N = - (_planeNormal | w);
        
        // compute intersect param
        double sI = N / D;
        
        if (sI >= 0.0 && sI <= 1.0 ){
            
            typename MeshT::Point cutPoint = p0 + sI * u;
            
            typename MeshT::Scalar dist = (cutPoint - _planePoint).sqrnorm();
            
            if ( dist < minDistance ){
                
                minDistance = dist;
                minEdge = *e_it;
            }
        }
    }
    
    return minEdge;
}




/**
*
* @param _mesh the mesh
* @param _planeNormal normal of the cut plane
* @param _planePoint point on the cut plane
* @return a list of intersection points for each connected intersection ring
*/
template< class MeshT >
std::vector< std::vector<ACG::Vec3d> > PolyLinePlugin::getMultipleIntersectionPoints( MeshT* _mesh,
                                                                                      ACG::Vec3d _planeNormal,
                                                                                      ACG::Vec3d _planePoint) {

  std::vector< std::vector<ACG::Vec3d> > lines;

  OpenMesh::HPropHandleT< bool > cut;
  _mesh->add_property(cut,"Plane Cut Property" );

  std::queue< typename MeshT::EdgeHandle > queue;

  // Mark all edges as not cut, and remember cutted edges as starting points if we get multiple unconnected cuts
  for( typename MeshT::EdgeIter e_it = _mesh->edges_begin(); e_it != _mesh->edges_end(); ++e_it ) {

    // Remember cutted edge
    if ( cutted(*_mesh, _mesh->halfedge_handle(*e_it, 0),_planeNormal, _planePoint, 0) ) {
      queue.push(*e_it);
    }

    //Initialize halfedge Property
    _mesh->property( cut, _mesh->halfedge_handle(*e_it, 0) ) = false;
    _mesh->property( cut, _mesh->halfedge_handle(*e_it, 1) ) = false;

  }


  // Used to catch all connected components
  while( !queue.empty() ) {

    // Get next element from queue
    typename MeshT::HalfedgeHandle hh = _mesh->halfedge_handle( queue.front() , 0);
    queue.pop();

    // Already visited so skip this one
    if ( _mesh->property(cut,hh) )
      continue;

    // Get the adjacent face
    typename MeshT::FaceHandle fh = _mesh->face_handle(hh);

    // If we are at a boundary, get next
    if ( !fh.is_valid() ) {
      fh = _mesh->face_handle(_mesh->opposite_halfedge_handle(hh));
    }

    // No face anywhere at this edge? This should not happen, so we just skip the edge
    if ( !fh.is_valid() )
      continue;


    bool closed = false;

    // Compute the polyline from current face
    lines.push_back(getIntersectionLoop(_mesh,fh.idx(),_planeNormal,_planePoint,closed) );

  }

  // Cleanup
  _mesh->remove_property( cut );

  return lines;

}



