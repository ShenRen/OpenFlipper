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

#ifndef VSI_CONTEXT_HH
#define VSI_CONTEXT_HH

//== INCLUDES =================================================================
#include <QVector>
#include <QStringList>
#include <QXmlQuery>

#include "element.hh"

//== FORWARDDECLARATIONS ======================================================
class QScriptEngine;

//== NAMESPACES ===============================================================
namespace VSI {

//== FORWARDDECLARATIONS ======================================================
class Input;
class Output;
class InOut;
class Type;

//== CLASS DEFINITION =========================================================

/** Class holding all the informations / parsed xml metadata for the visual scripting interface
 */
class Context {

  public:
    /// Constructor
    explicit Context (QScriptEngine *_engine);

    /// Destructor
    ~Context ();

    /// Parse xml content
    void parse (QByteArray _xml);

    /// Returns all available elements
    const QVector<Element *>& elements () const { return elements_; };

    /// Returns all available elements for a given category
    QVector<Element *> elements (QString _category);

    /// Returns the element with a given name
    Element *element (QString _name);

    /// List of categories
    QStringList categories ();

    /// Registers a supported datatype
    void registerType (Type *_type);

    /// Is the given type supported
    bool typeSupported (QString _type);

    /// Get type object for given type name
    Type *getType (QString _type);

    /// Can the given types be converted to each other
    bool canConvert (QString _type1, QString _type2);

    /// Return script engine pointer
    QScriptEngine *scriptEngine () { return scriptEngine_; };

    /// Converts the given string to bool
    static bool strToBool (QString _str);

    /// Gets the string of a xml query
    static QString getXmlString (QXmlQuery &_xml, QString _expr, QString _default = "");

  private:

    // parse element from xml
    void parseElement (QXmlQuery &_xml);

    // parse element input from xml
    Input *parseInput (QXmlQuery &_xml, Element *_e);

    // parse element output from xml
    Output *parseOutput (QXmlQuery &_xml, Element *_e);

    // parse element function from xml
    Function *parseFunction (QXmlQuery &_xml, Element *_e);

    // parse common input and output parts from xml
    bool parseInOutBase (QXmlQuery &_xml, InOut *_io);

  private:
    QVector <Element *> elements_;

    QMap <QString, Type*> supportedTypes_;

    QList <Type *> types_;

    QScriptEngine *scriptEngine_;
};

//=============================================================================
}
//=============================================================================

#endif
