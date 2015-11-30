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


#include "MeanCurvature.hh"

#include <OpenFlipper/BasePlugin/PluginFunctions.hh>
#include <ObjectTypes/PolyMesh/PolyMesh.hh>
#include <ObjectTypes/TriangleMesh/TriangleMesh.hh>

#include <MeshTools/Curvature.hh>

#ifdef USE_OPENMP
#endif

#if QT_VERSION >= 0x050000
#else
#include <QtGui>
#endif

MeanCurvaturePlugin::MeanCurvaturePlugin()
{
}


MeanCurvaturePlugin::~MeanCurvaturePlugin()
{
}


void MeanCurvaturePlugin::pluginsInitialized()
{
  emit addTexture( "Mean Curvature" , "mean_curvature.png" , 1 );
  emit setTextureMode("Mean Curvature","clamp=true,center=true,repeat=false,clamp_min=-20,clamp_max=20");

  emit setSlotDescription(tr("computeMeanCurvature(int)"), tr("Compute the mean curvature on a mesh. The curvature will be stored on the mesh on the vertex property called \"Mean Curvature\""),
        QStringList(tr("ObjectId")), QStringList(tr("Id of the mesh")));
}

void MeanCurvaturePlugin::slotUpdateTexture( QString _textureName , int _identifier )
{
   if ( _textureName != "Mean Curvature") {
      return;
  }

   BaseObjectData* object;
   if (! PluginFunctions::getObject(  _identifier , object ) ) {
      return;
   }

   if ( object->dataType( DATA_TRIANGLE_MESH ) ) {
      TriMesh* mesh = PluginFunctions::triMesh(object);
      computeMeanCurvature(mesh);
   }

   if ( object->dataType( DATA_POLY_MESH ) ) {
      PolyMesh* mesh = PluginFunctions::polyMesh(object);
      computeMeanCurvature(mesh);
   }

   emit updatedTextures("Mean Curvature",_identifier);
}

bool MeanCurvaturePlugin::computeMeanCurvature(int _objectId) {
  BaseObjectData* object;
  if (! PluginFunctions::getObject(  _objectId , object ) ) {
    return false;
  }

  if ( object->dataType( DATA_TRIANGLE_MESH ) ) {
    TriMesh* mesh = PluginFunctions::triMesh(object);
    computeMeanCurvature(mesh);
    return true;
  }

  if ( object->dataType( DATA_POLY_MESH ) ) {
    PolyMesh* mesh = PluginFunctions::polyMesh(object);
    computeMeanCurvature(mesh);
    return true;
  }

  return false;
}

template< typename MeshT >
void MeanCurvaturePlugin::computeMeanCurvature(MeshT* _mesh) {

  OpenMesh::VPropHandleT< double > mean;

  if(!_mesh->get_property_handle( mean, "Mean Curvature"))
    _mesh->add_property( mean, "Mean Curvature" );

  //QTime time;
  //time.start();
  std::vector< typename MeshT::VertexHandle > handles;
  handles.reserve(_mesh->n_vertices());
  for ( typename MeshT::VertexIter v_it = _mesh->vertices_begin() ; v_it != _mesh->vertices_end(); ++v_it)
    handles.push_back( *v_it );

  #ifdef USE_OPENMP
    #pragma omp parallel for
  #endif
  for ( int i = 0 ; i < (int)handles.size(); ++i ) {

    const typename MeshT::VertexHandle handle = handles[i];
    ACG::Vec3d curva(0.0,0.0,0.0);
    double area = 0.0;
    curvature::discrete_mean_curv_op<MeshT,ACG::Vec3d,double>(*_mesh,handle,curva,area);
    double curv  = curva.norm();

    if ( (curva | _mesh->normal(handle)) <0 )
      curv = -curv;

    _mesh->property(mean,handle) =  curv;
  }

}

#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2( meancurvatureplugin , MeanCurvaturePlugin );
#endif
