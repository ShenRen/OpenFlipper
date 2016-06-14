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
//  CLASS QtGLGraphicsScene - IMPLEMENTATION
//
//=============================================================================

//== INCLUDES =================================================================

#include "QtGLGraphicsScene.hh"
#include <QGraphicsSceneMouseEvent>
#include <QApplication>
#include <QPaintEngine>

//== NAMESPACES ===============================================================

namespace ACG {
namespace QtWidgets {


//== IMPLEMENTATION ===========================================================

QtGLGraphicsScene::QtGLGraphicsScene(QtBaseViewer* _w) :
  QGraphicsScene (),
  w_(_w)
{
}

 
void QtGLGraphicsScene::drawBackground(QPainter *_painter, const QRectF &_rect)
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
  
  // From now on we do OpenGL direct painting on the scene
  #if QT_VERSION >= 0x040600
  // Tell Qt that we directly use OpenGL
  _painter->beginNativePainting();
  #endif

  // Clear the depth buffer (This is required since QT4.6 Otherwise we get an emtpty scene!
  glClear(GL_DEPTH_BUFFER_BIT);
  
  w_->paintGL();

      #if QT_VERSION >= 0x040600
  // The rest is painting through QT again. 
  _painter->endNativePainting();
  #endif
}


void QtGLGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent* _e)
{
  QGraphicsScene::mousePressEvent(_e);
  if (_e->isAccepted())
    return;

  QPoint p (_e->scenePos().x(), _e->scenePos().y());
  QMouseEvent me(QEvent::MouseButtonPress ,p, _e->screenPos(), _e->button(),
	         _e->buttons(), _e->modifiers());
  w_->glMousePressEvent(&me);
  _e->accept();
}

void QtGLGraphicsScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* _e)
{
  QGraphicsScene::mouseDoubleClickEvent(_e);
  if (_e->isAccepted())
    return;

  QPoint p (_e->scenePos().x(), _e->scenePos().y());
  QMouseEvent me(QEvent::MouseButtonDblClick ,p, _e->screenPos(), _e->button(),
	         _e->buttons(), _e->modifiers());
  w_->glMouseDoubleClickEvent(&me);
  _e->accept();
}

void QtGLGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* _e)
{
  QGraphicsScene::mouseReleaseEvent(_e);
  if (_e->isAccepted())
    return;

  QPoint p (_e->scenePos().x(), _e->scenePos().y());
  QMouseEvent me(QEvent::MouseButtonRelease ,p, _e->screenPos(), _e->button(),
	         _e->buttons(), _e->modifiers());
  w_->glMouseReleaseEvent(&me);
  _e->accept();
}

void QtGLGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent* _e)
{
  QGraphicsScene::mouseMoveEvent(_e);
  if (_e->isAccepted())
    return;

  QPoint p (_e->scenePos().x(), _e->scenePos().y());
  QMouseEvent me(QEvent::MouseMove ,p, _e->screenPos(), _e->button(),
	         _e->buttons(), _e->modifiers());
  w_->glMouseMoveEvent(&me);
  _e->accept();
}

void QtGLGraphicsScene::wheelEvent(QGraphicsSceneWheelEvent* _e)
{
  QGraphicsScene::wheelEvent(_e);
  if (_e->isAccepted())
    return;
  QPoint p (_e->scenePos().x(), _e->scenePos().y());
  QWheelEvent we(p, _e->screenPos(), _e->delta(), _e->buttons(),
		 _e->modifiers(), _e->orientation());
  w_->glMouseWheelEvent(&we);
  _e->accept();
}

void QtGLGraphicsScene::keyPressEvent(QKeyEvent* _e)
{
  QGraphicsScene::keyPressEvent(_e);
  if (_e->isAccepted())
    return;
  w_->glKeyPressEvent (_e);
}

void QtGLGraphicsScene::keyReleaseEvent(QKeyEvent* _e)
{
  QGraphicsScene::keyReleaseEvent(_e);
  if (_e->isAccepted())
    return;
  w_->glKeyReleaseEvent (_e);
}

void QtGLGraphicsScene::contextMenuEvent(QGraphicsSceneContextMenuEvent* _e)
{
  QGraphicsScene::contextMenuEvent(_e);
  if (_e->isAccepted())
    return;

  QPoint p (_e->scenePos().x(), _e->scenePos().y());
  QContextMenuEvent ce(static_cast<QContextMenuEvent::Reason> (_e->reason()),
		       p, _e->screenPos(), _e->modifiers());
  w_->glContextMenuEvent(&ce);
  _e->accept();
}

void QtGLGraphicsScene::dragEnterEvent(QGraphicsSceneDragDropEvent* _e)
{
  QGraphicsScene::dragEnterEvent(_e);
  if (_e->isAccepted())
    return;

  QPoint p (_e->scenePos().x(), _e->scenePos().y());
  QDragEnterEvent de(p, _e->possibleActions(), _e->mimeData(), _e->buttons(),
		     _e->modifiers ());
  w_->glDragEnterEvent(&de);
  _e->accept();
}

void QtGLGraphicsScene::dropEvent(QGraphicsSceneDragDropEvent* _e)
{
  QGraphicsScene::dropEvent(_e);
  if (_e->isAccepted())
    return;
  QPoint p (_e->scenePos().x(), _e->scenePos().y());
  QDropEvent de(p, _e->possibleActions(), _e->mimeData(), _e->buttons(),
		_e->modifiers ());
  w_->glDropEvent(&de);
  _e->accept();
}

//=============================================================================
} // namespace QtWidgets
} // namespace ACG
//=============================================================================
