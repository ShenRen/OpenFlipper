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

#ifndef JOINTT_HH
#define JOINTT_HH

#include <vector>

template<typename PointT>
class SkeletonT;

/**
 * @brief Represents a single joint in the skeleton
 *
 * The skeleton is made up by a hierarchical structure of joints. The joints don't store their position
 * themselves. Instead, the joint positions are stored in poses. The poses are managed by the AnimationT class
 * and the skeleton.
 */
template <class PointT>
class JointT
{
  template< class >
  friend class SkeletonT;

public:
  typedef PointT                                 Point;
  typedef typename Point::value_type             Scalar;
  typedef JointT<PointT>                         Joint;
  typedef typename std::vector<Joint*>::iterator ChildIter;

public:
  /// Constructor
  JointT(const Joint &_other);
  JointT(Joint *_parent, std::string _name = "");
  /// Destructor
  ~JointT();
  
  /// returns the joint id
  inline size_t id() const;

  /// access parent of the joint
  inline void setParent(Joint *_newParent, SkeletonT<PointT> &_skeleton);
  inline Joint *parent();
  
  inline bool isRoot() const;

public:
  /**
    * @name Child access
    * Use this iterator to access the joints child nodes
    */
  //@{
  inline ChildIter begin();
  inline ChildIter end();
  inline size_t size() const;
  inline Joint *child(size_t _index);
  //@}

  /**
   * @name Selections
   * change and access selection state
   */
  //@{
  inline bool selected() const;
  inline void setSelected(bool _selected);
  //@}

  /// Access the name of the joint
  inline std::string name() const;
  inline void setName(const std::string& _name);

private:
  /// An unique identifier, guaranteed to be part of a continuous sequence starting from 0
  size_t id_;
  bool         selected_;

protected:
  inline void setId(const size_t _id);
  
  /// The parent joint; this joint is in its parents JointT::children_ vector. It's 0 for the root node.
  Joint *parent_;
  /// The joints children, use the JointT::getChild method to access them
  std::vector<Joint*> children_;
  /// the name of the joint
  std::string name_;
};

//=============================================================================
#if defined(INCLUDE_TEMPLATES) && !defined(JOINTT_C)
#define JOINTT_TEMPLATES
#include "JointT.cc"
#endif
//=============================================================================
#endif
