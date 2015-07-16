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

#include "GaussCurvature.hh"

#include <ACG/GL/GLState.hh>

#include <OpenFlipper/BasePlugin/PluginFunctions.hh>

#if QT_VERSION >= 0x050000 
  #include <QtWidgets>
#else
  #include <QtGui>
#endif

#include <MeshTools/Curvature.hh>

#include <ObjectTypes/PolyMesh/PolyMesh.hh>
#include <ObjectTypes/TriangleMesh/TriangleMesh.hh>

#ifdef USE_OPENMP
#include <omp.h>
#endif


GaussCurvaturePlugin::GaussCurvaturePlugin()
{
}


GaussCurvaturePlugin::~GaussCurvaturePlugin()
{
}



void GaussCurvaturePlugin::pluginsInitialized()
{
  emit addTexture( "Gaussian Curvature" , "gauss_curvature.png" , 1 );
  emit setTextureMode("Gaussian Curvature","clamp=true,clamp_min=-1,clamp_max=1,center=true");

  emit setSlotDescription(tr("computeGaussCurvature(int)"), tr("Compute the gaussian curvature on a mesh. The curvature will be stored on the mesh on the vertex property called \"Gaussian Curvature\""),
          QStringList(tr("ObjectId")), QStringList(tr("Id of the mesh")));
}

void GaussCurvaturePlugin::slotUpdateTexture( QString _textureName , int _identifier )
{
   if ( _textureName != "Gaussian Curvature")
      return;

   BaseObjectData* object;
   if (! PluginFunctions::getObject(  _identifier , object ) ) {
      return;
   }

   if ( object->dataType( DATA_TRIANGLE_MESH ) ) {
      TriMesh* mesh = PluginFunctions::triMesh(object);
      computeGaussianCurvature(mesh);
   }

   if ( object->dataType( DATA_POLY_MESH ) ) {
     PolyMesh* mesh = PluginFunctions::polyMesh(object);
      computeGaussianCurvature(mesh);
   }

   emit updatedTextures("Gaussian Curvature",_identifier);
}


bool GaussCurvaturePlugin::computeGaussCurvature(int _objectId) {
  BaseObjectData* object;
  if (! PluginFunctions::getObject(  _objectId , object ) ) {
    return false;
  }

  if ( object->dataType( DATA_TRIANGLE_MESH ) ) {
    TriMesh* mesh = PluginFunctions::triMesh(object);
    computeGaussianCurvature(mesh);
    return true;
  }

  if ( object->dataType( DATA_POLY_MESH ) ) {
    PolyMesh* mesh = PluginFunctions::polyMesh(object);
    computeGaussianCurvature(mesh);
    return true;
  }

  return false;
}


template< typename MeshT >
void GaussCurvaturePlugin::computeGaussianCurvature( MeshT* _mesh) {
  OpenMesh::VPropHandleT< double > gauss;

  if(!_mesh->get_property_handle( gauss, "Gaussian Curvature"))
    _mesh->add_property( gauss, "Gaussian Curvature" );

#ifdef USE_OPENMP
  std::vector<  typename MeshT::VertexHandle > handles;
  handles.reserve(_mesh->n_vertices());
  for ( typename MeshT::VertexIter v_it = _mesh->vertices_begin() ; v_it != _mesh->vertices_end(); ++v_it)
    handles.push_back( *v_it );


  #pragma omp parallel for
  for ( int i = 0 ; i < (int)handles.size(); ++i )
    _mesh->property(gauss,handles[i]) = curvature::gauss_curvature(*_mesh,handles[i]);

#else
  for ( typename MeshT::VertexIter v_it = _mesh->vertices_begin() ; v_it != _mesh->vertices_end(); ++v_it)
      _mesh->property(gauss,*v_it) =  curvature::gauss_curvature(*_mesh,*v_it);
#endif

}

#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2( gausscurvatureplugin , GaussCurvaturePlugin );
#endif

