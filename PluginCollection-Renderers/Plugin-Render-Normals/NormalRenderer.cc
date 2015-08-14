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

#include "NormalRenderer.hh"

#include <OpenFlipper/common/GlobalOptions.hh>
#include <OpenFlipper/BasePlugin/PluginFunctions.hh>
#include <ACG/GL/ShaderCache.hh>


#if QT_VERSION >= 0x050000
 #include <QOpenGLContext>
 #include <QSurfaceFormat>
#else
 #include <QGLFormat>
#endif


// =================================================

class NormalFragmentModifier : public ACG::ShaderModifier
{
public:

  void modifyVertexIO( ACG::ShaderGenerator* _shader )
  {
    // request object space normals
    _shader->addDefine("SG_REQUEST_NORMALOS");
  }

  void modifyFragmentEndCode(QStringList* _code)
  {
    _code->push_back("#ifdef SG_INPUT_NORMALOS");
    _code->push_back("outFragment = vec4(normalize(SG_INPUT_NORMALOS) * 0.5 + vec3(0.5, 0.5, 0.5),1.0);");
    _code->push_back("#endif ");
  }

  static NormalFragmentModifier instance;
};


NormalFragmentModifier NormalFragmentModifier::instance;

// =================================================

NormalRenderer::NormalRenderer()
{
  ACG::ShaderProgGenerator::registerModifier(&NormalFragmentModifier::instance);
}


NormalRenderer::~NormalRenderer()
{
}


void NormalRenderer::initializePlugin()
{
  ACG::ShaderProgGenerator::setShaderDir(OpenFlipper::Options::shaderDirStr());
}

QString NormalRenderer::renderObjectsInfo(bool _outputShaderInfo) {
  std::vector<ACG::ShaderModifier*> modifiers;
  modifiers.push_back(&NormalFragmentModifier::instance);
  return dumpCurrentRenderObjectsToString(&sortedObjects_[0], _outputShaderInfo, &modifiers);
}

void NormalRenderer::render(ACG::GLState* _glState, Viewer::ViewerProperties& _properties)
{
  // collect renderobjects + prepare OpenGL state
  prepareRenderingPipeline(_glState, _properties.drawMode(), PluginFunctions::getSceneGraphRootNode());

  // render every object
  for (int i = 0; i < getNumRenderObjects(); ++i) {

    // Take original shader and modify the output to take only the normal as the color
    GLSL::Program* prog = ACG::ShaderCache::getInstance()->getProgram(&sortedObjects_[i]->shaderDesc, NormalFragmentModifier::instance);
    renderObject(sortedObjects_[i],prog);
  }

  // restore common opengl state
  // log window remains hidden otherwise
  finishRenderingPipeline();
}

QString NormalRenderer::checkOpenGL()
{



#if QT_VERSION < 0x050000

  // Get version and check
  QGLFormat::OpenGLVersionFlags flags = QGLFormat::openGLVersionFlags();
  if ( !flags.testFlag(QGLFormat::OpenGL_Version_3_2) )
    return QString("Insufficient OpenGL Version! OpenGL 3.2 or higher required");

  // Check extensions
  QString glExtensions = QString((const char*)glGetString(GL_EXTENSIONS));
  QString missing("");
  if ( !glExtensions.contains("GL_ARB_vertex_buffer_object") )
    missing += "GL_ARB_vertex_buffer_object extension missing\n";

  #ifndef __APPLE__
    if ( !glExtensions.contains("GL_ARB_vertex_program") )
      missing += "GL_ARB_vertex_program extension missing\n";
  #endif

  return missing;

#else
  QOpenGLContext* context = QOpenGLContext::currentContext();
  if ( context ) {

    // Get version and check
    QSurfaceFormat format = context->format();

    if ( (format.majorVersion() < 4) && (format.minorVersion() < 2) ) {
      return QString("Insufficient OpenGL Version! OpenGL 3.2 or higher required");
    }

    // Check extensions
    QString missing("");

    if ( !context->hasExtension("GL_ARB_vertex_buffer_object") )
      missing += "GL_ARB_vertex_buffer_object extension missing\n";

    #ifndef __APPLE__
      if ( !context->hasExtension("GL_ARB_vertex_program") )
        missing += "GL_ARB_vertex_program extension missing\n";
    #endif

    return missing;
  } else {
    return name() + QString(": No context available");
  }

#endif

}

#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2( normalrenderer , NormalRenderer );
#endif


