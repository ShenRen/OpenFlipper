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


#include "../parser/input.hh"

#include "dynamicDialog.hh"
#include "dynamicValue.hh"
//== NAMESPACES ===============================================================
namespace VSI {

//=============================================================================
//
//  CLASS VSI::DynamicDialog - IMPLEMENTATION
//
//=============================================================================

/// Constructor
DynamicDialog::DynamicDialog (QVector<Input *> _inputs, QWidget * _parent) :
  QDialog (_parent),
  inputs_ (_inputs)
{
  QVBoxLayout *configL = new QVBoxLayout;

  foreach (Input *i, _inputs)
  {
    DynamicValue *v = new DynamicValue (i);

    if (!v->main_)
    {
      delete v;
      continue;
    }

    configL->addWidget (v->group_);

    values_.append (v);
  }

  configL->addStretch(1);

  QPushButton *ok = new QPushButton ("OK");

  connect (ok, SIGNAL (pressed ()), this, SLOT (accept ()));

  QHBoxLayout *buttonL = new QHBoxLayout;
  buttonL->addStretch();
  buttonL->addWidget (ok);

  QVBoxLayout *vL = new QVBoxLayout;

  QWidget *sW = new QWidget;
  sW->setLayout (configL);
  QScrollArea *sA = new QScrollArea;
  sA->setWidget (sW);
  sA->setWidgetResizable (true);
  
  vL->addWidget (sA);
  vL->addLayout (buttonL);

  setLayout (vL);

  resize (500, 300);
}

//------------------------------------------------------------------------------

/// Destructor
DynamicDialog::~ DynamicDialog()
{
  foreach (DynamicValue *v, values_)
    if (v)
      delete v;
}

//------------------------------------------------------------------------------

/// Results
QMap<QString, QString> DynamicDialog::getResults()
{
  QMap<QString, QString>  rv;

  foreach (DynamicValue *v, values_)
    rv[v->input_->name ()] = v->main_->toValue ();

  return rv;
}

//------------------------------------------------------------------------------
}
