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

#ifndef POSET_HH
#define POSET_HH

#include "ACG/Math/Matrix4x4T.hh"
#include "ACG/Math/VectorT.hh"

#include "ACG/Math/QuaternionT.hh"
#include "ACG/Math/DualQuaternionT.hh"


template<typename PointT>
class SkeletonT;

/**
 * @brief A general pose, used to store the frames of the animation
 *
 */
template<typename PointT>
class PoseT
{
  template<typename>
  friend class SkeletonT;
  template<typename>
  friend class AnimationT;
  template<typename>
  friend class FrameAnimationT;

protected:
  typedef PointT                                Point;
  typedef typename Point::value_type            Scalar;
  typedef typename ACG::VectorT<Scalar, 3>      Vector;
  typedef typename ACG::Matrix4x4T<Scalar>      Matrix;
  typedef typename ACG::QuaternionT<Scalar>     Quaternion;
  typedef typename ACG::DualQuaternionT<Scalar> DualQuaternion;

public:
  /// Constructor
  PoseT(SkeletonT<Point>* _skeleton);
  /// Copy Constructor
  PoseT(const PoseT<PointT>& _other);
  /// Destructor
  virtual ~PoseT();

  // =======================================================================================
  /** @anchor PoseEditing
    * @name   Pose editing
    * These methods update the other coordinate systems, changing the local coordinates will also change the global and vice versa.
    * @{ */
  // =======================================================================================
  
  /// local matrix manipulation
  /// the local matrix represents a joints orientation/translation in the coordinate frame of the parent joint
  inline const Matrix& localMatrix(unsigned int _joint) const;
  void setLocalMatrix(unsigned int _joint, const Matrix &_local, bool _keepLocalChildPositions=true);
  inline Vector localTranslation(unsigned int _joint);
  void setLocalTranslation(unsigned int _joint, const Vector &_position, bool _keepLocalChildPositions=true);  

  inline Matrix localMatrixInv(unsigned int _joint) const;
  
  /// global matrix manipulation
  /// the global matrix represents a joints orientation/translation in global coordinates
  inline const Matrix& globalMatrix(unsigned int _joint) const;
  void setGlobalMatrix(unsigned int _joint, const Matrix &_global, bool _keepGlobalChildPositions=true);
  inline Vector globalTranslation(unsigned int _joint);
  void setGlobalTranslation(unsigned int _joint, const Vector &_position, bool _keepGlobalChildPositions=true);
  
  virtual Matrix globalMatrixInv(unsigned int _joint) const;

  /** @} */

  // =======================================================================================
  /**
    * @name Synchronization
    * Use these methods to keep the pose in sync with the number (and indices) of the joints.
    * @{ */
  // =======================================================================================

  /**
   * \brief Called by the skeleton/animation as a new joint is inserted
   *
   * To keep the vectors storing the matrices for the joints in sync with the joints a new entry has to be inserted
   * in exactly the same place if a new joint is added to the skeleton. This is done here. Derived classes
   * have to overwrite this method to keep their data members in sync as well. Always call the base class
   * method first.
   *
   * @param _index The new joint is inserted at this position. Insert new joints at the end by passing
   *               SkeletonT<>::jointCount() as parameter.
   */
  virtual void insertJointAt(size_t _index);

  /**
   * \brief Called by the skeleton/animation as a joint is removed
   *
   * To keep the vectors storing the matrices for the joints in sync with the joints exactly the same entry
   * has to be removed as a joint is removed from the skeleton. This is done here. Derived classes
   * have to overwrite this method to keep their data members in sync as well. Always call the base class
   * method first.
   *
   * @param _index The new joint is inserted at this position. Insert new joints at the end by passing
   *         SkeletonT<>::jointCount() as parameter.
   */
  virtual void removeJointAt(size_t _index);

  /** @} */

protected:
  // =======================================================================================
  /** @name Coordinate system update methods
    * These methods propagate the change in one of the coordinate systems into the other. This will
    * keep intact the children nodes' positions per default (by recursively updating all children.).
    * This behavior can be influenced via the _keepChildPositions parameter.
    * @{ */
  // =======================================================================================

  void updateFromLocal(size_t _joint, bool _keepChildPositions=true);
  void updateFromGlobal(size_t _joint, bool _keepChildPositions=true);

  /** @} */
  
public:

  // =======================================================================================
  /** @anchor UnifiedMatrices
    * @name Unified Matrices
    * Use these methods to gain access to the precalculations performed by this derivation.
    * @{ */
  // =======================================================================================

  inline const Matrix&         unifiedMatrix(size_t _joint);
  inline const Quaternion&     unifiedRotation(size_t _joint);
  inline const DualQuaternion& unifiedDualQuaternion(size_t _joint);

  /** @} */

protected:

  /// a pointer to the skeleton
  SkeletonT<PointT>* skeleton_;
  /// the pose in local coordinates
  std::vector<Matrix> local_;
  /// the pose in global coordinates
  std::vector<Matrix> global_;

  /// the global pose matrix left-multiplied to the inverse global reference matrix: \f$ M_{pose} \cdot M^{-1}_{reference} \f$
  std::vector<Matrix> unified_;
  
  /// JointT::PoseT::unified in DualQuaternion representation
  /// note: the real part of the dual quaternion is the rotation part of the transformation as quaternion
  std::vector<DualQuaternion> unifiedDualQuaternion_;
};

//=============================================================================
//=============================================================================
#if defined(INCLUDE_TEMPLATES) && !defined(POSET_C)
#define POSET_TEMPLATES
#include "PoseT.cc"
#endif
//=============================================================================
#endif
//=============================================================================

