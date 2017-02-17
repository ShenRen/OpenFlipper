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

#ifndef VSI_TEXT_HH
#define VSI_TEXT_HH

//== INCLUDES =================================================================
#include <QGraphicsItem>
#include <QGraphicsLayoutItem>
#include <QString>
#include <QTimer>
#include <QPen>
#include <QBrush>
#include <QPainterPath>

//== NAMESPACES ===============================================================
namespace VSI {

//== FORWARDDECLARATIONS ======================================================
class GraphicsScene;

//== CLASS DEFINITION =========================================================

/** Layoutable text widget implementation with an optional background.
  * The background can have convex or concave sides.
  */
class Text : public QObject, public QGraphicsSimpleTextItem, public QGraphicsLayoutItem
{
  Q_OBJECT
  Q_INTERFACES(QGraphicsLayoutItem)

  public:

    /// Constructor
    explicit Text (QGraphicsItem *_parent = 0);
    Text (const QString &_text, QGraphicsItem *_parent = 0);

    /// Destrucotr
    ~Text ();

    /// Sets the geometry
    virtual void setGeometry (const QRectF &_rect);

    /// Returns the shape for proper repainting/event handling
    QPainterPath shape () const;

    /// Background painting
    virtual void paint (QPainter *_painter, const QStyleOptionGraphicsItem *_option, QWidget *_widget = 0);

    /// Bounding rectangle
    virtual QRectF boundingRect () const;

    /// Returns the current background brush
    QBrush backgroundBrush () { return backgroundBrush_; };

    /// Sets the background brush
    virtual void setBackgroundBrush (QBrush _brush);

    /// Returns the current background pen
    QPen backgroundPen () { return backgroundPen_; };

    /// Sets the background pen
    virtual void setBackgroundPen (QPen _pen);

    /// Enables background painting
    void setBackground (bool _leftOut, bool _rightOut);

    /// Disables backgorund painting
    void clearBackground ();

    /// Should this widget be stretchable in horizontal direction
    void setHorizontalStretch (bool _stretch);

    /// Placement of the text in a stretched widget 
    void setAlignment (Qt::Alignment _alignment);

    /// Set displayed text
    void setText (QString _text) {text_ = _text; update (); };

    /// Get displayed text
    QString text () { return text_; };

  protected:

    // size information for layouting
    virtual QSizeF sizeHint ( Qt::SizeHint _which, const QSizeF & _constraint = QSizeF()) const;

  private:

    QString text_;

    bool backgroundSet_;

    QBrush backgroundBrush_;
    QPen backgroundPen_;

    bool leftOut_;
    bool rightOut_;

    Qt::Alignment alignment_;
    bool hStretch_;
};

//=============================================================================
}
//=============================================================================

#endif
