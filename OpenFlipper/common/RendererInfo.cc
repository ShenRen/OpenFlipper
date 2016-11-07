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



#include <OpenFlipper/common/RendererInfo.hh>

static RenderManager        renderManager_;
static PostProcessorManager postProcessorManager_;

RenderManager& renderManager() {
  return renderManager_;
}

PostProcessorManager& postProcessorManager() {
  return postProcessorManager_;
}

RendererInfo::RendererInfo():
  plugin(0),
  name(""),
  optionsAction(0)
{
}

RendererInfo::RendererInfo(RenderInterface* _plugin,QString _name) :
    plugin(_plugin),
    name(_name),
    optionsAction(0)
{
}


RenderManager::RenderManager()
{
  availableRenderers_.clear();
  availableRenderers_.push_back(RendererInfo(0,"Default internal renderer"));

  activeRenderers_.clear();
}

bool RenderManager::rendererExists(QString _name) {

  for ( unsigned int i = 0 ; i < availableRenderers_.size() ; ++i)
    if ( availableRenderers_[i].name == _name)
      return true;

  return false;

}

RendererInfo* RenderManager::newRenderer(QString _name) {

  for ( unsigned int i = 0 ; i < availableRenderers_.size() ; ++i)
    if ( availableRenderers_[i].name == _name)
      return &availableRenderers_[i];

  availableRenderers_.push_back(RendererInfo());
  availableRenderers_[availableRenderers_.size()-1].name = _name;

  return &availableRenderers_[availableRenderers_.size()-1];
}

RendererInfo* RenderManager::getRenderer(QString _name) {

  for ( unsigned int i = 0 ; i < availableRenderers_.size() ; ++i)
    if ( availableRenderers_[i].name == _name)
      return &availableRenderers_[i];

  return 0;
}

int RenderManager::getRendererId(QString _name) {

  for ( unsigned int i = 0 ; i < availableRenderers_.size() ; ++i)
    if ( availableRenderers_[i].name == _name)
      return i;

  return -1;
}

int RenderManager::countRenderers(ACG::SceneGraph::DrawModes::DrawMode _mode) {

  int renderers = 0;

  // Skip first one as it is the default renderer
  for ( unsigned int i = 1 ; i < availableRenderers_.size() ; ++i)
     if ( (availableRenderers_[i].modes & _mode) )
       renderers++;

  return renderers;
}

RendererInfo* RenderManager::operator[](unsigned int _id) {

  if ( _id >= availableRenderers_.size())
    return 0;
  else
    return &availableRenderers_[_id];

}

size_t RenderManager::available() {
  return availableRenderers_.size();
}


void RenderManager::setActive(unsigned int _active, int _id) {

  if ( _id < 0 ) {
    std::cerr << "RenderManager::setActive illegal viewer id: " << _id << std::endl;
    return;
  }

  // Increase vector size
  if ( _id >= (int)activeRenderers_.size() )
    activeRenderers_.resize(_id +1 );

  if ( _active <  availableRenderers_.size() )
    activeRenderers_[_id] = _active;
  else
    std::cerr << "Out of range error when setting active renderer" << std::endl;
}

void RenderManager::setActive(QString _active, int _id) {

  if ( _id < 0 ) {
    std::cerr << "RenderManager::setActive illegal viewer id: " << _id << std::endl;
    return;
  }

  // Increase vector size
  if ( _id >= (int)activeRenderers_.size() ) {
    activeRenderers_.resize(_id +1 );

  }

  for ( unsigned int i = 0 ; i < availableRenderers_.size() ; ++i)
    if ( availableRenderers_[i].name == _active) {
      activeRenderers_[_id] = i;
    }

}

RendererInfo* RenderManager::active( int _id) {

  // Temporary viewer with no fixed id, return first renderer
  if ( _id < 0 )
    return 0;

  // Increase vector size
  if ( _id >= (int)activeRenderers_.size() )
    activeRenderers_.resize(_id +1 );

  return &availableRenderers_[activeRenderers_[_id]];
}

unsigned int RenderManager::activeId( int _id) {

  // Temporary viewer with no fixed id, return first renderer
  if ( _id < 0 )
    return 0;

  // Increase vector size
  if ( _id >= (int)activeRenderers_.size() )
    activeRenderers_.resize(_id +1 );

  return activeRenderers_[_id];
}

//===================================================================

PostProcessorInfo::PostProcessorInfo(PostProcessorInterface* _plugin, QString _name) :
          plugin(_plugin),
          name(_name),
          optionsAction(0)
{
}

PostProcessorInfo::PostProcessorInfo():
    plugin(0),
    name(""),
    optionsAction(0)
{
}


PostProcessorManager::PostProcessorManager()
{
  availablePostProcessors_.clear();
  availablePostProcessors_.push_back(PostProcessorInfo(0,"Default internal post processor"));

  activePostProcessors_.clear();
}


bool PostProcessorManager::postProcessorExists(QString _name) {

  for ( unsigned int i = 0 ; i < availablePostProcessors_.size() ; ++i)
    if ( availablePostProcessors_[i].name == _name)
      return true;

  return false;

}

PostProcessorInfo* PostProcessorManager::newPostProcessor(QString _name) {

  for ( unsigned int i = 0 ; i < availablePostProcessors_.size() ; ++i)
    if ( availablePostProcessors_[i].name == _name)
      return &availablePostProcessors_[i];

  availablePostProcessors_.push_back(PostProcessorInfo());
  availablePostProcessors_[availablePostProcessors_.size()-1].name = _name;

  return &availablePostProcessors_[availablePostProcessors_.size()-1];
}

PostProcessorInfo* PostProcessorManager::getPostProcessor(QString _name) {

  for ( unsigned int i = 0 ; i < availablePostProcessors_.size() ; ++i)
    if ( availablePostProcessors_[i].name == _name)
      return &availablePostProcessors_[i];

  return 0;
}

PostProcessorInfo* PostProcessorManager::operator[](unsigned int _id ) {

  if ( _id >= availablePostProcessors_.size())
    return 0;
  else
    return &availablePostProcessors_[_id];

}

size_t PostProcessorManager::available() {
  return availablePostProcessors_.size();
}

void PostProcessorManager::setActive(unsigned int _active, int _viewerId ) {

  // Temporary viewer with no fixed id
  if ( _viewerId < 0 ) {
    std::cerr << "PostProcessorManager::setActive illegal viewer id: " << _viewerId << std::endl;
    return;
  }

  // Increase vector size
  if ( _viewerId >= (int)activePostProcessors_.size() )
    activePostProcessors_.resize(_viewerId +1 );

  // clear chain and add postprocessor
  activePostProcessors_[_viewerId].clear();
  activePostProcessors_[_viewerId].push_back(_active);
}

void PostProcessorManager::setActive(QString _active, int _id ) {

  // Temporary viewer with no fixed id
  if ( _id < 0 ) {
    std::cerr << "PostProcessorManager::setActive illegal viewer id: " << _id << std::endl;
    return;
  }

  // Increase vector size
  if ( _id >= (int)activePostProcessors_.size() )
    activePostProcessors_.resize(_id +1 );

  for ( unsigned int i = 0 ; i < availablePostProcessors_.size() ; ++i)
    if ( availablePostProcessors_[i].name == _active) {

      // clear chain and add postprocessor
      activePostProcessors_[_id].clear();
      activePostProcessors_[_id].push_back(i);
    }

}

PostProcessorInfo* PostProcessorManager::active( int _id, int _chainIdx ) {

  // Temporary viewer with no fixed id, return first post processor
  if ( _id < 0 )
    return 0;

  // Increase vector size
   if ( _id >= (int)activePostProcessors_.size() )
     activePostProcessors_.resize(_id +1 );

   if ( (int)activePostProcessors_[_id].size() <= _chainIdx)
     return 0;

  return &availablePostProcessors_[activePostProcessors_[_id][_chainIdx]];
}

unsigned int PostProcessorManager::activeId( int _id, int _chainIdx ) {

  // Temporary viewer with no fixed id, return first post processor
  if ( _id < 0 )
    return 0;

  // Increase vector size
  if ( _id >= (int)activePostProcessors_.size() )
    activePostProcessors_.resize(_id +1 );

  if ( (int)activePostProcessors_[_id].size() <= _chainIdx)
    return 0;

  return activePostProcessors_[_id][_chainIdx];
}

int PostProcessorManager::numActive( int _id ) {

  // Temporary viewer with no fixed id, return first post processor
  if ( _id < 0 )
    return 0;

  // Increase vector size
  if ( _id >= (int)activePostProcessors_.size() )
    activePostProcessors_.resize(_id +1 );

  return (int)activePostProcessors_[_id].size();
}

void PostProcessorManager::append( unsigned int _active, int _viewerId ) {

  // Temporary viewer with no fixed id
  if ( _viewerId < 0 ) {
    std::cerr << "PostProcessorManager::append illegal viewer id: " << _viewerId << std::endl;
    return;
  }

  // Increase vector size
  if ( _viewerId >= (int)activePostProcessors_.size() )
    activePostProcessors_.resize(_viewerId +1 );

  if ( _active <  availablePostProcessors_.size() )
    activePostProcessors_[_viewerId].push_back(_active);
  else
    std::cerr << "Out of range error when setting active post processor" << std::endl;

}

void PostProcessorManager::append( QString _active, int _id ) {

  // Temporary viewer with no fixed id
  if ( _id < 0 ) {
    std::cerr << "PostProcessorManager::append illegal viewer id: " << _id << std::endl;
    return;
  }

  // Increase vector size
  if ( _id >= (int)activePostProcessors_.size() )
    activePostProcessors_.resize(_id +1 );

  for ( unsigned int i = 0 ; i < availablePostProcessors_.size() ; ++i)
    if ( availablePostProcessors_[i].name == _active) {
      activePostProcessors_[_id].push_back(i);
    }

}


void PostProcessorManager::insert( unsigned int _active, int _chainIdx, int _viewerId ) {

  // Temporary viewer with no fixed id
  if ( _viewerId < 0 ) {
    std::cerr << "PostProcessorManager::insert illegal viewer id: " << _viewerId << std::endl;
    return;
  }

  // Increase vector size
  if ( _viewerId >= (int)activePostProcessors_.size() )
    activePostProcessors_.resize(_viewerId +1 );

  if ( _active <  availablePostProcessors_.size() )
    activePostProcessors_[_viewerId].insert(activePostProcessors_[_viewerId].begin() + _chainIdx, _active);
  else
    std::cerr << "Out of range error when setting active post processor" << std::endl;

}

void PostProcessorManager::insert( QString _active, int _chainIdx, int _id ) {

  // Temporary viewer with no fixed id
  if ( _id < 0 ) {
    std::cerr << "PostProcessorManager::insert illegal viewer id: " << _id << std::endl;
    return;
  }

  // Increase vector size
  if ( _id >= (int)activePostProcessors_.size() )
    activePostProcessors_.resize(_id +1 );

  for ( unsigned int i = 0 ; i < availablePostProcessors_.size() ; ++i)
    if ( availablePostProcessors_[i].name == _active) {
      activePostProcessors_[_id].insert(activePostProcessors_[_id].begin() + _chainIdx, i);
    }

}

void PostProcessorManager::remove( int _id, int _chainIdx ) {

  // Temporary viewer with no fixed id, return first post processor
  if ( _id < 0 )
    return;

  // Increase vector size
  if ( _id >= (int)activePostProcessors_.size() )
    activePostProcessors_.resize(_id +1 );

  if ( (int)activePostProcessors_[_id].size() <= _chainIdx)
    return;

  activePostProcessors_[_id].erase(activePostProcessors_[_id].begin() + _chainIdx);

}



