/*===========================================================================*\
 *                                                                           *
 *                              OpenFlipper                                  *
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
 *   $Author$                                                      *
 *   $Date$                   *
 *                                                                           *
\*===========================================================================*/

//=============================================================================
//
//  CLASS PointNode
//
//=============================================================================

#ifndef PLANENODE_HH
#define PLANENODE_HH

//== INCLUDES =================================================================

#include <ObjectTypes/Plane/PlaneType.hh>
#include <ACG/Scenegraph/BaseNode.hh>
#include <ACG/Scenegraph/DrawModes.hh>
#include <ACG/Math/Matrix4x4T.hh>
#include <OpenFlipper/common/ObjectTypeDLLDefines.hh>
#include <ACG/GL/VertexDeclaration.hh>
#include <ACG/GL/IRenderer.hh>
#include <ACG/GL/GLPrimitives.hh>

//== NAMESPACES ===============================================================

//== CLASS DEFINITION =========================================================

class OBJECTTYPEDLLEXPORT PlaneNode : public ACG::SceneGraph::BaseNode
{
public:
    /** \brief Construct a plane rendering node
     *
     * @param _parent The parent node in the scenegraph
     * @param _name   The name of the new node (visible in the scenegraph dialogs)
     * @param _plane  A pointer to an existing plane
     */
    PlaneNode(Plane& _plane, BaseNode *_parent = 0, std::string _name = "<PlaneNode>");

    /// destructor
    ~PlaneNode();

    /// static name of this class
    ACG_CLASSNAME(PlaneNode);

    /// return available draw modes
    ACG::SceneGraph::DrawModes::DrawMode availableDrawModes() const;

    /// update bounding box
    void boundingBox(ACG::Vec3d & _bbMin, ACG::Vec3d & _bbMax);

    /// draw Plane
    void draw(ACG::GLState & _state, const ACG::SceneGraph::DrawModes::DrawMode & _drawMode);

    /// draw Plane for object picking
    void pick(ACG::GLState & _state, ACG::SceneGraph::PickTarget _target);

    /// get center position of the plane
    ACG::Vec3d position();

    /// get current normal
    ACG::Vec3d normal();

    /// local x direction (multiplied with width)
    ACG::Vec3d xDirection();

    /// local y direction (multiplied with height)
    ACG::Vec3d yDirection();

    /// Get the currently rendered plane
    Plane& getPlane();

    /// Set a new plane for rendering
    void setPlane(Plane plane);

    /** \brief Add the objects to the given renderer
     *
     * @param _renderer The renderer which will be used. Add your geometry into this class
     * @param _state    The current GL State when this object is called
     * @param _drawMode The active draw mode
     * @param _mat      Current material
     */
    void getRenderObjects(ACG::IRenderer* _renderer, ACG::GLState&  _state , const ACG::SceneGraph::DrawModes::DrawMode&  _drawMode , const ACG::SceneGraph::Material* _mat);

    /// updates the plane before the next render call
    void update();

private:
    void drawPlane(ACG::GLState & _state);
    void drawManipulator(ACG::GLState & _state);
    void drawPlanePick(ACG::GLState & _state);
    void drawManipulatorPick(ACG::GLState & _state);

    /// create and update VBO
    void updateVBO();


    void addSphereAt(ACG::Vec3d _pos, ACG::IRenderer* _renderer, ACG::GLState&  _state, ACG::RenderObject* _ro);

    Plane& plane_;

    /// VBO used to render the plane
    unsigned int vbo_;
    
    bool vboNeedsUpdate_;

    ACG::VertexDeclaration vertexDecl_;

    ACG::GLSphere* sphere_;

};

//=============================================================================
#endif // ACG_PLANENODE_HH defined
//=============================================================================
