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
//  CLASS SplatCloudNode
//
//    SplatCloudNode renders splats by passing positions, normals, pointsizes and colors (and picking colors) to the GL.
//    These elements are internally stored in an array using an OpenGL vertex-buffer-object
//    including vertices, normals, texcoords and colors.
//
//================================================================


#ifndef ACG_SPLATCLOUDNODE_HH
#define ACG_SPLATCLOUDNODE_HH


//== INCLUDES ====================================================


#include "SplatCloud/SplatCloud.hh"

#include <OpenFlipper/common/GlobalDefines.hh>

#include <ACG/Scenegraph/BaseNode.hh>

#include <ACG/Scenegraph/DrawModes.hh>

#include <ACG/GL/gl.hh>

#include <ACG/GL/VertexDeclaration.hh>

//== NAMESPACES ==================================================


namespace ACG {
namespace SceneGraph {


//== CLASS DEFINITION ============================================


/** \class SplatCloudNode SplatCloudNode.hh <ObjectTypes/SplatCloud/SplatCloudNode.hh>
 *
 * This class is able to render a SplatCloud as splats, dots or points
 */

class DLLEXPORT SplatCloudNode : public BaseNode
{
public:

  //-- TYPEDEFS ----------------------------------------------------

  typedef SplatCloud::Position  Position;
  typedef SplatCloud::Color     Color;
  typedef SplatCloud::Normal    Normal;
  typedef SplatCloud::Pointsize Pointsize;
  typedef SplatCloud::Index     Index;
  typedef SplatCloud::Viewlist  Viewlist;
  typedef SplatCloud::Selection Selection;

  //----------------------------------------------------------------

public:

  /// constructor
  SplatCloudNode( const SplatCloud &_splatCloud, BaseNode *_parent = 0, std::string _name = "<SplatCloudNode>" );

  /// destructor
  ~SplatCloudNode();

  ACG_CLASSNAME( SplatCloudNode );

  /// return available draw modes
  inline DrawModes::DrawMode availableDrawModes() const { return splatsDrawMode_ | dotsDrawMode_ | pointsDrawMode_; }

  /// update bounding box
  void boundingBox( ACG::Vec3d &_bbMin, ACG::Vec3d &_bbMax );

  /// draw the SplatCloud
  void draw( GLState &_state, const DrawModes::DrawMode &_drawMode );

  /// create render objects
  void getRenderObjects( IRenderer* _renderer, GLState& _state, const DrawModes::DrawMode& _drawMode, const Material* _mat );

  /// picking
  void pick( GLState &_state, PickTarget _target );

  // TODO: hack, because pick() doesn't get a drawmode
  inline void enterPick( GLState &_state, PickTarget _target, const DrawModes::DrawMode &_drawMode ) { pickDrawMode_ = _drawMode; }

  // ---- splat cloud ----

  inline const SplatCloud &splatCloud() const { return splatCloud_; }

  // ---- modification tags ----

  inline void modifiedPositions()  { positionsModified_  = true; }
  inline void modifiedColors()     { colorsModified_     = true; }
  inline void modifiedNormals()    { normalsModified_    = true; }
  inline void modifiedPointsizes() { pointsizesModified_ = true; }
  inline void modifiedSelections() { selectionsModified_ = true; }
  inline void modifiedPickColors() { pickColorsModified_ = true; }

  inline void modifiedAll()
  {
    modifiedPositions();
    modifiedColors();
    modifiedNormals();
    modifiedPointsizes();
    modifiedSelections();
    modifiedPickColors();
  }

  // ---- default values ----

  inline void setDefaultColor    ( const Color     &_color     ) { defaultColor_     = _color;     }
  inline void setDefaultNormal   ( const Normal    &_normal    ) { defaultNormal_    = _normal;    }
  inline void setDefaultPointsize( const Pointsize &_pointsize ) { defaultPointsize_ = _pointsize; }

  inline const Color     &defaultColor()     const { return defaultColor_;     }
  inline const Normal    &defaultNormal()    const { return defaultNormal_;    }
  inline const Pointsize &defaultPointsize() const { return defaultPointsize_; }

  /// if the data array exists, the entry with the given index is returned, otherwise the default value is returned
  inline const Position  &getPosition ( int _idx ) const { return splatCloud_.hasPositions()  ? splatCloud_.positions ( _idx ) : DEFAULT_POSITION;  }
  inline const Color     &getColor    ( int _idx ) const { return splatCloud_.hasColors()     ? splatCloud_.colors    ( _idx ) : defaultColor_;     }
  inline const Normal    &getNormal   ( int _idx ) const { return splatCloud_.hasNormals()    ? splatCloud_.normals   ( _idx ) : defaultNormal_;    }
  inline const Pointsize &getPointsize( int _idx ) const { return splatCloud_.hasPointsizes() ? splatCloud_.pointsizes( _idx ) : defaultPointsize_; }
  inline const Index     &getIndex    ( int _idx ) const { return splatCloud_.hasIndices()    ? splatCloud_.indices   ( _idx ) : DEFAULT_INDEX;     }
  inline const Viewlist  &getViewlist ( int _idx ) const { return splatCloud_.hasViewlists()  ? splatCloud_.viewlists ( _idx ) : DEFAULT_VIEWLIST;  }
  inline const Selection &getSelection( int _idx ) const { return splatCloud_.hasSelections() ? splatCloud_.selections( _idx ) : DEFAULT_SELECTION; }


  // ---- render object params ----

  inline void setPointsizeScale(float _scale) { pointsizeScale_ = _scale; }
  inline void setBackfaceCulling(bool _enable) { backfaceCulling_ = _enable; }

  //----------------------------------------------------------------

private:

  // ---- splat cloud ----

  /// reference to class containing all the data
  const SplatCloud &splatCloud_;

  // ---- modification tags ----

  /// marks if parts of the data has been modified
  bool positionsModified_;
  bool colorsModified_;
  bool normalsModified_;
  bool pointsizesModified_;
  bool selectionsModified_;
  bool pickColorsModified_;

  /// return true iff any of the data values in the VBO has to be changed
  inline bool vboModified() const
  {
    return positionsModified_  ||
           colorsModified_     ||
           normalsModified_    ||
           pointsizesModified_ ||
           selectionsModified_ ||
           pickColorsModified_;
  }

  // ---- default values ----

  /// the default values will be used when the specific array is not present
  Color     defaultColor_;
  Normal    defaultNormal_;
  Pointsize defaultPointsize_;

  // ---- draw modes ----

  DrawModes::DrawMode splatsDrawMode_;
  DrawModes::DrawMode dotsDrawMode_;
  DrawModes::DrawMode pointsDrawMode_;

  // ---- picking ----

  unsigned int pickingBaseIndex_;

  // TODO: hack, see enterPick()
  DrawModes::DrawMode pickDrawMode_;

  // ---- vertex buffer object ----

  GLuint        vboGlId_;
  unsigned int  vboNumSplats_;
  unsigned char *vboData_;
  unsigned int  vboStride_;

  /// offsets relative to vboData_ or -1 if *not* present in VBO
  int vboPositionsOffset_;
  int vboColorsOffset_;
  int vboNormalsOffset_;
  int vboPointsizesOffset_;
  int vboSelectionsOffset_;
  int vboPickColorsOffset_;

  ACG::VertexDeclaration vboDecl_;

  /// returns true iff the internal block structure of the VBO has to be changed
  inline bool vboStructureModified() const
  {
    return vboNumSplats_                != splatCloud_.numSplats()     ||
           (vboPositionsOffset_  != -1) != splatCloud_.hasPositions()  ||
           (vboColorsOffset_     != -1) != splatCloud_.hasColors()     ||
           (vboNormalsOffset_    != -1) != splatCloud_.hasNormals()    ||
           (vboPointsizesOffset_ != -1) != splatCloud_.hasPointsizes() ||
           (vboSelectionsOffset_ != -1) != splatCloud_.hasSelections();
  }

  void createVBO();
  void destroyVBO();
  void rebuildVBO( GLState &_state );

  void rebuildVBOPositions();
  void rebuildVBOColors();
  void rebuildVBONormals();
  void rebuildVBOPointsizes();
  void rebuildVBOSelections();
  void rebuildVBOPickColors( GLState &_state );

  static const Position  DEFAULT_POSITION;
  static const Index     DEFAULT_INDEX;
  static const Viewlist  DEFAULT_VIEWLIST;
  static const Selection DEFAULT_SELECTION;

  // ---- rendering params ----

  float pointsizeScale_;
  bool  backfaceCulling_;
};


//================================================================


} // namespace SceneGraph
} // namespace ACG


//================================================================


#endif // ACG_SPLATCLOUDNODE_HH
