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
//  CLASS QtGraphicsButton - IMPLEMENTATION
//
//=============================================================================

//== INCLUDES =================================================================

#include <QPainter>
#include <QGraphicsSceneMouseEvent>

#include "QtGraphicsButton.hh"

//== IMPLEMENTATION ==========================================================

QtGraphicsButton::QtGraphicsButton (const QImage &_image, QGraphicsItem *_parent, int _width, int _height) :
  QGraphicsItem (_parent),
  checkable_(false),
  checked_(false),
  pressed_(false),
  over_(false),
  width_(_width),
  height_(_height)
{
  if (width_ <= 0)
    width_ = _image.width ();
  if (height_ <= 0)
    height_ = _image.height ();

  // scale image to button dimensions
  QImage i = _image.scaled (width_, height_, Qt::IgnoreAspectRatio, Qt::SmoothTransformation).convertToFormat (QImage::Format_ARGB32);

  checkedPix_ = QPixmap::fromImage (i);

  // generate black/white and transparent images for the other button states
  QImage normal (width_, height_, QImage::Format_ARGB32);
  QImage over (width_, height_, QImage::Format_ARGB32);

  for (int x = 0; x < width_; x++)
    for (int y = 0; y < height_; y++)
  {
    QRgb pix = i.pixel (x, y);
    over.setPixel (x, y, qRgba (qRed (pix), qGreen (pix), qBlue (pix), qAlpha (pix) * 0.5));
    normal.setPixel (x, y, qRgba (qGray (pix), qGray (pix), qGray (pix), qAlpha (pix) * 0.3));
  }
  overPix_ = QPixmap::fromImage (over);
  normalPix_ = QPixmap::fromImage (normal);

  setAcceptHoverEvents (true);
}

//-----------------------------------------------------------------------------

QRectF QtGraphicsButton::boundingRect () const
{
  return QRectF (QPointF(0, 0), QSizeF (width_, height_));
}

//-----------------------------------------------------------------------------

void QtGraphicsButton::paint (QPainter *_painter, const QStyleOptionGraphicsItem *, QWidget *)
{
  _painter->setClipping (false);
  if (pressed_ || checked_)
    _painter->drawPixmap (0, 0, checkedPix_);
  else if (over_)
    _painter->drawPixmap (0, 0, overPix_);
  else
    _painter->drawPixmap (0, 0, normalPix_);
}

//-----------------------------------------------------------------------------

void QtGraphicsButton::setCheckable (bool _value)
{
  checkable_ = _value;
}

//-----------------------------------------------------------------------------

void QtGraphicsButton::setChecked (bool _value)
{
  checked_ = _value;
}

//-----------------------------------------------------------------------------

bool QtGraphicsButton::isChecked () const
{
  return checked_;
}

//-----------------------------------------------------------------------------

void QtGraphicsButton::hoverEnterEvent (QGraphicsSceneHoverEvent *)
{
  over_ = true;
  update ();
}

//-----------------------------------------------------------------------------

void QtGraphicsButton::hoverLeaveEvent (QGraphicsSceneHoverEvent *)
{
  over_ = false;
  update ();
}

//-----------------------------------------------------------------------------

void QtGraphicsButton::mousePressEvent ( QGraphicsSceneMouseEvent *_event)
{
  _event->accept ();
  pressed_ = true;
  if (checkable_)
    checked_ = !checked_;
  update ();

  emit pressed ();
}

//-----------------------------------------------------------------------------

void QtGraphicsButton::mouseReleaseEvent (QGraphicsSceneMouseEvent *_event)
{
  _event->accept ();
  pressed_ = false;
  update ();
}

//-----------------------------------------------------------------------------

QVariant QtGraphicsButton::itemChange (GraphicsItemChange _change, const QVariant &_value)
{
  // reset state if button was hidden
  if (_change == QGraphicsItem::ItemVisibleHasChanged)
  {
    pressed_ = false;
    over_ = false;
  }
  return QGraphicsItem::itemChange (_change, _value);
}

//=============================================================================
