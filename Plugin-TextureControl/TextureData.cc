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

#include "TextureData.hh"
#include <iostream>

//-----------------------------------------------------------------------------------

Texture::Texture() :
  name_("No Texture"),
  textureImageId_(0),
  visibleName_(""),
  filename_("Invalid"),
  id_(-1),
  glName_(0),
  dimension_(0),
  enabled_(false),
  hidden_(false),
  dirty_(true),
  type_(UNSET),
  indexMappingProperty_("f:textureindex")
{

}

TextureData::TextureData() :
  nextInternalID_(1)
{
  // map 0 to no texture
  textureMap_[0]  = 0;
  propertyMap_[0] = "No Texture";
}

//-----------------------------------------------------------------------------------

TextureData::~TextureData() {

}

//-----------------------------------------------------------------------------------

/** \brief Check if a texture exists
 *
 * @param _textureName name of the texture
 * @return returns wether the texture exists
 */
bool TextureData::textureExists(QString _textureName)
{
  return ( getTextureIndex(_textureName) != -1);
}

//-----------------------------------------------------------------------------------

/** \brief Check if a texture is enabled
 *
 * @param _textureName name of the texture
 * @return returns wether the texture is enabled
 */
bool TextureData::isEnabled(QString _textureName)
{
  int id = getTextureIndex(_textureName);

  if ( id != -1)
    return textures_[id].enabled();
  else
    return false;
}

//-----------------------------------------------------------------------------------

/** \brief Enable a given texture
 *
 * @param _textureName name of the texture
 * @param _exclusive disable other textures?
 */
bool TextureData::enableTexture(QString _textureName, bool _exclusive)
{
  int id = getTextureIndex(_textureName);

  if ( id != -1){

    textures_[id].enable();

    //disable other textures
    if (_exclusive)
      for ( int i = 0 ; i < (int)textures_.size() ; ++i )
        if (i != id)
          textures_[i].disable();
    return true;
  }
  return false;
}

//-----------------------------------------------------------------------------------

/** \brief Disable a given texture
 *
 * @param _textureName name of the texture
 */
void TextureData::disableTexture(QString _textureName)
{
  int id = getTextureIndex(_textureName);

  if ( id != -1)
    textures_[id].disable();
}

//-----------------------------------------------------------------------------------

/** \brief Add a Texture
 *
 * @param _textureName name of the texture
 * @param _filename filename of the texture
 * @param _dimension dimension of the texture
 * @param _glName glName for the texture
 * @return persistent id of the texture
 */
int TextureData::addTexture(QString _textureName, QString _filename, uint _dimension, GLuint _glName)
{
  //generate texture object
  Texture tex;
  tex.id( nextInternalID_++ );
  tex.name( _textureName );
  tex.glName( _glName );
  tex.filename( _filename );
  tex.dimension(_dimension);
  tex.enable();
  tex.setDirty();
  tex.type( VERTEXBASED );
  tex.hidden(false);
//   tex.parameters = TexParameters;

  textures_.push_back( tex );

  textureMap_[ tex.id() ]  = tex.glName();
  propertyMap_[ tex.id() ] = tex.name().toStdString();

  return tex.id();
}

int TextureData::addTexture ( Texture _texture, GLuint _glName ) {
  _texture.id( nextInternalID_++ );
  _texture.glName( _glName );
  textures_.push_back(_texture);

  textureMap_[ _texture.id() ]  = _texture.glName();
  propertyMap_[ _texture.id() ] = _texture.name().toStdString();

  return _texture.id();
}

bool TextureData::addMultiTexture( QString _textureName ) {
  int textureid = getTextureIndex(_textureName);

  if ( textureid != -1) {
    std::cerr << "Texture exists!" << std::endl;
    return false;
  }

  //generate texture object
  Texture tex;
  tex.id( nextInternalID_++ );
  tex.name( _textureName );
  tex.filename("MultiTexture");
  tex.setDirty();
  tex.type(MULTITEXTURE);
  tex.hidden(false);

  textures_.push_back( tex );

  return true;
}

/// Stores the given image in the texture information
bool TextureData::setImage( QString _textureName , int _id ) {
  int textureid = getTextureIndex(_textureName);

  if ( textureid == -1) {
    std::cerr << "setImage: Unknown Texture!" << std::endl;
    return false;
  }

  textures_[textureid].textureImageId(_id);
  return true;
}

//-----------------------------------------------------------------------------------

// void TextureData::deleteTexture(QString _textureName)
// {
//   int index = getTextureIndex(_textureName);
//
//   if ( index != -1){
//
//
//     textureMap_.erase( texture(_textureName).id );
//     propertyMap_.erase( texture(_textureName).id );
//     textures_.erase(textures_.begin()+index);
//   }
// }

//-----------------------------------------------------------------------------------

// TexParameters TextureData::textureParameters(QString _textureName)
// {
//   int id = getTextureIndex(_textureName);
//
//   if ( id != -1)
//     return textures_[id].parameters;
//   else
//     return TexParameters();
// }

//-----------------------------------------------------------------------------------

// void TextureData::setTextureParameters(QString _textureName, TexParameters _params)
// {
//   int id = getTextureIndex(_textureName);
//
//   if ( id != -1)
//     textures_[id].parameters = _params;
// }

//-----------------------------------------------------------------------------------

/** \brief get texture object of a given texture
 *
 * @param _textureName name of the texture
 * @return corresponding texture object
 */
Texture& TextureData::texture(QString _textureName)
{
  int id = getTextureIndex(_textureName);

  if ( id != -1)
    return textures_[id];
  else {
    std::cerr << "Invalid Texture" << _textureName.toStdString() << std::endl;
    return noTexture;
  }
}

//-----------------------------------------------------------------------------------

/** \brief Get the index of a given texture
 *
 * @param _textureName name of the texture
 * @return index in texture vector or -1 if not found
 */
int TextureData::getTextureIndex(QString _textureName)
{
  // Search the list of textures if we have the texture
  int textureid = -1;
  for ( int i = 0 ; i < (int)textures_.size() ; ++i ) {
    if ( (textures_[i].name() == _textureName) || (textures_[i].visibleName() == _textureName) ) {
      textureid = i;
      break;
    }
  }

  return textureid;
}

//-----------------------------------------------------------------------------------

/** \brief Get reference to the texture vector
 *
 * @return texture vector
 */
std::vector< Texture >& TextureData::textures(){
  return textures_;
}

//-----------------------------------------------------------------------------------

/** \brief Get pointer to the textureMap
 *
 * @return textureMap
 */
std::map< int, GLuint>* TextureData::textureMap(){
  return &textureMap_;
}

//-----------------------------------------------------------------------------------

/** \brief Get pointer to the propertyMap
 *
 * @return propertyMap
 */
std::map< int, std::string>* TextureData::propertyMap(){
  return &propertyMap_;
}

