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
#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>

#include "matrix4x4Widget.hh"

//== NAMESPACES ===============================================================
namespace VSI {

//=============================================================================
//
//  CLASS VSI::Matrix4x4Widget - IMPLEMENTATION
//
//=============================================================================

/// Constructor
Matrix4x4Widget::Matrix4x4Widget(QMap< QString, QString > &_hints, QString _typeName, QWidget *_parent) :
  TypeWidget (_hints, _typeName, _parent)
{
  bool ok;

  QGridLayout *gL = new QGridLayout;

  for (int i = 0; i < 16; i++)
  {
    default_[i] = 0.0;
    if (i%5 == 0) //set diagonal to 1, convenient for e.g. transformation matrices
      default_[i]  = 1.0;
  }

  ok = true;
  if (_hints.contains ("default"))
  {
    QStringList sl = _hints["default"].split (',');

    if (sl.length () == 16)
    {
      for (int i = 0; i < 16 && ok; i++)
        default_[i] = sl[i].toFloat (&ok);

      if (!ok)
        for (int i = 0; i < 16; i++)
          default_[i] = 0.0;
    }
  }

  for (int i = 0; i < 16; i++)
    fields_[i] = new QLineEdit;

  for (int i = 0; i < 4; i++)
  {
    gL->addWidget (new QLabel ("("),i, 0);
    gL->addWidget (fields_[i], i, 1);
    gL->addWidget (new QLabel (","), i, 2);
    gL->addWidget (fields_[i + 4], i, 3);
    gL->addWidget (new QLabel (","), i, 4);
    gL->addWidget (fields_[i + 8], i, 5);
    gL->addWidget (new QLabel (","), i, 6);
    gL->addWidget (fields_[i + 12], i, 7);
    gL->addWidget (new QLabel (")"), i, 8);
  }

  for (int i = 0; i < 16; i++)
  {
    fields_[i]->setText (QString::number (default_[i]));
    connect (fields_[i], SIGNAL (editingFinished ()), this, SLOT (editingFinished ()));
  }

  setLayout (gL);

  for (int i = 0; i < 16; i++)
    current_[i] = default_[i];
}

/// Destructor
Matrix4x4Widget::~ Matrix4x4Widget()
{
}

//------------------------------------------------------------------------------

/// Convert current value to string
QString Matrix4x4Widget::toValue()
{
  QString rv = "Matrix4x4 (";
  for (int i = 0; i < 16; i++)
    rv += QString::number (current_[i]) + ",";

  rv.remove (rv.length () - 1, 1);
  rv += ")";
  return rv;
}

//------------------------------------------------------------------------------

/// Read value from string
void Matrix4x4Widget::fromValue(QString _from)
{
  if (_from.startsWith ("Matrix4x4 ("))
    _from.remove (0, 11);
  if (_from.endsWith (")"))
    _from.remove (_from.length () - 1, 1);
  
  QStringList sl = _from.split (',');

  float v[16];
  bool ok = true;

  if (sl.length () == 16)
  {
    for (int i = 0; i < 16 && ok; i++)
      v[i] = sl[i].toFloat (&ok);

    if (ok)
      for (int i = 0; i < 16; i++)
      {
        current_[i] = v[i];
        fields_[i]->setText (QString::number (current_[i]));
      }
  }
}

//------------------------------------------------------------------------------

// handle slider changes
void Matrix4x4Widget::editingFinished ()
{
  bool ok;

  for (int i = 0; i < 16; i++)
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
void Matrix4x4Widget::toDefault()
{
  for (int i = 0; i < 16; i++)
  {
    current_[i] = default_[i];
    fields_[i]->setText (QString::number (current_[i]));
  }
}

//------------------------------------------------------------------------------
}

