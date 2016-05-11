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
#include "PostProcessorInterface.hh"



PostProcessorInput::PostProcessorInput( GLuint _colTex, GLuint _depthTex, int _width, int _height) :
  colorTex_(_colTex), depthTex_(_depthTex), width(_width), height(_height), sampleCount_(0), texfmt_(0)
{
  view_.identity(); 
  proj_.identity();
  depthRange_[0] = 0.0; depthRange_[1] = 1.0f;
}


void PostProcessorInput::bindColorTex( int _texSlot /*= 0*/ ) const
{
  glActiveTexture(GL_TEXTURE0 + _texSlot);
  glBindTexture(sampleCount_ ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, colorTex_);
}

void PostProcessorInput::bindDepthTex( int _texSlot /*= 0*/ ) const
{
  glActiveTexture(GL_TEXTURE0 + _texSlot);
  glBindTexture(sampleCount_ ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, depthTex_);
}


PostProcessorOutput::PostProcessorOutput( GLuint _fbo /*= 0*/, GLuint _drawBuffer /*= 0*/, int _width /*= 0*/, int _height /*= 0*/, const GLint* _viewport /*= 0*/ ) : fbo_(_fbo), drawBuffer_(_drawBuffer), width(_width), height(_height)
{
  if (_viewport)
  {
    viewport_[0] = _viewport[0];
    viewport_[1] = _viewport[1];
    viewport_[2] = _viewport[2];
    viewport_[3] = _viewport[3];
  }
  else
  {
    viewport_[0] = viewport_[1] = 0;
    viewport_[2] = _width; 
    viewport_[3] = _height;
  }
}

void PostProcessorOutput::bind() const
{
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
  glDrawBuffer(drawBuffer_);
  glViewport(viewport_[0], viewport_[1], viewport_[2], viewport_[3]);
}
