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

#ifdef ENABLE_OPENVOLUMEMESH_POLYHEDRAL_SUPPORT

#include "TetrahedralCuboidGenerator.hh"

void TetrahedralCuboidGenerator::add_vertices(Vector const& position, Vector const& length)
{
    vertices_.clear();
    vertices_.reserve((size_[0] + 1) * (size_[1] + 1) * (size_[2] + 1));

    Vector h(length[0] / size_[0], length[1] / size_[1], length[2] / size_[2]);
    Vector origin = position - 0.5 * length;

    for (std::size_t k = 0; k < size_[2] + 1; ++k)
        for (std::size_t j = 0; j < size_[1] + 1; ++j)
            for (std::size_t i = 0; i < size_[0] + 1; ++i)
                vertices_.push_back(mesh_->add_vertex(Vector(h[0]*i, h[1]*j, h[2]*k) + origin));
}

void TetrahedralCuboidGenerator::get_cube_vertices(std::size_t i, std::size_t j, std::size_t k,
        std::vector<OpenVolumeMesh::VertexHandle>& v) const
{
    v[0] = vertices_[k * (size_[0] + 1) * (size_[1] + 1) + j * (size_[0] + 1) + i];
    v[1] = vertices_[k * (size_[0] + 1) * (size_[1] + 1) + j * (size_[0] + 1) + i + 1];
    v[2] = vertices_[k * (size_[0] + 1) * (size_[1] + 1) + (j + 1) * (size_[0] + 1) + i];
    v[3] = vertices_[k * (size_[0] + 1) * (size_[1] + 1) + (j + 1) * (size_[0] + 1) + i + 1];
    v[4] = vertices_[(k + 1) * (size_[0] + 1) * (size_[1] + 1) + j * (size_[0] + 1) + i];
    v[5] = vertices_[(k + 1) * (size_[0] + 1) * (size_[1] + 1) + j * (size_[0] + 1) + i + 1];
    v[6] = vertices_[(k + 1) * (size_[0] + 1) * (size_[1] + 1) + (j + 1) * (size_[0] + 1) + i];
    v[7] = vertices_[(k + 1) * (size_[0] + 1) * (size_[1] + 1) + (j + 1) * (size_[0] + 1) + i + 1];
}

void TetrahedralCuboidGenerator::add_faces()
{
    std::vector<OpenVolumeMesh::VertexHandle> v(8);

    for (std::size_t i = 0; i < size_[0]; ++i)
        for (std::size_t j = 0; j < size_[1]; ++j)
            for (std::size_t k = 0; k < size_[2]; ++k)
            {
                get_cube_vertices(i, j, k, v);

                if ((i + j + k) % 2 == 0)
                    add_cube_type_1_faces(i, j, k, v);
                else
                    add_cube_type_2_faces(i, j, k, v);
            }
}

void TetrahedralCuboidGenerator::add_cube_type_1_faces(std::size_t i, std::size_t j, std::size_t k,
        std::vector<OpenVolumeMesh::VertexHandle> const& v)
{
    std::vector<OpenVolumeMesh::VertexHandle> fv(3);

    // left side
    fv[0] = v[0]; fv[1] = v[2]; fv[2] = v[6];
    faces_.insert(std::make_pair(SortedFace(fv), mesh_->add_face(fv)));
    fv[0] = v[0]; fv[1] = v[6]; fv[2] = v[4];
    faces_.insert(std::make_pair(SortedFace(fv), mesh_->add_face(fv)));

    // front side
    fv[0] = v[0]; fv[1] = v[4]; fv[2] = v[5];
    faces_.insert(std::make_pair(SortedFace(fv), mesh_->add_face(fv)));
    fv[0] = v[0]; fv[1] = v[5]; fv[2] = v[1];
    faces_.insert(std::make_pair(SortedFace(fv), mesh_->add_face(fv)));

    // bottom side
    fv[0] = v[0]; fv[1] = v[1]; fv[2] = v[3];
    faces_.insert(std::make_pair(SortedFace(fv), mesh_->add_face(fv)));
    fv[0] = v[0]; fv[1] = v[3]; fv[2] = v[2];
    faces_.insert(std::make_pair(SortedFace(fv), mesh_->add_face(fv)));

    // inner faces
    fv[0] = v[0]; fv[1] = v[5]; fv[2] = v[6];
    faces_.insert(std::make_pair(SortedFace(fv), mesh_->add_face(fv)));
    fv[0] = v[0]; fv[1] = v[3]; fv[2] = v[5];
    faces_.insert(std::make_pair(SortedFace(fv), mesh_->add_face(fv)));
    fv[0] = v[0]; fv[1] = v[6]; fv[2] = v[3];
    faces_.insert(std::make_pair(SortedFace(fv), mesh_->add_face(fv)));
    fv[0] = v[3]; fv[1] = v[6]; fv[2] = v[5];
    faces_.insert(std::make_pair(SortedFace(fv), mesh_->add_face(fv)));

    // right face
    if (i == size_[0] - 1) {
        fv[0] = v[3]; fv[1] = v[5]; fv[2] = v[1];
        faces_.insert(std::make_pair(SortedFace(fv), mesh_->add_face(fv)));
        fv[0] = v[3]; fv[1] = v[7]; fv[2] = v[5];
        faces_.insert(std::make_pair(SortedFace(fv), mesh_->add_face(fv)));
    }

    // back face
    if (j == size_[1] - 1) {
        fv[0] = v[3]; fv[1] = v[6]; fv[2] = v[7];
        faces_.insert(std::make_pair(SortedFace(fv), mesh_->add_face(fv)));
        fv[0] = v[3]; fv[1] = v[2]; fv[2] = v[6];
        faces_.insert(std::make_pair(SortedFace(fv), mesh_->add_face(fv)));
    }

    // top face
    if (k == size_[2] - 1) {
        fv[0] = v[5]; fv[1] = v[6]; fv[2] = v[4];
        faces_.insert(std::make_pair(SortedFace(fv), mesh_->add_face(fv)));
        fv[0] = v[5]; fv[1] = v[7]; fv[2] = v[6];
        faces_.insert(std::make_pair(SortedFace(fv), mesh_->add_face(fv)));
    }
}

void TetrahedralCuboidGenerator::add_cube_type_2_faces(std::size_t i, std::size_t j, std::size_t k,
        std::vector<OpenVolumeMesh::VertexHandle> const& v)
{
    std::vector<OpenVolumeMesh::VertexHandle> fv(3);

    // left side
    fv[0] = v[0]; fv[1] = v[2]; fv[2] = v[4];
    faces_.insert(std::make_pair(SortedFace(fv), mesh_->add_face(fv)));
    fv[0] = v[2]; fv[1] = v[6]; fv[2] = v[4];
    faces_.insert(std::make_pair(SortedFace(fv), mesh_->add_face(fv)));

    // front side
    fv[0] = v[0]; fv[1] = v[4]; fv[2] = v[1];
    faces_.insert(std::make_pair(SortedFace(fv), mesh_->add_face(fv)));
    fv[0] = v[4]; fv[1] = v[5]; fv[2] = v[1];
    faces_.insert(std::make_pair(SortedFace(fv), mesh_->add_face(fv)));

    // bottom side
    fv[0] = v[0]; fv[1] = v[1]; fv[2] = v[2];
    faces_.insert(std::make_pair(SortedFace(fv), mesh_->add_face(fv)));
    fv[0] = v[1]; fv[1] = v[3]; fv[2] = v[2];
    faces_.insert(std::make_pair(SortedFace(fv), mesh_->add_face(fv)));

    // inner faces
    fv[0] = v[1]; fv[1] = v[7]; fv[2] = v[4];
    faces_.insert(std::make_pair(SortedFace(fv), mesh_->add_face(fv)));
    fv[0] = v[1]; fv[1] = v[2]; fv[2] = v[7];
    faces_.insert(std::make_pair(SortedFace(fv), mesh_->add_face(fv)));
    fv[0] = v[2]; fv[1] = v[4]; fv[2] = v[7];
    faces_.insert(std::make_pair(SortedFace(fv), mesh_->add_face(fv)));
    fv[0] = v[1]; fv[1] = v[4]; fv[2] = v[2];
    faces_.insert(std::make_pair(SortedFace(fv), mesh_->add_face(fv)));

    // right face
    if (i == size_[0] - 1) {
        fv[0] = v[1]; fv[1] = v[7]; fv[2] = v[5];
        faces_.insert(std::make_pair(SortedFace(fv), mesh_->add_face(fv)));
        fv[0] = v[1]; fv[1] = v[3]; fv[2] = v[7];
        faces_.insert(std::make_pair(SortedFace(fv), mesh_->add_face(fv)));
    }

    // back face
    if (j == size_[1] - 1) {
        fv[0] = v[2]; fv[1] = v[7]; fv[2] = v[3];
        faces_.insert(std::make_pair(SortedFace(fv), mesh_->add_face(fv)));
        fv[0] = v[2]; fv[1] = v[6]; fv[2] = v[7];
        faces_.insert(std::make_pair(SortedFace(fv), mesh_->add_face(fv)));
    }

    // top face
    if (k == size_[2] - 1) {
        fv[0] = v[4]; fv[1] = v[7]; fv[2] = v[6];
        faces_.insert(std::make_pair(SortedFace(fv), mesh_->add_face(fv)));
        fv[0] = v[4]; fv[1] = v[5]; fv[2] = v[7];
        faces_.insert(std::make_pair(SortedFace(fv), mesh_->add_face(fv)));
    }
}

void TetrahedralCuboidGenerator::add_cells()
{
    std::vector<OpenVolumeMesh::VertexHandle> v(8);

    for (std::size_t i = 0; i < size_[0]; ++i)
        for (std::size_t j = 0; j < size_[1]; ++j)
            for (std::size_t k = 0; k < size_[2]; ++k)
            {
                get_cube_vertices(i, j, k, v);

                if ((i + j + k) % 2 == 0)
                    add_cube_type_1_cells(i, j, k, v);
                else
                    add_cube_type_2_cells(i, j, k, v);
            }
}

void TetrahedralCuboidGenerator::add_cube_type_1_cells(std::size_t i, std::size_t j, std::size_t k,
        std::vector<OpenVolumeMesh::VertexHandle> const& v)
{
    std::vector<OpenVolumeMesh::FaceHandle> f(4);
    std::vector<OpenVolumeMesh::HalfFaceHandle> hf(4);

    // inner cell
    f[0] = faces_[SortedFace(v[0], v[5], v[6])];
    f[1] = faces_[SortedFace(v[0], v[3], v[5])];
    f[2] = faces_[SortedFace(v[3], v[5], v[6])];
    f[3] = faces_[SortedFace(v[0], v[3], v[6])];
    hf[0] = mesh_->halfface_handle(f[0], 1);
    hf[1] = mesh_->halfface_handle(f[1], 1);
    hf[2] = mesh_->halfface_handle(f[2], 1);
    hf[3] = mesh_->halfface_handle(f[3], 1);
    mesh_->add_cell(hf);

    f[0] = faces_[SortedFace(v[0], v[4], v[6])];
    f[1] = faces_[SortedFace(v[0], v[4], v[5])];
    f[2] = faces_[SortedFace(v[4], v[5], v[6])];
    f[3] = faces_[SortedFace(v[0], v[5], v[6])];
    hf[0] = mesh_->halfface_handle(f[0], 0);
    hf[1] = mesh_->halfface_handle(f[1], 0);
    hf[2] = mesh_->halfface_handle(f[2], 1);
    hf[3] = mesh_->halfface_handle(f[3], 0);
    mesh_->add_cell(hf);

    f[0] = faces_[SortedFace(v[1], v[3], v[5])];
    f[1] = faces_[SortedFace(v[0], v[1], v[5])];
    f[2] = faces_[SortedFace(v[0], v[1], v[3])];
    f[3] = faces_[SortedFace(v[0], v[3], v[5])];
    hf[0] = mesh_->halfface_handle(f[0], 1);
    hf[1] = mesh_->halfface_handle(f[1], 0);
    hf[2] = mesh_->halfface_handle(f[2], 0);
    hf[3] = mesh_->halfface_handle(f[3], 0);
    mesh_->add_cell(hf);

    f[0] = faces_[SortedFace(v[3], v[5], v[7])];
    f[1] = faces_[SortedFace(v[3], v[6], v[7])];
    f[2] = faces_[SortedFace(v[5], v[6], v[7])];
    f[3] = faces_[SortedFace(v[3], v[5], v[6])];
    hf[0] = mesh_->halfface_handle(f[0], 1);
    hf[1] = mesh_->halfface_handle(f[1], 1);
    hf[2] = mesh_->halfface_handle(f[2], 1);
    hf[3] = mesh_->halfface_handle(f[3], 0);
    mesh_->add_cell(hf);

    f[0] = faces_[SortedFace(v[0], v[2], v[6])];
    f[1] = faces_[SortedFace(v[2], v[3], v[6])];
    f[2] = faces_[SortedFace(v[0], v[2], v[3])];
    f[3] = faces_[SortedFace(v[0], v[3], v[6])];
    hf[0] = mesh_->halfface_handle(f[0], 0);
    hf[1] = mesh_->halfface_handle(f[1], 1);
    hf[2] = mesh_->halfface_handle(f[2], 0);
    hf[3] = mesh_->halfface_handle(f[3], 0);
    mesh_->add_cell(hf);
}

void TetrahedralCuboidGenerator::add_cube_type_2_cells(std::size_t i, std::size_t j, std::size_t k,
        std::vector<OpenVolumeMesh::VertexHandle> const& v)
{
    std::vector<OpenVolumeMesh::FaceHandle> f(4);
    std::vector<OpenVolumeMesh::HalfFaceHandle> hf(4);

    // inner cell
    f[0] = faces_[SortedFace(v[1], v[2], v[4])];
    f[1] = faces_[SortedFace(v[1], v[4], v[7])];
    f[2] = faces_[SortedFace(v[1], v[2], v[7])];
    f[3] = faces_[SortedFace(v[2], v[4], v[7])];
    hf[0] = mesh_->halfface_handle(f[0], 1);
    hf[1] = mesh_->halfface_handle(f[1], 1);
    hf[2] = mesh_->halfface_handle(f[2], 1);
    hf[3] = mesh_->halfface_handle(f[3], 1);
    mesh_->add_cell(hf);

    f[0] = faces_[SortedFace(v[0], v[2], v[4])];
    f[1] = faces_[SortedFace(v[0], v[1], v[4])];
    f[2] = faces_[SortedFace(v[0], v[1], v[2])];
    f[3] = faces_[SortedFace(v[1], v[2], v[4])];
    hf[0] = mesh_->halfface_handle(f[0], 0);
    hf[1] = mesh_->halfface_handle(f[1], 0);
    hf[2] = mesh_->halfface_handle(f[2], 0);
    hf[3] = mesh_->halfface_handle(f[3], 0);
    mesh_->add_cell(hf);

    f[0] = faces_[SortedFace(v[1], v[5], v[7])];
    f[1] = faces_[SortedFace(v[1], v[4], v[5])];
    f[2] = faces_[SortedFace(v[4], v[5], v[7])];
    f[3] = faces_[SortedFace(v[1], v[4], v[7])];
    hf[0] = mesh_->halfface_handle(f[0], 1);
    hf[1] = mesh_->halfface_handle(f[1], 0);
    hf[2] = mesh_->halfface_handle(f[2], 1);
    hf[3] = mesh_->halfface_handle(f[3], 0);
    mesh_->add_cell(hf);

    f[0] = faces_[SortedFace(v[1], v[3], v[7])];
    f[1] = faces_[SortedFace(v[2], v[3], v[7])];
    f[2] = faces_[SortedFace(v[1], v[2], v[3])];
    f[3] = faces_[SortedFace(v[1], v[2], v[7])];
    hf[0] = mesh_->halfface_handle(f[0], 1);
    hf[1] = mesh_->halfface_handle(f[1], 1);
    hf[2] = mesh_->halfface_handle(f[2], 0);
    hf[3] = mesh_->halfface_handle(f[3], 0);
    mesh_->add_cell(hf);

    f[0] = faces_[SortedFace(v[2], v[4], v[6])];
    f[1] = faces_[SortedFace(v[2], v[6], v[7])];
    f[2] = faces_[SortedFace(v[4], v[6], v[7])];
    f[3] = faces_[SortedFace(v[2], v[4], v[7])];
    hf[0] = mesh_->halfface_handle(f[0], 0);
    hf[1] = mesh_->halfface_handle(f[1], 1);
    hf[2] = mesh_->halfface_handle(f[2], 1);
    hf[3] = mesh_->halfface_handle(f[3], 0);
    mesh_->add_cell(hf);
}

TetrahedralCuboidGenerator::TetrahedralCuboidGenerator(PolyhedralMesh& mesh,
                                                            Vector const& position,
                                                            Vector const& length,
                                                            unsigned const n_x,
                                                            unsigned const n_y,
                                                            unsigned const n_z) :
  mesh_(&mesh)
{
    mesh_->clear(false);

    size_[0] = n_x;
    size_[1] = n_y;
    size_[2] = n_z;

    add_vertices(position, length);
    add_faces();
    add_cells();

    vertices_.clear();
    faces_.clear();
}

#endif
