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

#include "ImageStorage.hh"

#include <OpenFlipper/common/GlobalOptions.hh>

static ImageStore store;

ImageStore& imageStore() {
  return store;
}

// ---------------------------------------------------------------------------

ImageStore::ImageStore():
    nextId_(1)
{
}

// ---------------------------------------------------------------------------

int ImageStore::addImageFile( QString _fileName ) {

  // Clean error string
  errorString_ = "No error";

  QString loadFilename;

  // Construct complete filename
  // if it starts with "/" or "." or contains a ":" it is a global filename and we don't touch it
  // otherwise we take a relative path from our texture dir
  if ( _fileName.startsWith("/") || _fileName.startsWith(".") || _fileName.contains(":") )
    loadFilename = _fileName;
  else
    loadFilename = OpenFlipper::Options::textureDirStr() + QDir::separator() + _fileName;

  // Check if it already exists and return the existing one rather than recreating a new one
  QMap< QString,QPair<int,QDateTime> >::const_iterator it = filenameMap_.find(loadFilename);
  QFileInfo fileInfo(loadFilename);
  if ( it != filenameMap_.end() && it->second == fileInfo.lastModified()) {

    // Get id of the existing image
    int existingId = filenameMap_[loadFilename].first;

    // Update refcount
    refCount_[existingId] = refCount_[existingId] + 1;

    // Return existing ones id
    return existingId;
  }

  if ( !fileInfo.exists() ) {
    errorString_ =  "addedEmptyObject: Cannot load global texture '"+ loadFilename +"'. File not found!" ;
    return -1;

  }

  QImage image;

  //QImage cannot handle tga directly
  if ( fileInfo.suffix().toLower() == "tga" ){

    QPixmap pic(loadFilename);
    image = pic.toImage();

  } else {
    //load the image
    if ( !image.load( loadFilename ) ){
      errorString_ = "Unable to load Image from file " + loadFilename;
      return -1;
    }
  }

  // Store the mapping from filename to id
  filenameMap_[loadFilename]= qMakePair(nextId_, fileInfo.lastModified());
  reverseFilenameMap_[nextId_] = loadFilename;

  // Initialize refcount
  refCount_[nextId_] = 1;

  // Store image
  imageMap_[nextId_] = image;

  // Increment for next image
  nextId_++;

  // return the id of the added one
  return (nextId_ - 1);


}

// ---------------------------------------------------------------------------

int  ImageStore::addImage( QImage _image ) {

  // No filename mapping as it does not have a filename

  // Initialize refcount
  refCount_[nextId_] = 1;

  // Store image
  imageMap_[nextId_] = _image;

  // Increment for next image
  nextId_++;

  // return the id of the added one
  return (nextId_ - 1);
}

// ---------------------------------------------------------------------------

int  ImageStore::getImageID(QString _filename) {

  // Check if it already exists and return the existing one rather than recreating a new one
  if ( filenameMap_.contains(_filename)) {
    return filenameMap_[_filename].first;
  }

  // Clean error string
  errorString_ = "getImageID failed. No such image :" + _filename;

  return -1;

}

// ---------------------------------------------------------------------------

QImage& ImageStore::getImage(int _id, bool* _ok) {

  if ( imageMap_.contains(_id)) {
    if ( _ok)
      *_ok = true;
    return imageMap_[_id];
  } else {
    if ( _ok)
      *_ok = false;
    errorString_ = "getImage: Unknown image id : " + QString::number(_id);
    return dummy_;
  }
}

// ---------------------------------------------------------------------------

void ImageStore::removeImage(int _id) {

  // Check if it exists
  if ( refCount_.contains(_id)) {
    if ( refCount_[_id] > 1 ) {
      refCount_[_id] = refCount_[_id] - 1;
      return;
    } else {

      // Remove from refCount
      refCount_.remove(_id);
      imageMap_.remove(_id);

      // This image is from a file and not directly added
      if ( reverseFilenameMap_.contains(_id))  {

        // Get the filename
        QString fileName = reverseFilenameMap_[_id];

        reverseFilenameMap_.remove(_id);
        filenameMap_.remove(fileName);

      }

    }
  }
}














