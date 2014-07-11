#include "PluginAlignMeshes.hh"

#include <Eigen/Dense>

PluginAlignMeshes::PluginAlignMeshes() :
toolBox_(0) {

}

PluginAlignMeshes::~PluginAlignMeshes() {

}

void PluginAlignMeshes::initializePlugin() {

  toolBox_ = new AlignMeshesToolbox();

  emit addToolbox("Align Meshes", toolBox_);

  connect(toolBox_->alignMeshesButton, SIGNAL(pressed()), SLOT(alignMeshes()));
}

void PluginAlignMeshes::pluginsInitialized() {

}

void PluginAlignMeshes::alignMeshes() {

  for (PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS, DATA_TRIANGLE_MESH); o_it
      != PluginFunctions::objectsEnd(); ++o_it) {
    TriMeshObject* tri_object = PluginFunctions::triMeshObject(*o_it);
    if (tri_object != NULL) {

      moveToMean(*tri_object);
      rotate(*tri_object);

      emit updatedObject(tri_object->id(), UPDATE_ALL);
    }
  }
}

void PluginAlignMeshes::moveToMean(TriMeshObject& _object) {
  TriMesh& mesh = *_object.mesh();

  ACG::Vec3d mean(0.0);
  for (TriMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it) {
    mean += mesh.point(*v_it);
  }

  mean /= (double)mesh.n_vertices();

  for (TriMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it) {
    mesh.set_point(*v_it, mesh.point(*v_it) - mean);
  }
}

void PluginAlignMeshes::rotate(TriMeshObject& _object) {
  using namespace Eigen;

  TriMesh& mesh = *_object.mesh();

  Matrix3Xd data = Matrix3Xd::Zero(3, mesh.n_vertices());
  size_t i(0);
  for (TriMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it, ++i) {
    const ACG::Vec3d tmp = mesh.point(*v_it);
    data.col(i) = Vector3d(tmp[0], tmp[1], tmp[2]);
  }

  Matrix3d covar = (data * data.transpose()) / (double)mesh.n_vertices();

  JacobiSVD<Matrix3d> svd(covar, ComputeThinU | ComputeThinV);

  const Matrix3d& u = svd.matrixU();

  Eigen::Vector3d v0 = u.col(0);
  Eigen::Vector3d v1 = u.col(1);
  Eigen::Vector3d v2 = v0.cross(v1);

  v0.normalize();
  v1.normalize();
  v2.normalize();

  Matrix3d trans;
  trans.col(0) = v0;
  trans.col(1) = v1;
  trans.col(2) = v2;

  Matrix3d invTrans = trans.inverse();

  ACG::Matrix4x4d mat;
  mat.identity();
  for (i = 0; i < 3; ++i) {
    for (size_t j = 0; j < 3; ++j) {
      mat(i,j) = invTrans(i,j);
    }
  }

  for (TriMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it, ++i) {
    const ACG::Vec4d tmp(mesh.point(*v_it)[0], mesh.point(*v_it)[1], mesh.point(*v_it)[2], 1.0);
    const ACG::Vec4d res = mat * tmp;

    mesh.set_point(*v_it, ACG::Vec3d(res[0], res[1],res[2]));
  }
}


#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2(pluginalignmeshes, PluginAlignMeshes)
#endif
