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
#include <QRadioButton>
#include <QPushButton>

#include "boolWidget.hh"


//== NAMESPACES ===============================================================
namespace VSI {

//=============================================================================
//
//  CLASS VSI::BoolWidget - IMPLEMENTATION
//
//=============================================================================

/// Constructor
BoolWidget::BoolWidget(QMap< QString, QString > &_hints, QString _typeName, QWidget *_parent) :
  TypeWidget (_hints, _typeName, _parent),
  default_ (true),
  true_ (0),
  false_ (0)
{
  QHBoxLayout *hL = new QHBoxLayout;

  true_ = new QRadioButton ("True");
  hL->addWidget (true_);

  false_ = new QRadioButton ("False");
  hL->addWidget (false_);

  if (_hints.contains ("default"))
    default_ = strToBool (_hints["default"]);

  if (default_)
    true_->setChecked (true);
  else
    false_->setChecked (true);

  setLayout (hL);
}

//------------------------------------------------------------------------------

/// Destructor
BoolWidget::~BoolWidget()
{
}

//------------------------------------------------------------------------------

/// Convert current value to string
QString BoolWidget::toValue()
{
  return (true_->isChecked()) ? "true" : "false";
}

//------------------------------------------------------------------------------

/// Read value from string
void BoolWidget::fromValue(QString _from)
{
  bool in = strToBool (_from);
  if (in)
    true_->setChecked (true);
  else
    false_->setChecked (true);
}

//------------------------------------------------------------------------------

/// Reset to default
void BoolWidget::toDefault()
{
  if (default_)
    true_->setChecked (true);
  else
    false_->setChecked (true);
}

//------------------------------------------------------------------------------

// Convert string to bool
bool BoolWidget::strToBool (QString _str)
{
  if (_str == "1" || _str == "true"  || _str == "True" || _str == "TRUE"  ||
      _str == "yes"  || _str == "Yes"  || _str == "YES")
    return true;
  return false;
}

//------------------------------------------------------------------------------
}
