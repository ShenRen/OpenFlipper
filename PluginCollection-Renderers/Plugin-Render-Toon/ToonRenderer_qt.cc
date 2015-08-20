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
*   $Revision: 18127 $                                                       *
*   $LastChangedBy: moebius $                                                *
*   $Date: 2014-02-05 10:12:54 +0100 (Wed, 05 Feb 2014) $                     *
*                                                                            *
\*===========================================================================*/

/** @file
 *
 *  Contains definitions of the ToonRenderer Renderer that require qt headers
 *  which are incompatible with glew.h.
 */

#include "ToonRenderer.hh"

#include <QDialog>
#include <QColor>
#include <QLabel>
#include <QSlider>
#include <QVBoxLayout>
#include <ACG/QtWidgets/QtColorChooserButton.hh>

void ToonRenderer::actionDialog( bool )
{
  //generate widget
  QDialog* optionsDlg = new QDialog();
  QVBoxLayout* layout = new QVBoxLayout();
  layout->setAlignment(Qt::AlignTop);

  QColor curColor;
  curColor.setRgbF(outlineCol_[0],outlineCol_[1],outlineCol_[2]);

  QLabel* label = new QLabel(tr("Palette Size [0, 10]:"));
  layout->addWidget(label);

  QSlider* paletteSizeSlider = new QSlider(Qt::Horizontal);
  paletteSizeSlider->setRange(0, 1000);
  paletteSizeSlider->setValue(int(paletteSize_ * 100.0));
  paletteSizeSlider->setTracking(true);
  layout->addWidget(paletteSizeSlider);

  QtColorChooserButton* outlineColorBtn = new QtColorChooserButton("Outline Color");
  layout->addWidget(outlineColorBtn);

  outlineColorBtn->setColor( curColor );

  optionsDlg->setLayout(layout);


  connect(paletteSizeSlider, SIGNAL(sliderMoved(int)), this, SLOT(paletteSizeChanged(int)));
  connect(outlineColorBtn, SIGNAL(colorChanged(QColor)), this, SLOT(outlineColorChanged(QColor)));


  optionsDlg->show();
}
