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
*   $Revision: 18127 $                                                       *
*   $LastChangedBy: moebius $                                                *
*   $Date: 2014-02-05 10:12:54 +0100 (Wed, 05 Feb 2014) $                     *
*                                                                            *
\*===========================================================================*/

#pragma once

#include <QObject>

#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/RenderInterface.hh>

#include <ACG/GL/IRenderer.hh>
#include <ACG/GL/FBO.hh>

class ToonRenderer : public QObject, BaseInterface, RenderInterface, ACG::IRenderer
{
  Q_OBJECT
    Q_INTERFACES(BaseInterface)
    Q_INTERFACES(RenderInterface)

#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "org.OpenFlipper.Plugins.Plugin-Render-Toon")
#endif

public:
  ToonRenderer();
  ~ToonRenderer();

  QString name() { return (QString("Toon Renderer Plugin")); };
  QString description( ) { return (QString(tr("Render with cel shading and edge outlining"))); };

public slots:
  QString version() { return QString("1.0"); };
  QString renderObjectsInfo(bool _outputShaderInfo);

  QAction* optionsAction();

private slots:

  //BaseInterface
  void initializePlugin();
  void exit(){}

  // RenderInterface
  void render(ACG::GLState* _glState, Viewer::ViewerProperties& _properties);
  QString rendererName() {return QString("Toon_Renderer");}
  void supportedDrawModes(ACG::SceneGraph::DrawModes::DrawMode& _mode) {_mode = ACG::SceneGraph::DrawModes::DEFAULT;}
  
  QString checkOpenGL();


  void actionDialog( bool );
  void paletteSizeChanged( int );
  void outlineColorChanged( QColor );

private:

  void loadShader();

  /// outline shader: multiply scene color with edge factor derived from edges in depth buffer
  GLSL::Program* progOutline_;

  /// size of cel shading palette
  float paletteSize_;

  /// outline color
  ACG::Vec3f outlineCol_;

  /// Collection of fbos for each viewport
  struct ViewerResources
  {
    ViewerResources() : scene_(0) {}
    ~ViewerResources() {delete scene_;}

    void resize( int _newWidth, int _newHeight );

    ACG::FBO* scene_;
  };

  /**
  * Stores fbo resources for each viewport.
  * Mapping: viewerID -> ViewerResources
  */
  std::map<int, ViewerResources> viewerRes_;
};

