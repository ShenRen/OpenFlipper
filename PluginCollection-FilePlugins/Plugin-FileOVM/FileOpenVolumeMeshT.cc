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
 *   $Revision: 13548 $                                                       *
 *   $LastChangedBy: kremer $                                                 *
 *   $Date: 2012-01-27 11:58:19 +0100 (Fr, 27 Jan 2012) $                    *
 *                                                                            *
 \*===========================================================================*/

#define FILEOPENVOLUMEMESHT_CC

#include "FileOpenVolumeMesh.hh"

//----------------------------------------------------------------------------

template <typename MeshT>
void FileOpenVolumeMeshPlugin::loadMesh(const char* _filename, MeshT& _mesh, bool _comp, bool _topCheck) {

    // found edges in file
    bool edges_in_file = false;

    typedef typename MeshT::PointT Vertex;
    _mesh.clear(false);

    std::ifstream iff(_filename, std::ios::in);

    std::string s;

    // read header
    iff >> s;
    if (s != "Vertices") {
        std::cerr << "ERROR reading OpenVolumeMesh (Vertex Section Failed)\n";
        iff.close();
        return;
    }

    // read vertices
    int nv;
    iff >> nv;
    for (int i = 0; i < nv; ++i) {
        double x, y, z;
        iff >> x;
        iff >> y;
        iff >> z;

        ACG::Vec3d v(x, y, z);
        _mesh.add_vertex(v);
    }

    iff >> s;
    if (s != "Edges") {
        std::cerr << "No edges found" << std::endl;
    } else {
        edges_in_file = true;
        int ne;
        iff >> ne;
        for (int e = 0; e < ne; ++e) {
            int v1, v2;
            iff >> v1;
            iff >> v2;
            _mesh.add_edge(OpenVolumeMesh::VertexHandle(v1), OpenVolumeMesh::VertexHandle(v2));
        }
    }

    if (edges_in_file) {
        iff >> s;
    }

    if (s != "Faces") {
        std::cerr << "ERROR reading OpenVolumeMesh (Face Section Failed)\n";
        iff.close();
        return;
    }

    if(_comp) {

        // Read vertex indices and define faces
        // via its vertices (in correct order)
        int nf;
        iff >> nf;
        for (int f = 0; f < nf; ++f) {
            int nfv;
            iff >> nfv;
            std::vector<OpenVolumeMesh::VertexHandle> vhs;
            for (int v = 0; v < nfv; ++v) {
                int i;
                iff >> i;
                vhs.push_back(OpenVolumeMesh::VertexHandle(i));
            }
            int pos_cell, neg_cell;
            iff >> pos_cell;
            iff >> neg_cell;
            _mesh.add_face(vhs);
        }
    } else {

        // Read halfface indices
        int nf;
        iff >> nf;
        for (int f = 0; f < nf; ++f) {
            int nfhe;
            iff >> nfhe;
            std::vector<OpenVolumeMesh::HalfEdgeHandle> hes;
            for (int he = 0; he < nfhe; ++he) {
                int i;
                iff >> i;
                hes.push_back(OpenVolumeMesh::HalfEdgeHandle(i));
            }

            _mesh.add_face(hes, _topCheck);
        }
    }

    // Read faces and find the respective halffaces
    iff >> s;
    if(s != "Polyhedra") {
        std::cerr << "ERROR reading OpenVolumeMesh (Polyhedra Section Failed)\n";
        iff.close();
        return;
    }

    if(_comp) {

        int nc;
        iff >> nc;

        for( int i = 0; i < nc; ++i) {

            int ncf;
            iff >> ncf;
            std::vector< OpenVolumeMesh::FaceHandle > faces;

            for( int f = 0; f < ncf; ++f) {

                int fidx;
                iff >> fidx;
                faces.push_back(OpenVolumeMesh::FaceHandle(fidx));
            }

            // Get right halffaces
            // First determine the barycenter of the polyhedron
            ACG::Vec3d c(0.0, 0.0, 0.0);
            unsigned int num_faces = faces.size();
            for(auto fh : faces)
            {

                std::vector<OpenVolumeMesh::HalfEdgeHandle> hes = _mesh.face(fh).halfedges();
                unsigned int val = hes.size();
                ACG::Vec3d f_mid(0.0, 0.0, 0.0);
                for(std::vector<OpenVolumeMesh::HalfEdgeHandle>::const_iterator p_it = hes.begin();
                        p_it != hes.end(); ++p_it) {
                    f_mid += (_mesh.vertex(_mesh.halfedge(*p_it).to_vertex()) / (double)val);
                }
                c += f_mid / (double)num_faces;
            }

            // Now determine all halffaces
            // Test whether their normals point into the polyhedron
            std::vector<OpenVolumeMesh::HalfFaceHandle> hfhandles;
            for(auto fh : faces)
            {

                // Get face's mid-point
                std::vector<OpenVolumeMesh::HalfEdgeHandle> hes = _mesh.face(fh).halfedges();
                unsigned int val = hes.size();
                ACG::Vec3d f_mid(0.0, 0.0, 0.0);
                for(std::vector<OpenVolumeMesh::HalfEdgeHandle>::const_iterator p_it = hes.begin();
                        p_it != hes.end(); ++p_it) {
                    f_mid += (_mesh.vertex(_mesh.halfedge(*p_it).to_vertex()) / (double)val);
                }

                // Now compute the face's normal
                if(hes.size() < 2) continue;
                ACG::Vec3d p0 = _mesh.vertex(_mesh.halfedge(hes[0]).from_vertex());
                ACG::Vec3d p1 = _mesh.vertex(_mesh.halfedge(hes[0]).to_vertex());
                ACG::Vec3d p2 = _mesh.vertex(_mesh.halfedge(hes[1]).to_vertex());
                ACG::Vec3d n = (p0 - p1) % (p2 - p1);
                n.normalize();

                if(((c - f_mid) | n) >= 0.0) hfhandles.push_back(_mesh.halfface_handle(fh, 0));
                else hfhandles.push_back(_mesh.halfface_handle(fh, 1));
            }

            if(hfhandles.size() > 3) {

                HexahedralMesh* hm = 0;
                hm = dynamic_cast<HexahedralMesh*>(&_mesh);

                if(hm) {
                    hm->add_cell(hfhandles, _topCheck);
                } else {
                    _mesh.add_cell(hfhandles, _topCheck);
                }
            } else {
                std::cerr << "Could not determine corresponding halffaces!" << std::endl;
            }
        }
    } else {

        // Just read the specified halffaces
        int nc;
        iff >> nc;
        for (int c = 0; c < nc; ++c) {

            int nhf;
            iff >> nhf;
            std::vector<OpenVolumeMesh::HalfFaceHandle> hfs;
            for (int hf = 0; hf < nhf; ++hf) {
                int i;
                iff >> i;
                hfs.push_back(OpenVolumeMesh::HalfFaceHandle(i));
            }

            // Implement hex mesh shit
            HexahedralMesh* hm = 0;
            hm = dynamic_cast<HexahedralMesh*>(&_mesh);

            if(hm) {
                hm->add_cell(hfs, _topCheck);
            } else {
                _mesh.add_cell(hfs, _topCheck);
            }
        }
    }
    iff.close();

    std::cerr << "######## openvolumemesh info #########\n";
    std::cerr << "#vertices: " << _mesh.n_vertices() << std::endl;
    std::cerr << "#edges   : " << _mesh.n_edges() << std::endl;
    std::cerr << "#faces   : " << _mesh.n_faces() << std::endl;
    std::cerr << "#cells   : " << _mesh.n_cells() << std::endl;
}

//----------------------------------------------------------------------------

template <typename MeshT>
void FileOpenVolumeMeshPlugin::saveMesh(const char* _filename, MeshT& _mesh, bool _comp) {

    typedef OpenVolumeMesh::OpenVolumeMeshFace Face;
    std::ofstream off(_filename, std::ios::out);

    int n_vertices(_mesh.n_vertices());
    off << "Vertices" << std::endl;
    off << n_vertices << std::endl;

    // write vertices
    for(OpenVolumeMesh::VertexIter v_it = _mesh.v_iter(); v_it; ++v_it) {

        ACG::Vec3d v = _mesh.vertex(*v_it);
        off << v[0] << " " << v[1] << " " << v[2] << std::endl;
    }

    int n_edges(_mesh.n_edges());
    off << "Edges" << std::endl;
    off << n_edges << std::endl;

    // write edges
    for(OpenVolumeMesh::EdgeIter e_it = _mesh.e_iter(); e_it; ++e_it) {

        OpenVolumeMesh::VertexHandle from_vertex = _mesh.edge(*e_it).from_vertex();
        OpenVolumeMesh::VertexHandle to_vertex   = _mesh.edge(*e_it).to_vertex();
        off << from_vertex << " " << to_vertex << std::endl;
    }

    int n_faces(_mesh.n_faces());
    off << "Faces" << std::endl;
    off << n_faces << std::endl;

    // write faces
    for(OpenVolumeMesh::FaceIter f_it = _mesh.f_iter(); f_it; ++f_it) {

        off << _mesh.face(*f_it).halfedges().size() << " ";

        std::vector<OpenVolumeMesh::HalfEdgeHandle> halfedges = _mesh.face(*f_it).halfedges();

        for (std::vector<OpenVolumeMesh::HalfEdgeHandle>::const_iterator it = halfedges.begin();
                it != halfedges.end(); ++it) {

            if(_comp) {
                off << _mesh.halfedge(*it).from_vertex();
            } else {
                off << *it;
            }

            if((it+1) != halfedges.end()) off << " ";
        }

        off << std::endl;
    }


    int n_cells(_mesh.n_cells());
    off << "Polyhedra" << std::endl;
    off << n_cells << std::endl;

    for(OpenVolumeMesh::CellIter c_it = _mesh.c_iter(); c_it; ++c_it) {

        off << _mesh.cell(*c_it).halffaces().size() << " ";

        std::vector<OpenVolumeMesh::HalfFaceHandle> halffaces = _mesh.cell(*c_it).halffaces();

        for (std::vector<OpenVolumeMesh::HalfFaceHandle>::const_iterator it = halffaces.begin();
                it != halffaces.end(); ++it) {

            if(_comp) {
                off << _mesh.face_handle(*it);
            } else {
                off << *it;
            }

            if((it+1) != halffaces.end()) off << " ";
        }

        off << std::endl;
    }

    off.close();
}
