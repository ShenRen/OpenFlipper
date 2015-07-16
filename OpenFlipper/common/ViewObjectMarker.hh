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

#ifndef VIEWOBJECTMARKER_HH
#define VIEWOBJECTMARKER_HH

//== INCLUDES =================================================================
#include <ACG/Math/VectorT.hh>

//== FORWARDDECLARATIONS ======================================================

class BaseObject;

//== CLASS DEFINITION =========================================================


/** \class ViewObjectMarker ViewObjectMarker.hh <OpenFlipper/BasePlugin/ViewObjectMarker.hh>

    Abstract base class to mark objects with help of the stencil buffer

    This system allows to blend the rendered object with a color,
    to visualize different object properties/states.

    The ViewObjectMarker class can be activated for a examiner. The examiner will
    then call stencilRefForObject for each object. If stencilRefForObject returns
    "true" then the object _obj will be painted with the stencil buffer
    reference _reference (has to be != 0).

    There are 2 operation modes

    PerNumber:
      The examiner will call blendForStencilRefNumber for each of the
      returned references of stencilRefForObject to ask for the blend values.

    PerBit:
      The examiner will call blendForStencilRefNumberBit for each of the bits
      in the returned references of stencilRefForObject to ask for the blend values.

      PerBit handling allows to blend the object multiple times with different blend
      values, to visualize multiple properties at once, but is limited to the number
      of bits available in the stencil buffer (usually 8).

 **/

class DLLEXPORT ViewObjectMarker
{
//-------------------------------------------------------------- public methods
public:

  //--------------------------------------------------- destructor

  /// Blending type for this marker
  enum Type {
    /// Mark per returned reference
    PerNumber,
    /// Mark per returned reference bits
    PerBit
  };

  /// Destructor.
  virtual ~ViewObjectMarker () {};

  virtual Type type () { return PerNumber; };

  /** Get stencil reference for object
    @param _obj Object
    @param _reference stencil reference for object painting
    @return should the reference be used
  */
  virtual bool stencilRefForObject (BaseObjectData *_obj, GLuint &_reference) = 0;

  /** Per reference number blending values
    @param _reference stencil reference for blending
    @param _src sfactor parameter for ACG::GLState::blendFunc function
    @param _dst dfactor parameter for ACG::GLState::blendFunc function
    @param _color color used for blending
    @return should blending be berformed for this reference
  */
  virtual bool blendForStencilRefNumber (GLuint _reference, GLenum & _src, GLenum & _dst, ACG::Vec4f & _color) { return false; };

  /** Per reference bit blending values
    @param _refbit stencil reference bit for blending
    @param _src sfactor parameter for ACG::GLState::blendFunc function
    @param _dst dfactor parameter for ACG::GLState::blendFunc function
    @param _color color used for blending
    @return should blending be performed for this reference
  */
  virtual bool blendForStencilRefBit (GLuint _refbit , GLenum & _src , GLenum & _dst , ACG::Vec4f & _color ) { return false; };

};


#endif

