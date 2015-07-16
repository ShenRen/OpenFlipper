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

#ifndef VSI_ELEMENT_HH_
#define VSI_ELEMENT_HH_

//== INCLUDES =================================================================
#include <QString>
#include <QVector>

//== NAMESPACES ===============================================================
namespace VSI {

//== FORWARDDECLARATIONS ======================================================
class Input;
class Output;
class InOut;
class Context;
class Function;

#define ELEMENT_FLAG_NO_DELETE 0x1
#define ELEMENT_FLAG_SKIP_TOOLBOX 0x2

//== CLASS DEFINITION =========================================================

/** Class that represents an element of the visual scripting interface.
  */
class Element {

  public:

    /// Constructor
    Element (Context *_context, QString _name);

    /// Destructor
    ~Element ();

    /// Context of element
    Context *context () const { return context_; };

    /// Element name
    QString name () const { return name_; };

    /// Element category
    const QString &category () const { return category_; };

    /// Short description
    const QString &shortDescription () const { return shortDesc_; };

    /// Long description
    const QString &longDescription () const { return longDesc_; };

    /// Inputs
    const QVector<Input *> &inputs () const { return inputs_; };

    /// Outputs
    const QVector<Output *> &outputs () const { return outputs_; };

    /// Functions
    const QVector <Function*> &functions () const { return functions_; };

    /// Scenegraph input
    Input *dataIn () { return dataIn_; };

    /// Scenegraph output
    Output *dataOut () { return dataOut_; };

    /// Flags
    unsigned int flags () const { return flags_; };

    /// Precode segment
    QString precode () const { return precode_; };

    /// Code segment
    QString code () const { return code_; };

    /// Returns an unused id number
    unsigned int getNewId ();

    /// sets the minimum for an unused id
    void setMinId (unsigned int _id);

    friend class Context;

  private:
    Context *context_;

    QString name_;
    QString category_;
    QString shortDesc_;
    QString longDesc_;

    QString precode_;
    QString code_;

    QVector <Input*> inputs_;
    QVector <Output*> outputs_;
    QVector <Function*> functions_;

    Input *dataIn_;
    Output *dataOut_;

    unsigned int flags_;

    unsigned int id_;
};

//=============================================================================
}
//=============================================================================

#endif
