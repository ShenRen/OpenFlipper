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

#include <QObject>

#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/RenderInterface.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>

#include <ACG/Scenegraph/SceneGraph.hh>

#include <map>


class DepthPeelingPlugin : public QObject, BaseInterface, RenderInterface, LoggingInterface
{
  Q_OBJECT
    Q_INTERFACES(BaseInterface)
    Q_INTERFACES(RenderInterface)
    Q_INTERFACES(LoggingInterface)

#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "org.OpenFlipper.Plugins.Plugin-Render-Old-DepthPeeling")
#endif

signals:
  // LoggingInterface
    void log(Logtype _type, QString _message);
    void log(QString _message);


public :

  DepthPeelingPlugin();
  ~DepthPeelingPlugin();

  QString name() { return (QString("Dual Depth Peeling Plugin using classic rendering pipeling")); };
  QString description( ) { return (QString(tr("Order independent transparency renderer (classic pipeline)"))); };

public slots:
  QString version() { return QString("1.0"); };

private slots:

  //BaseInterface
  void initializePlugin();
  void exit();

  // RenderInterface
  void render(ACG::GLState* _glState, Viewer::ViewerProperties& _properties);
  QString rendererName();
  void supportedDrawModes(ACG::SceneGraph::DrawModes::DrawMode& _mode);
  QString checkOpenGL();

private:

  /// free all gl resources
  void destroyResources();

  /// free viewer specific gl resources
  void destroyResources(int _viewerId);

  /// reload gl resources
  void reloadResources(int _viewerId);

  /// draw a quad in projection space (only positions)
  void drawQuadProj(float _x0 = -1.0f, float _y0 = 1.0f, 
    float _w = 2.0f, float _h = 2.0f);

  /// find all light nodes in the scene
  void traverseLightNodes(BaseNode* _node);

  /// peel shader generator based on lights and texture mode
  void generatePeelingShaders(GLSL::StringList* _strVertexShaderOut,
    GLSL::StringList* _strFragmentShaderOut,
    GLSL::StringList* _strGeometryShaderOut,
    bool _textured, bool _flatShaded, bool _phong, bool _vertexColor, bool _wireFrame);

  /// regenerates peeling shaders based on light nodes in scenegraph
  void updatePeelingShaderSet();

  /// draw the current scene
  void drawScenePass(ACG::GLState* _glState, ACG::SceneGraph::DrawModes::DrawMode _drawMode, BaseNode* _sceneGraphRoot);
  
  void drawScenePeelPass(ACG::GLState* _glState, ACG::SceneGraph::DrawModes::DrawMode _drawMode, BaseNode* _sceneGraphRoot, int _peelPass);
  
  template <class Action>
  void traverseDraw(BaseNode* _node, Action& _action, ACG::SceneGraph::DrawModes::DrawMode _globalDrawMode, int _pass, int _peelPass);
  
  template <class Action>
  bool traverseDrawApplyAction(BaseNode* _node, Action& _action, ACG::SceneGraph::DrawModes::DrawMode _globalDrawMode, int _pass, int _peelPass);

  /// converts a drawmode to the correct shading program index
  unsigned int getPeelShaderIndex(ACG::SceneGraph::DrawModes::DrawMode _drawMode);

private:
  
  struct ViewerResources
  {
    ViewerResources();

    /// viewer window width
    unsigned int glWidth_;

    /// viewer window height
    unsigned int glHeight_;

    /// render target width
    unsigned int rtWidth_; 
    /// render target height
    unsigned int rtHeight_;

    /// render target textures: {depth0, depth1,  front_blend0, front_blend1,   back_temp0, back_temp1,   back_blend}
    GLuint blendDualPeelTexID_[7];
    /// depth peeling fbo
    GLuint blendDualPeelFbo_;
  };

  std::map<int, ViewerResources> viewerRes_;

  /// shader resources
  GLSL::Shader* blendShaders_[8];

  /// depth peeling programs
  GLSL::Program* blendDualPeelProg_[4];

  // note that shader flags are only good easy shader indexing
  //  some combinations like phong + flat make no sense
  enum
  {
    PEEL_SHADER_WIREFRAME = 0, // wireframe is a special case here, not combinable with others
    PEEL_SHADER_HIDDENLINE = 1, // hiddenline is another special case, it renders geometry in background color
    PEEL_SHADER_TEXTURED = 0x1,
    PEEL_SHADER_PHONG = 0x2,
    PEEL_SHADER_GOURAUD = 0x4,
    PEEL_SHADER_FLAT = 0x8,
    PEEL_SHADER_VERTEXCOLORS = 0x10,
    PEEL_NUM_COMBINATIONS = 0x20,
    PEEL_SHADER_NUM_FLAGS = 5
  };
  /// generated shader set
  GLSL::Shader* peelShaders_[PEEL_NUM_COMBINATIONS*3]; // interleaved: vertex, geometry, fragment

  /// generates shader programs
  GLSL::Program* peelProgs_[PEEL_NUM_COMBINATIONS];

  /// fragment query
  GLuint blendQueryID_;

  /// current glState ptr for hiddenline rendering
  ACG::GLState* glStateTmp_;

  /// number of used lights in the scene
  GLuint numLights_;

  /// light type enumeration
  enum LightType
  {
    LIGHTTYPE_DIRECTIONAL = 0,
    LIGHTTYPE_POINT,
    LIGHTTYPE_SPOT
  };

  /// registered lights in the scene
  LightType lightTypes_[16];
  /// matching GL light id
  GLuint glLightIDs_[16];
};
