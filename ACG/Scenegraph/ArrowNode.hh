/*===========================================================================*\
 *                                                                           *
 *                              OpenFlipper                                  *
 *      Copyright (C) 2001-2014 by Computer Graphics Group, RWTH Aachen      *
 *                           www.openflipper.org                             *
 *                                                                           *
 *---------------------------------------------------------------------------*
 *  This file is part of OpenFlipper.                                        *
 *                                                                           *
 *  OpenFlipper is free software: you can redistribute it and/or modify      *
 *  it under the terms of the GNU Lesser General Public License as           *
 *  published by the Free Software Foundation, either version 3 of           *
 *  the License, or (at your option) any later version with the              *
 *  following exceptions:                                                    *
 *                                                                           *
 *  If other files instantiate templates or use macros                       *
 *  or inline functions from this file, or you compile this file and         *
 *  link it with other files to produce an executable, this file does        *
 *  not by itself cause the resulting executable to be covered by the        *
 *  GNU Lesser General Public License. This exception does not however       *
 *  invalidate any other reasons why the executable file might be            *
 *  covered by the GNU Lesser General Public License.                        *
 *                                                                           *
 *  OpenFlipper is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *  GNU Lesser General Public License for more details.                      *
 *                                                                           *
 *  You should have received a copy of the GNU LesserGeneral Public          *
 *  License along with OpenFlipper. If not,                                  *
 *  see <http://www.gnu.org/licenses/>.                                      *
 *                                                                           *
\*===========================================================================*/

/*===========================================================================*\
 *                                                                           *
 *   $Revision: 20771 $                                                       *
 *   $Author: moeller $                                                      *
 *   $Date: 2015-06-03 16:47:45 +0200 (Wed, 03 Jun 2015) $                   *
 *                                                                           *
\*===========================================================================*/




//=============================================================================
//
//  CLASS ArrowNode
//
//=============================================================================


#ifndef ACG_ARROWNODE_HH
#define ACG_ARROWNODE_HH


//== INCLUDES =================================================================

#include <ACG/Scenegraph/MaterialNode.hh>
#include <ACG/GL/globjects.hh>
#include <ACG/GL/VertexDeclaration.hh>

//== NAMESPACES ===============================================================

namespace ACG {
namespace SceneGraph {

//== CLASS DEFINITION =========================================================



/** \class ArrowNode ArrowNode.hh <ACG/Scenegraph/ArrowNode.hh>

    ArrowNode renders a set of arrows.
**/

class ACGDLLEXPORT ArrowNode : public MaterialNode
{
public:

  /// default constructor
  ArrowNode(BaseNode*    _parent = 0,
	          std::string  _name="<ArrowNode>" );

  /// destructor
  ~ArrowNode();


  /// static name of this class
  ACG_CLASSNAME(ArrowNode);

  /// return available draw modes
  DrawModes::DrawMode  availableDrawModes() const;

  /// update bounding box
  void boundingBox(Vec3d& _bbMin, Vec3d& _bbMax);
  
  /// reserve mem for _n arrows
  void reserve(int _n);

  /** \brief Add an arrow to the node
  *
  * @param _start  Start position of the arrow
  * @param _dir    Direction of the arrow
  * @param _normal Normal vector on the top surface of the arrow. Gets orthonormalized to _dir internally. If 0 or collinear to _dir, a random normal is chosen. 
  * @param _scale  Uniform scaling factor of the arrow. This is also the length of the arrow.
  * @param _color  Arrow color
  * @return ID of the arrow
  */
  int addArrow(const Vec3f& _start, const Vec3f& _dir, const Vec3f& _normal = Vec3f(0.0f, 1.0f, 0.0f), float _scale = 1.0f, const Vec4uc& _color = Vec4uc(82, 82, 82, 255));

  /** \brief Add an arrow to the node
  *
  * @param _start  Start position of the arrow
  * @param _dir    Direction of the arrow
  * @param _normal Normal vector on the top surface of the arrow. Gets orthonormalized to _dir internally. If 0 or collinear to _dir, a random normal is chosen.
  * @param _scale  Uniform scaling factor of the arrow. This is also the length of the arrow.
  * @param _color  Arrow color
  * @return id of the arrow
  */
  int addArrow(const Vec3d& _start, const Vec3d& _dir, const Vec3d& _normal = Vec3d(0.0f, 1.0f, 0.0f), double _scale = 1.0f, const Vec4uc& _color = Vec4uc(82, 82, 82, 255));

  /** \brief Return the start position of an arrow
  *
  * @param _arrowID  ID of the arrow that was returned by addArrow()
  * @return Start position of the arrow
  */
  Vec3f arrowStart(int _arrowID) const;

  /** \brief Set start point of an already added arrow
  *
  * @param _arrowID  ID of the arrow that was returned by addArrow()
  * @param _start  Start position of the arrow
  */
  void arrowStart(int _arrowID, const Vec3f& _start);


  /** \brief Return the direction of an arrow
  *
  * @param _arrowID  ID of the arrow that was returned by addArrow()
  * @return  Direction of the arrow
  */
  Vec3f arrowDir(int _arrowID) const;

  /** \brief Set direction of an already added arrow
  *
  * @param _arrowID  ID of the arrow that was returned by addArrow()
  * @param _dir  Direction of the arrow
  */
  void arrowDir(int _arrowID, const Vec3f& _dir);


  /** \brief Return the normal of an arrow
  *
  * @param _arrowID  ID of the arrow that was returned by addArrow()
  * @return  Normal of the arrow
  */
  Vec3f arrowNormal(int _arrowID) const;

  /** \brief Set normal of an already added arrow
  *
  * @param _arrowID  ID of the arrow that was returned by addArrow()
  * @param _dir  Normal of the arrow
  */
  void arrowNormal(int _arrowID, const Vec3f& _normal);

  /** \brief Return the scale of an arrow
  *
  * @param _arrowID  ID of the arrow that was returned by addArrow()
  * @return Arrow scale
  */
  float arrowScale(int _arrowID) const;

  /** \brief Set the scale of an already added arrow
  *
  * @param _arrowID  ID of the arrow that was returned by addArrow()
  * @param _scale  Arrow scale
  */
  void arrowScale(int _arrowID, float _scale);

  /** \brief Return the color of an arrow
  *
  * @param _arrowID  ID of the arrow that was returned by addArrow()
  * @return Arrow color
  */
  Vec4uc arrowColor(int _arrowID) const;

  /** \brief Set the color of an already added arrow
  *
  * @param _arrowID  ID of the arrow that was returned by addArrow()
  * @param _scale  Arrow color
  */
  void arrowColor(int _arrowID, const Vec4uc& _color);

  /// clear arrows
  void clear();

  /// number of arrows
  int n_arrows() const;

  /// draw arrows
  void draw(GLState& _state, const DrawModes::DrawMode& _drawMode);

  /** \brief Add the objects to the given renderer
   *
   * @param _renderer The renderer which will be used. Add your geometry into this class
   * @param _state    The current GL State when this object is called
   * @param _drawMode The active draw mode
   * @param _mat      Current material
   */
  void getRenderObjects(IRenderer* _renderer, GLState&  _state , const DrawModes::DrawMode&  _drawMode , const ACG::SceneGraph::Material* _mat);


private:
  void createArrowMesh();


  GLMatrixf computeWorldMatrix(int _arrow) const;
  GLMatrixf readWorldMatrix(int _arrow) const;
  void updateInstanceData();

  void updateInstanceBuffer();

  // arrow instances

  struct Arrow 
  {
    Vec3f start, dir;
    Vec3f normal;
    float scale;
    Vec4uc color;

    void orthonormalize();
  };

  std::vector<Arrow> arrows_;

  // arrow mesh:
  int               numVertices_;
  int               numIndices_;
  GeometryBuffer    vertexBuffer_;
  IndexBuffer       indexBuffer_;
  VertexDeclaration vertexDecl_;
  Vec3f             localArrowMin_;
  Vec3f             localArrowMax_;


  // instance data:
  //  float4x3 world transform
  //  ubyte4_norm color

  // dword offset of the instance data of an arrow ( modify this if more data gets appended )
  int instanceDataOffset(int _arrow) const {return _arrow * (4*3+1);}

  // size in dwords of instance data ( modify this if more data gets appended )
  int instanceDataSize() const {return 4*3 + 1;}


  std::vector<float> instanceData_;

  // instance vbo
  GeometryBuffer    instanceBuffer_;
  VertexDeclaration vertexDeclInstanced_;


  bool invalidateInstanceData_;
  bool invalidateInstanceBuffer_;
  int supportsInstancing_;
};


//=============================================================================
} // namespace SceneGraph
} // namespace ACG
//=============================================================================
#endif // ACG_ARROWNODE_HH defined
//=============================================================================

