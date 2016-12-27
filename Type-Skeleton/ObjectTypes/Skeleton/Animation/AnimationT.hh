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

#ifndef ANIMATIONT_HH
#define ANIMATIONT_HH

#include <vector>
#include <map>

#include "../PoseT.hh"
#include "AnimationHandle.hh"

/**
 * @brief Stores a single animation
 *
 * This class is used by a skeleton to store a single animation. An animation is an array of different Poses.
 * Both the reference pose and the skeleton hierarchy are not used by the animation class itself, but are
 * passed to the poses.
 */
template<class PointT>
class AnimationT
{
  template<typename>
  friend class SkeletonT;

public:
  typedef PointT                                Point;
  typedef typename Point::value_type            Scalar;
  typedef PoseT<PointT>                         Pose;

public:
  AnimationT(std::string _name = "") : name_(_name), fps_(60) {}
  virtual AnimationT* copy() = 0;
  virtual ~AnimationT() {}

public:
  /**
    * @name Frame access
    * There is one pose per frame.
    */
  //@{
  virtual Pose *pose(unsigned int _iFrame) = 0;
  virtual unsigned int frameCount() = 0;
  //@}

  /**
    * @name Synchronization
    * Use these methods to keep the poses in sync with the number (and indices) of the joints.
    */
  //@{
  virtual void insertJointAt(unsigned int _index) = 0;
  virtual void removeJointAt(unsigned int _index) = 0;
  //@}

  virtual void updateFromGlobal(unsigned int _index) = 0;
  virtual void clearPoseCache() {}

  inline std::string name() {
    return name_;
  }

  inline void setName(std::string _name) {
    name_ = _name;
  }

  inline int fps() {
    return fps_;
  }

  inline void setFps(int _fps) {
    fps_ = _fps;
  }

protected:
  std::string name_;
  int fps_;
};


#endif
