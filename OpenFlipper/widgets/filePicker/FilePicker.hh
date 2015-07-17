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

/*
 * FilePicker.hh
 *
 *  Created on: Jan 8, 2013
 *      Author: ebke
 */

#ifndef FILEPICKER_HH_
#define FILEPICKER_HH_

#include <OpenFlipper/common/RecentFiles.hh>

#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>


class DLLEXPORT FilePicker : public QWidget {
    Q_OBJECT
    Q_PROPERTY(QString recent_files_ini_key
               READ recent_files_ini_key
               WRITE set_recent_files_ini_key)
    Q_PROPERTY(QString dialog_title
               READ dialog_title
               WRITE set_dialog_title)
    Q_PROPERTY(QString file_filter
               READ file_filter
               WRITE set_file_filter)
    Q_PROPERTY(DialogType dialog_type
               READ dialog_type
               WRITE set_dialog_type)
    Q_PROPERTY(QString current_file_name
               READ current_file_name
               WRITE set_current_file_name)
    Q_ENUMS(DialogType)

    public:
        enum DialogType {
            DT_OPEN_FILE = OpenFlipper::Options::DT_OPEN,
            DT_SAVE_FILE = OpenFlipper::Options::DT_SAVE,
            DT_CHOOSE_PATH = OpenFlipper::Options::DT_CHOOSE_PATH
        };

        explicit FilePicker(QWidget *parent = 0);
        explicit FilePicker(QString recent_files_ini_key,
                              DialogType dialog_type,
                              QWidget *parent = 0);
        virtual ~FilePicker();

        QString current_file_name() {
            return textBox_->currentText();
        }

        void set_current_file_name(QString value) {
            return textBox_->lineEdit()->setText(value);
        }

        // Legacy:
        QString currentFileName() {
            return current_file_name();
        }

        QString recent_files_ini_key() { return recent_files_ini_key_; }
        void set_recent_files_ini_key(QString value);

        QString dialog_title() { return dialog_title_; }
        void set_dialog_title(QString value) {
            dialog_title_ = value;
        }

        QString file_filter() { return file_filter_; }
        void set_file_filter(QString value) {
            file_filter_ = value;
        }

        DialogType dialog_type() { return dialog_type_; }
        void set_dialog_type(DialogType value) {
            dialog_type_ = value;
        }

        bool overwrite_confirmed_by_user() {
            return overwrite_confirmed_by_user_;
        }

    public slots:
        bool confirm_overwrite_if_necessary();

    protected slots:
        void slot_browse();
        void path_changed();

    private:
        void init();

    protected:
        QComboBox *textBox_;
        QPushButton *browseButton_;
        QString recent_files_ini_key_;
        QString dialog_title_;
        QString file_filter_;
        DialogType dialog_type_;
        bool overwrite_confirmed_by_user_;
};

#endif /* FILEPICKER_HH_ */
