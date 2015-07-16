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

#ifndef CURSORPAINTER_HH
#define CURSORPAINTER_HH

//== INCLUDES =================================================================

#include <QObject>
#include <QCursor>
#include <QVector>
#include <QRectF>

#include <ACG/GL/GLState.hh>
#include <OpenFlipper/common/GlobalDefines.hh>

//== NAMESPACES ===============================================================

//== FORWARDDECLARATIONS ======================================================
class glViewer;

//== CLASS DEFINITION =========================================================


/** Class that paints the cursor using gl calls
*/

class DLLEXPORT CursorPainter : public QObject
{
 Q_OBJECT

  public:
    /// Constructor
    CursorPainter (QObject *_parent = 0);

    /// Destructor
    ~CursorPainter ();

    /// Sets the current used cursor
    void setCursor (const QCursor &_cursor);

    /// Needs to be called after the gl context has been set up to initialize internal values
    void initializeGL ();

    /// Add a glViewer that will use this CursorPainter
    void registerViewer (glViewer *_viewer);

    /// Cursor painting function. The _state has to be setup that 0,0,0 is at the cursor position.
    void paintCursor (ACG::GLState *_state);

    /// Sets the current cursor position
    void updateCursorPosition (QPointF _scenePos);

    /// Return the current cursor position
    QPointF cursorPosition ();

    /// Enabled/Disables gl cursor painting
    void setEnabled (bool _enabled);

    /// Enabled/Disables native cursors
    void setForceNative (bool _enabled);

    /// Returns true if cursor painting is enabled and compatible cursor is set
    bool enabled ();

    /// Inform the cursor painter about mouse enter / leave
    void setMouseIn (bool _in);

    /// Bounding box of the cursor
    QRectF cursorBoundingBox ();

  private:
    // Create a texture for the cursor
    void cursorToTexture ();
    void cursorToCursor ();

  private:
    QCursor cursor_;
    QCursor nativeCursor_;
    QPointF cursorPos_;

    bool initialized_;

    QVector<glViewer *> views_;

    bool enabled_;
    bool mouseIn_;
    bool forceNative_;

    // x and y offset between click position and texture (hotspot)
    int xOff_;
    int yOff_;
    GLuint texture_;
    bool hasCursor_;
};

//=============================================================================
#endif
//=============================================================================
