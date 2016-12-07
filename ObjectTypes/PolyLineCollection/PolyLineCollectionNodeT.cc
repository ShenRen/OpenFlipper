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
        vbo_(0),
        updateVBO_(true),
        sphere_(0),
        total_vertex_count_(0)

{
  // Initial default draw mode
  drawMode(DrawModes::WIREFRAME | DrawModes::POINTS );
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
  return (DrawModes::WIREFRAME | DrawModes::POINTS );
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
    ACG::GLState::bindBuffer(GL_ARRAY_BUFFER_ARB, vbo_);
    vertexDecl_.activateFixedFunction();

    ACG::Vec4f color = _state.ambient_color()  + _state.diffuse_color();

    // draw points
    if (_drawMode & DrawModes::POINTS)
    {
        _state.set_color( color );

        // Draw all vertices (don't care about selection)
        for(typename PolyLineCollection::index_iterator it = polyline_collection_.visible_iter(); it; ++it){
            typename PolyLineCollection::PolyLine* polyline = *it;

            if(polyline && polyline->n_vertices() > 0){
                glDrawArrays(GL_POINTS, offsets_[it.idx()].first, offsets_[it.idx()].second-1);
            }
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

    // draw line segments
    if (_drawMode & DrawModes::WIREFRAME) {

        _state.set_color( color );

        for(typename PolyLineCollection::index_iterator it = polyline_collection_.visible_iter(); it; ++it){
            typename PolyLineCollection::PolyLine* polyline = *it;

            if(polyline && polyline->n_vertices() > 0){
                if ( polyline->is_closed() ){
                    glDrawArrays(GL_LINE_STRIP, offsets_[it.idx()].first, offsets_[it.idx()].second);

                }else{
                    glDrawArrays(GL_LINE_STRIP, offsets_[it.idx()].first, offsets_[it.idx()].second-1);
                }
            }
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

  if (  polyline_collection_.n_polylines() == 0 )
    return;

  ACG::GLState::bindBuffer(GL_ARRAY_BUFFER_ARB, vbo_);
  vertexDecl_.activateFixedFunction();;

  _state.pick_set_maximum(2);

  glDepthRange(0.0, 0.999999);


  float point_size_old = _state.point_size();
  glPointSize(point_size_old+3.0f);
  if ((drawMode() & DrawModes::POINTS) && (_target == PICK_VERTEX || _target == PICK_ANYTHING))
  {
      _state.pick_set_name(0);

      for(typename PolyLineCollection::index_iterator it = polyline_collection_.visible_iter(); it; ++it){
          typename PolyLineCollection::PolyLine* polyline = *it;

          if(polyline && polyline->n_vertices() > 0){
              glDrawArrays(GL_POINTS, offsets_[it.idx()].first, offsets_[it.idx()].second-1);
          }
      }
  }
  glPointSize(point_size_old);

  float line_width_old = _state.line_width();
  glLineWidth(line_width_old+3.0f);
  // draw line segments
  if ((drawMode() & DrawModes::WIREFRAME) && (_target == PICK_EDGE || _target == PICK_ANYTHING)) {

      _state.pick_set_name(1);

      for(typename PolyLineCollection::index_iterator it = polyline_collection_.visible_iter(); it; ++it){
          typename PolyLineCollection::PolyLine* polyline = *it;

          if(polyline && polyline->n_vertices() > 0){
              if ( polyline->is_closed() ){
                  glDrawArrays(GL_LINE_STRIP, offsets_[it.idx()].first, offsets_[it.idx()].second);

              }else{
                  glDrawArrays(GL_LINE_STRIP, offsets_[it.idx()].first, offsets_[it.idx()].second-1);
              }
          }
      }
  }
  glLineWidth(line_width_old);
  glDepthRange(0.0, 1.0);

  vertexDecl_.deactivateFixedFunction();
  ACG::GLState::bindBuffer(GL_ARRAY_BUFFER_ARB, 0);
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

        offset += current_offset.second;
    }

    if(lines_did_change){

        // Update the vertex declaration based on the input data:
        vertexDecl_.clear();

        // We always output vertex positions
        vertexDecl_.addElement(GL_FLOAT, 3, ACG::VERTEX_USAGE_POSITION);

        // create vbo if it does not exist
        if (!vbo_)
            GLState::genBuffersARB(1, &vbo_);

        // size in bytes of vbo,  create additional vertex for closed loop indexing
        unsigned int bufferSize = vertexDecl_.getVertexStride() * offset;

        // Create the required array
        char* vboData_ = new char[bufferSize];

        for(typename PolyLineCollection::iterator it = polyline_collection_.iter(); it; ++it){
            typename PolyLineCollection::PolyLine* polyline = *it;

            if(polyline && polyline->n_vertices() > 0){
                size_t offset = offsets_[it.idx()].first;
                for (unsigned int  i = 0 ; i < polyline->n_vertices(); ++i) {
                    writeVertex(polyline, i, vboData_ + (offset + i) * vertexDecl_.getVertexStride());
                }

                // First point is added to the end for a closed loop
               writeVertex(polyline, 0, vboData_ + (offset + polyline->n_vertices()) * vertexDecl_.getVertexStride());
            }
        }

        // Move data to the buffer in gpu memory
        GLState::bindBufferARB(GL_ARRAY_BUFFER_ARB, vbo_);
        GLState::bufferDataARB(GL_ARRAY_BUFFER_ARB, bufferSize , vboData_ , GL_STATIC_DRAW_ARB);
        GLState::bindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

        // Remove the local storage
        delete[] vboData_;
    }

    // Update done.
    updateVBO_ = false;
}


//----------------------------------------------------------------------------

template <class PolyLineCollection>
void
PolyLineCollectionNodeT<PolyLineCollection>::
writeVertex(typename PolyLineCollection::PolyLine* _polyline, unsigned int _vertex, void* _dst) {

  ACG::Vec3f& pos = *((ACG::Vec3f*)_dst);

  pos = OpenMesh::vector_cast<ACG::Vec3f>(_polyline->point(_vertex));
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

  for (size_t i = 0; i < _drawMode.getNumLayers(); ++i) {
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


        for(typename PolyLineCollection::index_iterator it = polyline_collection_.visible_iter(); it; ++it){
            if(*it && (*it)->n_vertices() > 0){
                ro.glDrawArrays(GL_POINTS, offsets_[it.idx()].first, offsets_[it.idx()].second-1);

                // Point Size geometry shader
                ro.setupPointRendering(_mat->pointSize(), screenSize);

                // apply user settings
                applyRenderObjectSettings(props->primitive(), &ro);

                _renderer->addRenderObject(&ro);
            }
        }

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
        for(typename PolyLineCollection::index_iterator it = polyline_collection_.visible_iter(); it; ++it){
            if(*it && (*it)->n_vertices() > 0){
                if ( (*it)->is_closed() ){
                    ro.glDrawArrays(GL_LINE_STRIP, offsets_[it.idx()].first, offsets_[it.idx()].second);
                }else{
                    ro.glDrawArrays(GL_LINE_STRIP, offsets_[it.idx()].first, offsets_[it.idx()].second-1);
                }

                //offset += (*it)->n_vertices() + 1;

                // Line Width geometry shader
                ro.setupLineRendering(_state.line_width(), screenSize);

                // apply user settings
                applyRenderObjectSettings(props->primitive(), &ro);

                _renderer->addRenderObject(&ro);
            }


        }


    }
  }

}

//=============================================================================
} // namespace SceneGraph
} // namespace ACG
//=============================================================================
