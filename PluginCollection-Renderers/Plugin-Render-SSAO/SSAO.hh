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

class SSAOPlugin : public QObject, BaseInterface, RenderInterface, LoggingInterface
{
  Q_OBJECT
    Q_INTERFACES(BaseInterface)
    Q_INTERFACES(RenderInterface)
    Q_INTERFACES(LoggingInterface)

#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "org.OpenFlipper.Plugins.Plugin-Render-SSAO")
#endif

    
signals:
  // LoggingInterface
    void log(Logtype _type, QString _message);
    void log(QString _message);


public :

  SSAOPlugin();
  ~SSAOPlugin();

  QString name() { return (QString("SSAO Plugin")); };
  QString description( ) { return (QString(tr("Screen Space Ambient Occlusion"))); };

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
  void reloadResources(int _viewerId, unsigned int _sceneTexWidth, unsigned int _sceneTexHeight);

  /// draw a quad in projection space (only positions)
  void drawQuadProj(float _x0 = -1.0f, float _y0 = 1.0f, 
    float _w = 2.0f, float _h = 2.0f);

  /// find all light nodes in the scene
  void traverseLightNodes(BaseNode* _node);

  /// peel shader generator based on lights and texture mode
  void generatePeelingShaders(GLSL::StringList* _strVertexShaderOut,
    GLSL::StringList* _strFragmentShaderOut, bool _textured);

  /// draw the current scene
  void drawScenePass(ACG::GLState* _glState, Viewer::ViewerProperties& _properties, BaseNode* _sceneGraphRoot);

  /// gaussian blur pass
  struct ViewerResources;
  void gaussianBlurPass(const ViewerResources* _pViewer, const float* _texelSize, 
    GLenum _targetAttachement, GLuint _srcTexture);

  /// computes a hemisphere sampling kernel in [0,1] range
  void generateSamplingKernel();
private:
  
  struct ViewerResources
  {
    ViewerResources();

    /// viewer window width
    unsigned int glWidth_;

    /// viewer window height
    unsigned int glHeight_;

    /// scene render target width
    unsigned int rtSceneWidth_; 
    /// scene render target height
    unsigned int rtSceneHeight_;

    /// render target width
    unsigned int rtWidth_; 
    /// render target height
    unsigned int rtHeight_;

    /// downsampled rt width
    unsigned int rtDownWidth_;
    /// downsampled rt height
    unsigned int rtDownHeight_;

    /// depth buffer render target
    GLuint depthBufTex_;

    /// scene normal buffer render target
    /// R8G8B8 format
    GLuint sceneNormalTex_;

    /// depth renderbuffer for ssaoFbo
    GLuint depthSSAORenderBuf_;

    /// depth renderbuffer for sceneFbo
    GLuint depthSceneRenderBuf_;

    /// standard scene without a render target
    GLuint sceneBufTex_;

    /// downsampled depth render target
    GLuint downsampledTex_;

    /// downsampled temp rt for intermediate results
    GLuint downsampledTmpTex_;

    /// occlusion render target
    GLuint occlusionTex_;

    /// sceneFbo for scene color rendering only (seperated for multisampling)
    /// attachment order: only scene color texture
    GLuint sceneFbo_;

    /// ssaoFbo for deferred rendering
    /// attachment order: depth, normal, occlusion
    GLuint ssaoFbo_;

    /// blurFbo for downsampling and gaussian blur filter
    /// attachment order: downsampled, downsampledTmp, occlusion
    GLuint blurFbo_;
  };

  std::map<int, ViewerResources> viewerRes_;

  /// shader resources
  GLSL::Shader* shaders_[10];

  enum
  { 
    PROG_INIT = 0,  // early Z pass + standard scene rendering
    PROG_DOWNSAMPLING,
    PROG_BLUR,
    PROG_SSAO,
    PROG_FINAL,
    PROG_FINAL_MSAA
  };
  
  /// shader programs
  GLSL::Program* programs_[6];

  /// random vector table for sample offset rotation
  GLuint randomVecTex_;

  /// number of samples
  static const unsigned int numSamples_;

  /// ssao sampling kernel
  ACG::Vec3f samplingKernel_[128];
};
