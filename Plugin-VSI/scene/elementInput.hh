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

#ifndef VSI_ELEMENTINPUT_HH
#define VSI_ELEMENTINPUT_HH

//== INCLUDES =================================================================
#include <QString>

#include <QDomDocument>
#include <QDomElement>
#include <QXmlQuery>

#include "elementInOut.hh"
#include "../parser/input.hh"

//== NAMESPACES ===============================================================
namespace VSI {

//== CLASS DEFINITION =========================================================

/** Class for inputs of a scene element.
  * It handles all the different ways of input (connection to an output,
  * user configured during script creation, asking user during script execution,
  * optional)
  */
class ElementInput : public ElementInOut {

  public:

    /// Constructor
    ElementInput (Input *_in, SceneElement *_parent);

    /// Destructor
    ~ElementInput ();

    /// Type (=Input)
    Type type () const { return TypeInput; };

    /// Sets the valid flag (needed during code generation)
    void setValid (bool _value) { valid_ = _value; };

    /// Returns state of valid flag (needed during code generation)
    bool valid ();

    /// Set to a user value
    void setValue (QString _value);

    /// Returns value set by user
    QString value () const { return value_; };

    /// Add connection
    void addConnection (Connection *_conn);

    /// Remove connection
    void removeConnection (Connection *_conn);

    /// "Set" flag to mark input as set by user
    void set (bool _set);

    /// Return "set" flag
    bool isSet ();

    /// "ForceAsk" flag to mark an optional input for asking during script execution
    void setForceAsk (bool _set) { forceAsk_ = _set; };

    /// Return "ForceAsk" flag
    bool isForceAskSet () const { return forceAsk_; };

    /// VSI::Input state passthrough
    unsigned int state ();

    /// Save to xml
    void saveToXml (QDomDocument &_doc, QDomElement &_root);

    /// Load from xml
    void loadFromXml (QXmlQuery &_xml);

  private:
    Input *in_;


    bool valid_;

    bool isSet_;
    QString value_;

    bool forceAsk_;
};

//=============================================================================
}
//=============================================================================

#endif
