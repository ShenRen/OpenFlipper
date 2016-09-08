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



#include <ACG/Math/GLMatrixT.hh>
#include <ACG/GL/FBO.hh>
#include <ACG/GL/GLState.hh>

#include <OpenFlipper/BasePlugin/PostProcessorInterface.hh>


class PostProcessing
{
public:


  PostProcessing();
  virtual ~PostProcessing();

  /** \brief Number of post processors
  */
  int numProcessors() const;

  /** \brief Bind fbo for scene rendering
   *
   * Setup fbo and draw buffer as target for scene rendering.
   * Some post processing effects require special targets and setupScene() takes this into account.
   *
   * @param _viewerID OpenFlipper viewer id (viewport index)
   * @param _width texture width
   * @param _height texture height
   * @param _samples number of samples for multisampling
   * @param _stereoEye eye from which to render the scene: -1 (no stereo rendering), 0 (left eye), 1 (right eye)
   * @return negative (unchanged fbo ie. use current hardware fbo), 0 (fail), 1 (fbo has been changed)
  */
  int setupScene(int _viewerID, int _width, int _height, int _samples = 0, int _stereoEye = -1);


  /** \brief Perform all post processing
   *
   * Post processors might require modelview and projection matrix.
   * In stereo mode, the post processor chain is executed for each eye separately.
   * The resulting stereo buffers for each eye have to be resolved either by hardware OpenGL support or by manually calling resoleStereoAnaglyph() afterwards.
   *
   * @param _viewerID OpenFlipper viewer id (viewport index)
   * @param _modelview modelview matrix of the scene
   * @param _proj1 projection matrix 1 (left eye in stereo mode)
   * @param _proj2 projection matrix 2 (right eye in stereo mode)
   * @param _hwOpenGLStereo hardware stereo support should be used to resolve the stereo buffers (GL_BACK_LEFT, GL_BACK_RIGHT)
  */
  void postProcess(int _viewerID, ACG::GLState* _glstate, const ACG::GLMatrixd& _modelview, const ACG::GLMatrixd& _proj1, const ACG::GLMatrixd& _proj2, bool _hwOpenGLStereo = false);


  /** \brief Resolve stereo buffers as anaglyph
   *
   * Final textures are combined to an anaglyph image.
   *
   * @param _viewerID OpenFlipper viewer id (viewport index)
  */
  void resolveStereoAnyglyph(int _viewerID);


private:


  void setupFBO(ACG::FBO* _dst, int _width, int _height, GLuint _intfmt, int _samples);

  void resolveMultisampling(ACG::FBO* _dst, ACG::FBO* _src);

private:

  /// perform extra checks for debugging: 0 -> no checks, 1 -> fast checks, 2 -> more checks etc.
  int debugLevel_;

  /// FBO for scene rendering, one for each eye in stereo mode, possibly multisampled
  ACG::FBO sceneFBO_[2];

  /// post-processing read/write FBOs,  no msaa
  ACG::FBO procFBO_[2];

  /// store post processing results for each stereo eyes, input for anaglyph processor,  no msaa
  ACG::FBO stereoFBO_[2];


  /// backbuffer output,  not owned by this class
  GLuint backbufferFBO_,
    backbufferTarget_;
  GLfloat backbufferViewport_[4];

  /// postprocessing in stereo mode
  bool stereoMode_;
};
