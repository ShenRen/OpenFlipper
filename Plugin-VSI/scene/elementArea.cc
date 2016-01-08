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
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>

#include "../parser/element.hh"
#include "elementArea.hh"
#include "graphicsScene.hh"
#include "sceneElement.hh"

//== NAMESPACES ===============================================================
namespace VSI {

//=============================================================================
//
//  CLASS VSI::ElementArea - IMPLEMENTATION
//
//=============================================================================

/// Constructor
ElementArea::ElementArea (GraphicsScene *_scene) :
  scene_ (_scene)
{
}

//------------------------------------------------------------------------------

/// Destructor
ElementArea::~ElementArea ()
{
}

//------------------------------------------------------------------------------

/// Bounding rectangle
QRectF ElementArea::boundingRect () const
{
  return elementsBoundingRect_;
}

//------------------------------------------------------------------------------

/// Add a child element
void ElementArea::addElement (SceneElement *_element)
{
  foreach (QGraphicsItem *e, elements_)
    e->setSelected (false);

  elements_.append (_element);
  _element->setParentItem (this);
  prepareGeometryChange ();
  elementsBoundingRect_ = _element->boundingRect () | _element->childrenBoundingRect ();

  _element->setSelected (true);
  update ();
}

//------------------------------------------------------------------------------

/// Remove an element, if its removeable or _force is set
bool ElementArea::removeElement (SceneElement *_element, bool _force)
{
 
  if (!_force && _element->element ()->flags () & ELEMENT_FLAG_NO_DELETE)
    return false;
  if (elements_.contains (_element))
  {
    elements_.removeAll (_element);
    _element->setParentItem (0);
    prepareGeometryChange();
    elementsBoundingRect_ = childrenBoundingRect();
    return true;
  }
  else
    return false;
}

//------------------------------------------------------------------------------

/// All elements
const QList<SceneElement *>& ElementArea::elements () const
{
  return elements_;
}

//------------------------------------------------------------------------------
}
