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
 *   $Revision$                                                       *
 *   $Author$                                                      *
 *   $Date$                   *
 *                                                                           *
\*===========================================================================*/


//=============================================================================


//== INCLUDES =================================================================

#include "gltext.hh"
#include <ACG/Math/GLMatrixT.hh>

//=============================================================================

namespace ACG {

//=============================================================================


void glText(const Vec3f&        _pos, 
	    const std::string&  _text,
	    void*               _font)
{
  glRasterPos3fv(_pos.data());
  
  std::string::const_iterator s_it(_text.begin()), s_end(_text.end());
  for (; s_it!=s_end; ++s_it)
    glutBitmapCharacter(_font, *s_it);
}


//-----------------------------------------------------------------------------


void glText( const Vec2i&        _pos,
	     const std::string&  _text,
	     void*               _font)
{
  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);

  // set raster pos
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  GLMatrixf orthoProj;
  orthoProj.identity();
  orthoProj.ortho(0.0f, float(viewport[2]), 0.0f, float(viewport[3]), -1.0f, 1.0f);
  glLoadMatrixf(orthoProj.data());


  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glRasterPos2i(_pos[0], _pos[1]);
  

  // draw characters
  std::string::const_iterator s_it(_text.begin()), s_end(_text.end());
  for (; s_it!=s_end; ++s_it)
    glutBitmapCharacter(_font, *s_it);


  // restore matrices
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}


//=============================================================================
}  // namespace ACG
//=============================================================================
