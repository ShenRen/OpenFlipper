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
*   $Revision: 10745 $                                                       *
*   $LastChangedBy: moebius $                                                *
*   $Date: 2011-01-26 10:23:50 +0100 (Mi, 26. Jan 2011) $                     *
*                                                                            *
\*===========================================================================*/

//== INCLUDES =================================================================
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>

#include "vec4dWidget.hh"

//== NAMESPACES ===============================================================
namespace VSI {

//=============================================================================
//
//  CLASS VSI::Vec3DWidget - IMPLEMENTATION
//
//=============================================================================

/// Constructor
Vec4DWidget::Vec4DWidget(QMap< QString, QString > &_hints, QString _typeName, QWidget *_parent) :
  TypeWidget (_hints, _typeName, _parent)
{
  bool ok;

  QHBoxLayout *hL = new QHBoxLayout;

  for (int i = 0; i < 4; i++)
    default_[i] = 0.0;

  ok = true;
  if (_hints.contains ("default"))
  {
    QStringList sl = _hints["default"].split (',');

    if (sl.length () == 4)
    {
      for (int i = 0; i < 4 && ok; i++)
        default_[i] = sl[i].toFloat (&ok);

      if (!ok)
        for (int i = 0; i < 4; i++)
          default_[i] = 0.0;
    }
  }

  for (int i = 0; i < 4; i++)
    fields_[i] = new QLineEdit;

  hL->addWidget (new QLabel ("("));
  hL->addWidget (fields_[0]);
  hL->addWidget (new QLabel (","));
  hL->addWidget (fields_[1]);
  hL->addWidget (new QLabel (","));
  hL->addWidget (fields_[2]);
  hL->addWidget (new QLabel (","));
  hL->addWidget (fields_[3]);
  hL->addWidget (new QLabel (")"));

  for (int i = 0; i < 4; i++)
  {
    fields_[i]->setText (QString::number (default_[i]));
    connect (fields_[i], SIGNAL (editingFinished ()), this, SLOT (editingFinished ()));
  }

  setLayout (hL);

  for (int i = 0; i < 4; i++)
    current_[i] = default_[i];
}

/// Destructor
Vec4DWidget::~ Vec4DWidget()
{
}

//------------------------------------------------------------------------------

/// Convert current value to string
QString Vec4DWidget::toValue()
{
  QString rv = "Vector4 (";
  rv += QString::number (current_[0]) + ",";
  rv += QString::number (current_[1]) + ",";
  rv += QString::number (current_[2]) + ",";
  rv += QString::number (current_[3]) + ")";
  return rv;
}

//------------------------------------------------------------------------------

/// Read value from string
void Vec4DWidget::fromValue(QString _from)
{
  if (_from.startsWith ("Vector4 ("))
    _from.remove (0, 8);
  if (_from.endsWith (")"))
    _from.remove (_from.length () - 1, 1);
  
  QStringList sl = _from.split (',');

  float v[4];
  bool ok = true;

  if (sl.length () == 4)
  {
    for (int i = 0; i < 4 && ok; i++)
      v[i] = sl[i].toFloat (&ok);

    if (ok)
      for (int i = 0; i < 4; i++)
      {
        current_[i] = v[i];
        fields_[i]->setText (QString::number (current_[i]));
      }
  }
}

//------------------------------------------------------------------------------

// handle slider changes
void Vec4DWidget::editingFinished ()
{
  bool ok;

  for (int i = 0; i < 4; i++)
  {
    const float v = fields_[i]->text ().toFloat (&ok);

    if (ok)
      current_[i] = v;
    else
      fields_[i]->setText (QString::number (current_[i]));
  }
}



//------------------------------------------------------------------------------

/// Reset to default
void Vec4DWidget::toDefault()
{
  for (int i = 0; i < 4; i++)
  {
    current_[i] = default_[i];
    fields_[i]->setText (QString::number (current_[i]));
  }
}

//------------------------------------------------------------------------------
}

