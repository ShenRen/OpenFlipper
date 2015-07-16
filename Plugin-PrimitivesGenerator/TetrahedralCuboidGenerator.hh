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

#pragma once

#include <ObjectTypes/PolyhedralMesh/PolyhedralMesh.hh>

struct SortedFace
{
    explicit SortedFace(std::vector<OpenVolumeMesh::VertexHandle> const& face)
        : v(face)
    {
        std::sort(v.begin(), v.end());
    }

    SortedFace(OpenVolumeMesh::VertexHandle v1,
            OpenVolumeMesh::VertexHandle v2,
            OpenVolumeMesh::VertexHandle v3)
        : v(3)
    {
        v[0] = v1;
        v[1] = v2;
        v[2] = v3;
        std::sort(v.begin(), v.end());
    }

    std::vector<OpenVolumeMesh::VertexHandle> v;
};

inline bool operator<(SortedFace const& f1, SortedFace const& f2)
{
    return std::lexicographical_compare(f1.v.begin(), f1.v.end(),
            f2.v.begin(), f2.v.end());
}

class TetrahedralCuboidGenerator
{
public:
    TetrahedralCuboidGenerator(PolyhedralMesh& mesh, Vector const& position, Vector const& length,
                                    unsigned const n_x, unsigned const n_y, unsigned const n_z);

private:
    void add_vertices(Vector const& position, Vector const& length);
    void get_cube_vertices(std::size_t i, std::size_t j, std::size_t k,
            std::vector<OpenVolumeMesh::VertexHandle>& v) const;

    void add_faces();
    void add_cube_type_1_faces(std::size_t i, std::size_t j, std::size_t k,
            std::vector<OpenVolumeMesh::VertexHandle> const& v);
    void add_cube_type_2_faces(std::size_t i, std::size_t j, std::size_t k,
            std::vector<OpenVolumeMesh::VertexHandle> const& v);

    void add_cells();
    void add_cube_type_1_cells(std::size_t i, std::size_t j, std::size_t k,
            std::vector<OpenVolumeMesh::VertexHandle> const& v);
    void add_cube_type_2_cells(std::size_t i, std::size_t j, std::size_t k,
            std::vector<OpenVolumeMesh::VertexHandle> const& v);

    PolyhedralMesh* mesh_;

    std::size_t size_[3];
    std::vector<OpenVolumeMesh::VertexHandle> vertices_;
    std::map<SortedFace, OpenVolumeMesh::FaceHandle> faces_;
};

