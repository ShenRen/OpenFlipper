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

/*
 * helpBrowser.cc
 *
 *  Created on: Apr 8, 2009
 *      Author: kremer
 */

#include "helpBrowser.hh"

#include <iostream>

HelpBrowser::HelpBrowser(QHelpEngine* _helpEngine, QWidget* parent) :

	QTextBrowser(parent),
	helpEngine_(_helpEngine),
	currentPage_(0),
	currentVirtualFolder_(""),
	currentNameSpace_("")
{
  connect(this, SIGNAL(sourceChanged(const QUrl&)), this, SLOT(rememberHistory(const QUrl&)));
}

HelpBrowser::~HelpBrowser() {

}


void HelpBrowser::updateNameSpaceAndFolder (const QUrl& _url) {

  // Extract the global virtual folder from this link
  QString link = _url.toString();
  QStringList linkParts = link.split("/");

  if ( linkParts.size() > 3) {
    currentVirtualFolder_ = linkParts[3];
    currentNameSpace_     = QString("org.openflipper.")+ linkParts[3].toLower() ;
  } else {
    currentNameSpace_ = "";
    currentVirtualFolder_ = "";
    std::cerr << "Unable to detect virtual folder or namespace of this link" << _url.toString().toStdString() << std::endl;
  }
}

void HelpBrowser::rememberHistory (const QUrl& _url) {

  QUrl newUrl = resolveUrl(_url);

  //if the site is already the current site
  //don't change the memory stack
  if ((visitedPages_.size() > 0) && (newUrl == visitedPages_[currentPage_]))
    return;

  // Delete the visited pages after the current position if they exist
  if ( currentPage_ < visitedPages_.size()-1 )
    visitedPages_.erase((visitedPages_.begin()+currentPage_+1),visitedPages_.end());

  visitedPages_.push_back(newUrl);
  currentPage_ = visitedPages_.size()-1;

  emit historyChanged(_url);
}

QUrl HelpBrowser::resolveUrl(const QUrl &_url)
{
  if (_url.scheme() == "qthelp") {

    updateNameSpaceAndFolder(_url);
    return _url;

  } else {

    QUrl newUrl;

    if ( _url.toString().startsWith("..") ) {

      // Relative url. We jump to a new context, so we first remove the relative part
      QUrl tmpURL("qthelp://" + currentNameSpace_ + "/" + currentVirtualFolder_ + "/");
      newUrl = tmpURL.resolved(_url);

      // Update context
      updateNameSpaceAndFolder(newUrl);
      return newUrl;

    } else {

      // Normal URL without relative parts so we can safely combine them
      // and stay in the current context
      return "qthelp://" + currentNameSpace_ + "/" + currentVirtualFolder_ + "/" + _url.toString();

    }
  }
}

QVariant HelpBrowser::loadResource (int /*_type*/, const QUrl& _url) {

  QUrl newUrl = resolveUrl(_url);
  const QUrl newFileUrl = helpEngine_->findFile(newUrl);

  if(newFileUrl.isValid())
    return QVariant(helpEngine_->fileData(newFileUrl));
  else {
    std::cerr << "Unable to find file at url : " << _url.toString().toStdString() << std::endl;
    return QVariant("Page not Found.");
  }

}

void HelpBrowser::open(const QString& _url) {

	open(QUrl(_url));
}

void HelpBrowser::open(const QUrl& _url, bool _skipSave) {

	QVariant data = this->loadResource(QTextDocument::HtmlResource, _url);

	QString txt;

	txt = data.toString();

	setHtml(txt);

  //jumps to a reference (Doxygen reference name and not section name)
  //references are at the end of url after last '#'
  QStringList Anchor = _url.toString().split("#");
  if (Anchor.size() > 1)
    this->scrollToAnchor(Anchor[Anchor.size()-1]);

  if (_skipSave)
  {
    disconnect(this,SLOT(rememberHistory(const QUrl&)));
    emit sourceChanged( _url );
    connect(this, SIGNAL(sourceChanged(const QUrl&)), this, SLOT(rememberHistory(const QUrl&)));
  }
  else
    emit sourceChanged( _url );

}

QUrl HelpBrowser::getCurrentDir(const QUrl& _url) {

	QStringList str_list = _url.toString().split("/");

	if ( str_list.size() > 0  )
	  str_list[str_list.size() - 1] = "";
	else
	  std::cerr << "Warning, getCurrentDir got invalid input: " << _url.toString().toStdString() << std::endl;

	QString nstr = str_list.join("/");

	return QUrl(nstr);
}

bool HelpBrowser::isForwardAvailable() {

	return currentPage_ < visitedPages_.size() - 1;
}

bool HelpBrowser::isBackwardAvailable() {

	return currentPage_ > 0;
}

void HelpBrowser::backward() {

	if(isBackwardAvailable()) {
		currentPage_--;
		open(visitedPages_[currentPage_], true);
	}

}

void HelpBrowser::forward() {

	if(isForwardAvailable()) {
		currentPage_++;
		open(visitedPages_[currentPage_], true);
	}

}

