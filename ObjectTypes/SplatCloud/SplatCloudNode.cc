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

//================================================================
//
//  CLASS SplatCloudNode - IMPLEMENTATION
//
//================================================================


//== INCLUDES ====================================================

#include <ACG/GL/acg_glew.hh>
#include <ACG/GL/IRenderer.hh>

#include "SplatCloudNode.hh"


//== DEFINES =====================================================


//#define REPORT_VBO_UPDATES


//== NAMESPACES ==================================================


namespace ACG {
namespace SceneGraph {

//== STATIC MEMBER VARIABLES =====================================


const SplatCloudNode::Position  SplatCloudNode::DEFAULT_POSITION (0,0,0);
const SplatCloudNode::Index     SplatCloudNode::DEFAULT_INDEX    (   -1);
const SplatCloudNode::Viewlist  SplatCloudNode::DEFAULT_VIEWLIST (  0.0);
const SplatCloudNode::Selection SplatCloudNode::DEFAULT_SELECTION(false);


//== IMPLEMENTATION ==============================================


SplatCloudNode::SplatCloudNode( const SplatCloud &_splatCloud, BaseNode *_parent, std::string _name ) : 
  BaseNode            ( _parent, _name ), 
  splatCloud_         ( _splatCloud ), 
  positionsModified_  ( false ), 
  colorsModified_     ( false ), 
  normalsModified_    ( false ), 
  pointsizesModified_ ( false ), 
  selectionsModified_ ( false ), 
  pickColorsModified_ ( false ), 
  defaultColor_       ( Color    (0,0,0) ), 
  defaultNormal_      ( Normal   (0,0,0) ), 
  defaultPointsize_   ( Pointsize(  0.0) ), 
  splatsDrawMode_     ( DrawModes::NONE ), 
  dotsDrawMode_       ( DrawModes::NONE ), 
  pointsDrawMode_     ( DrawModes::NONE ), 
  pickingBaseIndex_   ( 0 ), 
  pickDrawMode_       ( DrawModes::NONE ), // TODO: hack, see enterPick()
  vboGlId_            ( 0 ), 
  vboNumSplats_       ( 0 ), 
  vboData_            ( 0 ), 
  vboStride_          ( 0 ),
  vboPositionsOffset_ ( -1 ), 
  vboColorsOffset_    ( -1 ), 
  vboNormalsOffset_   ( -1 ), 
  vboPointsizesOffset_( -1 ), 
  vboSelectionsOffset_( -1 ), 
  vboPickColorsOffset_( -1 ),
  pointsizeScale_     ( 1.0f ),
  backfaceCulling_    ( false )
{
  // add (possibly) new drawmodes
  pointsDrawMode_ = DrawModes::addDrawMode( "Points" );
  dotsDrawMode_   = DrawModes::addDrawMode( "Dots"   );
  splatsDrawMode_ = DrawModes::addDrawMode( "Splats" );

  // create a new VBO (will be invalid and rebuilt the next time drawn (or picked))
  createVBO();
}


//----------------------------------------------------------------


SplatCloudNode::~SplatCloudNode()
{
  destroyVBO();
}


//----------------------------------------------------------------


void SplatCloudNode::boundingBox( ACG::Vec3d &_bbMin, ACG::Vec3d &_bbMax )
{
  ACG::Vec3f bbMin( FLT_MAX, FLT_MAX, FLT_MAX );
  ACG::Vec3f bbMax(-FLT_MAX,-FLT_MAX,-FLT_MAX );

  if( splatCloud_.hasPositions() )
  {
    unsigned int i, num = splatCloud_.numSplats();
    for( i=0; i<num; ++i )
    {
      const Position &p = splatCloud_.positions( i );

      ACG::Vec3f acgp( p[0], p[1], p[2] );

      bbMin.minimize( acgp );
      bbMax.maximize( acgp );
    }
  }

  _bbMin.minimize( ACG::Vec3d( bbMin ) );
  _bbMax.maximize( ACG::Vec3d( bbMax ) );
}


//----------------------------------------------------------------


void SplatCloudNode::draw( GLState &_state, const DrawModes::DrawMode &_drawMode )
{

  static const int RENDERMODE_DOTS   = 1;
  static const int RENDERMODE_SPLATS = 2;

  // check if drawmode is valid
  int rendermode;
  if( _drawMode.containsAtomicDrawMode( splatsDrawMode_ ) )
    rendermode = RENDERMODE_SPLATS;
  else if( _drawMode.containsAtomicDrawMode( dotsDrawMode_ ) )
    rendermode = RENDERMODE_DOTS;
  else if( _drawMode.containsAtomicDrawMode( pointsDrawMode_ ) ) {
    static const int RENDERMODE_POINTS = 0;
    rendermode = RENDERMODE_POINTS;
  } else
     return;

  // set desired depth function
  ACG::GLState::depthFunc( _state.depthFunc() );

  // if VBO is invalid or was (partially) modified, then rebuild
  if( (vboData_ == 0) || vboModified() )
    rebuildVBO( _state );

  // if VBO is valid...
  if( vboData_ != 0 )
  {
    // activate VBO
    ACG::GLState::bindBufferARB( GL_ARRAY_BUFFER_ARB, vboGlId_ );

    // enable arrays:
    // --------------

    // positions
    if( vboPositionsOffset_ != -1 )
    {
      ACG::GLState::enableClientState( GL_VERTEX_ARRAY );
      ACG::GLState::vertexPointer( 3, GL_FLOAT, static_cast<GLsizei>(vboStride_), (unsigned char *) 0 + vboPositionsOffset_ );
    }
    else
    {
      ACG::GLState::disableClientState( GL_VERTEX_ARRAY );
    }

    // colors
    if( vboColorsOffset_ != -1 )
    {
      ACG::GLState::enableClientState( GL_SECONDARY_COLOR_ARRAY );
      glSecondaryColorPointer( 3, GL_UNSIGNED_BYTE, static_cast<GLsizei>(vboStride_), (unsigned char *) 0 + vboColorsOffset_ ); // TODO: use ACG::GLState::secondaryColorPointer() when implemented
    }
    else
    {
      ACG::GLState::disableClientState( GL_SECONDARY_COLOR_ARRAY );
      glSecondaryColor3ub( defaultColor_[0], defaultColor_[1], defaultColor_[2] );
    }

    // normals
    if( vboNormalsOffset_ != -1 )
    {
      ACG::GLState::enableClientState( GL_NORMAL_ARRAY );
      ACG::GLState::normalPointer( GL_FLOAT, static_cast<GLsizei>(vboStride_), (unsigned char *) 0 + vboNormalsOffset_ );
    }
    else
    {
      ACG::GLState::disableClientState( GL_NORMAL_ARRAY );
      glNormal3f( defaultNormal_[0], defaultNormal_[1], defaultNormal_[2] );
    }

    // pointsizes
    if( vboPointsizesOffset_ != -1 )
    {
      glClientActiveTexture( GL_TEXTURE0 ); // TODO: use ACG::GLState::clientActiveTexture() when implemented
      ACG::GLState::enableClientState( GL_TEXTURE_COORD_ARRAY );
      ACG::GLState::texcoordPointer( 1, GL_FLOAT, static_cast<GLsizei>(vboStride_), (unsigned char *) 0 + vboPointsizesOffset_ );
    }
    else
    {
      glClientActiveTexture( GL_TEXTURE0 ); // TODO: use ACG::GLState::clientActiveTexture() when implemented
      ACG::GLState::disableClientState( GL_TEXTURE_COORD_ARRAY );
      glMultiTexCoord1f( GL_TEXTURE0, defaultPointsize_ );
    }

    // selections
    if( vboSelectionsOffset_ != -1 )
    {
      glClientActiveTexture( GL_TEXTURE1 ); // TODO: use ACG::GLState::clientActiveTexture() when implemented
      ACG::GLState::enableClientState( GL_TEXTURE_COORD_ARRAY );
      ACG::GLState::texcoordPointer( 1, GL_FLOAT, static_cast<GLsizei>(vboStride_), (unsigned char *) 0 + vboSelectionsOffset_ );
    }
    else
    {
      glClientActiveTexture( GL_TEXTURE1 ); // TODO: use ACG::GLState::clientActiveTexture() when implemented
      ACG::GLState::disableClientState( GL_TEXTURE_COORD_ARRAY );
      glMultiTexCoord1f( GL_TEXTURE1, 0.0f );
    }

    // pick colors
    if( vboPickColorsOffset_ != -1 )
    {
      ACG::GLState::enableClientState( GL_COLOR_ARRAY );
      ACG::GLState::colorPointer( 4, GL_UNSIGNED_BYTE, static_cast<GLsizei>(vboStride_), (unsigned char *) 0 + vboPickColorsOffset_ );
    }
    else
    {
      ACG::GLState::disableClientState( GL_COLOR_ARRAY );
      glColor4ub( 255, 255, 255, 255 );
    }

    // render:
    // -------

    // enable "pointsize by program" depending on current rendermode
    if( rendermode == RENDERMODE_SPLATS || rendermode == RENDERMODE_DOTS )
      ACG::GLState::enable( GL_VERTEX_PROGRAM_POINT_SIZE );
    else
      ACG::GLState::disable( GL_VERTEX_PROGRAM_POINT_SIZE );

    // draw as GLpoints
    glDrawArrays( GL_POINTS, 0, vboNumSplats_ );

    // disable arrays:
    // ---------------

    // positions
    ACG::GLState::disableClientState( GL_VERTEX_ARRAY );

    // colors
    ACG::GLState::disableClientState( GL_SECONDARY_COLOR_ARRAY );

    // normals
    ACG::GLState::disableClientState( GL_NORMAL_ARRAY );

    // pointsizes
    glClientActiveTexture( GL_TEXTURE0 ); // TODO: use ACG::GLState::clientActiveTexture() when implemented
    ACG::GLState::disableClientState( GL_TEXTURE_COORD_ARRAY );

    // selections
    glClientActiveTexture( GL_TEXTURE1 ); // TODO: use ACG::GLState::clientActiveTexture() when implemented
    ACG::GLState::disableClientState( GL_TEXTURE_COORD_ARRAY );

    // pick colors
    ACG::GLState::disableClientState( GL_COLOR_ARRAY );

    // reset states:
    // -------------

    // disable "pointsize by program"
    ACG::GLState::disable( GL_VERTEX_PROGRAM_POINT_SIZE );

    // make defaults current again
    glClientActiveTexture( GL_TEXTURE0 ); // TODO: use ACG::GLState::clientActiveTexture() when implemented
    glColor4f         ( 1.0f, 1.0f, 1.0f, 1.0f );
    glSecondaryColor3f( 1.0f, 1.0f, 1.0f );

    // deactivate VBO
    ACG::GLState::bindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
  }
}

//----------------------------------------------------------------

void SplatCloudNode::getRenderObjects( IRenderer* _renderer, GLState& _state, const DrawModes::DrawMode& _drawMode, const Material* _mat )
{

  static const int RENDERMODE_DOTS = 1;
  static const int RENDERMODE_SPLATS = 2;

  // check if drawmode is valid
  int rendermode;
  if (_drawMode.containsAtomicDrawMode(splatsDrawMode_))
    rendermode = RENDERMODE_SPLATS;
  else if (_drawMode.containsAtomicDrawMode(dotsDrawMode_))
    rendermode = RENDERMODE_DOTS;
  else if (_drawMode.containsAtomicDrawMode(pointsDrawMode_)) {
    static const int RENDERMODE_POINTS = 0;
    rendermode = RENDERMODE_POINTS;
  }
  else
    return;

  ACG::RenderObject obj;

  obj.initFromState(&_state);
  obj.setMaterial(_mat);

  obj.depthTest = true;

  // if VBO is invalid or was (partially) modified, then rebuild
  if ((vboData_ == 0) || vboModified())
    rebuildVBO(_state);

  // if VBO is valid...
  if (vboData_ != 0)
  {
    // activate VBO
    obj.vertexBuffer = vboGlId_;


    // enable arrays:
    // --------------
    vboDecl_.clear();

    // positions
    if (vboPositionsOffset_ != -1)
      vboDecl_.addElement(GL_FLOAT, 3, ACG::VERTEX_USAGE_POSITION, size_t(vboPositionsOffset_));

    // colors
    if (vboColorsOffset_ != -1)
      vboDecl_.addElement(GL_UNSIGNED_BYTE, 3, ACG::VERTEX_USAGE_COLOR, size_t(vboColorsOffset_));
    else
    {
      obj.emissive = ACG::Vec3f(defaultColor_[0], defaultColor_[1], defaultColor_[2]);
      obj.emissive /= 255.0f;
    }

    // normals
    if (vboNormalsOffset_ != -1)
      vboDecl_.addElement(GL_FLOAT, 3, ACG::VERTEX_USAGE_NORMAL, size_t(vboNormalsOffset_));
    else
    {
      // todo
//      defaultNormal_;
    }

    // pointsizes
    if (vboPointsizesOffset_ != -1)
      vboDecl_.addElement(GL_FLOAT, 1, ACG::VERTEX_USAGE_TEXCOORD, size_t(vboPointsizesOffset_));
    else
    {
      // todo
//      glMultiTexCoord1f(GL_TEXTURE0, defaultPointsize_);
    }

    // selections
    if (vboSelectionsOffset_ != -1)
      vboDecl_.addElement(GL_FLOAT, 1, ACG::VERTEX_USAGE_SHADER_INPUT, size_t(vboSelectionsOffset_), "inSplatSelection");
    else
    {
      // todo
//      glMultiTexCoord1f(GL_TEXTURE1, 0.0f);
    }

    // pick colors
    if (vboPickColorsOffset_ != -1)
      vboDecl_.addElement(GL_UNSIGNED_BYTE, 4, ACG::VERTEX_USAGE_SHADER_INPUT, size_t(vboPickColorsOffset_), "inSplatPickColor");
    else
    {
      // todo
//       glColor4ub(255, 255, 255, 255);
    }

    vboDecl_.setVertexStride(vboStride_);

    obj.vertexDecl = &vboDecl_;

    // render:
    // -------

    // enable "pointsize by program" depending on current rendermode
    if (rendermode == RENDERMODE_SPLATS || rendermode == RENDERMODE_DOTS)
    {
      ACG::Vec2f screenSize(_state.viewport_width(), _state.viewport_height());
//      obj.setupPointRendering(defaultPointsize_, screenSize);
    }

    // setup shader
    obj.shaderDesc.shadeMode = SG_SHADE_UNLIT;
//     obj.shaderDesc.vertexTemplateFile = "SplatCloud_Splats/Vertex_shadergen.glsl";
//     obj.shaderDesc.fragmentTemplateFile = "SplatCloud_Splats/Fragment_shadergen.glsl";

    obj.shaderDesc.vertexTemplateFile = "SplatCloud_ShaderGen/splat_vs.glsl";
    obj.shaderDesc.geometryTemplateFile = "SplatCloud_ShaderGen/splat_gs.glsl";
    obj.shaderDesc.fragmentTemplateFile = "SplatCloud_ShaderGen/splat_fs.glsl";

    // setup uniforms
    /*
    uniform float pointsizeScale  = 1.0;
    uniform bool  backfaceCulling = false;
    uniform float modelviewScale;
    uniform float viewportScaleFov_y;

    uniform mat4  g_mPInv;

    uniform vec4  invViewportScale;
    uniform vec4  invViewportTransp;
    uniform float viewportScale_z;
    uniform float viewportTransp_z;
    */
    
    // get viewport
    int left, bottom, width, height;
    _state.get_viewport(left, bottom, width, height);

    float x = (float)left;
    float y = (float)bottom;
    float w = (float)width;
    float h = (float)height;

    // get depthrange
    // TODO: use glstate.get_depth_range when implemented
    GLfloat depthRange[2];
    glGetFloatv(GL_DEPTH_RANGE, depthRange);
    float z = (float)depthRange[0];
    float d = (float)depthRange[1] - z;

    // check if we are safe
    if (w <= 0.0f || h <= 0.0f || d <= 0.0f)
      return;

    // calculate window-coordinates to normalized-device-coordinates scale
    ACG::Vec4f invVPs;
    invVPs[0] = 2.0f / w;
    invVPs[1] = 2.0f / h;
    invVPs[2] = 2.0f / d;
    invVPs[3] = 0.0f;

    // calculate window-coordinates to normalized-device-coordinates transpose
    ACG::Vec4f invVPt;
    invVPt[0] = -(x * invVPs[0] + 1.0f);
    invVPt[1] = -(y * invVPs[1] + 1.0f);
    invVPt[2] = -(z * invVPs[2] + 1.0f);
    invVPt[3] = 1.0f;

    // calculate normalized-device-coordinates to window-coordinates scale and transpose
    GLfloat VPs_z = 0.5f * d;
    GLfloat VPt_z = z + VPs_z;

    // calculate scaling factor of modelview matrix
    static const double RCP_3 = 1.0 / 3.0;
    const ACG::GLMatrixd &mv = _state.modelview();
    double detMV = mv(0, 0) * (mv(1, 1)*mv(2, 2) - mv(1, 2)*mv(2, 1))
      + mv(0, 1) * (mv(1, 2)*mv(2, 0) - mv(1, 0)*mv(2, 2))
      + mv(0, 2) * (mv(1, 0)*mv(2, 1) - mv(1, 1)*mv(2, 0));
    GLfloat MVs = (GLfloat)pow(fabs(detMV), RCP_3);

    // calculate scale for pointsizes in eye-coordinates according to fovy and transformation to window-coordinates
    GLfloat VPsFov_y = _state.projection()(1, 1) * (0.5f * h);

    ACG::GLMatrixf projInv = _state.inverse_projection();

    obj.setUniform("invViewportScale", invVPs);
    obj.setUniform("invViewportTransp", invVPt);
    obj.setUniform("viewportScale_z", VPs_z);
    obj.setUniform("viewportTransp_z", VPt_z);
    obj.setUniform("modelviewScale", MVs);
    obj.setUniform("viewportScaleFov_y", VPsFov_y);
    obj.setUniform("invProjection", projInv);
    obj.setUniform("pointsizeScale", pointsizeScale_);
    obj.setUniform("backfaceCulling", backfaceCulling_);
    obj.setUniform("defaultPointsize", defaultPointsize_);
    obj.setUniform("defaultNormal", defaultNormal_);
    
    obj.programPointSize = true;
    
    if (vboColorsOffset_ != -1)
      obj.shaderDesc.vertexColors = true;

    // draw as GLpoints
    obj.glDrawArrays(GL_POINTS, 0, vboNumSplats_);

    _renderer->addRenderObject(&obj);
  }
}

//----------------------------------------------------------------


void SplatCloudNode::pick( GLState &_state, PickTarget _target )
{
  // if pick target is valid...
  if( _target == PICK_ANYTHING || _target == PICK_VERTEX )
  {
    // set number of pick colors used (each splats gets a unique pick color)
    if( !_state.pick_set_maximum( splatCloud_.numSplats() ) ) // number of splats could have changed, so use new number of splats (*not* the one in VBO memory!)
    {
      std::cerr << "SplatCloudNode::pick() : Color range too small, picking failed." << std::endl;
      return;
    }

    // if in color picking mode...
    if( _state.color_picking() )
    {
      // if picking base index has changed, rebuild pick colors block in VBO so new pick colors will be used
      if( pickingBaseIndex_ != _state.pick_current_index() )
      {
        pickColorsModified_ = true;
      }

      // TODO: hack, see enterPick()
      draw( _state, pickDrawMode_ );
    }
  }
}


//----------------------------------------------------------------


void SplatCloudNode::createVBO()
{
  // create new VBO (if *not* already existing)
  if( vboGlId_ == 0 )
  {
    glGenBuffersARB( 1, &vboGlId_ );
    vboNumSplats_ = 0;
    vboData_      = 0;

    modifiedAll(); // (re-)build all data block in VBO memory the first time
  }
}


//----------------------------------------------------------------


void SplatCloudNode::destroyVBO()
{
  // delete VBO (if existing)
  if( vboGlId_ != 0 )
  {
    glDeleteBuffersARB( 1, &vboGlId_ );
    vboGlId_      = 0;
    vboNumSplats_ = 0;
    vboData_      = 0;
  }
}


//----------------------------------------------------------------


void SplatCloudNode::rebuildVBO( GLState &_state )
{
  // if something went wrong in the initialization, make VBO invalid and abort
  if( vboGlId_ == 0 )
  {
    vboData_ = 0;
    return;
  }

  // activate VBO
  ACG::GLState::bindBufferARB( GL_ARRAY_BUFFER_ARB, vboGlId_ );

  // calculate size of data and offsets
  unsigned int numSplats = splatCloud_.numSplats();
  unsigned int stride    = 0;

  int positionsOffset  = -1;
  int colorsOffset     = -1;
  int normalsOffset    = -1;
  int pointsizesOffset = -1;
  int selectionsOffset = -1;
  int pickColorsOffset = -1;

  if( splatCloud_.hasPositions()  ) { positionsOffset  = stride; stride += 12; }
  if( splatCloud_.hasColors()     ) { colorsOffset     = stride; stride += 3;  }
  if( splatCloud_.hasNormals()    ) { normalsOffset    = stride; stride += 12; }
  if( splatCloud_.hasPointsizes() ) { pointsizesOffset = stride; stride += 4;  }
  if( splatCloud_.hasSelections() ) { selectionsOffset = stride; stride += 4;  }
  /* has pick colors is true     */ { pickColorsOffset = stride; stride += 4;  }

  // tell GL that we are seldomly updating the VBO but are often drawing it
  glBufferDataARB( GL_ARRAY_BUFFER_ARB, static_cast<GLsizei>(stride * numSplats), 0, GL_STATIC_DRAW_ARB );

  // get pointer to VBO memory
  unsigned char *data = (unsigned char *) glMapBufferARB( GL_ARRAY_BUFFER_ARB, GL_WRITE_ONLY_ARB );

  // check if something went wrong during VBO mapping
  if( data == 0 )
  {
    std::cout << "SplatCloudNode::rebuildVBO() : glMapBufferARB() failed." << std::endl;
    vboData_ = 0;
  }
  else
  {
    // if VBO memory block was moved or the internal block structure has to be changed, rebuild entire VBO
    if( (vboData_ != data) || vboStructureModified() )
    {
      vboNumSplats_ = numSplats;
      vboData_      = data;
      vboStride_    = stride;

      vboPositionsOffset_  = positionsOffset;
      vboColorsOffset_     = colorsOffset;
      vboNormalsOffset_    = normalsOffset;
      vboPointsizesOffset_ = pointsizesOffset;
      vboSelectionsOffset_ = selectionsOffset;
      vboPickColorsOffset_ = pickColorsOffset;

      // mark all data blocks to rebuild them lateron
      modifiedAll();
    }

    // if in color picking mode...
    if( _state.color_picking() )
    {
      // store picking base index
      pickingBaseIndex_ = _state.pick_current_index();
    }

    // rebuild data blocks if needed
    if( positionsModified_  ) rebuildVBOPositions();
    if( colorsModified_     ) rebuildVBOColors();
    if( normalsModified_    ) rebuildVBONormals();
    if( pointsizesModified_ ) rebuildVBOPointsizes();
    if( selectionsModified_ ) rebuildVBOSelections();
    if( pickColorsModified_ ) rebuildVBOPickColors( _state );

#   ifdef REPORT_VBO_UPDATES
    std::cout << std::endl;
#   endif

    // every block in VBO memory has been updated
    positionsModified_  = false;
    colorsModified_     = false;
    normalsModified_    = false;
    pointsizesModified_ = false;
    selectionsModified_ = false;
    pickColorsModified_ = false;

    // release pointer to VBO memory. if something went wrong, make VBO invalid and abort
    if( !glUnmapBufferARB( GL_ARRAY_BUFFER_ARB ) )
    {
      std::cout << "SplatCloudNode::rebuildVBO() : glUnmapBufferARB() failed." << std::endl;
      vboData_ = 0;
    }
  }

  // deactivate VBO
  ACG::GLState::bindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
}


//----------------------------------------------------------------


static void addFloatToBuffer( float _value, unsigned char *&_buffer )
{
  // get pointer
  unsigned char *v = (unsigned char *) &_value;

  // copy over 4 bytes
  *_buffer++ = *v++;
  *_buffer++ = *v++;
  *_buffer++ = *v++;
  *_buffer++ = *v;
}


//----------------------------------------------------------------


static void addIntToBuffer( int _value, unsigned char *&_buffer )
{
  // get pointer
  unsigned char *v = (unsigned char *) &_value;

  // copy over 4 bytes
  *_buffer++ = *v++;
  *_buffer++ = *v++;
  *_buffer++ = *v++;
  *_buffer++ = *v;
}


//----------------------------------------------------------------


static void addUCharToBuffer( unsigned char _value, unsigned char *&_buffer )
{
  // get pointer
  unsigned char *v = (unsigned char *) &_value;

  // copy over 1 byte
  *_buffer++ = *v;
}


//----------------------------------------------------------------


void SplatCloudNode::rebuildVBOPositions()
{
  if( (vboPositionsOffset_ == -1) || !splatCloud_.hasPositions() )
    return;

# ifdef REPORT_VBO_UPDATES
  std::cout << "SplatCloudNode::rebuildVBOPositions()" << std::endl;
# endif

  // get pointer to buffer
  unsigned char *buffer = vboData_ + vboPositionsOffset_;

  // for all positions...
  unsigned int i, num = splatCloud_.numSplats();
  for( i=0; i<num; ++i )
  {
    // add position
    const Position &p = getPosition( i );
    buffer = vboData_ + vboPositionsOffset_ + i * vboStride_;
    addFloatToBuffer( p[0], buffer );
    addFloatToBuffer( p[1], buffer );
    addFloatToBuffer( p[2], buffer );
  }
}


//----------------------------------------------------------------


void SplatCloudNode::rebuildVBOColors()
{
  if( (vboColorsOffset_ == -1) || !splatCloud_.hasColors()  )
    return;

# ifdef REPORT_VBO_UPDATES
  std::cout << "SplatCloudNode::rebuildVBOColors()" << std::endl;
# endif

  // get pointer to buffer
  unsigned char *buffer = vboData_ + vboColorsOffset_;

  // for all colors...
  unsigned int i, num = splatCloud_.numSplats();
  for( i=0; i<num; ++i )
  {
    // add color
    const Color &c = getColor( i );
    buffer = vboData_ + vboColorsOffset_ + i * vboStride_;

    addUCharToBuffer( c[0], buffer );
    addUCharToBuffer( c[1], buffer );
    addUCharToBuffer( c[2], buffer );
  }
}


//----------------------------------------------------------------


void SplatCloudNode::rebuildVBONormals()
{
  if( (vboNormalsOffset_ == -1) || !splatCloud_.hasNormals()  )
    return;

# ifdef REPORT_VBO_UPDATES
  std::cout << "SplatCloudNode::rebuildVBONormals()" << std::endl;
# endif

  // get pointer to buffer
  unsigned char *buffer = vboData_ + vboNormalsOffset_;

  // for all normals...
  unsigned int i, num = splatCloud_.numSplats();
  for( i=0; i<num; ++i )
  {
    // add normal
    const Normal &n = getNormal( i );
    buffer = vboData_ + vboNormalsOffset_ + i * vboStride_;

    addFloatToBuffer( n[0], buffer );
    addFloatToBuffer( n[1], buffer );
    addFloatToBuffer( n[2], buffer );
  }
}


//----------------------------------------------------------------


void SplatCloudNode::rebuildVBOPointsizes()
{
  if( (vboPointsizesOffset_ == -1) || !splatCloud_.hasPointsizes()  )
    return;

# ifdef REPORT_VBO_UPDATES
  std::cout << "SplatCloudNode::rebuildVBOPointsizes()" << std::endl;
# endif

  // get pointer to buffer
  unsigned char *buffer = vboData_ + vboPointsizesOffset_;

  // for all pointsizes...
  unsigned int i, num = splatCloud_.numSplats();
  for( i=0; i<num; ++i )
  {
    // add pointsize
    const Pointsize &ps = getPointsize( i );
    buffer = vboData_ + vboPointsizesOffset_ + i * vboStride_;
    
    addFloatToBuffer(ps, buffer);
  }
}


//----------------------------------------------------------------


void SplatCloudNode::rebuildVBOSelections()
{
  if( (vboSelectionsOffset_ == -1) || !splatCloud_.hasSelections()  )
    return;

# ifdef REPORT_VBO_UPDATES
  std::cout << "SplatCloudNode::rebuildVBOSelections()" << std::endl;
# endif

  // get pointer to buffer
  unsigned char *buffer = vboData_ + vboSelectionsOffset_;

  // for all selections...
  unsigned int i, num = splatCloud_.numSplats();
  for( i=0; i<num; ++i )
  {
    const bool &s = getSelection( i );
    buffer = vboData_ + vboSelectionsOffset_ + i * vboStride_;
    
    addFloatToBuffer((s ? 1.0f : 0.0f), buffer);
  }
}


//----------------------------------------------------------------


void SplatCloudNode::rebuildVBOPickColors( GLState &_state )
{
  if( (vboPickColorsOffset_ == -1) || !splatCloud_.hasPositions() )
    return;

# ifdef REPORT_VBO_UPDATES
  std::cout << "SplatCloudNode::rebuildVBOPickColors()" << std::endl;
# endif

  // get pointer to buffer
  unsigned char *buffer = vboData_ + vboPickColorsOffset_;

  // for all pick colors...
  unsigned int i, num = splatCloud_.numSplats();
  for( i=0; i<num; ++i )
  {
    // add pick color
    const Vec4uc &pc = _state.pick_get_name_color( i );
    buffer = vboData_ + vboPickColorsOffset_ + i * vboStride_;

    addUCharToBuffer( pc[0], buffer );
    addUCharToBuffer( pc[1], buffer );
    addUCharToBuffer( pc[2], buffer );
    addUCharToBuffer( pc[3], buffer );
  }
}


//================================================================


} // namespace SceneGraph
} // namespace ACG
