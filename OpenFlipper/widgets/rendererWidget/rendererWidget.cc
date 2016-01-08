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

#include "rendererWidget.hh"

#if QT_VERSION >= 0x050000 
  #include <QtWidgets>
#else
  #include <QtGui>
#endif

#include <OpenFlipper/common/GlobalOptions.hh>
#include <OpenFlipper/BasePlugin/PluginFunctions.hh>


RendererDialog::RendererDialog(QWidget *parent)
    : QDialog(parent)
{
  setupUi(this);

  list->setContextMenuPolicy(Qt::CustomContextMenu);

  connect(closeButton, SIGNAL(clicked()), this, SLOT(accept()));
  connect(list,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(slotContextMenu(const QPoint&)));

  //set icons
  QString iconPath = OpenFlipper::Options::iconDirStr()+OpenFlipper::Options::dirSeparator();

  closeButton->setIcon( QIcon(iconPath + "window-close.png"));

}

void RendererDialog::closeEvent(QCloseEvent *event)
{
  event->accept();
  accept();
}


void RendererDialog::showEvent ( QShowEvent * ) {
  update();
}

void RendererDialog::update()
{

  list->clear();

  for ( unsigned int i = 0 ; i < renderManager().available() ; ++i) {

    // Get and check post processor
    RendererInfo* renderer = renderManager()[i];
    if ( ! renderer )
      continue;

    QFrame* frame = new QFrame();
    QHBoxLayout* hlayout = new QHBoxLayout;

    QLabel* name = new QLabel( renderer->name );
    QFont font;
    font.setBold(true);
    font.setPointSize(10);
    name->setFont(font);
    QLabel* version = new QLabel( renderer->version );
    hlayout->addWidget(name);
    hlayout->addStretch();
    hlayout->addWidget(version);

    QVBoxLayout* vlayout = new QVBoxLayout;

    QLabel* description = new QLabel( renderer->description );
    descriptions_.push_back(description);

    vlayout->addLayout(hlayout,20);
    vlayout->addWidget(description);
    frame->setLayout(vlayout);

    QListWidgetItem *item = new QListWidgetItem("");
    frames_.push_back(frame);
    item->setSizeHint( QSize (100,50) );

    list->addItem(item);
    list->setItemWidget(item, frame);

    if ( renderManager().activeId(PluginFunctions::activeExaminer()) == i )
      item->setBackground( Qt::green );
  }

}

void RendererDialog::slotActivateRenderer() {
  for (int i=0; i < list->selectedItems().size(); ++i)
  {
    QListWidgetItem* widget = list->selectedItems()[i];

    renderManager().setActive( list->row( widget ), PluginFunctions::activeExaminer());
  }

  update();
}


void RendererDialog::slotContextMenu(const QPoint& _point)
{
  if (!list->count())
    return;

  QMenu *menu = new QMenu(list);
  QAction* action = 0;

  action = menu->addAction(tr("Activate"));
  connect(action,SIGNAL(triggered(bool)),this,SLOT(slotActivateRenderer()));


  menu->exec(list->mapToGlobal(_point),0);

}
