/*===========================================================================*\
 *                                                                           *
 *                              OpenFlipper                                  *
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
 *                                                                           *
\*===========================================================================*/

/*===========================================================================*\
 *                                                                           *
 *   $Revision$                                                      *
 *   $Author$                                                       *
 *   $Date$                   *
 *                                                                           *
\*===========================================================================*/


//================================================================
//
//  SplatCloudSelection
//
//================================================================


#ifndef SPLATCLOUDSELECTION_HH
#define SPLATCLOUDSELECTION_HH


/*! \file SplatCloudSelection.hh
    \brief Functions for selection on a SplatCloud

*/

//== INCLUDES =================================================================


#include <ObjectTypes/SplatCloud/SplatCloud.hh>

#include <vector>


//== NAMESPACES ===============================================================


namespace SplatCloudSelection {


//== DEFINITIONS ==============================================================


//===========================================================================
/** @name Vertex Selection
* @{ */
//===========================================================================

void             selectVertices       (       SplatCloud *_splatCloud, const std::vector<int> &_vertices ); //!< Select given vertices.
void             unselectVertices     (       SplatCloud *_splatCloud, const std::vector<int> &_vertices ); //!< Unselect given vertices.
void             selectAllVertices    (       SplatCloud *_splatCloud                                    ); //!< Select all vertices.
void             clearVertexSelection (       SplatCloud *_splatCloud                                    ); //!< Unselect all vertices.
void             invertVertexSelection(       SplatCloud *_splatCloud                                    ); //!< Invert vertex selection.
std::vector<int> getVertexSelection   ( const SplatCloud *_splatCloud                                    ); //!< Get current vertex selection.
std::vector<int> getVertexSelection   ( const SplatCloud *_splatCloud, bool &_inverted                   ); //!< Get current vertex selection. If _inverted is true, vector is inverted to save memory.

/** @} */


//=============================================================================


} // namespace SplatCloudSelection


//=============================================================================


#endif // SPLATCLOUDSELECTION_HH
