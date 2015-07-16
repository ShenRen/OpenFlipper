/*===========================================================================*\
 *                                                                           *
 *                             OpenFlipper                                   *
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
*   $Revision$                                                       *
*   $LastChangedBy$                                                *
*   $Date$                     *
*                                                                            *
\*===========================================================================*/

#ifndef FILEOBJPLUGIN_HH
#define FILEOBJPLUGIN_HH

#include <QObject>

#include <OpenFlipper/common/Types.hh>
#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/FileInterface.hh>
#include <OpenFlipper/BasePlugin/LoadSaveInterface.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>
#include <OpenFlipper/BasePlugin/ScriptInterface.hh>
#include <OpenFlipper/BasePlugin/TypeInterface.hh>
#include <OpenFlipper/BasePlugin/RPCInterface.hh>
#include <OpenFlipper/BasePlugin/StatusbarInterface.hh>
#include <OpenFlipper/BasePlugin/TextureInterface.hh>

#include <ObjectTypes/PolyMesh/PolyMesh.hh>
#include <ObjectTypes/TriangleMesh/TriangleMesh.hh>

#ifdef ENABLE_BSPLINECURVE_SUPPORT
#include <ObjectTypes/BSplineCurve/BSplineCurve.hh>
#endif

#ifdef ENABLE_BSPLINESURFACE_SUPPORT
#include <ObjectTypes/BSplineSurface/BSplineSurface.hh>
#endif

#include "OBJImporter.hh"
#include "Material.hh"

#include <map>

enum ReaderMode
{
  NONE    = 0,
  CURVE   = 1,
  SURFACE = 1 << 1
};

class FileOBJPlugin : public QObject, BaseInterface, FileInterface, LoadSaveInterface,
    LoggingInterface, ScriptInterface, StatusbarInterface, RPCInterface, TextureInterface
{
   Q_OBJECT
   Q_INTERFACES(FileInterface)
   Q_INTERFACES(LoadSaveInterface)
   Q_INTERFACES(LoggingInterface)
   Q_INTERFACES(BaseInterface)
   Q_INTERFACES(ScriptInterface)
   Q_INTERFACES(StatusbarInterface)
   Q_INTERFACES(RPCInterface)
   Q_INTERFACES(TextureInterface)
#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "org.OpenFlipper.Plugins.Plugin-FileOBJ")
#endif
  signals:
    void openedFile( int _id );
    void addEmptyObject( DataType _type, int& _id);
    void load(QString _filename, DataType _type, int& _id);
    void save(int _id , QString _filename );
    void log(Logtype _type, QString _message);
    void log(QString _message);
    void updateView();
    void updatedObject(int _identifier, const UpdateType& _type);

    void deleteObject( int _id );

    // StatusbarInterface
    void showStatusMessage(QString _message, int _timeout = 0);
    void setStatus( ApplicationStatus::applicationStatus _status);

    //RPCInterface
    void pluginExists( QString _pluginName , bool& _exists  );

    //TextureInterface
    void setTextureMode(QString _textureName, QString _mode, int _id );
    void switchTexture( QString _textureName, int _id );
    void addMultiTexture( QString _textureGroup, QString _name, QString _filename, int _id, int& _textureId);
    void textureFilename( int /*_id*/, QString /*_textureName*/, QString& /*_textureFilename*/ );
    void textureIndex(QString _name,int _id, int& _index);
    void getCurrentTexture(int /*_id*/, QString& /*_name*/);
    void textureName (int, int, QString &);
    void getSubTextures (int, QString, QStringList &);
    void textureIndexPropertyName(int, QString&);

  private slots:

    void fileOpened( int /*_id*/ ){};

    void noguiSupported( ) {} ;

    void initializePlugin();

    /// Slot called when user wants to save the given Load options as default
    void slotLoadDefault();

    /// Slot called when user wants to save the given Save options as default
    void slotSaveDefault();

    void slotHandleCheckBoxes(bool _checked);

  public :

     FileOBJPlugin();

     ~FileOBJPlugin() {};

     QString name() { return (QString("FileOBJ")); };
     QString description( ) { return (QString(tr("Load/Save OBJ-Files"))); };

     DataType supportedType();

     QString getSaveFilters();
     QString getLoadFilters();

     QWidget* saveOptionsWidget(QString /*_currentFilter*/);
     QWidget* loadOptionsWidget(QString /*_currentFilter*/);

  public slots:

    /// Loads Object and converts it to a triangle mesh if possible
    int loadObject(QString _filename);

    /// load object and force type
    int loadObject(QString _filename, DataType _type);

    bool saveObject(int _id, QString _filename);

    QString version() { return QString("1.0"); };

  private:

    /// Reader functions
    void checkTypes(QByteArray& _bufferedFile, QString _filename, OBJImporter& _importer, QStringList& _includes);

    bool readMaterial(QString _filename, OBJImporter& _importer);
    void readOBJFile(QByteArray& _bufferedFile, QString _filename, OBJImporter& _importer);
    void createAllGroupObjects(OBJImporter& _importer);
    //void addNewObject(OBJImporter& _importer, QString _name );
    template <class MeshT>
    void backupTextureCoordinates(MeshT& _mesh);
    void addTextures(OBJImporter& _importer, int _objectID );

    /// Convert non-valid filenames (e.g. of groups that end with .jpg) to valid .objs.
    void convertToOBJName(QString& _name);

  private :

    /// List that contains the material properties
    MaterialList materials_;

    template< class MeshT >
    Material& getMaterial(MeshT& _mesh, const OpenMesh::FaceHandle& _fh, int _objId);

    ///writer functions
    template< class MeshT >
    bool writeMaterial(QString _filename, MeshT& _mesh, int _objId );

    template< class MeshT >
    bool writeMesh(std::ostream& _out, QString _filename, MeshT& _mesh, int _objId);

    #ifdef ENABLE_BSPLINECURVE_SUPPORT
    bool writeCurve(std::ostream& _out, QString _filename, BSplineCurve* _curve );
    #endif

    #ifdef ENABLE_BSPLINESURFACE_SUPPORT
    bool writeSurface(std::ostream& _out, QString _filename, BSplineSurface* _surface );
    #endif


  private:

    //Option Widgets
    QWidget* loadOptions_;
    QWidget* saveOptions_;

    QCheckBox*   saveBinary_;
    QCheckBox*   saveVertexColor_;
    QCheckBox*   saveFaceColor_;
    QCheckBox*   saveAlpha_;
    QCheckBox*   saveNormals_;
    QCheckBox*   saveTexCoords_;
    QCheckBox*   saveTextures_;
    QCheckBox*   saveCopyTextures_;
    QCheckBox*   saveCreateTexFolder_;
    QLabel*      savePrecisionLabel_;
    QSpinBox*    savePrecision_;
    QPushButton* saveDefaultButton_;


    QComboBox*   triMeshHandling_;
    QCheckBox*   loadVertexColor_;
    QCheckBox*   loadFaceColor_;
    QCheckBox*   loadAlpha_;
    QCheckBox*   loadNormals_;
    QCheckBox*   loadTexCoords_;
    QCheckBox*   loadTextures_;
    QPushButton* loadDefaultButton_;

    bool forceTriangleMesh_;
    bool forcePolyMesh_;

    QString textureIndexPropertyName_;
    bool    textureIndexPropFetched_;
    std::map<int,QString> texIndexFileMap_;
};


#if defined(INCLUDE_TEMPLATES) && !defined(FILEOBJPLUGIN_C)
#define FILEOBJPLUGIN_TEMPLATES
#include "FileOBJT.cc"
#endif


#endif //FILEOBJPLUGIN_HH
