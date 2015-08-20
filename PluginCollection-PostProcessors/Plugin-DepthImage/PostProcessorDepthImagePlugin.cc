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

#include <ACG/GL/acg_glew.hh>

#include "PostProcessorDepthImagePlugin.hh"

#include <iostream>
#include <ACG/GL/GLState.hh>
#include <ACG/GL/ScreenQuad.hh>

#include <OpenFlipper/BasePlugin/PluginFunctions.hh>
#include <OpenFlipper/common/GlobalOptions.hh>


PostProcessorDepthImagePlugin::PostProcessorDepthImagePlugin()
: shader_(0)
{

}


PostProcessorDepthImagePlugin::~PostProcessorDepthImagePlugin()
{
  delete shader_;
}

QString PostProcessorDepthImagePlugin::postProcessorName() {
  return QString("Show Depth Image");
}

QString PostProcessorDepthImagePlugin::checkOpenGL() 
{
  if ( ! ACG::openGLVersion(3, 0) )
    return QString("Insufficient OpenGL Version! OpenGL 3.0 or higher required");

  return QString("");
}


void PostProcessorDepthImagePlugin::postProcess(ACG::GLState* _glstate, const std::vector<const PostProcessorInput*>& _input, const PostProcessorOutput& _output) {

  // ======================================================================================================
  // Load shader if needed
  // ======================================================================================================
  if (!shader_)
    shader_ = GLSL::loadProgram("ShowDepth/screenquad.glsl", "ShowDepth/depth.glsl");

  // ======================================================================================================
  // Bind input texture
  // ======================================================================================================

  _input[0]->bindDepthTex(0);

  // ======================================================================================================
  // Bind output FBO
  // ======================================================================================================

  _output.bind();

  // ======================================================================================================
  // Setup shader
  // ======================================================================================================

  shader_->use();
  shader_->setUniform("textureSampler", 0);

  // ======================================================================================================
  // Execute
  // ======================================================================================================

  ACG::ScreenQuad::draw(shader_);

  shader_->disable();
}

#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2( postprocessordepthimageplugin , PostProcessorDepthImagePlugin );
#endif

