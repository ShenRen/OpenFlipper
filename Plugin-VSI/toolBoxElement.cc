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
#include <QVBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QMimeData>
#include <QDrag>
#include <QMouseEvent>

#include "toolBoxElement.hh"
#include "parser/element.hh"

#define BACKGROUND_RED   0x00
#define BACKGROUND_GREEN 0x00
#define BACKGROUND_BLUE  0x00
#define BACKGROUND_ALPHA 0xff

//== NAMESPACES ===============================================================
namespace VSI {

//=============================================================================
//
//  CLASS ToolBoxElement - IMPLEMENTATION
//
//=============================================================================

/// Constructor
ToolBoxElement::ToolBoxElement (Element *_element, QWidget *_parent) :
  QWidget (_parent),
  element_ (_element)
{
  QVBoxLayout *layout = new QVBoxLayout ();

  layout->setContentsMargins (2, 2, 2, 2);

  QLabel *label = new QLabel (element_->shortDescription (), this);

  label->setAlignment (Qt::AlignHCenter);
  label->setWordWrap (true);

  QPalette p = label->palette ();
  p.setBrush (QPalette::WindowText, Qt::white);
  p.setBrush (QPalette::ButtonText, Qt::white);
  label->setPalette (p);
  
  layout->addWidget (label);

  setToolTip (element_->longDescription ());

  setLayout (layout);
}

//------------------------------------------------------------------------------

/// Destructor
ToolBoxElement::~ToolBoxElement ()
{
}

//------------------------------------------------------------------------------

// Paint a rounded rect as background
void ToolBoxElement::paintEvent(QPaintEvent * /*_event*/)
{
  QPainter p (this);
  p.setRenderHint(QPainter::Antialiasing);

  p.setBrush(QBrush(QColor(BACKGROUND_RED,
                                   BACKGROUND_GREEN,
                                   BACKGROUND_BLUE,
                                   BACKGROUND_ALPHA)));
  p.setPen(QColor(BACKGROUND_RED + 0x30,
                          BACKGROUND_GREEN + 0x30,
                          BACKGROUND_BLUE + 0x30,
                          BACKGROUND_ALPHA));
  p.drawRoundedRect(0, 0, width (), height (), 4, 4);

}

//------------------------------------------------------------------------------

/// Starts drag on mouse press
void ToolBoxElement::mousePressEvent(QMouseEvent *_event)
{
  QMimeData *mimeData = new QMimeData;

  QPoint off = _event->pos() - QPoint (width () / 2, height () / 2);

  QString data = QString::number (off.x ()) + ";";
  data += QString::number (off.y ()) + ";" + element_->name ();

  // internal mimetype
  mimeData->setData ("application/x-openflipper.vsi",data.toLocal8Bit ());

  QDrag *drag = new QDrag(this);
  drag->setMimeData(mimeData);

  QPixmap pixmap (size ());
  pixmap.fill(Qt::transparent);

  render (&pixmap);

  drag->setPixmap(pixmap);
  drag->setHotSpot(_event->pos());

  drag->exec ();
}

//------------------------------------------------------------------------------
}

