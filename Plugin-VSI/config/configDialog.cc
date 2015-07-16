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
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QScrollArea>
#include <QLineEdit>
#include <QLabel>


#include "scene/elementInput.hh"

#include "configDialog.hh"
#include "configValue.hh"
//== NAMESPACES ===============================================================
namespace VSI {

//=============================================================================
//
//  CLASS VSI::ConfigDialog - IMPLEMENTATION
//
//=============================================================================

/// Constructor
ConfigDialog::ConfigDialog (QVector<ElementInput *> _inputs, QString _name, QWidget * _parent) :
  QDialog (_parent),
  inputs_ (_inputs),
  name_(0)
{
  QVBoxLayout *vL = new QVBoxLayout;
  QVBoxLayout *configL = new QVBoxLayout;

  if (!_name.isNull ())
  {
    QHBoxLayout *nameL = new QHBoxLayout;
    QLabel *l = new QLabel(tr("Element name:"));
    name_ = new QLineEdit;
    name_->setText (_name);
    nameL->addWidget (l);
    nameL->addWidget (name_);
    nameL->setStretchFactor(name_, 1);

    vL->addLayout (nameL);
  }

  foreach (ElementInput *i, _inputs)
  {
    ConfigValue *v = new ConfigValue (i);

    if (!v->main_)
    {
      delete v;
      continue;
    }

    if (_inputs.size () > 1)
      configL->addWidget (v->group_);
    else
      vL->addWidget (v->group_);

    values_.append (v);
  }

  configL->addStretch(1);
  
  QPushButton *ok = new QPushButton ("OK");
  QPushButton *cancel = new QPushButton ("Cancel");

  connect (ok, SIGNAL (pressed ()), this, SLOT (ok ()));
  connect (cancel, SIGNAL (pressed ()), this, SLOT (reject ()));

  QHBoxLayout *buttonL = new QHBoxLayout;
  buttonL->addStretch();
  buttonL->addWidget (ok);
  buttonL->addWidget (cancel);

  if (_inputs.size () > 1)
  {
    QWidget *sW = new QWidget;
    sW->setLayout (configL);
    QScrollArea *sA = new QScrollArea;
    sA->setWidget (sW);
    sA->setWidgetResizable (true);
    vL->addWidget (sA);
    resize (500, 300);
  }
  else
    resize (500, 0);

  vL->addLayout (buttonL);

  setLayout (vL);

  
}

//------------------------------------------------------------------------------

/// Destructor
ConfigDialog::~ ConfigDialog()
{
  foreach (ConfigValue *v, values_)
    delete v;
}

//------------------------------------------------------------------------------

QString ConfigDialog::name ()
{

  if (name_)
    return name_->text ();

  return QString ();

}

//------------------------------------------------------------------------------

// OK pressed
void VSI::ConfigDialog::ok ()
{
  foreach (ConfigValue *v, values_)
  {
    if (!v->constant_ || v->constant_->isChecked ())
      v->input_->set (true);
    else
      v->input_->set (false);

    if (v->forceAskUser_ && v->forceAskUser_->isChecked ())
      v->input_->setForceAsk (true);
    else
      v->input_->setForceAsk (false);
    v->input_->setValue (v->main_->toValue ());
  }
  QDialog::accept ();
}

//------------------------------------------------------------------------------
}
