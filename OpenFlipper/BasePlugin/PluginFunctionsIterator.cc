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




//=============================================================================
//
//  Plugin Functions
//
//=============================================================================

#include <OpenFlipper/common/Types.hh>

#include "PluginFunctions.hh"

namespace PluginFunctions {


ObjectIterator::ObjectIterator( IteratorRestriction _restriction , DataType _dataType):
    pos_(0),
    dataType_(_dataType),
    restriction_(_restriction)
{


  // Start at the root Node
  BaseObject* currentPos = objectRoot();

  // Take the first element which is an baseObjectData
  proceedToNextBaseObjectData(currentPos);

  while ( (currentPos != objectRoot()) ) {

    // Return only target objects if requested
    if ( (restriction_ == TARGET_OBJECTS) && (! currentPos->target() ) ) {
      proceedToNextBaseObjectData(currentPos);
      continue;
    }

    // Return only source objects if requested
    if ( (restriction_ == SOURCE_OBJECTS) && (! currentPos->source() ) ) {
      proceedToNextBaseObjectData(currentPos);
      continue;
    }

    // Return only the right dataType
    if ( _dataType != DATA_ALL )
      if ( ! (currentPos->dataType( dataType_ ) ) ) {
        proceedToNextBaseObjectData(currentPos);
        continue;
      }

    // found a valid object
    pos_ = dynamic_cast<BaseObjectData* > (currentPos);
    break;
  }
}

ObjectIterator::ObjectIterator(BaseObjectData* pos, IteratorRestriction _restriction , DataType _data) :
        pos_(pos),
        dataType_(_data),
        restriction_(_restriction)
{
};


bool ObjectIterator::operator==( const ObjectIterator& _rhs) const {
   return ( _rhs.pos_ == pos_ );
}

bool ObjectIterator::operator!=( const ObjectIterator& _rhs) const {
   return ( _rhs.pos_ != pos_ );
}

ObjectIterator& ObjectIterator::operator=( const ObjectIterator& _rhs) {
   pos_         = _rhs.pos_;
   dataType_    = _rhs.dataType_;
   restriction_ = _rhs.restriction_;
   return *this;
}


ObjectIterator::pointer ObjectIterator::operator->(){
   return pos_;
}

ObjectIterator& ObjectIterator::operator++() {

  // Convert our pointer to the basic one
  BaseObject* currentPos = dynamic_cast< BaseObject* >(pos_);

  // Get the next objectData element in the tree
  proceedToNextBaseObjectData(currentPos);

  while ( (currentPos != objectRoot() ) ) {

    // Return only target objects if requested
    if ( (restriction_ == TARGET_OBJECTS) && (! currentPos->target() ) ) {
      proceedToNextBaseObjectData(currentPos);
      continue;
    }

    // Return only source objects if requested
    if ( (restriction_ == SOURCE_OBJECTS) && (! currentPos->source() ) ) {
      proceedToNextBaseObjectData(currentPos);
      continue;
    }

    // Return only the right dataType
    if ( ! (currentPos->dataType( dataType_ ) ) ) {
      proceedToNextBaseObjectData(currentPos);
      continue;
    }

    // found a valid object
    pos_ = dynamic_cast<BaseObjectData* > (currentPos);
    return *this;
  }

  // No valid object found
  pos_ = 0;
  return *this;
}

ObjectIterator& ObjectIterator::operator--() {
   std::cerr << "TODO :--" << std::endl;
   return *this;
}

/** This operator returns a pointer to the current object the iterator
 * points to.
 *
 * @return Pointer to the current ObjectData
 */
BaseObjectData* ObjectIterator::operator*() {
   return pos_;
}

/// Return Iterator to Object End
ObjectIterator objectsEnd() {
   return ObjectIterator(0);
}

void ObjectIterator::proceedToNextBaseObjectData(BaseObject*& _object) {

  _object = _object->next();

  // Go through the tree and stop at the root node or if we found a baseObjectData Object
  while ( (_object != objectRoot()) && !dynamic_cast<BaseObjectData* > (_object)  )
     _object = _object->next();
}



}
