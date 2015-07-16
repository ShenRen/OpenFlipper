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



#include "aboutWidget.hh"

#include <OpenFlipper/common/GlobalOptions.hh>

#include <iostream>

AboutWidget::AboutWidget(QWidget *parent )
    : QMainWindow(parent)
{
  setupUi(this);

  QIcon icon;
  icon.addFile(OpenFlipper::Options::iconDirStr()+OpenFlipper::Options::dirSeparator()+"window-close.png");
  actionClose->setIcon(icon);

  setWindowTitle(tr("About %1").arg(TOSTRING(PRODUCT_NAME)));
  const int idx = About->indexOf(tab_2);
  About->setTabText(idx, tr(TOSTRING(PRODUCT_NAME), "about box tab title"));

  if (QFile::exists(":/branding/license_text.html")) {
      OpenFlipperLicense->setText("");
      QFile licenseTextFile(":/branding/license_text.html");
      licenseTextFile.open(QFile::ReadOnly);
      QByteArray licenseTextBA = licenseTextFile.readAll();
      QString licenseText = QString::fromUtf8(licenseTextBA.data(), licenseTextBA.size());
      OpenFlipperLicense->setHtml(licenseText.arg(TOSTRING(PRODUCT_NAME)));
  } else {
      OpenFlipperLicense->setHtml(OpenFlipperLicense->toHtml().arg(TOSTRING(PRODUCT_NAME)));
  }

  connect( actionClose , SIGNAL(triggered() ) , this, SLOT(hide()) );
  connect( closeButton , SIGNAL(clicked()   ) , this, SLOT(hide()) );
  closeButton->setFocus();
}

void AboutWidget::keyPressEvent(QKeyEvent * event) {
    if (event->key() == Qt::Key_Escape || event->key() == Qt::Key_Enter || event->key() == Qt::Key_Space) {
        hide();
    }
}
