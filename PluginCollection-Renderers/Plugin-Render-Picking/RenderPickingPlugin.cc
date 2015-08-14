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

#include "RenderPickingPlugin.hh"

#include <iostream>
#include <ACG/GL/GLState.hh>

#include <OpenFlipper/BasePlugin/PluginFunctions.hh>
#include <OpenFlipper/common/GlobalOptions.hh>

#include <QGLFormat>
#include <QMenu>

#if QT_VERSION >= 0x050000 
#include <QtWidgets>
#else
#include <QtGui>
#endif

#if QT_VERSION >= 0x050000
 #include <QOpenGLContext>
 #include <QSurfaceFormat>
#else
 #include <QGLFormat>
#endif


void RenderPickingPlugin::initializePlugin()
{
  pickRendererMode_ = ACG::SceneGraph::PICK_ANYTHING;
}

QAction* RenderPickingPlugin::optionsAction() {

  QMenu* menu = new QMenu("Picking Renderer Target");

  // Recreate actionGroup
  QActionGroup* pickingTargetsGroup = new QActionGroup( this );
  pickingTargetsGroup->setExclusive( true );

  // Always set PickAnything ( will be overridden by others)
  QAction * action = new QAction("PICK_ANYTHING" , pickingTargetsGroup );
  action->setCheckable( true );
  action->setChecked(true);

  action = new QAction("PICK_VERTEX" , pickingTargetsGroup );
  action->setCheckable( true );
  if (pickRendererMode_ == ACG::SceneGraph::PICK_VERTEX)
    action->setChecked(true);

  action = new QAction("PICK_EDGE" , pickingTargetsGroup );
  action->setCheckable( true );
  if (pickRendererMode_ == ACG::SceneGraph::PICK_EDGE)
    action->setChecked(true);

  action = new QAction("PICK_SPLINE" , pickingTargetsGroup );
  action->setCheckable( true );
  if (pickRendererMode_ == ACG::SceneGraph::PICK_SPLINE)
    action->setChecked(true);

  action = new QAction("PICK_FACE" , pickingTargetsGroup );
  action->setCheckable( true );
  if (pickRendererMode_ == ACG::SceneGraph::PICK_FACE)
    action->setChecked(true);

  action = new QAction("PICK_FRONT_VERTEX" , pickingTargetsGroup );
  action->setCheckable( true );
  if (pickRendererMode_ == ACG::SceneGraph::PICK_FRONT_VERTEX)
    action->setChecked(true);

  action = new QAction("PICK_FRONT_EDGE" , pickingTargetsGroup );
  action->setCheckable( true );
  if (pickRendererMode_ == ACG::SceneGraph::PICK_FRONT_EDGE)
    action->setChecked(true);

  action = new QAction("PICK_CELL" , pickingTargetsGroup );
  action->setCheckable( true );
  if (pickRendererMode_ == ACG::SceneGraph::PICK_CELL)
    action->setChecked(true);

  menu->addActions(pickingTargetsGroup->actions());

  connect(pickingTargetsGroup,SIGNAL(triggered( QAction * )),this,SLOT(slotPickTargetChanged( QAction * )));

  return menu->menuAction();
}

void RenderPickingPlugin::slotPickTargetChanged( QAction *  _action) {

  // Prepare Picking Debugger Flag
  if ( _action->text() == "PICK_ANYTHING") {
    pickRendererMode_ = ACG::SceneGraph::PICK_ANYTHING;
  } else if ( _action->text() == "PICK_VERTEX") {
    pickRendererMode_ = ACG::SceneGraph::PICK_VERTEX;
  } else if ( _action->text() == "PICK_EDGE") {
    pickRendererMode_ = ACG::SceneGraph::PICK_EDGE;
  } else if ( _action->text() == "PICK_SPLINE") {
    pickRendererMode_ = ACG::SceneGraph::PICK_SPLINE;
  } else if ( _action->text() == "PICK_FACE") {
    pickRendererMode_ = ACG::SceneGraph::PICK_FACE;
  } else if ( _action->text() == "PICK_FRONT_VERTEX") {
    pickRendererMode_ = ACG::SceneGraph::PICK_FRONT_VERTEX;
  } else if ( _action->text() == "PICK_FRONT_EDGE") {
    pickRendererMode_ = ACG::SceneGraph::PICK_FRONT_EDGE;
  } else if ( _action->text() == "PICK_CELL") {
    pickRendererMode_ = ACG::SceneGraph::PICK_CELL;
  } else {
    std::cerr << "Error : optionHandling unable to find pick mode!!! " << _action->text().toStdString() << std::endl;
    pickRendererMode_ = ACG::SceneGraph::PICK_ANYTHING;
  }

}

QString RenderPickingPlugin::rendererName() {
  return QString("Picking renderer");
}

void RenderPickingPlugin::supportedDrawModes(ACG::SceneGraph::DrawModes::DrawMode& _mode) {
  _mode =  ACG::SceneGraph::DrawModes::DEFAULT;
}


void RenderPickingPlugin::render(ACG::GLState* _glState, Viewer::ViewerProperties& _properties) {

  ACG::GLState::disable(GL_LIGHTING);
  ACG::GLState::disable(GL_BLEND);
  glClear(GL_DEPTH_BUFFER_BIT);

  // do the picking
  _glState->pick_init (true);
  ACG::SceneGraph::PickAction action(*_glState, pickRendererMode_, _properties.drawMode());
  ACG::SceneGraph::traverse_multipass( PluginFunctions::getSceneGraphRootNode() , action,*_glState);

  ACG::GLState::enable(GL_LIGHTING);
  ACG::GLState::enable(GL_BLEND);

}

QString RenderPickingPlugin::checkOpenGL() {

#if QT_VERSION < 0x050000

  // Get version and check
  QGLFormat::OpenGLVersionFlags flags = QGLFormat::openGLVersionFlags();
  if ( ! flags.testFlag(QGLFormat::OpenGL_Version_2_0) )
    return QString("Insufficient OpenGL Version! OpenGL 2.0 or higher required");

  //Get OpenGL extensions
  QString glExtensions = QString((const char*)glGetString(GL_EXTENSIONS));

  // Collect missing extension
  QString missing = "";

  return missing;

#else
  QOpenGLContext* context = QOpenGLContext::currentContext();
  if ( context ) {

    // Get version and check
    QSurfaceFormat format = context->format();

    if ( (format.majorVersion() < 2) ) {
      return QString("Insufficient OpenGL Version! OpenGL 2.0 or higher required");
    }

    // Check extensions
    QString missing("");

    return missing;
  } else {
    return name() + QString(": No context available");
  }

#endif

}


#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2( renderpickingplugin , RenderPickingPlugin );
#endif

