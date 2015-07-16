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
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPainter>
#include <QPalette>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsGridLayout>
#include <QGraphicsLinearLayout>

#include "sceneTools.hh"
#include "sceneElement.hh"
#include "graphicsScene.hh"
#include "arrow.hh"
#include "zoomButton.hh"
#include "trash.hh"
#include "functionDisplay.hh"

//== NAMESPACES ===============================================================
namespace VSI {

//=============================================================================
//
//  CLASS VSI::SceneTools - IMPLEMENTATION
//
//=============================================================================

/// Constructor
SceneTools::SceneTools (GraphicsScene *_scene) :
  scene_ (_scene)
{
  QRectF rect = scene_->sceneRect();

  arrows_[0] = new Arrow (_scene, this, Arrow::North);
  arrows_[1] = new Arrow (_scene, this, Arrow::South);
  arrows_[2] = new Arrow (_scene, this, Arrow::East);
  arrows_[3] = new Arrow (_scene, this, Arrow::West);
  arrows_[4] = new Arrow (_scene, this, Arrow::Center);

  QGraphicsGridLayout *aLayout = new QGraphicsGridLayout;
  aLayout->addItem (arrows_[0], 0, 1, Qt::AlignCenter);
  aLayout->addItem (arrows_[1], 2, 1, Qt::AlignCenter);
  aLayout->addItem (arrows_[2], 1, 2, Qt::AlignCenter);
  aLayout->addItem (arrows_[3], 1, 0, Qt::AlignCenter);
  aLayout->addItem (arrows_[4], 1, 1, Qt::AlignCenter);

  trash_ = new Trash (_scene, this);

  QGraphicsLinearLayout *zLayout = new QGraphicsLinearLayout (Qt::Horizontal);
  zoom_[0] = new ZoomButton (_scene, this, ZoomButton::In);
  zoom_[1] = new ZoomButton (_scene, this, ZoomButton::Out);
  zLayout->addItem (zoom_[0]);
  zLayout->addItem (zoom_[1]);

  fDisplay_ = new FunctionDisplay (scene_);
  scene_->addItem (fDisplay_);

  setGeometry (rect);
  connect (scene_, SIGNAL (sceneRectChanged (const QRectF &)),
           this, SLOT (sceneRectChanged (const QRectF &)));

  QGraphicsGridLayout *mLayout = new QGraphicsGridLayout;

  mLayout->addItem (aLayout, 0, 2, Qt::AlignTop | Qt::AlignRight);
  mLayout->addItem (zLayout, 2, 0, Qt::AlignBottom | Qt::AlignLeft);
  mLayout->addItem (trash_, 2, 2, Qt::AlignBottom | Qt::AlignRight);
  mLayout->setColumnStretchFactor (1, 1);
  mLayout->setRowStretchFactor (1, 1);
  mLayout->setContentsMargins (7, 7, 7, 7);

  setLayout (mLayout);
}

//------------------------------------------------------------------------------

/// Destructor
SceneTools::~SceneTools ()
{
}

//------------------------------------------------------------------------------

// handle scene size changes
void SceneTools::sceneRectChanged (const QRectF &_rect)
{
  setGeometry (_rect);

  fDisplay_->setPos(_rect.topLeft ());
  updateArrows ();
}

//------------------------------------------------------------------------------

// update arrow depending on visible scene rect
void SceneTools::updateArrows ()
{
  QRectF bb = scene_->elementsBoundingRect ();
  QRectF rect = scene_->sceneRect();

  if (bb.top () < rect.top ())
    arrows_[0]->setHighlight (true);
  else
    arrows_[0]->setHighlight (false);

  if (bb.bottom () > rect.bottom ())
    arrows_[1]->setHighlight (true);
  else
    arrows_[1]->setHighlight (false);

  if (bb.right () > rect.right ())
    arrows_[2]->setHighlight (true);
  else
    arrows_[2]->setHighlight (false);

  if (bb.left () < rect.left ())
    arrows_[3]->setHighlight (true);
  else
    arrows_[3]->setHighlight (false);

  if (rect.center () != bb.center ())
    arrows_[4]->setHighlight (true);
  else
    arrows_[4]->setHighlight (false);
}

//------------------------------------------------------------------------------

/// Handles mouse movement (will be called by the scene, if the mouse with a draged element is moved)
void SceneTools::mouseMove (QPointF _pos)
{
  for (unsigned int i = 0; i < 5; i++)
  {
    if (arrows_[i]->contains (arrows_[i]->mapFromScene (_pos)))
      arrows_[i]->activate ();
    else
      arrows_[i]->reset ();
  }

  for (unsigned int i = 0; i < 2; i++)
  {
    if (zoom_[i]->contains (zoom_[i]->mapFromScene (_pos)))
      zoom_[i]->activate (_pos);
    else
      zoom_[i]->deactivate ();
  }

  if (trash_->contains (trash_->mapFromScene (_pos)))
    trash_->activate ();
  else
    trash_->deactivate ();
}

//------------------------------------------------------------------------------

/// Handles mouse release (will be called by the scene, if the element is dropped)
void SceneTools::mouseRelease (QPointF _pos, QGraphicsItem *_item)
{
  for (unsigned int i = 0; i < 5; i++)
  {
      arrows_[i]->reset ();
  }
  for (unsigned int i = 0; i < 2; i++)
  {
      zoom_[i]->deactivate ();
  }
  trash_->deactivate ();

  if (trash_->contains (trash_->mapFromScene (_pos)))
  {
    SceneElement *se = dynamic_cast<SceneElement *> (_item);
    if (se && scene_->removeElement (se))
      delete se;
  }
}

//------------------------------------------------------------------------------
}

