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

#include <OpenFlipper/BasePlugin/PluginFunctions.hh>
#include <OpenFlipper/common/RendererInfo.hh>
#include <OpenFlipper/common/GlobalOptions.hh>

#if QT_VERSION >= 0x050000
  #include <QtWidgets>
#else
  #include <QtGui>
#endif

#include "postProcessorWidget.hh"


PostProcessorDialog::PostProcessorDialog(QWidget *_parent)
    : QDialog(_parent)
{
  setupUi(this);

  list->setContextMenuPolicy(Qt::CustomContextMenu);
  activeList->setContextMenuPolicy(Qt::CustomContextMenu);

  connect(closeButton, SIGNAL(clicked()), this, SLOT(accept()));
  connect(list,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(slotContextMenuActivate(const QPoint&)));
  connect(activeList,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(slotContextMenuDeactivate(const QPoint&)));
  connect(activateButton,SIGNAL(clicked()),this,SLOT(slotActivatePostProcessor()));
  connect(deactivateButton,SIGNAL(clicked()),this,SLOT(slotDeactivatePostProcessor()));
  connect(upButton,SIGNAL(clicked()),this,SLOT(slotMoveUp()));
  connect(downButton,SIGNAL(clicked()),this,SLOT(slotMoveDown()));
  connect(saveButton,SIGNAL(clicked()),this,SLOT(slotSaveActive()));
  connect(refreshButton,SIGNAL(clicked()), this,SLOT(refresh()));

  //set icons
  QString iconPath = OpenFlipper::Options::iconDirStr()+OpenFlipper::Options::dirSeparator();

  closeButton->setIcon( QIcon(iconPath + "window-close.png"));
  saveButton->setIcon( QIcon(iconPath + "document-save.png"));
  refreshButton->setIcon( QIcon(iconPath + "edit-redo.png"));

}

void PostProcessorDialog::closeEvent(QCloseEvent *_event)
{
  _event->accept();
  accept();
}

void PostProcessorDialog::showEvent ( QShowEvent * )
{
  initWindow();
}

void PostProcessorDialog::initWindow()
{
  currentExaminer_ = PluginFunctions::activeExaminer();
  list->clear();
  activeList->clear();
  activeRowToRow_.clear();

  //first, fill already activated processors in the right order
  for (int i = 0; i < postProcessorManager().numActive(currentExaminer_); ++i)
  {
    unsigned int id = postProcessorManager().activeId(currentExaminer_, i);
    activeRowToRow_.push_back(id);

    QListWidgetItem *activeItem = new QListWidgetItem("");
    activeList->addItem(activeItem);
    QFrame* frame = createFrame(*postProcessorManager()[id]);
    activeItem->setSizeHint(frame->sizeHint());
    activeList->setItemWidget(activeItem,frame);
  }

  //list all available post processors (hidden, if active)
  for ( unsigned int i = 0 ; i < postProcessorManager().available() ; ++i)
  {

    // Get and check post processor
    PostProcessorInfo* processor = postProcessorManager()[i];
    if ( ! processor )
      continue;

    QFrame* frame = createFrame(*processor);

    QListWidgetItem *item = new QListWidgetItem("");
    item->setSizeHint( frame->sizeHint() );

    list->addItem(item);

    list->setItemWidget(item, frame);

    //is the postProcess active? if so, hide it
    bool found = false;
    for (std::vector<unsigned>::iterator iter = activeRowToRow_.begin(); iter != activeRowToRow_.end() && !found; ++iter)
      found = (*iter == i);
    if ( found )
      list->setRowHidden(list->row(item),true);
  }

}

void PostProcessorDialog::slotActivatePostProcessor()
{
  QList<QListWidgetItem*> selectedItems = list->selectedItems();

  for (int i=0; i < selectedItems.size(); ++i)
  {
    QListWidgetItem* item = selectedItems[i];
    const int currentRow = list->row( item );

    postProcessorManager().append( currentRow, currentExaminer_);

    //disable in aviable list
    item->setHidden(true);
    item->setSelected(false);

    //add to active list
    QListWidgetItem *activeItem = new QListWidgetItem("");
    activeList->addItem(activeItem);
    activeItem->setSelected(true);

    QFrame* frame = createFrame(*postProcessorManager()[currentRow]);
    activeItem->setSizeHint( frame->sizeHint() );
    activeList->setItemWidget(activeItem,frame);
    activeRowToRow_.push_back(currentRow);
  }

  emit updateExaminer(currentExaminer_);

}

void PostProcessorDialog::slotDeactivatePostProcessor()
{
  QList<QListWidgetItem*> selectedItems = activeList->selectedItems();

  for (int i=0; i < selectedItems.size(); ++i)
  {
    QListWidgetItem* activeItem = selectedItems[i];

    const unsigned chainPos = activeList->row(activeItem);
    const unsigned activeID = activeRowToRow_[chainPos];
    QListWidgetItem* item = list->item(activeID);

    //remove postprocessor
    postProcessorManager().remove(currentExaminer_, chainPos);

    //enable in aviable list
    item->setHidden(false);
    item->setSelected(true);

    //remove from active list
    //update active row ids
    for (unsigned i = chainPos; i < activeRowToRow_.size()-1; ++i)
      activeRowToRow_[i] = activeRowToRow_[i+1];

    //from qt doc: Items removed from a list widget will not be managed by Qt, and will need to be deleted manually.
    activeItem = activeList->takeItem(activeList->row(activeItem));
    delete activeItem;
  }
  activeRowToRow_.erase( activeRowToRow_.end()-selectedItems.size(), activeRowToRow_.end());

  emit updateExaminer(currentExaminer_);
}

void PostProcessorDialog::slotMovePostProcessor(unsigned _from,unsigned _to)
{

  if (_from >= static_cast<unsigned>(activeList->count()))
    return;

  if (_to >= static_cast<unsigned>(activeList->count()))
    _to = activeList->count()-1;

  if (_from == _to)
    return;

  //swap widget
  QListWidgetItem* activeItem = activeList->takeItem(_from);
  activeList->insertItem(_to,activeItem);
  QFrame* frame = createFrame(*postProcessorManager()[activeRowToRow_[_from]]);
  activeItem->setSizeHint(frame->sizeHint());
  activeList->setItemWidget(activeItem,frame);
  activeList->setItemSelected(activeItem,true);

  //swap postprocessor
  const int chainPos = _from;
  const int activeID = activeRowToRow_[_from];
  postProcessorManager().remove(currentExaminer_, chainPos);
  postProcessorManager().insert(activeID,_to,currentExaminer_);

  //swap active ID to current chain position map
  int inc = (_from > _to)? -1: +1;
  for(unsigned int currentRow = _from;currentRow != _to; currentRow += inc)
    std::swap(activeRowToRow_[currentRow+inc],activeRowToRow_[currentRow]);

  emit updateExaminer(currentExaminer_);
}


void PostProcessorDialog::slotContextMenuActivate(const QPoint& _point)
{
  if (!list->count())
    return;

  QMenu *menu = new QMenu(list);
  QAction* action = 0;

  action = menu->addAction(tr("Activate"));
  connect(action,SIGNAL(triggered(bool)),this,SLOT(slotActivatePostProcessor()));

  menu->exec(list->mapToGlobal(_point),0);

}

void PostProcessorDialog::slotContextMenuDeactivate(const QPoint& _point)
{
  if (!activeList->count())
    return;

  QMenu *menu = new QMenu(activeList);
  QAction* action = 0;

  action = menu->addAction(tr("Up"));
  connect(action,SIGNAL(triggered(bool)),this,SLOT(slotMoveUp()));
  action = menu->addAction(tr("Down"));
  connect(action,SIGNAL(triggered(bool)),this,SLOT(slotMoveDown()));
  action = menu->addAction(tr("Deactivate"));
  connect(action,SIGNAL(triggered(bool)),this,SLOT(slotDeactivatePostProcessor()));

  menu->exec(activeList->mapToGlobal(_point),0);

}

QFrame* PostProcessorDialog::createFrame(const PostProcessorInfo& _pPI)
{
  QFrame* frame = new QFrame();
  QHBoxLayout* hlayout = new QHBoxLayout;

  QLabel* name = new QLabel( _pPI.name );
  QFont font;
  font.setBold(true);
  font.setPointSize(10);
  name->setFont(font);
  QLabel* version = new QLabel( _pPI.version );
  QPushButton* optionsButton = new QPushButton("Options");
  hlayout->addWidget(name);
  hlayout->addStretch();
  hlayout->addWidget(version);

  optionsButton->setEnabled(false);
  if (_pPI.optionsAction != 0)
  {
    optionsButton->setEnabled(true);
    connect(optionsButton,SIGNAL(clicked()),_pPI.optionsAction,SLOT(trigger()));
  }

  QVBoxLayout* vlayout = new QVBoxLayout;

  QLabel* description = new QLabel( _pPI.description );

  vlayout->addLayout(hlayout,20);

  QHBoxLayout* optionsLayout = new QHBoxLayout();
  vlayout->addLayout(optionsLayout);
  optionsLayout->addWidget(description);
  optionsLayout->addStretch();
  optionsLayout->addWidget(optionsButton);

  frame->setLayout(vlayout);
  frame->adjustSize();

  return frame;
}



template<typename TCmp>
class QListWidgetRowCmp
{
  QListWidget* list_;
public:
  QListWidgetRowCmp(QListWidget* _list):list_(_list){}
  bool operator()(QListWidgetItem* left, QListWidgetItem* right)
  {
    return TCmp()(list_->row(left) , list_->row(right));
  }
};

void PostProcessorDialog::slotMoveDown()
{
  int start = 0;
  QList<QListWidgetItem*> selectedItems = activeList->selectedItems();

  //sort list, so the top is the last element
  std::sort(selectedItems.begin(), selectedItems.end(), QListWidgetRowCmp<std::greater<int> >(activeList));

  //dont move the last one
  //if the last one wasnt moved, dont move the direct followers
  for(int i=0; i < selectedItems.size() && activeList->row(selectedItems[i]) == activeList->count()-1-i;++i)
    --start;

  //move bottom first
  for (int i=selectedItems.size()-1+start; i >= 0 ; --i)
  {
    QListWidgetItem* activeItem = activeList->selectedItems()[i];
    unsigned selectedRow = activeList->row(activeItem);
    slotMovePostProcessor(selectedRow,selectedRow+1);
  }
}


void PostProcessorDialog::slotMoveUp()
{
  int start = 0;
  QList<QListWidgetItem*> selectedItems = activeList->selectedItems();

  //sort list, so the top is the first element
  std::sort(selectedItems.begin(), selectedItems.end(), QListWidgetRowCmp<std::less<int> >(activeList));

  //dont move the first one
  //if the first one wasnt moved, dont move the direct followers
  for(int i=0; i < selectedItems.size() && activeList->row(selectedItems[i]) == i;++i)
    ++start;

  //move top first
  for (int i=start; i < selectedItems.size(); ++i)
  {
    QListWidgetItem* activeItem = selectedItems[i];
    unsigned selectedRow = activeList->row(activeItem);
    slotMovePostProcessor(selectedRow,selectedRow-1);
  }
}

void PostProcessorDialog::slotSaveActive()
{
  QStringList activeList("");

  for (int i = 0; i < postProcessorManager().numActive(currentExaminer_); ++i)
  {
    unsigned int id = postProcessorManager().activeId(currentExaminer_, i);
    activeList.push_back(postProcessorManager()[id]->name);
  }

  OpenFlipperSettings().setValue(QString("PostProcessor/Viewer/%1").arg(currentExaminer_),activeList);
}

QStringList PostProcessorDialog::getSavedPostProcessorNames(const unsigned _examiner)
{
  return OpenFlipperSettings().value(QString("PostProcessor/Viewer/%1").arg(_examiner),QStringList("")).toStringList();
}

void PostProcessorDialog::loadSavedPostProcessors(const unsigned _examiner)
{
  QStringList active = getSavedPostProcessorNames(_examiner);
  for (QStringList::iterator iter = active.begin(); iter != active.end(); ++iter)
  {
    postProcessorManager().append(*iter,_examiner);
  }
}

void PostProcessorDialog::refresh()
{
  initWindow();
}
