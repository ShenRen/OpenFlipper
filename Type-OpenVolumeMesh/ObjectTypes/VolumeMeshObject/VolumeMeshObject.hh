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
*   $LastChangedBy$                                                 *
*   $Date$                    *
*                                                                            *
\*===========================================================================*/

#ifndef VOLUME_MESH_OBJECT_HH
#define VOLUME_MESH_OBJECT_HH

// Includes
#include <ACG/Utils/SmartPointer.hh>

#include <ACG/Math/VectorT.hh>
#include <ACG/Scenegraph/SeparatorNode.hh>
#include <ACG/Scenegraph/StatusNodesT.hh>
#include <ACG/Scenegraph/ShaderNode.hh>
#include <ACG/Scenegraph/TextureNode.hh>
#include <OpenFlipper/common/ObjectTypeDLLDefines.hh>
#include <OpenFlipper/common/BaseObjectData.hh>
#include <ObjectTypes/VolumeMeshObject/VolumeMeshNode.hh>

#include <OpenVolumeMesh/Attribs/StatusAttrib.hh>
#include <OpenVolumeMesh/Attribs/ColorAttrib.hh>
#include <OpenVolumeMesh/Attribs/NormalAttrib.hh>
#include <OpenVolumeMesh/Attribs/TexCoordAttrib.hh>


template<class MeshT>
class DLLEXPORTONLY VolumeMeshObject : public BaseObjectData {

  friend class TypePolyhedralMeshPlugin;
  friend class TypeHexahedralMeshPlugin;
  friend class TypeTetrahedralMeshPlugin;

public:

    /** \brief copy constructor
     *
     *  Create a copy of this object
     */
    VolumeMeshObject(const VolumeMeshObject& _object);

    /** \brief Constructor
    *
    * This is the standard constructor for MeshObjects. As triangle and Poly Meshes are handled by this class, the
    * typeId is passed to the MeshObject to specify it.
    *
    * @param _typeId   This is the type Id the Object will use. Should be typeId("TriangleMesh") or typeId("PolyMesh")
    */
    VolumeMeshObject(DataType _typeId);

    /// destructor
    virtual ~VolumeMeshObject();

    /// Reset current object, including all related nodes.
    virtual void cleanup();

  protected:

    /// Initialise current object, including all related nodes.
    virtual void init();

  //===========================================================================
  /** @name Name and Path handling
   * @{ */
  //===========================================================================
  public:

    /// Set the name of the Object
    void setName( QString _name );

  /** @} */

  //===========================================================================
  /** @name Content
   * @{ */
  //===========================================================================

  public:
    /// return a pointer to the mesh
    MeshT* mesh();

    /** return a full copy of this object ( All scenegraph nodes will be created )
     *  but the object will not be a part of the object tree.
     */
    BaseObject* copy();

    typedef OpenVolumeMesh::ColorAttrib<ACG::Vec4f> ColorAttrib;
    typedef OpenVolumeMesh::NormalAttrib<MeshT> NormalAttrib;
    typedef OpenVolumeMesh::StatusAttrib StatusAttrib;
    typedef OpenVolumeMesh::TexCoordAttrib<ACG::Vec2f> TexCoordAttrib;

    const StatusAttrib& status() const { return statusAttrib_; }
    StatusAttrib& status() { return statusAttrib_; }

    const ColorAttrib& colors() const { return colorAttrib_; }
    ColorAttrib& colors() { return colorAttrib_; }

    const NormalAttrib& normals() const { return normalAttrib_; }
    NormalAttrib& normals() { return normalAttrib_; }

    const TexCoordAttrib& texcoords() const { return texcoordAttrib_; }
    TexCoordAttrib& texcoords() { return texcoordAttrib_; }

  private:
    ptr::shared_ptr<MeshT> mesh_;

    StatusAttrib statusAttrib_;

    ColorAttrib colorAttrib_;

    NormalAttrib normalAttrib_;

    TexCoordAttrib texcoordAttrib_;

  /** @} */

  //===========================================================================
  /** @name Update handling
   *
   *  This is mostly private. Updates have to be triggered via
   *  emit updatedObject()
   *
   * @{ */
  //===========================================================================
  protected:

    /// Update the whole Object (Selection,Topology,...)
    virtual void update(UpdateType _type = UPDATE_ALL);

    /// Call this function to update the selection
    void updateSelection();

    /// Update Geometry of all data structures
    void updateGeometry();

    /// Update Colors of all data structures
    void updateColor();

    /// Update Texture of all data structures
    void updateTexture();

    /// Update Topology of all data structures
    void updateTopology();

  /** @} */
  //===========================================================================
  /** @name Visualization
   * @{ */
  //===========================================================================

  public:

    /// Return pointer to the texture node
    ACG::SceneGraph::TextureNode* textureNode();

    /// Return pointer to the shader node
    ACG::SceneGraph::ShaderNode* shaderNode();

    /// Get the Scenegraph Mesh Node
    ACG::SceneGraph::VolumeMeshNodeT<MeshT>* meshNode();

    /// Get the BoundingBox of this object
    void boundingBox(ACG::Vec3d& _bbMin, typename ACG::Vec3d& _bbMax);

    BaseNode* primaryNode();

  private:

    /// Scenegraph Mesh Node
    ACG::SceneGraph::VolumeMeshNodeT<MeshT>* meshNode_;

    /// Scenegraph Shader Node
    ACG::SceneGraph::ShaderNode* shaderNode_;

    /// Scenegraph Texture Node
    ACG::SceneGraph::TextureNode* textureNode_;

  /** @} */

  //===========================================================================
  /** @name Object Information
   * @{ */
  //===========================================================================
  public:
    /// Get all Info for the Object as a string
    QString getObjectinfo();

  /** @} */

  //===========================================================================
  /** @name Picking
   * @{ */
  //===========================================================================
  public:

    /// detect if the node has been picked
    bool picked(unsigned int _node_idx);

    /// Enable or disable picking for this Node
    void enablePicking(bool _enable);

    /// Check if picking is enabled for this Node
    bool pickingEnabled();

  /** @} */
};

//=============================================================================

#if defined(INCLUDE_TEMPLATES) && !defined(VOLUMEMESHOBJECTT_CC)
#define VOLUMEMESHOBJECT_TEMPLATES
#include "VolumeMeshObjectT.cc"
#endif
#endif
