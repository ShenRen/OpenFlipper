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
#include <QLineEdit>
#include <QPushButton>

#include "filenameWidget.hh"

//== NAMESPACES ===============================================================
namespace VSI {

//=============================================================================
//
//  CLASS VSI::FilenameWidget - IMPLEMENTATION
//
//=============================================================================

/// Constructor
FilenameWidget::FilenameWidget(QMap< QString, QString > &_hints, QString _typeName, QWidget *_parent) :
  TypeWidget (_hints, _typeName, _parent),
  default_ (""),
  line_ (0),
  selectButton_ (0),
  mode_ (QFileDialog::AcceptOpen),
  filter_ ("Any File (*)"),
  defaultSuffix_ (""),
  dirOnly_ (false)
{
  QHBoxLayout *hL = new QHBoxLayout;

  line_ = new QLineEdit;

  if (_hints.contains ("default"))
    default_ = _hints["default"];

  line_->setText (default_);
  
  hL->addWidget (line_);

  selectButton_ = new QPushButton ("Select");

  connect (selectButton_, SIGNAL (pressed ()), this, SLOT (showDialog ()));
  hL->addWidget (selectButton_);

  setLayout (hL);

  if (_typeName == "Directory")
    dirOnly_ = true;

  if (_hints.contains ("mode") && _hints["mode"] == "Save")
    mode_ = QFileDialog::AcceptSave;

  if (_hints.contains ("filter"))
    filter_ = _hints["filter"];

  if (_hints.contains ("default_suffix"))
    defaultSuffix_ = _hints["default_suffix"];

}

//------------------------------------------------------------------------------

/// Destructor
FilenameWidget::~ FilenameWidget()
{
}

//------------------------------------------------------------------------------

/// Convert current value to string
QString FilenameWidget::toValue()
{
  return "\"" + line_->text () + "\"";
}

//------------------------------------------------------------------------------

/// Read value from string
void FilenameWidget::fromValue(QString _from)
{
  if (_from.isEmpty ())
  {
    line_->clear ();
    return;
  }

  _from.remove (0, 1);
  _from.remove (_from.length () - 1, 1);

  line_->setText (_from);
}

//------------------------------------------------------------------------------

/// Reset to default
void FilenameWidget::toDefault()
{
  line_->setText (default_);
}

//------------------------------------------------------------------------------

// Show file dialog
void FilenameWidget::showDialog()
{

  QFileDialog d (this, tr("Select File"), QString() , filter_);
  d.setAcceptMode (mode_);
  d.setDefaultSuffix (defaultSuffix_);
  if (dirOnly_)
  {
    d.setFileMode (QFileDialog::Directory);
    d.setOption (QFileDialog::ShowDirsOnly, true);
  }

  if (QDialog::Accepted == d.exec ())
    line_->setText (d.selectedFiles ()[0]);
}

//------------------------------------------------------------------------------
}
