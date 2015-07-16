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
// CLASS ObjectIDPointerManagerT
//
//
// Author:  Henrik Zimmer <henrik@zimmer.to>
//
// Version: $Revision: 1$
// Date:    $Author$
//   $Date: XX-XX-200X$
//
//=============================================================================


#ifndef OBJECTIDPOINTERMANAGERT_HH
#define OBJECTIDPOINTERMANAGERT_HH


//== INCLUDES =================================================================

#include <map>
#include <vector>
#include <iostream>

#ifndef WIN32
#warning Included deprecated Header ObjectIDPointerManger
#warning You can attach data to objects by using the perObjectData class!
#warning Derive from perObjectData and use setObjectData on an Object
#warning to attach the data to it. You can read the data by using
#warning the objectData function of each object.
#endif

//== FORWARDDECLARATIONS ======================================================

//== NAMESPACES ===============================================================

//== CLASS DEFINITION =========================================================

/** Use this class to manage pointers associated with an object in the framework.
 */
template < class Pointer >
class ObjectIDPointerManagerT
{

public:

  /// typedef for the idmap
  typedef  std::map< int, Pointer > PointerObjectIDMap;

  /// typedef for the pairs
  typedef  std::pair< int, Pointer > PointerObjectIDPair;

  /// Constructor
  ObjectIDPointerManagerT() {}

  /// Destructor
  ~ObjectIDPointerManagerT() {}

  /** try to get a pointer with the given id
   * @param _identifier identifier of corresponding object
   * @param _pointer The returned pointer
   * @return true if object exists
   * */
  bool get_pointer(int _identifier, Pointer & _pointer);

  /** try to get a pointer with the given id
   * @param _identifier identifier of corresponding object
   * @return returns either a pointer or if the id does not exist NULL
   * */
  Pointer get_pointer(int _identifier);

  /** Add a pointer to the map
   * @param _identifier identifier of corresponding object
   * @param _pointer pointer to be added
   * */
  bool add_pointer(int _identifier, const Pointer & _pointer);

  /// Doesn't actually delete object (pointed to by pointer) only clears entry from the map
  bool delete_object(int _identifier);

  void get_object_ids(std::vector<int> & _ids);

private:

  /// Copy constructor (not used)
  ObjectIDPointerManagerT(const ObjectIDPointerManagerT& _rhs);

  /// Assignment operator (not used)
  ObjectIDPointerManagerT& operator=(const ObjectIDPointerManagerT& _rhs);

  /// mapping
  PointerObjectIDMap pointer_objectID_map_;
};


//=============================================================================
#if defined(INCLUDE_TEMPLATES) && !defined(OBJECTIDPOINTERMANAGERT_C)
#define OBJECTIDPOINTERMANAGERT_TEMPLATES
#include "ObjectIDPointerManagerT.cc"
#endif
//=============================================================================
#endif // OBJECTIDPOINTERMANAGERT_HH defined
//=============================================================================

