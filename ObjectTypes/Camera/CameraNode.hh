/* ========================================================================= *
 *                                                                           *
 *                               OpenFlipper                                 *
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
 * ========================================================================= */

/*===========================================================================*\
 *
 *   $Revision$
 *   $Date$
 *
\*===========================================================================*/





//=============================================================================
//
//  CLASS CameraNode
//
//=============================================================================


#ifndef ACG_CAMERAVIS_NODE_HH
#define ACG_CAMERAVIS_NODE_HH

//== INCLUDES =================================================================


#include <ACG/Scenegraph/BaseNode.hh>
#include <ACG/Scenegraph/DrawModes.hh>
#include <ACG/GL/GLPrimitives.hh>
#include <ACG/GL/globjects.hh>
#include <string>
#include <vector>

#include <ACG/Math/QuaternionT.hh>

#include <OpenFlipper/common/GlobalDefines.hh>

//== NAMESPACES ===============================================================

namespace ACG {
namespace SceneGraph {

//== CLASS DEFINITION =========================================================


/** \class CameraNode CameraNode.hh <ACG/Scenegraph/CameraNode.hh>

 This class is able to render spheres colorcoding the vertex CameraVis
 **/

class DLLEXPORT CameraNode: public BaseNode {

public:

    /// Default constructor.
    CameraNode( BaseNode* _parent = 0, std::string _name = "<CameraVis>");

    /// Destructor
    virtual ~CameraNode();

    ACG_CLASSNAME( CameraNode);

    /// return available draw modes
    DrawModes::DrawMode availableDrawModes() const;

    /// update bounding box
    void boundingBox(Vec3d& _bbMin, Vec3d& _bbMax);

    /// drawing
    void draw(GLState& _state, const DrawModes::DrawMode& _drawMode);

    /// create render objects
    void getRenderObjects(IRenderer* _renderer, GLState& _state, const DrawModes::DrawMode& _drawMode, const Material* _mat);

    /// picking
    void pick(GLState& _state, PickTarget _target);

    /// set model view matrix
    void setModelView(ACG::GLMatrixd _modelView) { modelView_ = _modelView; modelViewInv_ = _modelView; modelViewInv_.invert(); update_vbo_ = true; }

    /// Set projection Matrix ( used to calculate frustum ... )
    void setProjection(ACG::GLMatrixd _projection) { projection_ = _projection; update_vbo_ = true; }

    /// Set viewport size ( This will be used to compute the aspect ratio )
    void setSize(int _w, int _h) { /*obsolete*/ }

    /// Return encoded view string
    void setEncodedView(QString _encodedView) { encodedView_ = _encodedView; }

    /// Set distance to far plane ( e.g. scene radius )
    void setFarPlane(double _far) { /*obsolete*/ }

    /// Set distance to near plane
    void setNearPlane(double _near) { /*obsolete*/ }

    /// Return encoded view string
    QString getEncodedView() { return encodedView_; }

    /// Set if viewing frustum should be shown or not
    void showFrustum(bool _showFrustum) { showFrustum_ = _showFrustum; }

    /// Returns true if camera renders its whole frustum
    bool showFrustum() { return showFrustum_; }

private:

    void updateVBO();

    void updateBoundingBoxes();

    // recalculate frustum positions in world space
    void updateFrustumWS();

    OpenMesh::Vec3d bbmin_;
    OpenMesh::Vec3d bbmax_;

    ACG::GLMatrixd modelView_;
    ACG::GLMatrixd modelViewInv_;
    ACG::GLMatrixd projection_;

    GLCylinder* cylinder_;
    GLCone* cone_;

    GeometryBuffer      vbo_;
    IndexBuffer         ibo_;
    VertexDeclaration   vdecl_;
    bool                update_vbo_;
    std::vector<Vec4f>  vboData_; // 8 frustum vertices + camera position
    int                 offsetTris_; // offsets into index buffer
    int                 offsetLines_;
    int                 offsetFront_;

    QString encodedView_;

    bool showFrustum_;
};



//=============================================================================
} // namespace SceneGraph
} // namespace ACG

//=============================================================================
#endif // ACG_CameraVis_NODE_HH
//=============================================================================
