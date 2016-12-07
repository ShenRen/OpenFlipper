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

#define JOINTT_C

#include "JointT.hh"
#include <cassert>
#include <algorithm>

//-----------------------------------------------------------------------------------------------------

/**
 * @brief Default constructor, creates a joint pointing to the origin
 */
template <class PointT>
JointT<PointT>::JointT(Joint *_parent, std::string _name) :
  id_(0),
  selected_(false),
  parent_(_parent),
  name_(_name)
{
}

//-----------------------------------------------------------------------------------------------------

/**
 * @brief Copy constructor, creates an incomplete copy of the given joint
 *
 * The parent and children cannot be copied, since they are provided as pointers only. Furthermore,
 * if a skeleton is being cloned the corresponding joints may not yet exist in this skeleton.
 */
template<typename PointT>
JointT<PointT>::JointT(const Joint &_other) :
  id_(_other.id_),
  selected_(_other.selected_),
  parent_(0),
  name_(_other.name_)
{
}

//-----------------------------------------------------------------------------------------------------

template <class PointT>
JointT<PointT>::~JointT()
{
}

//-----------------------------------------------------------------------------------------------------

/**
 * @brief Returns the joints index
 *
 * The index is part of a continuous sequence out of the interval [0, number of joints). It is guaranteed
 * not to change, unless joints are deleted from the skeleton.
 */
template <class PointT>
inline size_t JointT<PointT>::id() const
{
  return id_;
}

//-----------------------------------------------------------------------------------------------------

template <class PointT>
inline void JointT<PointT>::setId(const size_t _id)
{
  id_ = _id;
}

//-----------------------------------------------------------------------------------------------------

/**
 * @brief Replaces this joints parent joint
 *
 * After calling this method this joint is child of the given joint. Keep in mind that this will corrupt
 * the joints local matrix in all poses.
 *
 * @param _newParent The new parent joint
 * @param _skeleton The skeleton this joint is part of, used to update the poses local matrices
 */
template <class PointT>
inline void JointT<PointT>::setParent(Joint *_newParent, SkeletonT<PointT> &_skeleton)
{
  // Check for cycles and do not continue operation if cycles would be created! (if this joint is a parent of newParent this will be the case)
  Joint *parent = _newParent;

  while (parent != 0){
    if (parent == this) {
      std::cerr << "Illegal setParent operation (joint " << _newParent->id() << " cannot be parent of " << this->id() << " because this would lead to a cycle. Cancelling." << std::endl;
      return;
    }
    parent = parent->parent();
  }

  if(parent_ != 0)
    if(std::remove(parent_->children_.begin(), parent_->children_.end(), this) != parent_->children_.end())	// remove from the last parent
      parent_->children_.resize(parent_->children_.size() - 1);

  parent_ = _newParent;

  if ( _newParent != 0)
    _newParent->children_.push_back(this);

  _skeleton.updateFromGlobal(id_);
}

//-----------------------------------------------------------------------------------------------------

/**
 * @brief Returns the parent joint
 *
 * If you want to know the root position of a joint, then just take its parents global position. They always
 * match, since a joint is always directly attached to its parent.
 */
template <class PointT>
inline JointT<PointT> *JointT<PointT>::parent()
{
  return parent_;
}

//-----------------------------------------------------------------------------------------------------

template <class PointT>
inline bool JointT<PointT>::isRoot() const
{
  return (parent_ == NULL);
}

//-----------------------------------------------------------------------------------------------------

/**
 * @brief Returns an iterator on the joints children
 */
template <class PointT>
inline typename JointT<PointT>::ChildIter JointT<PointT>::begin()
{
  return children_.begin();
}

//-----------------------------------------------------------------------------------------------------

/**
 * @brief Returns the end iterator for the joints children
 */
template <class PointT>
inline typename JointT<PointT>::ChildIter JointT<PointT>::end()
{
  return children_.end();
}

//-----------------------------------------------------------------------------------------------------

/**
 * @brief Returns the number of children
 */
template<typename PointT>
inline size_t JointT<PointT>::size() const
{
  return children_.size();
}

//-----------------------------------------------------------------------------------------------------

/**
 * @brief Returns the child joint with the given index
 *
 * @param _index The child nodes index
 * @return A pointer to the child joint or zero if the index was out of range
 */
template<typename PointT>
inline JointT<PointT> *JointT<PointT>::child(size_t _index)
{
  assert( _index < children_.size() );

  if(_index >= children_.size())
    return 0;
  return children_[_index];
}

//-----------------------------------------------------------------------------------------------------

/**
 * @brief Returns the joint's selection state
 *
 */
template <class PointT>
inline bool JointT<PointT>::selected() const
{
  return selected_;
}

//-----------------------------------------------------------------------------------------------------

/**
 * @brief Set the joint's selction state
 *
 */
template <class PointT>
inline void JointT<PointT>::setSelected(bool _selected)
{
  selected_ = _selected;
}

//-----------------------------------------------------------------------------------------------------

template<typename PointT>
inline std::string JointT<PointT>::name() const {
  return name_;
}

//-----------------------------------------------------------------------------------------------------

template<typename PointT>
inline void JointT<PointT>::setName(const std::string& _name) {
  name_ = _name;
}

//-----------------------------------------------------------------------------------------------------

