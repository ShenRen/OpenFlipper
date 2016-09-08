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

#include <ACG/GL/acg_glew.hh>
#include <ACG/GL/globjects.hh>
#include <ACG/GL/FilterKernels.hh>

#include "SnapshotDialog.hh"
#include <OpenFlipper/common/GlobalOptions.hh>

SnapshotDialog::SnapshotDialog(QString _suggest, bool _captureViewers, int _w, int _h, QWidget *parent)
 : QDialog(parent),
   captureViewers_(_captureViewers),
   aspect_((double)_w / (double)_h),
   blockSpinBox_(false),
   poissonImage_(128, 128, QImage::Format_RGB888),
   poissonFilter_(0)
{
  setupUi(this);

  filename->setText( _suggest );
  
  // Disable 'change resolution' button if
  // in viewer snapshot mode
  multisampling->setChecked(captureViewers_);
  changeRes_wdgt->setVisible(!captureViewers_);
  changeRes_pb->setEnabled(!captureViewers_);
  transparent->setDisabled(!captureViewers_);
  hideCoordsys->setDisabled(!captureViewers_);
  multisampling->setDisabled(!captureViewers_);
  supersampling->setDisabled(!captureViewers_);
  num_samples->setDisabled(!captureViewers_);
  supersampling_dist->setDisabled(!captureViewers_);
  
  snapWidth->setValue(_w);
  snapHeight->setValue(_h);
  
  warning_lb->setText("");

  // Load button states
  loadStates();
  
  if (keepAspect->isChecked()) {
      snapHeight->setValue((int)((double)snapWidth->value() / aspect_));
  }

  connect(snapWidth,  SIGNAL(valueChanged(int)), this, SLOT(snapWidthChanged(int)) );
  connect(snapHeight, SIGNAL(valueChanged(int)), this, SLOT(snapHeightChanged(int)) );
  connect(keepAspect, SIGNAL(stateChanged(int)), this, SLOT(keepAspectChanged()) );
  connect(multisampling, SIGNAL(stateChanged(int)), this, SLOT(multisampleChanged()));
  connect(supersampling, SIGNAL(stateChanged(int)), this, SLOT(supersampleChanged()));

  connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()) );
  connect(findButton, SIGNAL(clicked()), this, SLOT(findFile()) );
  connect(changeRes_pb,  SIGNAL(clicked()), this, SLOT(slotChangeResolution()) );
  connect(okButton,  SIGNAL(clicked()), this, SLOT(slotOk()) );

  connect(supersampling_dist, SIGNAL(valueChanged(double)), this, SLOT(updatePoisson()));
	connect(supersampling_res_incr, SIGNAL(valueChanged(int)), this, SLOT(updateResMultiplier()));

  connect(filename, SIGNAL(textChanged(const QString &)), this, SLOT(filenameChanged(const QString &)));


  updatePoisson();
}

SnapshotDialog::~SnapshotDialog() {
  delete poissonFilter_;
}

void SnapshotDialog::saveStates() {
    
    OpenFlipperSettings().setValue( "Viewer/SnapshotDialog/SnapWidth",      snapWidth->value());
    OpenFlipperSettings().setValue( "Viewer/SnapshotDialog/SnapHeight",     snapHeight->value());
    OpenFlipperSettings().setValue( "Viewer/SnapshotDialog/KeepAspect",     keepAspect->isChecked());
    OpenFlipperSettings().setValue( "Viewer/SnapshotDialog/Transparent",    transparent->isChecked());
    OpenFlipperSettings().setValue( "Viewer/SnapshotDialog/HideCoordsys",   hideCoordsys->isChecked());
    OpenFlipperSettings().setValue( "Viewer/SnapshotDialog/Multisampling",  multisampling->isChecked());
    OpenFlipperSettings().setValue( "Viewer/SnapshotDialog/NumSamples",     num_samples->value());
}

void SnapshotDialog::loadStates() {
    
    snapWidth->setValue(        OpenFlipperSettings().value( "Viewer/SnapshotDialog/SnapWidth",     snapWidth->value()).toInt());
    snapHeight->setValue(       OpenFlipperSettings().value( "Viewer/SnapshotDialog/SnapHeight",    snapHeight->value()).toInt());
    keepAspect->setChecked(     OpenFlipperSettings().value( "Viewer/SnapshotDialog/KeepAspect",    false).toBool());
    transparent->setChecked(    OpenFlipperSettings().value( "Viewer/SnapshotDialog/Transparent",   false).toBool());
    hideCoordsys->setChecked(   OpenFlipperSettings().value( "Viewer/SnapshotDialog/HideCoordsys",  false).toBool());
    multisampling->setChecked(  OpenFlipperSettings().value( "Viewer/SnapshotDialog/Multisampling", true).toBool());
    num_samples->setValue(      OpenFlipperSettings().value( "Viewer/SnapshotDialog/NumSamples",    16).toInt());
}

void SnapshotDialog::snapWidthChanged(int _w) {
    
    if(blockSpinBox_) return;
    
    if(keepAspect->isChecked()) {
        blockSpinBox_ = true;
        snapHeight->setValue((int)((double)_w / aspect_));
        blockSpinBox_ = false;
    }
}

void SnapshotDialog::snapHeightChanged(int _h) {
    
    if(blockSpinBox_) return;
    
    if(keepAspect->isChecked()) {
        blockSpinBox_ = true;
        snapWidth->setValue((int)((double)_h * aspect_));
        blockSpinBox_ = false;
    }
}

void SnapshotDialog::keepAspectChanged() {

    if(keepAspect->isChecked()) {
        blockSpinBox_ = true;
        snapHeight->setValue((int)((double)snapWidth->value() / aspect_));
        blockSpinBox_ = false;
    }
}

void SnapshotDialog::multisampleChanged() {
    num_samples->setDisabled (!multisampling->isChecked());
}


void SnapshotDialog::supersampleChanged() {
  supersampling_dist->setDisabled(!supersampling->isChecked());
}

void SnapshotDialog::slotChangeResolution()
{
  if ( !captureViewers_ )
    emit resizeApplication(snapWidth->value(), snapHeight->value());
}

void SnapshotDialog::slotOk()
{

  if (filename->text() == ""){
    QMessageBox msgBox;
    msgBox.setText(tr("The Filename is empty!"));
    msgBox.exec();
    return;
  }

  if ( !captureViewers_ )
    emit resizeApplication(snapWidth->value(), snapHeight->value());

  // Remember button states for next time...
  saveStates();
  
  accept();
}

void SnapshotDialog::findFile()
{

  QFileInfo fi( filename->text() );

  QFileDialog dialog(this);
  dialog.setFileMode(QFileDialog::AnyFile);
  dialog.setDefaultSuffix("png");
  dialog.setNameFilter(tr("Images (*.png *.ppm *.jpg)"));
  dialog.setFileMode(QFileDialog::AnyFile);
  dialog.setConfirmOverwrite(true);
  dialog.setDirectory( fi.path() );
  dialog.selectFile( filename->text() );
  dialog.setAcceptMode(QFileDialog::AcceptSave);
  dialog.setWindowTitle(tr("Save Snapshot"));

  bool ok = dialog.exec();

  if (ok)
    filename->setText( dialog.selectedFiles()[0] );
}

void SnapshotDialog::filenameChanged(const QString &new_filename) {
    QFileInfo fi(new_filename);
    if (!QFileInfo(fi.path()).isWritable()) {
        static const char *style = "background: #ffffcc;";
        filename->setStyleSheet(style);
        warning_lb->setText(trUtf8("Warning: Folder not writable."));
    } else if (fi.exists()) {
        static const char *style = "background: #ffcccc;";
        filename->setStyleSheet(style);
        warning_lb->setText(trUtf8("Warning: File exists and will be "
                "overwritten without further warning."));
    } else {
        static const char *style = "";
        filename->setStyleSheet(style);
        warning_lb->setText("");
    }
}

void SnapshotDialog::updatePoisson() {

  float d = supersampling_dist->value();

  delete poissonFilter_;
  poissonFilter_ = new ACG::PoissonBlurFilter(0.5f, d, 30, false);

  poissonFilter_->plotSamples(&poissonImage_);

	updateResMultiplier();
}

void SnapshotDialog::updateResMultiplier() {

	// separate distribution update and res multiplier update,
	// as adding grid lines for the muliplier is much faster than recomputing a poisson sample distribution

	// copy image
	poissonResMultImage_ = poissonImage_;

	int resMult = supersampling_res_incr->value();

	// add grid lines to visualize the resolution increase multiplier
	int w = poissonResMultImage_.width(),
		h = poissonResMultImage_.height();

	int dw = w / resMult,
		dh = h / resMult;

	QPainter painter;
	painter.begin(&poissonResMultImage_);
	painter.setPen(QPen(qRgb(0, 0, 0)));

	for (int i = 1; i < resMult; ++i) {
		painter.drawLine(0, dh * i, w, dh * i);
		painter.drawLine(dw * i, 0, dw * i, h);
	}

	// boundary
	painter.drawLine(0, 0, w, 0);
	painter.drawLine(0, h - 1, w, h - 1);
	painter.drawLine(0, 0, 0, h);
	painter.drawLine(w - 1, 0, w - 1, h);

	painter.end();

	poissonResMultPixmap_ = QPixmap::fromImage(poissonResMultImage_);
	poisson_samples->setPixmap(poissonResMultPixmap_);
}