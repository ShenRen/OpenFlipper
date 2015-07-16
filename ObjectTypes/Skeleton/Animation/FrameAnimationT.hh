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

#ifndef FRAMEANIMATIONT_HH
#define FRAMEANIMATIONT_HH

#include "AnimationT.hh"

template<class PointT>
class FrameAnimationT : public AnimationT<PointT>
{
  template<typename>
  friend class SkeletonT;

  public:
    typedef PointT                                Point;
    typedef typename Point::value_type            Scalar;
    typedef SkeletonT<PointT>                     Skeleton;
    typedef PoseT<PointT>                         Pose;

  public:
    FrameAnimationT(const PoseT<PointT> &_pose);

    FrameAnimationT(Skeleton* _skeleton);

    FrameAnimationT(Skeleton* _skeleton, unsigned int _iNumFrames);

    FrameAnimationT(const FrameAnimationT<PointT> &_other);

    virtual ~FrameAnimationT();

    virtual AnimationT<PointT>* copy();

    virtual void updateFromGlobal(unsigned int _index);

  public:

    // =======================================================================================
    /** @name Frame access
      * There is one pose per frame.
       * @{ */
    // =======================================================================================

    /** \brief Returns a pointer to the pose stored in the given frame
     *
     * @param _iFrame The poses frame number
     */
    inline Pose *pose(unsigned int _iFrame);

    /** \brief Returns the number of frames stored in this pose
     */
    inline unsigned int frameCount();

    /** \brief Set number of frames stored in this pose
     */
    void setFrameCount(unsigned int _frames);
    /** @} */

    // =======================================================================================
    /** @name Synchronization
      *
      * Use these methods to keep the poses in sync with the number (and indices) of the joints.
      * @{ */
    // =======================================================================================

    /** \brief Called by the skeleton as a new joint is inserted
     *
     * The call is dispatched to all poses stored in this animation. See BasePoseT<>::insert_at for more information.
     *
     * @param _index The new joint is inserted at this position. Insert new joints at the end by passing SkeletonT<>::jointCount as parameter.
     */
    void insertJointAt(unsigned int _index);

    /** \brief Called by the skeleton as a joint is deleted
     *
     * The call is dispatched to all poses stored in this animation. See BasePoseT<>::remove_at for more information.
     *
     * @param _index The index of the joint that is being deleted.
     */
    void removeJointAt(unsigned int _index);
    /** @} */

  private:
    /// Pointer to associated skeleton
    Skeleton* skeleton_;
    /// Every entry in this vector is a frame of the animation
    std::vector<Pose*> poses_;

};

//=============================================================================
#if defined(INCLUDE_TEMPLATES) && !defined(FRAMEANIMATIONT_C)
#define FRAMEANIMATIONT_TEMPLATES
#include "FrameAnimationT.cc"
#endif
//=============================================================================

#endif //FRAMEANIMATIONT_HH
