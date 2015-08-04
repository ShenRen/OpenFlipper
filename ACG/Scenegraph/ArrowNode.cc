/*===========================================================================*\
 *                                                                           *
 *                              OpenFlipper                                  *
 *      Copyright (C) 2001-2014 by Computer Graphics Group, RWTH Aachen      *
 *                           www.openflipper.org                             *
 *                                                                           *
 *---------------------------------------------------------------------------*
 *  This file is part of OpenFlipper.                                        *
 *                                                                           *
 *  OpenFlipper is free software: you can redistribute it and/or modify      *
 *  it under the terms of the GNU Lesser General Public License as           *
 *  published by the Free Software Foundation, either version 3 of           *
 *  the License, or (at your option) any later version with the              *
 *  following exceptions:                                                    *
 *                                                                           *
 *  If other files instantiate templates or use macros                       *
 *  or inline functions from this file, or you compile this file and         *
 *  link it with other files to produce an executable, this file does        *
 *  not by itself cause the resulting executable to be covered by the        *
 *  GNU Lesser General Public License. This exception does not however       *
 *  invalidate any other reasons why the executable file might be            *
 *  covered by the GNU Lesser General Public License.                        *
 *                                                                           *
 *  OpenFlipper is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *  GNU Lesser General Public License for more details.                      *
 *                                                                           *
 *  You should have received a copy of the GNU LesserGeneral Public          *
 *  License along with OpenFlipper. If not,                                  *
 *  see <http://www.gnu.org/licenses/>.                                      *
 *                                                                           *
\*===========================================================================*/

/*===========================================================================*\
 *                                                                           *
 *   $Revision: 20771 $                                                       *
 *   $Author: moeller $                                                      *
 *   $Date: 2015-06-03 16:47:45 +0200 (Wed, 03 Jun 2015) $                   *
 *                                                                           *
\*===========================================================================*/




//=============================================================================
//
//  CLASS ArrowNode - IMPLEMENTATION
//
//=============================================================================

//== INCLUDES =================================================================
#include <ACG/GL/acg_glew.hh>
#include "ArrowNode.hh"
#include <ACG/GL/IRenderer.hh>
#include <OpenMesh/Core/Utils/vector_cast.hh>

//== NAMESPACES ===============================================================

namespace ACG {
namespace SceneGraph {

//== IMPLEMENTATION ==========================================================

ArrowNode::ArrowNode(BaseNode*    _parent, std::string  _name )
 : MaterialNode(_parent, _name, MaterialNode::BaseColor),
   numVertices_(0),
   numIndices_(0),
   localArrowMin_(0.0f, 0.0f, 0.0f),
   localArrowMax_(0.0f, 0.0f, 0.0f),
   invalidateInstanceData_(true),
   invalidateInstanceBuffer_(true),
   supportsInstancing_(-1)
{
  drawMode(DrawModes::SOLID_FLAT_SHADED);
}

//----------------------------------------------------------------------------

ArrowNode::~ArrowNode() 
{

}
//----------------------------------------------------------------------------

void ArrowNode::boundingBox(Vec3d& _bbMin, Vec3d& _bbMax)
{
  float radius = std::max(localArrowMax_[1] - localArrowMin_[1], localArrowMax_[2] - localArrowMin_[2]) * 0.5f;

  size_t n = arrows_.size();
  for (size_t i = 0; i < n; ++i)
  {
    const Arrow* a = &arrows_[i];

    // start and end point
    Vec3f s = a->start;
    Vec3f e = a->start + a->dir * a->scale;

    // conv to double precision
    Vec3d sd = OpenMesh::vector_cast<Vec3d>(s);
    Vec3d ed = OpenMesh::vector_cast<Vec3d>(e);

    // enlarge aabb by some amount to account for the volumetric shape
    double offset = double(radius * a->scale);
    Vec3d volEnlargeOffset = Vec3d(offset, offset, offset);

    _bbMin.minimize(sd - volEnlargeOffset);
    _bbMin.minimize(ed - volEnlargeOffset);

    _bbMax.maximize(sd + volEnlargeOffset);
    _bbMax.maximize(ed + volEnlargeOffset);
  }
}


//----------------------------------------------------------------------------

DrawModes::DrawMode ArrowNode::availableDrawModes() const
{
  return DrawModes::SOLID_FLAT_SHADED;
}

//----------------------------------------------------------------------------

void ArrowNode::createArrowMesh()
{
  if (!numVertices_)
  {
    // arrow mesh data as result from meshcompiler:
    // interleaved float3 pos, float3 normal
    float vdata[] =
    {
      0.681818f, -0.034091f, -0.068182f, 0.000000f, -1.000000f, 0.000000f,
      0.681818f, -0.034091f, 0.068182f, 0.000000f, -1.000000f, 0.000000f,
      0.000000f, -0.034091f, 0.000000f, 0.000000f, -1.000000f, 0.000000f,
      1.000000f, -0.034091f, 0.000000f, 0.000000f, -1.000000f, 0.000000f,
      0.681818f, -0.034091f, -0.227273f, 0.000000f, -1.000000f, 0.000000f,
      0.681818f, -0.034091f, 0.227273f, 0.000000f, -1.000000f, 0.000000f,
      0.681818f, 0.034091f, -0.068182f, 0.000000f, 1.000000f, -0.000000f,
      1.000000f, 0.034091f, 0.000000f, 0.000000f, 1.000000f, 0.000000f,
      0.681818f, 0.034091f, -0.227273f, -0.000000f, 1.000000f, 0.000000f,
      0.681818f, 0.034091f, 0.068182f, 0.000000f, 1.000000f, -0.000000f,
      0.000000f, 0.034091f, 0.000000f, 0.000000f, 1.000000f, -0.000000f,
      0.681818f, 0.034091f, 0.227273f, 0.000000f, 1.000000f, 0.000000f,
      1.000000f, -0.034091f, 0.000000f, 0.581238f, 0.000000f, -0.813734f,
      0.681818f, 0.034091f, -0.227273f, 0.581238f, 0.000000f, -0.813734f,
      1.000000f, 0.034091f, 0.000000f, 0.581238f, 0.000000f, -0.813734f,
      0.681818f, -0.034091f, -0.227273f, 0.581238f, 0.000000f, -0.813734f,
      0.681818f, -0.034091f, 0.227273f, 0.581238f, 0.000000f, 0.813734f,
      1.000000f, 0.034091f, 0.000000f, 0.581238f, 0.000000f, 0.813734f,
      0.681818f, 0.034091f, 0.227273f, 0.581238f, -0.000000f, 0.813734f,
      1.000000f, -0.034091f, 0.000000f, 0.581238f, 0.000000f, 0.813734f,
      0.681818f, -0.034091f, 0.068182f, -1.000000f, -0.000000f, 0.000000f,
      0.681818f, 0.034091f, 0.227273f, -1.000000f, -0.000000f, 0.000000f,
      0.681818f, 0.034091f, 0.068182f, -1.000000f, 0.000000f, 0.000000f,
      0.681818f, -0.034091f, 0.227273f, -1.000000f, -0.000000f, 0.000000f,
      0.681818f, -0.034091f, -0.227273f, -1.000000f, -0.000000f, 0.000000f,
      0.681818f, 0.034091f, -0.068182f, -1.000000f, -0.000000f, 0.000000f,
      0.681818f, 0.034091f, -0.227273f, -1.000000f, 0.000000f, 0.000000f,
      0.681818f, -0.034091f, -0.068182f, -1.000000f, -0.000000f, 0.000000f,
      0.000000f, -0.034091f, 0.000000f, -0.099504f, 0.000000f, 0.995037f,
      0.681818f, 0.034091f, 0.068182f, -0.099504f, 0.000000f, 0.995037f,
      0.000000f, 0.034091f, 0.000000f, -0.099504f, 0.000000f, 0.995037f,
      0.681818f, -0.034091f, 0.068182f, -0.099504f, 0.000000f, 0.995037f,
      0.681818f, -0.034091f, -0.068182f, -0.099504f, -0.000000f, -0.995037f,
      0.000000f, 0.034091f, 0.000000f, -0.099504f, -0.000000f, -0.995037f,
      0.681818f, 0.034091f, -0.068182f, -0.099504f, 0.000000f, -0.995037f,
      0.000000f, -0.034091f, 0.000000f, -0.099504f, -0.000000f, -0.995037f,
    };

    // indices
    int idata[] =
    {
      0, 1, 2,
      0, 3, 1,
      0, 4, 3,
      1, 3, 5,
      6, 7, 8,
      6, 9, 7,
      6, 10, 9,
      7, 9, 11,
      12, 13, 14,
      13, 12, 15,
      16, 17, 18,
      17, 16, 19,
      20, 21, 22,
      21, 20, 23,
      24, 25, 26,
      25, 24, 27,
      28, 29, 30,
      29, 28, 31,
      32, 33, 34,
      33, 32, 35,
    };

    // vertex decl
    if (!vertexDecl_.getNumElements())
    {
      vertexDecl_.addElement(GL_FLOAT, 3, VERTEX_USAGE_POSITION);
      vertexDecl_.addElement(GL_FLOAT, 3, VERTEX_USAGE_NORMAL);
    }

    numVertices_ = sizeof(vdata) / vertexDecl_.getVertexStride();
    numIndices_ = sizeof(idata) / sizeof(idata[0]);

    // vertex + index buffer
    vertexBuffer_.upload(sizeof(vdata), vdata, GL_STATIC_DRAW);
    indexBuffer_.upload(sizeof(idata), idata, GL_STATIC_DRAW);

    // compute local aabb
    localArrowMin_ = Vec3f(vdata[0], vdata[1], vdata[2]);
    localArrowMax_ = localArrowMin_;

    for (int i = 1; i < numVertices_; ++i)
    {
      Vec3f v = Vec3f(vdata[i * 6], vdata[i * 6 + 1], vdata[i * 6 + 2]);
      localArrowMin_.minimize(v);
      localArrowMax_.maximize(v);
    }
  }
}

//----------------------------------------------------------------------------

void ArrowNode::draw(GLState&  _state, const DrawModes::DrawMode& _drawMode)
{
  createArrowMesh();
  updateInstanceData();

  vertexBuffer_.bind();
  indexBuffer_.bind();

  vertexDecl_.activateFixedFunction();

  // save model-view matrix 
  GLMatrixf viewMatrix;
  glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat*)&viewMatrix);


  // GL_COLOR_MATERIAL: multiply the glColor value with the ambient and diffuse term
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

  for (size_t i = 0; i < arrows_.size(); ++i)
  {
    // use transform of the individual arrows
    GLMatrixf worldView = viewMatrix * readWorldMatrix(i);
    glLoadMatrixf(worldView.data());

    Vec4uc c = arrows_[i].color;
    glColor4ub(c[0], c[1], c[2], c[3]);

    glDrawElements(GL_TRIANGLES, numIndices_, GL_UNSIGNED_INT, 0);
  }

  // restore model-view matrix
  glLoadMatrixf(viewMatrix.data());

  glDisable(GL_COLOR_MATERIAL);
  vertexDecl_.deactivateFixedFunction();
}

//----------------------------------------------------------------------------

void ArrowNode::getRenderObjects(IRenderer* _renderer, GLState&  _state , const DrawModes::DrawMode&  _drawMode , const ACG::SceneGraph::Material* _mat) 
{
  if (arrows_.empty())
    return;

  createArrowMesh();
  updateInstanceBuffer();

  
  RenderObject obj;
  obj.initFromState(&_state);
  obj.depthTest = true;

  obj.vertexBuffer = vertexBuffer_.id();
  obj.indexBuffer = indexBuffer_.id();


  // check support for instancing if not done yet
  if (supportsInstancing_ < 0)
    supportsInstancing_ = checkExtensionSupported("GL_ARB_instanced_arrays") ? 1 : 0;

  // config shader
  obj.shaderDesc.shadeMode = SG_SHADE_FLAT;

  if (supportsInstancing_)
  {
    // render with instancing
    obj.shaderDesc.vertexColors = true;
    obj.shaderDesc.vertexTemplateFile = "ArrowNode/instancing_vs.glsl";
    obj.vertexDecl = &vertexDeclInstanced_;
    obj.glDrawElementsInstanced(GL_TRIANGLES, numIndices_, GL_UNSIGNED_INT, 0, arrows_.size());

    _renderer->addRenderObject(&obj);
  }
  else
  {
    // no instancing support
    // might want to abort after the first few objects to avoid emitting thousands of objects here and overloading the renderer
    
    obj.shaderDesc.vertexColors = false;
    obj.vertexDecl = &vertexDecl_;

    GLMatrixf viewMatrix = obj.modelview;

    for (size_t i = 0; i < arrows_.size(); ++i)
    {
      const Arrow* a = &arrows_[i];

      obj.modelview = viewMatrix * readWorldMatrix(i);
      obj.diffuse = Vec3f(a->color[0] / 255.0f, a->color[1] / 255.0f, a->color[2] / 255.0f);
      obj.alpha = a->color[3] / 255.0f;

      obj.glDrawElements(GL_TRIANGLES, numIndices_, GL_UNSIGNED_INT, 0);
      _renderer->addRenderObject(&obj);
    }
  }
}

//----------------------------------------------------------------------------

void ArrowNode::reserve(int _n)
{
  arrows_.reserve(_n);
}

//----------------------------------------------------------------------------

void ArrowNode::clear()
{
  arrows_.clear();
}

//----------------------------------------------------------------------------

int ArrowNode::n_arrows() const
{
  return int(arrows_.size());
}

//----------------------------------------------------------------------------

GLMatrixf ArrowNode::computeWorldMatrix(int _arrow) const
{
  const Arrow* a = &arrows_[_arrow];

  GLMatrixf align;
  align.identity();

  align.translate(a->start);

  // orientation
  //  local mesh stored as: dir in +x,  normal in +y
  Vec3f binormal = a->dir % a->normal;
  for (int i = 0; i < 3; ++i)
  {
    align(i, 0) = a->dir[i];
    align(i, 1) = a->normal[i];
    align(i, 2) = binormal[i];
  }

  align.scale(a->scale, a->scale, a->scale);

  return align;
}

//----------------------------------------------------------------------------

int ArrowNode::addArrow(const Vec3f& _start, const Vec3f& _dir, const Vec3f& _normal /*= Vec3f(0.0f, 1.0f, 0.0f)*/, float _scale /*= 1.0f*/, const Vec4uc& _color /*= Vec4uc(82, 82, 82, 255)*/)
{
  Arrow a;
  a.start = _start;
  a.dir = _dir;
  a.scale = _scale;
  a.color = _color;
  a.normal = _normal;

  a.orthonormalize();

  arrows_.push_back(a);

  invalidateInstanceData_ = true;
  invalidateInstanceBuffer_ = true;

  return int(arrows_.size()) - 1;
}

//----------------------------------------------------------------------------

void ArrowNode::Arrow::orthonormalize()
{
  dir.normalize();

  // make sure dir and normal are linearly independent
  if (normal.sqrnorm() < 1e-6f)
    normal = Vec3f(0.0f, 1.0f, 0.0f);

  while (std::fabs(dir | normal) > 0.99f || normal.sqrnorm() < 0.01f)
  {
    for (int i = 0; i < 3; ++i)
      normal[i] = float(rand()) / float(RAND_MAX) * 2.0f - 1.0f;
    normal.normalize();
  }
  // orthogonalize normal dir
  Vec3f binormal = dir % normal;
  normal = (binormal % dir).normalized();
}

//----------------------------------------------------------------------------

int ArrowNode::addArrow(const Vec3d& _start, const Vec3d& _dir, const Vec3d& _normal /*= Vec3d(0.0f, 1.0f, 0.0f)*/, double _scale /*= 1.0f*/, const Vec4uc& _color /*= Vec4uc(82, 82, 82, 255)*/)
{
  return addArrow(OpenMesh::vector_cast<Vec3f>(_start), OpenMesh::vector_cast<Vec3f>(_dir), OpenMesh::vector_cast<Vec3f>(_normal), float(_scale), _color);
}

//----------------------------------------------------------------------------

void ArrowNode::updateInstanceData()
{
  if (invalidateInstanceData_)
  {
    const int numArrows = arrows_.size();

    // size in dwords of data for one instance
    const int instanceSize = instanceDataSize();

    instanceData_.resize(numArrows * instanceSize);

    for (int i = 0; i < numArrows; ++i)
    {
      // compute and store 4x3 world matrix
      GLMatrixf m = computeWorldMatrix(i);
      int offset = instanceDataOffset(i);

      // linearize matrix and store in row-major
      for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 4; ++c)
          instanceData_[offset + r*4 + c] = m(r,c);

      // store color in last dword as rgba8_unorm
//      instanceData_[offset + 4 * 3] = *(float*)(arrows_[i].color.data());
      memcpy(&instanceData_[offset + 4*3], arrows_[i].color.data(), 4);

      // append more data here as needed
    }

    invalidateInstanceData_ = false;
  }
}

//----------------------------------------------------------------------------

ACG::GLMatrixf ArrowNode::readWorldMatrix( int _arrow ) const
{
  int offset = instanceDataOffset(_arrow);
  GLMatrixf m;

  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 4; ++c)
      m(r,c) = instanceData_[offset + r*4 + c];

  // last row not explicitly stored
  m(3,0) = 0.0f;  m(3,1) = 0.0f;  m(3,2) = 0.0f;  m(3,3) = 1.0f;

  return m;
}

//----------------------------------------------------------------------------

void ArrowNode::updateInstanceBuffer()
{
  if (invalidateInstanceBuffer_ || invalidateInstanceData_)
  {
    updateInstanceData();

    if (!instanceData_.empty())
      instanceBuffer_.upload(4 * instanceData_.size(), &instanceData_[0], GL_STATIC_DRAW);

    if (!vertexDeclInstanced_.getNumElements())
    {
      // position and normal from static mesh vbo
      vertexDeclInstanced_.addElement(GL_FLOAT, 3, VERTEX_USAGE_POSITION);
      vertexDeclInstanced_.addElement(GL_FLOAT, 3, VERTEX_USAGE_NORMAL);

      // world matrix and color from instance data vbo
      vertexDeclInstanced_.addElement(GL_FLOAT, 4, VERTEX_USAGE_SHADER_INPUT, size_t(0), "inWorld0", 1, instanceBuffer_.id());
      vertexDeclInstanced_.addElement(GL_FLOAT, 4, VERTEX_USAGE_SHADER_INPUT, size_t(0), "inWorld1", 1, instanceBuffer_.id());
      vertexDeclInstanced_.addElement(GL_FLOAT, 4, VERTEX_USAGE_SHADER_INPUT, size_t(0), "inWorld2", 1, instanceBuffer_.id());
      vertexDeclInstanced_.addElement(GL_UNSIGNED_BYTE, 4, VERTEX_USAGE_COLOR, size_t(0), 0, 1, instanceBuffer_.id());
    }

    invalidateInstanceBuffer_ = false;
  }
}

//----------------------------------------------------------------------------

Vec3f ArrowNode::arrowStart(int _arrowID) const
{
  return arrows_[_arrowID].start;
}

void ArrowNode::arrowStart(int _arrowID, const Vec3f& _start)
{
  arrows_[_arrowID].start = _start;
  invalidateInstanceData_ = true;
}

Vec3f ArrowNode::arrowDir(int _arrowID) const
{
  return arrows_[_arrowID].dir;
}

void ArrowNode::arrowDir(int _arrowID, const Vec3f& _dir)
{
  arrows_[_arrowID].dir = _dir;
  arrows_[_arrowID].orthonormalize();
  invalidateInstanceData_ = true;
}

Vec3f ArrowNode::arrowNormal(int _arrowID) const
{
  return arrows_[_arrowID].normal;
}

void ArrowNode::arrowNormal(int _arrowID, const Vec3f& _normal)
{
  arrows_[_arrowID].normal = _normal;
  arrows_[_arrowID].orthonormalize();
  invalidateInstanceData_ = true;
}

float ArrowNode::arrowScale(int _arrowID) const
{
  return arrows_[_arrowID].scale;
}

void ArrowNode::arrowScale(int _arrowID, float _scale)
{
  arrows_[_arrowID].scale = _scale;
  invalidateInstanceData_ = true;
}

Vec4uc ArrowNode::arrowColor(int _arrowID) const
{
  return arrows_[_arrowID].color;
}

void ArrowNode::arrowColor(int _arrowID, const Vec4uc& _color)
{
  arrows_[_arrowID].color = _color;
  invalidateInstanceData_ = true;
}


//=============================================================================


} // namespace SceneGraph
} // namespace ACG
//=============================================================================
