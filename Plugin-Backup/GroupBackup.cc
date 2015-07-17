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

#include "GroupBackup.hh"

#include <algorithm>

#include <OpenFlipper/common/BackupData.hh>
#include <OpenFlipper/BasePlugin/PluginFunctions.hh>

//-----------------------------------------------------------------------------

GroupBackup::GroupBackup(IdList _objectIDs, QString _name)
  : BaseBackup(_name),
    objectIDs_(_objectIDs)
{
//   std::cerr << "Create GroupBackup with name:" << name_.toStdString() << "(id : " << id_ << ")" << std::endl;
  
  //store the backup ids of all objects
  for (unsigned int i=0; i < objectIDs_.size(); ++i ){
    BaseObjectData* object = 0;
    PluginFunctions::getObject(objectIDs_[i], object);
    int id = -1;

    if ( object != 0 ){

      //get backup object data
      BackupData* backupData = 0;

      if ( object->hasObjectData( OBJECT_BACKUPS ) ){
        backupData = dynamic_cast< BackupData* >(object->objectData(OBJECT_BACKUPS));
        id = backupData->currentID();

        //add links so that object backups know that they have to be applied together
        if ( objectIDs_.size() > 1 )
          backupData->setLinks(_objectIDs);
      }
    }
    backupIDs_.push_back(id);
  }
}

//-----------------------------------------------------------------------------

GroupBackup::~GroupBackup(){
//   std::cerr << "Delete GroupBackup with name:" << name_.toStdString() << "(id : " << id_ << ")" << std::endl;
}

//-----------------------------------------------------------------------------

void GroupBackup::apply(){

}

//-----------------------------------------------------------------------------

const IdList& GroupBackup::objectIDs() const{
  return objectIDs_;
}

//-----------------------------------------------------------------------------

bool GroupBackup::contains( int _objectid ) const{
  return std::find(objectIDs_.begin(), objectIDs_.end(), _objectid) != objectIDs_.end();
}

//-----------------------------------------------------------------------------
