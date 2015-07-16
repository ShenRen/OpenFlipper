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

#define FILEOFFPLUGIN_C

#include "FileOFF.hh"

#include <OpenMesh/Core/Utils/color_cast.hh>
#include <OpenMesh/Core/Geometry/VectorT.hh>
#include <sstream>


template< class MeshT >
bool FileOFFPlugin::writeMesh(std::ostream& _out, MeshT& _mesh, BaseObject &_baseObj){

    /*****************
    * HEADER
    ******************/

    // Write option ST
    if(_mesh.has_vertex_texcoords2D() && (userWriteOptions_ & OFFImporter::VERTEXTEXCOORDS) ) {
        _out << "ST";
    }

    // Write option C
    if(_mesh.has_vertex_colors() && (userWriteOptions_ & OFFImporter::VERTEXCOLOR) ) {
        _out << "C";
    }

    // Write option N
    if(_mesh.has_vertex_normals() && (userWriteOptions_ & OFFImporter::VERTEXNORMAL) ) {
        _out << "N";
    }

    // Write
    _out << "OFF";

    // Write BINARY
    if(userWriteOptions_ & OFFImporter::BINARY) {
        _out << " BINARY";
    }

    _out << "\n";


    /*
     * Comment
     */
    OpenMesh::MPropHandleT<std::string> mp_comment;

    if (_baseObj.hasComments()) {
        _out << "# %% BEGIN OPENFLIPPER_COMMENT %%" << std::endl;
        std::istringstream comment(_baseObj.getAllCommentsFlat().toStdString());
        std::string commentLine;
        while (std::getline(comment, commentLine)) {
            _out << "# " << commentLine << std::endl;
        }
        _out << "# %% END OPENFLIPPER_COMMENT %%" << std::endl;
    }

    /*****************
    * DATA
    ******************/

    // Call corresponding write routine
    if(userWriteOptions_ & OFFImporter::BINARY) {
        return writeBinaryData(_out, _mesh);
    } else {
      if ( !OpenFlipper::Options::savingSettings() && saveOptions_ != 0)
        _out.precision(savePrecision_->value());

      return writeASCIIData(_out, _mesh);
    }
}

//------------------------------------------------------------------------------------------------------

template< class MeshT >
bool FileOFFPlugin::writeASCIIData(std::ostream& _out, MeshT& _mesh ) {

    typename MeshT::Point p;
    typename MeshT::Normal n;
    typename OpenMesh::Vec4f c;
    typename MeshT::TexCoord2D t;

    typename MeshT::VertexIter vit = _mesh.vertices_begin();
    typename MeshT::VertexIter end_vit = _mesh.vertices_end();

    // #V #F #E
    _out << _mesh.n_vertices() << " " << _mesh.n_faces() << " " << _mesh.n_edges();

    // Write vertex data
    for(; vit != end_vit; ++vit) {

        _out << "\n";

        // Write vertex p[0] p[1] p[2]
        p = _mesh.point(*vit);
        _out << p[0] << " " << p[1] << " " << p[2];

        // Write vertex normals
        if(_mesh.has_vertex_normals() && (userWriteOptions_ & OFFImporter::VERTEXNORMAL)) {
            n = _mesh.normal(*vit);
            _out << " " << n[0] << " " << n[1] << " " << n[2];
        }

        // Write vertex colors
        // Note: Vertex colors always have only three components.
        // This has to be determined since it can not be read dynamically in binary files.
        if(_mesh.has_vertex_colors() && (userWriteOptions_ & OFFImporter::VERTEXCOLOR)) {
            c = OpenMesh::color_cast<OpenMesh::Vec4f> (_mesh.color(*vit));
            _out << " " << std::showpoint << c[0] << " " << std::showpoint << c[1] << " " << std::showpoint << c[2] << " " << std::showpoint << c[3];
        }

        // Write vertex texcoords
        if(_mesh.has_vertex_texcoords2D() && (userWriteOptions_ & OFFImporter::VERTEXTEXCOORDS)) {
            t = _mesh.texcoord2D(*vit);
            _out << " " << t[0] << " " << t[1];
        }
    }

    typename MeshT::FaceIter fit = _mesh.faces_begin();
    typename MeshT::FaceIter end_fit = _mesh.faces_end();
    typename MeshT::FaceVertexIter fvit;

    // Write face data
    for(; fit != end_fit; ++fit) {

        _out << "\n";

        // Write face valence
        _out << _mesh.valence(*fit);

        // Get face-vertex iterator
        fvit = _mesh.fv_iter(*fit);

        // Write vertex indices
        for(;fvit.is_valid(); ++fvit) {
            _out << " " << fvit->idx();
        }

        // Write face colors
        if(_mesh.has_face_colors() && (userWriteOptions_ & OFFImporter::FACECOLOR ) ) {
            c = OpenMesh::color_cast<OpenMesh::Vec4f> (_mesh.color(*fit));
            _out << " " << std::showpoint << c[0] << " " << std::showpoint << c[1] << " " << std::showpoint << c[2];

            if(userWriteOptions_ & OFFImporter::COLORALPHA) _out <<  " " << std::showpoint << c[3];
        }
    }

    return true;
}

//------------------------------------------------------------------------------------------------------

template< class MeshT >
bool FileOFFPlugin::writeBinaryData(std::ostream& _out, MeshT& _mesh ){

    Vec3f v, n;
    Vec2f t;
    OpenMesh::Vec4f c(1.0,1.0,1.0,1.0);
    OpenMesh::Vec3f p;

    typename MeshT::VertexIter vit = _mesh.vertices_begin();
    typename MeshT::VertexIter end_vit = _mesh.vertices_end();

    // #vertices, #faces, #edges
    writeValue(_out, (uint)_mesh.n_vertices() );
    writeValue(_out, (uint)_mesh.n_faces() );
    writeValue(_out, (uint)_mesh.n_edges() );

    // Write vertex data
    for(; vit != end_vit; ++vit) {

        // Write vertex p[0] p[1] p[2]
        p = _mesh.point(*vit);
        writeValue(_out, p[0]);
        writeValue(_out, p[1]);
        writeValue(_out, p[2]);

        // Write vertex normals
        if(_mesh.has_vertex_normals() && (userWriteOptions_ & OFFImporter::VERTEXNORMAL)) {
            n = _mesh.normal(*vit);
            writeValue(_out, n[0]);
            writeValue(_out, n[1]);
            writeValue(_out, n[2]);
        }

        // Write vertex colors
        // Note: Vertex colors always have only three components.
        // This has to be determined since it can not be read dynamically in binary files.
        if(_mesh.has_vertex_colors() && (userWriteOptions_ & OFFImporter::VERTEXCOLOR)) {
            c = OpenMesh::color_cast<OpenMesh::Vec4f> (_mesh.color(*vit));
            writeValue(_out, c[0]);
            writeValue(_out, c[1]);
            writeValue(_out, c[2]);
        }

        // Write vertex texcoords
        if(_mesh.has_vertex_texcoords2D() && (userWriteOptions_ & OFFImporter::VERTEXTEXCOORDS)) {
            t = _mesh.texcoord2D(*vit);
            writeValue(_out, t[0]);
            writeValue(_out, t[1]);
        }
    }

    typename MeshT::FaceIter fit = _mesh.faces_begin();
    typename MeshT::FaceIter end_fit = _mesh.faces_end();
    typename MeshT::FaceVertexIter fvit;

    // Write face data
    for(; fit != end_fit; ++fit) {

        // Write face valence
        writeValue(_out, _mesh.valence(*fit));

        // Get face-vertex iterator
        fvit = _mesh.fv_iter(*fit);

        // Write vertex indices
        for(;fvit.is_valid(); ++fvit) {
            writeValue(_out, fvit->idx());
        }

        // Write face colors
        if(_mesh.has_face_colors() && (userWriteOptions_ & OFFImporter::FACECOLOR)) {

            // Number of color components
            if(userWriteOptions_ & OFFImporter::COLORALPHA) writeValue(_out, (uint)4);
            else writeValue(_out, (uint)3);

            // Color itself
            c = OpenMesh::color_cast<OpenMesh::Vec4f> (_mesh.color(*fit));
            writeValue(_out, c[0]);
            writeValue(_out, c[1]);
            writeValue(_out, c[2]);

            if(userWriteOptions_ & OFFImporter::COLORALPHA) writeValue(_out, c[3]);
        }
    }

    return true;
}
