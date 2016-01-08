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
#include "elementInput.hh"
#include "elementOutput.hh"
#include "sceneElement.hh"
#include "connectionPoint.hh"
#include "connection.hh"
#include "graphicsScene.hh"
#include "text.hh"
#include "../parser/element.hh"
#include "../parser/context.hh"
#include "../parser/type.hh"
#include "../parser/typeWidget.hh"

//== NAMESPACES ===============================================================
namespace VSI {

//=============================================================================
//
//  CLASS VSI::ElementInput - IMPLEMENTATION
//
//=============================================================================

/// Constructor
ElementInput::ElementInput (Input *_in, SceneElement *_parent) :
  ElementInOut (_in, _parent),
  in_ (_in),
  isSet_ (false),
  value_ (""),
  forceAsk_ (false)
{
  if (state () & Input::Optional)
    connectionPointItem ()->setState (ConnectionPoint::Optional);
  else if (state () & Input::NoUserInput)
    connectionPointItem ()->setState (ConnectionPoint::NoValue);
  else
    connectionPointItem ()->setState (ConnectionPoint::RuntimeInput);

  VSI::Type *t = inOut ()->element ()->context ()->getType (inOut ()->typeString ());

  if (t)
  {
    TypeWidget *tW = t->widget (inOut ()->hints (), inOut ()->typeString ());
    if (tW)
    {
      value_ = tW->toValue ();
      delete tW;
    }
  }

  typeTextItem ()->setBackground (false, true);
  typeTextItem ()->setBackgroundBrush (Qt::gray);
  typeTextItem ()->setBrush (Qt::black);
  descriptionTextItem ()->setBackground (false, true);
  descriptionTextItem ()->setBackgroundBrush (Qt::white);
  descriptionTextItem ()->setBrush (Qt::black);

  connectionPointItem ()->setWidth (qMax (typeTextItem ()->preferredHeight (),
                                          descriptionTextItem ()->preferredHeight ()));
}

//------------------------------------------------------------------------------

/// Destructor
ElementInput::~ElementInput ()
{
}

//------------------------------------------------------------------------------

/// Returns state of valid flag (needed during code generation)
bool ElementInput::valid()
{
  if (!connections ().isEmpty ())
    return valid_;

  if (isSet () || !(state () & Input::NoRuntimeUserInput) || state () & Input::Optional)
    return true;

  return false;
}

//------------------------------------------------------------------------------

/// VSI::Input state passthrough
unsigned int VSI::ElementInput::state()
{
  return in_->state ();
}

//------------------------------------------------------------------------------

/// Add connection
void ElementInput::addConnection(Connection * _conn)
{
  ElementInOut::addConnection (_conn);

  // update state
  connectionPointItem ()->setState (ConnectionPoint::Connected);
}

//------------------------------------------------------------------------------

/// Remove connection
void ElementInput::removeConnection (Connection * _conn)
{
  ElementInOut::removeConnection (_conn);

  // update state
  if (connections().isEmpty())
  {
    if (isSet ())
      connectionPointItem ()->setState (ConnectionPoint::UserInput);
    else if (state () & Input::Optional && !forceAsk_)
    {
      connectionPointItem ()->setState (ConnectionPoint::Optional);
    }
    else if (state () & Input::NoUserInput)
    {
      connectionPointItem ()->setState (ConnectionPoint::NoValue);
    }
    else
      connectionPointItem ()->setState (ConnectionPoint::RuntimeInput);
  }
}

//------------------------------------------------------------------------------

/// "Set" flag to mark input as set by user
void ElementInput::set(bool _set)
{
  if (isSet_ != _set)
    element ()->graphicsScene ()->contentChange ();
  isSet_ = _set;

  // update state
  if (connections().isEmpty())
  {
    if (isSet ())
      connectionPointItem ()->setState (ConnectionPoint::UserInput);
    else if (state () & Input::Optional && !forceAsk_)
    {
      connectionPointItem ()->setState (ConnectionPoint::Optional);
    }
    else if (state () & Input::NoUserInput)
    {
      connectionPointItem ()->setState (ConnectionPoint::NoValue);
    }
    else
      connectionPointItem ()->setState (ConnectionPoint::RuntimeInput);
  }
}

//------------------------------------------------------------------------------

/// Return "set" flag
bool ElementInput::isSet ()
{
  // Only configured input
  if ((state () & Input::NoRuntimeUserInput) && (state () & Input::NoExternalInput))
    return true;
  return isSet_;
}

//------------------------------------------------------------------------------

/// Save to xml
void ElementInput::saveToXml(QDomDocument & _doc, QDomElement & _root)
{
  QDomText t;

  QDomElement main = _doc.createElement("input");
  _root.appendChild(main);
  main.setAttribute ("name",in_->name ());


  QDomElement set = _doc.createElement("is_set");
  main.appendChild(set);
  t = _doc.createTextNode(isSet_ ? "true" : "false");
  set.appendChild(t);

  if (state () & Input::Optional && forceAsk_)
  {
    QDomElement fA = _doc.createElement("force_ask");
    main.appendChild(fA);
    t = _doc.createTextNode(isSet_ ? "true" : "false");
    fA.appendChild(t);
  }

  if (isSet_)
  {
    QDomElement val = _doc.createElement("value");
    main.appendChild(val);
    t = _doc.createTextNode(value_);
    val.appendChild(t);
  }

  foreach (Connection *c, connections ())
  {
    QDomElement conn = _doc.createElement("connection");
    main.appendChild(conn);
    conn.setAttribute ("element",c->output ()->element ()->element ()->name ());
    conn.setAttribute ("element_id",QString::number (c->output ()->element ()->id ()));
    conn.setAttribute ("output",c->output ()->inOut ()->name ());
  }


}

//------------------------------------------------------------------------------

/// Load from xml
void ElementInput::loadFromXml(QXmlQuery & _xml)
{
  QString val = Context::getXmlString (_xml, "is_set/string(text())");

  isSet_ = Context::strToBool (val);

  if (state () & Input::Optional)
  {
    val = Context::getXmlString (_xml, "force_ask/string(text())");
    forceAsk_ = Context::strToBool (val);
  }

  if (isSet_)
  {
    value_ = Context::getXmlString (_xml, "value/string(text())");
  }

  if (isSet ())
    connectionPointItem ()->setState (ConnectionPoint::UserInput);
  else if (state () & Input::Optional && !forceAsk_)
  {
    connectionPointItem ()->setState (ConnectionPoint::Optional);
  }
  else if (state () & Input::NoUserInput)
  {
    connectionPointItem ()->setState (ConnectionPoint::NoValue);
  }
  else
    connectionPointItem ()->setState (ConnectionPoint::RuntimeInput);

}

//------------------------------------------------------------------------------

/// Set to a user value
void ElementInput::setValue(QString _value)
{
  if (value_ != _value)
    element ()->graphicsScene ()->contentChange ();
  value_ = _value;
}

//------------------------------------------------------------------------------
}

