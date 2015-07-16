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

//================================================================
//
//  CLASS SplatCloud - IMPLEMENTATION
//
//================================================================


//== INCLUDES ====================================================


#include "SplatCloud.hh"

#include <iostream>


//== STATIC MEMBERS ==============================================


const SplatCloud::PositionsHandle  SplatCloud::POSITIONS_HANDLE ( "<Positions>"  );
const SplatCloud::ColorsHandle     SplatCloud::COLORS_HANDLE    ( "<Colors>"     );
const SplatCloud::NormalsHandle    SplatCloud::NORMALS_HANDLE   ( "<Normals>"    );
const SplatCloud::PointsizesHandle SplatCloud::POINTSIZES_HANDLE( "<Pointsizes>" );
const SplatCloud::IndicesHandle    SplatCloud::INDICES_HANDLE   ( "<Indices>"    );
const SplatCloud::ViewlistsHandle  SplatCloud::VIEWLISTS_HANDLE ( "<Viewlists>"  );
const SplatCloud::SelectionsHandle SplatCloud::SELECTIONS_HANDLE( "<Selections>" );


//== IMPLEMENTATION ==============================================


void SplatCloud::copySplatProperties( const SplatCloud &_splatCloud )
{
  // deep copy all splat-properties
  SplatPropertyMap::const_iterator splatPropertyIter;
  for( splatPropertyIter = _splatCloud.splatProperties_.begin(); splatPropertyIter != _splatCloud.splatProperties_.end(); ++splatPropertyIter )
  {
    // create new deep copy of current splat-property
    SplatPropertyInterface *prop = splatPropertyIter->second.property_->clone();

    // check if out of memory
    if( prop == 0 )
    {
      std::cerr << "Out of memory for a copy of SplatCloud's splat-property \"" << splatPropertyIter->first << "\"." << std::endl;
      continue;
    }

    // insert new property map entry into splat-property map with same name as before
    splatProperties_[ splatPropertyIter->first ] = SplatPropertyMapEntry( prop, splatPropertyIter->second.numRequests_ );
  }

  // Get pointers to predefined splat-properties.
  // These can *not* be copied because they have to point to the newly
  // created deep copies of the properties and not to the old properties.
  getPredefinedSplatPropertyPointers();
}


//----------------------------------------------------------------


void SplatCloud::copyCloudProperties( const SplatCloud &_splatCloud )
{
  // deep copy all cloud-properties
  CloudPropertyMap::const_iterator cloudPropertyIter;
  for( cloudPropertyIter = _splatCloud.cloudProperties_.begin(); cloudPropertyIter != _splatCloud.cloudProperties_.end(); ++cloudPropertyIter )
  {
    // create new deep copy of current cloud-property
    CloudPropertyInterface *prop = cloudPropertyIter->second.property_->clone();

    // check if out of memory
    if( prop == 0 )
    {
      std::cerr << "Out of memory for a copy of SplatCloud's cloud-property \"" << cloudPropertyIter->first << "\"." << std::endl;
      continue;
    }

    // insert new property map entry into cloud-property map with same name as before
    cloudProperties_[ cloudPropertyIter->first ] = CloudPropertyMapEntry( prop, cloudPropertyIter->second.numRequests_ );
  }

  // Get pointers to predefined cloud-properties.
  // These can *not* be copied because they have to point to the newly
  // created deep copies of the properties and not to the old properties.
  getPredefinedCloudPropertyPointers();
}


//----------------------------------------------------------------


SplatCloud::SplatCloud( const SplatCloud &_splatCloud )
{
  // copy number of splats
  numSplats_ = _splatCloud.numSplats_;

  // deep copy all properties
  copySplatProperties( _splatCloud );
  copyCloudProperties( _splatCloud );
}


//----------------------------------------------------------------


void SplatCloud::clearSplatProperties()
{
  // free memory of all splat-properties
  SplatPropertyMap::const_iterator splatPropertyIter;
  for( splatPropertyIter = splatProperties_.begin(); splatPropertyIter != splatProperties_.end(); ++splatPropertyIter )
    delete splatPropertyIter->second.property_;

  // clear splat-property map
  SplatPropertyMap().swap( splatProperties_ );

  // reset pointers to predefined splat-properties
  resetPredefinedSplatPropertyPointers();
}


//----------------------------------------------------------------


void SplatCloud::clearCloudProperties()
{
  // free memory of all cloud-properties
  CloudPropertyMap::const_iterator cloudPropertyIter;
  for( cloudPropertyIter = cloudProperties_.begin(); cloudPropertyIter != cloudProperties_.end(); ++cloudPropertyIter )
    delete cloudPropertyIter->second.property_;

  // clear cloud-property map
  CloudPropertyMap().swap( cloudProperties_ );

  // reset pointers to predefined cloud-properties
  resetPredefinedCloudPropertyPointers();
}


//----------------------------------------------------------------


void SplatCloud::clear()
{
  // reset number of splats
  numSplats_ = 0;

  // clear all properties
  clearSplatProperties();
  clearCloudProperties();
}


//----------------------------------------------------------------


void SplatCloud::swap( SplatCloud &_splatCloud )
{
  // swap number of splats
  std::swap( numSplats_,       _splatCloud.numSplats_       );

  // swap all properties
  std::swap( splatProperties_, _splatCloud.splatProperties_ );
  std::swap( cloudProperties_, _splatCloud.cloudProperties_ );

  // swap pointers to predefined properties
  swapPredefinedSplatPropertyPointers( _splatCloud );
  swapPredefinedCloudPropertyPointers( _splatCloud );
}


//----------------------------------------------------------------


void SplatCloud::clearSplats()
{
  // clear data vector of all splat-properties
  SplatPropertyMap::const_iterator splatPropertyIter;
  for( splatPropertyIter = splatProperties_.begin(); splatPropertyIter != splatProperties_.end(); ++splatPropertyIter )
    splatPropertyIter->second.property_->clear();

  // reset number of splats
  numSplats_ = 0;
}


//----------------------------------------------------------------


void SplatCloud::pushbackSplat()
{
  // add one element at end of data vector of all splat-properties
  SplatPropertyMap::const_iterator splatPropertyIter;
  for( splatPropertyIter = splatProperties_.begin(); splatPropertyIter != splatProperties_.end(); ++splatPropertyIter )
    splatPropertyIter->second.property_->pushback();

  // increase number of splats
  ++numSplats_;
}


//----------------------------------------------------------------


void SplatCloud::resizeSplats( unsigned int _num )
{
  // resize data vector of all splat-properties
  SplatPropertyMap::const_iterator splatPropertyIter;
  for( splatPropertyIter = splatProperties_.begin(); splatPropertyIter != splatProperties_.end(); ++splatPropertyIter )
    splatPropertyIter->second.property_->resize( _num );

  // update number of splats
  numSplats_ = _num;
}
