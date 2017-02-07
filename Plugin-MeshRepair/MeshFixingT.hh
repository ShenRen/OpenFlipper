/*===========================================================================*\
 *                                                                           *
 *                               OpenMesh                                    *
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
 *                                                                           *
\*===========================================================================*/

/*===========================================================================*\
 *                                                                           *
 *   $Revision$                                                         *
 *   $Date$                   *
 *                                                                           *
\*===========================================================================*/


#ifndef OPENMESH_MESHFIXING_HH
#define OPENMESH_MESHFIXING_HH

//=============================================================================
//
//  CLASS MeshFixing
//
//=============================================================================

//== INCLUDES =================================================================

//== NAMESPACE ================================================================

//== CLASS DEFINITION =========================================================

/** \brief Compare two vectors
 *
 * If they are closer than a specified epsilon, we identify them as equal.
 *
 */
class CompareVectors
{
public:

  /// Constructor
  explicit CompareVectors(double _eps = FLT_MIN);

  /// comparison operator
  bool operator()( const ACG::Vec3d& _v0, const ACG::Vec3d& _v1 ) const;

private:
  const double epsilon_;
};


/** \brief Fix a mesh
 *
 */
template<class MeshT>
class MeshFixing {

public:

  MeshFixing(MeshT& _mesh, double _epsilon );

  bool fix();


private:

  TriMesh& mesh_;


  /** \brief Add a face to the fixing data
   *
   * Degenerated faces will already be discarded in this step
   *
   * @param _p0 Vertex 1
   * @param _p1 Vertex 2
   * @param _p2 Vertex 3
   */
  void add_face(const ACG::Vec3d& _p0, const ACG::Vec3d& _p1, const ACG::Vec3d& _p2);

  void fix_topology();
  void fix_orientation();

  int neighbor(unsigned int _f, unsigned int _v0, unsigned int _v1);



  /// Internal vertex type
  struct Vertex
  {
    explicit Vertex(const ACG::Vec3d& _p) : p(_p) { };
    ACG::Vec3d                    p;

    /// This vector will contain a list of all faces incident to the current vertex
    std::vector<unsigned int>     faces;
  };

  /// Internal face type
  struct Face
  {
    Face(int _i0, int _i1, int _i2) :
            component(-1)
    {
      v[0] = _i0;
      v[1] = _i1;
      v[2] = _i2;

    }

    unsigned int  v[3];
    int  component;
  };


  typedef std::map<ACG::Vec3d, int, CompareVectors>  VertexMap;

  typedef VertexMap::iterator MapIter;

  std::vector<Vertex>  vertices_;
  std::vector<Face>    faces_;


  /** Add a vertex to the map
   *
   * @param   _p New vertex
   * @return Index of the vertex
   */
  int add_vertex(const ACG::Vec3d& _p);

  VertexMap            vmap_;

};

#if defined(INCLUDE_TEMPLATES) && !defined(OPENMESH_MESHFIXING_CC)
#define OPENMESH_MESHFIXING_TEMPLATES
#include "MeshFixingT.cc"
#endif

#endif

