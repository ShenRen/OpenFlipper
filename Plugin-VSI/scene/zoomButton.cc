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
#include <QPixmap>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>

#include <OpenFlipper/common/GlobalOptions.hh>

#include "zoomButton.hh"
#include "graphicsScene.hh"

//== NAMESPACES ===============================================================
namespace VSI {

//=============================================================================
//
//  CLASS VSI::ZoomButton - IMPLEMENTATION
//
//=============================================================================

/// Constructor
ZoomButton::ZoomButton (GraphicsScene *_scene, QGraphicsItem *_parent, Type _type) :
  QGraphicsPixmapItem (_parent),
  scene_ (_scene),
  type_ (_type)
{

  // Load icon depending on type
  switch (type_)
  {
    case In:
      setPixmap (OpenFlipper::Options::iconDirStr()+OpenFlipper::Options::dirSeparator()+"zoom-in.png");
      break;
    case Out:
      setPixmap (OpenFlipper::Options::iconDirStr()+OpenFlipper::Options::dirSeparator()+"zoom-out.png");
      break;
  }

  setOpacity (0.4);

  setAcceptHoverEvents (true);

  connect (&timer_, SIGNAL (timeout ()), this, SLOT (timeout ()));
}

//------------------------------------------------------------------------------

/// Destructor
ZoomButton::~ZoomButton ()
{
}

//------------------------------------------------------------------------------

// make the widget opaque if the mouse is over it
void ZoomButton::hoverEnterEvent (QGraphicsSceneHoverEvent * /*_event*/)
{
  setOpacity (1.0);
}

//------------------------------------------------------------------------------

// make the widget transparent if the mouse leaves it
void ZoomButton::hoverLeaveEvent (QGraphicsSceneHoverEvent * /*_event*/)
{
  setOpacity (0.4);
}

//------------------------------------------------------------------------------

// zoom the scene on mouse press
void ZoomButton::mousePressEvent (QGraphicsSceneMouseEvent *_event)
{
  _event->accept ();

  switch (type_)
  {
    case In:
      scene_->scaleElements (1.25);
      break;
    case Out:
      scene_->scaleElements (0.8);
      break;
  }

  QRectF rect = scene_->sceneRect ();
  pos_ = QPointF (rect.x () + (rect.width () / 2.0), rect.y () + (rect.height () / 2.0));

  // start timer for zooming during the mouse is pressed
  timer_.start (500);
}

//------------------------------------------------------------------------------

// stop zooming on release
void ZoomButton::mouseReleaseEvent (QGraphicsSceneMouseEvent *_event)
{
  _event->accept ();
  timer_.stop ();
}

//------------------------------------------------------------------------------

// zoom on mouse wheel
void ZoomButton::wheelEvent (QGraphicsSceneWheelEvent *_event)
{
   _event->accept ();

  qreal delta;

  if (_event->delta () > 0)
    delta = 1.25;
  else
    delta = 0.8;

  scene_->scaleElements (delta);
}

//------------------------------------------------------------------------------

// zoom the scene on timeout
void ZoomButton::timeout ()
{
  switch (type_)
  {
    case In:
      scene_->scaleElements (1.25, pos_);
      break;
    case Out:
      scene_->scaleElements (0.8, pos_);
      break;
  }
}

//------------------------------------------------------------------------------

/// Activates the timer for zoom with center at _pos (will be called if an element is moved above)
void ZoomButton::activate (QPointF _pos)
{
  pos_ = _pos;
  setOpacity (1.0);
  if (!timer_.isActive ())
    timer_.start (500);
}

//------------------------------------------------------------------------------

/// Stop the timer
void ZoomButton::deactivate ()
{
  setOpacity (0.4);
  timer_.stop ();
}

//------------------------------------------------------------------------------

/// Sets the geometry 
void ZoomButton::setGeometry (const QRectF &_rect)
{
  QGraphicsPixmapItem::setPos (_rect.topLeft ());
  QGraphicsLayoutItem::setGeometry (_rect);
}

//------------------------------------------------------------------------------

// size information for layouting
QSizeF ZoomButton::sizeHint (Qt::SizeHint _which, const QSizeF &/*_constraint*/) const
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
}
