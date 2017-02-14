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




//=============================================================================
//
//  CLASS LightNode
//
//=============================================================================

#ifndef ACG_LIGHT_NODE_HH
#define ACG_LIGHT_NODE_HH


//== INCLUDES =================================================================

#include <ACG/Scenegraph/BaseNode.hh>
#include <OpenFlipper/common/ObjectTypeDLLDefines.hh>
#include <OpenFlipper/BasePlugin/PluginFunctions.hh>
#include <ACG/GL/gl.hh>
#include <ACG/GL/GLPrimitives.hh>
#include <OpenFlipper/common/GlobalOptions.hh>
#include <string>
#include <vector>


//== NAMESPACES ===============================================================

namespace ACG {
namespace SceneGraph {


//== CLASS DEFINITION =========================================================

/** \class LightNode LightNode.hh <ACG/Scenegraph/LightNode.hh>

    Set LightSources (0 to 7) for this node and all its children.
    All changes will be done in the enter() method undone
    in the leave() method.
**/

/// Structure to hold options for one LightSource
class OBJECTTYPEDLLEXPORT LightSource
{
  // Declare Node as friend for direct access to memberss
  friend class LightNode;

public:

  /// Default Constructor
  LightSource();

  /** \brief Set position for LightSource
  *
  * This function sets the position of a light source. OpenGL handles
  * a position for a light source if the homogeneous coordinate is
  * 1.0. If it is 0.0 the light source will be directional. So a
  * 1.0 is set in this function to get a positioned light source.
  */
  void position( Vec3d _pos);

  /** \brief Get the position of the LightSource
  *
  * Be careful with this function. Check if the light source is not directional with
  * the directional() function
  */
  Vec3d position() const;

  /** \brief Check if the light source is a directional light source
  *
  * If this light Source is a directional light source the function will return true.
  * Otherwise false as the light source has a position.
  */
  bool directional() const;

  /** \brief Get direction of the light source
  *
  * Get direction of light.
  */
  Vec3d direction() const;

  /** \brief Set the direction of the light source
  *
  * This function creates a directional light source. The position is not used by OpenGL.
  * Internally the positions homogeneous coordinate is set to 0.0.
  */
  void direction( Vec3d _pos);

   /// enable LightSource
  void enable();

  /// disable LightSource
  void disable();

  /// Get light source status
  bool enabled() const;

  /** \brief Set spot direction
  *
  * \todo Improve documentation
  */
  void spotDirection(Vec3d _pos);

  /** \brief get spot direction
  *
  * \todo Improve documentation
  */
  Vec3d spotDirection( ) const;

  /// set Ambient color for LightSource
  void ambientColor(  Vec4f _color);

  /// get Ambient color for LightSource
  Vec4f ambientColor() const;

  /// set Diffuse color for LightSource
  void diffuseColor(  Vec4f _color);

  /// get Diffuse color for LightSource
  Vec4f diffuseColor() const;

  /// set Specular color for LightSource
  void specularColor(  Vec4f _color);

  /// get Specular color for LightSource
  Vec4f specularColor() const;

  void setColor(const Vec4f& _ambient, const Vec4f& _diffuse, const Vec4f& _specular);

  /// make LightSource fixed or moveable with ModelViewMatrix
  void fixedPosition( bool _state);

  bool fixedPosition() const;

  void spotExponent(float _exponent);

  float spotExponent() const;

  void spotCutoff(float _cutoff);

  float spotCutoff() const;

  void constantAttenuation(float _constantAttenuation);

  float constantAttenuation() const;

  void linearAttenuation(float _linearAttenuation);

  float linearAttenuation() const;

  void quadraticAttenuation(float _quadraticAttenuation);

  float quadraticAttenuation() const;

  void brightness(const float _brightness);

  float brightness() const;

  /** \brief Get light source radius
  *
  * The light radius is the size of the lightsource. In OpenGL rendering this value is ignored.
  * You can use it for example in raytracers to implement soft shadows.
  */
  float radius() const { return radius_; }

  /** \brief Set light source radius
  *
  * The light radius is the size of the lightsource. In OpenGL rendering this value is ignored.
  * You can use it for example in raytracers to implement soft shadows.
  */
  void radius(const float _radius) { radius_ = _radius; }

protected:

  Vec4f position_;
  Vec4f realPosition_;

  bool enabled_;
  bool fixedPosition_;
  Vec4f ambientColor_;
  Vec4f diffuseColor_;
  Vec4f specularColor_;

  Vec3d spotDirection_;
  Vec3d realSpotDirection_;

  // Only if in fixed mode
  Vec4f initialPosition_;
  Vec3d initialSpotDirection_;
  bool initialPositionInit_;

  float spotExponent_;
  float spotCutoff_;
  float constantAttenuation_;
  float linearAttenuation_;
  float quadraticAttenuation_;

  float brightness_;

  /// When using ray tracing, light sources can have
  /// extent. We simplify to spherical light sources with
  /// radius radius_.
  float radius_;
};


class OBJECTTYPEDLLEXPORT LightNode : public BaseNode
{

public:

  /// Default constructor. Applies all properties.
  LightNode( BaseNode*           _parent = 0,
		   const std::string&  _name = "<LightNode>");

  /// Destructor.
  virtual ~LightNode();

  /// Set the light source parameters
  void setLightSource(LightSource _light ) { light_ = _light; };

  /// Get the light source parameters
  void getLightSource(LightSource* _light) const;

  /// Get transformed light source parameters
  /// where position and direction are in view space
  void getLightSourceViewSpace(LightSource* _light) const;


  ACG_CLASSNAME(LightNode);

  /// Get bounding box (for visualization purposes)
  void boundingBox(ACG::Vec3d& /* _bbMin */, ACG::Vec3d& /*_bbMax*/ );

  /// set current Light Sources
  void enter(GLState& _state, const DrawModes::DrawMode& _drawmode);

  /// restores original Light Sources
  void leave(GLState& _state, const DrawModes::DrawMode& _drawmode);

  /// Draw light source node
  void draw(GLState& _state, const DrawModes::DrawMode& _drawMode);

  /// Picking
  void pick(GLState& _state, PickTarget _target);

  /// Should node be visualized?
  bool visualize() { return visualize_; }

  /// Set whether node should be visualized
  void visualize(bool _visualize) { visualize_ = _visualize; }


  /// Add this light to shader render interface
  void getRenderObjects(IRenderer* _renderer, GLState&  _state , const DrawModes::DrawMode&  _drawMode , const Material* _mat);


private:

  /// set _light Options in OpenGL for GL_LIGHT#_index
  void setParameters(GLenum _index, LightSource& _light);

  /// get _light Options in OpenGL for GL_LIGHT#_index
  void getParameters(GLenum _index, LightSource& _light);

private:

  /// store LightSources of this node
  LightSource light_;

  /// pretransformed light position and direction in view space
  LightSource transformedLight_;

  /// save old LightSources
  LightSource lightSave_;

  /// Indicates whether light node should be visualized or not
  bool visualize_;

  /// Internal light id
  GLenum lightId_;

  ACG::GLCone* cone_;
  ACG::GLSphere* sphere_;
};

/**
* \class LightSourceHandle
*
* Assign a unique OpenGL light source identifier to each of the
* light source nodes in the scene. These relations are
* stored in a map. If there is no free light source left,
* return GL_INVALID_ENUM as light source enumerant.
*/
class OBJECTTYPEDLLEXPORT LightSourceHandle {
  public:
    LightSourceHandle() {
      GLint maxLights;

      // Get max number of lights
      if (OpenFlipper::Options::gui())
        glGetIntegerv(GL_MAX_LIGHTS, &maxLights);
      else
        maxLights = 50;

      for(int i = 0; i < maxLights; ++i) {
        lights_.insert(std::pair<GLenum, LightNode*>(GL_LIGHT0 + i, (LightNode*)0 ));
      }
    };

    GLenum getLight(LightNode* _node) {
      GLenum light = GL_INVALID_ENUM;
      for(std::map<GLenum, LightNode*>::iterator it = lights_.begin();
          it != lights_.end(); ++it) {

        if(it->second == 0) {
          lights_[it->first] = _node;
          light = it->first;
          break;
        }
      }
      return light;
    };

    void removeLight(LightNode* _node) {
      for(std::map<GLenum, LightNode*>::iterator it = lights_.begin();
          it != lights_.end(); ++it) {

        if(it->second == _node) {
          lights_[it->first] = 0;
          break;
        }
      }
    };

  private:
    std::map<GLenum, LightNode*> lights_;
};

//=============================================================================
} // namespace SceneGraph
} // namespace ACG
//=============================================================================
#endif // ACG_LIGHT_NODE_HH defined
//=============================================================================
