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
#include <QFont>

#include "sceneElement.hh"
#include "elementInOut.hh"
#include "text.hh"
#include "connection.hh"
#include "connectionPoint.hh"
#include "../parser/inout.hh"
#include "../parser/element.hh"
#include "../parser/context.hh"

#include <iostream>

//== NAMESPACES ===============================================================
namespace VSI {

//=============================================================================
//
//  CLASS VSI::ElementInOut - IMPLEMENTATION
//
//=============================================================================

/// Constructor
ElementInOut::ElementInOut (InOut *_io, SceneElement *_parent):
  io_ (_io),
  element_ (_parent)
{
  conn_ = new ConnectionPoint (this, _parent);
  typeText_ = new Text (_io->typeString (), _parent);
  QFont font = typeText_->font ();
  font.setItalic (true);
  font.setPointSize (8);
  typeText_->setFont (font);
  descText_ = new Text (_io->shortDescription (), _parent);
  font = descText_->font ();
  if (_io->name() == "data")
    font.setPointSize (10);
  else
    font.setPointSize (8);
  descText_->setFont (font);

  conn_->setToolTip (_io->typeString () + " : " + _io->longDescription ());
  typeText_->setToolTip (_io->typeString () + " : " + _io->longDescription ());
  descText_->setToolTip (_io->typeString () + " : " + _io->longDescription ());

  typeText_->setHorizontalStretch (true);
  descText_->setHorizontalStretch (true);
}


ElementInOut::ElementInOut(const ElementInOut& _orig) :
  io_(NULL),
  element_(NULL),
  conn_(NULL),
  typeText_(NULL),
  descText_(NULL)
{
  std::cerr << "Illegal use of copy constructor in class ElementInOut, which is not implemented yet!" << std::endl;
};

//------------------------------------------------------------------------------

/// Destructor
ElementInOut::~ElementInOut ()
{
  foreach (Connection *c, connections_)
    delete c;

  delete conn_;
  delete typeText_;
  delete descText_;
}

//------------------------------------------------------------------------------

/// Can this input/output be connected to _e
bool ElementInOut::validConnection (ElementInOut *_e)
{
  // Can't be connected to itself
  if (_e == this)
    return false;
  // Can't be connected to another input/output of the same element
  if (element_ == _e->element_)
    return false;

  // Can't be connected if types are different and can't be converted
  if (io_->typeString () != _e->io_->typeString () &&
      !io_->element ()->context ()->canConvert (io_->typeString (), _e->io_->typeString ()))
    return false;

  // An input can only have one connection
  if (type () == TypeInput && !connections ().isEmpty ())
    return false;
  if (_e->type () == TypeInput && !_e->connections().isEmpty ())
    return false;

  // Circular dependency check
  if ((type () == TypeInput && _e->element ()->isAfter (element ())) ||
      (type () == TypeOutput && _e->element ()->isBefore (element ())))
    return false;

  // inputs can only be connected to outputs and vice versa
  if (type () == _e->type ())
    return false;

  return true;
}

//------------------------------------------------------------------------------

/// Add the connection
void ElementInOut::addConnection (Connection *_conn)
{
  connections_.append (_conn);
}

//------------------------------------------------------------------------------

/// Remove the Connection
void ElementInOut::removeConnection (Connection *_conn)
{
  connections_.removeAll (_conn);
}

//------------------------------------------------------------------------------
}
