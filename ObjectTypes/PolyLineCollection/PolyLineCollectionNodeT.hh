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
*                                                                            *
\*===========================================================================*/

/*===========================================================================*\
*                                                                            *
*   $Revision$                                                       *
*   $LastChangedBy$                                                *
*   $Date$                     *
*                                                                            *
\*===========================================================================*/

#ifndef POLYLINE_COLLECTION_NODE_T_HH
#define POLYLINE_COLLECTION_NODE_T_HH


//== INCLUDES =================================================================

#include <ACG/Scenegraph/BaseNode.hh>
#include <ACG/Scenegraph/DrawModes.hh>
#include <ACG/GL/VertexDeclaration.hh>
#include <ACG/GL/IRenderer.hh>
#include <ACG/GL/GLPrimitives.hh>
#include <ACG/GL/globjects.hh>
#include <ObjectTypes/PolyLine/PolyLineNodeT.hh>

//== FORWARDDECLARATIONS ======================================================

//== NAMESPACES ===============================================================

namespace ACG {
namespace SceneGraph {

//== CLASS DEFINITION =========================================================




/** \class PolyLineNodeT PolyLineCollectionNodeT.hh <ACG/.../PolyLineCollectionNodeT.hh>

    This Node provides support for rendering polyline collections.
*/

template <class PolyLineCollection>
class PolyLineCollectionNodeT : public BaseNode
{
public:

  // typedefs for easy access
  typedef typename PolyLineCollection::PolyLine::Point Point;

  /// Constructor
  PolyLineCollectionNodeT(PolyLineCollection& _pl, BaseNode* _parent = 0, std::string _name = "<PolyLineCollectionNode>");

  /// Destructor
  virtual ~PolyLineCollectionNodeT();

  PolyLineCollection& polyline_collection() { return polyline_collection_; }

  /// static name of this class
  ACG_CLASSNAME(PolyLineCollectionNodeT);

  /// return available draw modes
  DrawModes::DrawMode availableDrawModes() const;

  /// update bounding box
  void boundingBox(Vec3d& _bbMin, Vec3d& _bbMax);

  /// draw lines and normals
  void draw(GLState& /*_state*/,const DrawModes::DrawMode& _drawMode);

  /// picking
  void pick(GLState& _state, PickTarget _target);

  /** \brief Add the objects to the given renderer
    *
    * @param _renderer The renderer which will be used. Add your geometry into this class
    * @param _state    The current GL State when this object is called
    * @param _drawMode The active draw mode
    * @param _mat      Current material
    */
  void getRenderObjects(ACG::IRenderer* _renderer, ACG::GLState&  _state , const ACG::SceneGraph::DrawModes::DrawMode&  _drawMode , const ACG::SceneGraph::Material* _mat);

  /// Trigger an update of the vbo
  void update() { updateVBO_ = true; }
  void resetVBO() {offsets_.clear();}

private:

  /// Copy constructor (not used)
  PolyLineCollectionNodeT(const PolyLineCollectionNodeT& _rhs);

  /// Assignment operator (not used)
  PolyLineCollectionNodeT& operator=(const PolyLineCollectionNodeT& _rhs);

  /// Buffer organization
  ACG::VertexDeclaration vertexDecl_;       // layout without colors
  ACG::VertexDeclaration vertexDeclVColor_; // layout with vertex colors
  ACG::VertexDeclaration vertexDeclEColor_; // layout with edge colors

  /** \brief Trigger an update of the vbo
   *
   * If the polyLineCollection is changed, you have to call this function to update the buffers.
   *
   */
  void updateVBO();


  void pick_vertices(GLState& _state);
  void pick_edges(GLState& _state, unsigned int _offset = 0);

private:

  /// The associated poly line collection
  PolyLineCollection& polyline_collection_;

  /// VBO used to render the poly line
  GeometryBuffer vbo_;

  /// Flag to trigger update of vbo
  bool updateVBO_;

  /// Sphere for VertexSphere DrawMode
  GLSphere* sphere_;

  std::vector<std::pair<size_t, size_t> > offsets_;

  std::vector< PolyLineNodeT<typename PolyLineCollection::PolyLine>* > polylineNodes_;

  size_t total_vertex_count_;
};


//=============================================================================
} // namespace SceneGraph
} // namespace ACG
//=============================================================================
#if defined(INCLUDE_TEMPLATES) && !defined(ACG_POLYLINECOLLECTIONNODET_C)
#define ACG_POLYLINECOLLECTIONNODET_TEMPLATES
#include "PolyLineCollectionNodeT.cc"
#endif


#endif // POLYLINE_COLLECTION_NODE_T_HH
