/**
 * \file PolyMesh.cc
 * This File contains all required includes for using Poly Meshes
*/




//== INCLUDES =================================================================

#include <ObjectTypes/PolyMesh/PolyMesh.hh>
#include <ACG/Geometry/Algorithms.hh>



PolyMeshObject::PolyMeshObject(const PolyMeshObject& _object) : MeshObject< PolyMesh >(_object) {

}


PolyMeshObject::PolyMeshObject(DataType _typeId) : MeshObject< PolyMesh >(_typeId) {

}


PolyMeshObject::~PolyMeshObject() {

}

BaseObject* PolyMeshObject::copy() {
  PolyMeshObject* object = new PolyMeshObject(*this );
  return object;
}

/// Refine picking on triangle meshes
ACG::Vec3d PolyMeshObject::refinePick(ACG::SceneGraph::PickTarget _pickTarget, const ACG::Vec3d _hitPoint, const ACG::Vec3d _start , const ACG::Vec3d _dir,  const unsigned int _targetIdx  ) {
  if ( _pickTarget == ACG::SceneGraph::PICK_FACE) {
    //don't refine poly faces
    return _hitPoint;
  }

  if ( _pickTarget == ACG::SceneGraph::PICK_EDGE) {
    // get picked edge handle
    PolyMesh::EdgeHandle eh = mesh()->edge_handle(_targetIdx);
    if(eh.is_valid())
    {
      PolyMesh::HalfedgeHandle heh = mesh()->halfedge_handle(eh,0);

      //get vertices of the edge
      PolyMesh::VertexHandle vhbegin = mesh()->to_vertex_handle(heh);
      PolyMesh::VertexHandle vhend = mesh()->from_vertex_handle(heh);
      ACG::Vec3d edgeStart = mesh()->point(vhbegin);
      ACG::Vec3d edgeEnd = mesh()->point(vhend);

      //retrieve the point on the edge that is closest to the backprojected hitpoint
      ACG::Vec3d hitPointNew;
      ACG::Geometry::distPointLineSquared(_hitPoint,edgeStart,edgeEnd,&hitPointNew);


      return hitPointNew;
    }
  }

  if ( _pickTarget == ACG::SceneGraph::PICK_VERTEX) {
    // get picked vertex handle
    PolyMesh::VertexHandle vh = mesh()->vertex_handle(_targetIdx);
    if(vh.is_valid())
    {
      ACG::Vec3d hitpointNew = mesh()->point(vh);

      //just return the vertex position
      return hitpointNew;
    }
  }

  return _hitPoint;
}


//=============================================================================



