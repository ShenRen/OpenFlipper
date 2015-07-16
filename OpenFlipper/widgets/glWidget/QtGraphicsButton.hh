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



#ifndef QT_GRAPHICS_BUTTON_
#define QT_GRAPHICS_BUTTON_

//=============================================================================
//
//  CLASS QtGraphicsButton - IMPLEMENTATION
//
//=============================================================================

//== INCLUDES =================================================================

#include <QGraphicsItem>

//== CLASS DEFINITION =========================================================

/** Simple Button implementation for QGraphicsScene
*/


class QtGraphicsButton : public QObject, public QGraphicsItem
{
  Q_OBJECT
  Q_INTERFACES(QGraphicsItem)

  public:

    /** Create a glViewer.
    \param _image button imahe
    \param _parent parent graphics scene item
    \param _width button width
    \param _height button height
    */

    QtGraphicsButton (const QImage &_image, QGraphicsItem *_parent = 0, int _width = -1, int _height = -1);

  public:

    /// returns the bounding rect
    virtual QRectF boundingRect () const;

    /// paints the button
    virtual void paint (QPainter *_painter, const QStyleOptionGraphicsItem *_option, QWidget *_widget = 0);

    /// event tracking
    virtual void hoverEnterEvent (QGraphicsSceneHoverEvent *_event);
    virtual void hoverLeaveEvent (QGraphicsSceneHoverEvent *_event);
    virtual void mousePressEvent (QGraphicsSceneMouseEvent *_event);
    virtual void mouseReleaseEvent (QGraphicsSceneMouseEvent *_event);

    virtual QVariant itemChange (GraphicsItemChange _change, const QVariant &_value);

    /// makes the button checkable
    void setCheckable (bool _value);

    /// sets button checked state
    void setChecked (bool _value);

    /// returns button checked state
    bool isChecked () const;

  signals:

    /// signals a button press
    void pressed ();

  private:

    /// button state
    bool checkable_;
    bool checked_;
    bool pressed_;
    bool over_;

    /// button size
    int width_;
    int height_;

    /// pixmaps for different button states
    QPixmap normalPix_;
    QPixmap overPix_;
    QPixmap checkedPix_;
};

#endif

