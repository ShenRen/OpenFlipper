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

#define MULTIINTERPOLATIONANIMATIONT_C

#include "AnimationT.hh"
#include <algorithm>

//-----------------------------------------------------------------------------------------------------

/** \brief Copy constructor
 *
 * @param _other The animation to copy from
 */
template<class PointT>
MultiInterpolationAnimationT<PointT>::MultiInterpolationAnimationT(const MultiInterpolationAnimationT<PointT> &_other) :
        InterpolationAnimationT<PointT>(NULL, NULL),
        interpolationAnimations_(_other.interpolationAnimations_)
{
}

//-----------------------------------------------------------------------------------------------------

template<class PointT>
AnimationT<PointT>* MultiInterpolationAnimationT<PointT>::copy() {
  return new MultiInterpolationAnimationT<PointT>(*this);
}

//-----------------------------------------------------------------------------------------------------

template<class PointT>
bool MultiInterpolationAnimationT<PointT>::getMinInput(Scalar& _result) {
  if (interpolationAnimations_.size() == 0)
    return false;
  else
    interpolationAnimations_[0]->getMinInput(_result);
  
  for (uint i=0;i<interpolationAnimations_.size();++i) {
    Scalar currentInput;
    interpolationAnimations_[i]->getMinInput(currentInput);
    
    if (currentInput < _result)
      _result = currentInput;
  }
  
  return true;
}

//-----------------------------------------------------------------------------------------------------

template<class PointT>
bool MultiInterpolationAnimationT<PointT>::getMaxInput(Scalar& _result) {
  if (interpolationAnimations_.size() == 0)
    return false;
  else
    interpolationAnimations_[0]->getMaxInput(_result);
  
  for (uint i=0;i<interpolationAnimations_.size();++i) {
    Scalar currentInput;
    interpolationAnimations_[i]->getMaxInput(currentInput);
    
    if (currentInput > _result)
      _result = currentInput;;
  }
  
  return true;
}

//-----------------------------------------------------------------------------------------------------

/** \brief Returns the number of frames that this animation can playback
 *
 * Note that this is not simply the sum of all animations' frame counts, as they can (and most likely will) overlap.
 */
template<class PointT>
unsigned int MultiInterpolationAnimationT<PointT>::frameCount()
{
  Scalar minInput=0, maxInput=0;
  if (getMinInput(minInput) && getMaxInput(maxInput)) {
    return ((maxInput - minInput) * FPS);
  }
  
  return 0;
}

//-----------------------------------------------------------------------------------------------------

template<class PointT>
PoseT<PointT> * MultiInterpolationAnimationT<PointT>::pose(unsigned int _iFrame) {
  //Use the reference pose of the first (in terms of the input value, i.e. the time in most cases)
 
  if (interpolationAnimations_.size() == 0)
    return NULL;
 
  Scalar minValue=0; uint minInterpolationAnimationIndex = 0;
  for (uint i=0; i<interpolationAnimations_.size(); ++i) {
    Scalar currentValue;
    interpolationAnimations_[i]->getMinInput(currentValue);
    Scalar minValueTmp = std::min(minValue, currentValue);
    minInterpolationAnimationIndex = (minValueTmp < minValue) ? i : minInterpolationAnimationIndex;
  }

  return pose(_iFrame, interpolationAnimations_[minValue]->getReference());
}

//-----------------------------------------------------------------------------------------------------

template<class PointT>
PoseT<PointT> * MultiInterpolationAnimationT<PointT>::pose(unsigned int _iFrame, Pose* _reference) {
  if (_iFrame == 0)
    return _reference;
  
  Pose* newPose = NULL;
  Pose* referenceCopy = new Pose(*_reference);
  
  for (uint i=0; i<interpolationAnimations_.size(); ++i) {
    Scalar minInput, maxInput;
    interpolationAnimations_[i]->getMinInput(minInput); interpolationAnimations_[i]->getMaxInput(maxInput);
    
    unsigned int minFrame = (minInput * FPS);
    unsigned int maxFrame = (maxInput * FPS);
    
    //Check, if the current animation is responsible for displaying this frame
    if (_iFrame < minFrame || _iFrame > maxFrame)
      continue;
    
    if (interpolationAnimations_[i]) {
      if (newPose == NULL)
        newPose = interpolationAnimations_[i]->pose(_iFrame - minFrame, referenceCopy);
      else
        newPose = interpolationAnimations_[i]->pose(_iFrame - minFrame, newPose);
    }
  }
  
  delete referenceCopy;
  
  if (newPose == NULL)
    newPose = _reference;
  
  return newPose;
}

//-----------------------------------------------------------------------------------------------------

template<class PointT>
InterpolationAnimationT<PointT>* MultiInterpolationAnimationT<PointT>::animation(unsigned int _index ) {
  if ( _index < interpolationAnimations_.size() )
    return interpolationAnimations_[ _index ];
  else 
    return 0;
}

//-----------------------------------------------------------------------------------------------------

