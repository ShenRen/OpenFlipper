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
*   $LastChangedBy$                                                 *
*   $Date$                    *
*                                                                            *
\*===========================================================================*/

#include <OpenFlipper/widgets/coreWidget/CoreWidget.hh>

/** Hide or show logger
 */
void CoreWidget::toggleLogger() {

    switch (OpenFlipper::Options::loggerState()) {
    case OpenFlipper::Options::InScene:
        OpenFlipper::Options::loggerState(OpenFlipper::Options::Normal);
        break;
    case OpenFlipper::Options::Normal:
        OpenFlipper::Options::loggerState(OpenFlipper::Options::Hidden);
        break;
    case OpenFlipper::Options::Hidden:
        OpenFlipper::Options::loggerState(OpenFlipper::Options::InScene);
        break;
    }

    // Hide/Show Logger
    showLogger(OpenFlipper::Options::loggerState());
}

//-----------------------------------------------------------------------------

/** Hide or show logger
 */
void CoreWidget::showLogger(OpenFlipper::Options::LoggerState _state) {

    //Hide Logger
    if (_state == loggerState_)
        return;

    switch (_state) {
    case OpenFlipper::Options::InScene: {

        showLoggerInSplitView(false);
        logWidget_->resize(logWidget_->width(), originalLoggerSize_);

        slidingLogger_->attachWidget(logWidget_);

        break;
    }
    case OpenFlipper::Options::Normal: {

        slidingLogger_->detachWidget();

        showLoggerInSplitView(true);

        logWidget_->show();

        break;
    }
    case OpenFlipper::Options::Hidden: {

        if (loggerState_ == OpenFlipper::Options::InScene)
            slidingLogger_->detachWidget();

        showLoggerInSplitView(false);

        break;
    }
    } // End switch

    loggerState_ = _state;
}

//-----------------------------------------------------------------------------

void CoreWidget::showLoggerInSplitView(bool _show) {

    if(!_show) {

        QList<int> wsizes = splitter_->sizes();

        // Remember old size
        if (loggerState_ == OpenFlipper::Options::Normal) {
            if(wsizes.size() > 1) {
                originalLoggerSize_ = wsizes[1];
            } else {
                originalLoggerSize_ = 240;
            }
        }

        if (originalLoggerSize_ == 0)
            originalLoggerSize_ = 240;

        logWidget_->setParent(0);
        wsizes[0] = 1;
        splitter_->setSizes(wsizes);
    } else {

        if (loggerState_ == OpenFlipper::Options::InScene)
            originalLoggerSize_ = logWidget_->height();

        if (originalLoggerSize_ == 0)
            originalLoggerSize_ = 240;

        logWidget_->setParent(splitter_);
        splitter_->insertWidget(1, logWidget_);
        QList<int> wsizes = splitter_->sizes();
        int totalHeight = wsizes[0] + wsizes[1];
        wsizes[1] = originalLoggerSize_;
        wsizes[0] = totalHeight - originalLoggerSize_;
        splitter_->setSizes(wsizes);
    }
}

//-----------------------------------------------------------------------------

void CoreWidget::updateInSceneLoggerGeometry() {

  if(loggerState_ == OpenFlipper::Options::InScene) {
      slidingLogger_->updateGeometry();
  }
}
