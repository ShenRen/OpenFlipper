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

#pragma once

#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>
#include <OpenFlipper/BasePlugin/ToolbarInterface.hh>
#include <OpenFlipper/BasePlugin/LoadSaveInterface.hh>
#include <OpenFlipper/BasePlugin/ScriptInterface.hh>

class MeshConvertPlugin: public QObject, BaseInterface, LoggingInterface, LoadSaveInterface, ScriptInterface, ToolbarInterface {
    Q_OBJECT
    Q_INTERFACES(BaseInterface)
    Q_INTERFACES(LoggingInterface)
    Q_INTERFACES(ToolbarInterface)
    Q_INTERFACES(LoadSaveInterface)
    Q_INTERFACES(ScriptInterface)

#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "org.OpenFlipper.Plugins.Plugin-MeshConvert")
#endif

signals:

    // LoggingInterface
    void log(Logtype _type, QString _message);
    void log(QString _message);

    //ToolbarInterface
    void addToolbar(QToolBar* _toolbar);
    void getToolBar(QString _name, QToolBar*& _toolbar);

    // LoadSaveInterface
    void addEmptyObject( DataType _type, int& _id);

    // BaseInterface
    void updatedObject(int _objectId);

    // ScriptInterface
    void setSlotDescription(QString     _slotName,   QString     _slotDescription,
                            QStringList _parameters, QStringList _descriptions);

public:
    MeshConvertPlugin();
    ~MeshConvertPlugin();

    // BaseInterface
    QString name() { return (QString("MeshConvert"));  }
    QString description() {  return (QString("Plugin used to convert PolyMesh to TriMesh and vice versa")); }

private slots:

    /// BaseInterface
    void initializePlugin();
    void pluginsInitialized();

    /// Tell system that this plugin runs without ui
    void noguiSupported( ) {} ;

private:

    QToolBar* toolbar;
    QActionGroup* grp;
    QAction* bidirectionalConversion;
    QAction* polyConversion;
    QAction* triConversion;


public slots:

    /**
     * @brief convert Converts trimesh to poly and vice versa depending on the Action that was called.
     */
    void convert(QAction*);

    /**
     * @brief convert    Convert a mesh to PolyMesh or to TriMesh. The old mesh remains unchanged.
     * @param _id        Object_ID to determine which object shall be converted.
     * @param _toTriMesh Flag to indicate conversion to TriMesh or Polymesh if set to false.
     * @return           the ID of the new mesh or -1 in case of error.
     */
    int convert(int _id, bool _toTriMesh);

    QString version() { return QString("1.0"); };

};
