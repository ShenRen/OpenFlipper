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
 *   $Revision: 18130 $                                                       *
 *   $Author: moebius $                                                      *
 *   $Date: 2014-02-05 10:41:16 +0100 (Wed, 05 Feb 2014) $                   *
 *                                                                           *
\*===========================================================================*/



//=============================================================================
//
//  OpenGL AntiAliasing methods
//
//=============================================================================


//== INCLUDES =================================================================

#include <ACG/GL/acg_glew.hh>
#include <ACG/GL/AntiAliasing.hh>
#include <ACG/ShaderUtils/GLSLShader.hh>
#include <ACG/GL/ScreenQuad.hh>

#include <ACG/GL/ShaderCache.hh>

//== DEFINES ==================================================================

// shader files
#define MSAA_SCREENQUAD_SHADER "ScreenQuad/screenquad.glsl"
#define MSAA_NEAREST_SHADER "MSAA/sample_nearest.glsl"
#define MSAA_NEAREST_DEPTH_SHADER "MSAA/sample_nearest_and_depth.glsl"
#define MSAA_LINEAR_SHADER "MSAA/sample_linear.glsl"



//== NAMESPACES ===============================================================

namespace ACG {


//== CLASS IMPLEMENTATION =====================================================

#ifdef GL_ARB_texture_multisample


MSFilterWeights::MSFilterWeights(int _numSamples) : numSamples_(_numSamples) {

  weights_.resize(_numSamples);
  float sumWeights = 0.0f;

  // texel center is at (0.5, 0.5) as specified in 
  //  http://www.opengl.org/sdk/docs/man3/xhtml/glGetMultisample.xml
  Vec2f texelCenter(0.5f, 0.5f);

  // query sample count of currently bound fbo to avoid error on calling glGetMultisample
  int fboSamples = 0;
  glGetIntegerv(GL_SAMPLES, &fboSamples);


  for (int i = 0; i < _numSamples; ++i) {
    GLfloat val[2];

    if (i < fboSamples)
      glGetMultisamplefv(GL_SAMPLE_POSITION, i, val);
    else
      val[0] = val[1] = 0.5f; // maybe output warning here
    
    Vec2f samplePosition(val[0], val[1]);

    // weighting based on distance to texel center
    float sampleDist = (samplePosition - texelCenter).norm();

    // samples close to the center are weighted higher than samples farther away
    weights_[i] = 1.0f - sampleDist;

    sumWeights += weights_[i];
  }

  // normalize weights

  for (int i = 0; i < _numSamples; ++i)
    weights_[i] /= sumWeights;
}

//=============================================================================

void MSFilterWeights::asTextureBuffer( TextureBuffer& out ) {
  if (numSamples_)
    out.setBufferData(numSamples_ * 4, &weights_[0], GL_R32F, GL_STATIC_DRAW);
}

//=============================================================================




//=============================================================================


MSTextureSampler::MSTextureSampler() : shaderNearest_(0), shaderNearestDepth_(0), shaderLinear_(0) {
}


MSTextureSampler::~MSTextureSampler() {
  delete shaderNearest_;
  delete shaderLinear_;
  delete shaderNearestDepth_;
}

MSTextureSampler& MSTextureSampler::instance() {
  static MSTextureSampler singleton;
  return singleton;
}

//=============================================================================

void MSTextureSampler::init() {
  if (!shaderNearest_)
    shaderNearest_ = GLSL::loadProgram(MSAA_SCREENQUAD_SHADER, MSAA_NEAREST_SHADER);

  if (!shaderNearestDepth_)
    shaderNearestDepth_ = GLSL::loadProgram(MSAA_SCREENQUAD_SHADER, MSAA_NEAREST_DEPTH_SHADER);

  if (!shaderLinear_)
    shaderLinear_ = GLSL::loadProgram(MSAA_SCREENQUAD_SHADER, MSAA_LINEAR_SHADER);
}

//=============================================================================

void MSTextureSampler::filterMSAATexture_Nearest( GLuint _texture, int _samples, const float* _weights /*= 0*/ ) {

  MSTextureSampler& sampler = instance();

  // load shader
  if (!sampler.shaderNearest_)
    sampler.init();

  GLSL::Program* shader = sampler.shaderNearest_;

  if (!shader)
    return;
  

  shader->use();

  // offset and scale of screenquad
  shader->setUniform("offset", Vec2f(0.0f, 0.0f));
  shader->setUniform("size", Vec2f(1.0f, 1.0f));
  
  // sample count and filter weights
  shader->setUniform("numSamples", _samples);

  // bind multisampled texture to slot 0
  shader->setUniform("inputTex", 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, _texture);

  // run texture filter
  ScreenQuad::draw(shader);

  shader->disable();
}

//=============================================================================

void MSTextureSampler::filterMSAATexture_Nearest( GLuint _texture, GLuint _depthTexture, int _samples, const float* _weights /*= 0*/ ) {

  MSTextureSampler& sampler = instance();

  // load shader
  if (!sampler.shaderNearestDepth_)
    sampler.init();

//  GLSL::Program* shader = sampler.shaderNearestDepth_;
  GLSL::Program* shader = ACG::ShaderCache::getInstance()->getProgram(MSAA_SCREENQUAD_SHADER, MSAA_NEAREST_DEPTH_SHADER);

  if (!shader)
    return;


  shader->use();

  // offset and scale of screenquad
  shader->setUniform("offset", Vec2f(0.0f, 0.0f));
  shader->setUniform("size", Vec2f(1.0f, 1.0f));

  // sample count and filter weights
  shader->setUniform("numSamples", _samples);

  // bind multisampled depth texture to slot 1
  shader->setUniform("inputDepthTex", 1);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, _depthTexture);

  // bind multisampled texture to slot 0
  shader->setUniform("inputTex", 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, _texture);

  // run texture filter
  ScreenQuad::draw(shader);

  shader->disable();
}

//=============================================================================

void MSTextureSampler::filterMSAATexture_Linear( GLuint _texture, int _samples, const float* _weights /*= 0*/ ) {

  MSTextureSampler& sampler = instance();

  // load shader
  if (!sampler.shaderLinear_)
    sampler.init();

  GLSL::Program* shader = sampler.shaderLinear_;

  if (!shader)
    return;


  shader->use();

  // offset and scale of screenquad
  shader->setUniform("offset", Vec2f(0.0f, 0.0f));
  shader->setUniform("size", Vec2f(1.0f, 1.0f));

  // sample count and filter weights
  shader->setUniform("numSamples", _samples);

  // bind multisampled texture to slot 0
  shader->setUniform("inputTex", 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, _texture);

  // run texture filter
  ScreenQuad::draw(shader);

  shader->disable();
}


//=============================================================================



#endif // GL_ARB_texture_multisample


//=============================================================================




//=============================================================================



SubpixelSupersampling::SubpixelSupersampling(int _width, int _height, int _resolutionIncrease, int _channels, float _sampleDistance, float _subpixelAreaWidth)
: width_(_width), height_(_height), 
resolutionIncrease_(_resolutionIncrease),
widthHi_(_width * resolutionIncrease_), heightHi_(_height * resolutionIncrease_),
channels_(_channels),
subpixels_(-1),
kernel_(0),
sampleDistance_(_sampleDistance),
subpixelAreaWidth_(_subpixelAreaWidth) {

  kernel_ = new PoissonBlurFilter(0.5f, _sampleDistance, 30, false);
  subpixels_ = kernel_->numSamples();

  composite_.resize(widthHi_ * heightHi_ * channels_, 0.0f);


  // compute number of subpixels in a group
  subpixelsPerGroup_.resize(resolutionIncrease_ * resolutionIncrease_, 0);

  for (int i = 0; i < subpixels_; ++i) {
  
    Vec2i group = subpixelGroup(i);
    int groupID = group[1] * resolutionIncrease_ + group[0];

    ++subpixelsPerGroup_[groupID];
  }
}

//=============================================================================

SubpixelSupersampling::~SubpixelSupersampling() {
  delete kernel_;
}

//=============================================================================

Vec2f SubpixelSupersampling::subpixelOffset(int i) const {
  // compute viewport offset for the subpixel
  Vec2f sample = kernel_->samples()[i];
  return sample * subpixelAreaWidth_;
}

//=============================================================================

ACG::Vec2i SubpixelSupersampling::subpixelGroup(int i) const {
  
  Vec2f offset = subpixelOffset(i) + Vec2f(0.5f, 0.5f);

  Vec2i group;

  for (int k = 0; k < 2; ++k) {
    // this gives the mirrored group id
    group[k] = int(offset[k] * float(resolutionIncrease_));

    // clamp to pixel area. 
    // if subpixelAreaWidth_ > 0 some samples are taken from the neighbors but are weighted in this group
    group[k] = std::max(group[k], 0);
    group[k] = std::min(group[k], resolutionIncrease_ - 1);

    group[k] = resolutionIncrease_ - 1 - group[k];
  }

  return group;
}

//=============================================================================

void SubpixelSupersampling::begin() {
  clearBuffer();

  glTexEnvi(GL_TEXTURE_FILTER_CONTROL, GL_TEXTURE_LOD_BIAS, 1 - resolutionIncrease_);
}

//=============================================================================

void SubpixelSupersampling::beginSubpixel(int i) {

  Vec2f offset = subpixelOffset(i);

  // use glViewportIndexedf because glViewport doesn't accept fractional offsets

  // save current viewport
  glGetFloatv(GL_VIEWPORT, prevViewport);

  // set viewport with subpixel offset
  glViewportIndexedf(0, offset[0], offset[1], width_, height_);
}

//=============================================================================

void SubpixelSupersampling::endSubpixel(int i) {

  // read framebuffer data
  std::vector<unsigned char> pixels(channels_ * width_ * height_);

  GLenum fmts[5] = { GL_NONE, GL_RED, GL_RG, GL_RGB, GL_RGBA };
  GLenum texformat = (channels_ < 0 || channels_ > 4) ? GL_NONE : fmts[channels_];

  glReadPixels(0, 0, width_, height_, texformat, GL_UNSIGNED_BYTE, &pixels[0]);
  

  // composite
  Vec2f offset = subpixelOffset(i);

  // find group of subpixel in the increased resolution
  Vec2i group = subpixelGroup(i);

  for (int y = 0; y < height_; ++y) {
    for (int x = 0; x < width_; ++x) {

      // subpixel id in high resolution image
      int x_hi = x * resolutionIncrease_ + group[0];
      int y_hi = y * resolutionIncrease_ + group[1];

     int pixel_hi = y_hi * widthHi_ + x_hi;


      // add pixel to group composite
      for (int c = 0; c < channels_; ++c)
        composite_[pixel_hi * channels_ + c] += float(pixels[(y * width_ + x) * channels_ + c]) / 255.0f;

      // track number of pixels per group
    }
  }
  
  // reset to previous viewport
  glViewportIndexedf(0, prevViewport[0], prevViewport[1], prevViewport[2], prevViewport[3]);
}

//=============================================================================

void SubpixelSupersampling::end() {

  // divide pixel value by number of samples
  for (int y = 0; y < height_; ++y) {
    for (int x = 0; x < width_; ++x) {

      for (int y_g = 0; y_g < resolutionIncrease_; ++y_g) {
        for (int x_g = 0; x_g < resolutionIncrease_; ++x_g) {

          int x_hi = x * resolutionIncrease_ + x_g;
          int y_hi = y * resolutionIncrease_ + y_g;
          int pixel_hi = y_hi * widthHi_ + x_hi;


          int groupID = y_g * resolutionIncrease_ + x_g;

          for (int c = 0; c < channels_; ++c)
            composite_[pixel_hi * channels_ + c] /= float(subpixelsPerGroup_[groupID]);
        }
      }

    }
  }


  glTexEnvi(GL_TEXTURE_FILTER_CONTROL, GL_TEXTURE_LOD_BIAS, 0);
}

//=============================================================================

void SubpixelSupersampling::clearBuffer() {
  memset(&composite_[0], 0, composite_.size() * sizeof(float));
}

//=============================================================================

} // namespace ACG
//=============================================================================
