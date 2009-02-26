//=============================================================================
//
//                               OpenFlipper
//        Copyright (C) 2008 by Computer Graphics Group, RWTH Aachen
//                           www.openflipper.org
//
//-----------------------------------------------------------------------------
//
//                                License
//
//  OpenFlipper is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
// 
//  OpenFlipper is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
// 
//  You should have received a copy of the GNU Lesser General Public License
//  along with OpenFlipper.  If not, see <http://www.gnu.org/licenses/>.
//
//-----------------------------------------------------------------------------
//
//   $Revision$
//   $Author$
//   $Date$
//
//=============================================================================




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
    trackball_(glstate_)
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
  glEnable(GL_LIGHTING);
  glShadeModel(GL_SMOOTH);
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

  near_  = 0.01 * radius_;
  far_   = 10.0 * radius_;
  update_projection();

  view_all();
}


//-----------------------------------------------------------------------------


void 
GlutExaminer::view_all()
{
  ACG::Vec3f t = (-(glstate_.modelview().transform_point(center_))
		  - Vec3f(0.0, 0.0, 3.0*radius_));
  glstate_.translate(t[0], t[1], t[2], MULT_FROM_LEFT);
}


//-----------------------------------------------------------------------------


double 
GlutExaminer::measure_fps()
{
  const Vec3f t = glstate_.modelview().transform_point(center_);

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
