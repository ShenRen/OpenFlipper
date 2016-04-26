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




//=============================================================================
//
//  CLASS SimpleGLGraphicsScene - IMPLEMENTATION
//
//=============================================================================

//== INCLUDES =================================================================

#include <ACG/GL/acg_glew.hh>
#include <OpenFlipper/common/GlobalOptions.hh>
#include <OpenFlipper/BasePlugin/PluginFunctions.hh>
#include <OpenFlipper/widgets/glWidget/QtBaseViewer.hh>
#include <QApplication>
#include <QPainter>
#include <QPaintEngine>
#include <QGraphicsSceneMouseEvent>

#include "simpleGLGraphicsScene.hh"

//== NAMESPACES ===============================================================

//== IMPLEMENTATION ===========================================================

SimpleGLGraphicsScene::SimpleGLGraphicsScene () :
  QGraphicsScene (),
  view_(),
  initialized_(false)
{
  cursorPainter_ = new CursorPainter (this);
  cursorPainter_->setEnabled( OpenFlipperSettings().value("Core/Gui/glViewer/nativeMouse",false).toBool() );
}


void SimpleGLGraphicsScene::drawBackground(QPainter *_painter, const QRectF &_rect)
{
  
  // Check for switch in qt4.6 to OpenGL2
  #if QT_VERSION >= 0x040600
  if (_painter->paintEngine()->type() != QPaintEngine::OpenGL && _painter->paintEngine()->type() != QPaintEngine::OpenGL2 ) {
    std::cerr << "QtGLGraphicsScene: drawBackground needs a QGLWidget to be set as viewport on the graphics view\n";
    return;
  }
  #else
  if (_painter->paintEngine()->type() != QPaintEngine::OpenGL ) {
    std::cerr << "QtGLGraphicsScene: drawBackground needs a QGLWidget to be set as viewport on the graphics view\n";
    return;
  }
  #endif
  
  // Initialize background first
  _painter->setBackground(QApplication::palette().window());
  _painter->eraseRect(_rect);

  if (!view_)
    return;
  
  // From now on we do OpenGL direct painting on the scene
  #if QT_VERSION >= 0x040600
  // Tell Qt that we directly use OpenGL
  _painter->beginNativePainting();
  #endif

  if (!initialized_)
  {
    // we use glLoadGen to manage extensions
    // initialize it first
    #ifndef __APPLE__
    ogl_LoadFunctions();
    #endif
    view_->initializeGL();
    cursorPainter_->initializeGL ();
    initialized_ = true;
  }

  if (cursorPainter_->enabled())
  {
    // avoid projection matrix stack overflow
    GLdouble mat[16];
    glGetDoublev(GL_PROJECTION_MATRIX, mat);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix ();

    glPushAttrib (GL_ALL_ATTRIB_BITS);
    view_->updateCursorPosition(cursorPainter_->cursorPosition ());
    glPopAttrib ();

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd (mat);
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix ();
    glClear(GL_DEPTH_BUFFER_BIT);
  }
  
  // Clear the depth buffer (This is required since QT4.6 Otherwise we get an emtpty scene!
  glClear(GL_DEPTH_BUFFER_BIT);

  view_->paintGL();
  
  #if QT_VERSION >= 0x040600
  // The rest is painting through QT again. 
  _painter->endNativePainting();
  #endif
}


void SimpleGLGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent* _e)
{
  QGraphicsScene::mouseMoveEvent(_e);
  if (_e->isAccepted())
    return;

  if (view_)
    view_->mouseMoveEvent(_e);
}

void SimpleGLGraphicsScene::setView(glViewer * _view)
{
  view_ = _view;
  cursorPainter_->registerViewer (view_);
}

//-----------------------------------------------------------------------------

bool SimpleGLGraphicsScene::event(QEvent *_event)
{
  if (_event->type() == QEvent::Enter)
  {
    cursorPainter_->setMouseIn (true);
  }
  else if (_event->type() == QEvent::Leave)
  {
    cursorPainter_->setMouseIn (false);
    update ();
  }
  else if (cursorPainter_ && _event->type() == QEvent::GraphicsSceneMouseMove)
  {
    QGraphicsSceneMouseEvent *e = static_cast<QGraphicsSceneMouseEvent*>(_event);
    cursorPainter_->updateCursorPosition (e->scenePos ());
    update ();
  }
  return QGraphicsScene::event (_event);
}

//=============================================================================
//=============================================================================


