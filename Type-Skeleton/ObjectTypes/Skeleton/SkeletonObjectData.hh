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

#ifndef SKELETONOBJECTDATA_HH
#define SKELETONOBJECTDATA_HH

#include "OpenFlipper/common/perObjectData.hh"

#define OBJECTDATA_SKELETON "Skeleton Object Data"

/**
 * @brief Data object attached to the skeleton
 *
 * It stores the object ids of skins attached to this skeleton
 *
 */
class SkeletonObjectData : public PerObjectData
{
public:
  SkeletonObjectData(){
  }

  SkeletonObjectData(const SkeletonObjectData& _copy) :
    PerObjectData(),
    skins_(_copy.skins_)
  {
  }

  ~SkeletonObjectData(){
  }

  // Copy function
  PerObjectData* copyPerObjectData() {
    SkeletonObjectData* copy = new SkeletonObjectData(*this);
    return copy;
  }

public:
  /**
   *  \brief Get the skin with given index (0 <= _index < skinCount())
   */ 
  int skin( unsigned int _index ){
    if ( _index >= skins_.size() )
      return -1;
    return skins_[_index];
  }

  /**
   *  \brief Get the number of associated skins
   */ 
  unsigned int skinCount(){
    return skins_.size();
  }

  /**
   *  \brief Add a skin to the skeleton
   */ 
  void addSkin(int _objectId){
    //check if already available
    for(unsigned int i=0; i < skins_.size(); i++)
      if ( skins_[i] == _objectId )
        return;
    //add the skin
    skins_.push_back(_objectId);
  }

  /**
   *  \brief Remove a skin from the skeleton
   */ 
  void removeSkin(int _objectId){
    //check if already available
    for(unsigned int i=0; i < skins_.size(); i++)
      if ( skins_[i] == _objectId ){
        skins_.erase( skins_.begin() + i );
        return;
      }
    //not found
    std::cerr << "Cannot remove skin with object id:" << _objectId << ". Not found!" << std::endl;
  }

  /**
   *  \brief Remove all skins from the skeleton
   */ 
  void clearSkins(){
    skins_.clear();
  }

private:
  //vector of object ids from all skins attached to this skeleton
  std::vector< int > skins_;
};

#endif //SKELETONOBJECTDATA_HH
