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
 *   $Revision: 13548 $                                                       *
 *   $LastChangedBy: kremer $                                                 *
 *   $Date: 2012-01-27 11:58:19 +0100 (Fr, 27 Jan 2012) $                    *
 *                                                                            *
 \*===========================================================================*/

#ifndef FILEOPENVOLUMEMESH_HH
#define FILEOPENVOLUMEMESH_HH

#include <QObject>
#include <QMenuBar>

#include <OpenFlipper/common/Types.hh>
#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/FileInterface.hh>
#include <OpenFlipper/BasePlugin/LoadSaveInterface.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>
#include <OpenFlipper/BasePlugin/ScriptInterface.hh>
#include <OpenFlipper/BasePlugin/INIInterface.hh>

#include <ObjectTypes/PolyhedralMesh/PolyhedralMesh.hh>
#include <ObjectTypes/HexahedralMesh/HexahedralMesh.hh>
#ifdef ENABLE_OPENVOLUMEMESH_TETRAHEDRAL_SUPPORT
#include <ObjectTypes/TetrahedralMesh/TetrahedralMesh.hh>
#endif // ENABLE_OPENVOLUMEMESH_TETRAHEDRAL_SUPPORT
#ifndef FILEOPENVOLUMEMESHT_CC
    #include <OpenVolumeMesh/FileManager/FileManager.hh>
#endif

class FileOpenVolumeMeshPlugin: public QObject,
        BaseInterface,
        FileInterface,
        LoadSaveInterface,
        LoggingInterface,
        ScriptInterface,
        INIInterface {
Q_OBJECT
Q_INTERFACES(FileInterface)
Q_INTERFACES(LoadSaveInterface)
Q_INTERFACES(LoggingInterface)
Q_INTERFACES(BaseInterface)
Q_INTERFACES(ScriptInterface)
Q_INTERFACES(INIInterface)
#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "org.OpenFlipper.Plugins.Plugin-FileOVM")
#endif
public:
    // Default constructor
    FileOpenVolumeMeshPlugin();

signals:
    void openedFile(int _id);
    void addEmptyObject(DataType _type, int& _id);
    void load(QString _filename, DataType _type, int& _id);
    void save(int _id, QString _filename);
    void log(Logtype _type, QString _message);
    void log(QString _message);

    void emptyObjectAdded(int _id);

    void updateView();
    void updatedObject(int _id, const UpdateType& _type);

    void registerKey(int _key, Qt::KeyboardModifiers _modifiers, QString _description, bool _multiUse = false);

private slots:

    void fileOpened(int /*_id*/) {}

    void loadIniFileLast(INIFile& _ini, int _id);
    void saveIniFile(INIFile& _ini, int _id);

    void initializePlugin();

public:

    ~FileOpenVolumeMeshPlugin() {}

    QString name() {
        return (QString("FileOpenVolumeMesh"));
    }

    QString description() {
        return (QString(tr("Load/Save OpenVolumeMeshes")));
    }

    DataType supportedType();

    QString getSaveFilters();
    QString getLoadFilters();

    QWidget* saveOptionsWidget(QString _currentFilter);
    QWidget* loadOptionsWidget(QString _currentFilter);

public slots:

    int loadObject(QString _filename);

    bool saveObject(int _id, QString _filename);

    QString version() {
        return QString("1.0");
    }

private:

    template <typename MeshT>
    void loadMesh(const char* _filename, MeshT& _mesh, bool _comp, bool _topCheck);

    template <typename MeshT>
    void saveMesh(const char* _filename, MeshT& _mesh, bool _comp);

private:

    QWidget* loadOptions_;
    QWidget* saveOptions_;

    QComboBox* typeCheck_;

    QCheckBox* loadCompMode_;
    QCheckBox* loadTopCheck_;
    QCheckBox* saveCompMode_;

    OpenVolumeMesh::IO::FileManager fileManager_;
};

#if defined(INCLUDE_TEMPLATES) && !defined(FILEOPENVOLUMEMESHT_CC)
#include "FileOpenVolumeMeshT.cc"
#endif

#endif //FILEOPENVOLUMEMESH_HH
