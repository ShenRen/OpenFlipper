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


#ifndef ACG_SKELETONNODET_HH
#define ACG_SKELETONNODET_HH


//== INCLUDES =================================================================

#include <ACG/Scenegraph/MaterialNode.hh>
#include <ACG/Scenegraph/DrawModes.hh>
#include <ACG/GL/GLPrimitives.hh>
#include <ACG/GL/globjects.hh>
#include "JointT.hh"

//== FORWARDDECLARATIONS ======================================================

//== NAMESPACES ===============================================================

namespace ACG {
namespace SceneGraph {

//== CLASS DEFINITION =========================================================




/** \class SkeletonNodeT SkeletonNodeT.hh <ACG/.../SkeletonNodeT.hh>

    Brief Description.

    A more elaborate description follows.
*/

template <class SkeletonType>
class SkeletonNodeT : public BaseNode
{
public:
  // static name of this class
  ACG_CLASSNAME(SkeletonNodeT);

  typedef typename SkeletonType::Point            Point;
  typedef JointT<typename SkeletonType::Point>    Joint;
  typedef typename SkeletonType::Pose              Pose;
  typedef typename SkeletonType::Matrix          Matrix;

public:
  
  /// Constructor
  SkeletonNodeT(SkeletonType &_skeleton, BaseNode *_parent=0, std::string _name="<SkeletonNode>");

  /// Destructor
  ~SkeletonNodeT();

public:
  /// Returns a pointer to the skeleton
  SkeletonType& skeleton();

  /// Returns available draw modes
  DrawModes::DrawMode availableDrawModes() const;
  /// Returns the bounding box of this node
  void boundingBox(Vec3d& _bbMin, Vec3d& _bbMax);
  /// Renders the nodes contents using the given render state and draw mode
  void draw(GLState& _state, const DrawModes::DrawMode& _drawMode);
  /// Renders the node in picking mode, restricted to the node components given by \e _target
  void pick(GLState& _state, PickTarget _target);

  /// Adds renderobjects to renderer for flexible shader pipeline
  void getRenderObjects(IRenderer* _renderer, GLState& _state, const DrawModes::DrawMode& _drawMode, const Material* _mat);

  /// Set the pose which should be rendered
  void setActivePose(const AnimationHandle &_hAni);
  /// Get the pose that is used for rendering
  AnimationHandle activePose();

  /// Toggle visibility of coordinate frames for all joints
  void showCoordFrames(bool _bVisible = true);

  bool coordFramesVisible();

	/// returns the size of the rendered coordinate system
	double frameSize();
  
private:
  /// Pick method for vertices
  void pick_vertices( GLState& _state );
  /// Pick method for edges
  void pick_edges(GLState& _state);

  /// Helper function to draw the bones
  void draw_bone(GLState &_state, DrawModes::DrawMode _drawMode, const Point& _parent, const Point& _axis);
 
  /// Helper function to create a renderobject for bones
  void addBoneToRenderer(IRenderer* _renderer, RenderObject& _base, const Point& _parent, const Point& _axis);

  /// Helper function to compute modelview matrices for the two cones composing a bone
  void computeConeMatrices(const GLMatrixf& _modelView, const Point& _parent, const Point& _axis,
    GLMatrixf* _outCone0, GLMatrixf* _outCone1);

  /// Normalizes a coordinate frame defined by the given matrix
  void NormalizeCoordinateFrame(Matrix &_mat);

  /// get a joint color suitable to the baseColor
  void getJointColor( const Vec4f& _baseColor, Vec4f& _result );
  
  /// Convert HSV color to RGB
  void HSVtoRGB(const Vec4f& _HSV, Vec4f& _RGB);
  /// Convert RGB color to HSV
  void RGBtoHSV(const Vec4f& _RGB, Vec4f& _HSV);
  

  /// Simulate glPointSize(12) with a sphere
  double unprojectPointSize(double _pointSize, const Vec3d& _point, GLState& _state);


private:
  /// Coordinate frames are visible if this is true, not visible if false
  bool bCoordFramesVisible_;
  /// The skeleton nodes skeleton
  SkeletonType& skeleton_;
  /// The active pose, this one is going to be rendered
  AnimationHandle hAni_;
  
  /// size for the coord-frame
  double fFrameSize_;

  int slices_;
  int stacks_;

  ACG::GLSphere* sphere_;
  ACG::GLCone* cone_;
  ACG::GLCylinder* cylinder_;


  // instanced rendering with render objects:
  // store modelview and color of instances in a separate vbo

  /// per instance data of joint spheres
  GeometryBuffer  pointInstanceData_;
  VertexDeclaration pointInstanceDecl_;

  /// per instance data of bones
  GeometryBuffer boneInstanceData_;
  VertexDeclaration boneInstanceDecl_;
};


//=============================================================================
} // namespace SceneGraph
} // namespace ACG
//=============================================================================
#if defined(INCLUDE_TEMPLATES) && !defined(ACG_SKELETONNODET_C)
#define ACG_SKELETONNODET_TEMPLATES
#include "SkeletonNodeT.cc"
#endif
//=============================================================================
#endif // ACG_SKELETONNODET_HH defined
//=============================================================================

