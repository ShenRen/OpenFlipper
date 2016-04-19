#pragma once

#include <ACG/GL/globjects.hh>
#include <ACG/GL/VertexDeclaration.hh>
#include <ACG/GL/GLPrimitives.hh>

#include "GCode.hh"

namespace Ultimaker{

class GCodeNode_renderer
{
public:
  enum DrawMode { Color, Heat, Speed, Type };

  GCodeNode_renderer();
  virtual ~GCodeNode_renderer();

  // Render the gcode at the given time
  void render(ACG::GLState& _state, const ACG::GLMatrixd& _proj, const ACG::GLMatrixd& _view, DrawMode _mode, double time_ = std::numeric_limits<double>::max(), ACG::Vec4f color = ACG::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));

  // Set the gcode to render, updates max_time accordingly
  void set_gcode(const GCode::Shared& _gcode);

protected:
  // Update the vbo for the current gcode
  void update_vbo();

  void compute_segment_axes(const ACG::Vec3d& _dir, ACG::Vec3f& _dx, ACG::Vec3f& _dy, ACG::Vec3f& _dz) const;

  void compute_instance_matrix(const ACG::Vec3f& _pos, const ACG::Vec3f& _dx, const ACG::Vec3f& _dy, const ACG::Vec3f& _dz, const ACG::Vec3f& _scale, ACG::GLMatrixf& _mat) const;

  ACG::Vec3f cube_scaling(float _length) const;

  void generate_base_cube();

  void generate_cap();

  int find_current_segment(double _time, double* _lerpFactor = 0) const;

protected:

  GCode::Shared gcode_;

  // sphere geometry
  ACG::GLSphere sphere_;

  // geometry shader technique

  struct PositionData
  {
    ACG::Vec4f position; // (x,y,z,scale)
    ACG::Vec4f dx; //(x,y,z,dist)
    ACG::Vec4f dy; //(x,y,z,dist)
    ACG::Vec4f dz; //(x,y,z,1.0)
    ACG::Vec4f params; // (time, speed, type, ...)
  };


  int num_vertices_;
  ACG::GeometryBuffer array_buffer_;
  ACG::VertexDeclaration decl_;

  bool update_vbo_;


  // instancing technique
  bool instanced_;

  int num_base_cube_vertices_;
  int num_base_cube_indices_;
  ACG::GeometryBuffer base_cube_vb_;
  ACG::IndexBuffer base_cube_ib_;
  ACG::VertexDeclaration base_cube_decl_;

  struct InstanceData 
  {
    // 4x3 transform to world space
    ACG::Vec4f world0;
    ACG::Vec4f world1;
    ACG::Vec4f world2;

    ACG::Vec4f params; // (start_time, end_time, speed, edge_type)
  };

  int num_instances_;
  ACG::GeometryBuffer instance_buf_;
  ACG::VertexDeclaration instanced_decl_;

  // map from edge id to num visible instances (num segments without move types)
  std::vector<int> visible_instances_;

  // cap instances

  struct CapInstanceData
  {
    ACG::Vec4f pos;
    ACG::Vec4f params;
  };
  int num_cap_vertices_;
  int num_cap_indices_;
  ACG::GeometryBuffer cap_vb_;
  ACG::IndexBuffer cap_ib_;

  int num_cap_instances_;
  ACG::GeometryBuffer cap_instance_buf_;
  ACG::VertexDeclaration cap_instanced_decl_;

};

}
