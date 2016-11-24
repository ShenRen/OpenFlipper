/* ========================================================================= *
 *                                                                           *
 *                               OpenFlipper                                 *
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
 *                                                                           *
 * ========================================================================= */

/*===========================================================================*\
 *
 *   $Revision$
 *   $Date$
 *
\*===========================================================================*/





//=============================================================================
//
//  CLASS GlutPrimitiveNode - IMPLEMENTATION
//
//=============================================================================

#define CAMERAVISNODE_C

//== INCLUDES =================================================================
#include "CameraNode.hh"

#include <OpenMesh/Core/Utils/vector_cast.hh>

#include <ACG/GL/IRenderer.hh>
#include <ACG/GL/ShaderCache.hh>
#include <ACG/ShaderUtils/GLSLShader.hh>

//== NAMESPACES ===============================================================

namespace ACG {
namespace SceneGraph {

//== IMPLEMENTATION ==========================================================

/// Default constructor.
CameraNode::CameraNode(BaseNode* _parent, std::string _name) :
    BaseNode(_parent, _name),
    bbmin_(FLT_MAX,FLT_MAX,FLT_MAX),
    bbmax_(FLT_MIN,FLT_MIN,FLT_MIN),
    cylinder_(0),
    cone_(0),
    axis_length_(0.1f),
    update_vbo_(true),
    showFrustum_(false) {

    modelView_.identity();

    projection_.identity();
    // Setup a standard projection ( Full fovy 90, aspect 1.0, near 1.0, far 2.0 )
    projection_.perspective(45 ,1.0,1.0,2.0);


    cylinder_ = new GLCylinder(8, 4, 1.0f, false, false);
    cone_ = new GLCone(8, 1, 1.0f, 0.0f, true, false);
}

CameraNode::~CameraNode() {
  if (cylinder_)
    delete cylinder_;

  if (cone_)
    delete cone_;
}

void CameraNode::boundingBox(Vec3d& _bbMin, Vec3d& _bbMax) {
  _bbMin.minimize(bbmin_);
  _bbMax.maximize(bbmax_);
}

//----------------------------------------------------------------------------

DrawModes::DrawMode CameraNode::availableDrawModes() const {
    return (DrawModes::POINTS |
            DrawModes::WIREFRAME |
            DrawModes::HIDDENLINE |
            DrawModes::SOLID_FLAT_SHADED |
            DrawModes::SOLID_SMOOTH_SHADED);
}

//----------------------------------------------------------------------------

void CameraNode::draw(GLState& _state, const DrawModes::DrawMode& /*_drawMode*/) {

    glPushAttrib(GL_LIGHTING_BIT);
    glPushAttrib(GL_ENABLE_BIT);
    ACG::GLState::shadeModel(GL_SMOOTH);
    ACG::GLState::enable(GL_LIGHTING); // Turn lighting on

    // Store modelview matrix
    _state.push_modelview_matrix();

    Vec4f lastBaseColor     = _state.base_color();
    Vec4f lastDiffuseColor  = _state.diffuse_color();
    Vec4f lastSpecularColor = _state.specular_color();

    _state.set_base_color(ACG::Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
    _state.set_diffuse_color(ACG::Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
    _state.set_specular_color(ACG::Vec4f(1.0f, 1.0f, 0.0f, 1.0f));

    // Draw camera box

    updateVBO();

    vbo_.bind();
    ibo_.bind();

    glPushAttrib(GL_LIGHTING_BIT);
    ACG::GLState::disable(GL_LIGHTING); // Disable lighting

    vdecl_.activateFixedFunction();


    bool hasOrigin = projection_.isPerspective();
    GLsizei lineOffset = offsetLines_;


    if (showFrustum_)
    {
      // tris
      glColor4f(0.66f, 0.66f, 0.66f, 1.0f);

      glDrawElements(GL_TRIANGLES, lineOffset, GL_UNSIGNED_INT, (GLvoid*)(offsetTris_ * sizeof(int)));

      // lines
      GLsizei lineCount = hasOrigin ? 4 * 7 : 4 * 6;

      glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
      glDrawElements(GL_LINES, lineCount * 2, GL_UNSIGNED_INT, (GLvoid*)(lineOffset * sizeof(int)));
    }
    else
    {
      glColor4f(0.0f, 0.0f, 0.0f, 1.0f);

      // only front plane
      GLsizei lineCount = hasOrigin ? 4 * 2 : 4 * 1;

      glDrawElements(GL_LINES, lineCount * 2, GL_UNSIGNED_INT, (GLvoid*)(offsetFront_ * sizeof(int)));
    }


    vdecl_.deactivateFixedFunction();

    glPopAttrib();


    vbo_.unbind();
    ibo_.unbind();


    // Coordinate axes

    _state.set_modelview(_state.modelview() * modelViewInv_);


    // Draw right vector
    _state.rotate(90, 0.0, 1.0, 0.0);

    _state.set_base_color(ACG::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
    _state.set_diffuse_color(ACG::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
    _state.set_specular_color(ACG::Vec4f(1.0f, 0.4f, 0.4f, 1.0f));

    cylinder_->setBottomRadius(axis_length_/20.0f);
    cylinder_->setTopRadius(axis_length_/20.0f);
    cylinder_->draw(_state, axis_length_);

    // Draw top
    _state.translate(0.0, 0.0, axis_length_ );
    cone_->setBottomRadius(axis_length_/5.0f);
    cone_->setTopRadius(0.0f);
    cone_->draw(_state, axis_length_/2.0f);
    _state.translate(0.0, 0.0, -axis_length_ );

    // Draw up vector
    _state.rotate(-90, 1.0, 0.0, 0.0);

    _state.set_base_color(ACG::Vec4f(0.0f, 1.0f, 0.0f, 1.0f));
    _state.set_diffuse_color(ACG::Vec4f(0.0f, 1.0f, 0.0f, 1.0f));
    _state.set_specular_color(ACG::Vec4f(0.4f, 1.0f, 0.4f, 1.0f));

    cylinder_->draw(_state, axis_length_);

    // Draw top
    _state.translate(0.0, 0.0, axis_length_ );
    cone_->draw(_state, axis_length_/2.0f);
    _state.translate(0.0, 0.0, -axis_length_ );

    // Draw viewing direction vector
    _state.rotate(-90, 0.0, 1.0, 0.0);

    _state.set_base_color(ACG::Vec4f(0.0f, 0.0f, 1.0f, 1.0f));
    _state.set_diffuse_color(ACG::Vec4f(0.0f, 0.0f, 1.0f, 1.0f));
    _state.set_specular_color(ACG::Vec4f(0.4f, 0.4f, 1.0f, 1.0f));

    cylinder_->draw(_state, axis_length_);

    // Draw top
    _state.translate(0.0, 0.0, axis_length_ );
    cone_->draw(_state, axis_length_/2.0f);
    _state.translate(0.0, 0.0, -axis_length_ );


    // Reset to previous modelview
    _state.pop_modelview_matrix();

    _state.set_base_color(lastBaseColor);
    _state.set_diffuse_color(lastDiffuseColor);
    _state.set_specular_color(lastSpecularColor);

    glPopAttrib(); // GL_ENABLE_BIT
    glPopAttrib(); // LIGHTING
}

//----------------------------------------------------------------------------

void CameraNode::getRenderObjects(IRenderer* _renderer, GLState& _state, const DrawModes::DrawMode& _drawMode, const Material* _mat)
{
  updateVBO();

  RenderObject obj;
  obj.initFromState(&_state);
  obj.depthTest = true;
  obj.shaderDesc.shadeMode = SG_SHADE_UNLIT;


  obj.vertexBuffer = vbo_.id();
  obj.indexBuffer = ibo_.id();
  obj.vertexDecl = &vdecl_;

  bool hasOrigin = projection_.isPerspective();

  GLsizei lineOffset = offsetLines_;

  if (showFrustum_)
  {
    // tris
    obj.debugName = "CameraNode.frustum_tris";
    obj.emissive = Vec3f(0.66f, 0.66f, 0.66f);
    obj.glDrawElements(GL_TRIANGLES, lineOffset, GL_UNSIGNED_INT, (GLvoid*)(offsetTris_ * sizeof(int)));

    obj.depthRange = Vec2f(0.01f, 1.0f);
    _renderer->addRenderObject(&obj);


    // lines
    GLsizei lineCount = hasOrigin ? 4 * 7 : 4 * 6;

    obj.debugName = "CameraNode.frustum_lines";
    obj.emissive = Vec3f(0.0f, 0.0f, 0.0f);

    obj.depthRange = Vec2f(0.0f, 1.0f);

    obj.setupLineRendering(_state.line_width(), Vec2f(_state.viewport_width(), _state.viewport_height()));

    obj.glDrawElements(GL_LINES, lineCount * 2, GL_UNSIGNED_INT, (GLvoid*)(lineOffset * sizeof(int)));
    _renderer->addRenderObject(&obj);

    obj.resetLineRendering();
  }
  else
  {
    obj.debugName = "CameraNode.frustum_lines";
    obj.emissive = Vec3f(0.0f, 0.0f, 0.0f);

    // only front plane
    GLsizei lineCount = hasOrigin ? 4 * 2 : 4 * 1;

    obj.setupLineRendering(_state.line_width(), Vec2f(_state.viewport_width(), _state.viewport_height()));

    obj.glDrawElements(GL_LINES, lineCount * 2, GL_UNSIGNED_INT, (GLvoid*)(offsetFront_ * sizeof(int)));
    _renderer->addRenderObject(&obj);
    obj.resetLineRendering();
  }


  obj.vertexBuffer = 0;
  obj.indexBuffer = 0;
  obj.vertexDecl = 0;


  // draw coordinate axis
  cylinder_->setBottomRadius(axis_length_/20.0f);
  cylinder_->setTopRadius(axis_length_/20.0f);

  cone_->setBottomRadius(axis_length_/5.0f);
  cone_->setTopRadius(0.0f);

  GLMatrixd matView = obj.modelview * modelViewInv_;

  // right vec
  obj.debugName = "CameraNode.right_vec";
  obj.emissive = Vec3f(1.0f, 0.0f, 0.0f);
  obj.diffuse = Vec3f(1.0f, 0.0f, 0.0f);
  obj.ambient = Vec3f(1.0f, 0.0f, 0.0f);
  obj.specular = Vec3f(1.0f, 0.4f, 0.4f);

  obj.modelview = matView;
  obj.modelview.rotateY(90.0);
  obj.modelview.scale(Vec3d(1.0, 1.0, axis_length_));
  cylinder_->addToRenderer_primitive(_renderer, &obj);

  // right top
  obj.debugName = "CameraNode.right_top";
  obj.modelview = matView;
  obj.modelview.rotateY(90.0);
  obj.modelview.translate(Vec3d(0.0, 0.0, axis_length_));
  obj.modelview.scale(Vec3d(1.0, 1.0, axis_length_ * 0.5));
  cone_->addToRenderer_primitive(_renderer, &obj);


  // up vec
  obj.debugName = "CameraNode.up_vec";
  obj.emissive = Vec3f(0.0f, 1.0f, 0.0f);
  obj.diffuse = Vec3f(0.0f, 1.0f, 0.0f);
  obj.ambient = Vec3f(0.0f, 1.0f, 0.0f);
  obj.specular = Vec3f(0.4f, 1.0f, 0.4f);

  obj.modelview = matView;
  obj.modelview.rotateX(-90.0);
  obj.modelview.scale(Vec3d(1.0, 1.0, axis_length_));
  cylinder_->addToRenderer_primitive(_renderer, &obj);

  // up top
  obj.debugName = "CameraNode.up_top";
  obj.modelview = matView;
  obj.modelview.rotateX(-90.0);
  obj.modelview.translate(Vec3d(0.0, 0.0, axis_length_));
  obj.modelview.scale(Vec3d(1.0, 1.0, axis_length_ * 0.5));
  cone_->addToRenderer_primitive(_renderer, &obj);



  // Draw viewing direction vector
  obj.debugName = "CameraNode.view_vec";
  obj.emissive = Vec3f(0.0f, 0.0f, 1.0f);
  obj.diffuse = Vec3f(0.0f, 0.0f, 1.0f);
  obj.ambient = Vec3f(0.0f, 0.0f, 1.0f);
  obj.specular = Vec3f(0.4f, 0.4f, 1.0f);

  obj.modelview = matView;
  obj.modelview.scale(Vec3d(1.0, 1.0, axis_length_));
  cylinder_->addToRenderer_primitive(_renderer, &obj);

  // top
  obj.debugName = "CameraNode.view_top";
  obj.modelview = matView;
  obj.modelview.translate(Vec3d(0.0, 0.0, axis_length_));
  obj.modelview.scale(Vec3d(1.0, 1.0, axis_length_ * 0.5));
  cone_->addToRenderer_primitive(_renderer, &obj);
}

//----------------------------------------------------------------------------

void CameraNode::pick(GLState& _state, PickTarget /*_target*/)
{
  _state.pick_set_maximum(2);


  updateVBO();

  vbo_.bind();
  ibo_.bind();


  GLSL::Program* pickShader = ShaderCache::getInstance()->getProgram("Picking/vertex.glsl", "Picking/single_color_fs.glsl");

  if (pickShader && pickShader->isLinked())
  {
    pickShader->use();

    // world view projection matrix
    GLMatrixf matFrustum = _state.projection() * _state.modelview();
    pickShader->setUniform("mWVP", matFrustum);

    Vec4f pickColor = _state.pick_get_name_color_norm(0);
    pickShader->setUniform("color", pickColor);



    // pick frustum

    vdecl_.activateShaderPipeline(pickShader);

    GLsizei lineOffset = offsetLines_;

    if (showFrustum_)
      glDrawElements(GL_TRIANGLES, lineOffset, GL_UNSIGNED_INT, (GLvoid*)(offsetTris_ * sizeof(int)));
    else
    {
      bool hasOrigin = projection_.isPerspective();
      GLsizei lineCount = hasOrigin ? 4 * 2 : 4 * 1;

      glDrawElements(GL_LINES, lineCount * 2, GL_UNSIGNED_INT, (GLvoid*)(offsetFront_ * sizeof(int)));
    }


    vdecl_.deactivateShaderPipeline(pickShader);


    // pick coordinate axis
    pickColor = _state.pick_get_name_color_norm(1);
    pickShader->setUniform("color", pickColor);


    cylinder_->setBottomRadius(axis_length_/20.0f);
    cylinder_->setTopRadius(axis_length_/20.0f);

    cone_->setBottomRadius(axis_length_/5.0f);
    cone_->setTopRadius(0.0f);


    GLMatrixf matVP = matFrustum * modelViewInv_;
    GLMatrixf mat;

    // right vec
    mat = matVP;
    mat.rotateY(90.0);
    mat.scale(Vec3f(1.0, 1.0, axis_length_));
    pickShader->setUniform("mWVP", mat);
    cylinder_->draw_primitive(pickShader);

    // right top
    mat = matVP;
    mat.rotateY(90.0);
    mat.translate(Vec3f(0.0, 0.0, axis_length_));
    mat.scale(Vec3f(1.0, 1.0, axis_length_ * 0.5));
    pickShader->setUniform("mWVP", mat);
    cone_->draw_primitive(pickShader);


    // up vec
    mat = matVP;
    mat.rotateX(-90.0);
    mat.scale(Vec3f(1.0, 1.0, axis_length_));
    pickShader->setUniform("mWVP", mat);
    cylinder_->draw_primitive(pickShader);

    // up top
    mat = matVP;
    mat.rotateX(-90.0);
    mat.translate(Vec3f(0.0, 0.0, axis_length_));
    mat.scale(Vec3f(1.0, 1.0, axis_length_ * 0.5));
    pickShader->setUniform("mWVP", mat);
    cone_->draw_primitive(pickShader);



    // Draw viewing direction vector
    mat = matVP;
    mat.scale(Vec3f(1.0, 1.0, axis_length_));
    pickShader->setUniform("mWVP", mat);
    cylinder_->draw_primitive(pickShader);

    // top
    mat = matVP;
    mat.translate(Vec3f(0.0, 0.0, axis_length_));
    mat.scale(Vec3f(1.0, 1.0, axis_length_ * 0.5));
    pickShader->setUniform("mWVP", mat);
    cone_->draw_primitive(pickShader);




    pickShader->disable();
  }
  else
  {
    _state.pick_set_name(0);

    // Store modelview matrix
    _state.push_modelview_matrix();

    // Draw camera box
    bool hasOrigin = projection_.isPerspective();
    GLsizei lineOffset = offsetLines_;


    vbo_.bind();
    ibo_.bind();

    vdecl_.activateFixedFunction();

    if (showFrustum_)
    {
      // tris
      glDrawElements(GL_TRIANGLES, lineOffset, GL_UNSIGNED_INT, (GLvoid*)(offsetTris_ * sizeof(int)));

      // lines
      GLsizei lineCount = hasOrigin ? 4 * 7 : 4 * 6;
      glDrawElements(GL_LINES, lineCount * 2, GL_UNSIGNED_INT, (GLvoid*)(lineOffset * sizeof(int)));
    }
    else
    {
      // only front plane
      GLsizei lineCount = hasOrigin ? 4 * 2 : 4 * 1;

      glDrawElements(GL_LINES, lineCount * 2, GL_UNSIGNED_INT, (GLvoid*)(offsetFront_ * sizeof(int)));
    }

    vdecl_.deactivateFixedFunction();

    ibo_.unbind();
    vbo_.unbind();


    // Set modelview matrix such that it matches
    // the remote settings (+ the local transformation).
    // This is performed by multiplying the local
    // modelview matrix by the inverse remote
    // modelview matrix: M_l' = M_l * M^{-1}_r
    _state.set_modelview(_state.modelview() * modelViewInv_);

    _state.pick_set_name(1);

    // Draw right vector
    _state.rotate(90, 0.0, 1.0, 0.0);

    cylinder_->setBottomRadius(axis_length_/20.0f);
    cylinder_->setTopRadius(axis_length_/20.0f);
    cylinder_->draw(_state, axis_length_);

    // Draw top
    _state.translate(0.0, 0.0, axis_length_ );
    cone_->setBottomRadius(axis_length_/5.0f);
    cone_->setTopRadius(0.0f);
    cone_->draw(_state, axis_length_/2.0f);
    _state.translate(0.0, 0.0, -axis_length_ );

    // Draw up vector
    _state.rotate(-90, 1.0, 0.0, 0.0);

    cylinder_->draw(_state, axis_length_);

    // Draw top
    _state.translate(0.0, 0.0, axis_length_ );
    cone_->draw(_state, axis_length_/2.0f);
    _state.translate(0.0, 0.0, -axis_length_ );

    // Draw viewing direction vector
    _state.rotate(-90, 0.0, 1.0, 0.0);

    cylinder_->draw(_state, axis_length_);

    // Draw top
    _state.translate(0.0, 0.0, axis_length_ );
    cone_->draw(_state, axis_length_/2.0f);
    _state.translate(0.0, 0.0, -axis_length_ );

    // Reset to previous modelview
    _state.pop_modelview_matrix();
  }

  vbo_.unbind();
  ibo_.unbind();
}

//----------------------------------------------------------------------------

void CameraNode::updateVBO()
{
  if (update_vbo_)
  {
    updateFrustumWS();

    vbo_.bind();
    vbo_.upload(sizeof(Vec4f) * vboData_.size(), &vboData_[0], GL_STATIC_DRAW);
    vbo_.unbind();

    vdecl_.clear();
    vdecl_.addElement(GL_FLOAT, 4, VERTEX_USAGE_POSITION);

    update_vbo_ = false;

    updateBoundingBoxes();
  }

  if (!ibo_.is_valid())
  {
    int data[] =
    {
      // frustum triangles (planes facing the inside of the frustum)
      3,2,6 , 6,5,3 , // right
      1,0,4 , 4,7,1 , // left
      4,5,6 , 6,7,4 , // top
      0,1,2 , 2,3,0 , // bottom
      0,3,5 , 5,4,0 , // back
//      2,1,7 , 7,6,2 , // front

      // frustum lines
      3,2, 2,6, 6,5, 5,3, // right
      1,0, 0,4, 4,7, 7,1, // left
      4,5, 5,6, 6,7, 7,4, // top
      0,1, 1,2, 2,3, 3,0, // bottom
      0,3, 3,5, 5,4, 4,0, // back
      2,1, 1,7, 7,6, 6,2, // front

      // cam origin to near plane lines
      8,1, 8,2, 8,6, 8,7
    };

    offsetTris_ = 0;
    offsetLines_ = 6*5;
    offsetFront_ = offsetLines_ + 8*5;

    ibo_.bind();
    ibo_.upload(sizeof(data), data, GL_STATIC_DRAW);
    ibo_.unbind();
  }
}

//----------------------------------------------------------------------------

void CameraNode::updateBoundingBoxes()
{
  bbmin_ = Vec3d(DBL_MAX, DBL_MAX, DBL_MAX);
  bbmax_ = Vec3d(-DBL_MAX, -DBL_MAX, -DBL_MAX);

  if (update_vbo_ || vboData_.empty())
    updateFrustumWS();

  for (size_t i = 0; i < vboData_.size(); ++i)
  {
    Vec3d v = OpenMesh::vector_cast<Vec3d, Vec4f>(vboData_[i]);

    bbmin_.minimize(v);
    bbmax_.maximize(v);
  }

  bbmin_ -= Vec3d(axis_length_ * 2.0);
  bbmax_ += Vec3d(axis_length_ * 2.0);
}

//----------------------------------------------------------------------------

void CameraNode::updateFrustumWS()
{
  vboData_.resize(9);

  // frustum vertices in clip space
  Vec4f posCS[8] =
  {
    Vec4f(-1, -1, 1, 1),  //0  frustum vertices..
    Vec4f(-1, -1, -1, 1), //1
    Vec4f(1, -1, -1, 1),  //2
    Vec4f(1, -1, 1, 1),   //3
    Vec4f(-1, 1, 1, 1),   //4
    Vec4f(1, 1, 1, 1),    //5
    Vec4f(1, 1, -1, 1),   //6
    Vec4f(-1, 1, -1, 1),  //7
  };

  // transform to world space

  GLMatrixf camWorldToClip = projection_ * modelView_;

  GLMatrixf camClipToWorld(camWorldToClip);
  camClipToWorld.invert();

  for (int i= 0; i < 8; ++i)
  {
    Vec4f posWS = camClipToWorld * posCS[i];
    posWS /= posWS[3];
    vboData_[i] = posWS;
  }

  // camera position in world space
  Vec4f camOriginWS(modelViewInv_(0, 3),
    modelViewInv_(1, 3),
    modelViewInv_(2, 3),
    1.0f);

  vboData_[8] = camOriginWS;


  // make axis length relative to frustum size in world space
  Vec3f rightVec = OpenMesh::vector_cast<Vec3f, Vec4f>(vboData_[3] - vboData_[0]);
  Vec3f upVec = OpenMesh::vector_cast<Vec3f, Vec4f>(vboData_[7] - vboData_[1]);
  axis_length_ = std::min(rightVec.norm(), upVec.norm()) * 0.015f;

  // minimum axis length
  axis_length_ = std::max(axis_length_, 0.05f);
}



//=============================================================================
} // namespace SceneGraph
} // namespace ACG
//=============================================================================
