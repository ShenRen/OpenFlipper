/*===========================================================================*\
 *                                                                           *
 *                              OpenFlipper                                  *
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
 *                                                                           *
\*===========================================================================*/

/*===========================================================================*\
 *                                                                           *
 *   $Revision$                                                       *
 *   $Author$                                                       *
 *   $Date$                   *
 *                                                                           *
\*===========================================================================*/

/*
 * SpinBoxEventFilter.cc
 *
 *  Created on: Aug 1, 2013
 *      Author: ebke
 */

//#define VERBOSE_DEBUGGING_OUTPUT

#include "SpinBoxEventFilter.hh"

#include <QAbstractSpinBox>
#include <QEvent>
#include <QMetaEnum>
#include <stack>

#ifdef VERBOSE_DEBUGGING_OUTPUT
#include <iostream>
#endif

SpinBoxEventFilter::SpinBoxEventFilter(QObject *parent) :
    QObject(parent), scrolling(false) {

    scrollingTimer.setSingleShot(true);
    scrollingTimer.setInterval(500);

    connect(&scrollingTimer, SIGNAL( timeout() ), this, SLOT( unsetScrolling() ));
}

SpinBoxEventFilter::~SpinBoxEventFilter() {
}

bool SpinBoxEventFilter::eventFilter(QObject *object, QEvent *event) {

    if (event->type() != QEvent::Wheel) return QObject::eventFilter(object, event);

    QWidget *widget = qobject_cast<QWidget*>(object);

    if (scrollAreas.find(widget) != scrollAreas.end()) {
#ifdef VERBOSE_DEBUGGING_OUTPUT
        std::cout << "Reset isScrolling." << std::endl;
#endif
        setScrolling();
        return QObject::eventFilter(object, event);
    }

    if (!isScrolling() && widget->isEnabled()) {
#ifdef VERBOSE_DEBUGGING_OUTPUT
        std::cout << "Not scrolling. Letting wheel event pass." << std::endl;
#endif
        return QObject::eventFilter(object, event);
    }

    setScrolling();
    event->ignore();
#ifdef VERBOSE_DEBUGGING_OUTPUT
    std::cout << "Swallowing wheel event." << std::endl;
#endif
    return true;

//#ifdef VERBOSE_DEBUGGING_OUTPUT
//    std::cout << "Not a spin box. Letting wheel event pass." << std::endl;
//#endif
//    return QObject::eventFilter(object, event);
}

inline bool SpinBoxEventFilter::isScrolling() {
    return scrolling;
}

void SpinBoxEventFilter::hookUpToWidgetTree(QWidget *root_widget) {
    std::stack<QWidget*> dfs;
    dfs.push(root_widget);

    while (!dfs.empty()) {
        QWidget *cur = dfs.top(); dfs.pop();

        cur->installEventFilter(this);

        const QObjectList &children = cur->children();
        for (QObjectList::const_iterator it = children.begin(), it_end = children.end();
                it != it_end; ++it) {

            QWidget *widget = qobject_cast<QWidget*>(*it);
            if (widget)
                dfs.push(widget);
        }
    }
}

void SpinBoxEventFilter::registerScrollArea(QWidget *scrollArea) {
    scrollAreas.insert(scrollArea);
    scrollArea->installEventFilter(this);
}

void SpinBoxEventFilter::setScrolling() {
    scrolling = true;
    scrollingTimer.start();
}

void SpinBoxEventFilter::unsetScrolling() {
#ifdef VERBOSE_DEBUGGING_OUTPUT
    std::cout << "Scrolling timeout." << std::endl;
#endif
    scrolling = false;
}
