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

#include "SideArea.hh"
#include "SideElement.hh"
//== IMPLEMENTATION ==========================================================

SideArea::SideArea (QWidget *_parent) :
  QWidget (_parent),
  lastPos_(0)
{

  layout_ = new QVBoxLayout;
  layout_->setSpacing (0);

  QVBoxLayout *l = new QVBoxLayout;
  l->addLayout (layout_);
  l->addStretch(1);

  setLayout (l);
}

//-----------------------------------------------------------------------------

void SideArea::addItem (QObject const * const _plugin, QWidget *_w, QString _name, QIcon *_icon)
{
  SideElement *e = new SideElement (this, _w, _name, _icon);
  layout_->addWidget (e);
  items_.push_back (e);
  plugins_.push_back(_plugin);
  itemNames_.push_back(_name);
}

//-----------------------------------------------------------------------------

void SideArea::addItem (QObject const * const _plugin, QWidget *_w, QString _name)
{
  SideElement *e = new SideElement (this, _w, _name);
  layout_->addWidget (e);
  items_.push_back (e);
  plugins_.push_back(_plugin);
  itemNames_.push_back(_name);
}

//-----------------------------------------------------------------------------

void SideArea::moveItemToPosition(const QString& _name, int _position) {

    // Position is in valid range
    if(_position < 0 || _position >= items_.size())
        return;

    // Search item
    QVector<SideElement*>::iterator it = items_.begin();
    for(; it != items_.end(); ++it) {
        if( (*it)->name() == _name )
            break;
    }

    if(it != items_.end()) {
        layout_->removeWidget(*it);
        layout_->insertWidget(_position, (*it));
    }
}

//-----------------------------------------------------------------------------

void SideArea::moveItemToPosition(QObject const * const _plugin, const QString& _name, int _position) {

    // Position is in valid range
    if(_position < 0 || _position >= items_.size())
        return;

    // Search item
    QVector<SideElement*>::iterator it = items_.begin();
    int i = 0;
    for(; it != items_.end(); ++it, ++i) {
        if(   ((*it)->name() == _name)
           && (plugins_[i] == _plugin)   )
            break;
    }

    bool active = (*it)->active();

    if(it != items_.end()) {
        layout_->removeWidget(*it);
        layout_->insertWidget(_position, (*it));
        if (active)
          (*it)->show();
    }
}

//-----------------------------------------------------------------------------

int SideArea::getNumberOfWidgets() const {
    return items_.size();
}

//-----------------------------------------------------------------------------

void SideArea::clear ()
{
  foreach (SideElement *e, items_)
  {
    layout_->removeWidget (e);
    delete e;
  }
  items_.clear ();
  plugins_.clear();
  itemNames_.clear();
  lastPos_ = 0;
}

//-----------------------------------------------------------------------------

void SideArea::expandAll()
{
  foreach (SideElement *e, items_)
  {
    e->setActive(true);
  }
}

void SideArea::expand(QWidget *sideElementWidget, bool expand)
{
  foreach (SideElement *e, items_)
  {
    if (e->widget() == sideElementWidget)
        e->setActive(expand);
  }
}

//-----------------------------------------------------------------------------

void SideArea::saveState (QSettings &_settings)
{
  _settings.beginGroup ("SideArea");
  foreach (SideElement *e, items_)
  {
    e->saveState (_settings);
  }
  _settings.endGroup ();
}

//-----------------------------------------------------------------------------

void SideArea::restoreState (QSettings &_settings)
{
  _settings.beginGroup ("SideArea");
  foreach (SideElement *e, items_)
  {
    e->restoreState (_settings);
  }
  _settings.endGroup ();
}

//-----------------------------------------------------------------------------

void SideArea::saveViewModeState(const QString& _viewMode) {
  foreach (SideElement *e, items_) {
    sideElementState_[_viewMode + e->name()] = e->active();
  }
}

//-----------------------------------------------------------------------------

void SideArea::restoreViewModeState(const QString& _viewMode) {
  foreach (SideElement *e, items_) {
    e->setActive(sideElementState_[_viewMode + e->name()]);
  }
}

//-----------------------------------------------------------------------------

void SideArea::setElementActive(QString _name, bool _active)
{
  for (int i=0; i < items_.count(); i++)
    if ( items_[i]->name() == _name ){
      items_[i]->setActive(_active);

      return;
    }
}

//-----------------------------------------------------------------------------

const QList<const QObject *>& SideArea::plugins() {
  return plugins_;
}

//-----------------------------------------------------------------------------

const QStringList& SideArea::names() {
  return itemNames_;
}


//=============================================================================
//=============================================================================
