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
//   $Revision: $
//   $Author: $
//   $Date: $
//
//=============================================================================




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
//=============================================================================
