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
#include <cmath>

#include <QHBoxLayout>
#include <QSlider>
#include <QPushButton>
#include <QDoubleSpinBox>

#include "numWidget.hh"

//== NAMESPACES ===============================================================
namespace VSI {

//=============================================================================
//
//  CLASS VSI::NumWidget - IMPLEMENTATION
//
//=============================================================================

/// Constructor
NumWidget::NumWidget(QMap< QString, QString > &_hints, QString _typeName, QWidget *_parent) :
  TypeWidget (_hints, _typeName, _parent),
  precision_ (1.0),
  isFloat_ (false),
  numDecimals_ (0),
  sliderMul_ (1.0),
  slider_ (0),
  spin_ (0)
{
  bool ok;

  QHBoxLayout *hL = new QHBoxLayout;

  if (_hints.contains ("precision"))
  {
    double v = _hints["precision"].toDouble (&ok);
    if (ok)
      precision_ = v;
  }

  

  while (precision_ * sliderMul_ != floor(precision_ * sliderMul_) && numDecimals_ <= 10)
  {
    sliderMul_ *= 10.0;
    numDecimals_++;
  }

  if (_typeName == "Float")
    isFloat_ = true;
  else if (_typeName == "Integer")
    isFloat_ = false;
  else if (precision_ != int(precision_))
    isFloat_ = true;
    

  if (_hints.contains ("min") && _hints.contains ("max"))
  {
    double min = _hints["min"].toDouble (&ok);
    if (!ok)
      min = -65535;
    if (!isFloat_)
      min = int (min);

    double max = _hints["max"].toDouble (&ok);
    if (!ok)
      max = 65535;
    if (!isFloat_)
      max = int (max);

    if (min > max)
      max = min + precision_;

    max *= sliderMul_;
    min *= sliderMul_;

    slider_ = new QSlider;
    slider_->setRange (min, max);
    slider_->setSingleStep (precision_ * sliderMul_);
    slider_->setOrientation (Qt::Horizontal);

    connect (slider_, SIGNAL (valueChanged(int)), this, SLOT (sliderValueChanged(int)));

    hL->addWidget(slider_);
  }
  else
  {
    hL->addStretch ();
  }

  spin_ = new QDoubleSpinBox;

  if (_hints.contains ("min") && _hints.contains ("max"))
  {
    double min = _hints["min"].toDouble (&ok);
    if (!ok)
      min = -65535;
    if (!isFloat_)
      min = int (min);

    double max = _hints["max"].toDouble (&ok);
    if (!ok)
      max = 65535;
    if (!isFloat_)
      max = int (max);

    if (min > max)
      max = min + precision_;

    spin_->setRange (min, max);
  }
  else
    spin_->setRange (-1000000, 1000000);

  spin_->setSingleStep (precision_);
  spin_->setDecimals (numDecimals_);
  connect (spin_, SIGNAL (valueChanged(double)), this, SLOT (spinValueChanged(double)));

  ok = false;
  if (_hints.contains ("default"))
    default_ = _hints["default"].toFloat (&ok);

  if (!ok)
    default_ = (spin_->minimum () + spin_->maximum ()) / 2;

  spin_->setValue (default_);

  hL->addWidget (spin_);

  setLayout (hL);
}

/// Destructor
NumWidget::~ NumWidget()
{
}

//------------------------------------------------------------------------------

/// Convert current value to string
QString NumWidget::toValue()
{
  return QString::number (spin_->value ());
}

//------------------------------------------------------------------------------

/// Read value from string
void NumWidget::fromValue(QString _from)
{
  spin_->setValue (_from.toDouble());
}

//------------------------------------------------------------------------------

// handle slider changes
void VSI::NumWidget::sliderValueChanged(int _value)
{
  double v = _value / sliderMul_;

  if (v != spin_->value ())
    spin_->setValue (v);
}

//------------------------------------------------------------------------------

// handle spinbox changes
void VSI::NumWidget::spinValueChanged(double _value)
{
  double v = _value * sliderMul_;

  if (slider_ && v != slider_->value ())
    slider_->setValue (v);
}

//------------------------------------------------------------------------------

/// Reset to default
void VSI::NumWidget::toDefault()
{
  spin_->setValue (default_);
}

//------------------------------------------------------------------------------
}

