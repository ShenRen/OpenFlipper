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
*   $Revision: 10745 $                                                       *
*   $LastChangedBy: moebius $                                                *
*   $Date: 2011-01-26 10:23:50 +0100 (Mi, 26. Jan 2011) $                     *
*                                                                            *
\*===========================================================================*/

#include "philipsStereoSettingsWidget.hh"

#include <OpenFlipper/common/GlobalOptions.hh>

#include <iostream>

PhilipsStereoSettingsWidget::PhilipsStereoSettingsWidget(QWidget* _parent) : QWidget(_parent) {
  setupUi(this);

  connect(closeButton,SIGNAL(clicked()),this,SLOT(closeWidget()));

  connect(headerContentType,SIGNAL(currentIndexChanged (int)),this,SLOT(updateValues()));
  connect(headerFactor,SIGNAL(valueChanged(int)),this,SLOT(updateValues()));
  connect(headerOffsetCC,SIGNAL(valueChanged(int)),this,SLOT(updateValues()));
  connect(headerSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(updateValues()));
}

void PhilipsStereoSettingsWidget::closeWidget()  {
  updateValues();
  hide();
}

void PhilipsStereoSettingsWidget::updateValues()  {

  // Update labels that display the current values
  factorCounter->setNum(headerFactor->value());
  offsetCounter->setNum(headerOffsetCC->value());

  // Set option entries
  OpenFlipperSettings().setValue("Core/Stereo/Philips/Content",headerContentType->currentIndex());
  OpenFlipperSettings().setValue("Core/Stereo/Philips/Factor",headerFactor->value());
  OpenFlipperSettings().setValue("Core/Stereo/Philips/Offset",headerOffsetCC->value());
  OpenFlipperSettings().setValue("Core/Stereo/Philips/Select",headerSelect->currentIndex());

  // Tell others that we should update the view
  emit updateView();
}

