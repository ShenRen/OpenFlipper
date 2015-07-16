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

//=============================================================================
//
//  CLASS QtSlideWindow
//
//=============================================================================


//== GLOBAL DEFINITIONS=========================================================

#define BACKGROUND_RED   0xff
#define BACKGROUND_GREEN 0xff
#define BACKGROUND_BLUE  0xff
#define BACKGROUND_ALPHA 0xcf

#define SLIDE_DURATION        300
#define WAIT_UNTIL_SLIDE_DOWN 500

//== INCLUDES =================================================================

#include <OpenFlipper/common/GlobalOptions.hh>

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDialog>
#include <QVBoxLayout>

#include "QtSlideWindow.hh"
#include "QtGraphicsButton.hh"

//== IMPLEMENTATION ==========================================================

QtSlideWindow::QtSlideWindow(QString _name, QGraphicsItem *_parent) :
    QGraphicsProxyWidget(_parent),
    name_(_name),
    mainWidget_(0),
    autohideButton_(0),
    detachButton_(0),
    dialog_(0),
    down_(false),
    animating_(false),
    timer_(0) {

    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    setWindowFrameMargins(2, 15, 2, 2);
    setZValue(2.0);

    QImage autohide(OpenFlipper::Options::iconDirStr() + OpenFlipper::Options::dirSeparator() + "button-autohide.png");
    QImage detach(OpenFlipper::Options::iconDirStr() + OpenFlipper::Options::dirSeparator() + "button-detach.png");

    autohideButton_ = new QtGraphicsButton(autohide, this, 12, 12);
    detachButton_ = new QtGraphicsButton(detach, this, 12, 12);

    autohideButton_->setCheckable(true);
    autohideButton_->setChecked(true);
    autohideButton_->setPos(geometry().width() - 12, -13);
    detachButton_->setPos(geometry().width() - 25, -13);

    connect(detachButton_, SIGNAL(pressed()), this, SLOT(detachPressed()));
    connect(autohideButton_, SIGNAL(pressed()), this, SLOT(autohidePressed()));

    // Create animation object
    animation_ = new QPropertyAnimation(this, "pos");
    animation_->setDuration(SLIDE_DURATION);
    connect(animation_, SIGNAL(finished()), this, SLOT(animationFinished()));

    // Create timer
    timer_ = new QTimer();
    timer_->setSingleShot(true);

    // Wait some milliseconds before starting the actual slide down animation
    connect(timer_, SIGNAL(timeout()), this, SLOT(startSlideDownAnimation()));

    // Hide widget
    hide();

    setAcceptHoverEvents(true);

    updateParentGeometry();
}

//-----------------------------------------------------------------------------

void QtSlideWindow::attachWidget(QWidget *_m) {

    if (!_m)
        return;

    mainWidget_ = _m;
    mainWidget_->setParent(0);

    if (autohideButton_->isChecked()) {
        setWidget(mainWidget_);
        setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

        down_ = true;

        show();
    }
    updateGeometry();
}

//-----------------------------------------------------------------------------

void QtSlideWindow::detachWidget() {

    setWidget(0);
    hide();

    if (mainWidget_) {
        mainWidget_->setParent(0);
        mainWidget_ = 0;
    }

    if (dialog_) {
        disconnect (dialog_, SIGNAL(finished (int)), this, SLOT(dialogClosed ()));
        dialog_->close();
        delete dialog_;
        dialog_ = 0;
    }
}

//-----------------------------------------------------------------------------

void QtSlideWindow::paintWindowFrame(QPainter *_painter, const QStyleOptionGraphicsItem* /*_option*/, QWidget* /*_widget*/) {

    int w = geometry().width();
    int h = geometry().height();

    _painter->setRenderHint(QPainter::Antialiasing, true);
    _painter->setBrush(QBrush(QColor(BACKGROUND_RED, BACKGROUND_GREEN, BACKGROUND_BLUE, BACKGROUND_ALPHA)));
    _painter->setPen(QColor(BACKGROUND_RED / 4, BACKGROUND_GREEN / 4, BACKGROUND_BLUE / 4, BACKGROUND_ALPHA));
    _painter->drawRoundedRect(-2, -15, w + 4, h + 40, 4, 4);

    _painter->setPen(Qt::SolidLine);

    _painter->drawText(2, -13, w - 4, 11, Qt::AlignCenter, name_);
}

//-----------------------------------------------------------------------------

bool QtSlideWindow::windowFrameEvent(QEvent *_e) {

    if(_e->type() == QEvent::GraphicsSceneMousePress || _e->type() == QEvent::GraphicsSceneMouseRelease) {
        QGraphicsSceneMouseEvent *ge = dynamic_cast<QGraphicsSceneMouseEvent*> (_e);
        if(windowFrameSectionAt(ge->pos()) != Qt::TopSection) {
            _e->accept();
            return false;
        }
    }

    return QGraphicsProxyWidget::windowFrameEvent(_e);
}

//-----------------------------------------------------------------------------

Qt::WindowFrameSection QtSlideWindow::windowFrameSectionAt(const QPointF &_pos) const {

    if (_pos.x() >= 2 && _pos.x() < geometry().width() - 2 - (13 * 2) && _pos.y() >= -15 && _pos.y() <= -10) {
        return Qt::TopSection;
    }

    return Qt::NoSection;
}

//-----------------------------------------------------------------------------

void QtSlideWindow::hoverEnterEvent(QGraphicsSceneHoverEvent *) {

    // Stop slide down action after timeout if the cursor
    // re-enters the logger within the timeout interval
    if(timer_->isActive()) {
        timer_->stop();
        return;
    }

    // Don't do anything if animation is currently in progress
    if(animating_) return;

    if (autohideButton_->isChecked() && down_) {
        slideUp();
    }
    if (!mainWidget_->isVisible()) {
        setWidget(mainWidget_);
        setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
        show();
    }
}

//-----------------------------------------------------------------------------

void QtSlideWindow::hoverLeaveEvent(QGraphicsSceneHoverEvent *) {

    // Don't do anything if animation is currently in progress
    if(animating_) return;

    if (autohideButton_->isChecked() && !down_) {
        slideDown();
    }
}

//-----------------------------------------------------------------------------

void QtSlideWindow::resizeEvent(QGraphicsSceneResizeEvent *_event) {

    QGraphicsProxyWidget::resizeEvent(_event);
}

//-----------------------------------------------------------------------------

void QtSlideWindow::moveEvent(QGraphicsSceneMoveEvent *_event) {

    QGraphicsProxyWidget::moveEvent(_event);
}

//-----------------------------------------------------------------------------

void QtSlideWindow::detachPressed() {

    setWidget(0);

    dialog_ = new QDialog(0, Qt::Window);
    dialog_->setWindowTitle(name_);
    dialog_->setLayout(new QVBoxLayout);
    dialog_->resize(mainWidget_->size());
    dialog_->layout()->addWidget(mainWidget_);

    if (scene() && scene()->views()[0] && scene()->views()[0]->window()) {
        QWidget *w = scene()->views()[0]->window();
        int x = (w->width() - mainWidget_->width()) / 2;
        x += w->x();
        x = qMax(0, x);
        int y = (w->height() - mainWidget_->height()) / 2;
        y += w->y();
        y = qMax(0, y);
        dialog_->move(x, y);
    }

    hide();

    dialog_->show();
    connect (dialog_, SIGNAL(finished (int)), this, SLOT(dialogClosed ()));
}

//-----------------------------------------------------------------------------

void QtSlideWindow::dialogClosed() {

    dialog_ = 0;
    mainWidget_->setParent(0);

    setWidget(mainWidget_);
    setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    show();

    updateGeometry();
}

//-----------------------------------------------------------------------------

void QtSlideWindow::autohidePressed() {

    if(!autohideButton_->isChecked() && down_) {
        slideUp();
    }
}

//-----------------------------------------------------------------------------

void QtSlideWindow::updateParentGeometry() {

    startP_.setX(10);
    startP_.setY(parentWidget()->geometry().height() - geometry().height());

    endP_.setX(10);
    endP_.setY(parentWidget()->geometry().height() - (size().height() - geometry().height()));
}

//-----------------------------------------------------------------------------

#include <iostream>
void QtSlideWindow::animationFinished() {

    animating_ = false;

    // If we reached up position,
    // check if the mouse is still inside (could leave in between as we block the leave event
    // when animating to avoid flickering
    // If mouse is not inside anymore, we start the countdown for slideDown
    if ( !down_ && !isUnderMouse() )
      timer_->start(WAIT_UNTIL_SLIDE_DOWN);
}

//-----------------------------------------------------------------------------

void QtSlideWindow::slideUp() {

    if(!widget()) return;

    updateParentGeometry();

    down_ = false;
    animating_ = true;

    animation_->setStartValue(endP_);
    animation_->setEndValue(startP_);

    animation_->start();
}

//-----------------------------------------------------------------------------

void QtSlideWindow::slideDown() {

    if(!widget()) return;

    updateParentGeometry();

    timer_->start(WAIT_UNTIL_SLIDE_DOWN);
}

//-----------------------------------------------------------------------------

void QtSlideWindow::startSlideDownAnimation() {

    down_ = true;
    animating_ = true;

    animation_->setStartValue(startP_);
    animation_->setEndValue(endP_);

    animation_->start();
}

//-----------------------------------------------------------------------------

void QtSlideWindow::updateGeometry() {

    if (parentWidget() && widget()) {

        updateParentGeometry();

        resize(parentWidget()->geometry().width() - 20, widget()->size().height());

        if(down_) {
            setPos(endP_);
        } else {
            setPos(startP_);
        }

        if (autohideButton_) {
            autohideButton_->setPos(geometry().width() - 12, -13);
        }
        if (detachButton_) {
            detachButton_->setPos(geometry().width() - 25, -13);
        }
    }
}

//-----------------------------------------------------------------------------

void QtSlideWindow::saveState(QSettings &_settings) {

    _settings.setValue("AutoHide", autohideButton_->isChecked());
    _settings.setValue("Detached", (dialog_ != 0));
    _settings.setValue("WidgedGeometry", (mainWidget_) ? mainWidget_->saveGeometry() : QByteArray());
    _settings.setValue("DialogGeometry", (dialog_) ? dialog_->saveGeometry() : QByteArray());
}

//-----------------------------------------------------------------------------

void QtSlideWindow::restoreState(QSettings &_settings) {

    autohideButton_->setChecked(_settings.value("AutoHide", autohideButton_->isChecked()).toBool());

    if (_settings.value("Detached", false).toBool() && !dialog_ && mainWidget_) {
        detachPressed();
    }

    if (mainWidget_) {
        mainWidget_->restoreGeometry(_settings.value("WidgedGeometry").toByteArray());
    }

    if (autohideButton_->isChecked()) {

    } else {
        if (!mainWidget_->isVisible()) {
            setWidget(mainWidget_);
            setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
            show();
        }
    }

    if (dialog_)
        dialog_->restoreGeometry(_settings.value("DialogGeometry").toByteArray());
}

//=============================================================================

