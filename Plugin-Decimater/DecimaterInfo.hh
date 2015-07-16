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

#include <OpenFlipper/common/perObjectData.hh>
#include <OpenFlipper/common/Types.hh>

class DecimaterInfo : public PerObjectData
{

  public :
  
  //Konstruktor
  DecimaterInfo();

  //Destruktor
  ~DecimaterInfo();
  
  // Copy function
  PerObjectData* copyPerObjectData();
  
  enum DecimationOrder { DISTANCE, NORMALDEV, EDGELENGTH };

  void setDistanceConstraint( double _value );
  void setNormalDeviationConstraint( int _value );
  void setNormalFlippingConstraint();
  void setRoundnessConstraint( double _value );
  void setAspectRatioConstraint( double _value );
  void setEdgeLengthConstraint( double _value );
  void setIndependentSetsConstraint();
  void setDecimationOrder( DecimationOrder _order );
  
  void removeDistanceConstraint();
  void removeNormalDeviationConstraint();
  void removeNormalFlippingConstraint();
  void removeRoundnessConstraint();
  void removeAspectRatioConstraint();
  void removeEdgeLengthConstraint();
  void removeIndependentSetsConstraint();
  
  // Get/Set methods
  bool normalDeviation()    { return normalDeviation_; }
  bool normalFlipping()     { return normalFlipping_; }
  bool distance()           { return distance_; }
  bool roundness()          { return roundness_; }
  bool aspectRatio()        { return aspectRatio_; }
  bool edgeLength()         { return edgeLength_; }
  bool independentSets()    { return independentSets_; }
  
  int normalDeviationValue()    { return normalDeviation_value_; }
  double distanceValue()        { return distance_value_; }
  double edgeLengthValue()      { return edgeLength_value_; }
  double aspectRatioValue()     { return aspectRatio_value_; }
  double roundnessValue()       { return roundness_value_; }

private :


  bool normalDeviation_;
  bool normalFlipping_;
  bool distance_;
  bool edgeLength_;
  bool aspectRatio_;
  bool roundness_;
  bool independentSets_;
  
  DecimationOrder decimationOrder_;
  int normalDeviation_value_;
  double distance_value_;
  double edgeLength_value_;
  double aspectRatio_value_;
  double roundness_value_;
};

