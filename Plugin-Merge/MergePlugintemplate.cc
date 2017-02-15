
#define MERGEPLUGIN_C

#include "MergePlugin.hh"
#include <OpenMesh/Core/Utils/Property.hh>
#include <ObjectTypes/PolyMesh/PolyMesh.hh>
#include <ObjectTypes/TriangleMesh/TriangleMesh.hh>

///merges Meshes into the first mesh
template< class MeshT >
void MergePlugin::mergeMeshes(const std::vector< MeshT* >& _meshes)
{
  typename MeshT::VertexHandle vhB;

  mergeMeshes(_meshes, vhB);
}

///merges meshes together and maps the VertexHandle _vhB from _meshes[max] to its new handle in _meshes[0]
template< class MeshT >
void MergePlugin::mergeMeshes(const std::vector< MeshT* >& _meshes, typename MeshT::VertexHandle& _vhB)
{

  for (uint i=0; i < _meshes.size(); i++)
  if ( _meshes[i] == 0) {
    emit log(LOGERR,"Unable to get Meshes.");
    return;
  }

  typename MeshT::VertexHandle tmp;

  for (uint i=1; i < _meshes.size(); i++){

    // COPY VERTICES into the first mesh
    //

    OpenMesh::VPropHandleT< typename MeshT::VertexHandle > vertexID;
    _meshes[i]->add_property(vertexID, "Vertex ID Property" );

    typename MeshT::VertexIter v_it;
    typename MeshT::VertexIter v_end = _meshes[i]->vertices_end();

    //iterate over all vertices of the current mesh
    for (v_it = _meshes[i]->vertices_begin(); v_it != v_end; ++v_it){
      //add vertex to _meshes[0] and set vertexID property
      typename MeshT::VertexHandle vh = _meshes[0]->add_vertex( _meshes[i]->point(*v_it) );
      _meshes[i]->property(vertexID, *v_it) = vh;

      //map vertexHandle _vhB
      if (*v_it == _vhB)
        tmp = vh;
    }

    // COPY FACES into the first mesh
    //
    typename MeshT::FaceIter f_it;
    typename MeshT::FaceIter f_end = _meshes[i]->faces_end();

    //itertate over all faces of meshes[i]
    for (f_it = _meshes[i]->faces_begin(); f_it != f_end; ++f_it){
      //get corresponding VertexHandles
      typename MeshT::FaceVertexIter fv_it;

      std::vector< typename MeshT::VertexHandle > vHandles;

      for (fv_it=_meshes[i]->fv_iter(*f_it); fv_it.is_valid(); ++fv_it)
        vHandles.push_back( _meshes[i]->property(vertexID, *fv_it) );

      //add faces to meshA
      _meshes[0]->add_face(vHandles);
    }

    _meshes[i]->remove_property( vertexID );

  }

  _vhB = tmp;
}

template void MergePlugin::mergeMeshes(const std::vector< PolyMesh* >& _meshes);
template void MergePlugin::mergeMeshes(const std::vector< TriMesh* >& _meshes);
