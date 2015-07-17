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
//  CLASS GlutViewer - IMPLEMENTATION
//
//=============================================================================

//== INCLUDES =================================================================


#include "GlutViewer.hh"
#include <cstdio>


//== NAMESPACES ===============================================================


namespace ACG {


//== IMPLEMENTATION ========================================================== 


std::map<int, GlutViewer*>  GlutViewer::windows__;


//-----------------------------------------------------------------------------


GlutViewer::GlutViewer(const char* _title, int _width, int _height) :
        width_(_width),
        height_(_height),
        fullscreen_(false),
        bak_left_(0),
        bak_top_(0),
        bak_width_(0),
        bak_height_(0)
{
  // create window
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE | GLUT_ALPHA);
  glutInitWindowSize(_width, _height);
  glViewport(0, 0, _width, _height); 
  windowID_ = glutCreateWindow(_title);
  windows__[windowID_] = this;


  // register callbacks
  glutDisplayFunc(display__);
  glutKeyboardFunc(keyboard__);
  glutSpecialFunc(special__);
  glutMouseFunc(mouse__);
  glutMotionFunc(motion__);
  glutPassiveMotionFunc(passivemotion__);
  glutReshapeFunc(reshape__); 
  glutVisibilityFunc(visibility__);


  // init GL
  init();
}
  

//-----------------------------------------------------------------------------


GlutViewer::
~GlutViewer()
{
  glutDestroyWindow(windowID_);
}


//-----------------------------------------------------------------------------


void
GlutViewer::init()
{
  // init GL state
  glstate_.initialize();


  // OpenGL state
  ACG::GLState::enable(GL_DEPTH_TEST);
  ACG::GLState::enable(GL_LIGHTING);
  ACG::GLState::disable(GL_DITHER);
  ACG::GLState::shadeModel(GL_FLAT);
  glFrontFace(GL_CCW);


  // light sources
  glLoadIdentity();
  GLfloat pos[4], col[4];
  col[0] = col[1] = col[2] = 0.6f;
  pos[3] = 0.0f;
  col[3] = 1.0f;

#define SET_LIGHT(i,x,y,z) { 			\
  pos[0]=x; pos[1]=y; pos[2]=z;			\
  glLightfv(GL_LIGHT##i, GL_POSITION, pos);	\
  glLightfv(GL_LIGHT##i, GL_DIFFUSE,  col);	\
  glLightfv(GL_LIGHT##i, GL_SPECULAR, col);	\
  ACG::GLState::enable(GL_LIGHT##i);			\
}

  SET_LIGHT(0,  0.0f,  0.0f, 1.0f);
  SET_LIGHT(1, -1.0f,  1.0f, 0.7f);
  SET_LIGHT(2,  1.0f,  1.0f, 0.7f);



  // projection
  near_ = 0.1f;
  far_  = 100.0f;
  fovy_ = 45.0f;
  update_projection();
  glstate_.viewport(0, 0, width_, height_);
  glstate_.translate(0,0,-3);
}


//-----------------------------------------------------------------------------


void 
GlutViewer::update_projection()
{
  glstate_.reset_projection();
  glstate_.perspective(fovy_, 
		       (GLfloat) width_ / (GLfloat) height_,
		       near_, far_);
}


//-----------------------------------------------------------------------------


GlutViewer* GlutViewer::current_window() { 
  return windows__[glutGetWindow()]; 
}

void GlutViewer::display__(void) {
  current_window()->display();
}

void GlutViewer::idle__(void) {
  current_window()->idle();
} 

void GlutViewer::keyboard__(unsigned char key, int x, int y) {
  current_window()->keyboard((int)key, x, y);
}

void GlutViewer::motion__(int x, int y) {
  current_window()->motion(x, y);
}

void GlutViewer::mouse__(int button, int state, int x, int y) {
  current_window()->mouse(button, state, x, y);
}

void GlutViewer::passivemotion__(int x, int y) {
  current_window()->passivemotion(x, y);
}

void GlutViewer::reshape__(int w, int h) {
  current_window()->reshape(w, h);
}

void GlutViewer::special__(int key, int x, int y) {
  current_window()->keyboard(key, x, y);
}   

void GlutViewer::visibility__(int visible) {
  current_window()->visibility(visible);
}


//-----------------------------------------------------------------------------


void GlutViewer::idle(void) {
} 

void GlutViewer::keyboard(int key, int /* x */ , int /* y */ ) 
{
  switch (key) {
    case 27: {
      exit(0);
    }

    case GLUT_KEY_F12: {
      if (!fullscreen_) {
        bak_left_ = glutGet(GLUT_WINDOW_X);
        bak_top_ = glutGet(GLUT_WINDOW_Y);
        bak_width_ = glutGet(GLUT_WINDOW_WIDTH);
        bak_height_ = glutGet(GLUT_WINDOW_HEIGHT);
        glutFullScreen();
        fullscreen_ = true;
      } else {
        glutReshapeWindow(bak_width_, bak_height_);
        glutPositionWindow(bak_left_, bak_top_);
        fullscreen_ = false;
      }
      break;
    }
  }
} 

void GlutViewer::motion(int /* x */ , int /* y */ ) {
}

void GlutViewer::mouse(int /* button */ , int /* state */ , int /* x */ , int /* y */ ) {
}

void GlutViewer::passivemotion(int /* x */ , int /* y */ ) {
}

void GlutViewer::visibility(int /* visible */ ) {
}

void GlutViewer::reshape(int w, int h) 
{
  width_=w; height_=h;
  glstate_.viewport(0, 0, width_, height_);

  glstate_.reset_projection();
  glstate_.perspective(fovy_, 
		       (GLfloat) width_ / (GLfloat) height_,
		       near_, far_);

  glutPostRedisplay();
}

void GlutViewer::display(void) 
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  draw();
  glutSwapBuffers();
}


//=============================================================================
} // namespace ACG
//=============================================================================
