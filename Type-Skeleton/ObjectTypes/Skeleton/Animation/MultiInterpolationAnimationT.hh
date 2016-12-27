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

#ifndef MULTIINTERPOLATIONANIMATIONT_HH
#define MULTIINTERPOLATIONANIMATIONT_HH

#include "InterpolationAnimationT.hh"

template<class PointT>
class MultiInterpolationAnimationT : public InterpolationAnimationT<PointT>
{
  template<typename>
  friend class SkeletonT;

  public:
    typedef PointT                                          Point;
    typedef typename Point::value_type                      Scalar;
    typedef PoseT<PointT>                                   Pose;                        ///< Typedef for the pose template
    typedef InterpolationMatrixManipulatorT<Scalar>         MatrixManipulator;
    typedef InterpolationT<Scalar>                          Interpolator;
    typedef std::vector<Scalar>                             TargetType;
    typedef std::vector< InterpolationAnimationT<ACG::Vec3d>* > InterpolationAnimations;

  public:
    MultiInterpolationAnimationT(const MultiInterpolationAnimationT<PointT> &_other);
    MultiInterpolationAnimationT() : InterpolationAnimationT<PointT>(NULL, NULL), interpolationAnimations_() {}
    MultiInterpolationAnimationT(const InterpolationAnimations& _interpolationAnimations) : interpolationAnimations_(_interpolationAnimations) {}

    virtual AnimationT<PointT>* copy();

    virtual void updateFromGlobal(unsigned int _index) {/*NOOP*/};

    virtual bool getMinInput(Scalar& _result);
    virtual bool getMaxInput(Scalar& _result);

  private:
    InterpolationAnimations interpolationAnimations_;

  public:
    using InterpolationAnimationT<PointT>::FPS;

  public:

    //===========================================================================
    /** @name Frame access
     *
     * There is one pose per frame.
     *
     * @{ */
    //===========================================================================

    /** \brief get a pose
     *
     * @param _iFrame    Frame
     * @return Requested pose
     */
    virtual Pose *pose(unsigned int _iFrame);

    /** \brief get a pose
     *
     * @param _iFrame    Frame
     * @param _reference Reference pose
     * @return Requested pose
     */
    virtual Pose *pose(unsigned int _iFrame, Pose* _reference);
    virtual unsigned int frameCount();
    /** @} */

    //===========================================================================
    /** @name Synchronization
     *
     * Use these methods to keep the poses in sync with the number (and indices) of the joints.
     * @{ */
    //===========================================================================
    /** \brief Called by the skeleton as a new joint is inserted
     *
     * The call is dispatched to all poses stored in the animation. See BaseNode::insert_at for more information.
     *
     * @param _index The new joint is inserted at this position. Insert new joints at the end by passing SkeletonT<>::jointCount as parameter.
     */
    void insertJointAt(unsigned int _index) {};

    /** \brief Called by the skeleton as a joint is deleted
     *
     * The call is dispatched to all poses stored in this animation. See BasePoseT<>::remove_at for more information.
     *
     * @param _index The index of the joint that is being deleted.
     */
    void removeJointAt(unsigned int _index) {};
    /** @} */

    //===========================================================================
    /** @name Animations access
     *
     * @{ */
    //===========================================================================
    void addInterpolationAnimation(InterpolationAnimationT<PointT> *_animation) {
      interpolationAnimations_.push_back(_animation);
    }
    unsigned int animationCount() {
      return interpolationAnimations_.size();
    }

    InterpolationAnimationT<PointT>* animation(unsigned int _index );
    /** @} */

    virtual void clearPoseCache() {
      for (uint i=0; i < interpolationAnimations_.size(); ++i) {
        interpolationAnimations_[i]->clearPoseCache();
      }
    }
};

//=============================================================================
#if defined(INCLUDE_TEMPLATES) && !defined(MULTIINTERPOLATIONANIMATIONT_C)
#define MULTIINTERPOLATIONANIMATIONT_TEMPLATES
#include "MultiInterpolationAnimationT.cc"
#endif
//=============================================================================

#endif //MULTIINTERPOLATIONANIMATIONT_HH
