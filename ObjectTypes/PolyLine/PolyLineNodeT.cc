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




//=============================================================================
//
//  CLASS PolyLineNodeT - IMPLEMENTATION
//
//=============================================================================

#define ACG_POLYLINENODET_C

//== INCLUDES =================================================================

#include "PolyLineNodeT.hh"
#include <ACG/GL/gl.hh>
#include <ACG/GL/ShaderCache.hh>
#include <ACG/Utils/VSToolsT.hh>
#include <vector>

//== NAMESPACES ===============================================================

namespace ACG {
namespace SceneGraph {

//== IMPLEMENTATION ==========================================================

/// Constructor
template <class PolyLine>
PolyLineNodeT<PolyLine>::PolyLineNodeT(PolyLine& _pl, BaseNode* _parent, std::string _name) :
        BaseNode(_parent, _name),
        polyline_(_pl),
        updateVBO_(true),
        sphere_(0)

{

  // Initialize our local draw mode references
  POINTS_SPHERES = DrawModes::DrawMode(ACG::SceneGraph::DrawModes::addDrawMode("Points (as Spheres)",
                                                                               ACG::SceneGraph::DrawModes::DrawModeProperties(ACG::SceneGraph::DrawModes::PRIMITIVE_POLYGON,
                                                                               ACG::SceneGraph::DrawModes::LIGHTSTAGE_SMOOTH,
                                                                               ACG::SceneGraph::DrawModes::NORMAL_PER_VERTEX)));

  POINTS_SPHERES_SCREEN = DrawModes::DrawMode(ACG::SceneGraph::DrawModes::addDrawMode("Points (as Spheres, constant screen size)",
                                                                               ACG::SceneGraph::DrawModes::DrawModeProperties(ACG::SceneGraph::DrawModes::PRIMITIVE_POLYGON,
                                                                               ACG::SceneGraph::DrawModes::LIGHTSTAGE_SMOOTH,
                                                                               ACG::SceneGraph::DrawModes::NORMAL_PER_VERTEX)));

  // Initial default draw mode
  drawMode(DrawModes::WIREFRAME | DrawModes::POINTS );
}

//----------------------------------------------------------------------------

template <class PolyLine>
PolyLineNodeT<PolyLine>::~PolyLineNodeT()
{
  delete sphere_;
}

//----------------------------------------------------------------------------

template <class PolyLine>
void
PolyLineNodeT<PolyLine>::
boundingBox(Vec3d& _bbMin, Vec3d& _bbMax)
{
  for (unsigned int i=0; i< polyline_.n_vertices(); ++i)
  {
    _bbMin.minimize(polyline_.point(i));
    _bbMax.maximize(polyline_.point(i));
  }
}


//----------------------------------------------------------------------------


template <class PolyLine>
DrawModes::DrawMode
PolyLineNodeT<PolyLine>::
availableDrawModes() const
{
  return (DrawModes::WIREFRAME | DrawModes::POINTS | DrawModes::POINTS_COLORED | POINTS_SPHERES | POINTS_SPHERES_SCREEN | DrawModes::EDGES_COLORED);
}


//----------------------------------------------------------------------------


template <class PolyLine>
void
PolyLineNodeT<PolyLine>::
draw(GLState& _state, const DrawModes::DrawMode& _drawMode)
{
  // Block if we do not have any vertices
  if ( polyline_.n_vertices() == 0 ) 
    return;

  // Update the vbo only if required.
  if ( updateVBO_ )
    updateVBO();

  ACG::GLState::disable(GL_LIGHTING);
  ACG::GLState::disable(GL_TEXTURE_2D);
  
  // Bind the vertex array
  vbo_.bind();

  ACG::Vec4f color = _state.ambient_color()  + _state.diffuse_color();

  // draw points
  if (_drawMode & DrawModes::POINTS || _drawMode & DrawModes::POINTS_COLORED)
  {
    vertexDecl_.activateFixedFunction();

    // draw selection
    if( polyline_.vertex_selections_available() && !selectedVertexIndexBuffer_.empty())
    {
      // save old values
      float point_size_old = _state.point_size();
      _state.set_color( Vec4f(1,0,0,1) );
      _state.set_point_size(point_size_old+4);

      glDrawElements(GL_POINTS, selectedVertexIndexBuffer_.size(), GL_UNSIGNED_INT, &(selectedVertexIndexBuffer_[0]));

      _state.set_point_size(point_size_old);
    }

    _state.set_color( color );


    if (_drawMode & DrawModes::POINTS_COLORED)
    {
      vertexDecl_.deactivateFixedFunction();
      vertexDeclVCol_.activateFixedFunction();
    }
    
    // Draw all vertices (don't care about selection)
    glDrawArrays(GL_POINTS, 0, polyline_.n_vertices());
    
    if (_drawMode & DrawModes::POINTS_COLORED)
      vertexDeclVCol_.deactivateFixedFunction();
    else
      vertexDecl_.deactivateFixedFunction();
  }

  // draw line segments
  if (_drawMode & DrawModes::WIREFRAME) {
    vertexDecl_.activateFixedFunction();

    // draw selection
    if (polyline_.edge_selections_available() && !selectedEdgeIndexBuffer_.empty()) {
      // save old values
      float line_width_old = _state.line_width();
      _state.set_color(Vec4f(1, 0, 0, 1));
      _state.set_line_width(2 * line_width_old);

      glDrawElements(GL_LINES, selectedEdgeIndexBuffer_.size(), GL_UNSIGNED_INT, &(selectedEdgeIndexBuffer_[0]));

      _state.set_line_width(line_width_old);
    }

    _state.set_color( color );

    if ( polyline_.is_closed() )
      glDrawArrays(GL_LINE_STRIP, 0, polyline_.n_vertices() + 1);
    else
      glDrawArrays(GL_LINE_STRIP, 0, polyline_.n_vertices());

    vertexDecl_.deactivateFixedFunction();
  }


  if (_drawMode & DrawModes::EDGES_COLORED) {
    vertexDecl_.activateFixedFunction();

    // draw selection
    if (polyline_.edge_selections_available() && !selectedEdgeIndexBuffer_.empty()) {
      // save old values
      float line_width_old = _state.line_width();
      _state.set_color(Vec4f(1, 0, 0, 1));
      _state.set_line_width(2 * line_width_old);

      glDrawElements(GL_LINES, selectedEdgeIndexBuffer_.size(), GL_UNSIGNED_INT, &(selectedEdgeIndexBuffer_[0]));

      _state.set_line_width(line_width_old);
    }

    vertexDecl_.deactivateFixedFunction();

    _state.set_color(color);


    vertexDeclECol_.activateFixedFunction();
    if (polyline_.is_closed())
      glDrawArrays(GL_LINE_STRIP, 0, polyline_.n_vertices() + 1);
    else
      glDrawArrays(GL_LINE_STRIP, 0, polyline_.n_vertices());

    vertexDeclECol_.deactivateFixedFunction();
  }


  // draw normals
  if (polyline_.vertex_normals_available()) {
    double avg_len = polyline_.n_edges() > 0 ? (polyline_.length() / polyline_.n_edges() * 0.75) : 0;
    std::vector<Point> ps;
    for (unsigned int i = 0; i < polyline_.n_vertices(); ++i) {
      ps.push_back(polyline_.point(i));
      ps.push_back(polyline_.point(i) + polyline_.vertex_normal(i) * avg_len);
      if (polyline_.vertex_binormals_available())
        ps.push_back(polyline_.point(i) + polyline_.vertex_binormal(i) * avg_len);
    }

    // Disable the big buffer and switch to in memory buffer
    vbo_.unbind();
    ACG::GLState::vertexPointer(&ps[0]);

    float line_width_old = _state.line_width();
    _state.set_color( Vec4f(0.8f, 0.f, 0.f, 1.f) );
    _state.set_line_width(1);

    int stride = polyline_.vertex_binormals_available() ? 3 : 2;
    glBegin(GL_LINES);
    for (unsigned int i = 0; i < polyline_.n_vertices(); ++i) {
      glArrayElement(stride * i);
      glArrayElement(stride * i + 1);
    }
    glEnd();

    if (polyline_.vertex_binormals_available()) {
      _state.set_color( Vec4f(0.f, 0.f, 0.8f, 1.f) );
      glBegin(GL_LINES);
      for (unsigned int i = 0; i < polyline_.n_vertices(); ++i) {
        glArrayElement(stride * i);
        glArrayElement(stride * i + 2);
      }
      glEnd();
    }

    _state.set_line_width(line_width_old);
  }
  
  vbo_.unbind();

  //Disable the vertex array
  ACG::GLState::disableClientState(GL_VERTEX_ARRAY);

  // draw vertices as spheres, using the radius given in the polyline
  if (_drawMode & POINTS_SPHERES)
  {
    // create sphere if not yet done
    if(!sphere_)
      sphere_ = new GLSphere(10,10);

    if( polyline_.vertex_selections_available())
    {
      for(unsigned int i=0; i<polyline_.n_vertices(); ++i)
      {
        if(polyline_.vertex_selected(i))
          _state.set_color( Vec4f(1,0,0,1) );
        else
          _state.set_color( color );

        sphere_->draw(_state, _state.point_size(), (Vec3f)polyline_.point(i));
      }
    }
    else
    {
      _state.set_color( color );
      for(unsigned int i=0; i<polyline_.n_vertices(); ++i)
        sphere_->draw(_state, _state.point_size(), (Vec3f)polyline_.point(i));
    }
  }
  // draw vertices as spheres with constant size on screen
  if (_drawMode & POINTS_SPHERES_SCREEN)
  {
    // create sphere if not yet done
    if(!sphere_)
      sphere_ = new GLSphere(10,10);

    // precompute desired radius of projected sphere
    double r = 0.5*_state.point_size() / double(_state.viewport_height()) * 2.0 * tan(0.5*_state.fovy());

    if( polyline_.vertex_selections_available())
    {
      for(unsigned int i=0; i<polyline_.n_vertices(); ++i)
      {
        if(polyline_.vertex_selected(i))
          _state.set_color( Vec4f(1,0,0,1) );
        else
          _state.set_color( color );

        // compute radius in 3D
        const Vec3d p = (Vec3d)polyline_.point(i) - _state.eye();
        const double l = (p|_state.viewing_direction());
        sphere_->draw(_state, r*l, (Vec3f)polyline_.point(i));
      }
    }
    else
    {
      _state.set_color( color );
      for(unsigned int i=0; i<polyline_.n_vertices(); ++i)
      {
        // compute radius in 3D
        const Vec3d p = (Vec3d)polyline_.point(i) - _state.eye();
        const double l = (p|_state.viewing_direction());
        sphere_->draw(_state, r*l, (Vec3f)polyline_.point(i));
      }

    }
  }
}

//----------------------------------------------------------------------------


template <class PolyLine>
void
PolyLineNodeT<PolyLine>::
pick(GLState& _state, PickTarget _target)
{
  if (  polyline_.n_vertices() == 0 ) 
    return;
  
  // Bind the vertex array
  ACG::GLState::bindBuffer(GL_ARRAY_BUFFER_ARB, 0);
  ACG::GLState::vertexPointer( &(polyline_.points())[0] );   
  ACG::GLState::enableClientState(GL_VERTEX_ARRAY);
  
  unsigned int n_end = polyline_.n_edges()+1;
  if( !polyline_.is_closed()) --n_end;
  
  // (draw lines slightly in front of everything else)
  //disabled right now because of rendering artifacts.
  //This probably doesn't make sense anyways since the lines are drawn as cylinders and therefore have a width
  // glDepthRange(0.0,0.99)

  switch (_target)
  {
    case PICK_VERTEX:
    {
      _state.pick_set_maximum (polyline_.n_vertices());
      if (drawMode() & DrawModes::POINTS)
          pick_vertices( _state);

      if (drawMode() & POINTS_SPHERES)
          pick_spheres( _state);

      if (drawMode() & POINTS_SPHERES_SCREEN)
          pick_spheres_screen( _state);

      break;
    }

    case PICK_EDGE:
    {
      _state.pick_set_maximum (n_end);
      pick_edges(_state, 0);
      break;
    }

    case PICK_ANYTHING:
    {
      _state.pick_set_maximum (polyline_.n_vertices() + n_end);

      if (drawMode() & DrawModes::POINTS)
          pick_vertices( _state);

      if (drawMode() & POINTS_SPHERES)
          pick_spheres( _state);

      if (drawMode() & POINTS_SPHERES_SCREEN)
          pick_spheres_screen( _state);

      pick_edges( _state, polyline_.n_vertices());
      break;
    }

    default:
      break;
  }
   
  //see above
  // glDepthRange(0.0,1.0)
  
  //Disable the vertex array
  ACG::GLState::disableClientState(GL_VERTEX_ARRAY);
  
}


//----------------------------------------------------------------------------


template <class PolyLine>
void
PolyLineNodeT<PolyLine>::
pick_vertices( GLState& _state )
{
  if (!polyline_.n_vertices())
    return;

  float point_size_old = _state.point_size();
  glPointSize(18);

  glDepthRange(0.0, 0.999999);

  GLSL::Program* pickShader = ACG::ShaderCache::getInstance()->getProgram("Picking/pick_vertices_vs.glsl", "Picking/pick_vertices_fs.glsl", 0, false);

  if (pickShader && pickShader->isLinked())
  {
    // Update the vbo only if required.
    if (updateVBO_)
      updateVBO();

    // Bind the vertex array
    vbo_.bind();

    int pickOffsetIndex = int(_state.pick_current_index());

    vertexDecl_.activateShaderPipeline(pickShader);

    pickShader->use();

    ACG::GLMatrixf transform = _state.projection() * _state.modelview();

    pickShader->setUniform("mWVP", transform);
    pickShader->setUniform("pickVertexOffset", pickOffsetIndex);

    glDrawArrays(GL_POINTS, 0, polyline_.n_vertices());

    vertexDecl_.deactivateShaderPipeline(pickShader);
    pickShader->disable();


    vbo_.unbind();
  }
  else
  {
    for (unsigned int i = 0; i < polyline_.n_vertices(); ++i) {
      _state.pick_set_name(i);
      glBegin(GL_POINTS);
      glArrayElement(i);
      glEnd();
    }
  }

  glDepthRange(0.0, 1.0);
  

  
  glPointSize(point_size_old);
}


//----------------------------------------------------------------------------


template <class PolyLine>
void
PolyLineNodeT<PolyLine>::
pick_spheres( GLState& _state )
{
  if(!sphere_)
    sphere_ = new GLSphere(10,10);

  _state.pick_set_name(0);

  for(unsigned int i=0; i<polyline_.n_vertices(); ++i)
  {
    _state.pick_set_name (i);
    sphere_->draw(_state, _state.point_size(), (Vec3f)polyline_.point(i));
  }
}

//----------------------------------------------------------------------------


template <class PolyLine>
void
PolyLineNodeT<PolyLine>::
pick_spheres_screen( GLState& _state )
{
  if(!sphere_)
    sphere_ = new GLSphere(10,10);

  _state.pick_set_name(0);

  // precompute desired radius of projected sphere
  double r = 0.5*_state.point_size()/double(_state.viewport_height())*2.0*tan(0.5*_state.fovy());

  for(unsigned int i=0; i<polyline_.n_vertices(); ++i)
  {
    _state.pick_set_name (i);
    // compute radius in 3D
    const Vec3d p = (Vec3d)polyline_.point(i) - _state.eye();
    double l = (p|_state.viewing_direction());
    sphere_->draw(_state, r*l, (Vec3f)polyline_.point(i));

//    ToFix: _state does still not provide the near_plane in picking mode!!!
//    std::cerr << "radius in picking: " << r*l << std::endl;
  }
}


//----------------------------------------------------------------------------


template <class PolyLine>
void
PolyLineNodeT<PolyLine>::
pick_edges( GLState& _state, unsigned int _offset)
{
  // Check if we have any edges to draw ( % 0 causes division by zero on windows)
  if ( polyline_.n_edges() == 0 )
    return;
  
  glDepthRange(0.0, 0.999999);

  GLSL::Program* pickShader = ACG::ShaderCache::getInstance()->getProgram("Picking/vertex.glsl", "Picking/pick_vertices_fs2.glsl", 0, false);

  if (pickShader && pickShader->isLinked())
  {
    // Update the vbo only if required.
    if (updateVBO_)
      updateVBO();

    // Bind the vertex array
    vbo_.bind();

    int pickOffsetIndex = int(_state.pick_current_index());

    vertexDecl_.activateShaderPipeline(pickShader);

    pickShader->use();

    ACG::GLMatrixf transform = _state.projection() * _state.modelview();

    pickShader->setUniform("mWVP", transform);
    pickShader->setUniform("pickVertexOffset", pickOffsetIndex);

    int numIndices = polyline_.n_vertices() + (polyline_.is_closed() ? 1 : 0);
    glDrawArrays(GL_LINE_STRIP, 0, numIndices);

    vertexDecl_.deactivateShaderPipeline(pickShader);
    pickShader->disable();

    vbo_.unbind();
  }
  else
  {
    // save old values
    float line_width_old = _state.line_width();
    //  _state.set_line_width(2*line_width_old);
    _state.set_line_width(14);

    unsigned int n_end = polyline_.n_edges() + 1;
    if (!polyline_.is_closed()) --n_end;

    for (unsigned int i = 0; i < n_end; ++i) {
      _state.pick_set_name(i + _offset);
      glBegin(GL_LINES);
      glArrayElement(i     % polyline_.n_vertices());
      glArrayElement((i + 1) % polyline_.n_vertices());
      glEnd();
    }

    _state.set_line_width(line_width_old);
  }

  glDepthRange(0.0, 1.0);

}

//----------------------------------------------------------------------------

template <class PolyLine>
void 
PolyLineNodeT<PolyLine>::
setupVertexDeclaration(VertexDeclaration* _dst, int _colorSource) const {
  // Update the vertex declaration based on the input data:
  _dst->clear();


  // We always output vertex positions
  _dst->addElement(GL_FLOAT, 3, ACG::VERTEX_USAGE_POSITION);

  // current byte offset
  size_t curOffset = 12;

  // Use the normals if available
  if (polyline_.vertex_normals_available())
  {
    _dst->addElement(GL_FLOAT, 3, ACG::VERTEX_USAGE_NORMAL, curOffset);
    curOffset += 12;
  }

  // colors
  if (polyline_.vertex_colors_available())
  {
    if (_colorSource == 1)
      _dst->addElement(GL_UNSIGNED_BYTE, 4, ACG::VERTEX_USAGE_COLOR, curOffset);
    curOffset += 4;
  }

  if (polyline_.edge_colors_available())
  {
    if (_colorSource == 2)
      _dst->addElement(GL_UNSIGNED_BYTE, 4, ACG::VERTEX_USAGE_COLOR, curOffset);
    curOffset += 4;
  }


  // Add custom vertex elements to declaration
  for (size_t i = 0; i < customBuffers_.size(); ++i) {
    ACG::VertexElement tmp = customBuffers_[i].first;
    tmp.pointer_ = 0;
    tmp.usage_ = ACG::VERTEX_USAGE_SHADER_INPUT;
    tmp.setByteOffset(curOffset);
    _dst->addElement(&tmp);

    curOffset += VertexDeclaration::getElementSize(&tmp);
  }

  _dst->setVertexStride(curOffset);
}

//----------------------------------------------------------------------------

template <class PolyLine>
size_t
PolyLineNodeT<PolyLine>::
fillVertexBuffer(void *_buf, size_t _bufSize, bool _addLineStripEndVertex) {

  // register custom properties defined in polyline

  for (unsigned int i = 0; i < polyline_.get_num_custom_properties(); ++i) {

    typename PolyLine::CustomPropertyHandle proph = polyline_.enumerate_custom_property_handles(i);



    const void* propDataBuf = polyline_.get_custom_property_buffer(proph);

    typename std::map< typename PolyLine::CustomPropertyHandle, int >::iterator mapEntry = polylinePropMap_.find(proph);

    // insert newly defined properties
    if (mapEntry == polylinePropMap_.end()) {

      // setup element description
      ACG::VertexElement desc;

      unsigned int propSize = 0;
      if (polyline_.get_custom_property_shader_binding(proph, &propSize, &desc.shaderInputName_, &desc.type_)) {
        // assume aligned memory without byte padding
        desc.numElements_ = propSize / VertexDeclaration::getGLTypeSize(desc.type_);
        desc.pointer_ = 0;

        polylinePropMap_[proph] = addCustomBuffer(desc, propDataBuf);
      }
    }
    else // otherwise update pointer of property data buffer
      setCustomBuffer(mapEntry->second, propDataBuf);
  }


  // Update vertex declarations
  setupVertexDeclaration(&vertexDecl_, 0);
  setupVertexDeclaration(&vertexDeclVCol_, 1);
  setupVertexDeclaration(&vertexDeclECol_, 2);


  // fill buffer

  const unsigned int stride = vertexDecl_.getVertexStride();

  char* data = static_cast<char*>(_buf);
  size_t bytesWritten = 0;

  for (unsigned int  i = 0 ; i < polyline_.n_vertices() && bytesWritten + stride < _bufSize; ++i) {
    writeVertex(i, data + i * stride);
    bytesWritten += stride;
  }

  if (_addLineStripEndVertex && bytesWritten + stride < _bufSize) {
    // First point is added to the end for a closed loop
    writeVertex(0, data + polyline_.n_vertices() * stride);
    bytesWritten += stride;
  }

  return bytesWritten;
}

//----------------------------------------------------------------------------

template <class PolyLine>
void
PolyLineNodeT<PolyLine>::
updateVBO() {

  setupVertexDeclaration(&vertexDecl_, 0);

  const unsigned int stride = vertexDecl_.getVertexStride();

  // size in bytes of vbo,  create additional vertex for closed loop indexing
  size_t bufferSize = stride * (polyline_.n_vertices() + 1);

  // Create the required array
  std::vector<char> vboData(bufferSize);

  if (bufferSize > 0) {
    size_t bytesWritten = fillVertexBuffer(&vboData[0], bufferSize, true);

    if (bytesWritten != bufferSize)
      std::cerr << "PolyLineNode: fill vertex buffer only wrote " << bytesWritten << " bytes instead of expected " << bufferSize << " bytes" << std::endl;

    // Move data to the buffer in gpu memory
    vbo_.upload(bufferSize, &vboData[0], GL_STATIC_DRAW);
    vbo_.unbind();
  }

  // Index buffer for selected vertices
  selectedVertexIndexBuffer_.clear();

  // Index buffer for selected vertices
  selectedEdgeIndexBuffer_.clear();

  for (unsigned int  i = 0 ; i < polyline_.n_vertices(); ++i) {

    // Create an ibo in system memory for vertex selection
    if ( polyline_.vertex_selections_available() && polyline_.vertex_selected(i) )
      selectedVertexIndexBuffer_.push_back(i);

    // Create an ibo in system memory for edge selection
    if ( polyline_.edge_selections_available() && polyline_.edge_selected(i) ) {
      selectedEdgeIndexBuffer_.push_back(i);
      selectedEdgeIndexBuffer_.push_back( (i + 1) % polyline_.n_vertices() );
    }

  }

  // Update done.
  updateVBO_ = false;
}

//----------------------------------------------------------------------------

template <class PolyLine>
void 
PolyLineNodeT<PolyLine>::
writeVertexColor(unsigned int _vertex, bool _colorSourceVertex, void* _dst) const
{
  const VertexDeclaration* declToUse = _colorSourceVertex ? &vertexDeclVCol_ : &vertexDeclECol_;

  unsigned int byteOffset = declToUse->findElementByUsage(VERTEX_USAGE_COLOR)->getByteOffset();
  unsigned char* ucdata = ((unsigned char*)_dst) + byteOffset;

  Point col;
  if (_colorSourceVertex)
    col = polyline_.vertex_color(_vertex); // per vertex
  else
  {
    // edge colors
    // use the 2nd vertex of each edge as the provoking vertex
    int edgeID = (_vertex + polyline_.n_edges() - 1) % polyline_.n_edges();
    col = polyline_.edge_color(edgeID);
  }

  // rgb
  for (int i = 0; i < 3; ++i)
  {
    // convert to normalized ubyte
    int ival = int(col[i] * 255.0);
    ival = std::min(std::max(ival, 0), 255);
    ucdata[i] = ival;
  }
  ucdata[3] = 0xff; // alpha
}

//----------------------------------------------------------------------------

template <class PolyLine>
void
PolyLineNodeT<PolyLine>::
writeVertex(unsigned int _vertex, void* _dst) {

  // position and normal in float
  float* fdata = (float*)_dst;

  // Copy from internal storage to VBO in CPU memory
  for ( unsigned int j = 0 ; j < 3 ; ++j)
    *(fdata++) = polyline_.point(_vertex)[j];

  // Also write normal into buffer if available
  if ( polyline_.vertex_normals_available()  )
    for ( unsigned int j = 0 ; j < 3 ; ++j)
      *(fdata++) = polyline_.vertex_normal(_vertex)[j];

  if (polyline_.vertex_colors_available())
    writeVertexColor(_vertex, true, _dst);

  if (polyline_.edge_colors_available())
    writeVertexColor(_vertex, false, _dst);

  int customElementOffset = vertexDeclVCol_.findElementIdByUsage(VERTEX_USAGE_SHADER_INPUT);

  if (customElementOffset >= 0)
  {
    // copy custom data byte-wise
    for (unsigned int i = 0; i < customBuffers_.size(); ++i) {

      // element in custom input buffer
      const ACG::VertexElement* veInput = &customBuffers_[i].first;
      unsigned int elementInputStride = veInput->getByteOffset();
      unsigned int elementSize = ACG::VertexDeclaration::getElementSize(veInput);

      if (!elementInputStride)
        elementInputStride = elementSize;

      // element in vertex buffer
      const ACG::VertexElement* ve = vertexDeclVCol_.getElement(i + static_cast<unsigned int>(customElementOffset));

      const char* src = (const char*)customBuffers_[i].second;

      memcpy((char*)_dst + ve->getByteOffset(), src + elementInputStride * _vertex, elementSize);
    }
  }
}

//----------------------------------------------------------------------------

template <class PolyLine>
void
PolyLineNodeT<PolyLine>::
getRenderObjects(ACG::IRenderer* _renderer, ACG::GLState&  _state , const ACG::SceneGraph::DrawModes::DrawMode&  _drawMode , const ACG::SceneGraph::Material* _mat) {

  // Block if we do not have any vertices
  if ( polyline_.n_vertices() == 0 )
    return;

  // init base render object
  ACG::RenderObject ro;

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
  ro.vertexBuffer = vbo_.id();

  // Set style
  ro.debugName = "PolyLine";
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
    ro.vertexDecl = &vertexDecl_;

    //---------------------------------------------------
    // No lighting!
    // Therefore we need some emissive color
    //---------------------------------------------------
    localMaterial.baseColor(defaultColor);
    ro.setMaterial(&localMaterial);

  
    switch (props->primitive()) {

      case ACG::SceneGraph::DrawModes::PRIMITIVE_POINT:

        // Render all vertices which are selected via an index buffer
        ro.debugName = "polyline.Points.selected";
        localMaterial.baseColor(selectionColor);
        ro.setMaterial(&localMaterial);

        // Point Size geometry shader
        ro.setupPointRendering(_mat->pointSize(), screenSize);
        
        // selection without colors
        ro.shaderDesc.vertexColors = false;

        if (!selectedVertexIndexBuffer_.empty())
        {
          ro.glDrawElements(GL_POINTS, selectedVertexIndexBuffer_.size(), GL_UNSIGNED_INT, &(selectedVertexIndexBuffer_[0]));
          // apply user settings
          applyRenderObjectSettings(props->primitive(), &ro);

          _renderer->addRenderObject(&ro);
        }

        // Render all vertices (ignore selection here!)
        ro.debugName = "polyline.Points";
        localMaterial.baseColor(defaultColor);
        ro.setMaterial(&localMaterial);
        ro.glDrawArrays(GL_POINTS, 0, polyline_.n_vertices());

        if (props->colored() && polyline_.vertex_colors_available())
        {
          ro.vertexDecl = &vertexDeclVCol_;
          ro.shaderDesc.vertexColors = true;
        }


        // Point Size geometry shader
        ro.setupPointRendering(_mat->pointSize(), screenSize);

        // apply user settings
        applyRenderObjectSettings(props->primitive(), &ro);

        _renderer->addRenderObject(&ro);

        break;

      case ACG::SceneGraph::DrawModes::PRIMITIVE_WIREFRAME:
      case ACG::SceneGraph::DrawModes::PRIMITIVE_EDGE:

        // Render all edges which are selected via an index buffer
        ro.debugName = "polyline.Wireframe.selected";
        localMaterial.baseColor(selectionColor);
        ro.setMaterial(&localMaterial);

        // Line Width geometry shader
        ro.setupLineRendering(_state.line_width(), screenSize);

        // selection without colors
        ro.shaderDesc.vertexColors = false;

        if (!selectedEdgeIndexBuffer_.empty())
        {
          ro.glDrawElements(GL_LINES, selectedEdgeIndexBuffer_.size(), GL_UNSIGNED_INT, &(selectedEdgeIndexBuffer_[0]));

          // apply user settings
          applyRenderObjectSettings(props->primitive(), &ro);

          _renderer->addRenderObject(&ro);
        }

        ro.debugName = "polyline.Wireframe";
        localMaterial.baseColor(defaultColor);
        ro.setMaterial(&localMaterial);
        // The first point is mapped to an additional last point in buffer, so we can
        // just Render one point more to get a closed line
        if ( polyline_.is_closed() )
          ro.glDrawArrays(GL_LINE_STRIP, 0, polyline_.n_vertices() + 1);
        else
          ro.glDrawArrays(GL_LINE_STRIP, 0, polyline_.n_vertices());

        if (props->colored() && polyline_.edge_colors_available())
        {
          ro.vertexDecl = &vertexDeclECol_;
          ro.shaderDesc.vertexColors = true;
        }

        // Line Width geometry shader
        ro.setupLineRendering(_state.line_width(), screenSize);

        // apply user settings
        applyRenderObjectSettings(props->primitive(), &ro);

        _renderer->addRenderObject(&ro);

        break;


      case ACG::SceneGraph::DrawModes::PRIMITIVE_POLYGON:
        {
          // create sphere object for each vertex
          // potential optimization: create only one render object and use instancing 

          // use world space radius or screen space point size?
          bool screenScale = _drawMode & POINTS_SPHERES_SCREEN;

          // clear shaders used by thick line / point drawing
          ro.shaderDesc.vertexTemplateFile.clear();
          ro.shaderDesc.geometryTemplateFile.clear();
          ro.shaderDesc.fragmentTemplateFile.clear();

          // create sphere if not yet done
          if (!sphere_)
            sphere_ = new GLSphere(10, 10);

          // precompute desired radius of projected sphere
          double r = 1.0;
          if (screenScale)
            r = 0.5*_state.point_size() / double(_state.viewport_height())*2.0*tan(0.5*_state.fovy());

          // get eye position and view direction in world space
          Vec3d eyePos = _state.eye();
          Vec3d viewDir = _state.viewing_direction();

          // render-objects for the selected points with selection color
          if (polyline_.vertex_selections_available())
          {
            ro.debugName = "polyline.Sphere.selected";
            localMaterial.baseColor(selectionColor);
            ro.setMaterial(&localMaterial);

            for (unsigned int i = 0; i < polyline_.n_vertices(); ++i)
            {
              if (polyline_.vertex_selected(i))
              {
                double radius = _state.point_size();
                if (screenScale)
                {
                  // compute radius in 3D
                  const Vec3d p = (Vec3d)polyline_.point(i) - eyePos;
                  radius = (p | viewDir) * r;
                }

                sphere_->addToRenderer(_renderer, &ro, radius, (Vec3f)polyline_.point(i));
              }
            }
          }

          // unselected points with default color
          ro.debugName = "polyline.Sphere";
          localMaterial.baseColor(defaultColor);
          ro.setMaterial(&localMaterial);

          for (unsigned int i = 0; i < polyline_.n_vertices(); ++i)
          {
            if (!polyline_.vertex_selections_available() || !polyline_.vertex_selected(i))
            {
              double radius = _state.point_size();
              if (screenScale)
              {
                // compute radius in 3D
                const Vec3d p = (Vec3d)polyline_.point(i) - eyePos;
                radius = (p | viewDir) * r;
              }

              sphere_->addToRenderer(_renderer, &ro, radius, (Vec3f)polyline_.point(i));
            }
          }
        } break;


      default:
        break;
    }

  }

}

//----------------------------------------------------------------------------

template <class PolyLine>
int
PolyLineNodeT<PolyLine>::
addCustomBuffer( const ACG::VertexElement& _desc, const void* _buffer) {

  if (_buffer) {
    customBuffers_.push_back( std::pair<ACG::VertexElement, const void*>(_desc, _buffer) );
    update();

    return int(customBuffers_.size()-1);
  }
  else
  {
    std::cerr << "PolyLineNodeT::addCustomBuffer - null pointer buffer" << std::endl;
    return -1;
  }
}

//----------------------------------------------------------------------------

template <class PolyLine>
void
PolyLineNodeT<PolyLine>::
setCustomBuffer( int _id, const void* _buffer) {

  customBuffers_[_id].second = _buffer;
  update();
}

//=============================================================================
} // namespace SceneGraph
} // namespace ACG
//=============================================================================
