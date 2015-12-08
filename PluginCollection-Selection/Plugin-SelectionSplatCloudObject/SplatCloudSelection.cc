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
//  SplatCloudSelection - IMPLEMENTATION
//
//================================================================


//== INCLUDES =================================================================


#include "SplatCloudSelection.hh"


//== NAMESPACES ===============================================================


namespace SplatCloudSelection {


//== IMPLEMENTATION ==========================================================


//=========================================================
//== Vertex Selection =====================================
//=========================================================


void selectVertices( SplatCloud *_splatCloud, const std::vector<int> &_vertices )
{
  if( _splatCloud == 0 )
    return; // error

  if( _vertices.empty() )
    return; // done

  if( !_splatCloud->hasSelections() )
  {
    if( !_splatCloud->requestSelections() )
      return; // error

    unsigned int i, num = _splatCloud->numSplats();
    for( i=0; i<num; ++i )
      _splatCloud->selections( i ) = false;
  }

  int n_vertices = (int) _splatCloud->numSplats();

  unsigned int i;
  for( i=0; i<_vertices.size(); ++i )
  {
    int v = _vertices[ i ];
    if( (v >= 0) && (v < n_vertices) )
      _splatCloud->selections( v ) = true;
  }
}


//----------------------------------------------------------------


void unselectVertices( SplatCloud *_splatCloud, const std::vector<int> &_vertices )
{
  if( _splatCloud == 0 )
    return; // error

  if( _vertices.empty() )
    return; // done

  if( !_splatCloud->hasSelections() )
    return; // done

  int n_vertices = (int) _splatCloud->numSplats();

  unsigned int i;
  for( i=0; i<_vertices.size(); ++i )
  {
    int v = _vertices[ i ];
    if( (v >= 0) && (v < n_vertices) )
      _splatCloud->selections( v ) = false;
  }
}


//----------------------------------------------------------------


void selectAllVertices( SplatCloud *_splatCloud )
{
  if( _splatCloud == 0 )
    return; // error

  if( !_splatCloud->hasSelections() )
  {
    if( !_splatCloud->requestSelections() )
      return; // error
  }

  unsigned int i, num = _splatCloud->numSplats();
  for( i=0; i<num; ++i )
    _splatCloud->selections( i ) = true;
}


//----------------------------------------------------------------


void clearVertexSelection( SplatCloud *_splatCloud )
{
  if( _splatCloud == 0 )
    return; // error

  if( !_splatCloud->hasSelections() )
    return; // done

  unsigned int i, num = _splatCloud->numSplats();
  for( i=0; i<num; ++i )
    _splatCloud->selections( i ) = false;
}


//----------------------------------------------------------------


void invertVertexSelection( SplatCloud *_splatCloud )
{
  if( _splatCloud == 0 )
    return; // error

  if( _splatCloud->hasSelections() )
  {
    unsigned int i, num = _splatCloud->numSplats();
    for( i=0; i<num; ++i )
      _splatCloud->selections( i ) = !_splatCloud->selections( i );
  }
  else
  {
    if( !_splatCloud->requestSelections() )
      return; // error

    unsigned int i, num = _splatCloud->numSplats();
    for( i=0; i<num; ++i )
      _splatCloud->selections( i ) = true;
  }
}


//----------------------------------------------------------------


static unsigned int countSelected( const SplatCloud *_splatCloud )
{
  unsigned int count = 0;

  unsigned int i, num = _splatCloud->numSplats();
  for( i=0; i<num; ++i )
  {
    if( _splatCloud->selections( i ) )
      ++count;
  }

  return count;
}


//----------------------------------------------------------------


std::vector<int> getVertexSelection( const SplatCloud *_splatCloud )
{
  std::vector<int> vertices;

  if( _splatCloud == 0 )
    return vertices; // error

  if( !_splatCloud->hasSelections() )
    return vertices; // done

  unsigned int numSelected = countSelected( _splatCloud );

  vertices.reserve( numSelected );

  unsigned int i, num = _splatCloud->numSplats();
  for( i=0; i<num; ++i )
  {
    if( _splatCloud->selections( i ) )
      vertices.push_back( i );
  }

  return vertices;
}


//----------------------------------------------------------------


std::vector<int> getVertexSelection( const SplatCloud *_splatCloud, bool &_inverted )
{
  _inverted = false;

  std::vector<int> vertices;

  if( _splatCloud == 0 )
    return vertices; // error

  if( !_splatCloud->hasSelections() )
    return vertices; // done

  unsigned int numSelected   = countSelected( _splatCloud );
  unsigned int numUnselected = _splatCloud->numSplats() - numSelected;

  if( numSelected <= numUnselected )
  {
    vertices.reserve( numSelected );

    unsigned int i, num = _splatCloud->numSplats();
    for( i=0; i<num; ++i )
    {
      if( _splatCloud->selections( i ) )
        vertices.push_back( i );
    }
  }
  else
  {
    _inverted = true;

    vertices.reserve( numUnselected );

    unsigned int i, num = _splatCloud->numSplats();
    for( i=0; i<num; ++i )
    {
      if( !_splatCloud->selections( i ) )
        vertices.push_back( i );
    }
  }

  return vertices;
}


//=============================================================================


} // namespace SplatCloudSelection


//=============================================================================
