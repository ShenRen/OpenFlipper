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
//
//  CLASS GlutExaminer - IMPLEMENTATION
//
//=============================================================================

//== INCLUDES =================================================================


#include "GlutExaminer.hh"
#include "../Utils/StopWatch.hh"


//== NAMESPACES ===============================================================


namespace ACG {


//== IMPLEMENTATION ========================================================== 


GlutExaminer::
GlutExaminer(const char* _title, int _width, int _height)
  : GlutViewer(_title, _width, _height),
    trackball_(glstate_),
    center_(Vec3f(0.0,0.0,0.0)),
    radius_(1.0)
{
  init();
}


//-----------------------------------------------------------------------------


void 
GlutExaminer::init()
{
  trackball_.set_center(Vec3f(0,0,0));
}


//-----------------------------------------------------------------------------


void 
GlutExaminer::draw()
{
  ACG::GLState::enable(GL_LIGHTING);
  ACG::GLState::shadeModel(GL_SMOOTH);
  glutSolidTeapot(0.5);
}


//-----------------------------------------------------------------------------


void 
GlutExaminer::mouse(int button, int state, int x, int y)
{
  if (state == GLUT_DOWN)  trackball_.mouse_press(button, x, y);
  else                     trackball_.mouse_release(button, x, y);
  glutPostRedisplay();
}


//-----------------------------------------------------------------------------


void 
GlutExaminer::motion(int x, int y)
{
  trackball_.mouse_move(x, y);
  glutPostRedisplay();
}


//-----------------------------------------------------------------------------


void 
GlutExaminer::keyboard(int key, int x, int y) 
{
  switch (key)
  {
    case 'f':
    {
      std::cerr << "Performance test: ";
      double fps = measure_fps();
      std::cerr << fps << " FPS\n";
      break;
    }

    default:
    {
      GlutViewer::keyboard(key, x, y);
      break;
    }
  }
}


//-----------------------------------------------------------------------------


void 
GlutExaminer::setup_scene(const Vec3f& _center, float _radius)
{
  center_ = _center;
  radius_ = _radius;

  trackball_.set_center(_center);

  near_  = 0.01f * radius_;
  far_   = 10.0f * radius_;
  update_projection();

  view_all();
}


//-----------------------------------------------------------------------------


void 
GlutExaminer::view_all()
{
  ACG::Vec3d t = (-(glstate_.modelview().transform_point(center_))
		  - Vec3d(0.0f, 0.0f, 3.0f*radius_));
  glstate_.translate(t[0], t[1], t[2], MULT_FROM_LEFT);
}


//-----------------------------------------------------------------------------


double 
GlutExaminer::measure_fps()
{
  const Vec3d t = glstate_.modelview().transform_point(center_);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  StopWatch timer; timer.start();
	
  for (int i=0; i<72; ++i)
  {
    glstate_.translate(-t[0], -t[1], -t[2], MULT_FROM_LEFT);
    glstate_.rotate(5.0f, 0.0f, 1.0f, 0.0f, MULT_FROM_LEFT);
    glstate_.translate( t[0],  t[1],  t[2], MULT_FROM_LEFT);
    display();
  }
  glFinish();

  double elapsed = timer.stop();

  glPopMatrix();
  glutPostRedisplay();

  return (1000.0 / elapsed * 72.0);
}


//=============================================================================
} // namespace ACG
//=============================================================================
