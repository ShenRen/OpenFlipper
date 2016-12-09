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

#define ACG_POLYLINECOLLECTIONNODET_C

//== INCLUDES =================================================================



#include "PolyLineCollectionNodeT.hh"
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
template <class PolyLineCollection>
PolyLineCollectionNodeT<PolyLineCollection>::PolyLineCollectionNodeT(PolyLineCollection& _pl, BaseNode* _parent, std::string _name) :
        BaseNode(_parent, _name),
        polyline_collection_(_pl),
        updateVBO_(true),
        sphere_(0),
        total_vertex_count_(0)

{
  // Initial default draw mode
  drawMode(DrawModes::WIREFRAME | DrawModes::POINTS );
}

//----------------------------------------------------------------------------

template <class PolyLineCollection>
PolyLineCollectionNodeT<PolyLineCollection>::~PolyLineCollectionNodeT()
{
  for (size_t i = 0; i < polylineNodes_.size(); ++i)
    delete polylineNodes_[i];
}

//----------------------------------------------------------------------------

template <class PolyLineCollection>
void
PolyLineCollectionNodeT<PolyLineCollection>::
boundingBox(Vec3d& _bbMin, Vec3d& _bbMax)
{
    for(typename PolyLineCollection::iterator it = polyline_collection_.iter(); it; ++it){
        typename PolyLineCollection::PolyLine* polyline = *it;

        if(polyline){
            for (unsigned int i=0; i< polyline->n_vertices(); ++i)
            {
                _bbMin.minimize(polyline->point(i));
                _bbMax.maximize(polyline->point(i));
            }
        }
    }
}


//----------------------------------------------------------------------------


template <class PolyLineCollection>
DrawModes::DrawMode
PolyLineCollectionNodeT<PolyLineCollection>::
availableDrawModes() const
{
  return (DrawModes::WIREFRAME | DrawModes::POINTS | DrawModes::POINTS_COLORED | DrawModes::EDGES_COLORED );
}


//----------------------------------------------------------------------------


template <class PolyLineCollection>
void
PolyLineCollectionNodeT<PolyLineCollection>::
draw(GLState& _state, const DrawModes::DrawMode& _drawMode)
{
    // Block if we do not have any polylines
    if ( polyline_collection_.n_polylines() == 0 )
        return;

    // Update the vbo only if required.
    if ( updateVBO_ )
        updateVBO();

    ACG::GLState::disable(GL_LIGHTING);
    ACG::GLState::disable(GL_TEXTURE_2D);

    // Bind the vertex array
    vbo_.bind();
    vertexDecl_.activateFixedFunction();

    ACG::Vec4f color = _state.ambient_color()  + _state.diffuse_color();

    bool vertexColors = _drawMode & DrawModes::POINTS_COLORED;

    // draw points
    if (_drawMode & DrawModes::POINTS || vertexColors)
    {
        _state.set_color( color );

        if (vertexColors) {
          vertexDecl_.deactivateFixedFunction();
          vertexDeclVColor_.activateFixedFunction();
        }

        // Draw all vertices (don't care about selection)
        glDrawArrays(GL_POINTS, 0, total_vertex_count_);

        if (vertexColors) {
          vertexDeclVColor_.deactivateFixedFunction();
          vertexDecl_.activateFixedFunction();
        }


        float point_size_old = _state.point_size();
        _state.set_point_size(point_size_old+4.0f);

        _state.set_color( Vec4f(1.0f,0.0f,0.0f,1.0f) );

        // Draw selected polylines
        for(typename PolyLineCollection::index_iterator it = polyline_collection_.selected_iter(); it; ++it){
            typename PolyLineCollection::PolyLine* polyline = *it;

            if(polyline && polyline->n_vertices() > 0){
                glDrawArrays(GL_POINTS, offsets_[it.idx()].first, offsets_[it.idx()].second-1);
            }
        }

        _state.set_point_size(point_size_old);

    }


    bool edgeColors = _drawMode & DrawModes::EDGES_COLORED;

    // draw line segments
    if (_drawMode & DrawModes::WIREFRAME || edgeColors) {

        _state.set_color( color );

        if (edgeColors) {
          vertexDecl_.deactivateFixedFunction();
          vertexDeclEColor_.activateFixedFunction();
        }

        ibo_.bind();
        glDrawElements(GL_LINES, total_segment_count_ * 2, GL_UNSIGNED_INT, 0);
        ibo_.unbind();

        if (edgeColors) {
          vertexDeclEColor_.deactivateFixedFunction();
          vertexDecl_.activateFixedFunction();
        }

        float line_width_old = _state.line_width();
        _state.set_color(Vec4f(1, 0, 0, 1));
        _state.set_line_width(2 * line_width_old);

        for(typename PolyLineCollection::index_iterator it = polyline_collection_.selected_iter(); it; ++it){
            typename PolyLineCollection::PolyLine* polyline = *it;

            if(polyline && polyline->n_vertices() > 0){
                if ( polyline->is_closed() ){
                    glDrawArrays(GL_LINE_STRIP, offsets_[it.idx()].first, offsets_[it.idx()].second);
                }else{
                    glDrawArrays(GL_LINE_STRIP, offsets_[it.idx()].first, offsets_[it.idx()].second-1);
                }

                //offset += polyline->n_vertices() + 1;
            }
        }

        // Draw selected edges
        std::vector<int> selected_indices;
        for(typename PolyLineCollection::index_iterator it = polyline_collection_.visible_iter(); it; ++it){
            typename PolyLineCollection::PolyLine* polyline = *it;


            if(polyline){
                int offset = offsets_[it.idx()].first;
                for(size_t i = 0; i < polyline->n_vertices(); ++i){
                    if(!polyline->is_closed() && i == polyline->n_vertices() - 1){
                        continue;
                    }
                    if(polyline->edge_selected(i)){
                        selected_indices.push_back(offset + i);
                        selected_indices.push_back(offset + i + 1);
                    }
                }
            }
        }

        if(selected_indices.size() > 0){
            glDrawElements(GL_LINES, selected_indices.size(), GL_UNSIGNED_INT, selected_indices.data());
        }

        _state.set_line_width(line_width_old);

    }

    vertexDecl_.deactivateFixedFunction();
    ACG::GLState::bindBuffer(GL_ARRAY_BUFFER_ARB, 0);
}

//----------------------------------------------------------------------------


template <class PolyLineCollection>
void
PolyLineCollectionNodeT<PolyLineCollection>::
pick(GLState& _state, PickTarget _target)
{
  size_t numPolyLines = polyline_collection_.n_polylines();

  if (  numPolyLines == 0 )
    return;


  switch (_target)
  {
    case PICK_VERTEX:
    {
      size_t numPickIDs = 0;

      for (size_t i = 0; i < numPolyLines; ++i)
      {
        typename PolyLineCollection::PolyLine* line = polyline_collection_.polyline(i);

        if (line)
          numPickIDs += line->n_vertices();
      }

      _state.pick_set_maximum (numPickIDs);
      pick_vertices( _state);
      break;
    }

    case PICK_EDGE:
    {
      size_t numPickIDs = 0;

      for (size_t i = 0; i < numPolyLines; ++i)
      {
        typename PolyLineCollection::PolyLine* line = polyline_collection_.polyline(i);

        if (line)
          numPickIDs += line->n_edges();
      }

      _state.pick_set_maximum (numPickIDs);
      pick_edges(_state, 0);
      break;
    }

    case PICK_ANYTHING:
    {
      size_t numPickVertices = 0;
      size_t numPickIDs = 0;

      for (size_t i = 0; i < numPolyLines; ++i)
      {
        typename PolyLineCollection::PolyLine* line = polyline_collection_.polyline(i);

        if (line)
        {
          numPickIDs += line->n_vertices();
          numPickIDs += line->n_edges();

          numPickVertices += line->n_vertices();
        }
      }

      _state.pick_set_maximum (numPickIDs);

      pick_vertices( _state);
      pick_edges( _state, static_cast<unsigned int>(numPickVertices));
      break;
    }

    default:
      break;
  }
}

//----------------------------------------------------------------------------

template <class PolyLineCollection>
void
PolyLineCollectionNodeT<PolyLineCollection>::
pick_vertices(GLState &_state)
{
  size_t numPolyLines = polyline_collection_.n_polylines();

  // draw vertex pick IDs

  float point_size_old = _state.point_size();
  glPointSize(point_size_old+3.0f);


  GLSL::Program* pickVertexShader = ACG::ShaderCache::getInstance()->getProgram("Picking/pick_vertices_vs.glsl", "Picking/pick_vertices_fs.glsl");

  if (pickVertexShader && pickVertexShader->isLinked())
  {
    // shader needs picking ID offset and transform matrix
    int pickVertexOffset = int(_state.pick_current_index());
    GLMatrixf mWVP = _state.projection() * _state.modelview();

    pickVertexShader->use();

    pickVertexShader->setUniform("mWVP", mWVP);

    vbo_.bind();

    vertexDecl_.activateShaderPipeline(pickVertexShader);


    for (size_t i = 0; i < numPolyLines; ++i)
    {
      typename PolyLineCollection::PolyLine* line = polyline_collection_.polyline(i);

      if (line && line->n_vertices() > 0)
      {
        pickVertexShader->setUniform("pickVertexOffset", pickVertexOffset);

        glDrawArrays(GL_POINTS, offsets_[i].first, offsets_[i].second-1);

        pickVertexOffset += int(offsets_[i].second - 1);
      }
    }

    vertexDecl_.deactivateShaderPipeline(pickVertexShader);

    vbo_.unbind();

    pickVertexShader->disable();
  }
  else
  {
    // fallback implementation: draw picking ids without shader
    glBegin(GL_POINTS);

    size_t curPickID = 0;

    for (size_t i = 0; i < numPolyLines; ++i)
    {
      typename PolyLineCollection::PolyLine* line = polyline_collection_.polyline(i);

      if (line && line->n_vertices() > 0)
      {
        for (size_t v = 0; v < line->n_vertices(); ++v)
        {
          _state.pick_set_name(curPickID++);

          glVertex3d(line->point(v)[0], line->point(v)[1], line->point(v)[2]);
        }
      }
    }

    glEnd();
  }

  glPointSize(point_size_old);
}

//----------------------------------------------------------------------------

template <class PolyLineCollection>
void
PolyLineCollectionNodeT<PolyLineCollection>::
pick_edges(GLState &_state, unsigned int _offset)
{
  size_t numPolyLines = polyline_collection_.n_polylines();

  // draw edge picking IDs
  float line_width_old = _state.line_width();
  glLineWidth(line_width_old+3.0f);

  GLSL::Program* pickEdgeShader = ACG::ShaderCache::getInstance()->getProgram("Picking/vertex.glsl", "Picking/pick_vertices_fs2.glsl");

  if (pickEdgeShader && pickEdgeShader->isLinked())
  {
    // shader needs picking ID offset and transform matrix
    int pickVertexOffset = int(_state.pick_current_index() + _offset);
    GLMatrixf mWVP = _state.projection() * _state.modelview();

    pickEdgeShader->use();

    pickEdgeShader->setUniform("mWVP", mWVP);

    vbo_.bind();

    vertexDecl_.activateShaderPipeline(pickEdgeShader);

    for (size_t i = 0; i < numPolyLines; ++i)
    {
      typename PolyLineCollection::PolyLine* line = polyline_collection_.polyline(i);

      if (line && line->n_vertices() > 0)
      {
        pickEdgeShader->setUniform("pickVertexOffset", pickVertexOffset);

        size_t numIndices = line->is_closed() ? offsets_[i].second : offsets_[i].second - 1;
        glDrawArrays(GL_LINE_STRIP, offsets_[i].first, numIndices);

        pickVertexOffset += int(line->n_edges());
      }
    }

    vertexDecl_.deactivateShaderPipeline(pickEdgeShader);

    vbo_.unbind();

    pickEdgeShader->disable();
  }
  else
  {
    // fallback implementation: draw picking ids without shader
    glBegin(GL_LINES);

    size_t curPickID = _offset;

    for (size_t i = 0; i < numPolyLines; ++i)
    {
      typename PolyLineCollection::PolyLine* line = polyline_collection_.polyline(i);

      if (line)
      {
        size_t nv = line->n_vertices();

        for (size_t v = 0; v < line->n_edges(); ++v)
        {
          _state.pick_set_name(curPickID++);

          glVertex3d(line->point(v)[0], line->point(v)[1], line->point(v)[2]);

          size_t e = (v+1)%nv;
          glVertex3d(line->point(e)[0], line->point(e)[1], line->point(e)[2]);
        }
      }
    }

    glEnd();
  }

  glLineWidth(line_width_old);
}

//----------------------------------------------------------------------------

template <class PolyLineCollection>
void
PolyLineCollectionNodeT<PolyLineCollection>::
updateVBO() {

    bool lines_did_change = false;

    if( offsets_.size() != polyline_collection_.n_polylines() ){
        offsets_.resize(polyline_collection_.n_polylines());
        lines_did_change = true;
    }

    int offset = 0;
    total_vertex_count_ = 0;
    total_segment_count_ = 0;
    for(typename PolyLineCollection::iterator it = polyline_collection_.iter(); it; ++it){
        std::pair<size_t, size_t> current_offset;
        current_offset.first = offset;
        if(*it){
            current_offset.second = it->n_vertices() + 1;
        }else{
            current_offset.second = 0;
        }

        if(current_offset != offsets_[it.idx()]){
            lines_did_change = true;
        }

        offsets_[it.idx()] = current_offset;
        total_vertex_count_ += current_offset.second;
        total_segment_count_ += it->n_edges();

        offset += current_offset.second;
    }

    if(lines_did_change){


      // update internal line node
      // these are used exclusively used to fill the vertex buffer


      size_t prevLineCount = polylineNodes_.size();
      size_t curLineCount = polyline_collection_.n_polylines();


      // free memory for removed lines
      if (curLineCount < prevLineCount)
      {
        for (size_t i = curLineCount; i < prevLineCount; ++i)
          delete polylineNodes_[i];
      }

      polylineNodes_.resize(curLineCount);


      for(size_t i = 0; i < curLineCount; ++i) {
        typename PolyLineCollection::PolyLine* polyline = polyline_collection_.polyline(i);
        polylineNodes_[i] = new PolyLineNodeT<typename PolyLineCollection::PolyLine>(*polyline);
      }



      // Update the vertex declaration based on the input data:
      vertexDecl_.clear();
      vertexDeclVColor_.clear();
      vertexDeclEColor_.clear();


      if (curLineCount > 0) {
        polylineNodes_[0]->setupVertexDeclaration(&vertexDecl_, 0);
        polylineNodes_[0]->setupVertexDeclaration(&vertexDeclVColor_, 1);
        polylineNodes_[0]->setupVertexDeclaration(&vertexDeclEColor_, 2);


        // make sure that all polylines have the same vertex format
        bool equalVertexFormat = true;

        for (size_t i = 1; i < curLineCount; ++i) {

          VertexDeclaration decl[3];

          for (int k = 0; k < 3; ++k)
            polylineNodes_[i]->setupVertexDeclaration(&decl[k], k);

          if (decl[0] != vertexDecl_ ||
              decl[1] != vertexDeclVColor_ ||
              decl[2] != vertexDeclEColor_)
            equalVertexFormat = false;
        }


        if (!equalVertexFormat)
          std::cerr << "PolyLineCollection error: polylines do not have the same vertex format, so the collection vbo could not be created" << std::endl;
        else {

          // size in bytes of vbo,  create additional vertex for closed loop indexing
          size_t stride = vertexDecl_.getVertexStride();
          size_t bufferSize = stride * offset;

          if (bufferSize > 0) {
            std::vector<char> vboData(bufferSize);
            std::vector<unsigned int> iboData(total_segment_count_ * 2);

            size_t offsetSegment = 0;

            for(size_t i = 0; i < curLineCount; ++i) {
              typename PolyLineCollection::PolyLine* polyline = polyline_collection_.polyline(i);


              if (polyline && polylineNodes_[i] && polyline->n_vertices() > 0) {

                // fill vertex buffer data
                size_t offset = offsets_[i].first;

                polylineNodes_[i]->fillVertexBuffer(&vboData[(offset) * stride], bufferSize, true);

                // fill index buffer data
                for (size_t k = 0; k < polyline->n_edges(); ++k) {
                  iboData[(offsetSegment + k) * 2] = offset + k;
                  iboData[(offsetSegment + k) * 2 + 1] = offset + (k + 1) % polyline->n_vertices();
                }
                offsetSegment += polyline->n_edges();

              }

            }

            // Move data to the buffer in gpu memory
            vbo_.upload(vboData.size() * sizeof(vboData[0]), &vboData[0], GL_STATIC_DRAW);
            vbo_.unbind();

            ibo_.upload(iboData.size() * sizeof(iboData[0]), &iboData[0], GL_STATIC_DRAW);
            ibo_.unbind();
          }

        }

      }
    }



    // Update done.
    updateVBO_ = false;
}

//----------------------------------------------------------------------------

template <class PolyLineCollection>
void
PolyLineCollectionNodeT<PolyLineCollection>::
getRenderObjects(ACG::IRenderer* _renderer, ACG::GLState&  _state , const ACG::SceneGraph::DrawModes::DrawMode&  _drawMode , const ACG::SceneGraph::Material* _mat) {

  // Block if we do not have any vertices
  if ( polyline_collection_.n_polylines() == 0 )
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
  ro.debugName = "PolyLineCollection";
  ro.blending = false;
  ro.depthTest = true;

  // Default color
  ACG::Vec4f defaultColor   = _state.ambient_color()  + _state.diffuse_color();
  ACG::Vec4f selectionColor = ACG::Vec4f(1.0,0.0,0.0,1.0);

  // Viewport size
  ACG::Vec2f screenSize(float(_state.viewport_width()), float(_state.viewport_height()));

  for (size_t i = 0; i < _drawMode.getNumLayers(); ++i) {
    ACG::SceneGraph::Material localMaterial = *_mat;

    const ACG::SceneGraph::DrawModes::DrawModeProperties* props = _drawMode.getLayer(i);

    ro.setupShaderGenFromDrawmode(props);
    ro.shaderDesc.shadeMode = SG_SHADE_UNLIT;


    if (props->colored())
    {
      if (props->colorSource() == ACG::SceneGraph::DrawModes::COLOR_PER_EDGE)
      {
        ro.vertexDecl = &vertexDeclEColor_;
        ro.shaderDesc.vertexColorsInterpolator = "flat";
      }
      else
      {
        ro.vertexDecl = &vertexDeclVColor_;
        ro.shaderDesc.vertexColorsInterpolator.clear();
      }

      ro.shaderDesc.vertexColors = true;
    }
    else
    {
      ro.vertexDecl = &vertexDecl_;
      ro.shaderDesc.vertexColors = false;
    }

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


        // Point Size geometry shader
        ro.setupPointRendering(_mat->pointSize(), screenSize);

        // apply user settings
        applyRenderObjectSettings(props->primitive(), &ro);


        ro.glDrawArrays(GL_POINTS, 0, total_vertex_count_);
        _renderer->addRenderObject(&ro);

    }else if(props->primitive() == ACG::SceneGraph::DrawModes::PRIMITIVE_WIREFRAME ||
      props->primitive() == ACG::SceneGraph::DrawModes::PRIMITIVE_EDGE){
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


        // Line Width geometry shader
        ro.setupLineRendering(_state.line_width(), screenSize);

        // apply user settings
        applyRenderObjectSettings(props->primitive(), &ro);


        ro.indexBuffer = ibo_.id();
        ro.glDrawElements(GL_LINES, total_segment_count_ * 2, GL_UNSIGNED_INT, 0);

        _renderer->addRenderObject(&ro);
        ro.indexBuffer = 0;

    }
  }

}

//=============================================================================
} // namespace SceneGraph
} // namespace ACG
//=============================================================================
