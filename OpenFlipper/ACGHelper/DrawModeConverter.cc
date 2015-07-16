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



//=============================================================================
//
//
//=============================================================================


//== INCLUDES =================================================================

#include "DrawModeConverter.hh"

//== TYPEDEFS =================================================================

//== CLASS DEFINITION =========================================================

/// Convert a drawmode to a vector of QStrings
std::vector< QString > drawModeToList(ACG::SceneGraph::DrawModes::DrawMode _drawMode) {
  std::vector< QString > draw_modes;
  
  if ( _drawMode & ACG::SceneGraph::DrawModes::DEFAULT )
    draw_modes.push_back("DEFAULT");
  if ( _drawMode & ACG::SceneGraph::DrawModes::SOLID_SMOOTH_SHADED)
    draw_modes.push_back("SOLID_SMOOTH_SHADED");
  if ( _drawMode & ACG::SceneGraph::DrawModes::HIDDENLINE)
    draw_modes.push_back("HIDDENLINE");
  if ( _drawMode & ACG::SceneGraph::DrawModes::WIREFRAME)
    draw_modes.push_back("WIREFRAME");
  if ( _drawMode & ACG::SceneGraph::DrawModes::POINTS)
    draw_modes.push_back("POINTS");
  if ( _drawMode & ACG::SceneGraph::DrawModes::POINTS_COLORED)
    draw_modes.push_back("POINTS_COLORED");
  if ( _drawMode & ACG::SceneGraph::DrawModes::POINTS_SHADED)
    draw_modes.push_back("POINTS_SHADED");
  if ( _drawMode & ACG::SceneGraph::DrawModes::SOLID_FLAT_SHADED) 
    draw_modes.push_back("SOLID_FLAT_SHADED");
  if ( _drawMode & ACG::SceneGraph::DrawModes::SOLID_PHONG_SHADED)
    draw_modes.push_back("SOLID_PHONG_SHADED");
  if ( _drawMode & ACG::SceneGraph::DrawModes::SOLID_FACES_COLORED)
    draw_modes.push_back("SOLID_FACES_COLORED");
  if ( _drawMode & ACG::SceneGraph::DrawModes::SOLID_POINTS_COLORED)
    draw_modes.push_back("SOLID_POINTS_COLORED");
  if ( _drawMode & ACG::SceneGraph::DrawModes::SOLID_POINTS_COLORED_SHADED)
    draw_modes.push_back("SOLID_POINTS_COLORED_SHADED");
  if ( _drawMode & ACG::SceneGraph::DrawModes::SOLID_ENV_MAPPED)
    draw_modes.push_back("SOLID_ENV_MAPPED");
  if ( _drawMode & ACG::SceneGraph::DrawModes::SOLID_TEXTURED)
    draw_modes.push_back("SOLID_TEXTURED");    
  if ( _drawMode & ACG::SceneGraph::DrawModes::SOLID_TEXTURED_SHADED)
    draw_modes.push_back("SOLID_TEXTURED_SHADED");
  if ( _drawMode & ACG::SceneGraph::DrawModes::SOLID_1DTEXTURED)
    draw_modes.push_back("SOLID_1DTEXTURED");        
  if ( _drawMode & ACG::SceneGraph::DrawModes::SOLID_1DTEXTURED_SHADED)
    draw_modes.push_back("SOLID_1DTEXTURED_SHADED");     
  if ( _drawMode & ACG::SceneGraph::DrawModes::SOLID_3DTEXTURED)
    draw_modes.push_back("SOLID_3DTEXTURED");             
  if ( _drawMode & ACG::SceneGraph::DrawModes::SOLID_3DTEXTURED_SHADED)
    draw_modes.push_back("SOLID_3DTEXTURED_SHADED");    
  if ( _drawMode & ACG::SceneGraph::DrawModes::SOLID_FACES_COLORED_FLAT_SHADED)
    draw_modes.push_back("SOLID_FACES_COLORED_FLAT_SHADED");
  if ( _drawMode & ACG::SceneGraph::DrawModes::SOLID_FACES_COLORED_SMOOTH_SHADED)
    draw_modes.push_back("SOLID_FACES_COLORED_SMOOTH_SHADED");
  if (_drawMode & ACG::SceneGraph::DrawModes::SOLID_FACES_COLORED_2DTEXTURED_FACE_SMOOTH_SHADED)
    draw_modes.push_back("SOLID_FACES_COLORED_2DTEXTURED_FACE_SMOOTH_SHADED");
  if ( _drawMode & ACG::SceneGraph::DrawModes::SOLID_2DTEXTURED_FACE)
    draw_modes.push_back("SOLID_2DTEXTURED_FACE");    
  if ( _drawMode & ACG::SceneGraph::DrawModes::SOLID_2DTEXTURED_FACE_SHADED)
    draw_modes.push_back("SOLID_2DTEXTURED_FACE_SHADED");    
  if ( _drawMode & ACG::SceneGraph::DrawModes::SOLID_SHADER)
    draw_modes.push_back("SOLID_SHADER");    
  if ( _drawMode & ACG::SceneGraph::DrawModes::SOLID_SMOOTH_SHADED_FEATURES)
    draw_modes.push_back("SOLID_SMOOTH_SHADED_FEATURES");

  return draw_modes;
}

/// Convert a vector of QStrings to a draw mode
ACG::SceneGraph::DrawModes::DrawMode listToDrawMode( std::vector< QString > _draw_modes ) {
    
  ACG::SceneGraph::DrawModes::DrawMode drawMode( ACG::SceneGraph::DrawModes::NONE );
  
  for ( uint i = 0 ; i < _draw_modes.size() ; ++i ) {
    if( _draw_modes[i] == "DEFAULT" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::DEFAULT;
    } else  if( _draw_modes[i] == "SOLID_SMOOTH_SHADED" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::SOLID_SMOOTH_SHADED;
    } else if( _draw_modes[i] == "HIDDENLINE" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::HIDDENLINE;
    } else if( _draw_modes[i] == "WIREFRAME" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::WIREFRAME;
    } else if( _draw_modes[i] == "POINTS" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::POINTS;
    } else if( _draw_modes[i] == "POINTS_COLORED" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::POINTS_COLORED;
    } else if( _draw_modes[i] == "POINTS_SHADED" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::POINTS_SHADED;
    } else if( _draw_modes[i] == "SOLID_FLAT_SHADED" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::SOLID_FLAT_SHADED;
    } else if( _draw_modes[i] == "SOLID_PHONG_SHADED" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::SOLID_PHONG_SHADED;
    } else if( _draw_modes[i] == "SOLID_FACES_COLORED" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::SOLID_FACES_COLORED;
    } else if( _draw_modes[i] == "SOLID_POINTS_COLORED" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::SOLID_POINTS_COLORED;
    } else if( _draw_modes[i] == "SOLID_POINTS_COLORED_SHADED" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::SOLID_POINTS_COLORED_SHADED;
    } else if( _draw_modes[i] == "SOLID_ENV_MAPPED" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::SOLID_ENV_MAPPED;
    } else if( _draw_modes[i] == "SOLID_TEXTURED" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::SOLID_TEXTURED;
    } else if( _draw_modes[i] == "SOLID_TEXTURED_SHADED" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::SOLID_TEXTURED_SHADED;
    } else if( _draw_modes[i] == "SOLID_1DTEXTURED" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::SOLID_1DTEXTURED;
    } else if( _draw_modes[i] == "SOLID_1DTEXTURED_SHADED" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::SOLID_1DTEXTURED_SHADED;
    } else if( _draw_modes[i] == "SOLID_3DTEXTURED" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::SOLID_3DTEXTURED;
    } else if( _draw_modes[i] == "SOLID_3DTEXTURED_SHADED" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::SOLID_3DTEXTURED_SHADED;
    } else if( _draw_modes[i] == "SOLID_FACES_COLORED_FLAT_SHADED" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::SOLID_FACES_COLORED_FLAT_SHADED;
    } else if( _draw_modes[i] == "SOLID_FACES_COLORED_SMOOTH_SHADED" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::SOLID_FACES_COLORED_SMOOTH_SHADED;
    } else if (_draw_modes[i] == "SOLID_FACES_COLORED_2DTEXTURED_FACE_SMOOTH_SHADED") {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::SOLID_FACES_COLORED_2DTEXTURED_FACE_SMOOTH_SHADED;
    } else if (_draw_modes[i] == "SOLID_2DTEXTURED_FACE") {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::SOLID_2DTEXTURED_FACE;
    } else if( _draw_modes[i] == "SOLID_2DTEXTURED_FACE_SHADED" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::SOLID_2DTEXTURED_FACE_SHADED;
    } else if( _draw_modes[i] == "SOLID_SHADER" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::SOLID_SHADER;
    } else if( _draw_modes[i] == "SOLID_SMOOTH_SHADED_FEATURES" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::SOLID_SMOOTH_SHADED_FEATURES;
    }
  }
  
  return drawMode; 
 
}

/// Convert a vector of DrawMode-Descriptions to a draw mode
ACG::SceneGraph::DrawModes::DrawMode descriptionsToDrawMode ( std::vector< QString > _draw_modes ) {
    
  ACG::SceneGraph::DrawModes::DrawMode drawMode( ACG::SceneGraph::DrawModes::NONE );
  
  for ( uint i = 0 ; i < _draw_modes.size() ; ++i ) {
    if( _draw_modes[i] == "Default" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::DEFAULT;
    } else  if( _draw_modes[i] == "Solid (smooth shaded)" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::SOLID_SMOOTH_SHADED;
    } else if( _draw_modes[i] == "Hiddenline" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::HIDDENLINE;
    } else if( _draw_modes[i] == "Wireframe" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::WIREFRAME;
    } else if( _draw_modes[i] == "Points" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::POINTS;
    } else if( _draw_modes[i] == "Points (colored)" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::POINTS_COLORED;
    } else if( _draw_modes[i] == "Points (shaded)" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::POINTS_SHADED;
    } else if( _draw_modes[i] == "Solid (flat shaded)" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::SOLID_FLAT_SHADED;
    } else if( _draw_modes[i] == "Solid (Phong shaded)" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::SOLID_PHONG_SHADED;
    } else if( _draw_modes[i] == "Solid (colored per-face)" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::SOLID_FACES_COLORED;
    } else if( _draw_modes[i] == "Solid (colored per-vertex)" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::SOLID_POINTS_COLORED;
    } else if( _draw_modes[i] == "Solid (colored per-vertex, shaded)" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::SOLID_POINTS_COLORED_SHADED;
    } else if( _draw_modes[i] == "Solid (environment mapped)" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::SOLID_ENV_MAPPED;
    } else if( _draw_modes[i] == "Solid (textured)" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::SOLID_TEXTURED;
    } else if( _draw_modes[i] == "Solid (textured, shaded)" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::SOLID_TEXTURED_SHADED;
    } else if( _draw_modes[i] == "Solid (scalar field)" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::SOLID_1DTEXTURED;
    } else if( _draw_modes[i] == "Solid (scalar field, shaded)" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::SOLID_1DTEXTURED_SHADED;
    } else if( _draw_modes[i] == "Solid (3D textured)" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::SOLID_3DTEXTURED;
    } else if( _draw_modes[i] == "Solid (3D textured, shaded)" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::SOLID_3DTEXTURED_SHADED;
    } else if( _draw_modes[i] == "Solid (colored per-face, flat shaded)" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::SOLID_FACES_COLORED_FLAT_SHADED;
    } else if( _draw_modes[i] == "Solid (colored per-face, smooth shaded)" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::SOLID_FACES_COLORED_SMOOTH_SHADED;
    } else if( _draw_modes[i] == "Solid (colored per-face, face textured, smooth shaded)" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::SOLID_FACES_COLORED_2DTEXTURED_FACE_SMOOTH_SHADED;
    } else if( _draw_modes[i] == "Solid (face textured)" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::SOLID_2DTEXTURED_FACE;
    } else if( _draw_modes[i] == "Solid (face textured, shaded)" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::SOLID_2DTEXTURED_FACE_SHADED;
    } else if( _draw_modes[i] == "Shader controlled" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::SOLID_SHADER;
    } else if( _draw_modes[i] == "Solid (smooth shaded, features)" ) {
      drawMode = drawMode | ACG::SceneGraph::DrawModes::SOLID_SMOOTH_SHADED_FEATURES;
    }
  }
  
  return drawMode; 
}

/// Convert a drawmode to a vector of QStrings
std::vector< QString > drawModeToDescriptions(ACG::SceneGraph::DrawModes::DrawMode _drawMode) {
  std::vector< QString > draw_modes;
  
  if ( _drawMode & ACG::SceneGraph::DrawModes::DEFAULT )
    draw_modes.push_back("Default");
  if ( _drawMode & ACG::SceneGraph::DrawModes::SOLID_SMOOTH_SHADED)
    draw_modes.push_back("Solid (smooth shaded)");
  if ( _drawMode & ACG::SceneGraph::DrawModes::HIDDENLINE)
    draw_modes.push_back("Hiddenline");
  if ( _drawMode & ACG::SceneGraph::DrawModes::WIREFRAME)
    draw_modes.push_back("Wireframe");
  if ( _drawMode & ACG::SceneGraph::DrawModes::POINTS)
    draw_modes.push_back("Points");
  if ( _drawMode & ACG::SceneGraph::DrawModes::POINTS_COLORED)
    draw_modes.push_back("Points (colored)");
  if ( _drawMode & ACG::SceneGraph::DrawModes::POINTS_SHADED)
    draw_modes.push_back("Points (shaded)");
  if ( _drawMode & ACG::SceneGraph::DrawModes::SOLID_FLAT_SHADED) 
    draw_modes.push_back("Solid (flat shaded)");
  if ( _drawMode & ACG::SceneGraph::DrawModes::SOLID_PHONG_SHADED)
    draw_modes.push_back("Solid (Phong shaded)");
  if ( _drawMode & ACG::SceneGraph::DrawModes::SOLID_FACES_COLORED)
    draw_modes.push_back("Solid (colored per-face)");
  if ( _drawMode & ACG::SceneGraph::DrawModes::SOLID_POINTS_COLORED)
    draw_modes.push_back("Solid (colored per-vertex)");
  if ( _drawMode & ACG::SceneGraph::DrawModes::SOLID_POINTS_COLORED_SHADED)
    draw_modes.push_back("Solid (colored per-vertex, shaded)");
  if ( _drawMode & ACG::SceneGraph::DrawModes::SOLID_ENV_MAPPED)
    draw_modes.push_back("Solid (environment mapped)");
  if ( _drawMode & ACG::SceneGraph::DrawModes::SOLID_TEXTURED)
    draw_modes.push_back("Solid (textured)");    
  if ( _drawMode & ACG::SceneGraph::DrawModes::SOLID_TEXTURED_SHADED)
    draw_modes.push_back("Solid (textured, shaded)");
  if ( _drawMode & ACG::SceneGraph::DrawModes::SOLID_1DTEXTURED)
    draw_modes.push_back("Solid (scalar field)");        
  if ( _drawMode & ACG::SceneGraph::DrawModes::SOLID_1DTEXTURED_SHADED)
    draw_modes.push_back("Solid (scalar field, shaded)");     
  if ( _drawMode & ACG::SceneGraph::DrawModes::SOLID_3DTEXTURED)
    draw_modes.push_back("Solid (3D textured)");             
  if ( _drawMode & ACG::SceneGraph::DrawModes::SOLID_3DTEXTURED_SHADED)
    draw_modes.push_back("Solid (3D textured, shaded)");    
  if ( _drawMode & ACG::SceneGraph::DrawModes::SOLID_FACES_COLORED_FLAT_SHADED)
    draw_modes.push_back("Solid (colored per-face, flat shaded)");    
  if ( _drawMode & ACG::SceneGraph::DrawModes::SOLID_FACES_COLORED_SMOOTH_SHADED)
    draw_modes.push_back("Solid (colored per-face, smooth shaded)");
  if (_drawMode & ACG::SceneGraph::DrawModes::SOLID_FACES_COLORED_2DTEXTURED_FACE_SMOOTH_SHADED)
    draw_modes.push_back("Solid (colored per-face, face textured, smooth shaded)");
  if ( _drawMode & ACG::SceneGraph::DrawModes::SOLID_2DTEXTURED_FACE)
    draw_modes.push_back("Solid (face textured)");    
  if ( _drawMode & ACG::SceneGraph::DrawModes::SOLID_2DTEXTURED_FACE_SHADED)
    draw_modes.push_back("Solid (face textured, shaded)");    
  if ( _drawMode & ACG::SceneGraph::DrawModes::SOLID_SHADER)
    draw_modes.push_back("Shader controlled");    
  if ( _drawMode & ACG::SceneGraph::DrawModes::SOLID_SMOOTH_SHADED_FEATURES)
    draw_modes.push_back("Solid (smooth shaded, features)");

  return draw_modes;
}
//=============================================================================

