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
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

#include "../parser/context.hh"
#include "selectionWidget.hh"

//== NAMESPACES ===============================================================
namespace VSI {

//=============================================================================
//
//  CLASS VSI::SelectionWidget - IMPLEMENTATION
//
//=============================================================================

/// Constructor
SelectionWidget::SelectionWidget(QMap< QString, QString > &_hints, QString _typeName, QWidget *_parent) :
  TypeWidget (_hints, _typeName, _parent),
  multiple_ (false),
  default_ (),
  combo_ (0)
{
  QHBoxLayout *hL = new QHBoxLayout;

  if (_hints.contains ("names"))
  {
    QStringList n = _hints["names"].split (',');
    QStringList d;

    if (_hints.contains ("descriptions"))
    {
      d = _hints["descriptions"].split (',');
      if (n.length () != d.length ())
        d = n;
    }

    for (int i = 0; i < n.length (); i++)
      names_.append (QPair<QString,QString> (n[i], d[i]));
  }
  else
    names_.append (QPair<QString,QString> ("none", "None"));

  if (_hints.contains ("multiple"))
    multiple_ = Context::strToBool (_hints["multiple"]);

  if (_hints.contains ("default"))
    default_ = _hints["default"].split (',');

  if (!multiple_)
  {
    combo_ = new QComboBox ();
    for (int i  = 0; i < names_.size (); i++)
      combo_->addItem (names_[i].second, names_[i].first);

    if (default_.length ())
      combo_->setCurrentIndex (combo_->findData (default_[0]));
    else
      combo_->setCurrentIndex (0);

    hL->addWidget (combo_);
  }
  else
  {
    QGridLayout *gL = new QGridLayout;
    for (int i = 0; i < names_.size (); i++)
    {
      QCheckBox *c = new QCheckBox (names_[i].second);
      checkBox_.append (c);
      gL->addWidget (c, i / 2, i & 1);

      if (default_.contains (names_[i].first))
        c->setChecked (true);
      else
        c->setChecked (false);
    }
    hL->addLayout (gL);
  }

  setLayout (hL);
}

//------------------------------------------------------------------------------

/// Destructor
SelectionWidget::~ SelectionWidget()
{
}

//------------------------------------------------------------------------------

/// Convert current value to string
QString SelectionWidget::toValue()
{
  QString rv;

  if (!multiple_)
  {
    rv = combo_->itemData (combo_->currentIndex ()).toString ();
  }
  else
  {
    QStringList sl;

    for (int i = 0; i < names_.size (); i++)
    {
      if (checkBox_[i]->isChecked ())
        sl << names_[i].first;
    }

    rv = sl.join (",");
  }

  return "\"" + rv + "\"";
}

//------------------------------------------------------------------------------

/// Read value from string
void SelectionWidget::fromValue(QString _from)
{
  if (_from.isEmpty ())
  {
    if (!multiple_)
      combo_->setCurrentIndex (0);
    else
      foreach (QCheckBox *c, checkBox_)
        c->setChecked (false);
    return;
  }

  _from.remove (0, 1);
  _from.remove (_from.length () - 1, 1);

  QStringList values = _from.split (',');

  if (!multiple_)
  {
    if (values.length ())
      combo_->setCurrentIndex (combo_->findData (values[0]));
    else
      combo_->setCurrentIndex (0);
  }
  else
    for (int i = 0; i < names_.size (); i++)
    {
      if (values.contains (names_[i].first))
        checkBox_[i]->setChecked (true);
      else
        checkBox_[i]->setChecked (false);
    }
}

//------------------------------------------------------------------------------

/// Reset to default
void SelectionWidget::toDefault()
{
  if (!multiple_)
  {
    if (default_.length ())
      combo_->setCurrentIndex (combo_->findData (default_[0]));
    else
      combo_->setCurrentIndex (0);
  }
  else
    for (int i = 0; i < names_.size (); i++)
    {
      if (default_.contains (names_[i].first))
        checkBox_[i]->setChecked (true);
      else
        checkBox_[i]->setChecked (false);
    }

}

//------------------------------------------------------------------------------
}
