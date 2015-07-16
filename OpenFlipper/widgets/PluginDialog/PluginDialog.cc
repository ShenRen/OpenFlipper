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
#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/common/GlobalOptions.hh>

#include "PluginDialog.hh"

#include <QMessageBox>

const QColor PluginDialog::blockColor_  = QColor(228, 155, 18);
const QColor PluginDialog::unloadColor_ = QColor(172, 172, 172);
const QColor PluginDialog::loadedBuiltInColor_ = QColor(208, 240, 192);
const QColor PluginDialog::failColor_ = Qt::red;
const QColor PluginDialog::loadedExternalColor_ = QColor(152, 255, 152);

PluginDialog::PluginDialog(std::vector<PluginInfo>& _plugins, QWidget *parent)
    : QDialog(parent),
      plugins_(_plugins)
{
  setupUi(this);

  list->setContextMenuPolicy(Qt::CustomContextMenu);

  connect(closeButton, SIGNAL(clicked()), this, SLOT(accept()));
  connect(loadButton, SIGNAL(clicked()), this, SIGNAL(loadPlugin()));
  connect(loadButton, SIGNAL(clicked()), this, SLOT(reject()));
  connect(list,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(slotContextMenu(const QPoint&)));

  //set icons
  QString iconPath = OpenFlipper::Options::iconDirStr()+OpenFlipper::Options::dirSeparator();

  closeButton->setIcon( QIcon(iconPath + "window-close.png"));
  loadButton->setIcon( QIcon(iconPath + "network-connect.png"));

}

void PluginDialog::closeEvent(QCloseEvent *event)
{
  event->accept();
  accept();
}

int PluginDialog::exec()
{

  for (uint i = 0; i < plugins_.size(); i++){
    QFrame* frame = new QFrame();
    QHBoxLayout* hlayout = new QHBoxLayout;
    QLabel* name = new QLabel( plugins_[i].name );
    QFont font;
    font.setBold(true);
    font.setPointSize(10);
    name->setFont(font);
    QLabel* version = new QLabel( plugins_[i].version );
//     QLabel* author = new QLabel( "RWTH Computer Graphics Group" );

    hlayout->addWidget(name);
    hlayout->addStretch();
    hlayout->addWidget(version);
//     hlayout->addSpacing(10);
//     hlayout->addWidget(author);

    QVBoxLayout* vlayout = new QVBoxLayout;

    QLabel* description = new QLabel( plugins_[i].description );
    descriptions_.push_back(description);

    vlayout->addLayout(hlayout,20);
    vlayout->addWidget(description);
    frame->setLayout(vlayout);

    QListWidgetItem *item = new QListWidgetItem("");
    frames_.push_back(frame);
    item->setSizeHint( QSize (100,50) );

    //set color depending on the current status
    switch(plugins_[i].status)
    {
      case PluginInfo::LOADED:
        if (plugins_[i].buildIn)
          item->setBackground(loadedBuiltInColor_);
        else
        {
          item->setBackground(loadedExternalColor_);
          description->setText(description->text()+tr(" *EXTERNAL*"));
        }
        break;
      case PluginInfo::FAILED:
        item->setBackground(failColor_);
        description->setText(description->text()+tr(" *FAILED*"));
        break;
      case PluginInfo::BLOCKED:
        item->setBackground(blockColor_);
        description->setText(description->text()+tr(" *BLOCKED*"));
        break;
      case PluginInfo::UNLOADED:
        item->setBackground(unloadColor_);
        description->setText(description->text()+tr(" *UNLOADED*"));
        break;
    }
    list->addItem(item);
    list->setItemWidget(item, frame);
  }

  int ret = QDialog::exec();

  for (int i=0; i < frames_.count(); i++)
    delete frames_[i];

  return ret;
}

void PluginDialog::slotBlockPlugin()
{
  for (int i=0; i < list->selectedItems().size(); ++i)
  {
    QListWidgetItem* widget = list->selectedItems()[i];
    widget->setBackground(blockColor_);

    PluginInfo* plugin = &plugins_[ list->row( widget ) ];
    descriptions_[list->row( widget )]->setText(plugin->description + tr(" *BLOCKED*"));

    emit blockPlugin(plugin->name);
  }
}
void PluginDialog::slotUnBlockPlugin()
{
  for (int i=0; i < list->selectedItems().size(); ++i)
  {
    QListWidgetItem* widget = list->selectedItems()[i];
    widget->setBackground(unloadColor_);

    PluginInfo* plugin = &plugins_[ list->row( widget ) ];
    descriptions_[list->row( widget )]->setText(plugin->description);

    emit unBlockPlugin(plugin->name);
  }
}

void PluginDialog::slotLoadPlugin()
{
  for (int i=0; i < list->selectedItems().size(); ++i)
  {
    QListWidgetItem* widget = list->selectedItems()[i];

    PluginInfo* plugin = &plugins_[ list->row( widget ) ];

    if (plugin->status == PluginInfo::BLOCKED)
    {
      QMessageBox msgBox;
      msgBox.setText("Plugin is blocked. Unblock it?");
      msgBox.setWindowTitle("Plugin blocked");
      msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
      msgBox.setDefaultButton(QMessageBox::Yes);
      int rep = msgBox.exec();
      if ( rep == QMessageBox::Yes)
        emit unBlockPlugin(plugin->name);
    }

    if (plugin->buildIn)
      widget->setBackground(loadedBuiltInColor_);
    else
      widget->setBackground(loadedExternalColor_);

    descriptions_[list->row( widget )]->setText(plugin->description);

    QString licenseErros;
    emit loadPlugin(plugin->path,false,licenseErros,plugin->plugin);

    if (plugin->status == PluginInfo::FAILED)
    {
      descriptions_[list->row( widget )]->setText(plugin->description + tr(" *FAILED*"));
      widget->setBackground(failColor_);
    }else
    {
      plugin->status = PluginInfo::LOADED;
    }
  }
}

void PluginDialog::slotContextMenu(const QPoint& _point)
{
  if (!list->count())
    return;

  QMenu *menu = new QMenu(list);
  QAction* action = 0;

  PluginInfo* plugin = &plugins_[list->currentRow()];

  if ( plugin->status != PluginInfo::BLOCKED)//not blocked
  {
    action = menu->addAction(tr("Block Plugin"));
    connect(action,SIGNAL(triggered(bool)),this,SLOT(slotBlockPlugin()));
  }else//blocked
  {
    action = menu->addAction(tr("Unblock Plugin"));
    connect(action,SIGNAL(triggered(bool)),this,SLOT(slotUnBlockPlugin()));
  }

  if ( plugin->status != PluginInfo::LOADED)
  {
    action = menu->addAction(tr("Load Plugin"));
    connect(action,SIGNAL(triggered(bool)),this,SLOT(slotLoadPlugin()));
  }

  menu->exec(list->mapToGlobal(_point),0);

}
