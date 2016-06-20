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
//  CLASS SimpleViewer - IMPLEMENTATION
//
//=============================================================================

//== INCLUDES =================================================================

#include <OpenFlipper/common/GlobalOptions.hh>
#include <OpenFlipper/BasePlugin/PluginFunctions.hh>
#include <OpenFlipper/widgets/glWidget/QtBaseViewer.hh>
#include <ACG/Scenegraph/SceneGraphAnalysis.hh>
#include "simpleGLGraphicsScene.hh"


#include "simpleViewer.hh"

#if (QT_VERSION >= QT_VERSION_CHECK(5,4,0))
#include <QOpenGLWidget>
#endif

//== NAMESPACES ===============================================================


//== CLASS DEFINITION =========================================================


SimpleViewer::SimpleViewer(QWidget* _parent, bool useDefaultSceneGraph) :
  QGraphicsView (_parent),
  props_(-1),
  actionMode_ (Viewer::ExamineMode)
{
  OFGLWidget *share = PluginFunctions::shareGLWidget (); 
  if (!share)
    initialize (OFGLFormat::defaultFormat (), 0, useDefaultSceneGraph);
  else
    initialize (share->format(), share, useDefaultSceneGraph);
}

SimpleViewer::~SimpleViewer()
{
}

void SimpleViewer::resizeEvent(QResizeEvent *_event) {
  QGraphicsView::resizeEvent(_event);
  if (scene())
  {
    scene()->setSceneRect(QRect(QPoint(0, 0), _event->size()));
    mainWidget_->setGeometry (QRect(QPoint(0, 0), _event->size()));
  }
  
}


//=============================================================================
//=============================================================================

void SimpleViewer::initialize(const OFGLFormat & _format, OFGLWidget *_shareWidget, bool useDefaultSceneGraph)
{

  connect (&props_, SIGNAL( getPickMode(std::string&) ),
           this, SLOT( getPickMode(std::string&) ),Qt::DirectConnection );
  connect (&props_, SIGNAL( getActionMode(Viewer::ActionMode&) ),
           this, SLOT( getActionMode(Viewer::ActionMode&) ),Qt::DirectConnection );
  connect (&props_, SIGNAL( setActionMode(const Viewer::ActionMode) ),
           this, SLOT( setActionMode(const Viewer::ActionMode)), Qt::DirectConnection );

#if (QT_VERSION >= QT_VERSION_CHECK(5, 4, 0))
  glWidget_ = new OFGLWidget();
  glWidget_->setFormat(_format);
#else
  glWidget_ = new OFGLWidget(_format, 0, _shareWidget);
#endif

  setFocusPolicy (Qt::StrongFocus);
  setAcceptDrops (true);
  setViewport(glWidget_);
  setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

  scene_ = new SimpleGLGraphicsScene ();

  setScene (scene_);

  mainWidget_ = new glViewer (scene_, glWidget_, props_);

  scene_->addItem (mainWidget_);
  scene_->setView (mainWidget_);
  mainWidget_->setGeometry (QRect(QPoint(0, 0), size()));
  setFrameStyle(QFrame::NoFrame);

  if (useDefaultSceneGraph)
      setSceneGraph(PluginFunctions::getSceneGraphRootNode());

  props_.drawMode (OpenFlipper::Options::defaultDrawMode(0));
}

void SimpleViewer::setSceneGraph(ACG::SceneGraph::BaseNode *rootNode) {
    unsigned int maxPases = 1;
    ACG::Vec3d bbmin,bbmax;
    ACG::SceneGraph::analyzeSceneGraph(rootNode, maxPases, bbmin, bbmax);

    mainWidget_->sceneGraph(rootNode, maxPases, bbmin, bbmax, true);
}

void SimpleViewer::setActionMode(const Viewer::ActionMode _am)
{
  actionMode_ = _am;
}

void SimpleViewer::getActionMode(Viewer::ActionMode & _am)
{
  _am = actionMode_;
}

void SimpleViewer::getPickMode(std::string & _name)
{
  _name = "SimpleViewer";
}

Viewer::ViewerProperties & SimpleViewer::properties()
{
  return props_;
}

glViewer * SimpleViewer::viewer()
{
  return mainWidget_;
}

