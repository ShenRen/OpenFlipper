/*===========================================================================*\
*                                                                            *
*                              OpenFlipper                                   *
*      Copyright (C) 2001-2014 by Computer Graphics Group, RWTH Aachen       *
*                           www.openflipper.org                              *
*                                                                            *
*--------------------------------------------------------------------------- *
*  This file is part of OpenFlipper.                                         *
*                                                                            *
*  OpenFlipper is free software: you can redistribute it and/or modify       *
*  it under the terms of the GNU Lesser General Public License as            *
*  published by the Free Software Foundation, either version 3 of            *
*  the License, or (at your option) any later version with the               *
*  following exceptions:                                                     *
*                                                                            *
*  If other files instantiate templates or use macros                        *
*  or inline functions from this file, or you compile this file and          *
*  link it with other files to produce an executable, this file does         *
*  not by itself cause the resulting executable to be covered by the         *
*  GNU Lesser General Public License. This exception does not however        *
*  invalidate any other reasons why the executable file might be             *
*  covered by the GNU Lesser General Public License.                         *
*                                                                            *
*  OpenFlipper is distributed in the hope that it will be useful,            *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of            *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
*  GNU Lesser General Public License for more details.                       *
*                                                                            *
*  You should have received a copy of the GNU LesserGeneral Public           *
*  License along with OpenFlipper. If not,                                   *
*  see <http://www.gnu.org/licenses/>.                                       *
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
        void updatedObject(int /*_identifier*/, const UpdateType /*_type*/);

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

