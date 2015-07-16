/*===========================================================================*\
 *                                                                           *
 *                             OpenFlipper                                   *
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
*                                                                            *
*   $Revision: 13581 $                                                       *
*   $LastChangedBy: moebius $                                                *
*   $Date: 2012-01-30 14:22:52 +0100 (Mo, 30 Jan 2012) $                     *
*                                                                            *
\*===========================================================================*/

#include "ImageDialog.hh"

#include <iostream>

ImageDialog::ImageDialog(QImage _image, QWidget *parent) :
        QDialog(parent),
        imageWidth_(_image.width() ),
        imageHeight_(_image.height() ),
        dragging_(false)
{
  setupUi(this);

  pixmap_ = QPixmap::fromImage(_image).scaled(QSize(400,400),Qt::KeepAspectRatio,Qt::SmoothTransformation);

  image->setPixmap(pixmap_);

  // Set the correct boundaries for the spinboxes
  minX->setMaximum(imageWidth_);
  maxX->setMaximum(imageWidth_);
  maxX->setValue(imageWidth_);
  minY->setMaximum(imageHeight_);
  maxY->setMaximum(imageHeight_);
  maxY->setValue(imageHeight_);

  // Connect the spin boxes
  connect(minX,SIGNAL(valueChanged(int )),this,SLOT(slidersChanged()));
  connect(maxX,SIGNAL(valueChanged(int )),this,SLOT(slidersChanged()));
  connect(minY,SIGNAL(valueChanged(int )),this,SLOT(slidersChanged()));
  connect(maxY,SIGNAL(valueChanged(int )),this,SLOT(slidersChanged()));

  connect(image,SIGNAL(mouseButtonMoveEvent ( QPoint ))   ,this,SLOT(mouseButtonMoveEvent ( QPoint )));
  connect(image,SIGNAL(mouseButtonPressEvent ( QPoint ))  ,this,SLOT(mouseButtonPressEvent ( QPoint )));
  connect(image,SIGNAL(mouseButtonReleaseEvent ( QPoint )),this,SLOT(mouseButtonReleaseEvent ( QPoint )));

  setMouseTracking ( true);

}


void ImageDialog::slidersChanged() {

  // Copy to internal buffer
  buffer_ = pixmap_;

  QPainter painter( &buffer_);

  QPen pen(Qt::red);

  pen.setWidth(2);
  painter.setPen( pen );

  painter.setBrush(QBrush(Qt::red,Qt::NoBrush));

  const int minXVal = minX->value();
  const int maxXVal = maxX->value();
  const int minYVal = minY->value();
  const int maxYVal = maxY->value();

  // Calculate the scaling factor that is used to fit the image to the widget
  const double scale = (double)pixmap_.width() / (double)imageWidth_;

  // Working on the actual pixmap -> no offsets!
  painter.drawRect( minXVal * scale  , minYVal  * scale , (maxXVal-minXVal)  * scale, (maxYVal-minYVal)  * scale);

  painter.end();

  image->setPixmap(buffer_);
}

void ImageDialog::mouseButtonMoveEvent ( QPoint _p) {
  if ( dragging_ ) {

    // Calculate the right values for the position.
    // The image is scaled to 400x400
    // But the aspect ratio is kept. So we need to calculate the new sizes.



    // Calculate the free space above and before the real image
    // As we are working on the pixmap widget, we need to consider these offsets to calculate
    // coordinates inside the real pixmap.
    int xoffset = (400 - image->pixmap()->width() ) / 2;
    int yoffset = (400 - image->pixmap()->height()) / 2;

    // Calculate the scaling factor that is used to fit the image to the widget
    const double scale = (double)imageWidth_ / (double)image->pixmap()->width();

    int newXMin = (std::min(dragStartPoint_.x(),_p.x()) - xoffset ) * scale;
    int newXMax = (std::max(dragStartPoint_.x(),_p.x()) - xoffset ) * scale;

    int newYMin = (std::min(dragStartPoint_.y(),_p.y()) - yoffset ) * scale;
    int newYMax = (std::max(dragStartPoint_.y(),_p.y()) - yoffset ) * scale;

    minX->blockSignals(true);
    maxX->blockSignals(true);
    minY->blockSignals(true);
    maxY->blockSignals(true);

    minX->setValue(newXMin);
    maxX->setValue(newXMax);
    minY->setValue(newYMin);
    maxY->setValue(newYMax);

    minX->blockSignals(false);
    maxX->blockSignals(false);
    minY->blockSignals(false);
    maxY->blockSignals(false);

    slidersChanged();
  }
}

void ImageDialog::mouseButtonPressEvent (  QPoint _p ) {
  dragStartPoint_ = _p;
  dragging_       = true;
}

void ImageDialog::mouseButtonReleaseEvent (  QPoint _p ) {
  dragging_ = false;
}





