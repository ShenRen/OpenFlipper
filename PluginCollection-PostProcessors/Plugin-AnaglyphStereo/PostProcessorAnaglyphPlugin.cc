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
*   $Revision: 19248 $                                                       *
*   $LastChangedBy: moebius $                                                *
*   $Date: 2014-07-21 14:27:08 +0200 (Mon, 21 Jul 2014) $                     *
*                                                                            *
\*===========================================================================*/

#include "PostProcessorAnaglyphPlugin.hh"

#include <iostream>
#include <ACG/GL/GLState.hh>
#include <ACG/GL/gl.hh>
#include <ACG/GL/ScreenQuad.hh>
#include <ACG/GL/ShaderCache.hh>

#include <OpenFlipper/BasePlugin/PluginFunctions.hh>
#include <OpenFlipper/common/GlobalOptions.hh>


PostProcessorAnaglyphPlugin::PostProcessorAnaglyphPlugin()
{
}

PostProcessorAnaglyphPlugin::~PostProcessorAnaglyphPlugin()
{
}

QString PostProcessorAnaglyphPlugin::checkOpenGL() {
  if (!ACG::openGLVersion(3, 0))
    return QString("Insufficient OpenGL Version! OpenGL 3.0 or higher required");

  return QString("");
}



QString PostProcessorAnaglyphPlugin::postProcessorName() {
  return QString("AnaglyphStereo");
}


void PostProcessorAnaglyphPlugin::postProcess(ACG::GLState* _glstate, const std::vector<const PostProcessorInput*>& _input, const PostProcessorOutput& _output) {

  if (_input.size() != 2) {
    std::cerr << "PostProcessorAnaglyphPlugin: two input images required!" << std::endl;
    return;
  }

  // ======================================================================================================
  // Fetch shader from cache
  // ======================================================================================================

  QStringList macros;

  if (OpenFlipper::Options::stereoMode () == OpenFlipper::Options::AnaglyphCustom)
    macros.push_back("#define ANAGLYPH_CUSTOM");

  GLSL::Program* shader = ACG::ShaderCache::getInstance()->getProgram("ScreenQuad/screenquad.glsl", "AnaglyphStereo/anaglyph.glsl", &macros);

  // ======================================================================================================
  // Bind input texture
  // ======================================================================================================

  _input[1]->bindColorTex(1);
  _input[0]->bindColorTex(0);

  // ======================================================================================================
  // Bind output FBO
  // ======================================================================================================

  _output.bind();

  // ======================================================================================================
  // Setup render states
  // ======================================================================================================

  glDepthMask(1);
  glColorMask(1,1,1,1);

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);

  // ======================================================================================================
  // Setup shader
  // ======================================================================================================

  shader->use();
  shader->setUniform("SceneLeft", 0);
  shader->setUniform("SceneRight", 1);

  if (OpenFlipper::Options::stereoMode () == OpenFlipper::Options::AnaglyphCustom)
  {
    // column major
    std::vector<float> le = OpenFlipper::Options::anaglyphLeftEyeColorMatrix();
    std::vector<float> re = OpenFlipper::Options::anaglyphRightEyeColorMatrix();

    ACG::GLMatrixf ml, mr;

    for (int r = 0; r < 3; ++r)
    {
      for (int c = 0; c < 3; ++c)
      {
        ml(r,c) = le[c*3 + r];
        mr(r,c) = re[c*3 + r];
      }
    }

    shader->setUniformMat3("EyeColMatrixLeft", ml);
    shader->setUniformMat3("EyeColMatrixRight", mr);
  }

  // ======================================================================================================
  // Execute
  // ======================================================================================================

  ACG::ScreenQuad::draw(shader);


  shader->disable();
}


#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2( postprocessoranaglyphstereoplugin , PostProcessorAnaglyphPlugin );
#endif

