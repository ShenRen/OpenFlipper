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

#ifndef VSI_ARROW_HH
#define VSI_ARROW_HH

//== INCLUDES =================================================================
#include <QGraphicsWidget>
#include <QGraphicsPathItem>
#include <QString>
#include <QTimer>

//== NAMESPACES ===============================================================
namespace VSI {

//== FORWARDDECLARATIONS ======================================================
class GraphicsScene;

//== CLASS DEFINITION =========================================================

/// Scene movement widget
class Arrow : public QObject, public QGraphicsPixmapItem, public QGraphicsLayoutItem
{
  Q_OBJECT
  Q_INTERFACES(QGraphicsLayoutItem)

  public:

    /// Movement direction
    enum Direction
    {
      North,
      South,
      East,
      West,
      Center
    };

  public:

    /// Constructor
    Arrow (GraphicsScene *_scene, QGraphicsItem *_parent, Direction _dir);

    /// Destructor
    ~Arrow ();

    /// Activates the timer for movement (will be called if an element is moved above)
    void activate ();

    /// Stop the timer
    void reset ();

    /// Highlights the widget if the scene can be moved in this direction
    void setHighlight (bool _highlight);

    /// Sets the geometry 
    virtual void setGeometry (const QRectF &_rect);

  protected:

    // catch mouse hover events
    virtual void hoverEnterEvent (QGraphicsSceneHoverEvent *_event);
    virtual void hoverLeaveEvent (QGraphicsSceneHoverEvent *_event);

    // catch mouse press/release events
    virtual void mousePressEvent (QGraphicsSceneMouseEvent *_event);
    virtual void mouseReleaseEvent (QGraphicsSceneMouseEvent *_event);

    // catch mouse wheel events
    virtual void wheelEvent (QGraphicsSceneWheelEvent *_event);

    // size information for layouting
    virtual QSizeF sizeHint ( Qt::SizeHint _which, const QSizeF & _constraint = QSizeF()) const;

  private slots:
    void timeout ();

  private:
    GraphicsScene *scene_;

    Direction dir_;

    QTimer timer_;

    int interval_;

    bool moveSelected_;

    bool highlight_;
};

//=============================================================================
}
//=============================================================================

#endif
