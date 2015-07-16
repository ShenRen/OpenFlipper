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


#include "LaplacePlugin.hh"

#include <iostream>
#include <ACG/GL/GLState.hh>

#include "OpenFlipper/BasePlugin/PluginFunctions.hh"
#include <ObjectTypes/PolyMesh/PolyMesh.hh>
#include <ObjectTypes/TriangleMesh/TriangleMesh.hh>

#ifdef USE_OPENMP
#include <omp.h>
#endif

#if QT_VERSION >= 0x050000 
#include <QtWidgets>
#else
#include <QtGui>
#endif

#define UNIFORM_LAPLACE_NAME "Uniform Laplace Length"
#define UNIFORM_LAPLACE_SQUARED_NAME "Uniform Laplace Squared Length"

void LaplaceLengthPlugin::pluginsInitialized()
{
  emit addTexture( UNIFORM_LAPLACE_NAME , "laplace_length.png" , 1 );
  emit setTextureMode(UNIFORM_LAPLACE_NAME,"clamp=true,center=true,repeat=false,clamp_min=-20,clamp_max=20");

  emit addTexture( UNIFORM_LAPLACE_SQUARED_NAME , "laplace_length.png" , 1 );
  emit setTextureMode(UNIFORM_LAPLACE_SQUARED_NAME,"clamp=true,center=true,repeat=false,clamp_min=-20,clamp_max=20");
}

void LaplaceLengthPlugin::slotUpdateTexture( QString _textureName , int _identifier )
{
   if ( (_textureName != UNIFORM_LAPLACE_SQUARED_NAME) && (_textureName != UNIFORM_LAPLACE_NAME ) ) {
      return;
   }

   BaseObjectData* object;
   if (! PluginFunctions::getObject(  _identifier , object ) ) {
      return;
   }

   if ( object->dataType( DATA_TRIANGLE_MESH ) ) {
      TriMesh* mesh = PluginFunctions::triMesh(object);
      if ( _textureName == UNIFORM_LAPLACE_NAME ) {
         computeLaplaceLength(mesh);
         emit updatedTextures(UNIFORM_LAPLACE_NAME,_identifier);
      }
      if ( _textureName == UNIFORM_LAPLACE_SQUARED_NAME ) {
         computeLaplaceSquaredLength(mesh);
         emit updatedTextures(UNIFORM_LAPLACE_SQUARED_NAME,_identifier);
      }
   }

   if ( object->dataType( DATA_POLY_MESH ) ) {
      PolyMesh* mesh = PluginFunctions::polyMesh(object);
      if ( _textureName == UNIFORM_LAPLACE_NAME ) {
         computeLaplaceLength(mesh);
         emit updatedTextures(UNIFORM_LAPLACE_NAME,_identifier);
      }
      if ( _textureName == UNIFORM_LAPLACE_SQUARED_NAME ) {
         computeLaplaceSquaredLength(mesh);
         emit updatedTextures(UNIFORM_LAPLACE_SQUARED_NAME,_identifier);
      }
   }
}

template< typename MeshT >
void LaplaceLengthPlugin::computeLaplaceLength(MeshT* _mesh) {
  OpenMesh::VPropHandleT< ACG::Vec3d > laplace_vector_property;
  OpenMesh::VPropHandleT< double > laplace_length_property;

  if(!_mesh->get_property_handle( laplace_vector_property , "Laplace Vector" ))
    _mesh->add_property( laplace_vector_property, "Laplace Vector" );

  if(!_mesh->get_property_handle( laplace_length_property , UNIFORM_LAPLACE_NAME ))
    _mesh->add_property( laplace_length_property, UNIFORM_LAPLACE_NAME );


  QTime time;
  time.start();
  std::vector< typename MeshT::VertexHandle > handles;
  handles.reserve(_mesh->n_vertices());
  for ( typename MeshT::VertexIter v_it = _mesh->vertices_begin() ; v_it != _mesh->vertices_end(); ++v_it)
    handles.push_back( *v_it );

  #ifdef USE_OPENMP
    #pragma omp parallel for
  #endif
  for ( int i = 0 ; i < (int)handles.size(); ++i ) {
    const typename MeshT::VertexHandle handle = handles[i];

    ACG::Vec3d laplace(0.0,0.0,0.0);
    for ( typename MeshT::VertexVertexIter vv_it(*_mesh , handle) ; vv_it.is_valid() ; ++vv_it )
        laplace += _mesh->point(*vv_it) - _mesh->point(handle);

    laplace = 1.0 /(double)_mesh->valence(handle) * laplace;
    _mesh->property(laplace_vector_property,handle) = laplace;
    _mesh->property(laplace_length_property,handle) = laplace.norm();
  }

//   #ifdef USE_OPENMP
//     std::cerr << "Laplace parallel took : " << time.elapsed() << std::endl;
//   #else
//     std::cerr << "Laplace sequential took : " << time.elapsed() << std::endl;
//   #endif

}

template< typename MeshT >
void LaplaceLengthPlugin::computeLaplaceSquaredLength(MeshT* _mesh) {
  computeLaplaceLength(_mesh);

  OpenMesh::VPropHandleT< ACG::Vec3d > laplace_property;
  OpenMesh::VPropHandleT< double > laplace_squared;

  if(!_mesh->get_property_handle( laplace_property , "Laplace Vector" )) {
    std::cerr << "LaplaceLengthPlugin : Unable to get Laplace Vector property" << std::endl;
    return;
  }

  if(!_mesh->get_property_handle( laplace_squared , UNIFORM_LAPLACE_SQUARED_NAME ))
    _mesh->add_property( laplace_squared, UNIFORM_LAPLACE_SQUARED_NAME );

  QTime time;
  time.start();
  std::vector< typename MeshT::VertexHandle > handles;
  handles.reserve(_mesh->n_vertices());
  for ( typename MeshT::VertexIter v_it = _mesh->vertices_begin() ; v_it != _mesh->vertices_end(); ++v_it)
    handles.push_back( *v_it );

  #ifdef USE_OPENMP
    #pragma omp parallel for
  #endif
  for ( int i = 0 ; i < (int)handles.size(); ++i ) {
    const typename MeshT::VertexHandle handle = handles[i];

    ACG::Vec3d laplace(0.0,0.0,0.0);
    for ( typename MeshT::VertexVertexIter vv_it(*_mesh , handle) ; vv_it.is_valid() ; ++vv_it )
        laplace += _mesh->property(laplace_property,*vv_it) - _mesh->property(laplace_property,handle);
    laplace = 1.0 /(double)_mesh->valence(handle) * laplace;
    _mesh->property(laplace_squared,handle) = laplace.norm();
  }

//   #ifdef USE_OPENMP
//     std::cerr << "Laplace Squared  parallel took : " << time.elapsed() << std::endl;
//   #else
//     std::cerr << "Laplace Squared sequential took : " << time.elapsed() << std::endl;
//   #endif
}

#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2( laplacelengthplugin , LaplaceLengthPlugin );
#endif



