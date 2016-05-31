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




#ifndef TEXTURECONTROLPLUGIN_HH
#define TEXTURECONTROLPLUGIN_HH

#define OM_FORCE_STATIC_CAST

#include <QObject>
#include <QMenuBar>

#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/BackupInterface.hh>
#include <OpenFlipper/BasePlugin/TextureInterface.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>
#include <OpenFlipper/BasePlugin/MenuInterface.hh>
#include <OpenFlipper/BasePlugin/LoadSaveInterface.hh>
#include <OpenFlipper/BasePlugin/ContextMenuInterface.hh>
#include <ObjectTypes/PolyMesh/PolyMesh.hh>
#include <ObjectTypes/TriangleMesh/TriangleMesh.hh>

#ifdef ENABLE_OPENVOLUMEMESH_POLYHEDRAL_SUPPORT
    #include <ObjectTypes/PolyhedralMesh/PolyhedralMesh.hh>
#endif
#ifdef ENABLE_OPENVOLUMEMESH_HEXAHEDRAL_SUPPORT
    #include <ObjectTypes/HexahedralMesh/HexahedralMesh.hh>
#endif

#include <OpenFlipper/common/Types.hh>
#include "textureProperties.hh"

#include "TextureData.hh"

class TextureControlPlugin : public QObject, BaseInterface, BackupInterface, TextureInterface, MenuInterface, LoggingInterface, LoadSaveInterface,ContextMenuInterface
{
  Q_OBJECT
  Q_INTERFACES(BaseInterface)
  Q_INTERFACES(TextureInterface)
  Q_INTERFACES(MenuInterface)
  Q_INTERFACES(LoggingInterface)
  Q_INTERFACES(LoadSaveInterface)
  Q_INTERFACES(ContextMenuInterface)
  Q_INTERFACES(BackupInterface)

#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "org.OpenFlipper.Plugins.Plugin-TextureControl")
#endif

  public:

  signals:

    // BaseInterface
    void updateView();
    void updatedObject(int _identifier, const UpdateType& _type);

    // TextureInterface
    void updateTexture( QString , int);

    // LoggingInterface
    void log(Logtype _type, QString _message);
    void log(QString _message);

    // MenuInterface
    void addMenubarAction(QAction* _action, QString _type );

    // ContextMenuInterface
    void addContextMenuItem(QAction* _action ,DataType _objectType , ContextMenuType _type );


  private slots:

    // BaseInterface
    void pluginsInitialized();
    void slotObjectUpdated(int _identifier, const UpdateType& _type);
    void slotDrawModeChanged(int _viewerId );

    // TextureInterface
    void slotUpdateAllTextures( );
    void slotTextureAdded( QString _textureName , QString _filename , uint _dimension , int _id );
    void slotTextureAdded( QString _textureName , QString _filename , uint _dimension );
    void slotMultiTextureAdded( QString _textureGroup , QString _name , QString _filename , int _id , int& _textureId );
    void slotTextureUpdated( QString _textureName , int _identifier );
    void slotSetTextureMode(QString _textureName ,QString _mode, int _id);
    void slotSetTextureMode(QString _textureName ,QString _mode);
    void slotSwitchTexture( QString _textureName, int _id );
    void slotSwitchTexture( QString _textureName );
    void slotUpdateTexture( QString _textureName , int _identifier);
    void slotTextureChangeImage( QString _textureName , QImage& _image , int _id );
    void slotTextureChangeImage( QString _textureName , QImage& _image );
    void slotTextureGetImage( QString _textureName, QImage& _image, int _id );
    void slotTextureGetImage( QString _textureName, QImage& _image );
    void slotTextureIndex( QString _textureName, int _id, int& _index);
    void slotTextureIndexPropertyName( int _id, QString& _propertyName);
    void slotTextureName( int _id, int _textureIndex, QString& _textureName);
    void slotTextureFilename( int _id, QString _textureName, QString& _textureFilename);
    void slotGetCurrentTexture( int _id, QString& _textureName );
    void slotGetSubTextures( int _id, QString _multiTextureName, QStringList& _subTextures );
    
    // LoadSaveInterface
    void fileOpened( int _id );
    void addedEmptyObject( int _id );
    
    //BackupInterface
    void slotAboutToRestore( int _objectid );
    void slotRestored( int _objectid);

  private slots:

    void doSwitchTexture( QString _textureName, int _id );

    /// Called when an action in the TextureMenu is triggered
    void slotTextureMenu(QAction* _action);

    /// Slot for showing the TextureProperties Dialog
    void slotSetTextureProperties();

  public :
    /// Constructor
    TextureControlPlugin();
    /// Destructor
    ~TextureControlPlugin() {delete(settingsDialog_);};

    QString name() { return (QString("TextureControl")); };
    QString description( ) { return (QString("Handles Textures which are written to mesh properties")); };

  private :

    bool StringToBool(QString _value);

    /// Checks for a correct drawmode and changes if necessary
    void switchDrawMode( TextureType _type, int _id );

    TextureData globalTextures_;

    texturePropertiesWidget* settingsDialog_;

  private slots:
    void applyDialogSettings(TextureData* _texData, QString _textureName, int _id);

    //compute histogram for the given texture property
    void getCoordinates1D(QString _textureName, int _id, std::vector< double >& _x );

  private:

    /// Calls the correct \a copyTexture() function to copy the texture property into the displayed OM property
    template< typename MeshT >
    void doUpdateTexture ( Texture& _texture , MeshT& _mesh);

#ifdef ENABLE_OPENVOLUMEMESH_SUPPORT
    /// Calls the correct \a copyTexture() function to copy the texture property into the displayed OVM property
    template< typename VolumeMeshT, typename VolumeMeshObjectT >
    void doUpdateTextureOVM ( Texture& _texture , VolumeMeshT& _mesh, VolumeMeshObjectT& meshObj);
#endif

    template< typename MeshT >
    void getOriginalHistogram(std::vector< double>& _x, std::vector< double>& _y,
                              int _textureid, MeshT& _mesh,
                              OpenMesh::VPropHandleT< double > _texProp);

    /// Handles data stored in new opened files ( e.g. Texture Information )
    template< typename MeshT >
    void handleFileOpenTextures( MeshT*& _mesh  , int _objectId );

#ifdef ENABLE_OPENVOLUMEMESH_SUPPORT
    /// Handles data stored in new opened files ( e.g. Texture Information )
    template< typename VolumeMeshObjectT >
    void handleFileOpenTexturesOVM( VolumeMeshObjectT* _obj, int _id );
#endif

    /** \brief parse texture mode settings
     * Parses the string _mode and changes the settings in _texture according to the string.
     * If the setting changes, it will return true.
     */
    bool parseMode( QString _mode, Texture& _texture );

    //===========================================================================
    /** @name 1D Texture copy functions( copy 1D input textures to target texture coordinates and transform values )
      * @{ */
    //===========================================================================

    /// Copy the supplied 1D vertex property to both coordinates of the 2D vertex OM texture property
    template< typename MeshT >
    void copyTexture(Texture& _texture , MeshT& _mesh, OpenMesh::VPropHandleT< double > _texProp );

    /// Copy the supplied 1D halfedge property to both coordinates of the 2D halfedge OM texture property
    template< typename MeshT >
    void copyTexture(Texture& _texture , MeshT& _mesh, OpenMesh::HPropHandleT< double > _texProp );

#ifdef ENABLE_OPENVOLUMEMESH_SUPPORT
    /// Copy the supplied 1D vertex property to both coordinates of the 2D vertex OVM texture property
    template< typename VolumeMeshT, typename VolumeMeshObjectT >
    void copyTexture(Texture& _texture , VolumeMeshT& _mesh, VolumeMeshObjectT& _obj, OpenVolumeMesh::VertexPropertyT< double > _texProp );
#endif

    /** @} */

    //===========================================================================
    /** @name 2D Texture copy functions( copy 2D input textures to target texture coordinates and transform values )
      * @{ */
    //===========================================================================


    /// Copy the supplied 2D vertex property to the 2D vertex OM property
    template< typename MeshT >
    void copyTexture(Texture& _texture, MeshT& _mesh, OpenMesh::VPropHandleT< ACG::Vec2d > _texProp );
    /// Copy the supplied 2D halfedge property to the 2D halfedge OM property
    template< typename MeshT >
    void copyTexture(Texture& _texture, MeshT& _mesh, OpenMesh::HPropHandleT< ACG::Vec2d > _texProp );

#ifdef ENABLE_OPENVOLUMEMESH_SUPPORT
    /// Copy the supplied 2D vertex property to the 2D vertex OVM property
    template< typename VolumeMeshT, typename VolumeMeshObjectT >
    void copyTexture(Texture& _texture, VolumeMeshT& _mesh, VolumeMeshObjectT& _obj, OpenVolumeMesh::VertexPropertyT< ACG::Vec2d > _texProp );
#endif

    /** @} */

    //===========================================================================
    /** @name Global texture menu
      * @{ */
    //===========================================================================

  private:
    // Global Texture menu
    QMenu *textureMenu_;

    // Action group for global texture menu
    QActionGroup* actionGroup_;

    // All actions in the global texture menu
    std::vector<QAction*> textureActions_;

    /** @} */

    //===========================================================================
    /** @name Context Menu
      * @{ */
    //===========================================================================

  private slots:

    /** \brief Slot from the context menu interface.
     *
     * This slot is called, when the core requests a context menu and creates and updates
     * the context menu.
     */
    void slotUpdateContextMenu( int _objectId );


    /** \brief Called when the context menu has been triggered
     *
     *  This slot is called when the per object context menu is triggered.
     *  It will switch to the requested texture and mode.
     */
    void slotTextureContextMenu( QAction * _action );

  private:
    /// Stores the per object context menu
    QMenu* contextMenu_;

    /** @} */
    
  public slots:
    QString version() { return QString("1.0"); };
 };

//=============================================================================
#if defined(INCLUDE_TEMPLATES) && !defined(TEXTURECONTROL_1D_TEXTURE_HANDLING_C)
#define TEXTURECONTROL_1D_TEXTURE_HANDLING_TEMPLATES
#include "TextureControl1DTextureHandlingT.cc"
#endif

#if defined(INCLUDE_TEMPLATES) && !defined(TEXTURECONTROL_2D_TEXTURE_HANDLING_C)
#define TEXTURECONTROL_2D_TEXTURE_HANDLING_TEMPLATES
#include "TextureControl2DTextureHandlingT.cc"
#endif

#ifdef ENABLE_OPENVOLUMEMESH_SUPPORT
  #if defined(INCLUDE_TEMPLATES) && !defined(TEXTURECONTROL_1D_TEXTURE_HANDLING_OVM_C)
  #define TEXTURECONTROL_1D_TEXTURE_HANDLING_OVM_TEMPLATES
  #include "TextureControl1DTextureHandlingOVMT.cc"
  #endif

  #if defined(INCLUDE_TEMPLATES) && !defined(TEXTURECONTROL_2D_TEXTURE_HANDLING_OVM_C)
  #define TEXTURECONTROL_2D_TEXTURE_HANDLING_OVM_TEMPLATES
  #include "TextureControl2DTextureHandlingOVMT.cc"
  #endif
#endif

#if defined(INCLUDE_TEMPLATES) && !defined(TEXTURECONTROL_HISTOGRAMS_C)
#define TEXTURECONTROL_HISTOGRAMS_TEMPLATES
#include "TextureControlHistogramsT.cc"
#endif


#endif //TEXTURECONTROLPLUGIN_HH
