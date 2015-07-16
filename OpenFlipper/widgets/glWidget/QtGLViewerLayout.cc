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
//  CLASS QtGLViewerLayout - IMPLEMENTATION
//
//=============================================================================

//== INCLUDES =================================================================

#include "QtGLViewerLayout.hh"

//== NAMESPACES ===============================================================


//== CLASS IMPLEMENTATION======================================================

QtGLViewerLayout::QtGLViewerLayout (QGraphicsLayoutItem * _parent) :
  QGraphicsLayout (_parent),
  wheelX_ (0),
  wheelY_ (0),
  wheelZ_ (0)
{
}

//-----------------------------------------------------------------------------

void QtGLViewerLayout::addWheelX (QGraphicsWidget *_item)
{
  if (wheelX_)
    items_.remove(items_.indexOf(wheelX_));
  wheelX_ = _item;
  items_.append(_item);
}

//-----------------------------------------------------------------------------

void QtGLViewerLayout::addWheelY (QGraphicsWidget *_item)
{
  if (wheelY_)
    items_.remove(items_.indexOf(wheelY_));
  wheelY_ = _item;
  items_.append(_item);
}

//-----------------------------------------------------------------------------

void QtGLViewerLayout::addWheelZ (QGraphicsWidget *_item)
{
  if (wheelZ_)
    items_.remove(items_.indexOf(wheelZ_));
  wheelZ_ = _item;
  items_.append(_item);
}

//-----------------------------------------------------------------------------

int QtGLViewerLayout::count() const
{
  return items_.size(); 
}

//-----------------------------------------------------------------------------

QGraphicsLayoutItem * QtGLViewerLayout::itemAt(int _i) const
{
  if (_i < 0 || _i >= items_.size())
    return 0;

  return items_[_i];
}

//-----------------------------------------------------------------------------

void QtGLViewerLayout::removeAt (int _index)
{
  if (_index < 0 || _index >= items_.size())
    return;

  if (items_[_index] == wheelX_)
    wheelX_ = 0;
  if (items_[_index] == wheelY_)
    wheelY_ = 0;
  if (items_[_index] == wheelZ_)
    wheelZ_ = 0;

  items_.remove(_index);
}

//-----------------------------------------------------------------------------

QSizeF QtGLViewerLayout::sizeHint(Qt::SizeHint /*_which*/, const QSizeF & _constraint) const
{
  return _constraint;
}

//-----------------------------------------------------------------------------

void QtGLViewerLayout::setGeometry(const QRectF & rect)
{
  QGraphicsLayoutItem::setGeometry (rect);
  reLayout ();
}

//-----------------------------------------------------------------------------

void QtGLViewerLayout::reLayout ()
{
  if (!wheelX_ || !wheelY_ || !wheelZ_)
    return;

  QRectF r = contentsRect ();
  float scale = qMin(300.0, qMin(r.width(), r.height())) / 300.0;

  foreach (QGraphicsWidget *item, items_)
  {
    if (item->size() != item->preferredSize ())
      item->resize (item->preferredSize ());
    item->resetTransform();
    item->setScale (scale);
  }

  wheelX_->setPos (r.left(),
		   r.bottom() - ((wheelY_->size().height() + wheelX_->size().height()) * scale));
  wheelY_->setPos (r.left() + (wheelX_->size().width() * scale),
		   r.bottom() - (wheelY_->size().height() * scale));
  wheelZ_->setPos (r.right() - (wheelZ_->size().width() * scale),
		   r.bottom() - ((wheelY_->size().height() + wheelZ_->size().height()) * scale));
}


//=============================================================================
//=============================================================================
