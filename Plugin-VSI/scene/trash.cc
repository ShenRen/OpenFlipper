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

#include "trash.hh"
#include "graphicsScene.hh"
#include "sceneElement.hh"

//== NAMESPACES ===============================================================
namespace VSI {

//=============================================================================
//
//  CLASS VSI::Trash - IMPLEMENTATION
//
//=============================================================================


/// Constructor
Trash::Trash (GraphicsScene *_scene, QGraphicsItem *_parent) :
  QGraphicsPixmapItem (_parent),
  scene_ (_scene)
{
  setPixmap (OpenFlipper::Options::iconDirStr()+OpenFlipper::Options::dirSeparator()+"user-trash.png");

  setOpacity (0.4);

  setAcceptHoverEvents (true);
}

//------------------------------------------------------------------------------

/// Destructor
Trash::~Trash ()
{
}

//------------------------------------------------------------------------------

// make the trash opaque if mouse is over and we have selected elements
void Trash::hoverEnterEvent (QGraphicsSceneHoverEvent * /*_event*/)
{
  if (!scene_->selectedItems ().isEmpty ())
    setOpacity (1.0);
}

//------------------------------------------------------------------------------

// make trash transparent on mouse leave
void Trash::hoverLeaveEvent (QGraphicsSceneHoverEvent * /*_event*/)
{
  setOpacity (0.4);
}

//------------------------------------------------------------------------------

// delete selected elements on mouse press
void Trash::mousePressEvent (QGraphicsSceneMouseEvent * /*_event*/)
{
  if (!scene_->selectedItems ().isEmpty ())
    foreach (QGraphicsItem *e, scene_->selectedItems ())
    {
      SceneElement *se = dynamic_cast<SceneElement *> (e);
      if (se && scene_->removeElement (se))
        delete se;
    }
}

//------------------------------------------------------------------------------

/// Makes the trash opaque (will be called of an element is moved above this widget)
void Trash::activate ()
{
  setOpacity (1.0);
}

//------------------------------------------------------------------------------

/// Makes the trash transparent (will be called of an element is moved away from this widget)
void Trash::deactivate ()
{
  setOpacity (0.4);
}

//------------------------------------------------------------------------------

/// Sets the geometry
void Trash::setGeometry (const QRectF &_rect)
{
  QGraphicsPixmapItem::setPos (_rect.topLeft ());
  QGraphicsLayoutItem::setGeometry (_rect);
}

//------------------------------------------------------------------------------

// size information for layouting
QSizeF Trash::sizeHint (Qt::SizeHint _which, const QSizeF &/*_constraint*/) const
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
