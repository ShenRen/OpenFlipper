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


#include <ACG/GL/acg_glew.hh>

#include "GCodeNode_renderer.hh"

#include <ACG/GL/ShaderCache.hh>

#include <OpenMesh/Core/Utils/vector_cast.hh>

#include "GCodeEdgeTypes.hh"



namespace Ultimaker{

GCodeNode_renderer::GCodeNode_renderer()
  : sphere_(10, 10),
  num_vertices_(0),
  update_vbo_(false),
  instanced_(true),
  num_base_cube_vertices_(0),
  num_base_cube_indices_(0),
  num_cap_vertices_(0),
  num_cap_indices_(0),
  num_cap_instances_(0)
{
  decl_.addElement(GL_FLOAT, 4, ACG::VERTEX_USAGE_SHADER_INPUT, size_t(0), "aPosition");
  decl_.addElement(GL_FLOAT, 4, ACG::VERTEX_USAGE_SHADER_INPUT, size_t(0), "aDx");
  decl_.addElement(GL_FLOAT, 4, ACG::VERTEX_USAGE_SHADER_INPUT, size_t(0), "aDy");
  decl_.addElement(GL_FLOAT, 4, ACG::VERTEX_USAGE_SHADER_INPUT, size_t(0), "aDz");
  decl_.addElement(GL_FLOAT, 4, ACG::VERTEX_USAGE_SHADER_INPUT, size_t(0), "aParams");
}

GCodeNode_renderer::~GCodeNode_renderer()
{

}



int GCodeNode_renderer::find_current_segment(double _time, double* _lerpFactor) const
{
  int id = -1;

  if (gcode_)
  {
    int numVertices = gcode_->polyline()->n_vertices();

    // linear search
//     for (int i = 0; i < numVertices; ++i)
//     {
//       double t = gcode_->polyline_states()[i].time;
// 
//       if (t > _time)
//       {
//         id = i - 1;
//         break;
//       }
//     }

    // binary search
    int low = 0;
    int high = numVertices - 1;
    while (low < high)
    {
      int mid = (low + high) >> 1;

      double t = gcode_->polyline_states()[mid].time;

      if (t < _time)
        low = mid + 1;
      else
        high = mid;
    }

    id = high - 1;

    if (_lerpFactor && 0 <= id && id < numVertices - 1)
    {
      double t0 = gcode_->polyline_states()[id].time;
      double t1 = gcode_->polyline_states()[id + 1].time;
      double dur = t1 - t0;

      if (dur > 0.0)
        *_lerpFactor = (_time - t0) / dur;
      else
        *_lerpFactor = 0.0;
    }
  }

  return id;
}

// Render the gcode at the given time
void GCodeNode_renderer::render(ACG::GLState& _state, const ACG::GLMatrixd& _proj, const ACG::GLMatrixd& _view, DrawMode _mode, double time_, ACG::Vec4f color)
{
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  if (update_vbo_)
  {
    update_vbo();
    update_vbo_ = false;
  }


  ACG::GLMatrixf matViewProj(_proj * _view);
  ACG::GLMatrixf viewInv = _view;
  viewInv.invert();

  double lerpFactor = 0.0;
  int currentSegment = find_current_segment(time_, &lerpFactor);


  if (instanced_ && currentSegment >= 0)
  {
    const char* frag_shader = 0;
    switch (_mode)
    {
    case Color: frag_shader = "GCode/gcode_color_fs.glsl"; break;
    case Heat: frag_shader = "GCode/gcode_heat_fs.glsl"; break;
    case Speed: frag_shader = "GCode/gcode_speed_fs.glsl"; break;
    case Type: frag_shader = "GCode/gcode_type_fs.glsl"; break;
    default: return;
    }

    GLSL::Program* shader = ACG::ShaderCache::getInstance()->getProgram("GCode/gcode_instanced_vs.glsl", frag_shader);

    if (!shader || !shader->isLinked()) return;


    ACG::Vec2d clipPlanes = _proj.extract_planes();

    shader->use();
    shader->setUniform("g_mWVP", matViewProj);
    shader->setUniformMat3("g_mWVIT", viewInv, true);
    shader->setUniform("uTime", float(time_));
    shader->setUniform("uColor", color);


    int numVisibleInstances = visible_instances_[currentSegment] - 1; // current segment is rendered in a different pass -> subtract
    if (numVisibleInstances > 0)
    {
      instanced_decl_.activateShaderPipeline(shader);
      base_cube_ib_.bind();

      glDrawElementsInstanced(GL_TRIANGLES, num_base_cube_indices_, GL_UNSIGNED_INT, 0, numVisibleInstances);

      base_cube_ib_.unbind();
      instanced_decl_.deactivateShaderPipeline(shader);
      shader->disable();


      // render caps to close open parts
      GLSL::Program* capShader = ACG::ShaderCache::getInstance()->getProgram("GCode/gcode_instanced_cap_vs.glsl", frag_shader);

      if (capShader && capShader->isLinked())
      {
        // also render start cap of the current segment
        int numVisibleCaps = std::min(numVisibleInstances + 1, num_cap_instances_);

        capShader->use();
        capShader->setUniform("g_mWVP", matViewProj);
        capShader->setUniformMat3("g_mWVIT", viewInv, true);
        capShader->setUniform("g_Scale", ACG::Vec3f(gcode_->nozzle_size(), gcode_->nozzle_size(), gcode_->layer_height()));
        capShader->setUniform("uTime", float(time_));
        capShader->setUniform("uColor", color);

        cap_instanced_decl_.activateShaderPipeline(capShader);
        cap_ib_.bind();

        glDrawElementsInstanced(GL_TRIANGLES, num_cap_indices_, GL_UNSIGNED_INT, 0, numVisibleCaps);

        cap_ib_.unbind();
        cap_instanced_decl_.deactivateShaderPipeline(capShader);
        capShader->disable();
      }
    }


    // render interpolated segment if available

    if (gcode_)
    {
      ACG::GLMatrixf transformLerp;

      if (currentSegment >= 0)
      {
        int etype = int(gcode_->polyline()->edge_scalar(currentSegment));

        if (etype != GC_MOVE)
        {
          GLSL::Program* shader = ACG::ShaderCache::getInstance()->getProgram("GCode/gcode_cube_lerp_vs.glsl", frag_shader);

          if (!shader || !shader->isLinked()) return;

          ACG::Vec3d pos = gcode_->polyline()->point(currentSegment);
          ACG::Vec3d dir = gcode_->polyline()->edge_vector(currentSegment);

          // compute length
          double t0 = gcode_->polyline_states()[currentSegment].time;
          double t1 = gcode_->polyline_states()[currentSegment + 1].time;
          double length = 0.0;
          double dur = t1 - t0;

          if (dir.sqrnorm() > 1e-8 && dur > 1e-8)
          {
            double alpha = (time_ - t0) / dur;
            length = dir.norm() * alpha;
            dir.normalize();
          }
          else
            dir = ACG::Vec3d(0.0, 0.0, 0.0);

          // build transform
          ACG::Vec3f dx, dy, dz;
          compute_segment_axes(dir, dx, dy, dz);

          compute_instance_matrix(ACG::Vec3f(pos[0], pos[1], pos[2]),
            dx, dy, dz,
            cube_scaling(length),
            transformLerp);

          ACG::GLMatrixf matViewProjLerp = matViewProj * transformLerp;
          ACG::GLMatrixf matViewLerpIT = _view;
          matViewLerpIT *= transformLerp;
          matViewLerpIT.invert();

          shader->use();
          shader->setUniform("g_mWVP", matViewProjLerp);
          shader->setUniformMat3("g_mWVIT", matViewLerpIT, true);
          shader->setUniform("uTime", float(time_));
          shader->setUniform("uColor", color);

          // segment params: start time, end time, speed, edge type
          ACG::Vec4f params;
          params[0] = gcode_->polyline_states()[currentSegment].time;
          if (currentSegment + 1 < int(gcode_->polyline()->n_vertices()))
          {
            params[1] = gcode_->polyline_states()[currentSegment + 1].time;
            params[2] = gcode_->polyline_states()[currentSegment + 1].speed;
          }
          else
          {
            params[1] = params[0];
            params[2] = gcode_->polyline_states()[currentSegment].speed;
          }

          params[3] = gcode_->polyline()->edge_scalar(currentSegment);

          shader->setUniform("uParams", params);

          base_cube_ib_.bind();
          base_cube_vb_.bind();

          base_cube_decl_.activateShaderPipeline(shader);

          glDrawElements(GL_TRIANGLES, num_base_cube_indices_, GL_UNSIGNED_INT, 0);

          base_cube_decl_.deactivateShaderPipeline(shader);

          base_cube_vb_.unbind();
          base_cube_ib_.unbind();

          shader->disable();
        }
      }
    }
  }
  else
  {
    // geometry shader generates clockwise faces
    glFrontFace(GL_CW);

    GLSL::Program* shader;
    switch (_mode)
    {
    case Color: shader = ACG::ShaderCache::getInstance()->getProgram("GCode/gcode_vs.glsl", 0, 0, "GCode/gcode_gs.glsl", "GCode/gcode_color_fs.glsl"); break;
    case Heat: shader = ACG::ShaderCache::getInstance()->getProgram("GCode/gcode_vs.glsl", 0, 0, "GCode/gcode_gs.glsl", "GCode/gcode_heat_fs.glsl"); break;
    case Speed: shader = ACG::ShaderCache::getInstance()->getProgram("GCode/gcode_vs.glsl", 0, 0, "GCode/gcode_gs.glsl", "GCode/gcode_speed_fs.glsl"); break;
    case Type: shader = ACG::ShaderCache::getInstance()->getProgram("GCode/gcode_vs.glsl", 0, 0, "GCode/gcode_gs.glsl", "GCode/gcode_type_fs.glsl"); break;
    default: shader = 0;
    }

    if (!shader || !shader->isLinked()) return;

    shader->use();
    shader->setUniform("uProj", matViewProj);
    shader->setUniform("uView", ACG::GLMatrixf(_view));
    shader->setUniformMat3("uViewIT", viewInv, true);
    shader->setUniform("uTime", float(time_));
    shader->setUniform("uColor", color);

    array_buffer_.bind();
    decl_.activateShaderPipeline(shader);

    glDrawArrays(GL_LINE_STRIP, 0, num_vertices_);

    decl_.deactivateShaderPipeline(shader);
    array_buffer_.unbind();
    shader->disable();

    // revert winding order back to default
    glFrontFace(GL_CCW);
  }

  

  // render head position

  if (currentSegment >= 0)
  {
    // interpolate position on segment
    const ACG::Vec3d& p0 = gcode_->polyline()->point(currentSegment);
    const ACG::Vec3d& p1 = gcode_->polyline()->point(currentSegment + 1);

    ACG::Vec3d head_position = p0 * (1.0 - lerpFactor) + p1 * lerpFactor;

    // draw sphere geometry
    _state.set_color(ACG::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
    sphere_.draw(_state, 0.5 * gcode_->nozzle_size(), OpenMesh::vector_cast<ACG::Vec3f, ACG::Vec3d>(head_position));

    // draw point with glPointSize
//     glDisable(GL_PROGRAM_POINT_SIZE);
// 
//     GLSL::Program* shader_printhead = ACG::ShaderCache::getInstance()->getProgram("GCode/printhead_vs.glsl", "GCode/printhead_fs.glsl");
// 
//     if (shader_printhead && shader_printhead->isLinked())
//     {
//       shader_printhead->use();
//       shader_printhead->setUniform("uProj", matViewProj);
//       shader_printhead->setUniform("uHeadPosition", ACG::Vec4f(head_position[0], head_position[1], head_position[2], 1.0f));
//       shader_printhead->setUniform("uColor", ACG::Vec4f(1.0f, 0.0f, 0.0f, 0.0f));
//       glPointSize(10.0);
// 
//       glDrawArrays(GL_POINTS, 0, 1);
//       shader_printhead->disable();
//     }
  }
}

// Set the gcode to render, updates max_time accordingly
void GCodeNode_renderer::set_gcode(const GCode::Shared& _gcode)
{
  gcode_ = _gcode;

  update_vbo_ = true;
}

void GCodeNode_renderer::compute_segment_axes(const ACG::Vec3d& _dir, ACG::Vec3f& _dx, ACG::Vec3f& _dy, ACG::Vec3f& _dz) const
{
  ACG::Vec3d dir_y = ACG::Vec3d(0.0, 0.0, 1.0);
  if (fabs(fabs(_dir | dir_y) - 1.0) < 1e-3)
    dir_y = ACG::Vec3d(0.0, 1.0, 0.0);
  ACG::Vec3d dir_x = (dir_y % _dir).normalized();
  dir_y = (dir_x % _dir).normalized();

  _dx = ACG::Vec3f(dir_x[0], dir_x[1], dir_x[2]);
  _dy = ACG::Vec3f(dir_y[0], dir_y[1], dir_y[2]);
  _dz = ACG::Vec3f(_dir[0], _dir[1], _dir[2]);
}


void GCodeNode_renderer::compute_instance_matrix(const ACG::Vec3f& _pos, const ACG::Vec3f& _dx, const ACG::Vec3f& _dy, const ACG::Vec3f& _dz, const ACG::Vec3f& _scale, ACG::GLMatrixf& _mat) const
{
  _mat.identity();

  // translate
  _mat.translate(_pos);

  // rotate -  directions as column vectors
  ACG::GLMatrixf rot;
  rot.identity();
  for (int k = 0; k < 3; ++k)
  {
    rot(k, 0) = _dx[k];
    rot(k, 1) = _dy[k];
    rot(k, 2) = _dz[k];
  }
  _mat *= rot;

  // scale
  _mat.scale(_scale);
}

ACG::Vec3f GCodeNode_renderer::cube_scaling(float _length) const
{
  float scale_x = gcode_ ? float(gcode_->nozzle_size()) : 1.0f;
  float scale_y = gcode_ ? float(gcode_->layer_height()) : 1.0f;
  return ACG::Vec3f(scale_x, scale_y, _length);
}

void GCodeNode_renderer::update_vbo(){
  std::vector<PositionData> positions;
  std::vector<InstanceData> instances;
  std::vector<CapInstanceData> caps;

  if (gcode_)
  {
    ptr::shared_ptr<PolyLine> line = gcode_->polyline();
    std::vector<GCode::State> states = gcode_->polyline_states();

    if (!line || line->n_vertices() < 2)
    {
      std::cout << "Empty line" << std::endl;
      return;
    }


    int n_verts_to_process = line->n_vertices();
    if (instanced_)
      --n_verts_to_process;

    for (int i = 0; i < n_verts_to_process; ++i)
    {
      GCode::State node_state;
      GCode::State next_node_state;


      int last_edge_type;
      int next_edge_type;
      ACG::Vec3d pos = line->point(i);
      ACG::Vec3d last_dir, next_dir, next_vec;
      if (i == 0 && line->n_vertices() >= 2)
      {
        last_edge_type = line->edge_scalar(i);
        next_edge_type = line->edge_scalar(i);

        last_dir = line->edge_vector(i).normalized();
        next_vec = line->edge_vector(i);

        node_state = states[i];
        next_node_state = states[i + 1];

      }
      else if (i + 1 != int(line->n_vertices()))
      {
        last_edge_type = line->edge_scalar(i - 1);
        next_edge_type = line->edge_scalar(i);

        last_dir = line->edge_vector(i - 1).normalized();
        next_vec = line->edge_vector(i);

        node_state = states[i];
        next_node_state = states[i + 1];
      }
      else
      {
        last_edge_type = line->edge_scalar(i - 1);
        next_edge_type = line->edge_scalar(i - 1);

        last_dir = line->edge_vector(i - 1).normalized();
        next_vec = line->edge_vector(i - 1);

        node_state = states[i];
        next_node_state = states[i];
      }
      next_dir = next_vec.normalized();

      if (last_edge_type != next_edge_type && (last_edge_type == GC_MOVE || next_edge_type == GC_MOVE))
      {
        if (last_edge_type == GC_MOVE)
          last_dir = next_dir;
        else
          next_dir = last_dir;
      }

      ACG::Vec3d dir = next_dir;//0.5*(last_dir + next_dir);

      ACG::Vec3f dx, dy, dz;
      compute_segment_axes(dir, dx, dy, dz);

      ACG::Vec3f scale = cube_scaling(next_vec.norm());

      float speed = (i + 1 != int(line->n_vertices())) ? next_node_state.speed : 9000.0f;

      if (instanced_)
      {
        if (instances.empty())
        {
          instances.resize(line->n_vertices() - 1);
          num_instances_ = 0;
        }

        if (visible_instances_.empty())
          visible_instances_.resize(line->n_vertices());

        if (caps.empty())
          caps.resize(line->n_vertices());

        if (next_edge_type != GC_MOVE)
        {
          CapInstanceData& cap = caps[num_instances_];

          InstanceData& data = instances[num_instances_++];

          // build world matrix of the instance
          ACG::GLMatrixf world;

          compute_instance_matrix(ACG::Vec3f(pos[0], pos[1], pos[2]),
            dx, dy, dz,
            scale,
            world);

          // store in row vectors
          for (int k = 0; k < 4; ++k)
          {
            data.world0[k] = world(0, k);
            data.world1[k] = world(1, k);
            data.world2[k] = world(2, k);
          }

          // store segment params
          data.params[0] = float(node_state.time);
          data.params[1] = float(next_node_state.time);
          data.params[2] = speed;
          data.params[3] = float(next_edge_type);

          // generate cap at start of segment
          ACG::Vec3f pos3 = OpenMesh::vector_cast<ACG::Vec3f, ACG::Vec3d>(pos);
          cap.pos = ACG::Vec4f(pos3[0], pos3[1], pos3[2], 1.0f);
          cap.params = data.params;
        }

        visible_instances_[i] = num_instances_;
      }
      else
      {
        if (positions.empty())
          positions.resize(line->n_vertices());

        PositionData& data = positions[i];
        data.position[0] = pos[0]; data.position[1] = pos[1]; data.position[2] = pos[2]; data.position[3] = node_state.time;
        data.dx = ACG::Vec4f(dx[0], dx[1], dx[2], scale[0] * 0.5f);
        data.dy = ACG::Vec4f(dy[0], dy[1], dy[2], scale[1] * 0.5f);
        data.dz = ACG::Vec4f(dz[0], dz[1], dz[2], 1.0f);
        data.params[0] = speed;
        data.params[1] = next_edge_type; // Edge type
        data.params[2] = 0.0; // Unused
        data.params[3] = 0.0; // Unused
      }
    }
  }

  if (instanced_ && num_instances_ > 0)
  {
    if (!num_base_cube_vertices_)
      generate_base_cube();

    visible_instances_.back() = num_instances_;

    instance_buf_.upload(num_instances_ * sizeof(InstanceData), &instances[0], GL_STATIC_DRAW);

    instanced_decl_.clear();
    instanced_decl_.addElement(GL_FLOAT, 3, ACG::VERTEX_USAGE_POSITION, size_t(0), "inPosition", 0, base_cube_vb_.id());
    instanced_decl_.addElement(GL_FLOAT, 3, ACG::VERTEX_USAGE_NORMAL, size_t(0), "inNormal", 0, base_cube_vb_.id());

    // set instance source
    instanced_decl_.addElement(GL_FLOAT, 4, ACG::VERTEX_USAGE_SHADER_INPUT, size_t(0), "inWorld0", 1, instance_buf_.id());
    instanced_decl_.addElement(GL_FLOAT, 4, ACG::VERTEX_USAGE_SHADER_INPUT, size_t(0), "inWorld1", 1, instance_buf_.id());
    instanced_decl_.addElement(GL_FLOAT, 4, ACG::VERTEX_USAGE_SHADER_INPUT, size_t(0), "inWorld2", 1, instance_buf_.id());
    instanced_decl_.addElement(GL_FLOAT, 4, ACG::VERTEX_USAGE_SHADER_INPUT, size_t(0), "inParams", 1, instance_buf_.id());



    // cap instances
    // todo: store pos and rotation*scale parts of matrix separately to reuse instance buffer here

    if (!num_cap_vertices_)
      generate_cap();

    num_cap_instances_ = num_instances_;
    cap_instance_buf_.upload(num_cap_instances_ * sizeof(CapInstanceData), &caps[0], GL_STATIC_DRAW);

    cap_instanced_decl_.clear();
    cap_instanced_decl_.addElement(GL_FLOAT, 3, ACG::VERTEX_USAGE_POSITION, size_t(0), "inPosition", 0, cap_vb_.id());
    cap_instanced_decl_.addElement(GL_FLOAT, 3, ACG::VERTEX_USAGE_NORMAL, size_t(0), "inNormal", 0, cap_vb_.id());

    // set instance source
    cap_instanced_decl_.addElement(GL_FLOAT, 4, ACG::VERTEX_USAGE_SHADER_INPUT, size_t(0), "inTranslation", 1, cap_instance_buf_.id());
    cap_instanced_decl_.addElement(GL_FLOAT, 4, ACG::VERTEX_USAGE_SHADER_INPUT, size_t(0), "inParams", 1, cap_instance_buf_.id());
  }
  else
  {
    num_vertices_ = int(positions.size());
    array_buffer_.upload(positions.size() * sizeof(PositionData), &positions[0], GL_STATIC_DRAW);
  }
}


void GCodeNode_renderer::generate_base_cube()
{
  // stripified unit cube without faces at start and end of a gcode line segment

  /*
  tristrips:

  1-----0
  |  /  |   // bottom face
  7-----6
  |  /  |   // back face
  5-----4
  |  /  |   // top face
  3-----2
  |  /  |   // front face
  1-----0
  */

//   float pos[] =
//   {
//     0.5f, -0.5f, 1.0f, // 0
//     0.5f, -0.5f, 0.0f, // 1
//     0.5f, 0.5f, 1.0f,  // 2
//     0.5f, 0.5f, 0.0f,  // 3
//     -0.5f, 0.5f, 1.0f, // 4
//     -0.5f, 0.5f, 0.0f, // 5
//     -0.5f, -0.5f, 1.0f,// 6
//     -0.5f, -0.5f, 0.0f,// 7
//     0.5f, -0.5f, 1.0f, // 0
//     0.5f, -0.5f, 0.0f  // 1
//   };
// 
//   num_base_cube_vertices_ = 10;
//   num_base_cube_indices_ = 0;
// 
//   base_cube_.upload(sizeof(pos), pos, GL_STATIC_DRAW);
//   base_cube_decl_.clear();
//   base_cube_decl_.addElement(GL_FLOAT, 3, ACG::VERTEX_USAGE_POSITION);
// problem: normal information difficult to encode for stripified mesh


  // unit cube with missing side faces:
  float vb[] =
  {
    -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f,
    0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f,
    0.5f, -0.5f, 1.0f, 0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f, 1.0f, 0.0f, -1.0f, 0.0f,
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, 1.0f, -1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 1.0f, -1.0f, 0.0f, 0.0f
  };
  int ib[] =
  {
    0, 1, 2,
    2, 3, 0,
    4, 5, 6,
    6, 7, 4,
    8, 9, 10,
    10, 11, 8,
    12, 13, 14,
    14, 15, 12
  };

  num_base_cube_vertices_ = sizeof(vb) / sizeof(vb[0]);

  base_cube_decl_.clear();
  base_cube_decl_.addElement(GL_FLOAT, 3, ACG::VERTEX_USAGE_POSITION);
  base_cube_decl_.addElement(GL_FLOAT, 3, ACG::VERTEX_USAGE_NORMAL);

  base_cube_vb_.upload(sizeof(vb), vb, GL_STATIC_DRAW);


  num_base_cube_indices_ = sizeof(ib) / sizeof(ib[0]);

  // indices are stored as cw
  // flip winding to comply with default ccw as front face
  for (int i = 0; i < num_base_cube_indices_ / 3; ++i)
    std::swap(ib[i * 3], ib[i * 3 + 1]);


  base_cube_ib_.upload(sizeof(ib), ib, GL_STATIC_DRAW);
}

void GCodeNode_renderer::generate_cap()
{
  //nverts = 40
  //ntris = 36
  float vb[] =
  {
    -0.0f, -0.5f, 0.5f, -0.0f, -1.0f, -0.0f,
    -0.3f, -0.4f, 0.5f, -0.6f, -0.8f, -0.0f,
    -0.3f, -0.4f, -0.5f, -0.6f, -0.8f, -0.0f,
    0.0f, -0.5f, -0.5f, -0.0f, -1.0f, -0.0f,
    0.3f, -0.4f, 0.5f, 0.6f, -0.8f, 0.0f,
    -0.5f, -0.2f, 0.5f, -1.0f, -0.3f, -0.0f,
    -0.5f, -0.2f, -0.5f, -1.0f, -0.3f, -0.0f,
    -0.5f, 0.2f, 0.5f, -1.0f, 0.3f, -0.0f,
    -0.5f, 0.2f, -0.5f, -1.0f, 0.3f, -0.0f,
    -0.3f, 0.4f, 0.5f, -0.6f, 0.8f, -0.0f,
    -0.3f, 0.4f, -0.5f, -0.6f, 0.8f, -0.0f,
    0.0f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    0.3f, 0.4f, 0.5f, 0.6f, 0.8f, 0.0f,
    0.3f, 0.4f, -0.5f, 0.6f, 0.8f, 0.0f,
    0.5f, 0.2f, 0.5f, 1.0f, 0.3f, 0.0f,
    0.5f, 0.2f, -0.5f, 1.0f, 0.3f, 0.0f,
    0.5f, -0.2f, 0.5f, 1.0f, -0.3f, 0.0f,
    0.5f, -0.2f, -0.5f, 1.0f, -0.3f, 0.0f,
    0.3f, -0.4f, -0.5f, 0.6f, -0.8f, 0.0f,
    0.5f, -0.2f, 0.5f, -0.0f, 0.0f, 1.0f,
    0.5f, 0.2f, 0.5f, -0.0f, 0.0f, 1.0f,
    0.3f, 0.4f, 0.5f, -0.0f, 0.0f, 1.0f,
    -0.0f, -0.5f, 0.5f, -0.0f, 0.0f, 1.0f,
    0.3f, -0.4f, 0.5f, -0.0f, 0.0f, 1.0f,
    0.0f, 0.5f, 0.5f, -0.0f, 0.0f, 1.0f,
    -0.3f, 0.4f, 0.5f, -0.0f, 0.0f, 1.0f,
    -0.5f, -0.2f, 0.5f, -0.0f, 0.0f, 1.0f,
    -0.3f, -0.4f, 0.5f, -0.0f, 0.0f, 1.0f,
    -0.5f, 0.2f, 0.5f, -0.0f, 0.0f, 1.0f,
    -0.3f, -0.4f, -0.5f, 0.0f, -0.0f, -1.0f,
    -0.5f, -0.2f, -0.5f, 0.0f, -0.0f, -1.0f,
    -0.5f, 0.2f, -0.5f, 0.0f, -0.0f, -1.0f,
    0.3f, -0.4f, -0.5f, 0.0f, -0.0f, -1.0f,
    0.0f, -0.5f, -0.5f, 0.0f, -0.0f, -1.0f,
    -0.3f, 0.4f, -0.5f, 0.0f, -0.0f, -1.0f,
    0.0f, 0.5f, -0.5f, 0.0f, -0.0f, -1.0f,
    0.5f, 0.2f, -0.5f, 0.0f, -0.0f, -1.0f,
    0.5f, -0.2f, -0.5f, 0.0f, -0.0f, -1.0f,
    0.3f, 0.4f, -0.5f, 0.0f, -0.0f, -1.0f
  };
  int ib[] =
  {
    0, 1, 2,
    2, 3, 0,
    4, 0, 3,
    1, 5, 6,
    6, 2, 1,
    5, 7, 8,
    8, 6, 5,
    7, 9, 10,
    10, 8, 7,
    9, 11, 12,
    12, 10, 9,
    11, 13, 14,
    14, 12, 11,
    13, 15, 16,
    16, 14, 13,
    15, 17, 18,
    18, 16, 15,
    17, 4, 19,
    19, 18, 17,
    3, 19, 4,
    20, 21, 22,
    20, 22, 23,
    24, 20, 23,
    22, 25, 26,
    22, 26, 23,
    27, 28, 23,
    26, 27, 23,
    26, 29, 27,
    30, 31, 32,
    30, 32, 33,
    34, 30, 33,
    32, 35, 36,
    32, 36, 33,
    37, 38, 33,
    36, 37, 33,
    36, 39, 37
  };

  num_cap_vertices_ = sizeof(vb) / sizeof(vb[0]);
  num_cap_indices_ = sizeof(ib) / sizeof(ib[0]);

  cap_vb_.upload(sizeof(vb), vb, GL_STATIC_DRAW);
  cap_ib_.upload(sizeof(ib), ib, GL_STATIC_DRAW);
}


}
