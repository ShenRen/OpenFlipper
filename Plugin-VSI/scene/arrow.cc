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

//== INCLUDES =================================================================

#include <QPainterPath>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>

#include <OpenFlipper/common/GlobalOptions.hh>

#include "arrow.hh"
#include "graphicsScene.hh"

//== NAMESPACES ===============================================================
namespace VSI {

//=============================================================================
//
//  CLASS VSI::Arrow - IMPLEMENTATION
//
//=============================================================================


/// Constructor
Arrow::Arrow (GraphicsScene *_scene, QGraphicsItem *_parent, Direction _dir) :
  QGraphicsPixmapItem (_parent),
  scene_ (_scene),
  dir_ (_dir),
  highlight_ (false)
{
  // load icon depending on direction
  switch (_dir)
  {
    case North:
      setPixmap (OpenFlipper::Options::iconDirStr()+OpenFlipper::Options::dirSeparator()+"go-up.png");
      break;
    case South:
      setPixmap (OpenFlipper::Options::iconDirStr()+OpenFlipper::Options::dirSeparator()+"go-down.png");
      break;
    case East:
      setPixmap (OpenFlipper::Options::iconDirStr()+OpenFlipper::Options::dirSeparator()+"go-right.png");
      break;
    case West:
      setPixmap (OpenFlipper::Options::iconDirStr()+OpenFlipper::Options::dirSeparator()+"go-left.png");
      break;
    case Center:
      setPixmap (OpenFlipper::Options::iconDirStr()+OpenFlipper::Options::dirSeparator()+"go-center.png");
      break;
  }

  setOpacity ((highlight_)? 0.5 : 0.2);

  setAcceptHoverEvents (true);

  connect (&timer_, SIGNAL (timeout ()), this, SLOT (timeout ()));
}

//------------------------------------------------------------------------------

/// Destructor
Arrow::~Arrow ()
{
}

//------------------------------------------------------------------------------

// make the widget opaque if the mouse is over it
void Arrow::hoverEnterEvent (QGraphicsSceneHoverEvent * /*_event*/)
{
  setOpacity (1.0);
}

//------------------------------------------------------------------------------

// make the widget transparent if the mouse leaves it
void Arrow::hoverLeaveEvent (QGraphicsSceneHoverEvent * /*_event*/)
{
  setOpacity ((highlight_)? 0.5 : 0.2);
}

//------------------------------------------------------------------------------

// move the scene on mouse press
void Arrow::mousePressEvent (QGraphicsSceneMouseEvent *_event)
{
  _event->accept ();

  qreal dx = 0.0, dy = 0.0;

  switch (dir_)
  {
    case North:
      dy = 5;
      break;
    case South:
      dy = -5;
      break;
    case East:
      dx = -5;
      break;
    case West:
      dx = 5;
      break;
    case Center:
      {
        QRectF bb = scene_->elementsBoundingRect ();
        QRectF rect = scene_->sceneRect();
        dx = qMax (qMin ((rect.center () - bb.center ()).x (), 5.0), -5.0);
        dy = qMax (qMin ((rect.center () - bb.center ()).y (), 5.0), -5.0);
      }
      break;
  }
  scene_->moveElements (dx, dy, true);

  // start timer for movement during the mouse is pressed
  timer_.start (500);
  moveSelected_ = true;
}

//------------------------------------------------------------------------------

// stop movement on release
void Arrow::mouseReleaseEvent (QGraphicsSceneMouseEvent *_event)
{
  _event->accept ();
  timer_.stop ();
}

//------------------------------------------------------------------------------

// move on mouse wheel
void Arrow::wheelEvent (QGraphicsSceneWheelEvent *_event)
{
   _event->accept ();

  qreal dx = 0.0, dy = 0.0;
  
  switch (dir_)
  {
    case North:
    case South:
      dy = 1;
      break;
    case East:
    case West:
      dx = 1;
      break;
    case Center:
      break;
  }
  dx *= _event->delta () / 4;
  dy *= _event->delta () / 4;
  scene_->moveElements (dx, dy, true);
}

//------------------------------------------------------------------------------

// move the scene on timeout and decrease timer interval for faster movement
void Arrow::timeout ()
{
  if (timer_.interval () == 500)
    interval_ = 50;
  timer_.setInterval (interval_ );
  if (interval_ > 5)
    interval_--;

  qreal dx = 0.0, dy = 0.0;
  
  switch (dir_)
  {
    case North:
      dy = 1;
      break;
    case South:
      dy = -1;
      break;
    case East:
      dx = -1;
      break;
    case West:
      dx = 1;
      break;
    case Center:
      {
        QRectF bb = scene_->elementsBoundingRect ();
        QRectF rect = scene_->sceneRect();
        dx = qMax (qMin ((rect.center () - bb.center ()).x (), 2.0), -2.0);
        dy = qMax (qMin ((rect.center () - bb.center ()).y (), 2.0), -2.0);
      }
      break;
  }
  scene_->moveElements (dx, dy, moveSelected_);
}

//------------------------------------------------------------------------------

/// Activates the timer for movement (will be called if an element is moved above)
void Arrow::activate ()
{
  setOpacity (1.0);
  moveSelected_ = false;
  if (!timer_.isActive ())
    timer_.start (500);
}

//------------------------------------------------------------------------------

/// Stop the timer
void Arrow::reset ()
{
  setOpacity ((highlight_)? 0.5 : 0.2);
  timer_.stop ();
}

//------------------------------------------------------------------------------

// set geometry
void Arrow::setGeometry (const QRectF &_rect)
{
  QGraphicsPixmapItem::setPos (_rect.topLeft ());
  QGraphicsLayoutItem::setGeometry (_rect);
}

//------------------------------------------------------------------------------

// return size information for layouting
QSizeF Arrow::sizeHint (Qt::SizeHint _which, const QSizeF &/*_constraint*/) const
{
  QSizeF sh;
  switch (_which) {
    case Qt::MinimumSize:
    case Qt::PreferredSize:
    case Qt::MaximumSize:
      sh = QSizeF (pixmap ().width (), pixmap ().height ());
      break;
    default:
      break;
  }

  return sh;
}

//------------------------------------------------------------------------------

/// Highlights the widget if the scene can be moved in this direction
void Arrow::setHighlight (bool _highlight)
{
  highlight_ = _highlight;

  if (opacity () != 1.0)
    setOpacity ((highlight_)? 0.5 : 0.2);

}

//------------------------------------------------------------------------------
}

