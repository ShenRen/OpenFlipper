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
#include "elementFunction.hh"

#include "graphicsScene.hh"
#include "sceneElement.hh"
#include "../parser/function.hh"
#include "../parser/element.hh"

#include "baseWidget.hh"

//== NAMESPACES ===============================================================
namespace VSI {

//=============================================================================
//
//  CLASS VSI::ElementFunction - IMPLEMENTATION
//
//=============================================================================

/// Constructor
ElementFunction::ElementFunction (Function *_f, SceneElement *_element) :
  element_ (_element),
  function_ (_f)
{
  scene_ = new GraphicsScene (_f->element()->context(), this);
  connect (scene_, SIGNAL (contentChanged ()),
           _element->graphicsScene (), SLOT (contentChange ()));
}

//------------------------------------------------------------------------------

/// Destructor
ElementFunction::~ElementFunction ()
{
  delete scene_;
}

//------------------------------------------------------------------------------

/// Save to xml
void ElementFunction::saveToXml(QDomDocument & _doc, QDomElement & _root)
{
  QDomElement main = _doc.createElement("function");
  _root.appendChild(main);
  main.setAttribute ("name",function_->name ());
  scene_->saveToXml (_doc, main);
}

//------------------------------------------------------------------------------

/// Load from xml
void ElementFunction::loadFromXml(QXmlQuery & _xml)
{
  scene_->loadFromXml (_xml);
}

//------------------------------------------------------------------------------

// initiate editing
void VSI::ElementFunction::editFunction()
{
  BaseWidget::getBaseWidget ()->pushScene (scene_);
}

//------------------------------------------------------------------------------
}



