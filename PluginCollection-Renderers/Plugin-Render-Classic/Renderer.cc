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
*   $Revision: 21016 $                                                       *
*   $LastChangedBy: schultz $                                                *
*   $Date: 2015-07-16 16:48:42 +0200 (Do, 16 Jul 2015) $                     *
*                                                                            *
\*===========================================================================*/

#include <cstdio>
#include <cstring>
#include <iostream>
#include <cstdlib>

#include <ACG/GL/acg_glew.hh>
#include <ACG/GL/gl.hh>

#include "Renderer.hh"

#include <OpenFlipper/common/ViewObjectMarker.hh>

using namespace ACG;

// =================================================

Renderer::Renderer()
{
}


Renderer::~Renderer()
{
}


void Renderer::initializePlugin()
{
}


void Renderer::render(ACG::GLState* _glState, Viewer::ViewerProperties& _properties)
{
  ACG::SceneGraph::BaseNode* root = PluginFunctions::getSceneGraphRootNode();

  if (root) {
    ViewObjectMarker *oM = _properties.objectMarker();
    GLuint refBits = 0;
    QSet<GLuint> references;

    if (oM)
    {
      glClear (GL_STENCIL_BUFFER_BIT);
      ACG::GLState::enable (GL_STENCIL_TEST);
      glStencilOp (GL_KEEP, GL_KEEP, GL_ZERO);
      glStencilFunc (GL_ALWAYS, 0, ~0);

      for (PluginFunctions::ObjectIterator o_it(PluginFunctions::ALL_OBJECTS, DATA_ALL) ;
          o_it != PluginFunctions::objectsEnd(); ++o_it)
      {
        bool ok;
        GLuint ref;

        ok = oM->stencilRefForObject(*o_it, ref);

        if (ok)
        {
          o_it->stencilRefNode ()->setReference (ref);
          o_it->stencilRefNode ()->show ();
          refBits |= ref;
          references << ref;
        }
        else
          o_it->stencilRefNode ()->hide ();
      }
    }

    ACG::SceneGraph::DrawAction action( _properties.drawMode(), *_glState , false);
    ACG::SceneGraph::traverse_multipass(root, action, *_glState, _properties.drawMode() );

    // Second pass for blending
    ACG::SceneGraph::DrawAction action1(_properties.drawMode(), *_glState, true);
    ACG::SceneGraph::traverse_multipass(root, action1, *_glState, _properties.drawMode());

    if (oM)
    {
      if (oM->type() == ViewObjectMarker::PerBit)
      {
        references.clear ();
        for (unsigned int i = 0; i < sizeof (GLuint) * 8; i++)
          if (refBits & (1 << i))
            references << (1 << i);
      }

      glPushAttrib(GL_ALL_ATTRIB_BITS);

      ACG::GLState::enable(GL_BLEND);
      ACG::GLState::disable(GL_DEPTH_TEST);
      ACG::GLState::disable(GL_LIGHTING);
      ACG::GLState::disable(GL_DITHER);

      int vp_l, vp_b, vp_w, vp_h;
      _glState->get_viewport (vp_l, vp_b, vp_w, vp_h);

      glMatrixMode(GL_PROJECTION);
      glPushMatrix ();
      glLoadIdentity();
      glOrtho(0, vp_w, vp_h, 0, 0, 1.0);
      glMatrixMode(GL_MODELVIEW);
      glPushMatrix ();
      glLoadIdentity();

      glStencilOp (GL_KEEP, GL_KEEP, GL_KEEP);

      foreach (unsigned int ref, references)
      {
        bool ok;
        GLenum sfactor;
        GLenum dfactor;
        ACG::Vec4f color;
        unsigned int mask = ~0;

        if (oM->type() == ViewObjectMarker::PerBit)
        {
          ok = oM->blendForStencilRefBit (ref, sfactor, dfactor, color);
          mask = ref;
        }
        else
          ok = oM->blendForStencilRefNumber (ref, sfactor, dfactor, color);

        if (!ok)
          continue;

        glStencilFunc (GL_EQUAL, ref, mask);

        ACG::GLState::blendFunc (sfactor, dfactor);
        glColor4f (color[0], color [1], color [2], color[3]);

        glBegin (GL_QUADS);
        glVertex2i(0, 0);
        glVertex2i(0, vp_h);
        glVertex2i(vp_w, vp_h);
        glVertex2i(vp_w, 0);
        glEnd ();

      }

      glMatrixMode(GL_PROJECTION);
      glPopMatrix ();
      glMatrixMode(GL_MODELVIEW);
      glPopMatrix ();

      glPopAttrib ();
      ACG::GLState::disable (GL_STENCIL_TEST);
    }


  }
}

QString Renderer::checkOpenGL()
{
  // This renderer plugin should run on almost any old style hardware
  return QString("");
}


#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2( classicalrenderer , Renderer );
#endif


