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

//================================================================
//
//  Types
//
//================================================================


/**
 * \file SplatCloudObject.hh
 * This File contains the SplatCloud Object
 */


#ifndef SPLATCLOUDOBJECT_HH
#define SPLATCLOUDOBJECT_HH


//== INCLUDES ====================================================


#include "SplatCloudTypes.hh"

#include "SplatCloud/SplatCloud.hh"

#include <OpenFlipper/common/BaseObjectData.hh>

#include <OpenFlipper/common/ObjectTypeDLLDefines.hh>


//== CLASS DEFINITION ============================================


class OBJECTTYPEDLLEXPORT SplatCloudObject : public BaseObjectData
{

  friend class TypeSplatCloudPlugin;

public:
  /// Constructor
  SplatCloudObject();

  /** \brief Copy constructor
   *
   *  Create a copy of this Object
   */
  SplatCloudObject( const SplatCloudObject &_object );

  /// Destructor
  virtual ~SplatCloudObject();

  /// Reset current Object, including all related Nodes
  virtual void cleanup();

  /** Return a full copy of this Object ( all scenegraph Nodes will be created )
   *  but the object will not be a part of the object tree
   */
  BaseObject *copy();

  /** \brief Reload standard and picking Shaders from file
   *
   *  Shader uniforms has to be updated after this!
   */
  void reloadShaders();

  /** \brief Enable or disable backface culling for all Shaders
   */
  void enableBackfaceCulling( bool _enable );

  /** \brief Set the scaling factor for pointsizes for all Shaders
   */
  void setPointsizeScale( float _scale );

  inline bool isBackfaceCullingEnabled() const { return backfaceCullingEnabled_; }
  inline float pointsizeScale() const { return pointsizeScale_; }

private:
  bool  backfaceCullingEnabled_;
  float pointsizeScale_;

protected:
  /// Initialise current Object, including all related Nodes
  virtual void init( const SplatCloud *_splatCloud = 0 );

//===========================================================================
/** @name Update handling
 *
 *  This is mostly private. Updates have to be triggered via
 *  emit updatedObject()
 *
 * @{ */
//===========================================================================
  protected:

  /// Called by the core if the object has to be updated
  void update( UpdateType _type = UPDATE_ALL );

  /** @} */

//--------------------------------
/** @name Name and Path handling
 * @{ */
//--------------------------------
public:
  /// Set the name of the Object
  void setName( QString _name );

/** @} */

//--------------------------------
/** @name Content
 * @{ */
//--------------------------------
public:
  /// Get SplatCloud
  inline       SplatCloud *splatCloud()       { return splatCloud_; }
  inline const SplatCloud *splatCloud() const { return splatCloud_; }

private:
  SplatCloud *splatCloud_;

/** @} */

//--------------------------------
/** @name Visualization
 * @{ */
//--------------------------------
public:
  /// Get Shader's scenegraph Node
  inline       ShaderNode *shaderNode()       { return shaderNode_; }
  inline const ShaderNode *shaderNode() const { return shaderNode_; }

  /// Get SplatCloud's scenegraph Node
  inline       SplatCloudNode *splatCloudNode()       { return splatCloudNode_; }
  inline const SplatCloudNode *splatCloudNode() const { return splatCloudNode_; }

private:
  ShaderNode     *shaderNode_;
  SplatCloudNode *splatCloudNode_;

/** @} */

//--------------------------------
/** @name Object Information
 * @{ */
//--------------------------------
public:
  /// Get all Info for the Object as a string
  QString getObjectinfo();

/** @} */

//--------------------------------
/** @name Picking
 * @{ */
//--------------------------------
public:
  /// Detect if the node has been picked
  bool picked( uint _node_idx );

  /// Enable or disable picking for this Object
  void enablePicking( bool _enable );

  /// Check if picking is enabled for this Object
  bool pickingEnabled();

/** @} */

};


//================================================================


#endif // SPLATCLOUDOBJECT_HH
