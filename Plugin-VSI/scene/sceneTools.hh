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

#ifndef VSI_SCENETOOLS_HH
#define VSI_SCENETOOLS_HH

//== INCLUDES =================================================================
#include <QGraphicsProxyWidget>
#include <QString>

//== NAMESPACES ===============================================================
namespace VSI {

//== FORWARDDECLARATIONS ======================================================
class GraphicsScene;
class Arrow;
class Trash;
class ZoomButton;
class FunctionDisplay;

//== CLASS DEFINITION =========================================================

/** Widget that acts as parent for the scene manipulation widgets (Arrows, ZoomButtons, Trash, ...)
  * It layouts itself (and also its childs) to the visible scene rectangle. It also
  * forwards events from the scene to its child widgets
  */
class SceneTools : public QGraphicsWidget
{
  Q_OBJECT

  public:

    /// Constructor
    explicit SceneTools (GraphicsScene *_scene);

    /// Destructor
    ~SceneTools ();

    /// Handles mouse movement (will be called by the scene, if the mouse with a draged element is moved)
    void mouseMove (QPointF _pos);

    /// Handles mouse release (will be called by the scene, if the element is dropped)
    void mouseRelease (QPointF _pos, QGraphicsItem *_item);

  private slots:

    // handle scene size changes
    void sceneRectChanged (const QRectF &_rect);

    // update arrow depending on visible scene rect
    void updateArrows ();

  private:
    GraphicsScene *scene_;

    Arrow *arrows_[5];
    Trash *trash_;
    ZoomButton *zoom_[2];

    FunctionDisplay *fDisplay_;
};

//=============================================================================
}
//=============================================================================

#endif
