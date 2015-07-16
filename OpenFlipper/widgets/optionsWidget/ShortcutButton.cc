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


#include "ShortcutButton.hh"

#include <iostream>

///Constructor
ShortcutButton::ShortcutButton(QWidget* _parent) :
QPushButton(_parent),
key_(-1),
oldKey_(-1),
checked_(false),
firstPress_(true)
{
  setCheckable(true);
}

///set the current shortcut
void ShortcutButton::setCurrentShortcut(int _key, Qt::KeyboardModifiers _modifiers)
{
  key_ = _key;
  modifiers_ = _modifiers;

  showCurrentShortcut();
}

///get current key
int ShortcutButton::key()
{
  return key_;
}

///get current modifiers
Qt::KeyboardModifiers ShortcutButton::modifiers()
{
  return modifiers_;
}

///key press
void ShortcutButton::keyPressEvent(QKeyEvent* event)
{
  if (firstPress_){
    oldKey_ = key_;
    oldModifiers_ = modifiers_;
  }

  firstPress_ = false;

  key_ = event->key();
  modifiers_ = event->modifiers();
}

///key release
void ShortcutButton::keyReleaseEvent(QKeyEvent* /*event*/)
{
  showCurrentShortcut();
  emit keyChanged();
}

///show the current shortcut
void ShortcutButton::showCurrentShortcut()
{

  firstPress_ = true;

  QString keyString;

  if (key_ == -1){
    keyString = tr("unassigned");
  }else if (key_ == Qt::Key_AltGr || key_ == Qt::Key_Alt || key_ == Qt::Key_Control || key_ == Qt::Key_Shift || key_ == Qt::Key_Meta){
    keyString = QKeySequence( modifiers_ ).toString();
    keyString = keyString.left(keyString.size()-1);
  }else
    keyString = QKeySequence( key_ + modifiers_ ).toString();

  setText(keyString);
}

void ShortcutButton::focusInEvent(QFocusEvent* event){
  setChecked(true);
  setText("");
  QPushButton::focusInEvent(event);
}

void ShortcutButton::focusOutEvent(QFocusEvent* event){
  setChecked(false);

  key_ = oldKey_;
  modifiers_ = oldModifiers_;

  showCurrentShortcut();
  QPushButton::focusOutEvent(event);
}

void ShortcutButton::mousePressEvent(QMouseEvent* event){
  checked_ = isChecked();
  QPushButton::mousePressEvent(event);
}

void ShortcutButton::mouseReleaseEvent(QMouseEvent* event){
  QPushButton::mouseReleaseEvent(event);
  setChecked( checked_ );
}

