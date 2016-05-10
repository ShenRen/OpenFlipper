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
 * \file LightObject.hh 
 * This File contains the Light Object
 */


#ifndef LIGHTOBJECT_HH
#define LIGHTOBJECT_HH

 

//== INCLUDES =================================================================

#include <OpenFlipper/common/BaseObjectData.hh>       
#include <OpenFlipper/common/GlobalDefines.hh>

#include "LightTypes.hh"

//== TYPEDEFS =================================================================

// //== CLASS DEFINITION =========================================================

class DLLEXPORT LightObject : public BaseObjectData {

  friend class TypeLightPlugin;
  friend class LightWidget;

  public: 
    /// constructor
    LightObject();
    
    /** \brief copy constructor
     *
     *  Create a copy of this object
     */
    LightObject(const LightObject& _object);

    /// destructor
    virtual ~LightObject();
    
    /// Reset current object, including all related nodes.
    virtual void cleanup();   

    /** return a full copy of this object ( All scenegraph nodes will be created )
     *  but the object will not be a part of the object tree.
     */
    BaseObject* copy();


  protected:
    /// Initialize current object, including all related nodes.
    virtual void init(LightNode* _light = 0, LightNode* _lightVis = 0);

  //===========================================================================
  /** @name Name and Path handling
   * @{ */
  //===========================================================================       
  public:
    
    /// Set the name of the Object
    void setName( QString _name );
    
  /** @} */   
  
  //===========================================================================
  /** @name Contents
   * @{ */
  //=========================================================================== 
  public:
    
    LightSource* lightSource();
    

    /// Is light default light source?
    bool defaultLight() const { return defaultLightSource_; }
    
    /// Set light object to be default light
    void defaultLight( const bool _default ) { defaultLightSource_ = _default; }
    
  private:
    LightSource lightSource_;
    
    /// True if light is default light source initially
    /// added to a blank scene
    bool defaultLightSource_;
    
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
    /** \brief Update the Light Object
    *
    *   Updates the rendering of the light object
    */
    virtual void update(UpdateType _type = UPDATE_ALL);

  /** @} */      
       
  //===========================================================================
  /** @name Visualization
   * @{ */
  //=========================================================================== 

  public:
    /// Get the scenegraph Node
    LightNode* lightNode();
    
    /// Get the scenegraph Node
    LightNode* lightNodeVis();
    
    virtual bool hasNode(BaseNode* _node);
    
  private:
    ///  Light status node
    LightNode* lightNode_;
    
    /// Light rendering node (only for rendering purposes)
    LightNode* lightNodeVis_;

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
  
  public:
    
    /// Show Light Node
    virtual void visible(bool _visible);
    
    /// Show Light Node
    virtual bool visible();
    
    /// Show Light Node
    virtual void show();
    
    /// Hide Light Node
    virtual void hide();
    

};

//=============================================================================
#endif // LIGHTOBJECT_HH defined
//=============================================================================
