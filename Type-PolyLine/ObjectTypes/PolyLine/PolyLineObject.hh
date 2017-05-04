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
 * \file PolyLineObject.hh 
 * This File contains the PolyLine Object
 */


#ifndef POLYLINEOBJECT_HH
#define POLYLINEOBJECT_HH

 

//== INCLUDES =================================================================

#include "PolyLineTypes.hh"
#include <OpenFlipper/common/BaseObjectData.hh>       

#include <ObjectTypes/PolyLine/PolyLineNodeT.hh>

#include <OpenFlipper/common/ObjectTypeDLLDefines.hh>

//== TYPEDEFS =================================================================

//== CLASS DEFINITION =========================================================

/** This class provides the functionality for all kind of meshes for the framework
 */
class OBJECTTYPEDLLEXPORT PolyLineObject : public BaseObjectData {

  friend class TypePolyLinePlugin;

  public: 

    typedef PolyLine PolyLineType;

    /// constructor
    PolyLineObject();
    
    /** \brief copy constructor
     *
     *  Create a copy of this object
     */
    PolyLineObject(const PolyLineObject& _object);

    /// destructor
    virtual ~PolyLineObject();
    
	/// Reset current object, including all related nodes.
    virtual void cleanup();   

    /** return a full copy of this object ( All scenegraph nodes will be created )
     *  but the object will not be a part of the object tree.
     */
    BaseObject* copy();

  protected:
    
    /// Initialise current object, including all related nodes.
    virtual void init(PolyLine* _line = 0);
    
  //===========================================================================
  /** @name Name and Path handling
   * @{ */
  //===========================================================================       
  public:
    
    /// Set the name of the Object
    void setName( QString _name );
    
    
  //===========================================================================
  /** @name Content
   * @{ */
  //===========================================================================
    
  public:

    /// return a pointer to the line
    PolyLine* line(); 

  private:
    /// Pointer to the polyline
    PolyLine*           line_;

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
    /** \brief  This function is called to update the object
     *
     * If the object changes, the core will call this function. Normally this will update
     * the corresponding scenegraph nodes or trigger other data handling which has to be done
     * when the object changes.
     *
     * \note Do not call this function yourself to avoid unnecessary overhead(the core will call it when it is required)
     */
    virtual void update(UpdateType _type = UPDATE_ALL );
    
  /** @} */ 
    
  //===========================================================================
  /** @name Visualization
   * @{ */
  //=========================================================================== 
    
  public :    
    
    /// Get the scenegraph Node
    ACG::SceneGraph::PolyLineNodeT< PolyLine >* lineNode();
    
  private :
    
    /// Scenegraph Mesh Node
    ACG::SceneGraph::PolyLineNodeT< PolyLine >*                     lineNode_;
      
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


    /// Refine picking on triangle meshes
    ACG::Vec3d refinePick(ACG::SceneGraph::PickTarget _pickTarget,
                                 const ACG::Vec3d _hitPoint,
                                 const ACG::Vec3d _start ,
                                 const ACG::Vec3d _dir,
                                 const unsigned int _targetIdx );
    
  /** @} */  
    
};

//=============================================================================
#endif // POLYLINEOBJECT_HH defined
//=============================================================================
