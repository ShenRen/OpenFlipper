#pragma once

//== INCLUDES =================================================================

#include "GCodeType.hh"
#include <ACG/Scenegraph/BaseNode.hh>
#include <ACG/Scenegraph/DrawModes.hh>
#include <OpenFlipper/common/GlobalDefines.hh>
#include <ObjectTypes/PolyLine/PolyLine.hh>
#include <ACG/GL/VertexDeclaration.hh>
#include <ACG/GL/IRenderer.hh>
#include <ACG/GL/GLPrimitives.hh>

//== FORWARDDECLARATIONS ======================================================

//== NAMESPACES ===============================================================

namespace ACG {
namespace SceneGraph {

//== CLASS DEFINITION =========================================================

class DLLEXPORT GCodeNode : public BaseNode
{
public:

  // typedefs for easy access
  typedef PolyLine::Point Point;

  /// Constructor
  GCodeNode(const GCode::Shared& _gc, BaseNode* _parent = 0, std::string _name = "<PolyLineCollectionNode>");

  /// Destructor
  virtual ~GCodeNode();

  GCode::Shared gcode() { return gcode_; }

  /// static name of this class
  ACG_CLASSNAME(GCodeNode);

  /// return available draw modes
  DrawModes::DrawMode availableDrawModes() const;

  /// update bounding box
  void boundingBox(Vec3d& _bbMin, Vec3d& _bbMax);

  /// draw lines and normals
  void draw(GLState& /*_state*/,const DrawModes::DrawMode& _drawMode);

  /// picking
  void pick(GLState& _state, PickTarget _target);

  /** \brief Add the objects to the given renderer
    *
    * @param _renderer The renderer which will be used. Add your geometry into this class
    * @param _state    The current GL State when this object is called
    * @param _drawMode The active draw mode
    * @param _mat      Current material
    */
  void getRenderObjects(ACG::IRenderer* _renderer, ACG::GLState&  _state , const ACG::SceneGraph::DrawModes::DrawMode&  _drawMode , const ACG::SceneGraph::Material* _mat);

  /// Trigger an update of the vbo
  void update();

  void reset_vbo(){
      offset_.first = 0;
      offset_.second = 0;
      updateVBO_ = true;
  }

    void set_distance(double _value){distance_ = _value; }

private:

  /// Copy constructor (not used)
  GCodeNode(const GCodeNode& _rhs);

  /// Assignment operator (not used)
  GCodeNode& operator=(const GCodeNode& _rhs);

  /// Buffer organization
  ACG::VertexDeclaration vertexDecl_;

  /** \brief Trigger an update of the vbo
   *
   * If the polyLine is changed, you have to call this function to update the buffers.
   *
   */
  void updateVBO();

  /** \brief Write vertex data for rendering to a buffer
   *
   * @param _vertex index of polyline vertex
   * @param _dst address of vertex in buffer
   */
  void writeVertex(PolyLine* _polyline, unsigned int _vertex, void* _dst);



private:

  /// The associated poly line
  GCode::Shared gcode_;

  /// VBO used to render the poly line
  unsigned int vbo_;

  /// Flag to trigger update of vbo
  bool updateVBO_;

  /// Sphere for VertexSphere DrawMode
  GLSphere* sphere_;

  std::pair<int, int> offset_;

  int total_vertex_count_;

  double distance_;

  void* renderer_;

  DrawModes::DrawMode MODE_COLOR;
  DrawModes::DrawMode MODE_HEAT;
  DrawModes::DrawMode MODE_SPEED;
  DrawModes::DrawMode MODE_TYPE;
};


//=============================================================================
} // namespace SceneGraph
} // namespace ACG
//=============================================================================

