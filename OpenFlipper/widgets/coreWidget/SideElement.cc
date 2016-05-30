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



//== INCLUDES =================================================================

#include <QVBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QAction>
#include <QMouseEvent>
#include <QDialog>

#include <OpenFlipper/common/GlobalOptions.hh>

#include "SideElement.hh"

//== IMPLEMENTATION ==========================================================

SideElement::SideElement (SideArea *_parent, QWidget *_w, QString _name, QIcon* _icon) :
  parent_ (_parent),
  widget_ (_w),
  name_ (_name),
  icon_ (_icon),
  active_ (0),
  dialog_ (0)
{
  QFont font;
  font.setBold (false);

  QHBoxLayout *hl = new QHBoxLayout;
  hl->setContentsMargins(0, 0, 0, 0);

  SideElement::TopArea *tra = new SideElement::TopArea (this);

  label_ = new QLabel (_name);
  label_->setFont (font);

  iconHolder_ = new QLabel ();

  if (icon_ != 0)
    iconHolder_->setPixmap( icon_->pixmap(22,22) );
  else{
    QPixmap pic(QSize(22,22));
    pic.fill( QColor(0,0,0,0) );

    iconHolder_->setPixmap( pic );
  }

  detachButton_ = new QToolButton ();
  detachButton_->setAutoRaise(true);
  hl->addWidget (iconHolder_);
  hl->addWidget (label_);
  hl->addStretch (1);
  hl->addWidget (detachButton_);


  QIcon detachIcon(OpenFlipper::Options::iconDirStr()+OpenFlipper::Options::dirSeparator()+"button-detach.png");
  detachIcon.addPixmap(QPixmap(OpenFlipper::Options::iconDirStr()+OpenFlipper::Options::dirSeparator()+"button-detach_over.png"), QIcon::Active);
  detachIcon.addPixmap(QPixmap(OpenFlipper::Options::iconDirStr()+OpenFlipper::Options::dirSeparator()+"button-detach_over.png"), QIcon::Selected);

  detachAction_ = new QAction ( detachIcon, "", this);
  detachAction_->setToolTip( tr("Show as separate window") );
  detachAction_->setCheckable (true);
  detachButton_->setDefaultAction (detachAction_);

  connect (detachAction_, SIGNAL (triggered (bool)), this, SLOT (detachPressed (bool)));

  tra->setLayout (hl);

  QFrame *f = new QFrame ();
  f->setFrameShape (QFrame::HLine);

  mainLayout_ = new QVBoxLayout;

  mainLayout_->addWidget (f);
  mainLayout_->addWidget (tra);
  mainLayout_->addWidget (_w);
  mainLayout_->setSpacing (0);
  mainLayout_->setContentsMargins(0, 0, 0, 0);
  setLayout (mainLayout_);

  _w->hide ();
}

//-----------------------------------------------------------------------------

SideElement::~SideElement ()
{
  if (dialog_) {
    disconnect(dialog_,SIGNAL(finished(int)),this,SLOT(dialogClosed()));
    dialog_->close ();
  }
  widget_->setParent (0);
}

//-----------------------------------------------------------------------------

void SideElement::labelPress ()
{
  if (dialog_)
  {
    dialog_->raise ();
    dialog_->activateWindow ();
  }
  else
  {
    active_ = !active_;
    if (active_)
      widget_->show ();
    else
      widget_->hide ();

    QFont font;
    font.setBold (active_);
    label_->setFont (font);
  }
}

//-----------------------------------------------------------------------------

void SideElement::setActive(bool _active)
{
  if ( dialog_ )
  {
    dialog_->raise ();
    dialog_->activateWindow ();
  }
  else
  {
    active_ = _active;
    if (active_)
      widget_->show ();
    else
      widget_->hide ();

    QFont font;
    font.setBold (active_);
    label_->setFont (font);
  }
}

//-----------------------------------------------------------------------------

void SideElement::detachPressed (bool checked_)
{
  if (checked_)
  {
    mainLayout_->removeWidget (widget_);
    dialog_ = new QDialog(0, Qt::Window);
    dialog_->setWindowTitle (name_);
    dialog_->setWindowIcon(QIcon(OpenFlipper::Options::iconDirStr()+OpenFlipper::Options::dirSeparator()+"button-detach.png"));
    dialog_->setLayout (new QVBoxLayout);
    dialog_->resize (widget_->size ());
    if (window ())
    {
      int x = (window ()->width () - widget_->width ()) / 2;
      x += window ()->x ();
      x = qMax (0, x);
      int y = (window ()->height () - widget_->height ()) / 2;
      y += window ()->y ();
      y = qMax (0, y);
      dialog_->move (x, y);
    }
    dialog_->layout ()->addWidget (widget_);
    dialog_->show ();
    widget_->setAttribute(Qt::WA_DeleteOnClose, false);
    widget_->show ();

    connect (dialog_, SIGNAL(finished (int)), this, SLOT(dialogClosed ()));

    QFont font;
    font.setBold (true);
    font.setItalic (true);
    label_->setFont (font);
  }
  else if (dialog_)
    dialog_->close ();

}

//-----------------------------------------------------------------------------

void SideElement::dialogClosed ()
{
  dialog_ = 0;
  mainLayout_->addWidget (widget_);
  widget_->setAttribute(Qt::WA_DeleteOnClose, true);

  if (active_)
    widget_->show ();
  else
    widget_->hide ();

  detachAction_->setChecked (false);
  QFont font;
  font.setBold (active_);
  label_->setFont (font);
}

//-----------------------------------------------------------------------------

void SideElement::saveState (QSettings &_settings)
{
  _settings.beginGroup (name_);
  _settings.setValue ("Active", active());
  _settings.setValue ("Detached", (dialog_ != 0));
  _settings.setValue ("DialogGeometry", (dialog_) ? dialog_->saveGeometry (): QByteArray());
  _settings.endGroup ();
}

//-----------------------------------------------------------------------------
void SideElement::restoreState (QSettings &_settings)
{
  _settings.beginGroup (name_);

  active_ = _settings.value ("Active", active_).toBool ();

  if (active_)
    widget_->show ();
  else
    widget_->hide ();

  QFont font;
  font.setBold (active_);
  label_->setFont (font);

  if (_settings.value ("Detached", false).toBool () && !dialog_)
    detachPressed (true);

  if (dialog_)
    dialog_->restoreGeometry (_settings.value ("DialogGeometry").toByteArray ());

  _settings.endGroup ();
}

//-----------------------------------------------------------------------------

const QString& SideElement::name(){
  return name_;
}

//-----------------------------------------------------------------------------

bool SideElement::active(){
  return widget_->isVisible();
}

//-----------------------------------------------------------------------------

QWidget const * SideElement::widget() {
  return widget_;
}

//-----------------------------------------------------------------------------

SideElement::TopArea::TopArea (SideElement *_e) :
  e_ (_e)
{
}

//-----------------------------------------------------------------------------

void SideElement::TopArea::mousePressEvent (QMouseEvent *_event)
{
  e_->labelPress ();
  _event->accept ();
}

//=============================================================================
//=============================================================================

