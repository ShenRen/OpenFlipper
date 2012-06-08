/*===========================================================================*\
 *                                                                           *
 *                              OpenFlipper                                  *
 *      Copyright (C) 2001-2011 by Computer Graphics Group, RWTH Aachen      *
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
 *   $Revision$                                                       *
 *   $Author$                                                      *
 *   $Date$                   *
 *                                                                           *
\*===========================================================================*/

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>

#include <ACG/GL/gl.hh>

#include <ACG/GL/IRenderer.hh>

#include <ACG/GL/VertexDeclaration.hh>
#include <ACG/GL/GLState.hh>

void ACG::RenderObject::initFromState( GLState* _glState )
{
//  memset(this, 0, sizeof(RenderObject)); <- bad

  culling = true;
  blending = false;
  depthTest = true;
  depthWrite = true;
  alphaTest = false;

  depthRange = Vec2f(0.0f, 1.0f);
  depthFunc = GL_LESS;

  blendSrc = GL_ONE;
  blendDest = GL_ZERO;

  if (_glState)
  {
    modelview = _glState->modelview();
    proj = _glState->projection();

    culling =_glState->isStateEnabled(GL_CULL_FACE);
    blending = _glState->isStateEnabled(GL_BLEND);
    depthTest = _glState->isStateEnabled(GL_DEPTH_TEST);

//    shadeModel = _glState->getShadeModel();

    _glState->getBlendFunc(&blendSrc, &blendDest);

    GLclampd zn, zf;
    _glState->getDepthRange(&zn, &zf);
    depthRange = Vec2f(zn, zf);

    depthFunc = _glState->depthFunc();


    alphaTest = _glState->isStateEnabled(GL_ALPHA_TEST);

    for (int i = 0; i < 3; ++i)
    {
      diffuse[i] = _glState->diffuse_color()[i];
      ambient[i] = _glState->ambient_color()[i];
      specular[i] = _glState->specular_color()[i];
      emissive[i] = _glState->base_color()[i];
    }
    shininess = _glState->shininess();
  }
}