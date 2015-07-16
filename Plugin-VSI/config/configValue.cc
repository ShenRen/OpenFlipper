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
#include <QHBoxLayout>
#include <QFrame>

#include "../scene/elementInput.hh"
#include "../parser/type.hh"
#include "../parser/inout.hh"
#include "../parser/element.hh"
#include "../parser/context.hh"

#include "configValue.hh"

//== NAMESPACES ===============================================================
namespace VSI {

//=============================================================================
//
//  CLASS VSI::ConfigValue - IMPLEMENTATION
//
//=============================================================================

/// Constructor
ConfigValue::ConfigValue (ElementInput *_input) :
  input_ (_input),
  main_ (0),
  constant_ (0),
  optional_ (0),
  forceAskUser_ (0),
  buttonGroup_ (0)
{
  group_ = new QGroupBox (_input->inOut ()->shortDescription ());
  group_->setCheckable (false);

  Type *t = _input->inOut ()->element ()->context ()->getType (_input->inOut ()->typeString ());
  if (!t)
    return;

  QVBoxLayout *vL = new QVBoxLayout;
  QHBoxLayout *hL = new QHBoxLayout;

  if (_input->state () & Input::Optional || !(_input->state () & Input::NoExternalInput) ||
      !(_input->state () & Input::NoRuntimeUserInput))
  {
    QHBoxLayout *bL = new QHBoxLayout;
    QString dI = (_input->state () & Input::NoExternalInput) ? "" : "Direct Input / ";

    buttonGroup_ = new QButtonGroup (this);

    constant_ = new QRadioButton ("Constant value");
    if (_input->state () & Input::Optional)
    {
      optional_ = new QRadioButton (dI + "Optional");
      if (!(_input->state () & Input::NoRuntimeUserInput))
      {
        forceAskUser_ = new QRadioButton ("Ask during execution");
        buttonGroup_->addButton (forceAskUser_, 2);
      }
    }
    else
      optional_ = new QRadioButton (dI + "Ask during execution");
    buttonGroup_->addButton (optional_, 0);
    buttonGroup_->addButton (constant_, 1);

    bL->addWidget (optional_);
    bL->addWidget (constant_);
    if (forceAskUser_)
    {
      bL->addWidget (forceAskUser_);
      buttonGroup_->addButton (forceAskUser_, 2);
    }

    vL->addLayout (bL);

    QFrame *f = new QFrame ();

    f->setFrameStyle (QFrame::HLine | QFrame::Plain);
    vL->addWidget (f);

    connect (buttonGroup_, SIGNAL (buttonClicked (int)),
             this, SLOT (selectionChange()));
  }


  main_ = t->widget (_input->inOut ()->hints (), _input->inOut ()->typeString ());

  hL->addWidget (main_);
  hL->setStretchFactor (main_, 2);

  QFrame *f = new QFrame ();

  f->setFrameStyle (QFrame::VLine | QFrame::Plain);
  hL->addWidget (f);

  default_ = new QPushButton ("Default");
  hL->addWidget (default_);

  vL->addLayout (hL);
  vL->addStretch(1);
  group_->setLayout (vL);

  if (_input->isSet ())
  {
    main_->fromValue (_input->value ());

    if (constant_)
      constant_->setChecked (true);
  }
  else
  {
    if (!_input->value ().isEmpty ())
      main_->fromValue (_input->value ());

    if (_input->isForceAskSet () && forceAskUser_)
    {
      forceAskUser_->setChecked (true);
      main_->setEnabled (false);
      default_->setEnabled (false);
    }
    else if (optional_)
    {
      optional_->setChecked (true);
      main_->setEnabled (false);
      default_->setEnabled (false);
    }
  }

  connect (default_, SIGNAL (pressed()), this, SLOT (toDefault()));
}

//------------------------------------------------------------------------------

/// Destructor
ConfigValue::~ ConfigValue ()
{
  if (main_)
    delete main_;
  delete group_;
}

//------------------------------------------------------------------------------

// reset to default
void ConfigValue::toDefault()
{
  if (main_)
    main_->toDefault ();
}

//------------------------------------------------------------------------------

void ConfigValue::selectionChange()
{

  if (constant_->isChecked())
  {
    main_->setEnabled (true);
    default_->setEnabled (true);
  }
  else
  {
    main_->setEnabled (false);
    default_->setEnabled (false);
  }
}


//------------------------------------------------------------------------------
}

