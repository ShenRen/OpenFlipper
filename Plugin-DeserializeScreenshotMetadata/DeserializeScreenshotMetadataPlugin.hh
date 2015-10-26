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
*   $Revision: 18129 $                                                       *
*   $LastChangedBy: moebius $                                                *
*   $Date: 2014-02-05 10:25:53 +0100 (Mi, 05 Feb 2014) $                     *
*                                                                            *
\*===========================================================================*/

//=============================================================================
//
//  CLASS DeserializeScreenshotMetadataPlugin
//
//=============================================================================

#ifndef DESERIALIZESCREENSHOTMETADATAPLUGIN_HH
#define DESERIALIZESCREENSHOTMETADATAPLUGIN_HH

//== INCLUDES =================================================================

#include <QObject>

#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/ToolbarInterface.hh>
#include <OpenFlipper/BasePlugin/MetadataInterface.hh>

#include <ACG/QtWidgets/QtExaminerViewer.hh>

#include "RestoreFromScreenshotDlg.hh"

//== CLASS DEFINITION =========================================================

class DeserializeScreenshotMetadataPlugin: public QObject,
                                  BaseInterface,
                                  ToolbarInterface,
                                  MetadataInterface {

        Q_OBJECT
        Q_INTERFACES(BaseInterface)
        Q_INTERFACES(ToolbarInterface)
        Q_INTERFACES(MetadataInterface)

#if QT_VERSION >= 0x050000
        Q_PLUGIN_METADATA(IID "org.OpenFlipper.Plugins.Plugin-DeserializeScreenshotMetadataPlugin")
#endif

    signals:
        void updateView();
        void updatedObject(int /*_identifier*/, const UpdateType & /*_type*/);

        // ToolboxInterface
        void addToolbar(QToolBar* _toolbar);

        // MetadataInterface
        void metadataDeserialized(
                const QVector<QPair<QString, QString> > &data);

    private slots:

        // initialization functions
        void initializePlugin();
        void pluginsInitialized();
        void slot_restore_from_screenshot();
        void slot_do_restore_from_screenshot();

        void slotGenericMetadataDeserialized(QString key, QString value);
        void slotObjectMetadataDeserialized(QString object_name, QString value);
#if QT_VERSION >= 0x050000
        void slotObjectMetadataDeserializedJson(
                QString object_name, QJsonDocument value);
#endif


    public:

        DeserializeScreenshotMetadataPlugin();

        ~DeserializeScreenshotMetadataPlugin() { }

        QString name() {
            return (QString("DeserializeScreenshotMetadata"));
        }

        QString description() {
            return (QString("Deserializes meta data from viewer snapshots."));
        }

    private:
        QToolBar *toolbar;
        QAction *restoreAction;
        RestoreFromScreenshotDlg *restore_from_screenshot_dlg;
};

//=============================================================================
#endif // DESERIALIZESCREENSHOTMETADATAPLUGIN_HH defined
//=============================================================================

