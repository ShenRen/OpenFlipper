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




#include "addEmptyWidget.hh"
#include <OpenFlipper/common/GlobalOptions.hh>

addEmptyWidget::addEmptyWidget(std::vector< DataType > _types , QStringList _typeNames, QWidget *parent)
  : QDialog(parent),
    types_(_types),
    typeNames_(_typeNames)
{
  setupUi(this);

  typeBox->setDuplicatesEnabled(false);

  for (int i=0; i < (int)types_.size(); i++)
    typeBox->addItem(typeNames_[i],QVariant(types_[i].value()));

    //set last used DataType as currentItem
  for (int i=0; i < typeBox->count(); i++)
    if (typeBox->itemText(i) == OpenFlipper::Options::lastDataType())
      typeBox->setCurrentIndex(i);

  connect(cancelButton,SIGNAL(clicked()),this,SLOT(close()));
  connect(addButton,SIGNAL(clicked()),this,SLOT(slotAddEmpty()));
}

void addEmptyWidget::slotAddEmpty(){
  int index = typeBox->currentIndex();
  int unused;

  DataType type = (DataType) typeBox->itemData(index).toInt();
  for (int i=0; i < (int)types_.size(); i++)
    if (types_[i] == type){
      emit chosen(type,unused);
      OpenFlipper::Options::lastDataType(typeBox->currentText());
      this->close();
      return;
    }
  emit chosen(DATA_UNKNOWN,unused);
  this->close();
}
