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

#include "BackupData.hh"

//-----------------------------------------------------------------------------

BackupData::BackupData(BaseObjectData* _object) : currentState_(0), object_(_object), maxBackups_(20)
{
}

//-----------------------------------------------------------------------------

BackupData::~BackupData(){
  clear();
}

//-----------------------------------------------------------------------------

size_t BackupData::maxBackups()
{
  return maxBackups_;
}

//-----------------------------------------------------------------------------

void BackupData::setMaxBackups(size_t _max)
{
  maxBackups_ = _max;
}

//-----------------------------------------------------------------------------

void BackupData::storeBackup(BaseBackup* _backup){

  //store state
  if ( currentState_ != 0 )
    undoStates_.push_back( currentState_ );

  currentState_ = _backup;
  
  //clear redo states
  while( !redoStates_.empty() ){
    delete redoStates_.back();
    redoStates_.pop_back();
  }

  //delete undo backups if there are too many
  while( undoStates_.size() > maxBackups_ ){
    delete undoStates_.front();
    undoStates_.erase( undoStates_.begin() );
  }
}

//-----------------------------------------------------------------------------

void BackupData::undo(){

  if ( undoStates_.empty() )
    return;

  //get backup
  BaseBackup* backup = undoStates_.back();
  
  //apply
  backup->apply();
  
  // update current state
  undoStates_.pop_back();
  redoStates_.push_back( currentState_ );
  currentState_ = backup;
}

//-----------------------------------------------------------------------------

void BackupData::redo(){

  if ( redoStates_.empty() )
    return;

  //get backup
  BaseBackup* backup = redoStates_.back();
  
  //apply
  backup->apply();
  
  // update current state
  redoStates_.pop_back();
  undoStates_.push_back( currentState_ );
  currentState_ = backup;
}

//-----------------------------------------------------------------------------

QString BackupData::undoName(){

  if ( undoStates_.empty() )
    return QString();

  return currentState_->name();
}

//-----------------------------------------------------------------------------

QString BackupData::redoName(){

  if ( redoStates_.empty() )
    return QString();

  return redoStates_.back()->name();
}

//-----------------------------------------------------------------------------

int BackupData::undoID(){

  if ( undoStates_.empty() )
    return -1;

  return undoStates_.back()->id();
}

//-----------------------------------------------------------------------------

int BackupData::redoID(){

  if ( redoStates_.empty() )
    return -1;

  return redoStates_.back()->id();
}

//-----------------------------------------------------------------------------

int BackupData::currentID(){

  if ( currentState_ == 0 )
    return -1;
  else
    return currentState_->id();
}

//-----------------------------------------------------------------------------

bool BackupData::undoAvailable(){
  return !undoStates_.empty();
}

//-----------------------------------------------------------------------------

bool BackupData::redoAvailable(){
  return !redoStates_.empty();
}

//-----------------------------------------------------------------------------

bool BackupData::undoBlocked(){

  if( !undoStates_.empty() )
    return currentState_->blocked();
  else
    return false;
}

//-----------------------------------------------------------------------------

bool BackupData::redoBlocked(){

  if( !redoStates_.empty() )
    return redoStates_.back()->blocked();
  else
    return false;
}
//-----------------------------------------------------------------------------

void BackupData::clear(){

  while( !undoStates_.empty() ){
    delete undoStates_.back();
    undoStates_.pop_back();
  }

  while( !redoStates_.empty() ){
    delete redoStates_.back();
    redoStates_.pop_back();
  }

  if ( currentState_ != 0 )
    delete currentState_;
  currentState_ = 0;
}

//-----------------------------------------------------------------------------

void BackupData::setLinks(IdList _objectIDs){

  if ( currentState_ != 0 )
    currentState_->setLinks(_objectIDs);
}

//-----------------------------------------------------------------------------

BaseBackup* BackupData::currentState(){
  return currentState_;
}

//-----------------------------------------------------------------------------
