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
#include <QPushButton>
#include <QComboBox>

#include <OpenFlipper/BasePlugin/PluginFunctions.hh>

#include "objectSelectionWidget.hh"
#include "objectPickDialog.hh"

//== NAMESPACES ===============================================================

//=============================================================================
//
//  CLASS ObjectSelectionWidget - IMPLEMENTATION
//
//=============================================================================

/// Constructor
ObjectSelectionWidget::ObjectSelectionWidget(QMap< QString, QString > &_hints, QString /*_typeName*/, QWidget * /*_parent*/) :
  QWidget(0),
  pickButton_ (0),
  combo_ (0)
{
  int n_ok = 0;

  QHBoxLayout *hL = new QHBoxLayout;

  combo_ = new QComboBox ();
  pickButton_ = new QPushButton (tr("Pick Object"));

  hL->addWidget (combo_);
  hL->setStretchFactor (combo_, 1);
  hL->addWidget (pickButton_);

  if (_hints.contains ("flags"))
    flags_ = _hints["flags"].split(',');

  if (_hints.contains ("types"))
    types_ = _hints["types"].split(',');

  if (flags_.contains("all", Qt::CaseInsensitive))
    flags_.clear ();

  withGroups_ = types_.contains("Group");

  if (types_.contains("All"))
    types_.clear ();

  setLayout (hL);

  connect (pickButton_, SIGNAL (pressed()), this, SLOT (showPickDialog ()));

  for (PluginFunctions::BaseObjectIterator o_it; o_it != PluginFunctions::baseObjectsEnd(); ++o_it)  {

    bool ok = true;

    if (!flags_.empty ())
    {
      bool found = false;
      foreach (QString flag, flags_)
        if (o_it->flag (flag))
        {
          found = true;
          break;
        }

      if (!found)
        ok = false;
    }

    if (!types_.empty ())
    {
      if (!types_.contains (typeName (o_it->dataType())))
        ok = false;
    }

    if (o_it->isGroup() && !withGroups_)
      continue;

    if (ok)
    {
      combo_->addItem (o_it->name() + " (" + QString::number (o_it->id ()) + ")", QVariant (o_it->id()));
      n_ok++;
    }
  }

  if (n_ok < 2)
  {
    pickButton_->setEnabled (false);
    combo_->setEnabled (false);
  }
}

/// Destructor
ObjectSelectionWidget::~ ObjectSelectionWidget()
{
  for (PluginFunctions::BaseObjectIterator o_it; o_it != PluginFunctions::baseObjectsEnd(); ++o_it)  {
    o_it->setFlag("vsi_objectId_selected", false);
  }
}

//------------------------------------------------------------------------------

void ObjectSelectionWidget::showPickDialog()
{
  ObjectPickDialog d(flags_, types_, withGroups_);
  d.selectedId (combo_->itemData (combo_->currentIndex()).toInt ());

  if (d.exec () == QDialog::Accepted)
    combo_->setCurrentIndex (combo_->findData (QVariant (d.selectedId())));
}

//------------------------------------------------------------------------------


