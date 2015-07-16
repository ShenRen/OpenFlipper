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

#ifndef ANIMATIONHANDLE_HH
#define ANIMATIONHANDLE_HH

/**
 * @brief A handle used to refer to an animation or to a specific frame in an animation
 */
class AnimationHandle
{
public:
  /// Constructs an invalid animation handle (interpreted as handle of the reference pose)
  AnimationHandle() : idAnimation(0), iFrame(0) {};
  /// Constructs a valid handle for the given animation and frame
  AnimationHandle(unsigned int idAnimation, unsigned int iFrame = 0) : idAnimation(idAnimation + 1), iFrame(iFrame) {};
  /// Returns true if the handle is valid
  inline bool isValid() const { return idAnimation > 0; }
  /// Returns the animation index (zero based)
  inline unsigned int animationIndex() const { return idAnimation - 1; }
  
  /// Returns the selected frame (zero based)
  inline unsigned int frame() const { return iFrame; }
  /// Sets the current animation frame (not failsafe)
  inline void setFrame(unsigned int _iFrame) { iFrame = _iFrame; }

  /// Returns to the first frame
  inline void firstFrame() { iFrame = 0; }
  /// Increases the handle to the next frame (not failsafe)
  inline void operator++() { ++iFrame; }
  /// Decreases the handle to the previous frame (not failsafe)
  inline void operator--() { --iFrame; }

  inline bool operator==(const AnimationHandle &rhs) { return (idAnimation == rhs.idAnimation) && (iFrame == rhs.iFrame); }
  inline bool operator!=(const AnimationHandle &rhs) { return (idAnimation != rhs.idAnimation) || (iFrame != rhs.iFrame); }

private:
  /// The one based index of the animation, set to 0 for invalid (or reference pose)
  unsigned int idAnimation;
  /// The frame number
  unsigned int iFrame;
};

#endif // ANIMATIONHANDLE_HH
