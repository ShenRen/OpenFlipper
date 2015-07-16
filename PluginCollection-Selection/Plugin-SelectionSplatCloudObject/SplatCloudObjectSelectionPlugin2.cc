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
//  CLASS SplatCloudObjectSelectionPlugin - IMPLEMENTATION (continued)
//
//================================================================


//== INCLUDES ====================================================


#include "SplatCloudObjectSelectionPlugin.hh"

#include "SplatCloudSelection.hh"

#include <set>


//== IMPLEMENTATION ==============================================


/** \brief Delete all selected elements of a SplatCloud
 *
 * @param _splatCloud a SplatCloud
 * @param _primitiveType bitmask of active primitive types
 * @return returns true if the SplatCloud was changed (i.e there was something selected that was deleted)
 */
bool SplatCloudObjectSelectionPlugin::splatCloudDeleteSelection( SplatCloud *_splatCloud, PrimitiveType _primitiveType )
{
  if( (_primitiveType & vertexType_) == 0 )
    return false; // done

  if( _splatCloud == 0 )
    return false; // error

  unsigned int numDeleted = _splatCloud->eraseSplatsByIndex( SplatCloudSelection::getVertexSelection( _splatCloud ) );

  return (numDeleted != 0);
}


//----------------------------------------------------------------


/** \brief Toggle the selection state of SplatCloud primitives
 *
 * @param _splatCloud a SplatCloud
 * @param _index index of vertex that was picked
 * @param _hit_point point that was picked
 * @param _primitiveType primitive types to be selected
 */
void SplatCloudObjectSelectionPlugin::splatCloudToggleSelection( SplatCloud *_splatCloud, uint _index, ACG::Vec3d &_hit_point, PrimitiveType _primitiveType )
{
  if( (_primitiveType & vertexType_) == 0 )
    return; // done

  if( _splatCloud == 0 )
    return; // error

  if( /*(_index < 0) ||*/ (_index >= _splatCloud->numSplats()) )
    return; // error

  if( !_splatCloud->hasSelections() )
  {
    if( !_splatCloud->requestSelections() )
      return; // error

    unsigned int i, num = _splatCloud->numSplats();
    for( i=0; i<num; ++i )
      _splatCloud->selections( i ) = false;
  }

  _splatCloud->selections( _index ) = !_splatCloud->selections( _index );

  if( _splatCloud->selections( _index ) )
    emit scriptInfo( "selectVertices(ObjectId , [" + QString::number( _index ) + "])" );
  else
    emit scriptInfo( "unselectVertices(ObjectId , [" + QString::number( _index ) + "])" );
}


//----------------------------------------------------------------


/** \brief paint selection with a sphere
 *
 * @param _splatCloud a SplatCloud
 * @param _index index of vertex that was picked
 * @param _hit_point point that was picked
 * @param _radius selection sphere radius
 * @param _primitiveType primitive types to be selected
 * @param _deselection true, if entities should be deselected
 */
void SplatCloudObjectSelectionPlugin::splatCloudSphereSelection( SplatCloud *_splatCloud, uint _index, ACG::Vec3d &_hit_point, double _radius, PrimitiveType _primitiveType, bool _deselection )
{
  if( (_primitiveType & vertexType_) == 0 )
    return; // done

  if( _splatCloud == 0 )
    return; // error

  if( /*(_index < 0) ||*/ (_index >= _splatCloud->numSplats()) )
    return; // error

  if( !_splatCloud->hasPositions() )
    return; // error

  if( !_splatCloud->hasSelections() )
  {
    if( _deselection )
      return; // done

    if( !_splatCloud->requestSelections() )
      return; // error

    unsigned int i, num = _splatCloud->numSplats();
    for( i=0; i<num; ++i )
      _splatCloud->selections( i ) = false;
  }

  if( _radius < 0.0 )
    return; // done

  double sqrRadius = _radius * _radius;

  SplatCloud::Selection selection = !_deselection;

  unsigned int i, num = _splatCloud->numSplats();
  for( i=0; i<num; ++i )
  {
    const SplatCloud::Position &pos = _splatCloud->positions( i );

    double dx = pos[0] - _hit_point[0];
    double dy = pos[1] - _hit_point[1];
    double dz = pos[2] - _hit_point[2];

    double sqrDist = dx*dx + dy*dy + dz*dz;

    if( sqrDist <= sqrRadius )
      _splatCloud->selections( i ) = selection;
  }
}


//----------------------------------------------------------------


/** \brief Select all primitves that are projected to the given region
 *
 * @param _splatCloud a SplatCloud
 * @param _state current gl state
 * @param _region region
 * @param _primitiveType primitive types to be selected
 * @param _deselection true, if entities should be deselected
 * @return true, if something was selected
 */
bool SplatCloudObjectSelectionPlugin::splatCloudVolumeSelection( SplatCloud *_splatCloud, ACG::GLState &_state, QRegion *_region, PrimitiveType _primitiveType, bool _deselection )
{
  if( (_primitiveType & vertexType_) == 0 )
    return false; // done

  if( _splatCloud == 0 )
    return false; // error

  if( !_splatCloud->hasPositions() )
    return false; // error

  bool modify = true;

  if( !_splatCloud->hasSelections() )
  {
    if( _deselection )
    {
      modify = false;
    }
    else
    {
      if( !_splatCloud->requestSelections() )
        return false; // error

      unsigned int i, num = _splatCloud->numSplats();
      for( i=0; i<num; ++i )
        _splatCloud->selections( i ) = false;
    }
  }

  bool rv = false;

  SplatCloud::Selection selection = !_deselection;

  int i, num = static_cast<int>(_splatCloud->numSplats());
#ifdef USE_OPENMP
    #pragma omp parallel for
#endif
  for( i=0; i<num; ++i )
  {
    const SplatCloud::Position &pos = _splatCloud->positions( i );

    ACG::Vec3d proj = _state.project( ACG::Vec3d( pos[0], pos[1], pos[2] ) );

    if( _region->contains( QPoint( (int) proj[0], _state.context_height() - (int) proj[1] ) ) )
    {
      if( modify )
        _splatCloud->selections( i ) = selection;

      rv = true;
    }
  }

  return rv;
}


//----------------------------------------------------------------


/** \brief Colorize a selection
 *
 * @param _splatCloud a SplatCloud
 * @param _primitiveTypes selection types
 * @param _r red   rgba color-component
 * @param _g green rgba color-component
 * @param _b blue  rgba color-component
 * @param _a blue  rgba color-component
 */
void SplatCloudObjectSelectionPlugin::splatCloudColorizeSelection( SplatCloud *_splatCloud, PrimitiveType _primitiveTypes, int _r, int _g, int _b, int _a )
{
  if( (_primitiveTypes & vertexType_) == 0 )
    return; // done

  if( _splatCloud == 0 )
    return; // error

  if( !_splatCloud->hasColors() )
  {
    if( !_splatCloud->requestColors() )
      return; // error

    SplatCloud::Color black( 0, 0, 0 );

    unsigned int i, num = _splatCloud->numSplats();
    for( i=0; i<num; ++i )
      _splatCloud->colors( i ) = black;
  }

  if( !_splatCloud->hasSelections() )
    return; // done

  unsigned char r = (_r < 0) ? 0 : (_r > 255) ? 255 : (unsigned char) _r;
  unsigned char g = (_g < 0) ? 0 : (_g > 255) ? 255 : (unsigned char) _g;
  unsigned char b = (_b < 0) ? 0 : (_b > 255) ? 255 : (unsigned char) _b;

  SplatCloud::Color color( r, g, b ); // drop alpha

  unsigned int i, num = _splatCloud->numSplats();
  for( i=0; i<num; ++i )
  {
    if( _splatCloud->selections( i ) )
      _splatCloud->colors( i ) = color;
  }
}
