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

#define TEXTURECONTROL_2D_TEXTURE_HANDLING_C

#include "TextureControl.hh"

template< typename MeshT >
void TextureControlPlugin::copyTexture ( Texture& /*_texture*/, MeshT& _mesh, OpenMesh::VPropHandleT< OpenMesh::Vec2d > _texProp )
{
  if ( !_mesh.has_vertex_texcoords2D() )
    _mesh.request_vertex_texcoords2D();
  
  for ( typename MeshT::VertexIter v_it = _mesh.vertices_begin(); v_it != _mesh.vertices_end(); ++v_it) {

    // Get the value of the property
    OpenMesh::Vec2d value = _mesh.property(_texProp, *v_it);
    
    // Write result to the openmesh texture coordinates
    _mesh.set_texcoord2D( *v_it, ACG::Vec2f(float(value[0]), float(value[1]) ) );
  }
}

template< typename MeshT >
void TextureControlPlugin::copyTexture ( Texture& /*_texture*/, MeshT& _mesh, OpenMesh::HPropHandleT< OpenMesh::Vec2d > _texProp )
{
  if ( !_mesh.has_halfedge_texcoords2D() )
    _mesh.request_halfedge_texcoords2D();
  
  for ( typename MeshT::HalfedgeIter h_it = _mesh.halfedges_begin(); h_it != _mesh.halfedges_end(); ++h_it) {

    // Get the value of the property
    OpenMesh::Vec2d value = _mesh.property(_texProp, *h_it);

    // Write result to the openmesh texture coordinates
    _mesh.set_texcoord2D( *h_it, ACG::Vec2f(float(value[0]), float(value[1]) ) );
  }
}


