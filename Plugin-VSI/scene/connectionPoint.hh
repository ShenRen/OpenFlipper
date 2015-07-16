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

#ifndef VSI_CONNECTIONPOINT_HH
#define VSI_CONNECTIONPOINT_HH

//== INCLUDES =================================================================
#include <QGraphicsItem>
#include <QGraphicsLayoutItem>
#include <QGraphicsSceneMouseEvent>
#include <QString>
#include <QTimer>
#include <QPointF>

//== NAMESPACES ===============================================================
namespace VSI {

//== FORWARDDECLARATIONS ======================================================
class GraphicsScene;
class ElementInOut;

//== CLASS DEFINITION =========================================================

/** Widget that represents the point where connection can be connected to/from.
  * It also displays the current status of its input/output in a different color
  */
class ConnectionPoint : public QObject, public QGraphicsEllipseItem, public QGraphicsLayoutItem
{
  Q_OBJECT
  Q_INTERFACES(QGraphicsLayoutItem)

  public:

    /// State of the connection point
    enum State {
      NoValue = 0,
      Optional = 1,
      RuntimeInput = 2,
      UserInput = 3,
      Connected = 4,
      OutputNotConnected = 5
    };

    /// Constructor
    ConnectionPoint (ElementInOut *_e, QGraphicsItem *_parent);

    /// Destructor
    ~ConnectionPoint ();

    /// Input/output element
    ElementInOut *inOut () const { return inout_; };

    /// Position for connections
    QPointF connectPos ();

    /// Sets the geometry
    virtual void setGeometry (const QRectF &_rect);

    /// sets the state
    void setState (State _state);

    /// Sets the diameter
    void setWidth (int _width);

    // painting
    void paint (QPainter *_painter, const QStyleOptionGraphicsItem *_option, QWidget *_widget = 0);

  protected:

    // start a new connection on press
    void mousePressEvent (QGraphicsSceneMouseEvent *_event);

    // highlight connection if mouse over
    void hoverEnterEvent (QGraphicsSceneHoverEvent *_event);
    void hoverLeaveEvent (QGraphicsSceneHoverEvent *_event);

    // size informations for layouting
    virtual QSizeF sizeHint ( Qt::SizeHint _which, const QSizeF & _constraint = QSizeF()) const;

  private:

    ElementInOut *inout_;

    int width_;
};

}

#endif
