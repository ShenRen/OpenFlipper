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
 *  Contains definitions of the PostProcessorPhilipsStereoPlugin that require qt headers
 *  which are incompatible with glew.h.
 */

#include "PostProcessorPhilipsStereo.hh"
#include "widgets/philipsStereoSettingsWidget.hh"
#include <OpenFlipper/common/GlobalOptions.hh>


#if QT_VERSION >= 0x050000
#else
 #include <QGLFormat>
#endif
void  PostProcessorPhilipsStereoPlugin::slotShowOptionsMenu() {

  // Create widget if it does not exist
  if ( settingsWidget_ == 0) {
    settingsWidget_ = new PhilipsStereoSettingsWidget(0);
    connect(settingsWidget_,SIGNAL(updateView()),this,SIGNAL(updateView()));
  }

  // Set values Philips stereo mode

  // Block signals such that slotApplyStereoSettings
  // won't be called when setting the initial values here...
  settingsWidget_->headerContentType->blockSignals(true);
  settingsWidget_->headerSelect->blockSignals(true);
  settingsWidget_->offsetCounter->blockSignals(true);
  settingsWidget_->headerFactor->blockSignals(true);
  settingsWidget_->factorCounter->blockSignals(true);
  settingsWidget_->headerOffsetCC->blockSignals(true);

  settingsWidget_->headerContentType->setCurrentIndex(OpenFlipperSettings().value("Core/Stereo/Philips/Content",3).toInt());
  settingsWidget_->headerFactor->setValue(OpenFlipperSettings().value("Core/Stereo/Philips/Factor",64).toInt());
  settingsWidget_->headerOffsetCC->setValue(OpenFlipperSettings().value("Core/Stereo/Philips/Offset",128).toInt());
  settingsWidget_->factorCounter->setNum(OpenFlipperSettings().value("Core/Stereo/Philips/Factor",64).toInt());
  settingsWidget_->offsetCounter->setNum(OpenFlipperSettings().value("Core/Stereo/Philips/Offset",128).toInt());
  settingsWidget_->headerSelect->setCurrentIndex(OpenFlipperSettings().value("Core/Stereo/Philips/Select",0).toInt());

  // Unblock signals
  settingsWidget_->headerContentType->blockSignals(false);
  settingsWidget_->headerSelect->blockSignals(false);
  settingsWidget_->offsetCounter->blockSignals(false);
  settingsWidget_->headerFactor->blockSignals(false);
  settingsWidget_->factorCounter->blockSignals(false);
  settingsWidget_->headerOffsetCC->blockSignals(false);



  // Move widget to the position of the cursor
  settingsWidget_->move( QCursor::pos() - QPoint((int)(settingsWidget_->width()/2), 0));

  settingsWidget_->show();

}
