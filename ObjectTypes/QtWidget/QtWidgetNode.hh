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
//  CLASS QtWidgetNode
//
//=============================================================================

#ifndef QT_WIDGET_NODE_HH
#define QT_WIDGET_NODE_HH

//== INCLUDES =================================================================

#include <ObjectTypes/Plane/PlaneType.hh>
#include <ACG/Scenegraph/BaseNode.hh>
#include <ACG/Scenegraph/DrawModes.hh>
#include <ACG/GL/VertexDeclaration.hh>
#include <ACG/GL/IRenderer.hh>
#include <ACG/GL/GLPrimitives.hh>

#include <QObject>
#include <QWidget>

//== NAMESPACES ===============================================================

namespace ACG {
namespace SceneGraph {

//== CLASS DEFINITION =========================================================

class DLLEXPORT QtWidgetNode : public BaseNode
{
public:
    /** \brief Construct a QtWidget Node
     *
     * @param _widget The widget which will be rendered in the viewport
     * @param _parent The parent node in the scenegraph
     * @param _name   The name of the new node (visible in the scenegraph dialogs)
     */
    QtWidgetNode(QWidget* _widget, BaseNode *_parent = 0, std::string _name = "<QtWidgetNode>");

    /// destructor
    ~QtWidgetNode();

    /// static name of this class
    ACG_CLASSNAME(QtWidgetNode);

    /// return available draw modes
    ACG::SceneGraph::DrawModes::DrawMode availableDrawModes() const;

    /// update bounding box
    void boundingBox(ACG::Vec3d & _bbMin, ACG::Vec3d & _bbMax);

    /** \brief Add the objects to the given renderer
     *
     * @param _renderer The renderer which will be used. Add your geometry into this class
     * @param _state    The current GL State when this object is called
     * @param _drawMode The active draw mode
     * @param _mat      Current material
     */
    void getRenderObjects(ACG::IRenderer* _renderer, ACG::GLState&  _state , const ACG::SceneGraph::DrawModes::DrawMode&  _drawMode , const ACG::SceneGraph::Material* _mat);


    void mouseEvent(GLState& _state, QMouseEvent* _event);
    void mouseEvent(QMouseEvent* _event);

    QWidget* widget()const{return widget_;}
    /// set a new widget at the current widgets position (if last widget wasn't zero)
    void setWidget(QWidget* _w);

private:
    class NodeEventFilter : public QObject
    {
    public:
      NodeEventFilter(QtWidgetNode* p){node_ = p;}
    protected:
      bool eventFilter(QObject *obj, QEvent *event);
    private:
      QtWidgetNode *node_;
    } *ef_;

    friend class NodeEventFilter;

    /// create and update the widget texture
    void createTexture();

    /// widgetgeometry will be screen aligned. the width/height and position is in respect to the _state projection matrix
    void createGeometry(GLState& _state);

    /// update geometry on current position with old projection/view matrix
    void updateGeometry();

    /// upload widget plane data to graphics card
    void uploadPlane();

    /// VBO used to render the plane
    unsigned int vbo_;
    GLuint texID_;
    ACG::VertexDeclaration vertexDecl_;

    /// current widget
    QWidget* widget_;

    /// initial widgetHeight/Width. Is 0, if widget is 0 or if plane wasn't initialized with current view/projMatrix
    int oldWidgetWidth_;
    int oldWidgetHeight_;

    /// plane position and dimensions
    Plane plane_;
    bool planeCreated_;

    /// last state
    GLState* state_;

    bool anisotropicSupport_;

};

//=============================================================================
} // namespace SceneGraph
} // namespace ACG

//=============================================================================
#endif // QT_WIDGET_NODE_HH defined
//=============================================================================
