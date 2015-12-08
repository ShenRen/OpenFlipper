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
*   $Revision: 15910 $                                                       *
*   $LastChangedBy: moeller $                                                *
*   $Date: 2012-12-05 12:53:39 +0100 (Mi, 05 Dez 2012) $                     *
*                                                                            *
\*===========================================================================*/

#include "rendererObjectWidget.hh"

#if QT_VERSION >= 0x050000
  #include <QtWidgets>
#else
  #include <QtGui>
#endif

#include <OpenFlipper/common/GlobalOptions.hh>
#include <OpenFlipper/BasePlugin/PluginFunctions.hh>


RendererObjectWidget::RendererObjectWidget(QWidget *parent)
    : QDialog(parent),
      highlighter_(0),
      textBrowser_(0)
{
  setupUi(this);

  connect(closeButton, SIGNAL(clicked()), this, SLOT(accept()));

  //set icons
  QString iconPath = OpenFlipper::Options::iconDirStr()+OpenFlipper::Options::dirSeparator();

  closeButton->setIcon( QIcon(iconPath + "window-close.png"));

  textBrowser_ = new TextBrowserWidget(this);
  textBrowserLayout->addWidget(textBrowser_);

  highlighter_ = new RenderObjectHighlighter( textBrowser_->document() );

  connect(showShadersBox,SIGNAL(clicked()),this,SLOT(update()));
}

void RendererObjectWidget::closeEvent(QCloseEvent *event)
{
  event->accept();
  accept();
}


void RendererObjectWidget::showEvent ( QShowEvent * ) {
  update();
}

void RendererObjectWidget::update()
{
  RendererInfo* renderer = renderManager().active(PluginFunctions::activeExaminer());

  textBrowser_->clear();

  if ( renderer ) {
    textBrowser_->insertPlainText(tr("Current renderer: ") + renderer->name +"\n");
    textBrowser_->insertPlainText(tr("Description:      ") + renderer->description +"\n");
    textBrowser_->insertPlainText(tr("Version:          ") + renderer->version + "\n" );
    textBrowser_->insertPlainText("\n" );

    //TODO: Flag for shader output activate/deactivate
    if (  renderManager().activeId(PluginFunctions::activeExaminer()) != 0 )
      textBrowser_->insertPlainText(renderer->plugin->renderObjectsInfo(showShadersBox->isChecked()));

  } else {
    textBrowser_->insertPlainText("Unable to get renderer!");
  }
}

