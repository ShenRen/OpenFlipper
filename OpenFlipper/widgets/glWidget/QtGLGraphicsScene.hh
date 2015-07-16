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



#ifndef QTGLGRAPHICSSCENE_HH
#define QTGLGRAPHICSSCENE_HH

//=============================================================================
//
//  CLASS QtGLGraphicsScene - IMPLEMENTATION
//
//=============================================================================

//== INCLUDES =================================================================

#include <OpenFlipper/common/GlobalDefines.hh>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include "QtBaseViewer.hh"
#include "CursorPainter.hh"

//== FORWARDDECLARATIONS ======================================================
class glViewer;
class QtMultiViewLayout;
class CursorPainter;

//== NAMESPACES ===============================================================

//== CLASS DEFINITION =========================================================



/** OpenGL drawing area and widget scene -- for \a internal use only.
    The scene basically redirects calls to a
    ACG::QtWidgets::glViewer, the corresponding virtual methods there
    are prefixed with \c gl.
    \sa ACG::QtWidgets::glViewer
*/

class DLLEXPORT QtGLGraphicsScene : public QGraphicsScene
{
Q_OBJECT

public:
  QtGLGraphicsScene(std::vector< glViewer *> *_views, QtMultiViewLayout *_layout);

  void setCursorPainter (CursorPainter *_cursorPainter);
  
protected:

  virtual void drawBackground(QPainter *_painter, const QRectF &_rect);

  virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* _e);

  // Catch enter and leave events for 3d cursor
  virtual bool event (QEvent *_event);

private:

  glViewer* findView (const QPointF &_p, bool _setActive = false);

  std::vector< glViewer *> *views_;
  QtMultiViewLayout *layout_;

  CursorPainter *cursorPainter_;
};

#endif

//=============================================================================
//=============================================================================
