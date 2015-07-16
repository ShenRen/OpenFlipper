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


#ifndef TEXTUREINTERFACE_HH
#define TEXTUREINTERFACE_HH

 #include <OpenFlipper/common/Types.hh>
 
#if QT_VERSION >= 0x050000 
  #include <QtWidgets>
#else
  #include <QtGui>
#endif

 #include <QMenuBar>


 /**
  * \brief Provide texture support for a plugin.
  *
  * This Interface should be used by plugins which will provide a texture. The texture coordinates have
  * to be written into a mesh property. You do not need to write texture information into the texture
  * coordinates of the mesh. As other plugins may provide textures to, the visualization of the
  * textures is handled by the main application (or a texture control plugin).
 */
class TextureInterface {
   signals :

      /** \brief Emit this Signal if a texture has been added (Property Name,filename,Dimension)
       *
       *  Emit this signal if a texture for a specific object has been added
       *  This signal can be called from any thread.\n
       *
       *  @param  _name Name of the property which contains the tex coords (double or vec2d)
       *  @param _filename Filename of the texture (either local in OpenFlippers texture dir or global ( "./Textures/<name>")
       *  @param _dimension 1D texture ( currenty only supports 1 )
       *  @param _id id of an object
       */
      virtual void addTexture( QString _name , QString _filename , uint _dimension , int _id ) {};

      /** \brief Emit this Signal if a texture has been added (Property Name,filename,Dimension)
       *
       *  Emit this signal if a global texture has been added
       *  This signal can be called from any thread.\n
       *
       *  @param  _name Name of the property which contains the tex coords (double or vec2d)
       *  @param _filename Filename of the texture (either local in OpenFlippers texture dir or global ( "./Textures/<name>")
       *  @param _dimension 1D texture ( currenty only supports 1 )
       */
      virtual void addTexture( QString _name , QString _filename , uint _dimension ) {};

      /** \brief Emit this Signal if you want to add a texture for a multitexturing mode
       *
       *  Emit this signal if a texture should be added to a multitexturing mode.
       *
       *  The first parameter defines a texturegroup which collects all textures
       *  that should be painted in the multitexturing mode. This group does not
       *  have to exist on the first call but will be created automatically.
       *
       *  The second parameter defines the single textures name used in the gui.\n
       *  This signal can be called from any thread.\n
       *
       *  @param _textureGroup Multitexturing group using this texture
       *  @param _name         Name of the property which contains the tex coords (double or vec2d)
       *  @param _filename     Filename of the texture (either local in OpenFlippers texture dir or global ( "./Textures/<name>")
       *  @param _id           Id of the object which should use that texture
       *  @param _textureId    The new internal id of the texture( This id is object related!!). Use this id in your mesh as the texture index (Use mesh->set_texture_index on the face using this texture).
       */
      virtual void addMultiTexture( QString _textureGroup , QString _name , QString _filename , int _id , int& _textureId ) {};

      /** \brief Tell Plugins to update the given texture for the given identifier
       * This signal can be called from any thread.\n
       */
      virtual void updateTexture( QString _textureName  , int _identifier) {};

      /** \brief Tell Plugins to update all textures
       */
      virtual void updateAllTextures( ) {};

      /** \brief emit this signal if you updated a texture
       *
       *  Give the name of the texture and the id of the object or -1 if all objects were update
      */
      virtual void updatedTextures( QString , int ) {};

      /** \brief emit this signal if you want to switch the texture of a specific object
       * This signal can be called from any thread.\n
       */
      virtual void switchTexture( QString _textureName , int _id  ) {};

      /** \brief emit this signal if you want to switch the global texture
       * This signal can be called from any thread.\n
       */
      virtual void switchTexture( QString _textureName ) {};

      /** \brief emit this signal if you want to set a special mode for this texture (Clamping,...)
       *
       *  emit this signal if you want to set a special mode for this texture
       *
       * The values are modified in the following way:\n
       *
       * First the absolute value of the property is taken if requested:\n
       * abs=true/false \n
       *
       * Then this value is clamped against the given values:\n
       * clamp=true/false \n
       * clamp_min=value \n
       * clamp_max=value \n
       *
       * If the texture should be repeated, the values are translated such that the minimum is at min_val(0.0 by default)
       * and than scaled such that the maximum is at max_val. This data is than passed to the rendering: \n
       * repeat=true/false \n
       * min_val=value \n
       * max_val=value \n
       *
       * If the texture should not be repeated you could choose to center the data around 0.5. The negative values are than mapped to 0..0.5
       * and the positive values to 0.5..1:\n
       * center=true/false\n
       *
       * Otherwise, the values are mapped directly to 0..1. You can also disable that final scaling with:\n
       * scale=true/false\n
       *
       * Examples:\n
       * Pass texture without modification: abs=false,clamp=false,repeat=false,center=false,scale=false\n
       * This signal can be called from any thread.\n
       *  @param _textureName Name of your Texture
       *  @param _mode colon separated String describing your settings (e.g. clamp,abs )
      */
      virtual void setTextureMode(QString _textureName ,QString _mode) {};

      /** \brief emit this signal if you want to set a special mode for this texture (Clamping,...)
       *
       * for info about the _mode parameter see setTextureMode(QString,QString)
       * This signal can be called from any thread.\n
       *
       *  @param _textureName Name of your Texture
       *  @param _mode colon separated String describing your settings (e.g. clamp,abs )
       *  @param _id id of an object
       */
      virtual void setTextureMode(QString _textureName ,QString _mode, int _id ) {};

      /** \brief Change the texture image of a given texture
       *
       *  @param _textureName The name of the texture which should be changed
       *  @param _image The new image for the texture
       *  @param _id Id of the object where the texture should be changed
       */
      virtual void textureChangeImage( QString _textureName , QImage& _image , int _id ) {};

      /** \brief Change the texture image of a given global texture
       *
       *  @param _textureName The name of the texture which should be changed
       *  @param _image The new image for the texture
       */
      virtual void textureChangeImage( QString _textureName , QImage& _image ) {};

      /** \brief get the texture image of a given texture
       *
       *  @param _textureName The name of the texture which should be addressed
       *  @param _image reference to the image for the texture
       *  @param _id Id of the object where the texture should be fetched from
       */
      virtual void textureGetImage( QString _textureName , QImage& _image , int _id ) {};

      /** \brief Get the texture image of a given global texture
       *
       *  @param _textureName The name of the texture which should be addressed
       *  @param _image reference to the image for the texture
       */
      virtual void textureGetImage( QString _textureName , QImage& _image ) {};

      /** \brief Get the texture index of a given texture
       *
       *  @param _textureName The name of the texture which should be addressed
       *  @param _id Id of the object where the texture is defined on
       *  @param _index the index of the texture (used for multiTexturing)
       */
      virtual void textureIndex( QString _textureName, int _id, int& _index) {};
      
      /** \brief Get the name of the texture index property
       *
       *  Get the name of the property that holds the texture index (face property).
       *
       *  @param _id The id of the mesh object
       *  @param _propertyName The name of the property that holds the texture indices
       */
      virtual void textureIndexPropertyName( int _id, QString& _propertyName) {};
      
       /** \brief get the name of the texture with given texture index
       *
       *  When using multiTexturing you can retrieve the texture index of a face with 'mesh.texture_index(Handle)'
       *  This function maps the texture index to the name of the actual texture that is used to texture
       *  the face. Note that some plugins may change the mesh's internal property name for the texture
       *  index such that mesh.texture_index(Handle) won't return a valid texture index.
       *
       *  @param _id Id of the object where the texture should be fetched from
       *  @param _textureIndex texture index of the wanted texture
       *  @param _textureName this returns the name of the texture
       */
      virtual void textureName( int _id, int _textureIndex, QString& _textureName ) {};
      
      /** \brief get the filename of the texture with given texture index
       *
       *  Get the filename of a given texture with name _textureName. Retrieve the texture's
       *  name via signal textureName(int,int,QString&) first.
       *
       *  @param _id Id of the object where the texture should be fetched from
       *  @param _textureName The name of the texture whose filename will be fetched
       *  @param _textureFilename this returns the name of the texture
       */
      virtual void textureFilename( int _id, QString _textureName, QString& _textureFilename ) {};

       /** \brief get the name of the texture which is currently enabled
       *
       *  @param _id Id of the object where the current texture should be fetched from
       *  @param _textureName this returns the name of the texture
       */
      virtual void getCurrentTexture( int _id, QString& _textureName ) {};
      
       /** \brief get the names of all sub-textures under the given multiTexture
       *
       *  @param _id Id of the object where the current texture should be fetched from
       *  @param _multiTextureName name of a multi texture
       *  @param _subTextures this returns the names of all sub textures that are combined under the given multi texture
       */
      virtual void getSubTextures( int _id, QString _multiTextureName, QStringList& _subTextures ) {};

   private slots :
      /** \brief update the texture with the given Name ( if this plugin provides this texture ) for all meshes
       *
       * This function is called if the texture of the object is about to be shown and the object has changed
       * since the last rendering of the texture.
       *
       * @param _textureName Name of the Texture to be updated
       * @param _identifier The id of the object to update
      */
      virtual void slotUpdateTexture( QString _textureName , int _identifier) {};

      /** \brief update all textures provided by this plugin
       */
      virtual void slotUpdateAllTextures( ) {};

      /** \brief A texture has been updated
       *
       *  A plugin has updated a Texture
       *  @param _textureName The name of the updated texture
       *  @param _identifier -1 if all objects updated, otherwise the identifier of the object
       */
      virtual void slotTextureUpdated( QString _textureName , int _identifier ) {};

      /** \brief This slot is called when a plugin requests to switch an objects texture
       * This signal can be called from any thread.\n
       *
       * @param _textureName Name of the Texture
       * @param _id id of an object
       */
      virtual void slotSwitchTexture( QString _textureName, int _id ) {};

      /** \brief This slot is called when a plugin requests to switch to a different texture mode
       * This signal can be called from any thread.\n
       *
       * @param _textureName Name of the Texture
      */
      virtual void slotSwitchTexture( QString _textureName ) {};

   public :

      /// Destructor
      virtual ~TextureInterface() {};

   //===========================================================================
    /** @name Functions handled by textureControlPlugin
     *
     * Normally you dont have to do anything when these functions are called.
     * Texturecontrol handles visualization and updates of textures.
    * @{ */
   //===========================================================================

  private slots :

      /** \brief A texture has been added by a plugin.
       *
       * This slot is called when a texture for a specific object has been added by a plugin.\n
       * This slot will be executed on the main thread.\n
       *
       * @param _textureName Name of the Added texture (has to be equal to the property name)
       * @param _filename Filename of the Texture Image to be used
       * @param _dimension Dimension of the Texture (currently only 1D and 2D Textures are supported
       * @param _id id of an object
       */
      virtual void slotTextureAdded( QString _textureName , QString _filename , uint _dimension, int _id ) {};

      /** \brief A texture has been added by a plugin.
       *
       * This slot is called when a global texture has been added by a plugin.\n
       * This slot will be executed on the main thread.\n
       *
       * @param _textureName Name of the Added texture (has to be equal to the property name)
       * @param _filename Filename of the Texture Image to be used
       * @param _dimension Dimension of the Texture (currently only 1D and 2D Textures are supported
       */
      virtual void slotTextureAdded( QString _textureName , QString _filename , uint _dimension ) {};

      /** \brief A multiTexture has been added by a plugin.
       *
       * This slot is called when a multi Texture has been added by a plugin.
       *
       * A multi texture has a global name which is defined as the texture group and consists of
       * mutliple sub textures which have their own names but are all used when the group is active.\n
       * This slot will be executed on the main thread.\n
       *
       * @param _textureGroup Name of the texture group that is associated with the texture.
       * @param _name      Name of the Added texture (has to be equal to the property name)
       * @param _filename Filename of the Texture Image to be used
       * @param _id Id of the object which should use the texture
       * @param _textureId    The new id of the texture( This id is object related!!)
       */
      virtual void slotMultiTextureAdded( QString _textureGroup , QString _name ,
                                          QString _filename , int _id , int& _textureId ) {};

      /** \brief Texturemode for texture should be changed
       *
       *  A plugin has updated the Texture settings for a texture
       *  @param _textureName The name of the updated texture
       *  @param _mode New mode flags for the given texture
       *  @param _id id of an object
       */
      virtual void slotSetTextureMode(QString _textureName ,QString _mode, int _id ) {};

      /** \brief Changes the texture image of a given texture
       *
       *  @param _textureName The name of the texture which should be changed
       *  @param _image The new image for the texture
       *  @param _id Id of the object where the texture should be changed
       */
      virtual void slotTextureChangeImage( QString _textureName , QImage& _image , int _id ) {};

      /** \brief Changes the texture image of a given global texture
       *
       *  @param _textureName The name of the texture which should be changed
       *  @param _image The new image for the texture
       */
      virtual void slotTextureChangeImage( QString _textureName , QImage& _image ) {};

      /** \brief Texturemode for texture should be changed
       *
       *  A plugin has updated the Texture settings for a texture
       *  @param _textureName The name of the updated texture
       *  @param _mode New mode flags for the given texture
       */
      virtual void slotSetTextureMode(QString _textureName ,QString _mode) {};
      
      /** \brief fetches the texture image of a given texture
       *
       *  @param _textureName The name of the texture which should be addressed
       *  @param _image reference to the image for the texture
       *  @param _id Id of the object where the texture should be fetched from
       */
      virtual void slotTextureGetImage( QString _textureName , QImage& _image , int _id ) {};

      /** \brief fetches the texture image of a given global texture
       *
       *  @param _textureName The name of the texture which should be addressed
       *  @param _image reference to the image for the texture
       */
      virtual void slotTextureGetImage( QString _textureName , QImage& _image ) {};
      
      /** \brief Get the texture index of a given texture
       *
       *  @param _textureName The name of the texture which should be addressed
       *  @param _id Id of the object where the texture is defined on
       *  @param _index the index of the texture (used for multiTexturing)
       */
      virtual void slotTextureIndex( QString _textureName, int _id, int& _index) {};
      
      /** \brief Get the name of the texture index property
       *
       *  Get the name of the property that holds the texture index (face property).
       *
       *  @param _id The id of the mesh object
       *  @param _propertyName The name of the property that holds the texture indices
       */
      virtual void slotTextureIndexPropertyName( int _id, QString& _propertyName) {};
      
       /** \brief get the name of the texture with given texture index
       *
       *  @param _id Id of the object where the texture should be fetched from
       *  @param _textureIndex texture index of the wanted texture
       *  @param _textureName this returns the name of the texture
       */
      virtual void slotTextureName( int _id, int _textureIndex, QString& _textureName ) {};
      
      /** \brief get the filename of the texture with given texture name
       *
       *  Get the filename to texture with name _textureName. Retrieve texture names
       *  via signal textureName(int,int,QString&) using the object's id and
       *  the texture index.
       *
       *  @param _id Id of the object where the texture should be fetched from
       *  @param _textureName texture name of the wanted texture
       *  @param _textureFilename this returns the filename of the texture
       */
      virtual void slotTextureFilename( int _id, QString _textureName, QString& _textureFilename ) {};
      
      /** \brief get the number of textures per object
       *
       *  @param _id Id of the object
       *  @param _numTextures returns the number of textures
       */
      virtual void slotNumberOfTextures( int _id, int& _numTextures ) {};

       /** \brief fetches the name of the texture which is currently enabled
       *
       *  @param _id Id of the object where the current texture should be fetched from
       *  @param _textureName this returns the name of the texture
       */
      virtual void slotGetCurrentTexture( int _id, QString& _textureName ) {};
      
       /** \brief fetches the names of all sub-textures under the given multiTexture
       *
       *  @param _id Id of the object where the current texture should be fetched from
       *  @param _multiTextureName name of a multi texture
       *  @param _subTextures this returns the names of all sub textures that are combined under the given multi texture
       */
      virtual void slotGetSubTextures( int _id, QString _multiTextureName, QStringList& _subTextures ) {};

   /** @} */
};

Q_DECLARE_INTERFACE(TextureInterface,"OpenFlipper.TextureInterface/1.0")

#endif // TEXTUREINTERFACE_HH
