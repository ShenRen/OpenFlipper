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
 * helpWidget.hh
 *
 *  Created on: Apr 7, 2009
 *      Author: kremer
 */

#ifndef HELPWIDGET_HH_
#define HELPWIDGET_HH_

#include "ui_helpWidget.hh"

#if QT_VERSION >= 0x050000 
  #include <QtWidgets>
#else
  #include <QtGui>
#endif


#include <QtHelp>
#include <QTextDocument>

#include <QHelpSearchEngine>
#include <QTabWidget>

#include <iostream>

#include "helpBrowser.hh"

class HelpWidget : public QMainWindow, public Ui::HelpWidget
{
  Q_OBJECT

  public:

	  HelpWidget(QWidget* parent = 0, const QString& _homeSite = "", const bool _loadHomeSite = true);

	  virtual ~HelpWidget();

	  void setHomeSite(const QString& _homeSite);

private slots:

	void startSearch();

	void goBack();

	void goForward();

	void goHome();

	void showSearchWidget();

	//the entry was already selected by user
	void linkActivated(const QUrl& _url);

	//activate the url and jump to the entry
	void activateLink(const QUrl& _url);

	void showFoundSite(const QUrl& _url);

	void showResults(int _hits);

	// updates the buttons an the content widget
	// the content widget is synchronized with the helpBrowser
	void update(const QUrl& url);

	void openFoundSite(QListWidgetItem* _item);

	//updates only the buttons (for/back)
	void updateButtons();

  private:

	// For the search popup
	QDockWidget* searchWidget_;

	QTabWidget* tabWidget_;

	HelpBrowser* textWindow_;

	QHelpEngine* helpEngine_;
	QHelpSearchEngine* searchEngine_;

	QString homeSite_;

	int homeIndex_;

};


#endif /* HELPWIDGET_HH_ */
