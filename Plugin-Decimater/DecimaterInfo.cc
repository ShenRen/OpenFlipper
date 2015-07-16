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

#include "DecimaterInfo.hh"

DecimaterInfo::DecimaterInfo() :
  normalDeviation_(false),
  normalFlipping_(false),
  distance_(false),
  edgeLength_(false),
  aspectRatio_(false),
  roundness_(false),
  independentSets_(false),
  decimationOrder_(DISTANCE),
  normalDeviation_value_(0),
  distance_value_(0),
  edgeLength_value_(0),
  aspectRatio_value_(0),
  roundness_value_(0) {}


DecimaterInfo::~DecimaterInfo() {
}

PerObjectData* DecimaterInfo::copyPerObjectData() {
    
    DecimaterInfo* di_copy = new DecimaterInfo(*this);
    
    return di_copy;
}

//-----------------------------------------------------------------------------------

void DecimaterInfo::setDistanceConstraint( double _value ){

  distance_ = true;
  distance_value_ = _value;
}


//-----------------------------------------------------------------------------------

void DecimaterInfo::setNormalDeviationConstraint( int _value ){

  normalDeviation_ = true;
  normalDeviation_value_ = _value;
}


//-----------------------------------------------------------------------------------

void DecimaterInfo::setNormalFlippingConstraint(){

  normalFlipping_ = true;
}


//-----------------------------------------------------------------------------------

void DecimaterInfo::setIndependentSetsConstraint(){

  independentSets_ = true;
}


//-----------------------------------------------------------------------------------

void DecimaterInfo::setRoundnessConstraint( double _value ){

  roundness_ = true;
  roundness_value_ = _value;
}

//-----------------------------------------------------------------------------------

void DecimaterInfo::setAspectRatioConstraint( double _value ){

  aspectRatio_ = true;
  aspectRatio_value_ = _value;
}

//-----------------------------------------------------------------------------------

void DecimaterInfo::setEdgeLengthConstraint( double _value ){

  edgeLength_ = true;
  edgeLength_value_ = _value;
}

//-----------------------------------------------------------------------------------

void DecimaterInfo::setDecimationOrder( DecimationOrder _order ){

  decimationOrder_ = _order;
}

//-----------------------------------------------------------------------------------

void DecimaterInfo::removeDistanceConstraint(){

  if (  distance_ ) {
    distance_ = false;
    distance_value_ = 0;
  }
}


//-----------------------------------------------------------------------------------

void DecimaterInfo::removeEdgeLengthConstraint(){

  if (  edgeLength_ ) {
    edgeLength_ = false;
    edgeLength_value_ = 0;
  }
}


//-----------------------------------------------------------------------------------

void DecimaterInfo::removeNormalDeviationConstraint(){

  if ( normalDeviation_ ) {
    normalDeviation_ = false;
    normalDeviation_value_ = 0;
  }
}


//-----------------------------------------------------------------------------------

void DecimaterInfo::removeNormalFlippingConstraint(){

  if ( normalFlipping_ ) {
    normalFlipping_ = false;
  }
}


//-----------------------------------------------------------------------------------

void DecimaterInfo::removeIndependentSetsConstraint(){

  if ( independentSets_ ) {
    independentSets_ = false;
  }
}


//-----------------------------------------------------------------------------------

void DecimaterInfo::removeAspectRatioConstraint(){

  if ( aspectRatio_ ) {
    aspectRatio_ = false;
    aspectRatio_value_ = 0;
  }
}


//-----------------------------------------------------------------------------------

void DecimaterInfo::removeRoundnessConstraint(){

  if ( roundness_ ) {
    roundness_ = false;
    roundness_value_ = 0;
  }
}


