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
 * \file perObjectData.hh
 * This File contains a basic data class used to attach data to objects.
 */


#ifndef PEROBJECTDATA_HH
#define PEROBJECTDATA_HH


//== INCLUDES =================================================================

#include <OpenFlipper/common/GlobalDefines.hh>

//== TYPEDEFS =================================================================


//== CLASS DEFINITION =========================================================

/** \brief Object Payload
 *
 * This class is used to add arbitrary data to objects
 * in OpenFlipper. You can derive any kind of class from
 * PerObjectData and attach it to an object. See
 * BaseObject::setObjectData() for more details.
 *
 * */
class DLLEXPORT PerObjectData {

  public :
    /// You have to provide your own constructor for your object
    PerObjectData();
    
    virtual ~PerObjectData();

    /** \brief Copy Function
     *
    * You have to reimplement this function to allow the core to create a copies of your Object.
    * By default it will return 0;
    *
    * The function has to create a deep copy of the object, as it will also be used to create backups.
    * If you use pointer inside your class, remember to not copy the pointer only but also the data!
    *
    */
    virtual PerObjectData* copyPerObjectData( );

};

//=============================================================================

#define PER_OBJECT_DATA(_CLASS, _VALUE)                                      \
class DLLEXPORT _CLASS : public PerObjectData {                              \
                                                                             \
  public:                                                                    \
    inline _CLASS()                 : data_( 0) { }                          \
    inline explicit _CLASS(const _VALUE& _x) : data_(_x) { }                 \
    inline virtual ~_CLASS() { }                                             \
    inline virtual _CLASS* copyPerObjectData() { return new _CLASS(data_); } \
    inline       _VALUE& data()       { return data_; }                      \
    inline const _VALUE& data() const { return data_; }                      \
  private:                                                                   \
    _VALUE data_;                                                            \
}

//=============================================================================

PER_OBJECT_DATA(BoolPerObjectData,   bool         );
PER_OBJECT_DATA(CharPerObjectData,   char         );
PER_OBJECT_DATA(UCharPerObjectData,  unsigned char);
PER_OBJECT_DATA(IntPerObjectData,    int          );
PER_OBJECT_DATA(UIntPerObjectData,   unsigned int );
PER_OBJECT_DATA(FloatPerObjectData,  float        );
PER_OBJECT_DATA(DoublePerObjectData, double       );

//=============================================================================
#endif // PEROBJECTDATA_HH defined
//=============================================================================
