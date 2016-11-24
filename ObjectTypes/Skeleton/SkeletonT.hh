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

#ifndef SKELETONT_HH
#define SKELETONT_HH


//== INCLUDES =================================================================
#include <map>
#include <vector>
#include <iostream>
#include "JointT.hh"
#include "Properties.hh"
#include "Animation/AnimationT.hh"
#include "PoseT.hh"

#include <stack>

template <class PointT>
class SkeletonT : public Properties
{
  template<typename>
  friend class JointT;

public:
  typedef PointT                                  Point;
  typedef typename Point::value_type             Scalar;
  typedef JointT<Point>                           Joint;
  typedef PoseT<PointT>                            Pose;
  typedef AnimationT<PointT>                  Animation;
  typedef typename ACG::Matrix4x4T<Scalar>       Matrix;

public:

  /**
    * @brief Iterator class for the skeleton
    *
    * This iterator can be used to iterate over all joints in a top-down (root to leaf), left-to-right order.
    * It is possible to iterate over subtrees by constructing an iterator passing the root of the subtree
    * as parameter. Cast the iterator to boolean to test if it iterated all joints.
    */
  class Iterator
  {
  public:
    Iterator();
    Iterator(Joint *_root);
    Iterator(const Iterator &other);
    ~Iterator();
    Iterator &operator=(const Iterator &other);

  public:
    Iterator &operator++();
    bool operator!=(const Iterator &other) const;
    bool operator==(const Iterator &other) const;
    Joint *operator*() const;
    Joint *operator->() const;
    operator bool() const;

  private:
    Joint *nextSibling(Joint *_pParent, Joint *_pJoint);

  private:
    // Holds the current position in the tree
    Joint *pCurrent_;
    // The stack of joints, marking a path back to the root joint
    std::stack<Joint*> stJoints_;
  };

  /**
    * @brief Iterator class for the animations attached to a skeleton
    *
    * This iterator can be used to iterate over all animations that are attached to the skeleton
    */
  class AnimationIterator {
  public:
    AnimationIterator(std::vector<Animation*>& _animations );
    AnimationIterator(std::vector<Animation*>& _animations, size_t _animationIndex );

  public:
    AnimationIterator &operator++();
    AnimationHandle operator*() const;
    AnimationIterator &operator=(const AnimationIterator &other);
    operator bool() const;

  private:
    size_t currentIndex_;

    std::vector<Animation*>& animations_;
  };

public:
  /// Default constructor
  SkeletonT();
  /// Copy constructor
  SkeletonT(const SkeletonT<PointT>& _other);
  /// Assignment operator
  SkeletonT& operator= (const SkeletonT<PointT>& _other);

  /// Destructor
  ~SkeletonT();

public:
  /**
    * @name Modifying the tree structure
    * Use these methods to edit the skeleton tree.
    */
  ///@{
  void addJoint(typename SkeletonT<PointT>::Joint *_pParent, typename SkeletonT<PointT>::Joint *_pJoint);
  void insertJoint(typename SkeletonT<PointT>::Joint *_pChild, typename SkeletonT<PointT>::Joint *_pInsert);
  void removeJoint(typename SkeletonT<PointT>::Joint *_pJoint);
  inline void clear();
  ///@}

  /** \anchor JointAccess
    * @name   Basic Joint Access
    *
    * Use these methods to access joints in the skeleton.
    */
  ///@{
  inline Joint *root();
  inline Joint *joint(const size_t &_index);
  int parent(size_t _joint);
  size_t childCount(size_t _joint);
  size_t child(size_t _joint, size_t _child);
  size_t jointCount();
  
  /// Iterator over joints of the skeletal tree in TOP-DOWN order (from root to leafs)
  Iterator begin();
  Iterator end();
  ///@}
 
  /** \anchor AnimationAccess
    * @name   Animation
    * Use these methods to equip the skeleton with animation data.
    */
  ///@{

  inline Pose *pose(const AnimationHandle &_hAni);
  inline Pose *referencePose();

  AnimationHandle addAnimation(std::string _name, Animation* _animation);
  AnimationHandle cloneAnimation(std::string _name, const AnimationHandle &_hAni);
  AnimationHandle animationHandle(std::string _name);
  Animation *animation(std::string _name);
  Animation *animation(const AnimationHandle &_hAni);
  void removeAnimation(std::string _name);
  void removeAnimation(const AnimationHandle &_hAni);
  void clearAnimations();

  void replaceAnimationName(const std::string& _strOld, const std::string& _strNew) {
      std::map<std::string,size_t>::iterator f = names_.find(_strOld);
      if(f != names_.end()) {
          size_t c = f->second;
          names_.erase(f);
          names_[_strNew] = c;
      }
  }

  /// Iterator over the animations
  AnimationIterator animationsBegin();
  AnimationIterator animationsEnd();

  size_t animationCount();
  const std::string &animationName(size_t _index);
  ///@}


protected:
  /// update the structure when parent changes for a joint
  void updateFromGlobal(size_t _idJoint);

protected:
  
  /// Joints of the skeleton
  std::vector<Joint*> joints_;

  /// Binds a name to each animation
  std::map<std::string, size_t> names_;
  /// Animations defined on the skeleton
  std::vector<Animation*> animations_;

  /// The skeletons reference pose
  Pose referencePose_;
};

//=============================================================================
//=============================================================================
#if defined(INCLUDE_TEMPLATES) && !defined(SKELETON_C)
#define SKELETONT_TEMPLATES
#include "SkeletonT.cc"
#endif
//=============================================================================
#endif // SKELETONT_HH defined
//=============================================================================

