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

//=============================================================================
//
//  Types
//
//=============================================================================

/**
 * \file SkeletonObject.hh
 * This File contains the Skeleton Object
 */


#ifndef SKELETONOBJECT_HH
#define SKELETONOBJECT_HH


//== INCLUDES =================================================================

#include "SkeletonTypes.hh"
#include <OpenFlipper/common/BaseObjectData.hh>
#include <OpenFlipper/common/GlobalDefines.hh>
#include <OpenFlipper/common/GlobalOptions.hh>

#include "SkeletonNodeT.hh"
#include "SkeletonTypes.hh"

//== TYPEDEFS =================================================================

//== CLASS DEFINITION =========================================================

/** This class provides the functionality for all kind of meshes for the framework
 */
class DLLEXPORT SkeletonObject : public BaseObjectData {

  friend class TypeSkeletonPlugin;

  public:
    /// Constructor
    SkeletonObject();
    /// Copy Constructor
    SkeletonObject(const SkeletonObject &_other);
    /// Destructor
    virtual ~SkeletonObject();
    /// Reset current object, including all related nodes.
    virtual void cleanup();

    virtual BaseObject* copy();

  protected:
    /// Initialize current object, including all related nodes.
    virtual void init();
    void init(Skeleton* _pSkeleton);

  /**
   * @name Name and Path handling
   */
  //@{
  public:
    /// Set the name of the Object
    void setName( QString _name );
  //@}

  /**
   * @name Content
   */
  //@{
  public:
    /// Returns a pointer to the skeleton
    Skeleton* skeleton();

  private:
    /// A pointer to the skeleton data
    Skeleton* skeleton_;
  //@}

  /**
   * @name Visualization
   */
  //@{
  public:
    /// Returns the skeleton scenegraph node
    ACG::SceneGraph::SkeletonNodeT< Skeleton >* skeletonNode();

  private:
    /// A pointer to the scenegraph node
    ACG::SceneGraph::SkeletonNodeT< Skeleton >* skeletonNode_;
  //@}

  /**
   * @name Object Information
   */
  //@{
  public:
    /// Returns a string holding information on this object
    QString getObjectinfo();
  //@}

  /**
   * @name Picking
   */
  //@{
  public:
    /// Returns true if the picked node given by _node_idx is this objects scenegraph node
    bool picked( uint _node_idx );
    /// Enable or disable picking for this object
    void enablePicking( bool _enable );
    /// Check if picking is enabled for this object
    bool pickingEnabled();
  //@}

  AnimationHandle activePose();
  void setActivePose(const AnimationHandle &_hAni);
  
  void showIndices(bool _bVisible = true);
  bool indicesVisible();

  void updateIndices();
  void updateIndices(const AnimationHandle &_hAni);
  
  void showMotionPath(bool _visible = true);
  bool motionPathVisible();

  void updateMotionPath();
  void updateMotionPath(const AnimationHandle &_hAni);


  //===========================================================================
  /** @name Update handling
   *
   *  This is mostly private. Updates have to be triggered via
   *  emit updatedObject()
   *
   * @{ */
  //===========================================================================

  private:
    /** \brief  This function is called to update the object
     *
     * If the object changes, the core will call this function. This function
     * triggers an vbo update in the plane node.
     *
     * \note Do not call this function yourself to avoid unnecessary overhead(the core will call it when it is required)
     */
    void update(UpdateType _type = UPDATE_ALL );

   /** @} */
};

//=============================================================================
#endif // SKELETONOBJECT_HH defined
//=============================================================================
