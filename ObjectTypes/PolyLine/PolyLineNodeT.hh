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
// CLASS PolyLineNodeT
//
// Author:  David Bommes <bommes@cs.rwth-aachen.de>
//
//=============================================================================


#ifndef ACG_POLYLINENODET_HH
#define ACG_POLYLINENODET_HH


//== INCLUDES =================================================================

#include <ACG/Scenegraph/BaseNode.hh>
#include <ACG/Scenegraph/DrawModes.hh>
#include <ACG/GL/VertexDeclaration.hh>
#include <ACG/GL/IRenderer.hh>
#include <ACG/GL/GLPrimitives.hh>

//== FORWARDDECLARATIONS ======================================================

//== NAMESPACES ===============================================================

namespace ACG {
namespace SceneGraph {

//== CLASS DEFINITION =========================================================




/** \class PolyLineNodeT PolyLineNodeT.hh <ACG/.../PolyLineNodeT.hh>

    Brief Description.

    A more elaborate description follows.
*/

template <class PolyLine>
class PolyLineNodeT : public BaseNode
{
public:

  // typedefs for easy access
  typedef typename PolyLine::Point Point;

  /// Constructor
  PolyLineNodeT(PolyLine& _pl, BaseNode* _parent = 0, std::string _name = "<PolyLineNode>");

  /// Destructor
  ~PolyLineNodeT();

  PolyLine& polyline() { return polyline_; }

  /// static name of this class
  ACG_CLASSNAME(PolyLineNodeT);

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
  void update() { updateVBO_ = true; };


  /** Add custom data to the vertex buffer for rendering with shaders
   *
   * The element description declares the type of data and the name in the shader.
   * The provided buffer has to store data for each vertex: num elements in buffer = polyline().n_vertices()
   * If the stride offset in  _desc.pointer_ is 0, it is assumed that the buffer stores the data without memory alignment.
   * This function does not create a local copy of the buffer, so the provided memory address
   * has to be valid whenever an internal vbo update is triggered.
   * Custom attributes are then available in the vertex shader by accessing the input with name tangentElement.shaderInputName_.
   *
   * Example:
   *
   * in plugin:
   *  have allocated buffer: tangentData_ of type Vec3f*  (member of a class)
   *  
   *  ACG::VertexElement tangentDesc;
   *  tangentDesc.type_ = GL_FLOAT;
   *  tangentDesc.numElements_ = 3;
   *  tangentDesc.usage_ = ACG::VERTEX_USAGE_USER_DEFINED;
   *  tangentDesc.shaderInputName_ = "myTangent";
   *  tangentDesc.pointer_ = 0;
   *
   *  polylineNode->addCustomBuffer(tangentDesc, tangentData_);
   *
   * in vertex shader:
   *  in vec3 myTangent;
   *  ..
   *  
   *
   * @param _desc  type description of an element in the buffer
   * @param _buffer pointer to data buffer, has to be a valid address for the remaining time
   * @return id of custom buffer
   */
  int addCustomBuffer(const ACG::VertexElement& _desc, const void* _buffer);


  /** Update pointer to custom data
   *
   * @param _id  id of the custom buffer; return value of addCustomBuffer
   * @param _buffer pointer to data buffer, has to be a valid address for the remaining time
   */
  void setCustomBuffer(int _id, const void* _buffer);

private:

  void pick_vertices       ( GLState& _state );
  void pick_spheres        ( GLState& _state );
  void pick_spheres_screen ( GLState& _state );
  void pick_edges          ( GLState& _state, unsigned int _offset);

  /// Copy constructor (not used)
  PolyLineNodeT(const PolyLineNodeT& _rhs);

  /// Assignment operator (not used)
  PolyLineNodeT& operator=(const PolyLineNodeT& _rhs);

  /// Vertex layout without vertex colors
  ACG::VertexDeclaration vertexDecl_;

  /// Vertex layout with vertex colors
  ACG::VertexDeclaration vertexDeclVCol_;

  /// Vertex layout with edge colors
  ACG::VertexDeclaration vertexDeclECol_;

  /// Custom vertex data for shader based rendering
  std::vector< std::pair<ACG::VertexElement, const void*> > customBuffers_;

  /// Map from custom properties in PolyLine to id in customBuffers_
  std::map< typename PolyLine::CustomPropertyHandle, int> polylinePropMap_;

  /** \brief Trigger an update of the vbo
   *
   * If the polyLine is changed, you have to call this function to update the buffers.
   *
   */
  void updateVBO();

  /** \brief Create the vertex declaration
  *
  * The vertex data in the vbo contains both vertex and edge colors,
  * so the vertex declaration decides which bytes to use for colored rendering (if any).
  * @param _dst Vertex declaration to initialize
  * @param _colorSource 0 - no colors, 1 - vertex colors, 2 - edge colors
  *
  */
  void setupVertexDeclaration(VertexDeclaration* _dst, int _colorSource) const;

  /** \brief Write vertex data for rendering to a buffer
   *
   * @param _vertex index of polyline vertex
   * @param _dst address of vertex in buffer
   */
  void writeVertex(unsigned int _vertex, void* _dst);

  /** \brief Write color for rendering to a buffer
  *
  * @param _vertex index of polyline vertex
  * @param _colorSourceVertex vertex or edge color?
  * @param _dst address of vertex in buffer
  */
  void writeVertexColor(unsigned int _vertex, bool _colorSourceVertex, void* _dst) const;

private:

  /// The associated poly line
  PolyLine& polyline_;

  /// VBO used to render the poly line
  unsigned int vbo_;

  /// Index buffer for selected vertices
  std::vector<unsigned int> selectedVertexIndexBuffer_;

  /// Index buffer for selected edges
  std::vector<unsigned int> selectedEdgeIndexBuffer_;

  /// Flag to trigger update of vbo
  bool updateVBO_;

  /// Sphere for VertexSphere DrawMode
  GLSphere* sphere_;

  /// This defines a local point spheres draw mode for all polyLine nodes
  DrawModes::DrawMode POINTS_SPHERES;

  /// This defines a local point spheres draw mode for all polyLine nodes with constant screen size
  DrawModes::DrawMode POINTS_SPHERES_SCREEN;
};


//=============================================================================
} // namespace SceneGraph
} // namespace ACG
//=============================================================================
#if defined(INCLUDE_TEMPLATES) && !defined(ACG_POLYLINENODET_C)
#define ACG_POLYLINENODET_TEMPLATES
#include "PolyLineNodeT.cc"
#endif
//=============================================================================
#endif // ACG_POLYLINENODET_HH defined
//=============================================================================

