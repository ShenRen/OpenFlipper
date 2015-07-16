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
 * helpWidget.cc
 *
 *  Created on: Apr 7, 2009
 *      Author: kremer
 */

#include "helpWidget.hh"

#include <OpenFlipper/common/GlobalOptions.hh>

#include <iostream>

#include <QTextStream>
#include <QDir>

//#define DEBUG_HELP_CONTENTS

HelpWidget::HelpWidget(QWidget* parent, const QString& _homeSite /*=""*/, const bool _loadHomeSite /*= true*/)
  : QMainWindow(parent),
  searchWidget_(0),
  tabWidget_(0),
  textWindow_(0),
  helpEngine_(0),
  searchEngine_(0) {

  setupUi(this);

  homeSite_ = _homeSite;

  QString filename = QString( OpenFlipper::Options::configDirStr() );
  filename += OpenFlipper::Options::dirSeparator();
  filename += "Help.qhc";
    
  QDir helpDir = QDir(OpenFlipper::Options::helpDirStr());

  QString iconPath = QString(OpenFlipper::Options::iconDirStr())+QString(OpenFlipper::Options::dirSeparator());

  // Set Buttons
  backButton_->setIcon(QIcon(iconPath+"arrow-left.png"));
  forwardButton_->setIcon(QIcon(iconPath+"arrow-right.png"));
  homeButton_->setIcon(QIcon(iconPath+"go-home.png"));
  searchButton_->setIcon(QIcon(iconPath+"edit-find.png"));

  tabWidget_ = new QTabWidget(this);

  // Generate the help engine. The filename is in the users home directory and
  // contains all registered help resources.
  helpEngine_ = new QHelpEngine(filename, this);
  
  helpEngine_->setupData();
  
  // Get all currently registered nameSpaces
  QStringList registeredNamespaces =helpEngine_->registeredDocumentations();

  QStringList documentationFiles;

  // Get a list of all loaded documentation files from the namespaces
  QStringList helpFiles = helpDir.entryList(QStringList("*.qch"),QDir::Files);

  // Write absolute path into filenames
  for (QStringList::iterator iter = helpFiles.begin(); iter != helpFiles.end(); ++iter)
    *iter = helpDir.path()+ OpenFlipper::Options::dirSeparator() + *iter;

  for ( int i = 0; i < registeredNamespaces.size() ; ++i)
  {
    QString registredFilename (helpEngine_->documentationFileName(registeredNamespaces[i]));

    // re-register documentation if location changed
    if (helpFiles.indexOf(registredFilename) != -1)
      documentationFiles.push_back( registredFilename );
    else
      helpEngine_->unregisterDocumentation(registeredNamespaces[i]);
  }

  for ( int i = 0 ; i < helpFiles.size() ; ++i ) {
    const QString filename = helpFiles[i];

    // Don't register files twice (stored between multiple OpenFlipper executions)
    if (documentationFiles.contains(filename))
      continue;

    // Try to register the file
    if ( !helpEngine_->registerDocumentation( filename ) ) {
      std::cerr << "Error when trying to register file " << filename.toStdString() << std::endl;
      std::cerr << helpFiles[i].toStdString() << " :" << helpEngine_->error().toStdString() << std::endl;
    }

  }

  searchEngine_ = new QHelpSearchEngine(helpEngine_, this);

  textWindow_ = new HelpBrowser(helpEngine_, this);

  homeIndex_ = tabWidget_->addTab(textWindow_, tr("Home"));

  gridLayout_->addWidget(helpEngine_->contentWidget(), 1, 0);
  gridLayout_->addWidget(tabWidget_, 1, 1);

  gridLayout_->setColumnStretch(0, 1);
  gridLayout_->setColumnStretch(1, 3);

  // Search popup at bottom of window
  searchWidget_ = new QDockWidget(tr("Search results"), this);
  searchWidget_->setFeatures( QDockWidget::DockWidgetClosable );

  searchWidget_->resize(this->size().width(), floor( double(this->size().height() / 3)) );

  searchWidget_->setWidget(searchEngine_->queryWidget());
  //searchWidget_->setWidget(results_);

  searchWidget_->hide();
  addDockWidget(Qt::BottomDockWidgetArea, searchWidget_);


  // Entry in tree view has been clicked
  connect(helpEngine_->contentWidget(), SIGNAL(linkActivated(const QUrl&)),
          this, SLOT(linkActivated(const QUrl&)));

  // Search button
  connect(searchButton_, SIGNAL(clicked()), this, SLOT(showSearchWidget()));

  // Search button
  connect(searchEngine_->queryWidget(), SIGNAL(search()), this, SLOT(startSearch()));

  // Show results if search is finished
  connect(searchEngine_, SIGNAL(searchingFinished(int)), this, SLOT(showResults(int)));

  // Show results if search is finished
  connect(searchEngine_->resultWidget(), SIGNAL(requestShowLink(const QUrl&)),
              this, SLOT(showFoundSite(const QUrl&)));

  // Back button
  connect(backButton_, SIGNAL(clicked()), this, SLOT(goBack()));

  // Forward button
  connect(forwardButton_, SIGNAL(clicked()), this, SLOT(goForward()));

  // Forward button
  connect(homeButton_, SIGNAL(clicked()), this, SLOT(goHome()));

  // Source has been reloaded, so the buttons need an update and the modelview
  connect(textWindow_, SIGNAL(sourceChanged(const QUrl&)), this, SLOT(update(const QUrl&)));

  // the history has changed, so the buttons need an update
  connect(textWindow_, SIGNAL(historyChanged(const QUrl&)), this, SLOT(updateButtons()));

  // Register documentation
  // Seems to be an unneeded call!
  //helpEngine_->registerDocumentation(filename);

  QStringList tmp = helpEngine_->registeredDocumentations ();

  #ifdef DEBUG_HELP_CONTENTS
  for ( int i = 0 ; i < tmp.size(); ++i) {
    std::cerr << "=========================================================================================" << std::endl;
    std::cerr << "Registered namespace: " << tmp[i].toStdString() << std::endl;
    std::cerr << "From file : " << helpEngine_->documentationFileName(tmp[i]).toStdString() << std::endl;


    QList<QStringList> filterAttribs = helpEngine_->filterAttributeSets (tmp[i]);
//    std::cerr << "Filter attributes:" << std::endl;
//    for ( int j = 0 ; j < filterAttribs.size(); ++j) {
//      for ( int k = 0 ; k < filterAttribs[j].size(); ++k) {
//        std::cerr <<  filterAttribs[j][k].toStdString() << std::endl;
//      }
//    }

    // Print a list of all files included in this help file
    if ( !filterAttribs.empty() ) {
      QList<QUrl> list = helpEngine_->files ( tmp[i], filterAttribs[0]);
      for ( int j = 0 ; j < list.size(); ++j) {
        std::cerr << list[j].toString().toStdString() << std::endl;
      }
    } else {
      std::cerr << "Error, empty filter! Unable to get list of included files." << std::endl;
    }


  }
  #endif

  if (_loadHomeSite)
  {
    // Load main page
    textWindow_->open(QUrl(homeSite_));
  }
}

void HelpWidget::activateLink(const QUrl& _url)
{
	//open and show the url
	linkActivated(_url);
}

void HelpWidget::linkActivated(const QUrl& _url) {

  textWindow_->open(_url);
  tabWidget_->setCurrentIndex(homeIndex_);
}

void HelpWidget::startSearch() {

  searchEngine_->search(searchEngine_->queryWidget()->query());
}

void HelpWidget::setHomeSite(const QString& _homeSite) {

  homeSite_ = _homeSite;
}

void HelpWidget::showFoundSite(const QUrl& _url) {

  textWindow_->open(_url);
  tabWidget_->setCurrentIndex(homeIndex_);
}

void HelpWidget::update(const QUrl&  _url ) {

  updateButtons();

  //search for the entry and select the item in the contentWidget
  //in our case, it is the treeView on the left side
  QUrl newUrl = textWindow_->resolveUrl(_url);
  //search for the selected url
  QModelIndex modelIndex = helpEngine_->contentWidget()->indexOf(newUrl);

  //select this url in content widget
  if (modelIndex.isValid())
    helpEngine_->contentWidget()->setCurrentIndex( modelIndex );
}

void HelpWidget::goForward() {

  textWindow_->forward();

  tabWidget_->setCurrentIndex(homeIndex_);

  updateButtons();
}

void HelpWidget::goBack() {

  textWindow_->backward();

  tabWidget_->setCurrentIndex(homeIndex_);

  updateButtons();
}

void HelpWidget::goHome() {

  textWindow_->open(homeSite_);

  tabWidget_->setCurrentIndex(homeIndex_);

  updateButtons();
}

void HelpWidget::updateButtons() {

  if(!textWindow_->isBackwardAvailable()) {
          backButton_->setEnabled(false);
  } else {
          backButton_->setEnabled(true);
  }

  if(!textWindow_->isForwardAvailable()) {
          forwardButton_->setEnabled(false);
  } else {
          forwardButton_->setEnabled(true);
  }
}

void HelpWidget::showSearchWidget() {

  searchWidget_->show();
}

void HelpWidget::showResults(int /*_hits*/) {

  searchWidget_->hide();

  int resultsTabIndex_ = tabWidget_->addTab(searchEngine_->resultWidget(), tr("Results") );
  tabWidget_->setCurrentIndex(resultsTabIndex_);
}

void HelpWidget::openFoundSite(QListWidgetItem* /*_item*/) {

}

HelpWidget::~HelpWidget() {

  delete searchWidget_;
  delete textWindow_;
}
