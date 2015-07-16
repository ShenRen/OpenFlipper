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
//  CLASS ObjectIDPointerManagerT - IMPLEMENTATION
//
//=============================================================================

#define OBJECTIDPOINTERMANAGERT_C

//== INCLUDES =================================================================

#include "ObjectIDPointerManagerT.hh"

//== NAMESPACES ===============================================================

//== IMPLEMENTATION ==========================================================

template< class Pointer >
bool ObjectIDPointerManagerT<Pointer>::get_pointer(int _identifier, Pointer & _pointer) {
  typename PointerObjectIDMap::iterator po_itr;
  typename PointerObjectIDMap::iterator po_itr_end = pointer_objectID_map_.end();

  po_itr =  pointer_objectID_map_.find(_identifier);

  // Last position reached = no element with ID _identifier in map
  if (po_itr == po_itr_end)
    return false;

  // Give pointer back
  _pointer = (Pointer)po_itr->second;
  return true;
}

//-----------------------------------------------------------------------------

template< class Pointer >
Pointer ObjectIDPointerManagerT<Pointer>::get_pointer(int _identifier) {
  Pointer tmp;
  if ( get_pointer(_identifier,tmp) )
    return tmp;
  else
    return NULL;
}

//-----------------------------------------------------------------------------

template< class Pointer >
bool ObjectIDPointerManagerT<Pointer>::add_pointer(int _identifier, const Pointer & _pointer) {
  typename PointerObjectIDMap::iterator po_itr;
  typename PointerObjectIDMap::iterator po_itr_end = pointer_objectID_map_.end();

  po_itr =  pointer_objectID_map_.find(_identifier);

  // Was already there
  if (po_itr != po_itr_end)
      return false;

  pointer_objectID_map_.insert(PointerObjectIDPair(_identifier, _pointer));
  return true;
}

//-----------------------------------------------------------------------------

template< class Pointer >
bool ObjectIDPointerManagerT<Pointer>::delete_object(int _identifier) {
  typename PointerObjectIDMap::iterator po_itr;
  typename PointerObjectIDMap::iterator po_itr_end = pointer_objectID_map_.end();

  po_itr =  pointer_objectID_map_.find(_identifier);

  // Last position reached = no element with ID _identifier in map
  if (po_itr == po_itr_end)
      return false;

  pointer_objectID_map_.erase(_identifier);
  return true;
}

//-----------------------------------------------------------------------------

template< class Pointer >
void ObjectIDPointerManagerT<Pointer>::get_object_ids(std::vector<int> & _ids)
{
  _ids.clear();

  typename PointerObjectIDMap::iterator po_itr;
  typename PointerObjectIDMap::iterator po_itr_end = pointer_objectID_map_.end();

  for (po_itr = pointer_objectID_map_.begin(); po_itr != po_itr_end; ++po_itr)
    _ids.push_back((*po_itr).first);
}

//-----------------------------------------------------------------------------

//=============================================================================
//=============================================================================
