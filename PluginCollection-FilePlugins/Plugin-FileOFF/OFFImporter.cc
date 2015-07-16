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

#include "OFFImporter.hh"

#include <OpenMesh/Core/Utils/vector_cast.hh>

//-----------------------------------------------------------------------------

/// base class needs virtual destructor
OFFImporter::~OFFImporter(){
  
}

//-----------------------------------------------------------------------------

/// constructor
OFFImporter::OFFImporter() :
polyMesh_(0),
triMesh_(0),
object_(0),
objectOptions_(0),
maxFaceValence_(0) {}

//-----------------------------------------------------------------------------

/// add a mesh
void OFFImporter::addObject( BaseObject* _object ){
    
    PolyMeshObject* polyMeshObj = dynamic_cast< PolyMeshObject* > (_object);
    TriMeshObject*  triMeshObj  = dynamic_cast< TriMeshObject*  > (_object);
    
    if ( polyMeshObj ){
        
      polyMesh_ = polyMeshObj->mesh();
      object_ = _object;
      objectOptions_ |= POLYMESH;
      objectOptions_ &= ~TRIMESH;

    } else if ( triMeshObj ){
      
      triMesh_ = triMeshObj->mesh();
      object_ = _object;
      objectOptions_ |= TRIMESH;
      objectOptions_ &= ~POLYMESH;
        
    } else {
        std::cerr << "Error: Cannot add object. Type is unknown!" << std::endl;
    }
}

//-----------------------------------------------------------------------------

/// get vertex with given index
Vec3f OFFImporter::vertex(uint _index){
  
  if ( vertices_.size() > _index )
    return vertices_[ _index ];
  else
    return Vec3f();
}

//-----------------------------------------------------------------------------

/// add texture coordinates
int OFFImporter::addTexCoord(const Vec2f& _coord){
  texCoords_.push_back( _coord );
  
  return texCoords_.size()-1;
}

//-----------------------------------------------------------------------------

/// add a normal
int OFFImporter::addNormal(const Vec3f& _normal){
  normals_.push_back( _normal );
  
  return normals_.size()-1;
}

//-----------------------------------------------------------------------------

/// add a color
int OFFImporter::addColor(const Vec4f& _color) {
    colors_.push_back( _color );
    
    return colors_.size()-1;
}

//-----------------------------------------------------------------------------

/// get mesh as polyMesh
PolyMesh* OFFImporter::polyMesh(){
  if (polyMesh_ == 0)
    return 0;
  else
    return polyMesh_;
}

//-----------------------------------------------------------------------------

/// get mesh as triMesh
TriMesh* OFFImporter::triMesh(){
  if (triMesh_ == 0)
    return 0;
  else
    return triMesh_;
}

//-----------------------------------------------------------------------------

/// set vertex texture coordinate
void OFFImporter::setVertexTexCoord(VertexHandle _vh, int _texCoordID){
  
  if ( isTriangleMesh() ){
  
    //handle triangle meshes
    if ( !triMesh() ) return;
    
    if ( _texCoordID < (int) texCoords_.size() ){
    
      //perhaps request texCoords for the mesh
      if ( !triMesh()->has_vertex_texcoords2D() )
        triMesh()->request_vertex_texcoords2D();
      
      if ( vertexMapTri_.find( _vh ) != vertexMapTri_.end() )
      triMesh()->set_texcoord2D( vertexMapTri_[_vh], texCoords_[ _texCoordID ] );
      objectOptions_ |= VERTEXTEXCOORDS;

    }else{
      std::cerr << "Error: TexCoord ID too large" << std::endl;
    }
  
  } else if ( isPolyMesh() ){
    
    //handle poly meshes
    if ( !polyMesh() ) return;
    
    if ( _texCoordID < (int) texCoords_.size() ){
    
      //perhaps request texCoords for the mesh
      if ( !polyMesh()->has_vertex_texcoords2D() )
        polyMesh()->request_vertex_texcoords2D();
      
      if ( vertexMapPoly_.find( _vh ) != vertexMapPoly_.end() )
      polyMesh()->set_texcoord2D( vertexMapPoly_[_vh], texCoords_[ _texCoordID ] ); 
      objectOptions_ |= VERTEXTEXCOORDS;

    }else{
      std::cerr << "Error: TexCoord ID too large" << std::endl;
    }
  }
}

//-----------------------------------------------------------------------------

/// set vertex normal
void OFFImporter::setNormal(VertexHandle _vh, int _normalID){
  
  if ( isTriangleMesh() ){
  
    //handle triangle meshes
    if ( !triMesh() ) return;
    
    if ( _normalID < (int) normals_.size() ){
    
      if ( vertexMapTri_.find( _vh ) != vertexMapTri_.end() ){
        triMesh()->set_normal( vertexMapTri_[_vh], (TriMesh::Point) normals_[ _normalID ] ); 
        objectOptions_ |= VERTEXNORMAL;
      }

    }else{
      std::cerr << "Error: normal ID too large" << std::endl;
    }
  
  } else if ( isPolyMesh() ){
    
    //handle poly meshes
    if ( !polyMesh() ) return;
    
    if ( _normalID < (int) normals_.size() ){
    
      if ( vertexMapPoly_.find( _vh ) != vertexMapPoly_.end() ){
        polyMesh()->set_normal( vertexMapPoly_[_vh], (PolyMesh::Point) normals_[ _normalID ] ); 
        objectOptions_ |= VERTEXNORMAL;
      }

    }else{
      std::cerr << "Error: normal ID too large" << std::endl;
    }
    
  }
}

//-----------------------------------------------------------------------------

/// add a vertex with coordinate \c _point
VertexHandle OFFImporter::addVertex(const Vec3f& _point){
    
    vertices_.push_back( _point );
    
    int id = vertices_.size()-1;
    
    if ( isTriangleMesh() ){
        
        //handle triangle meshes
        if ( !triMesh() ) return false;
        vertexMapTri_[ id ] = triMesh()->add_vertex( (TriMesh::Point) vertices_[id] );
        
    } else if ( isPolyMesh() ){
        
        //handle poly meshes
        if ( !polyMesh() ) return false;
        vertexMapPoly_[ id ] = polyMesh()->add_vertex( (PolyMesh::Point) vertices_[id] );
    }
    
    return id;
}

//-----------------------------------------------------------------------------

/// add a face with indices _indices refering to vertices
int OFFImporter::addFace(const VHandles& _indices) {
   
  int faceIndex = -1;
    
  if ( isTriangleMesh() ){
  
    //handle triangle meshes
    if ( !triMesh() ) return -1;
    
    std::vector< TriMesh::VertexHandle > vertices;
    
    for (uint i=0; i < _indices.size(); i++){

      if ( vertexMapTri_.find( _indices[i] ) != vertexMapTri_.end() ){
    
        vertices.push_back( vertexMapTri_[ _indices[i] ] );

      } else {
        std::cerr << "Error: Cannot add face. Undefined index (" <<  _indices[i] << ")" << std::endl;
        return -1;
      }
    }
    
    TriMesh::FaceHandle fh = triMesh()->add_face( vertices ); 
    
    if(fh.is_valid()) {
        faceMapTri_.push_back( fh );
        faceIndex = faceMapTri_.size()-1;
    } else {
        // Store non-manifold face
        invalidFaces_.push_back( vertices );
    }
    
  } else if ( isPolyMesh() ){
  
    //handle poly meshes
    if ( !polyMesh() ) return -1;
    
    std::vector< PolyMesh::VertexHandle > vertices;
    
    for (uint i=0; i < _indices.size(); i++){
      
      if ( vertexMapPoly_.find( _indices[i] ) != vertexMapPoly_.end() ){
    
        vertices.push_back( vertexMapPoly_[ _indices[i] ] );

      }else{
        std::cerr << "Error: Cannot add face. Undefined index (" <<  _indices[i] << ")" << std::endl;
        return -1;
      }
    }
    
    PolyMesh::FaceHandle fh = polyMesh()->add_face( vertices );
    
    if(fh.is_valid()) {
        faceMapPoly_.push_back(fh);
        faceIndex = faceMapPoly_.size()-1;
    } else {
        // Store non-manifold face vertices
        invalidFaces_.push_back( vertices );
    }
  }
  
  return faceIndex;
}

//-----------------------------------------------------------------------------

void OFFImporter::finish() {
    
    if(invalidFaces_.empty()) return;
    
    if ( isTriangleMesh() ) {
        
        for(std::vector<OMVHandles>::iterator it = invalidFaces_.begin();
                it != invalidFaces_.end(); ++it) {

            OMVHandles& vhandles = *it;

            // double vertices
            for (unsigned int j = 0; j < vhandles.size(); ++j)
            {
              TriMesh::Point p = triMesh()->point(vhandles[j]);
              vhandles[j] = triMesh()->add_vertex(p);
              // DO STORE p, reference may not work since vertex array
              // may be relocated after adding a new vertex !

              // Mark vertices of failed face as non-manifold
              if (triMesh()->has_vertex_status()) {
                  triMesh()->status(vhandles[j]).set_fixed_nonmanifold(true);
              }
            }

            // add face
            OpenMesh::FaceHandle fh = triMesh()->add_face(vhandles);

            // Mark failed face as non-manifold
            if (triMesh()->has_face_status())
                triMesh()->status(fh).set_fixed_nonmanifold(true);

            // Mark edges of failed face as non-two-manifold
            if (triMesh()->has_edge_status()) {
                TriMesh::FaceEdgeIter fe_it = triMesh()->fe_iter(fh);
                for(; fe_it.is_valid(); ++fe_it) {
                    triMesh()->status(*fe_it).set_fixed_nonmanifold(true);
                }
            }
            
            faceMapTri_.push_back(fh);
        }
        
    } else if ( isPolyMesh() ) {
        
        for(std::vector<OMVHandles>::iterator it = invalidFaces_.begin();
                it != invalidFaces_.end(); ++it) {

            OMVHandles& vhandles = *it;

            // double vertices
            for (unsigned int j = 0; j < vhandles.size(); ++j)
            {
              TriMesh::Point p = polyMesh()->point(vhandles[j]);
              vhandles[j] = polyMesh()->add_vertex(p);
              // DO STORE p, reference may not work since vertex array
              // may be relocated after adding a new vertex !

              // Mark vertices of failed face as non-manifold
              if (polyMesh()->has_vertex_status()) {
                  polyMesh()->status(vhandles[j]).set_fixed_nonmanifold(true);
              }
            }

            // add face
            OpenMesh::FaceHandle fh = polyMesh()->add_face(vhandles);

            // Mark failed face as non-manifold
            if (polyMesh()->has_face_status())
                polyMesh()->status(fh).set_fixed_nonmanifold(true);

            // Mark edges of failed face as non-two-manifold
            if (polyMesh()->has_edge_status()) {
                TriMesh::FaceEdgeIter fe_it = polyMesh()->fe_iter(fh);
                for(; fe_it.is_valid(); ++fe_it) {
                    polyMesh()->status(*fe_it).set_fixed_nonmanifold(true);
                }
            }
            
            faceMapPoly_.push_back(fh);
        }
    }
    
    // Clear all invalid faces
    invalidFaces_.clear();
}

//-----------------------------------------------------------------------------

bool OFFImporter::isTriangleMesh(){
  return objectOptions_ & TRIMESH;
}

//-----------------------------------------------------------------------------

bool OFFImporter::isPolyMesh(){
  return objectOptions_ & POLYMESH;
}

//-----------------------------------------------------------------------------

bool OFFImporter::isBinary(){
    return objectOptions_ & BINARY;
}

//-----------------------------------------------------------------------------

bool OFFImporter::hasVertexNormals(){
  
  return objectOptions_ & VERTEXNORMAL;
}

//-----------------------------------------------------------------------------

bool OFFImporter::hasTextureCoords(){
  return objectOptions_ & VERTEXTEXCOORDS;
}

//-----------------------------------------------------------------------------

bool OFFImporter::hasVertexColors() {
    return objectOptions_ & VERTEXCOLOR;
}

//-----------------------------------------------------------------------------

bool OFFImporter::hasFaceColors() {
    return objectOptions_ & FACECOLOR;
}

//-----------------------------------------------------------------------------

bool OFFImporter::hasOption(ObjectOptionsE _option) {
    return objectOptions_ & _option;
}

//-----------------------------------------------------------------------------

uint OFFImporter::n_vertices(){
  return vertices_.size();
}

//-----------------------------------------------------------------------------

uint OFFImporter::n_normals(){
  return normals_.size();
}

//-----------------------------------------------------------------------------

uint OFFImporter::n_texCoords(){
  return texCoords_.size();
}

//-----------------------------------------------------------------------------

void OFFImporter::reserve(unsigned int _nv, unsigned int _ne, unsigned int _nf) {

    vertices_.reserve(_nv);
    normals_.reserve(_nv);
    texCoords_.reserve(_nv);
    colors_.reserve(_nv);

    if(isPolyMesh() && polyMesh_ != 0) {
        polyMesh_->reserve(_nv, _ne, _nf);
    }

    if(isTriangleMesh() && triMesh_ != 0) {
        triMesh_->reserve(_nv, _ne, _nf);
    }
}

//-----------------------------------------------------------------------------

BaseObject* OFFImporter::getObject(){
  
    return object_;
}

//-----------------------------------------------------------------------------

QString OFFImporter::path(){
  return path_;
}

//-----------------------------------------------------------------------------

void OFFImporter::setPath(QString _path){
  path_ = _path;
}

//-----------------------------------------------------------------------------

void OFFImporter::setObjectOptions(ObjectOptions _options){
  objectOptions_ = _options;
}

//-----------------------------------------------------------------------------

void OFFImporter::addOption(ObjectOptionsE _option) {
    objectOptions_ |= _option;
}

//-----------------------------------------------------------------------------

void OFFImporter::removeOption(ObjectOptionsE _option) {
    if(objectOptions_ & _option) objectOptions_ -= _option;
}

//-----------------------------------------------------------------------------

OFFImporter::ObjectOptions& OFFImporter::objectOptions(){
  return objectOptions_;
}

//-----------------------------------------------------------------------------

void OFFImporter::setObjectName(QString _name){
  
  if ( object_ != 0 )
    object_->setName( _name );
}

//------------------------------------------------------------------------------

/// set RGBA color of vertex
void OFFImporter::setVertexColor(VertexHandle _vh, int _colorIndex) {

    if ( isTriangleMesh() ){
        
        //handle triangle meshes
        if ( !triMesh() ) return;
        
        if ( _colorIndex < (int) colors_.size() ){
            
            if ( vertexMapTri_.find( _vh ) != vertexMapTri_.end() ){
                triMesh()->set_color( vertexMapTri_[_vh], colors_[_colorIndex] );
                objectOptions_ |= VERTEXCOLOR;
            }
            
        }else{
            std::cerr << "Error: Color ID too large" << std::endl;
        }
        
    } else if ( isPolyMesh() ){
        
        //handle poly meshes
        if ( !polyMesh() ) return;
        
        if ( _colorIndex < (int) colors_.size() ){
            
            if ( vertexMapPoly_.find( _vh ) != vertexMapPoly_.end() ){
                polyMesh()->set_color( vertexMapPoly_[_vh], colors_[_colorIndex] );
                objectOptions_ |= VERTEXCOLOR;
            }
            
        }else{
            std::cerr << "Error: Color ID too large" << std::endl;
        }
        
    }
}

//------------------------------------------------------------------------------

/// set RGBA color of face
void OFFImporter::setFaceColor(FaceHandle _fh, int _colorIndex) {
    
    if ( isTriangleMesh() ){
        
        //handle triangle meshes
        if ( !triMesh() ) return;
        
        if ( _colorIndex < (int) colors_.size() ){
            
            if ( _fh < (int)faceMapTri_.size() ) {
                triMesh()->set_color( faceMapTri_[_fh],colors_[_colorIndex] );
                objectOptions_ |= FACECOLOR;
            }
            
        }else{
            std::cerr << "Error: Color ID too large" << std::endl;
        }
        
    } else if ( isPolyMesh() ){
        
        //handle poly meshes
        if ( !polyMesh() ) return;
        
        if ( _colorIndex < (int) colors_.size() ){
            
            if ( _fh < (int)faceMapPoly_.size() ) {
                polyMesh()->set_color( faceMapPoly_[_fh], colors_[_colorIndex] );
                objectOptions_ |= FACECOLOR;
            }
            
        }else{
            std::cerr << "Error: Color ID too large" << std::endl;
        }
        
    }
}
