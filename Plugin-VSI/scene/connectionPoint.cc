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
#include <QPolygonF>
#include <QGraphicsItem>
#include <QFontMetrics>
#include <QPainter>

#include "connectionPoint.hh"
#include "connection.hh"
#include "elementInOut.hh"
#include "graphicsScene.hh"
#include "elementArea.hh"
#include "text.hh"
#include "../parser/inout.hh"

#define SIZE  20

//== NAMESPACES ===============================================================
namespace VSI {

//=============================================================================
//
//  CLASS VSI::ConnectionPoint - IMPLEMENTATION
//
//=============================================================================


QColor colors_[] = { QColor(255, 0, 0),
                     QColor(255, 0, 255),
                     QColor(255, 128, 0),
                     QColor(224, 224, 0),
                     QColor(0, 192, 0),
                     QColor(0, 0, 255) };

//------------------------------------------------------------------------------

/// Constructor
ConnectionPoint::ConnectionPoint (ElementInOut *_e, QGraphicsItem *_parent) :
  QGraphicsEllipseItem (_parent),
  inout_ (_e),
  width_ (5)
{
  setRect (0, 0, width_, width_);

  setBrush (QColor (64,64,64));

  setAcceptHoverEvents (true);
}

//------------------------------------------------------------------------------

/// Destructor
ConnectionPoint::~ConnectionPoint ()
{
}

//------------------------------------------------------------------------------

/// Sets the geometry
void ConnectionPoint::setGeometry (const QRectF &_rect)
{
  QGraphicsEllipseItem::setPos (_rect.topLeft ());
  QGraphicsLayoutItem::setGeometry (_rect);
  setRect (0, 0, width_, width_);
}

//------------------------------------------------------------------------------

// size informations for layouting
QSizeF ConnectionPoint::sizeHint (Qt::SizeHint _which, const QSizeF &/*_constraint*/) const
{
  QSizeF sh;
  switch (_which) {
    case Qt::MinimumSize:
    case Qt::PreferredSize:
    case Qt::MaximumSize:
      sh = QSizeF(width_, width_);
      break;
    default:
      break;
  }

  return sh;
}

//------------------------------------------------------------------------------

// start a new connection on press
void ConnectionPoint::mousePressEvent (QGraphicsSceneMouseEvent *_event)
{
  Connection *c = new Connection (this, scene ());
  static_cast<GraphicsScene *>(scene ())->setActiveConnection (c);
  QGraphicsEllipseItem::mousePressEvent(_event);
}

//------------------------------------------------------------------------------

/// Position for connections
QPointF ConnectionPoint::connectPos ()
{
  QGraphicsItem *elementArea = dynamic_cast<GraphicsScene *> (scene ())->elementArea ();
  
  if (inout_->type () == ElementInOut::TypeInput)
    return elementArea->mapFromScene (mapToScene (QPointF (0, width_ / 2)));
  return elementArea->mapFromScene (mapToScene (QPointF (width_, width_ / 2)));
}

//------------------------------------------------------------------------------

/// sets the state
void ConnectionPoint::setState(State _state)
{
  setBrush (colors_[_state]);

  QString stateStr;

  switch (_state)
  {
    case NoValue:
      stateStr = "[NOT CONNECTED]";
      break;
    case Optional:
      stateStr = "[OPTIONAL]";
      break;
    case RuntimeInput:
      stateStr = "[ASK DURING EXECUTION]";
      break;
    case UserInput:
      stateStr = "[CONSTANT SET]";
      break;
    case Connected:
      stateStr = "[CONNECTED]";
      break;
    case OutputNotConnected:
      stateStr = "[NOT CONNECTED]";
      break;
  }

  stateStr += " " + inout_->inOut ()->longDescription () + " (" + inout_->inOut ()->typeString () + ")";
  setToolTip (stateStr);
  inout_->typeTextItem ()->setToolTip (stateStr);
  inout_->descriptionTextItem ()->setToolTip (stateStr);
}

//------------------------------------------------------------------------------

// highlight connection if mouse over
void ConnectionPoint::hoverEnterEvent (QGraphicsSceneHoverEvent * /*_event*/)
{
  foreach (Connection *c, inout_->connections ())
  {
    QPen p = c->pen ();

    if (inOut ()->inOut ()->typeString () == "data")
      p.setWidth (6);
    else
      p.setWidth (4);

    p.setColor (QColor (0, 0, 255));
    c->setPen (p);
  }
}

//------------------------------------------------------------------------------

// remove highlight 
void ConnectionPoint::hoverLeaveEvent (QGraphicsSceneHoverEvent * /*_event*/)
{
  foreach (Connection *c, inout_->connections ())
  {
    QPen p = c->pen ();

    if (inOut ()->inOut ()->typeString () == "data")
      p.setWidth (4);
    else
      p.setWidth (2);

    p.setColor (QColor (0, 0, 0));
    c->setPen (p);
  }
}

//------------------------------------------------------------------------------

/// Sets the diameter
void ConnectionPoint::setWidth(int _width)
{
  width_ = _width;
  updateGeometry ();
}

//------------------------------------------------------------------------------

// painting
void ConnectionPoint::paint(QPainter *_painter, const QStyleOptionGraphicsItem * _option, QWidget *_widget)
{
  QGraphicsEllipseItem::paint (_painter, _option, _widget);

  float wH = width_ / 2.0;
  float wT = width_ / 3.0;
  float wS = width_ / 6.0;

  QPainterPath path;
  path.moveTo (0, 0);
  path.arcTo (0, 0, width_, width_, 0, 360);

  QRadialGradient rG (QPointF(width_ * 0.8, width_), width_ * 0.8);
  rG.setColorAt(0, QColor (255, 255, 255, 128));
  rG.setColorAt(1, Qt::transparent);
  _painter->setBrush (rG);
  _painter->setPen (Qt::NoPen);
  _painter->drawPath (path);

  path = QPainterPath ();
  path.moveTo (1, wH);
  path.arcTo (1, 1, width_ - 2, width_ - 2, 180, - 165);
  path.cubicTo (QPointF (wH + wT, wH + wS), QPointF (wH + wS, wH - wS), QPointF (wH, wH));
  path.cubicTo (QPointF (wT, wH + wS), QPointF (wS, wH + wS), QPointF (1, wH));

  rG.setCenter (wS, 0);
  rG.setFocalPoint(wS, 0);
  rG.setRadius (width_);
  rG.setColorAt(0, Qt::white);
  rG.setColorAt(1, Qt::transparent);
  _painter->setBrush (rG);
  _painter->setPen (Qt::NoPen);
  _painter->drawPath(path);
}

//------------------------------------------------------------------------------
}
