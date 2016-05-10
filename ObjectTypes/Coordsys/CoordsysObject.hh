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
 * \file CoordsysObject.hh
 * This File contains the Coordsys Object
 */


#ifndef COORDSYSOBJECT_HH
#define COORDSYSOBJECT_HH

 

//== INCLUDES =================================================================

#include <OpenFlipper/common/BaseObjectData.hh>       

#include <OpenFlipper/common/GlobalDefines.hh>

#include "CoordsysTypes.hh"

//== TYPEDEFS =================================================================

// //== CLASS DEFINITION =========================================================

class DLLEXPORT CoordsysObject : public BaseObjectData {

  friend class TypeCoordsysPlugin;

  public: 
    /// constructor
    CoordsysObject();
    
    /** \brief copy constructor
     *
     *  Create a copy of this object
     */
    CoordsysObject(const CoordsysObject& _object);

    /// destructor
    virtual ~CoordsysObject();
    
    /// Reset current object, including all related nodes.
    virtual void cleanup();   

    /** return a full copy of this object ( All scenegraph nodes will be created )
     *  but the object will not be a part of the object tree.
     */
    BaseObject* copy();


  protected:
    /// Initialize current object, including all related nodes.
    virtual void init(CoordsysNode* _coordsys = 0);

  //===========================================================================
  /** @name Name and Path handling
   * @{ */
  //===========================================================================       
  public:
    
    /// Set the name of the Object
    void setName( QString _name );
        
  //===========================================================================
  /** @name Visualization
   * @{ */
  //=========================================================================== 

  public:
    /// Get the scenegraph Node
    CoordsysNode* coordsysNode();
    
  private:
    CoordsysNode* coordsysNode_;

  /** @} */ 
    
  //===========================================================================
  /** @name Object Information
   * @{ */
  //===========================================================================    
  public:
    /// Get all Info for the Object as a string
    QString getObjectinfo();
    
  /** @} */  
    
    
  //===========================================================================
  /** @name Picking
   * @{ */
  //===========================================================================    
  public:
    /// detect if the node has been picked
    bool picked( uint _node_idx );

    /// Enable or disable picking for this Object
    void enablePicking( bool _enable );

    /// Check if picking is enabled for this Object
    bool pickingEnabled();
    
  /** @} */  
    
  //===========================================================================
  /** @name Update handling
   *
   *  This is mostly private. Updates have to be triggered via
   *  emit updatedObject()
   *
   * @{ */
  //===========================================================================

  protected:

    /// Update the whole Object (Selection,Topology,...)
    virtual void update(UpdateType _type = UPDATE_ALL);

  /** @} */

};

//=============================================================================
#endif // COORDSYSOBJECT_HH defined
//=============================================================================
