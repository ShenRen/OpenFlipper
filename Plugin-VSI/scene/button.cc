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
#include <cmath>

#include <QPolygonF>
#include <QGraphicsItem>
#include <QFontMetrics>
#include <QPen>
#include <QPainter>
#include <QPainterPath>
#include <QGraphicsSceneMouseEvent>

#include "button.hh"

#define BACK_OFFSET 2

//== NAMESPACES ===============================================================
namespace VSI {

//=============================================================================
//
//  CLASS VSI::Button - IMPLEMENTATION
//
//=============================================================================

/// Constructor
Button::Button (QGraphicsItem *_parent) :
  Text (_parent),
  glow_ (false)
{
  setBackground (true, true);
  setAcceptHoverEvents (true);
}

//------------------------------------------------------------------------------

/// Constructor
Button::Button (const QString &_text, QGraphicsItem *_parent) :
  Text (_text, _parent),
  glow_ (false)
{
  setBackground (true, true);
  setAcceptHoverEvents (true);
}

//------------------------------------------------------------------------------

/// Destructor
Button::~Button ()
{
}

//------------------------------------------------------------------------------

// glow on mouse enter
void Button::hoverEnterEvent (QGraphicsSceneHoverEvent * /*_event*/)
{
  glow_ = true;
  update ();

  QPen pen = backgroundPen_;
  pen.setWidthF (1.0);
  pen.setColor (QColor (127, 166, 218));
  Text::setBackgroundPen (pen);
}

//------------------------------------------------------------------------------

// stop glowing on mouse leave
void Button::hoverLeaveEvent (QGraphicsSceneHoverEvent * /*_event*/)
{
  glow_ = false;
  update ();

  Text::setBackgroundPen (backgroundPen_);
}

//------------------------------------------------------------------------------

// emit pressed () on mouse press
void Button::mousePressEvent (QGraphicsSceneMouseEvent *_event)
{
  QBrush brush = backgroundBrush_;
  
  brush.setColor (brush.color ().lighter (50));
  Text::setBackgroundBrush (brush);
  emit pressed ();
  _event->accept ();
}

//------------------------------------------------------------------------------

// change brush back to normal
void Button::mouseReleaseEvent (QGraphicsSceneMouseEvent *_event)
{
  Text::setBackgroundBrush (backgroundBrush_);
  _event->accept ();
}

//------------------------------------------------------------------------------

/// Sets the background brush
void Button::setBackgroundBrush(QBrush _brush)
{
  backgroundBrush_ = _brush;
  Text::setBackgroundBrush (_brush);
}

//------------------------------------------------------------------------------

/// Sets the background pen
void Button::setBackgroundPen(QPen _pen)
{
  backgroundPen_ = _pen;
  Text::setBackgroundPen (_pen);
}

//------------------------------------------------------------------------------

/// Button glow painting
void Button::paint (QPainter *_painter, const QStyleOptionGraphicsItem *_option, QWidget *_widget)
{
  if (glow_)
  {
    QPainterPath path = shape ();
    QPen curr = _painter->pen ();

    QPen pen = backgroundPen_;
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);

    pen.setColor (QColor (127, 166, 218, 64));
    pen.setWidthF (7.0);
    _painter->setPen (pen);
    _painter->drawPath (path);
    pen.setColor (QColor (127, 166, 218, 128));
    pen.setWidthF (5.0);
    _painter->setPen (pen);
    _painter->drawPath (path);
    pen.setColor (QColor (127, 166, 218, 192));
    pen.setWidthF (3.0);
    _painter->setPen (pen);
    _painter->drawPath (path);

    _painter->setPen (curr);
  }
  Text::paint (_painter, _option, _widget);
}

//------------------------------------------------------------------------------

/// Bounding rectangle
QRectF Button::boundingRect() const
{
  QRectF rect = Text::boundingRect ();

  if (rect.width () && rect.height ())
    rect.adjust(-3, -3, 3, 3);

  return rect;
}

//------------------------------------------------------------------------------

/// Sets the geometry
void Button::setGeometry(const QRectF & _rect)
{
  Text::setGeometry (_rect);
}

//------------------------------------------------------------------------------
}
