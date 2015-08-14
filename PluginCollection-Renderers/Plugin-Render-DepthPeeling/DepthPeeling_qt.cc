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
*   $Revision: 18127 $                                                       *
*   $LastChangedBy: moebius $                                                *
*   $Date: 2014-02-05 10:12:54 +0100 (Wed, 05 Feb 2014) $                     *
*                                                                            *
\*===========================================================================*/

/** @file
 *
 *  Contains definitions of the DepthPeeling Renderer that require qt headers
 *  which are incompatible with glew.h.
 */

#include "DepthPeeling.hh"

#include <QAction>
#include <QMenu>

#if QT_VERSION >= 0x050000
 #include <QOpenGLContext>
 #include <QSurfaceFormat>
#else
 #include <QGLFormat>
#endif


QString DepthPeeling::checkOpenGL()
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

  if ( !glExtensions.contains("GL_ARB_occlusion_query") )
    missing += "GL_ARB_occlusion_query extension missing\n";

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

    if ( !context->hasExtension("GL_ARB_occlusion_query") )
       missing += "GL_ARB_occlusion_query extension missing\n";

      return missing;
  } else {
    return name() + QString(": No context available");
  }

#endif
}

QAction* DepthPeeling::optionsAction()
{
  QMenu* menu = new QMenu("Depth Peeling Mode");

  // Recreate actionGroup
  QActionGroup* modeGroup = new QActionGroup( this );
  modeGroup->setExclusive( true );

  QAction * action = new QAction("Front to Back" , modeGroup );
  action->setCheckable( true );

  action = new QAction("Dual" , modeGroup );
  action->setCheckable( true );
  action->setChecked(true);


  menu->addActions(modeGroup->actions());

  connect(modeGroup,SIGNAL(triggered( QAction * )),this,SLOT(slotModeChanged( QAction * )));

  return menu->menuAction();
}
