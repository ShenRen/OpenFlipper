/*===========================================================================*\
 *                                                                           *
 *                              OpenFlipper                                  *
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
\*===========================================================================*/

/*===========================================================================*\
 *                                                                           *
 *   $Revision$                                                       *
 *   $Author$                                                      *
 *   $Date$                   *
 *                                                                           *
\*===========================================================================*/






//== INCLUDES =================================================================

#include "GCodeNode_renderer.hh"

#include "GCodeNode.hh"
#include <ACG/GL/gl.hh>
#include <ACG/Utils/VSToolsT.hh>
#include <vector>
#include <OpenMesh/Core/Utils/vector_cast.hh>

#ifdef _WIN32
#include <cstdint>
#else
#include <stdint.h>
#endif

//== NAMESPACES ===============================================================

namespace ACG {
namespace SceneGraph {

//== IMPLEMENTATION ==========================================================

/// Constructor
GCodeNode::GCodeNode(const GCode::Shared& _gc, BaseNode* _parent, std::string _name) :
        BaseNode(_parent, _name),
        gcode_(_gc),
        vbo_(0),
        updateVBO_(true),
        sphere_(0)

{
    Ultimaker::GCodeNode_renderer* renderer = new Ultimaker::GCodeNode_renderer();
    renderer->set_gcode(_gc);

    renderer_ = renderer;


  /*
  DrawModes::DrawMode MODE_COLOR;
  DrawModes::DrawMode MODE_HEAT;
  DrawModes::DrawMode MODE_SPEED;
  DrawModes::DrawMode MODE_TYPE;
*/

  MODE_COLOR = DrawModes::DrawMode(ACG::SceneGraph::DrawModes::addDrawMode("Color",
                                                                           ACG::SceneGraph::DrawModes::DrawModeProperties(ACG::SceneGraph::DrawModes::PRIMITIVE_POLYGON,
                                                                           ACG::SceneGraph::DrawModes::LIGHTSTAGE_SMOOTH,
                                                                           ACG::SceneGraph::DrawModes::NORMAL_PER_VERTEX)));

  MODE_HEAT = DrawModes::DrawMode(ACG::SceneGraph::DrawModes::addDrawMode("Heat",
                                                                           ACG::SceneGraph::DrawModes::DrawModeProperties(ACG::SceneGraph::DrawModes::PRIMITIVE_POLYGON,
                                                                           ACG::SceneGraph::DrawModes::LIGHTSTAGE_SMOOTH,
                                                                           ACG::SceneGraph::DrawModes::NORMAL_PER_VERTEX)));

  MODE_SPEED = DrawModes::DrawMode(ACG::SceneGraph::DrawModes::addDrawMode("Speed",
                                                                           ACG::SceneGraph::DrawModes::DrawModeProperties(ACG::SceneGraph::DrawModes::PRIMITIVE_POLYGON,
                                                                           ACG::SceneGraph::DrawModes::LIGHTSTAGE_SMOOTH,
                                                                           ACG::SceneGraph::DrawModes::NORMAL_PER_VERTEX)));

  MODE_TYPE = DrawModes::DrawMode(ACG::SceneGraph::DrawModes::addDrawMode("Edge Type",
                                                                           ACG::SceneGraph::DrawModes::DrawModeProperties(ACG::SceneGraph::DrawModes::PRIMITIVE_POLYGON,
                                                                           ACG::SceneGraph::DrawModes::LIGHTSTAGE_SMOOTH,
                                                                           ACG::SceneGraph::DrawModes::NORMAL_PER_VERTEX)));

  // Initial default draw mode
  drawMode(MODE_COLOR);
}

GCodeNode::~GCodeNode(){
    Ultimaker::GCodeNode_renderer* renderer = (Ultimaker::GCodeNode_renderer*)renderer_;
    delete renderer;
    renderer_ = 0;
}

//----------------------------------------------------------------------------

void GCodeNode::boundingBox(Vec3d& _bbMin, Vec3d& _bbMax)
{
    ptr::shared_ptr<PolyLine> polyline = gcode_->polyline();

    if(polyline)
    {
      int n = polyline->n_vertices();
        for (int i = 0; i < n; ++i)
        {
          if (int(polyline->edge_scalar(i)) != 2)
          {
            ACG::Vec3d pos = polyline->point(i);
            // skip empty segments
            if (i < n - 1)
            {
              ACG::Vec3d v = polyline->point(i + 1) - pos;
              if (v.sqrnorm() < 1e-8)
                continue;
            }
            _bbMin.minimize(pos);
            _bbMax.maximize(pos);
          }
        }
    }
}


//----------------------------------------------------------------------------


DrawModes::DrawMode GCodeNode::availableDrawModes() const
{
  return (MODE_COLOR | MODE_HEAT | MODE_SPEED | MODE_TYPE);
}


//----------------------------------------------------------------------------

void GCodeNode::draw(GLState& _state, const DrawModes::DrawMode& _drawMode)
{
    Ultimaker::GCodeNode_renderer* renderer = (Ultimaker::GCodeNode_renderer*)renderer_;

    Ultimaker::GCodeNode_renderer::DrawMode drawmode = Ultimaker::GCodeNode_renderer::Color;

    ACG::Vec4f color = _state.diffuse_color();

    if (_drawMode & MODE_COLOR)
    {
        drawmode = Ultimaker::GCodeNode_renderer::Color;

    }else if (_drawMode & MODE_HEAT)
    {
        drawmode = Ultimaker::GCodeNode_renderer::Heat;
    }else if (_drawMode & MODE_SPEED)
    {
        drawmode = Ultimaker::GCodeNode_renderer::Speed;
    }else if (_drawMode & MODE_TYPE)
    {
        drawmode = Ultimaker::GCodeNode_renderer::Type;
    }

    renderer->render(_state, _state.projection(), _state.modelview(), drawmode, distance_, color);
}

void GCodeNode::update()
{
    updateVBO_ = true;
    Ultimaker::GCodeNode_renderer* renderer = (Ultimaker::GCodeNode_renderer*)renderer_;
    renderer->set_gcode(gcode_);
}

//----------------------------------------------------------------------------

void GCodeNode::pick(GLState& _state, PickTarget _target)
{
    // TODO
  if (  !gcode_ )
    return;

  if ( updateVBO_ )
      updateVBO();

  ACG::GLState::bindBuffer(GL_ARRAY_BUFFER_ARB, vbo_);
  ACG::GLState::vertexPointer(3, GL_FLOAT, vertexDecl_.getVertexStride(), 0);
  ACG::GLState::enableClientState(GL_VERTEX_ARRAY);

  _state.pick_set_maximum(2);

  glDepthRange(0.0, 0.999999);


  float point_size_old = _state.point_size();
  glPointSize(point_size_old+3.0f);
  if ((_target == PICK_VERTEX || _target == PICK_ANYTHING))
  {
      _state.pick_set_name(0);

      glDrawArrays(GL_POINTS, offset_.first, offset_.second);
  }
  glPointSize(point_size_old);

  float line_width_old = _state.line_width();
  glLineWidth(line_width_old+3.0f);
  // draw line segments
  if ((_target == PICK_EDGE || _target == PICK_ANYTHING)) {

      _state.pick_set_name(1);

      glDrawArrays(GL_LINE_STRIP, offset_.first, offset_.second);
  }
  glLineWidth(line_width_old);
  glDepthRange(0.0, 1.0);

  ACG::GLState::bindBuffer(GL_ARRAY_BUFFER_ARB, 0);
  ACG::GLState::disableClientState(GL_VERTEX_ARRAY);

}

//----------------------------------------------------------------------------

void GCodeNode::updateVBO() {
    if(!gcode_){
        return;
    }

    //std::pair<ptr::shared_ptr<PolyLine>, PolyLine::CustomPropertyHandle> line = gcode_->create_polyline();
    ptr::shared_ptr<PolyLine> polyline = gcode_->polyline();

    offset_.first = 0;
    offset_.second = polyline->n_vertices();

    // Update the vertex declaration based on the input data:
    vertexDecl_.clear();

    // We always output vertex positions
    vertexDecl_.addElement(GL_FLOAT, 3, ACG::VERTEX_USAGE_POSITION);
    vertexDecl_.addElement(GL_UNSIGNED_BYTE, 4, ACG::VERTEX_USAGE_COLOR);

    // create vbo if it does not exist
    if (!vbo_)
        GLState::genBuffersARB(1, &vbo_);

    // size in bytes of vbo,  create additional vertex for closed loop indexing
    unsigned int bufferSize = vertexDecl_.getVertexStride() * offset_.second;

    // Create the required array
    char* vboData_ = new char[bufferSize];



    if(polyline && polyline->n_vertices() > 0){
        for (unsigned int  i = 0 ; i < polyline->n_vertices(); ++i) {
            writeVertex(polyline.get(), i, vboData_ + (offset_.first + i) * vertexDecl_.getVertexStride());
        }

        // First point is added to the end for a closed loop
       // writeVertex(polyline, 0, vboData_ + (offset_.first+polyline->n_vertices()) * vertexDecl_.getVertexStride());
    }

    // Move data to the buffer in gpu memory
    GLState::bindBufferARB(GL_ARRAY_BUFFER_ARB, vbo_);
    GLState::bufferDataARB(GL_ARRAY_BUFFER_ARB, bufferSize , vboData_ , GL_STATIC_DRAW_ARB);
    GLState::bindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

    // Remove the local storage
    delete[] vboData_;

    // Update done.
    updateVBO_ = false;
}


//----------------------------------------------------------------------------

void GCodeNode::writeVertex(PolyLine* _polyline, unsigned int _vertex, void* _dst) {

  ACG::Vec3f& pos = *((ACG::Vec3f*)_dst);
  ACG::Vec4uc& color = *((ACG::Vec4uc*)(&pos + 1));

  pos = OpenMesh::vector_cast<ACG::Vec3f>(_polyline->point(_vertex));
  color = ACG::Vec4uc(255, 0, 0, 255);//polyline_collection_.color(_polyline->edge_scalar((int(_vertex)-1+int(_polyline->n_vertices()))%int(_polyline->n_vertices())));
}

//----------------------------------------------------------------------------

void GCodeNode::getRenderObjects(ACG::IRenderer* _renderer, ACG::GLState&  _state , const ACG::SceneGraph::DrawModes::DrawMode&  _drawMode , const ACG::SceneGraph::Material* _mat) {

  Ultimaker::GCodeNode_renderer* renderer = (Ultimaker::GCodeNode_renderer*)renderer_;

  Ultimaker::GCodeNode_renderer::DrawMode drawmode = Ultimaker::GCodeNode_renderer::Color;

  ACG::Vec4f color = _state.diffuse_color();

  if (_drawMode & MODE_COLOR)
  {
    drawmode = Ultimaker::GCodeNode_renderer::Color;

  }
  else if (_drawMode & MODE_HEAT)
  {
    drawmode = Ultimaker::GCodeNode_renderer::Heat;
  }
  else if (_drawMode & MODE_SPEED)
  {
    drawmode = Ultimaker::GCodeNode_renderer::Speed;
  }
  else if (_drawMode & MODE_TYPE)
  {
    drawmode = Ultimaker::GCodeNode_renderer::Type;
  }

  renderer->createRenderObjects(_renderer, _state, drawmode, distance_, color);
  // init base render object
 /* ACG::RenderObject ro;

  _state.enable(GL_COLOR_MATERIAL);
  _state.enable(GL_LIGHTING);
  ro.initFromState(&_state);

  ro.setMaterial(_mat);

  // draw after scene-meshes
  ro.priority = 1;

  // Update the vbo only if required.
  if ( updateVBO_ )
    updateVBO();

  // Set to the right vbo
  ro.vertexBuffer = vbo_;

  // decl must be static or member,  renderer does not make a copy
  ro.vertexDecl = &vertexDecl_;

  // Set style
  ro.debugName = "PolyLineCollection";
  ro.blending = false;
  ro.depthTest = true;

  // Default color
  ACG::Vec4f defaultColor   = _state.ambient_color()  + _state.diffuse_color();
  ACG::Vec4f selectionColor = ACG::Vec4f(1.0,0.0,0.0,1.0);

  // Viewport size
  ACG::Vec2f screenSize(float(_state.viewport_width()), float(_state.viewport_height()));

  for (unsigned int i = 0; i < _drawMode.getNumLayers(); ++i) {
    ACG::SceneGraph::Material localMaterial = *_mat;

    const ACG::SceneGraph::DrawModes::DrawModeProperties* props = _drawMode.getLayer(i);

    ro.setupShaderGenFromDrawmode(props);
    ro.shaderDesc.shadeMode = SG_SHADE_UNLIT;

    //---------------------------------------------------
    // No lighting!
    // Therefore we need some emissive color
    //---------------------------------------------------
    localMaterial.baseColor(defaultColor);
    ro.setMaterial(&localMaterial);


    if(props->primitive() == ACG::SceneGraph::DrawModes::PRIMITIVE_POINT){
        // Render all vertices which are selected via an index buffer
        ro.debugName = "polyline.Points.selected";
        localMaterial.baseColor(selectionColor);
        ro.setMaterial(&localMaterial);

        // Point Size geometry shader
        ro.setupPointRendering(_mat->pointSize(), screenSize);


        // Render all vertices (ignore selection here!)
        ro.debugName = "polylinecollection.Points";
        localMaterial.baseColor(defaultColor);
        ro.setMaterial(&localMaterial);

        PolyLine* polyline = gcode_->line();
        if(polyline && polyline->n_vertices() > 0){
            ro.glDrawArrays(GL_POINTS, offset_.first, offset_.second-1);
        }

        // Point Size geometry shader
        ro.setupPointRendering(_mat->pointSize(), screenSize);

        // apply user settings
        applyRenderObjectSettings(props->primitive(), &ro);

        _renderer->addRenderObject(&ro);
    }else if(props->primitive() == ACG::SceneGraph::DrawModes::PRIMITIVE_WIREFRAME){
        // Render all edges which are selected via an index buffer
        ro.debugName = "polyline.Wireframe.selected";
        localMaterial.baseColor(selectionColor);
        ro.setMaterial(&localMaterial);

        // Line Width geometry shader
        ro.setupLineRendering(_state.line_width(), screenSize);



        ro.debugName = "polylinecollection.Wireframe";
        localMaterial.baseColor(defaultColor);
        ro.setMaterial(&localMaterial);
        // The first point is mapped to an additional last point in buffer, so we can
        // just Render one point more to get a closed line

        //int offset = 0;
        PolyLine* polyline = gcode_->line();
        if(polyline && polyline->n_vertices() > 0){
            if ( polyline->is_closed() ){
                ro.glDrawArrays(GL_LINE_STRIP, offset_.first, offset_.second);
            }else{
                ro.glDrawArrays(GL_LINE_STRIP, offset_.first, offset_.second-1);
            }
        }

        // Line Width geometry shader
        ro.setupLineRendering(_state.line_width(), screenSize);

        // apply user settings
        applyRenderObjectSettings(props->primitive(), &ro);

        _renderer->addRenderObject(&ro);
    }
  }*/

}

//=============================================================================
} // namespace SceneGraph
} // namespace ACG
//=============================================================================

