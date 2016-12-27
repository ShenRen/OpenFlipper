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
 *   $Revision$                                                       *
 *   $Author$                                                      *
 *   $Date$                   *
 *                                                                           *
\*===========================================================================*/




//=============================================================================
//
//  CLASS PointNode
//
//=============================================================================


#ifndef ACG_COORDSYSNODE_HH
#define ACG_COORDSYSNODE_HH


//== INCLUDES =================================================================

#include <ACG/Scenegraph/BaseNode.hh>
#include <ACG/Scenegraph/DrawModes.hh>
#include <ACG/GL/GLPrimitives.hh>
#include <OpenFlipper/common/ObjectTypeDLLDefines.hh>
#include <vector>

//== NAMESPACES ===============================================================

namespace ACG {
namespace SceneGraph {

//== CLASS DEFINITION =========================================================


/** \class CoordinateSystemNode
 * \brief Node for displaying coordinate systems

 CoordinateSystemNode renders a coordinate system.

**/

class OBJECTTYPEDLLEXPORT CoordinateSystemNode : public BaseNode
{

public:
  
  /** default constructor
   * @param _parent Define the parent Node this node gets attached to
   * @param _name Name of this Node
   */
  CoordinateSystemNode( BaseNode* _parent=0, std::string  _name="<TextNode>");

  /// destructor
  ~CoordinateSystemNode();

  /// static name of this class
  ACG_CLASSNAME(CoordinateSystemNode);

  /// return available draw modes
  ACG::SceneGraph::DrawModes::DrawMode  availableDrawModes() const;

  /// update bounding box
  void boundingBox(Vec3d& _bbMin, Vec3d& _bbMax);

  /// draw Coordsys
  void draw(GLState& _state, const DrawModes::DrawMode& _drawMode);
  
  /// draw Coordsys for object picking
  void pick(GLState& _state, PickTarget _target);

  /// set position of the coordsys
  void position(const Vec3d& _pos);

  /// Get current position of the coordinate system;
  Vec3d position();

  /// Get current rotation of the coordinate system;
  Matrix4x4d rotation();

  /// Set the rotation of the coordinate system;
  void rotation(const Matrix4x4d & _rotation);

  /// set size of the coordsys ( Size is length of one of the axis )
  void size(const double _size);

  /// Get current size
  double size();

  private:

    void drawCoordsys(GLState&  _state);
    void drawCoordsysPick(GLState&  _state);

    /// 3d position of the coordsys origin
    Vec3d  position_;
    
    /// Orientation of coordsys
    Matrix4x4d rotation_;

    /// Size of the coordsys
    double coordsysSize_;

    int slices_;
    int stacks_;
    int loops_;

    GLSphere* sphere_;
    GLCone* cone_;
    GLCylinder* cylinder_;
    GLDisk* disk_;
};


//=============================================================================
} // namespace SceneGraph
} // namespace ACG
//=============================================================================
#endif // ACG_COORDSYSNODE_HH defined
//=============================================================================
