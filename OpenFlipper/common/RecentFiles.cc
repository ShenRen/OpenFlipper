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




//=============================================================================
//
//  Options used throughout the System
//
//=============================================================================

#include <QDir>

#include "RecentFiles.hh"

#include <OpenFlipper/common/GlobalOptions.hh>

namespace OpenFlipper {
namespace Options {
  

void addRecentFile(QString _file, DataType _type) {
  
  
  QStringList recentFiles = OpenFlipperSettings().value("Core/File/RecentFiles", QStringList()).toStringList();
  QStringList recentTypes = OpenFlipperSettings().value("Core/File/RecentTypes", QStringList()).toStringList();
  
  QString type = typeName(_type);
  
  // Check if file already in recent list
  for ( int i = 0 ; i < recentFiles.size() ; ++i)
    if ( _file == recentFiles[i] && type == recentTypes[i] ){
      recentFiles.removeAt(i);
      recentTypes.removeAt(i);
    }
  
  // Erase if too many files in list
  if ( recentFiles.size() >= OpenFlipperSettings().value("Core/File/MaxRecent",15).toInt() ) {
    recentFiles.pop_back();
    recentTypes.pop_back();
  }

  recentFiles.push_front(_file);
  recentTypes.push_front(type);
  
  OpenFlipperSettings().setValue("Core/File/RecentFiles", recentFiles);
  OpenFlipperSettings().setValue("Core/File/RecentTypes", recentTypes); 
}

void rememberRecentItem(const QString &propName, const QString &itemName, const int RECENT_ITEMS_MAX_SIZE) {
    // Read setting.
    QStringList recentItems = OpenFlipperSettings().value(propName, QStringList()).toStringList();

    // If file already on list, remove it (so it appears on top, eventually).
    int position = recentItems.indexOf(QRegExp(QRegExp::escape(itemName)));
    if (position != -1) recentItems.removeAt(position);

    // Truncate list if too long.
    if (recentItems.size() >= RECENT_ITEMS_MAX_SIZE) recentItems.removeLast();

    // Add new file to front.
    recentItems.push_front(itemName);

    // Write back setting.
    OpenFlipperSettings().setValue(propName, recentItems);
}

QStringList getRecentItems(const QString &propName) {
    return OpenFlipperSettings().value(propName, QStringList()).toStringList();
}

QString getMostRecentItem(const QString &propName) {
    QStringList sl = OpenFlipperSettings().value(propName, QStringList()).toStringList();
    if (sl.empty()) return QString();
    return sl[0];
}

}
}

//=============================================================================

