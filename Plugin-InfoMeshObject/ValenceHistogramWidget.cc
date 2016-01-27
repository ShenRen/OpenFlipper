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

/*
 * ValenceHistogramWidget.cc
 *
 *  Created on: Jan 27, 2016
 *      Author: hc
 */

#include "ValenceHistogramWidget.hh"

#include <QPainter>
#include <iostream>

ValenceHistogramWidget::ValenceHistogramWidget(QWidget *parent) :
    histogram_(0) {

}

void ValenceHistogramWidget::paintEvent(QPaintEvent *event) {
    if (!histogram_) {
        QWidget::paintEvent(event);
        return;
    }

    /*
     * Analyze histogram/
     */
    std::vector<size_t>::iterator nonzero_begin = histogram_->begin();
    for (; nonzero_begin != histogram_->end() && *nonzero_begin == 0;
            ++nonzero_begin);
    if (nonzero_begin == histogram_->end()) return;

    std::vector<size_t>::iterator nonzero_end = histogram_->end();
    for (; (nonzero_end-1) != nonzero_begin && *(nonzero_end-1) == 0;
            --nonzero_end);

    const size_t hist_size = std::distance(nonzero_begin, nonzero_end);
    const size_t hist_max =  *std::max_element(nonzero_begin, nonzero_end);
    const size_t ofs = std::distance(histogram_->begin(), nonzero_begin);

    /*
     * Establish regions
     */
    QRectF paint_rect = this->contentsRect();
    QRectF bargraph_rect = paint_rect;
    bargraph_rect.setBottom(bargraph_rect.bottom() - 16);
    QRectF label_rect = paint_rect;
    label_rect.setTop(bargraph_rect.bottom());
    QPainter painter(this);

    /*
     * Painter attributes.
     */
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QColor::fromRgbF(0.518, 0.573, 0.643, 1.0));
    painter.setFont(this->font());

    const qreal stride =
            static_cast<qreal>(bargraph_rect.width()) / hist_size;
    const qreal gap = (stride > 8) ? 1.0 : 0.0;
    const qreal label_gap = 4;
    QRectF barRect(0, 0, stride - gap, 0);
    const qreal scale = static_cast<qreal>(bargraph_rect.height()) / hist_max;

    /*
     * Draw.
     */
    int cnt = 0;
    qreal lastLabelX = label_rect.left();
    for (std::vector<size_t>::iterator it = nonzero_begin;
            it != nonzero_end; ++it, ++cnt) {
        // Bar
        painter.setPen(Qt::NoPen);
        barRect.setHeight(scale * (*it));
        barRect.moveBottomLeft(
            bargraph_rect.bottomLeft() + QPoint(stride * cnt, 0));
        if (gap > 0.0)
            painter.drawRoundedRect(barRect, 3, 3, Qt::AbsoluteSize);
        else
            painter.drawRect(barRect);

        // Label
        painter.setPen(Qt::black);
        QString labelText = QString::number(cnt + ofs);
        QRectF labelBB =
            painter.boundingRect(
                QRectF(barRect.center().x()-50, label_rect.y(),
                        100.0, label_rect.height()),
                Qt::AlignHCenter | Qt::AlignBottom, labelText);
        if (labelBB.left() >= lastLabelX + label_gap) {
            painter.drawText(labelBB, Qt::AlignHCenter | Qt::AlignBottom,
                    labelText);
            lastLabelX = labelBB.right();
        }
    }
}

void ValenceHistogramWidget::setHistogram(std::vector<size_t> *histogram) {
    histogram_ = histogram;
    this->update();
}
