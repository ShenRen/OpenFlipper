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
#include <QWidget>
#include <QPainter>
#include <QGraphicsLinearLayout>
#include <QGraphicsProxyWidget>
#include <QGraphicsGridLayout>

#include "baseWidget.hh"

#include "functionDisplay.hh"
#include "elementFunction.hh"
#include "text.hh"
#include "button.hh"

#include "../parser/function.hh"
#include "../parser/element.hh"

#define BACKGROUND_RED   0x0
#define BACKGROUND_GREEN 0x0
#define BACKGROUND_BLUE  0x0
#define BACKGROUND_ALPHA 0xff


//== NAMESPACES ===============================================================
namespace VSI {

//=============================================================================
//
//  CLASS VSI::FunctionDisplay - IMPLEMENTATION
//
//=============================================================================

/// Constructor
FunctionDisplay::FunctionDisplay (GraphicsScene *_scene) :
  scene_ (_scene)
{
  QGraphicsGridLayout *tL = new QGraphicsGridLayout;

  // If the scene belongs to a function, then display its name and add the "Go back" button
  if (scene_->function ())
  {

    Text *f = new Text (scene_->function ()->function ()->element ()->shortDescription (), this);
    Text *fn = new Text (scene_->function ()->function ()->shortDescription (), this);
    Button *goBack = new Button (tr("Go back"), this);

    f->setBackground (true, false);
    f->setBrush (Qt::black);
    f->setBackgroundBrush (Qt::gray);
    fn->setBackground (true, false);
    fn->setBrush (Qt::black);
    fn->setBackgroundBrush (Qt::white);
    goBack->setBrush (Qt::black);
    goBack->setBackgroundBrush (Qt::lightGray);
    goBack->setBackground (true, true);

    connect (goBack, SIGNAL (pressed ()), BaseWidget::getBaseWidget(), SLOT (popScene ()));

    QFont font = f->font ();
    fn->setFont (font);
    font.setBold (true);
    f->setFont (font);


    tL->addItem (f, 0, 0, Qt::AlignVCenter | Qt::AlignLeft);
    tL->addItem (fn, 0, 1, Qt::AlignVCenter | Qt::AlignLeft);
    tL->addItem (goBack, 0, 2, Qt::AlignVCenter | Qt::AlignLeft);
  }
  // Scene is the main scene
  else
  {
    Text *f = new Text (tr("Main"), this);
    f->setBackground (true, true);
    f->setBrush (Qt::black);
    f->setBackgroundBrush (Qt::white);

    tL->addItem (f, 0, 0);

    QFont font = f->font ();
    font.setBold (true);
    f->setFont (font);
  }

  tL->setHorizontalSpacing (1);
  tL->setContentsMargins (3, 3, 3, 3);
  setLayout (tL);

  setMinimumWidth (tL->preferredWidth());
}

//------------------------------------------------------------------------------

/// Destructor
FunctionDisplay::~FunctionDisplay ()
{
}

//------------------------------------------------------------------------------

/// Paints the background
void FunctionDisplay::paint (QPainter *_painter, const QStyleOptionGraphicsItem *_option, QWidget *_widget)
{
  int w = geometry().width();
  int h = geometry().height();

  _painter->setRenderHint(QPainter::Antialiasing, true);

  _painter->setBrush(QBrush(QColor(BACKGROUND_RED,
                                   BACKGROUND_GREEN,
                                   BACKGROUND_BLUE,
                                   BACKGROUND_ALPHA)));
  
  _painter->setPen(QColor(BACKGROUND_RED,
                          BACKGROUND_GREEN,
                          BACKGROUND_BLUE,
                          BACKGROUND_ALPHA));

  QPainterPath path;
  path.moveTo (0, 0);
  path.lineTo (0, h);
  path.lineTo (w - 10, h);
  path.arcTo (w - 20, h - 20, 20, 20, -90, 90);
  path.lineTo (w, 0);
  path.lineTo (0, 0);
  _painter->drawPath (path);


  QLinearGradient lG;
  QRadialGradient rG;

  _painter->setPen (Qt::NoPen);

  lG.setStart (w - 5, 0);
  lG.setFinalStop(w, 0);
  lG.setColorAt(0, Qt::transparent);
  lG.setColorAt(1, QColor (255, 255, 255, 128));
  _painter->setBrush (lG);
  _painter->drawRect (w - 5, 0, 5, h - 10);

  lG.setStart (0, h - 5);
  lG.setFinalStop (0, h);
  _painter->setBrush (lG);
  _painter->drawRect (0, h - 5, w - 10, 5);

  path = QPainterPath ();
  path.moveTo (w - 10, h);
  path.arcTo (w - 20, h - 20, 20, 20, -90, 90);
  path.lineTo (w - 10, h - 10);
  path.lineTo (w - 10, h);
  rG.setCenter (w - 10, h - 10);
  rG.setFocalPoint(w -10, h - 10);
  rG.setRadius (10);
  rG.setColorAt (0, Qt::transparent);
  rG.setColorAt (0.5, Qt::transparent);
  rG.setColorAt (1, QColor (255, 255, 255, 128));
  _painter->setBrush (rG);
  _painter->drawPath(path);

  QGraphicsWidget::paint (_painter, _option, _widget);
}

//------------------------------------------------------------------------------
}

