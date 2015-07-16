/*===========================================================================*\
 *                                                                           *
 *                              OpenFlipper                                  *
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
 *                                                                           *
 \*===========================================================================*/

/*===========================================================================*\
*                                                                            *
*   $Revision: 11279 $                                                       *
*   $LastChangedBy: kremer $                                                 *
*   $Date: 2011-04-11 17:35:27 +0200 (Mon, 11 Apr 2011) $                    *
*                                                                            *
\*===========================================================================*/

#ifndef TYPEHEXAHEDRALMESH_HH
#define TYPEHEXAHEDRALMESH_HH

#include <QObject>

#include <ObjectTypes/HexahedralMesh/HexahedralMesh.hh>
#include <OpenFlipper/common/Types.hh>
#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/LoadSaveInterface.hh>
#include <OpenFlipper/BasePlugin/TypeInterface.hh>
#include <OpenFlipper/BasePlugin/KeyInterface.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>
#include <OpenFlipper/BasePlugin/ContextMenuInterface.hh>

class TypeHexahedralMeshPlugin : public QObject, BaseInterface, LoadSaveInterface, TypeInterface, KeyInterface, LoggingInterface, ContextMenuInterface {
Q_OBJECT
Q_INTERFACES(BaseInterface)
Q_INTERFACES(LoadSaveInterface)
Q_INTERFACES(TypeInterface)
Q_INTERFACES(KeyInterface)
Q_INTERFACES(LoggingInterface)
Q_INTERFACES(ContextMenuInterface)

#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "org.OpenFlipper.Plugins.Plugin-TypeHexahedralMesh")
#endif

signals:
    // Logging interface
    void log(Logtype _type, QString _message);
    void log(QString _message);

    // LoadSave Interface
    void emptyObjectAdded(int _id);

    // ContextMenuInterface
    void addContextMenuItem(QAction* /*_action*/, DataType /*_objectType*/, ContextMenuType /*_type*/);

    // BaseInterface
    void updatedObject(int _objId, const UpdateType& _type);

private slots:

    // BaseInterface
    void slotObjectUpdated(int _identifier, const UpdateType& _type);
    void objectDeleted(int _identifier);
    void noguiSupported() {}
    void pluginsInitialized();

    // KeyInterface
    void slotKeyEvent(QKeyEvent* _event);

    // ContextMenuInterface
    void slotUpdateContextMenu(int _objectId);

    // Switches

    void slot_change_shrinkage();

    void slot_update_planes_in_scenegraph_node(int _deletedObject = -1);

    void switchRendering();

    void setTranslucencyFactor();

public:

    ~TypeHexahedralMeshPlugin() {
    }

    TypeHexahedralMeshPlugin();

    QString name() {
        return (QString("TypeHexahedralMesh"));
    }

    QString description() {
        return (QString(tr("Register HexahedralMesh type ")));
    }

    bool registerType();

public slots:

    QString version() {
        return QString("1.0");
    }

    // Type Interface
    int addEmpty();

    DataType supportedType() {
        return DATA_HEXAHEDRAL_MESH;
    }
    

private:

    QAction* render_switch_;

    QAction* translucency_factor_action_;

};

#endif //TYPEHEXAHEDRALMESH_HH
