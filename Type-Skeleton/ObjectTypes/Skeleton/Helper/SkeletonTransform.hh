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

#ifndef SKELETONTRANSFORM_HH
#define SKELETONTRANSFORM_HH

#include "../Skeleton.hh"
#include <ACG/Math/Matrix4x4T.hh>
#include <OpenFlipper/common/ObjectTypeDLLDefines.hh>

/** \brief Skeleton transformation class
 *
 * This class should be used to manipulate skeletons since
 * it makes sure that all necessary transformations are applied
 * and that the transformations do not mess up the skeleton
 *
 **/
class OBJECTTYPEDLLEXPORT SkeletonTransform {

  public:
    SkeletonTransform(Skeleton& _skeleton);
    ~SkeletonTransform() {}

  public:
    /// scale all bones of the skeleton by the given factor
    void scaleSkeleton(double _factor, Skeleton::Pose* _pose = 0);

    /// translate the skeleton
    void translateSkeleton(ACG::Vec3d _translation, Skeleton::Pose* _pose = 0);

    /// transform the skeleton
    void transformSkeleton(Matrix4x4 _transformation, Skeleton::Pose* _pose = 0);

    /// apply a translation to a joint in the refPose
    void translateJoint(Skeleton::Joint* _joint, ACG::Vec3d _translation, bool _keepChildPositions = true);

    /// apply a transformation to a joint in the refPose
    void transformJoint(Skeleton::Joint* _joint, Matrix4x4 _matrix, bool _keepChildPositions = true);

    /// rotate a joint in an arbitrary Pose
    void rotateJoint(Skeleton::Joint* _joint, Skeleton::Pose* _pose, Matrix4x4 _rotation, bool _applyToWholeAnimation = true);

    /// compute determinant to check if matrix is rotation matrix
    static double determinant(Matrix4x4& _m);
    
  private:
    Skeleton& skeleton_;
    Skeleton::Pose* refPose_;
};

#endif //SKELETONTRANSFORM_HH
