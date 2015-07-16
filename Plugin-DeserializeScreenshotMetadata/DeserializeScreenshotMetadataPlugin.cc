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

