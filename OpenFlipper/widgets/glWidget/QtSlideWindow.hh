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

#ifndef QT_SLIDE_WINDOW_
#define QT_SLIDE_WINDOW_

//=============================================================================
//
//  CLASS QtSlideWindow
//
//=============================================================================

//== INCLUDES =================================================================

#include <QSettings>
#include <QGraphicsProxyWidget>
#include <QPropertyAnimation>
#include <QTimer>

#include <OpenFlipper/common/GlobalDefines.hh>

//== FORWARDDECLARATIONS ======================================================

class QtGraphicsButton;
class QTimeLine;
class QGraphicsItemAnimation;

//== CLASS DEFINITION =========================================================


/** \class QtSlideWindow QtSlideWindow.hh <OpenFlipper/widgets/glWidget/QtSlideWindow.hh>

   A graphics scene widget that has a hover slide effect and detach functionality
   for a child widget
 **/

class DLLEXPORT QtSlideWindow : public QGraphicsProxyWidget
{
    Q_OBJECT

  public:
    /** Create a glViewer.
    \param _name displayed titlebar name
    \param _parent parent graphics item
    */
    QtSlideWindow (QString _name = 0, QGraphicsItem *_parent = 0);

    /// recalculate geometry
    void updateGeometry ();

    /// attach a child widget
    void attachWidget (QWidget *_m);

    /// detach child widget
    void detachWidget ();


    /// saves the current state
    void saveState (QSettings &_settings);

    /// restores the state
    void restoreState (QSettings &_settings);

    /// Call this to correctly set start and ending positions
    void updateParentGeometry();

  private:

    /// paints decoration
    virtual void paintWindowFrame(QPainter *_painter, const QStyleOptionGraphicsItem *_option, QWidget *_widget = 0);

    /// track frame events
    virtual bool windowFrameEvent(QEvent *_e);

    /// categorize frame area
    virtual Qt::WindowFrameSection windowFrameSectionAt(const QPointF &_pos) const;

    /// hove event tracking
    virtual void hoverEnterEvent (QGraphicsSceneHoverEvent *_event);
    virtual void hoverLeaveEvent (QGraphicsSceneHoverEvent *_event);

    /// size & position event tracking
    virtual void resizeEvent (QGraphicsSceneResizeEvent *_event);
    virtual void moveEvent (QGraphicsSceneMoveEvent *_event);

  private slots:
    /// detach button pressed
    void detachPressed ();

    /// detached dialog closed
    void dialogClosed ();

    /// autohide button presed
    void autohidePressed ();

    /// Slide widget up
    void slideUp();

    /// Slide widget down
    void slideDown();

    /// Slot is called whenever the animation is finished
    void animationFinished();

    /// Start actual slide down
    void startSlideDownAnimation();

  private:

    // name
    QString name_;

    // child widget
    QWidget *mainWidget_;

    // buttons
    QtGraphicsButton *autohideButton_;
    QtGraphicsButton *detachButton_;

    // detached dialog
    QDialog *dialog_;

    // Starting position (for animation)
    QPointF startP_;

    // Ending position (for animation)
    QPointF endP_;

    // Track if widget is at bottom position
    bool down_;

    // Animation object
    QPropertyAnimation* animation_;

    // Is widget animating in this moment?
    bool animating_;

    // Wait some milliseconds before sliding widget down
    QTimer* timer_;
};

//=============================================================================
//=============================================================================
#endif // QT_SLIDE_WINDOW_ defined
//=============================================================================

