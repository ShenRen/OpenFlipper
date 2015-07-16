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
*   $Revision: 10413 $                                                       *
*   $LastChangedBy: moebius $                                                *
*   $Date: 2010-12-09 15:40:04 +0100 (Thu, 09 Dec 2010) $                     *
*                                                                            *
\*===========================================================================*/

#include <iostream>
#include "OpenFlipperQSettings.hh"

QVariant OpenFlipperQSettings::value(const QString& key, const QVariant& defaultValue) const
{
  QVariant var = QSettings::value(key, defaultValue);
  if (!var.isValid())
    std::cerr << "Tried loading OpenFlipper settings value for " << key.toStdString() << ", but it was not found and no default value given!" << std::endl;
    

  #ifdef OPENFLIPPER_SETTINGS_DEBUG
    std::cerr << "Settings: Read key " << key.toStdString() << " with value " << var.toString().toStdString() << std::endl;
  #endif

  return var;
}


void OpenFlipperQSettings::setValue ( const QString & key, const QVariant & value ) {

  #ifdef OPENFLIPPER_SETTINGS_DEBUG
    std::cerr << "Settings: Set key " << key.toStdString() << " to value " << value.toString().toStdString() << std::endl;
  #endif

  QSettings::setValue(key,value);
}
