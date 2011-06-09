/*===========================================================================*\
 *                                                                           *
 *                              OpenFlipper                                  *
 *      Copyright (C) 2001-2011 by Computer Graphics Group, RWTH Aachen      *
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
 *   $Revision$                                                       *
 *   $Author$                                                      *
 *   $Date$                   *
 *                                                                           *
\*===========================================================================*/


//=============================================================================
//
//  CLASS GLState
//
//=============================================================================


#ifndef ACG_GLSTATE_HH
#define ACG_GLSTATE_HH


//== INCLUDES =================================================================


#include "gl.hh"
#include "../Math/GLMatrixT.hh"
#include "../Math/VectorT.hh"
#include "../Config/ACGDefines.hh"
#include "ColorStack.hh"
#include <stack>
#include <vector>
#include <bitset>

#ifdef WIN32
	#pragma warning(push)
	#pragma warning(disable:4251)
#endif

//== NAMESPACES ===============================================================


namespace ACG {


//== CLASS DEFINITION =========================================================


/** \class GLState GLState.hh <ACG/Scenegraph/GLState.hh>

    This class stores all relevant OpenGL states and can therefore
    provide some nice and efficient functions like projecting,
    unprojecting, eye point or viewing direction.

    \note This class needs an OpenGL context. This is currently
    given in a QGLContext.
    
    \section pMatrix The projection matrix
    
    The GLState class stores its own projection matrix which is of type
    GLMatrixT. Creating a viewing frustum of a perspective projection
    will result in the following projection matrix:
    
    \image html pmatrix.png
    
    Where n is the orthogonal distance from the viewing plane (near plane)
    to the eye position, f is the orthogonal distance from the back clipping plane
    (far plane) to the eye position, l, r, t, b are scalars indicating the left, right,
    top and bottom coordinates of the viewing plane (so in fact we have a rectangle between
    the points (b,l) and (t,r)) and phi is the opening angle (field of view), also
    see figure 1.
    
    \image html projection.png "Figure 1. A schematic illustration of a viewing frustum"
    
    Some useful values that can directly be computed out of the projection matrix:
    
    \image html pfovy.png "Computing the fovy out of the projection matrix"
    
    \image html paspect.png "Computing the aspect out of the projection matrix"
**/

class ACGDLLEXPORT GLState
{
public:

  /// Default constructor
  GLState(bool _updateGL = true);

  /// destructor
  ~GLState() {}


  void makeCurrent() {  }

  /// synchronize this class with the OpenGL state machine
  void syncFromGL();

  /// initialize all state variables (called by constructor)
  void initialize();

  /// should GL matrices be updated after each matrix operation
  bool updateGL() const { return updateGL_; }
  /// should GL matrices be updated after each matrix operation
  void set_updateGL(bool _b) { updateGL_ = _b; }

  /// time passed since last redraw in milliseconds
  unsigned int msSinceLastRedraw () const { return msSinceLastRedraw_; }

  /// set time passed since last redraw in milliseconds
  void set_msSinceLastRedraw (unsigned int _ms) { msSinceLastRedraw_ = _ms; }

  /// set the whole stored gl state
  void setState ();

  /// clear buffers viewport rectangle
  void clearBuffers ();


  //===========================================================================
  /** @name glEnable / glDisable functionality
  * @{ */
  //===========================================================================

  /// replaces glEnable, but supports locking
  void enable(GLenum _cap);

  /// replaces glDisable, but supports locking
  void disable(GLenum _cap);

  /// locks a specific cap state, such that enable() or disable() has no effect
  void lockState(GLenum _cap);

  /// unlocks a specific cap state
  void unlockState(GLenum _cap);

  /// returns true, if a cap state is locked
  bool isStateLocked(GLenum _cap);

  /// returns true, if a cpa state is enabled
  bool isStateEnabled(GLenum _cap);


  /// replaces glBlendFunc, supports locking
  void blendFunc(GLenum _sfactor, GLenum _dfactor);

  /// get blend function, null-ptr safe
  void getBlendFunc(GLenum* _sfactor, GLenum* _dfactor);

  void lockBlendFunc() {blendFuncLock_ = true;}
  void unlockBlendFunc() {blendFuncLock_ = false;}
  bool isBlendFuncLocked() {return blendFuncLock_;}

  /// replaces glBlendEquation, supports locking
  void blendEquation(GLenum _mode);

  GLenum getBlendEquation() {return blendEquationState_;}

  void lockBlendEquation() {blendEquationLock_ = true;}
  void unlockBlendEquation() {blendEquationLock_ = false;}
  bool isBlendEquationLocked() {return blendEquationLock_;}

  /// replaces glBlendColor, supports locking
  void blendColor(GLclampf _red, GLclampf _green, GLclampf _blue, GLclampf _alpha);

  // get blend color, not null-ptr safe, 4 element color output: RGBA
  void getBlendColor(GLclampf* _col);

  void lockBlendColor() {blendColorLock_ = true;}
  void unlockBlendColor() {blendColorLock_ = false;}
  bool isBlendColorLocked() {return blendColorLock_;}


  /// replaces glAlphaFunc, supports locking
  void alphaFunc(GLenum _func, GLclampf _ref);

  /// get alpha function, null-ptr safe
  void getAlphaFunc(GLenum* _func, GLclampf* _ref);

  void lockAlphaFunc() {alphaFuncLock_ = true;}
  void unlockAlphaFunc() {alphaFuncLock_ = false;}
  bool isAlphaFuncLocked() {return alphaFuncLock_;}

  /** @} */

  //===========================================================================
  /** @name GL buffer binding
  * @{ */
  //===========================================================================

  /// replaces glBindBuffer, supports locking
  void bindBuffer(GLenum _target, GLuint _buffer);

  void lockBufferTarget(GLenum _target);
  
  void unlockBufferTarget(GLenum _target);

  bool isBufferTargetLocked(GLenum _target);

  GLuint getBoundBuf(GLenum _target);

  /** @} */

private:

  // bijective map from GLenum buffer_target to [0..3], -1 if unsupported
  int getBufferTargetIndex(GLenum _target);

public:

  //===========================================================================
  /** @name GL texture binding
  * @{ */
  //===========================================================================

  /// replaces glActiveTexture, no locking support
  inline void setActiveTexture(GLenum _texunit) {activeTexture_ = _texunit;}

  inline GLenum getActiveTexture() {return activeTexture_;}

  /// replaces glBindTexture, supports locking
  void bindTexture(GLenum _target, GLuint _buffer);

  /// locks the current texture stage (set by setActiveTexture)
  void lockTextureTarget(GLenum _target);

  void unlockTextureTarget(GLenum _target);

  bool isTextureTargetLocked(GLenum _target);

  GLuint getBoundTextureBuffer(GLenum _target); 

  /** @} */

private:

  // bijective map from GLenum texture_target to [0..3], -1 if unsupported
  int getTextureTargetIndex(GLenum _target);

public:

  //===========================================================================
  /** @name set GL projection matrix
  * @{ */
  //===========================================================================

  /// reset projection matrix (load identity)
  void reset_projection();

  /// set projection
  void set_projection(const GLMatrixd& _m) {
    GLMatrixd inv_m(_m);  inv_m.invert();  set_projection(_m, inv_m);
  }
  /// set projection and provide inverse projection matrix
  void set_projection(const GLMatrixd& _m, const GLMatrixd& _inv_m);

  /// orthographic projection
  void ortho( double _left, double _right,
	      double _bottom, double _top,
	      double _near_plane, double _far_plane );

  /// perspective projection
  void frustum( double _left, double _right,
		double _bottom, double _top,
		double _near_plane, double _far_plane );

  /// perspective projection
  void perspective( double _fovY, double _aspect,
		    double _near_plane, double _far_plane );

  /// set viewport (lower left corner, width, height, glcontext width, height)
  void viewport( int _left, int _bottom, int _width, int _height,
	         int _glwidth = 0, int _glheight = 0);


  /** @} */

  //===========================================================================
  /** @name  set GL modelview matrix
  * @{ */
  //===========================================================================


  /// reset modelview matrix (load identity)
  void reset_modelview();

  /// set modelview
  void set_modelview(const GLMatrixd& _m) {
    GLMatrixd inv_m(_m);  inv_m.invert();  set_modelview(_m, inv_m);
  }
  /// set modelview and provide inverse modelview matrix
  void set_modelview(const GLMatrixd& _m, const GLMatrixd& _inv_m);

  /// set camera by lookAt
  void lookAt( const Vec3d& _eye, const Vec3d& _center, const Vec3d& _up );

  /// translate by (_x, _y, _z)
  void translate( double _x, double _y, double _z,
                  MultiplyFrom _mult_from = MULT_FROM_RIGHT );
                  
  /// translate by _vector
  void translate( Vec3d _vector,
                  MultiplyFrom _mult_from = MULT_FROM_RIGHT );

  /// rotate around axis (_x, _y, _z) by _angle
  void rotate( double _angle, double _x, double _y, double _z,
	       MultiplyFrom _mult_from = MULT_FROM_RIGHT );

  /// scale by (_s, _s, _s)
  void scale( double _s )
  { scale(_s, _s, _s); }

  /// scale by (_s, _s, _s)
  void scale( double _s,
	      MultiplyFrom /* _mult_from = MULT_FROM_RIGHT  */ )
  { scale(_s, _s, _s); }

  /// scale by (_sx, _sy, _sz)
  void scale( double _sx, double _sy, double _sz,
	      MultiplyFrom _mult_from = MULT_FROM_RIGHT );

  /// multiply by a given transformation matrix
  void mult_matrix( const GLMatrixd& _m, const GLMatrixd& _inv_m,
		    MultiplyFrom _mult_from = MULT_FROM_RIGHT );


  /// push projection matrix
  void push_projection_matrix();
  /// pop projection matrix
  void pop_projection_matrix();

  /// push modelview matrix
  void push_modelview_matrix();
  /// pop modelview matrix
  void pop_modelview_matrix();

  /** @} */

  //===========================================================================
  /** @name  get GL states, matrices and projection details
  * @{ */
  //===========================================================================


  /// get projection matrix
  const GLMatrixd& projection() const {
    return projection_;
  }

  /// get modelview matrix
  const GLMatrixd& modelview() const {
    return modelview_;
  }

  /// get viewport matrix
  const GLMatrixd& viewport() const {
    return window2viewport_;
  }

  /// get forward projection matrix
  const GLMatrixd& forward_projection() const {
    return forward_projection_;
  }

  /// get inverse projection matrix
  const GLMatrixd& inverse_projection() const {
    return inverse_projection_;
  }

  /// get inverse modelview matrix
  const GLMatrixd& inverse_modelview() const {
    return inverse_modelview_;
  }

  /// get viewport
  void get_viewport( int& _left, int& _bottom,
		     int& _width, int& _height ) const {
    _left = left_; _bottom = bottom_; _width = width_; _height = height_;
  }

  /// get viewport width
  int viewport_width() const { return width_; }
  /// get viewport height
  int viewport_height() const { return height_; }

  /// get gl context width
  int context_width() const { return glwidth_; }

  /// get gl context height
  int context_height() const { return glheight_; }

  /// get near clipping distance
  double near_plane() const { return near_plane_; }

  /// get far clipping distance
  double far_plane() const { return far_plane_; }

  /// get field of view in y direction
  double fovy() const;

  /// get aspect ratio
  double aspect() const;

  /// get eye point
  Vec3d eye() const;

  /// get viewing ray
  Vec3d viewing_direction() const {
    return viewing_direction(width_>>1, height_>>1);
  }

  /// get viewing ray through pixel (_x,_y)
  Vec3d viewing_direction(int _x, int _y) const;

  /// get up-vector w.r.t. camera coordinates
  Vec3d up() const;

  /// get right-vector w.r.t. camera coordinates
  Vec3d right() const;

  /** get viewing ray (_orgin & _direction) through pixel (_x,_y).
      _direction will be nornalized.
  */
  void viewing_ray(int _x, int _y, Vec3d& _origin, Vec3d& _direction) const;
  
  /** @} */


  /// get glDepthFunc() that is supposed to be active
  const GLenum& depthFunc() const;
  /// Call glDepthFunc() to actually change the depth comparison function, and store the new value in this GLState
  void set_depthFunc(const GLenum& _depth_func);

//   inline void lockDepthFunc() {depthFuncLock_ = true;}
//   inline void unlockDepthFunc() {depthFuncLock_ = false;}
//   inline bool isDepthFuncLocked() const {return depthFuncLock_;}


  //--- project and unproject points ------------------------------------------

  /// project point in world coordinates to window coordinates
  Vec3d project(const Vec3d& _point) const;

  /// unproject point in window coordinates _winPoint to world coordinates
  Vec3d unproject(const Vec3d& _winPoint) const;




  //--- color & material ------------------------------------------------------

  /// default value for clear color
  static const Vec4f default_clear_color;
  /// default value for base color
  static const Vec4f default_base_color;
  /// default value for ambient color
  static const Vec4f default_ambient_color;
  /// default value for diffuse color
  static const Vec4f default_diffuse_color;
  /// default value for speculat color
  static const Vec4f default_specular_color;
  /// default value for shininess
  static const float default_shininess;


  /// set background color
  void set_clear_color(const Vec4f& _col);
  /// get background color
  const Vec4f& clear_color() const { return clear_color_; }

  /// set base color (used when lighting is off)
  void set_base_color(const Vec4f& _col);
  /// get base color (used when lighting is off)
  const Vec4f& base_color() const { return base_color_; }

  /// set ambient color
  void set_ambient_color(const Vec4f& _col);
  /// get ambient color
  const Vec4f& ambient_color() const { return ambient_color_; }

  /// set diffuse color
  void set_diffuse_color(const Vec4f& _col);
  /// get diffuse color
  const Vec4f& diffuse_color() const { return diffuse_color_; }

  /// set specular color
  void set_specular_color(const Vec4f& _col);
  /// get specular color
  const Vec4f& specular_color() const { return specular_color_; }

  /// set specular shininess (must be in [0, 128])
  void set_shininess(float _shininess);
  /// get specular shininess (must be in [0, 128])
  float shininess() const { return shininess_; }




  //--- thickness -------------------------------------------------------------

  /// set point size
  void set_point_size(float _f);
  /// get point size
  float point_size() const { return point_size_; }

  /// set line width
  void set_line_width(float _f);
  /// get line width
  float line_width() const { return line_width_; }

  //===========================================================================
  /** @name Render pass controls
   * @{ */
  //===========================================================================

public:
  /// get current render pass counter
  unsigned int render_pass() const { return render_pass_; }

  /// reset render pass counter
  void reset_render_pass() { render_pass_ = 1; }

  /// increment render pass counter
  void next_render_pass() { ++render_pass_; }

  /// get maximum number of render passes
  unsigned int max_render_passes() const { return max_render_passes_; }

  /// set maximum number of render passes
  void set_max_render_passes(const unsigned int _max) { max_render_passes_ = _max; }

private:
  /// holds the current render pass number (1-indexed)
  /// this has to be set externally
  unsigned int render_pass_;

  /// holds the maximum render pass number
  /// this has to be set externally
  unsigned int max_render_passes_;

  /** @} */
  
  //===========================================================================
  /** @name Scene Infos
  * @{ */
  //===========================================================================  

  public:
    /// Provide scene Information about the boundingbox via this function 
    /// It is not computed by the state itself!!
    void set_bounding_box(ACG::Vec3d _min, ACG::Vec3d _max );
    
    /// Get the current bounding box of the scene.
    /// This has to be set before traversal happens in the viewer by using set_bounding_box
    void get_bounding_box(ACG::Vec3d& _min, ACG::Vec3d& _max );
    
    
  private:
    ACG::Vec3d bb_min_,bb_max_;
    

  /** @} */
  //--- misc ------------------------------------------------------------------

public:
  /// set whether transparent or solid objects should be drawn
  void set_blending(bool _b) { blending_ = _b; }
  /// get whether transparenet or solid objects should be drawn
  bool blending() { return blending_; }

  /// set whether transparent or solid objects should be drawn
  void set_twosided_lighting(bool _b);
  /// get whether transparenet or solid objects should be drawn
  bool twosided_lighting() { return twosided_lighting_; }

  //--- Multi Sampling --------------------------------------------------------

  /// Enable or disable multisampling
  void set_multisampling( bool _b );

  /// Get current multisampling state
  bool multisampling() { return multisampling_; };

  /// Disable multisampling globally
  void allow_multisampling( bool _b ) { allow_multisampling_ = _b; };

  /// Check if Multisampling is globally disabled
  bool multisampling_allowed(){ return allow_multisampling_; };
  
  /// Get max number of available texture units
  int max_texture_units() const { return num_texture_units_; }
  
  //--- Mipmapping ------------------------------------------------------------
  
  /** \brief Allow mipmapping globally
  *
  * Note: This actually does not change the opengl state
  * since mipmapping is turned on/off via texture parameters
  * To change the behaviour see in TextureNode
  */
  
  void allow_mipmapping(bool _b) { mipmapping_ = _b; }
  
  /// Get current global mipmapping state
  bool mipmapping_allowed() const { return mipmapping_; }

  //--- picking ---------------------------------------------------------------

  /** Follows the OpenGL selection buffer implementation (OpenGL Red Book Chapter 13)
    * http://www.glprogramming.com/red/chapter13.html
    *
    * Color picking is another method to realize picking. This functions replace the original OpenGL
    * functions, to allow the handling of selection buffer picking and color based picking with
    * the same code.
    */

  /// initialize name/color picking stack (like glInitNames())
  void pick_init (bool _color);

  /** \brief Set the maximal number of primitives/components of your object
   *
   * Sets the maximum used name index at current stack position (only used in color picking)
   * A restriction inside the color picking implementation forces to set the maximum used
   * number for the following pick_set_name calls between the two pick_push_name/pick_pop_name calls.
   *
   * If you create picking for your nodes, the values set with pick_set_name have to be between zero and
   * this value.
   */
  bool pick_set_maximum (unsigned int _idx);

  /** \brief sets the current name/color (like glLoadName(_idx))
   *
   * This will be the index of the primitive/component in the object.
   * If you create picking for your nodes, the values set here will be returned as the target index. So
   * if your object has three parts, you call pick_set_name(0), than render the first component
   * and then pick_set_name(1), ...
   *
   * If you perform the picking and click on the second component, the returned index will be 1 as set in
   * the node.
   */
  void pick_set_name (unsigned int _idx);

  /// returns the color that will be used for index _idx during color picking if this index will be set
  /// with pick_set_name. This can be used to generate color arrays instead of using direct gl calls
  /// for each primitive
  Vec4uc pick_get_name_color (unsigned int _idx);

  /// creates a new name the stack (like glPushName())
  void pick_push_name (unsigned int _idx);

  /// pops the current name from the stack (like glPopName())
  void pick_pop_name ();

  /// converts the given color to index values on the stack (only used in color picking)
  /// This can be compared to the results of the selection buffer results
  std::vector<unsigned int> pick_color_to_stack (Vec4uc _rgba) const;

  /// returns the number of still available colors during color picking
  unsigned int pick_free_indicies () const;

  /// Did an error occur during picking (only used in color picking)
  /// Mostly to small color depth or wrong handling of pick_set_maximum
  bool pick_error () const;

  /** \brief Returns the current color picking index (can be used for caching)
   *
   * Is this value equal to a value used in a previous picking run, then the same colors will be used.
   * In this case a previously calculated color array/display list can be reused.
   *
   * This is basically the name of the whole object ( which would be the same as the name of the first component
   * of the object), not the name of the primitives/components in the object!
   *
   */
  unsigned int pick_current_index () const;

  /// Is color picking active?
  bool color_picking () const;


private: //--------------------------------------------------------------------

  // update forward projection and backward projection matrices
  void update_matrices();


  // matrix stacks
  std::stack<GLMatrixd>  stack_projection_,
                         stack_modelview_,
                         stack_inverse_projection_,
                         stack_inverse_modelview_;

  // current matrices
  GLMatrixd  projection_,
             inverse_projection_,
             modelview_,
             inverse_modelview_,
             window2viewport_,
             inverse_window2viewport_,
             forward_projection_,
             backward_projection_;

  // viewport
  int left_, bottom_, width_, height_;

  // gl context
  int glwidth_, glheight_;

  // projection
  double near_plane_, far_plane_;

  // colors & materials
  Vec4f   clear_color_,
          base_color_,
          ambient_color_,
          diffuse_color_,
          specular_color_;

  float shininess_;

  // thickness
  float  point_size_, line_width_;

  // lighting
  bool twosided_lighting_;

  // Multisampling settings
  bool multisampling_;
  bool allow_multisampling_;
  
  int num_texture_units_;
  
  // Mipmapping settings
  bool mipmapping_;
  
  // helper: should GL matrices be updated
  bool updateGL_;

  // true -> draw tranparent Objects
  bool blending_;

  // time since last redraw
  unsigned int msSinceLastRedraw_;

  // stack for color picking
  ColorStack colorStack_;

  // are we using color picking
  bool colorPicking_;
  
  // depth comparison function (GL_LESS by default)
  GLenum depth_func_;
  bool depthFuncLock_;


  // glEnable / glDisable states locker
  // iff a bit is set for a state, it is locked
  std::bitset<0xFFFF+1> glStateLock_;

  // glEnable / glDisable states
  // iff a bit is set for a state, it is enabled in OpenGL
  std::bitset<0xFFFF+1> glStateEnabled_;

  // element 0: sfactor, 1: dfactor
  GLenum blendFuncState_[2];
  bool blendFuncLock_;


  GLenum blendEquationState_;
  bool blendEquationLock_;

  GLclampf blendColorState_[4];
  bool blendColorLock_;

  GLenum alphaFuncState_;
  GLclampf alphaRefState_;
  bool alphaFuncLock_;


  // 4 buffer targets:
  // GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER, GL_PIXEL_PACK_BUFFER, GL_PIXEL_UNPACK_BUFFER
  int glBufferTargetLock_[4];

  // current state of a buffer target
  GLuint glBufferTargetState_[4];


  // active texture unit: GL_TEXTUREi
  GLenum activeTexture_;

  // 16 texture stages, 4 targets
  // GL_TEXTURE_1D, GL_TEXTURE_2D, GL_TEXTURE_3D, GL_TEXTURE_CUBE_MAP
  int glTextureTargetLock_[16][4];

  // current state of a texture target
  GLuint glTextureTargetState_[16][4];


  // graphics card caps, retrieved via glGet
  int maxTextureCoords_;
  int maxCombinedTextureImageUnits_;
};

/** \page pickingDocumentation Picking in the ACG SceneGraph

\section Overview
Picking is the operation to find an object in the 3D scene that is visible at a specific position. Usually you click
with the mouse into the scene and want to get the node index of the object you clicked on and possibly the specific
primitive you hit.

Picking in the ACG Scenegraph is done as color picking. Each node in the scenegraph has functions that are used
only when the SceneGraph is traversed in picking mode (  ACG::SceneGraph::traverse() with ACG::SceneGraph::PickAction ).
These functions render the scene in this special mode, and all objects and their primitives are rendered with a
different color.

After rendering, the color at the position, where a mouse click is done, is read. The color is than split into the object
and the primitive id used while rendering. Therefore the developer can map the numbers back to what the user clicked on
or what is visible at the specific location.

\section Rendering
For rendering the color picking, you have to use the ACG::SceneGraph::traverse() function with the
ACG::SceneGraph::PickAction. The Action will than traverse the SceneGraph and each of the nodes do their rendering.

The PickAction traverses the SceneGraph in the same hierarchy as the draw function. But for entering, it calls
ACG::SceneGraph::BaseNode::enterPick() and for leaving ACG::SceneGraph::BaseNode::leavePick(). The actual drawing is
done in ACG::SceneGraph::BaseNode::pick() between these calls. The node index is set as the picking object name via
GLState::pick_push_name() and is valid during the ACG::SceneGraph::BaseNode::pick() function. Afterwards it is
reset via GLState::pick_pop_name(). The nodes do not need to handle this explicitly as it is done by the traversal.

\section pickingUsage Usage

\subsection pickingUsageInNodes Implementation in Nodes
If you implement your own node and need picking, you have to implement the functions ACG::SceneGraph::BaseNode::pick() and
if you need to change states ACG::SceneGraph::BaseNode::enterPick() and ACG::SceneGraph::BaseNode::leavePick().

In the pick function, you get the current state and the ACG::SceneGraph::PickTarget. The PickTarget defines the primitives
the user wants to pick. For a mesh, this would be vertices, edges or faces. With ACG::SceneGraph::PICK_ANYTHING you should
render all primitives, one after the other with increasing indices.

The main pick function usually splits the picking for each of the primitives in the object. You have to set
the maximum number of primitives that should be distinguished by the picking with the GLState::pick_set_maximum()
function:

\code
// The picking function
// Global object variable object_ assumed
void ExampleNode::pick(GLState& _state, PickTarget _target)
{

  // Switch based on the target
  switch (_target)
  {
    case PICK_VERTEX:
    {
      // Set the maximal number of primitives for this object
      // In this case the number of vertices
      _state.pick_set_maximum (object_.n_vertices());
      pick_vertices( _state);
      break;
    }

    case PICK_EDGE:
    {
      _state.pick_set_maximum (object_.n_edges());
      pick_edges(_state, 0);
      break;
    }

    case PICK_ANYTHING:
    {
      _state.pick_set_maximum (object_.n_vertices() + object_.n_edges());
      pick_vertices( _state);
      pick_edges( _state, object_.n_vertices() );
      break;
    }

    default:
      break;
  }
}
\endcode

The pick vertices function would than look like the following example.
You have to set the name for all of the primitives you want to render.
You can render multiple primitives with the same index. This means that
if you click on one of these primitives, they will return the same id.
This is useful, if one of your components you want to pick consists
of multiple parts (e.g. an arrow consisting of the cylinder and a cone)
and should be handled as one component for the picking.

\code
void ExampleNode::pick_vertices( GLState& _state )
{
  for (unsigned int i=0; i< object_.n_vertices(); ++i) {

    // Set the picking name of the next primitive.
    // This will be the target index returned by the picking.
    _state.pick_set_name (i);

    // Render an arbitrary number of primitives which will all get the same picking index.
    glBegin(GL_POINTS);
      // Render the vertex with number i (note that this number matches the picking index returned by the picking)
      glVertex3fv( object_.vertex(i) );
    glEnd();
  }

}
\endcode

For the edges the code would be a bit more complex. If they are picked with ACG::SceneGraph::PICK_EDGE they
will be handled as the vertices. If ACG::SceneGraph::PICK_ANYTHING is used, they will be used with the
vertices. Therefore the indices given to the edges have to start after the vertices, which is implemented
as an offset:

\code
void ExampleNode::pick_edges( GLState& _state, unsigned int _offset)
{
  for (unsigned int i=0; i<object_n_edges(); ++i) {

    // Set the name for the current edge
    _state.pick_set_name (i + _offset);

    // Render current edge
    glBegin(GL_LINES);
      glVertex3fv( object_.vertex(  i      % object_.n_vertices() ) );
      glVertex3fv( object_.vertex( (i + 1) % object_.n_vertices() ) );
    glEnd();

  }
}
\endcode

\subsection pickingRetrieval Getting the picking information
After rendering, you just retrieve the rgba color at the click position. This color can be converted to
the node id and the component:

\code
  // Color at the picked position
  ACG::Vec4uc rgba;

  // Read color from framebuffer at desired position into rgba
  std::vector<unsigned int> rv = glState_.pick_color_to_stack(rgba);

  // something wrong with the color stack
  if (rv.size () < 2)
    return;

  // Index of the picked Node in the SceneGraph ( This can be used to fetch the node from the scenegraph ).
  _nodeIdx   = rv[1];

  // This is the component of the object at the picked position which has been set with pick_set_name()
  _targetIdx = rv[0];
\endcode

To get the node with the returned index, you can use the ACG::SceneGraph::FindNodeAction.

\subsection pickingNEVER NEVER DO THIS
Do not enable lighting, shading, alpha,... basically everything that shades colors! The picking relies on
fixed colors. Otherwise the correspondence to the objects/components gets lost and will always fail in weird ways.

*/

#ifdef WIN32
	#pragma warning(push)
#endif

//=============================================================================
} // namespace ACG
//=============================================================================
#endif // ACG_GLSTATE_HH defined
//=============================================================================
