/*
 * RestoreFromScreenshotDlg.hh
 *
 *  Created on: Jan 7, 2014
 *      Author: ebke
 */

#ifndef RESTOREFROMSCREENSHOTDLG_HH_
#define RESTOREFROMSCREENSHOTDLG_HH_

#include "ui_RestoreFromScreenshotDlg.hh"

class RestoreFromScreenshotDlg : public QDialog,
                                 public Ui::RestoreFromScreenshotDlg {

    Q_OBJECT

    public:
        RestoreFromScreenshotDlg(QWidget * parent = 0) :
                QDialog(parent) {
            setupUi(this);
        }

        QString getRestoreFileName() {
            return restore_file_picker->currentFileName();
        }
};

#endif /* RESTOREFROMSCREENSHOTDLG_HH_ */
