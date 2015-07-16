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




#include "optionsWidget.hh"
#include <iostream>
#include <OpenFlipper/common/GlobalOptions.hh>

void OptionsWidget::startDownload( QString _url ) {
   QUrl url(_url);

  QFileInfo urlInfo(_url);

  // Download the file to the Home Directory
  QFileInfo fileInfo( QDir::home().absolutePath() + OpenFlipper::Options::dirSeparator() +
                      ".OpenFlipper" + OpenFlipper::Options::dirSeparator() + urlInfo.fileName() );

  QString fileName = fileInfo.filePath();

  if (QFile::exists(fileName)) {
    QFile::remove(fileName);
  }

  file = new QFile(fileName);
  if (!file->open(QIODevice::WriteOnly)) {
    std::cerr << "Unable to Open local file " + fileName.toStdString() + " for writing" << std::endl;
    delete file;
    file = 0;
    checkUpdateButton->setEnabled(true);
  } else {
    QNetworkRequest req;
    req.setUrl(url);

    httpRequestAborted = false;
    QByteArray path = QUrl::toPercentEncoding(url.path(), "!$&'()*+,;=:@/");
    if (path.isEmpty())
      path = "/";

    statusLabel->setText(tr("Getting Versions file from Server"));

    progressDialog->setWindowTitle(tr("HTTP"));
    progressDialog->setLabelText(tr("Downloading %1.").arg(fileName));
    progressDialog->show();

    downloadRep_ = networkMan_->get(req);

    connect(downloadRep_, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(showError(QNetworkReply::NetworkError)));
    connect(downloadRep_,SIGNAL(downloadProgress(qint64 , qint64 )),
            this,SLOT(updateDataReadProgress(qint64 , qint64 )));

    checkUpdateButton->setEnabled(false);
  }

}

void OptionsWidget::authentication  ( QNetworkReply* _reply, QAuthenticator* _authenticator )
{
  if ( ! updateUser->text().isEmpty() )
    _authenticator->setUser(updateUser->text());

  if ( ! updatePass->text().isEmpty() )
    _authenticator->setPassword(updatePass->text());
}

void OptionsWidget::httpRequestFinished(QNetworkReply* _qnr)
{
  if (_qnr != downloadRep_)
    return;

  QNetworkReply::NetworkError error = _qnr->error();

  if (httpRequestAborted) {
    if (file) {
      file->close();
      file->remove();
      delete file;
      file = 0;
    }

    progressDialog->hide();
    checkUpdateButton->setEnabled(true);
    return;
  }

  progressDialog->hide();
  file->close();

  if (error != QNetworkReply::NoError) {
    file->remove();
  } else {
    QString fileName = QFileInfo(QUrl(updateURL->text()).path()).fileName();
    statusLabel->setText(tr("Downloaded %1").arg(file->fileName() ));
  }

  checkUpdateButton->setEnabled(true);
  delete file;
  file = 0;

  if ( error == QNetworkReply::NoError ) {
    if ( downloadType == VERSIONS_FILE )
      compareVersions();
    if ( downloadType == PLUGIN )
      updateComponent();
  }
}

void OptionsWidget::cancelDownload()
{
  statusLabel->setText(tr("download canceled."));
  httpRequestAborted = true;
  if (downloadRep_)
    downloadRep_->abort();
  checkUpdateButton->setEnabled(true);
}

void OptionsWidget::updateDataReadProgress(qint64 _bytesReceived, qint64 _bytesTotal)
{
  if (httpRequestAborted)
    return;

  progressDialog->setMaximum(_bytesTotal);
  progressDialog->setValue(_bytesReceived);
}

void OptionsWidget::showError(QNetworkReply::NetworkError _error)
{
  if (_error == QNetworkReply::NoError)
    return;
  statusLabel->setText(tr("Download failed: %1.").arg(downloadRep_->errorString()));
  QMessageBox::information(this, tr("HTTP Error"),
      tr("Download failed: %1.")
      .arg(downloadRep_->errorString()) + file->fileName() );
}





