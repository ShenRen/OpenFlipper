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
#ifndef VSI_ELEMENTINOUT_HH
#define VSI_ELEMENTINOUT_HH

//== INCLUDES =================================================================

#include <QList>

//== FORWARDDECLARATIONS ======================================================
class QGraphicsItem;

//== NAMESPACES ===============================================================
namespace VSI {

//== FORWARDDECLARATIONS ======================================================
class Text;
class ConnectionPoint;
class Connection;
class InOut;
class GraphicsScene;
class SceneElement;

//== CLASS DEFINITION =========================================================

/** Base class for inputs and outputs of scene elements.
  * It provides the visible widgets and handles connections
  */
class ElementInOut {

  public:

    /// Type
    enum Type {
      TypeInput,
      TypeOutput
    };

    /// Constructor
    ElementInOut (InOut *_io, SceneElement *_parent);

    /// Destructor
    virtual ~ElementInOut ();

    /// Type
    virtual Type type () const = 0;

    /// Connection point widget
    ConnectionPoint *connectionPointItem () const { return conn_; }

    /// Type text widget
    Text *typeTextItem () const { return typeText_; }

    /// Short description widget
    Text *descriptionTextItem () const { return descText_; }

    /// Connections
    QList<Connection *> connections () const { return connections_; };

    /// Add the connection
    virtual void addConnection (Connection *_conn);

    /// Remove the Connection
    virtual void removeConnection (Connection *_conn);

    /// Can this input/output be connected to _e
    bool validConnection (ElementInOut *_e);

    /// InOut context object
    InOut* inOut () const { return io_; };

    /// Scene element
    SceneElement* element() { return element_; };

  private:

    /// Private copy constructor to avoid accidental copy
    ElementInOut( const ElementInOut& _orig );

    InOut *io_;
    SceneElement *element_;

    ConnectionPoint *conn_;
    Text            *typeText_;
    Text            *descText_;

    QList<Connection *> connections_;
};

//=============================================================================
}
//=============================================================================

#endif
