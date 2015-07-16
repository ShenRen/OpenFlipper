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

#ifndef BLOCKINGWIDGET_HH
#define BLOCKINGWIDGET_HH

#include "processManagerWidget.hh"

class BlockingWidget : public QWidget {
Q_OBJECT

signals:
	void cancelRequested(QString _jobId);

private slots:

	void cancelPressed() {
		JobCancelButton* button = 0;
		button = dynamic_cast<JobCancelButton*>(QObject::sender());
		if(button != 0) {
			emit cancelRequested(button->jobId());
		}
	};

public:
	BlockingWidget(QString _jobId, QString _description,
			int _minSteps, int _maxSteps, QWidget* _parent = 0) : QWidget(_parent),
			layout_(0),
			description_(0),
			progress_(0),
			cancelButton_(0) {

        // Block all other windows of application
        setWindowModality(Qt::ApplicationModal);
		setWindowTitle(_jobId);
		// Set window size
		QSize size(300, 150);
		resize(size);

		layout_ = new QVBoxLayout();
		description_ = new QLabel(_description);
		layout_->addWidget(description_);

		progress_ = new QProgressBar();
		progress_->setMinimum(_minSteps);
		progress_->setMaximum(_maxSteps);
		progress_->setValue(0);
		progress_->setTextVisible(true);
		layout_->addWidget(progress_);

		cancelButton_ = new JobCancelButton("Cancel", _jobId, this);
		layout_->addWidget(cancelButton_);

		setLayout(layout_);

		// Connect cancel button
		connect(cancelButton_, SIGNAL(pressed()), this, SLOT(cancelPressed()));
	};

	void updateStatus(int _value) { progress_->setValue(_value); };

	void setJobId(QString _jobId) { setWindowTitle(_jobId); };

	void setJobDescription(QString _description) { description_->setText(_description); };

private:
	QVBoxLayout* 		layout_;
	QLabel* 			description_;
	QProgressBar* 		progress_;
	JobCancelButton*	cancelButton_;
};

#endif // BLOCKINGWIDGET_HH
