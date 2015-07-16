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

#define FRAMEANIMATIONT_C

#include <vector>
#include <cassert>

//-----------------------------------------------------------------------------

/**
 * @brief Constructor - Creates a new animation consisting of a single pose
 *
 * The animation will hold a single frame, made up by a copy of the given pose. After this call returns the
 * pose given by \e _pose is no longer needed and independent from this instance.
 *
 * You can use this operation to clone the reference pose as new instance, then modify the pose.
 *
 * @param _pose This pose will make up the only frame in this new animation
 */
template<class PointT>
FrameAnimationT<PointT>::FrameAnimationT(const PoseT<PointT> &_pose) :
  skeleton_(_pose.skeleton_)
{
  poses_.push_back(new Pose(_pose));
}

//-----------------------------------------------------------------------------

/** \brief Constructor - Creates a new empty animation
 *
 * @param _skeleton The skeleton that will hold this animation
 */
template<class PointT>
FrameAnimationT<PointT>::FrameAnimationT(Skeleton* _skeleton) :
  skeleton_(_skeleton)
{
}

//-----------------------------------------------------------------------------

/**
 * @brief Constructor - Creates a new animation with the given number of frames
 *
 * The poses in the given number of frames will all hold identity matrices for all joints. Make sure you write
 * data to the poses before you use it.
 *
 * @param _skeleton    The skeleton that will hold this animation
 * @param _iNumFrames  The number of frames for this animation
 */
template<class PointT>
FrameAnimationT<PointT>::FrameAnimationT(Skeleton* _skeleton, unsigned int _iNumFrames) :
  skeleton_(_skeleton)
{
  for(unsigned int i = 0; i < _iNumFrames; ++i)
    poses_.push_back(new Pose(_skeleton));
}

//-----------------------------------------------------------------------------

/**
 * @brief Copy constructor
 *
 * This animation will copy all frames from the given animation. After the call returns they are completely
 * independent.
 *
 * @param _other The animation to copy from
 */
template<class PointT>
FrameAnimationT<PointT>::FrameAnimationT(const FrameAnimationT<PointT> &_other) :
  AnimationT<PointT>(),
  skeleton_(_other.skeleton_)
{
  for(typename std::vector<Pose*>::const_iterator it = _other.poses_.begin(); it != _other.poses_.end(); ++it)
    poses_.push_back(new Pose(**it));
}

//-----------------------------------------------------------------------------

/**
 * @brief Destructor
 */
template<class PointT>
FrameAnimationT<PointT>::~FrameAnimationT()
{
  for(typename std::vector<Pose*>::iterator it = poses_.begin(); it != poses_.end(); ++it)
    delete *it;
  poses_.clear();
}

//-----------------------------------------------------------------------------

/**
 * @brief Copy Function
 */
template<class PointT>
AnimationT<PointT>* FrameAnimationT<PointT>::copy() {
  return new FrameAnimationT<PointT>(*this);
}

//-----------------------------------------------------------------------------


template<class PointT>
typename FrameAnimationT<PointT>::Pose *FrameAnimationT<PointT>::pose(unsigned int _iFrame)
{
  assert(_iFrame < poses_.size());

  return poses_[_iFrame];
}

//-----------------------------------------------------------------------------

template<class PointT>
unsigned int FrameAnimationT<PointT>::frameCount()
{
  return poses_.size();
}

//-----------------------------------------------------------------------------

template<class PointT>
void FrameAnimationT<PointT>::setFrameCount(unsigned int _frames)
{
  //delete poses
  while ( _frames < poses_.size() ){
    Pose* pose = poses_.back();
    poses_.pop_back();
    delete pose;
  }

  //add poses
  while ( _frames > poses_.size() )
    poses_.push_back(new Pose(skeleton_));
}

//-----------------------------------------------------------------------------


template<class PointT>
void FrameAnimationT<PointT>::insertJointAt(unsigned int _index)
{
  for(typename std::vector<Pose*>::iterator it = poses_.begin(); it != poses_.end(); ++it)
    (*it)->insertJointAt(_index);
}

//-----------------------------------------------------------------------------


template<class PointT>
void FrameAnimationT<PointT>::removeJointAt(unsigned int _index)
{
  for(typename std::vector<Pose*>::iterator it = poses_.begin(); it != poses_.end(); ++it)
    (*it)->removeJointAt(_index);
}

//-----------------------------------------------------------------------------

/**
 * \brief Updates the local matrix using the global matrix
 *
 * Called when a joints parent is changed.
 *
 * @param _index The joints index
 */
template<class PointT>
void FrameAnimationT<PointT>::updateFromGlobal(unsigned int _index)
{
  for(typename std::vector<Pose*>::iterator it = poses_.begin(); it != poses_.end(); ++it)
    (*it)->updateFromGlobal(_index);
}

//-----------------------------------------------------------------------------
