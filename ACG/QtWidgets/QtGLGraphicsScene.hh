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

#include <QGraphicsScene>
#include <QGraphicsItem>
#include "QtBaseViewer.hh"

//== NAMESPACES ===============================================================

namespace ACG {
namespace QtWidgets {


//== CLASS DEFINITION =========================================================


/** OpenGL drawing area and widget scene -- for \a internal use only.
    The scene basically redirects calls to a
    ACG::QtWidgets::QtBaseViewer, the corresponding virtual methods there
    are prefixed with \c gl.
    \sa ACG::QtWidgets::QtBaseViewer
*/

class QtGLGraphicsScene : public QGraphicsScene
{
Q_OBJECT

public:
  QtGLGraphicsScene(QtBaseViewer* _w);

protected:
  friend class QtBaseViewer;

  virtual void drawBackground(QPainter *_painter, const QRectF &_rect);

  virtual void mousePressEvent(QGraphicsSceneMouseEvent* _e);
  virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* _e);
  virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* _e);
  virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* _e);
  virtual void wheelEvent(QGraphicsSceneWheelEvent* _e);
  virtual void keyPressEvent(QKeyEvent* _e);
  virtual void keyReleaseEvent(QKeyEvent* _e);
  virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* _e);
  virtual void dragEnterEvent(QGraphicsSceneDragDropEvent* _e);
  virtual void dropEvent(QGraphicsSceneDragDropEvent* _e);

private:
  QtBaseViewer* w_;
};

//=============================================================================
} // namespace QtWidgets
} // namespace ACG
//=============================================================================
