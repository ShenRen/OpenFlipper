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
//  CLASS DeserializeScreenshotMetadataPlugin - IMPLEMENTATION
//
//=============================================================================

//== INCLUDES =================================================================

#include "DeserializeScreenshotMetadataPlugin.hh"

#include <OpenFlipper/BasePlugin/PluginFunctions.hh>
#include <OpenFlipper/BasePlugin/RPCWrappers.hh>
#include <OpenFlipper/common/GlobalOptions.hh>

//== IMPLEMENTATION ==========================================================

DeserializeScreenshotMetadataPlugin::DeserializeScreenshotMetadataPlugin() :
        toolbar(0),
        restoreAction(0),
        restore_from_screenshot_dlg(0) {
}

void DeserializeScreenshotMetadataPlugin::initializePlugin() {
}

void DeserializeScreenshotMetadataPlugin::pluginsInitialized() {
    toolbar = new QToolBar(tr("Restore from Screenshot"));

    restoreAction = new QAction(tr("Restore Scene From Screenshot"), toolbar);
    restoreAction->setIcon(
            QIcon(OpenFlipper::Options::iconDirStr() +
                  OpenFlipper::Options::dirSeparator() +
                  "restore_from_screenshot.png"));

    toolbar->addAction(restoreAction);

    emit addToolbar(toolbar);

    connect(restoreAction, SIGNAL(triggered(bool)), this,
            SLOT(slot_restore_from_screenshot()));
}

void DeserializeScreenshotMetadataPlugin::slot_do_restore_from_screenshot() {
    if (!restore_from_screenshot_dlg) return;

    const QString restoreFileName =
            restore_from_screenshot_dlg->getRestoreFileName();

    QImage img(restoreFileName);
    if (img.isNull()) {
        QMessageBox::warning(0, tr("Unable to load image"),
                             tr("Unable to load image. "
                                "Unrecognized format or file not existent."),
                             QMessageBox::Ok);
        return;
    }

    /*
     * Deserialize View
     */
    if (restore_from_screenshot_dlg->restore_viewer_cb->isChecked()) {
        QString view = img.text("View");
        if (!view.isEmpty()) {
            RPC::callFunction("core", "setViewAndWindowGeometry", view);
        }
    }

    /*
     * Deserialize Materials
     */
    if (restore_from_screenshot_dlg->restore_materials_cb->isChecked()) {
        QString materials_json =
                QString::fromUtf8("{") + img.text("Mesh Materials") +
                QString::fromUtf8("}");
        QVariantMap materials = ACG::json_to_variant_map(materials_json);

        QMap<QString, ACG::SceneGraph::Material*> objname_to_material;
        for (PluginFunctions::ObjectIterator
                o_it(PluginFunctions::ALL_OBJECTS, DATA_ALL);
                o_it != PluginFunctions::objectsEnd(); ++o_it) {

            if (!o_it->materialNode()) continue;

            objname_to_material[o_it->name()] = &o_it->materialNode()->material();
        }

        for (QVariantMap::const_iterator it = materials.begin();
                it != materials.end(); ++it) {

            if (objname_to_material.contains(it.key())) {
                objname_to_material[it.key()]->deserializeFromVariantMap(
                        it.value().toMap());
            }
        }
    }

    /*
     * Notify core.
     */
    if (restore_from_screenshot_dlg->restore_objectmd_cb->isChecked()) {
        QStringList textKeys = img.textKeys();
        QVector<QPair<QString, QString> > metadata;
        metadata.reserve(textKeys.size());
        for (QStringList::iterator it = textKeys.begin();
                it != textKeys.end(); ++it) {
            metadata.push_back(QPair<QString, QString>(*it, img.text(*it)));
        }
        emit metadataDeserialized(metadata);
    }

}

void DeserializeScreenshotMetadataPlugin::slot_restore_from_screenshot() {
    if (!restore_from_screenshot_dlg) {
        restore_from_screenshot_dlg = new RestoreFromScreenshotDlg();
        connect(restore_from_screenshot_dlg->restore_pb,
                SIGNAL(clicked()),
                this,
                SLOT(slot_do_restore_from_screenshot()));
    }
    restore_from_screenshot_dlg->show();
    restore_from_screenshot_dlg->raise();
}

void DeserializeScreenshotMetadataPlugin::slotGenericMetadataDeserialized(
        QString key, QString value) {

}

void DeserializeScreenshotMetadataPlugin::slotObjectMetadataDeserialized(
        QString object_name, QString value) {

}

#if QT_VERSION >= 0x050000
void DeserializeScreenshotMetadataPlugin::slotObjectMetadataDeserializedJson(
        QString object_name, QJsonDocument value) {

}
#endif

//-----------------------------------------------------------------------------

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(DeserializeScreenshotMetadataPlugin, DeserializeScreenshotMetadataPlugin);
#endif

