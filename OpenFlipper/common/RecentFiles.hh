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

/**
\file GlobalOptions.hh 
 * This Header provides access to globally defined variables such as 
 * Aplication Directories. You may use these Functions inside your plugins.
*/

#ifndef RECENTFILES_HH
#define RECENTFILES_HH

#include <QDir>
#include <QStringList>
#include <QString>
#include <QComboBox>
#include <QFileDialog>
#include <stdexcept>

#include <OpenFlipper/common/Types.hh>

namespace OpenFlipper {
namespace Options {
  

//===========================================================================
/** @name Recent Files
* @{ */
//===========================================================================  
  
  /// Add a file to the recent files list ( removes one, if list grows to larger then maxRecent )
  DLLEXPORT
  void addRecentFile(QString _file, DataType _type);

  /**
   * Bookkeeping helper for a list of recent items.
   *
   * Usage example:
   * OpenFlipper::Options::rememberRecentItem(QString::fromUtf8("Plugin-MIQParameterize/File/RecentGurobiEnvironmentFiles"), file);
   */
  DLLEXPORT
  void rememberRecentItem(const QString &propName, const QString &itemName, const int RECENT_ITEMS_MAX_SIZE = 10);

  /**
   * Bookkeeping helper for a list of recent items.
   */
  DLLEXPORT
  QStringList getRecentItems(const QString &propName);

  /**
   * Get the most recent item out of a list of recent items.
   * Returns an empty string if no recent item exists.
   */
  DLLEXPORT
  QString getMostRecentItem(const QString &propName);

  /**
   * Update combo box text. At the same time, store text in recent items list.
   *
   * @param cb The combo box to update.
   * @param value The value to update the combo box with.
   * @param propName The name of the recent item property.
   */
  inline static void updateComboBox(QComboBox *cb, const QString &value, const char *propName) {
      if (propName != 0) {
          cb->clear();
          if (value.length() > 0)
              rememberRecentItem(QString::fromUtf8(propName), value);
          const QStringList content = getRecentItems(QString::fromUtf8(propName));
          cb->insertItems(0, content);
      }
      cb->setEditText(value);
  }

  enum DialogType {
      DT_OPEN,
      DT_SAVE,
      DT_CHOOSE_PATH,
  };

  /**
   * Obtain a path name from the user. Update the specified
   * combo box with that path name and remember it in the specified
   * recent items property.
   *
   * @param dialog_type The type of dialog to prompt the user with.
   * @return The chosen path name.
   */
  inline static QString obtainPathName(QComboBox *cb, const char *title, const char *filters,
                                       const char *propName, DialogType dialog_type = DT_OPEN) {
      QString result;
      switch (dialog_type) {
          case DT_OPEN:
              result = QFileDialog::getOpenFileName(0, QObject::tr(title), cb->currentText(), QObject::tr(filters));
              break;
          case DT_SAVE:
              result = QFileDialog::getSaveFileName(0, QObject::tr(title), cb->currentText(), QObject::tr(filters));
              break;
          case DT_CHOOSE_PATH:
              result = QFileDialog::getExistingDirectory(0, QObject::tr(title), cb->currentText());
              break;
          default:
              throw std::logic_error("Unknown value for dialog_type.");
      }

      if (result.length() > 0)
          updateComboBox(cb, result, propName);

      return result;
  }
/** @} */
  
  
}
}

//=============================================================================
#endif // OPTIONS_HH defined
//=============================================================================
