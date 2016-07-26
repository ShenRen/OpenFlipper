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


#include "SlicePlugin.hh"

#include "OpenFlipper/BasePlugin/PluginFunctions.hh"
#include <OpenFlipper/common/GlobalOptions.hh>

SlicePlugin::SlicePlugin() :
        tool_(0),
        toolIcon_(0),
        node_(0)
{

}

void SlicePlugin::initializePlugin(){
  //init the slice node
  node_ = new ACG::SceneGraph::ClippingNode(0,"Clipping Node");

 
  PluginFunctions::addObjectRenderingNode( node_ );

  node_->set_status( ACG::SceneGraph::BaseNode::HideNode );
  node_->setMultipassStatus(ACG::SceneGraph::BaseNode::NOPASS);
  
  tool_ = new SliceToolBox();
  
  QSize size(300, 300);
  tool_->resize(size);
  
  QButtonGroup* bbGroup = new QButtonGroup();
  bbGroup->setExclusive( true );
  bbGroup->addButton( tool_->radioAll );
  bbGroup->addButton( tool_->radioTarget );
  
  QButtonGroup* axisGroup = new QButtonGroup();
  axisGroup->setExclusive( true );
  axisGroup->addButton( tool_->radioX );
  axisGroup->addButton( tool_->radioY );
  axisGroup->addButton( tool_->radioZ );
  
  tool_->radioAll->setChecked( true );
  tool_->radioX->setChecked( true );
  
  connect(tool_->radioAll, SIGNAL( released() ), this, SLOT( updateSlice() ) );
  connect(tool_->radioTarget, SIGNAL( released() ), this, SLOT( updateSlice() ) );
  connect(tool_->resetButton, SIGNAL( released() ), this, SLOT( resetParameters() ) );
  connect(tool_->enabled, SIGNAL( released() ), this, SLOT( updateSlice() ) );
  connect(tool_->radioX, SIGNAL( released() ), this, SLOT( updateSlice() ) );
  connect(tool_->radioY, SIGNAL( released() ), this, SLOT( updateSlice() ) );
  connect(tool_->radioZ, SIGNAL( released() ), this, SLOT( updateSlice() ) );
  connect(tool_->posSlider, SIGNAL( valueChanged(int) ), this, SLOT( updateSlice(int) ) );
  connect(tool_->sizeSlider, SIGNAL( valueChanged(int) ), this, SLOT( updateSlice(int) ) );

  toolIcon_ = new QIcon(OpenFlipper::Options::iconDirStr()+OpenFlipper::Options::dirSeparator()+"slice.png");
  emit addToolbox( tr("Slice") , tool_, toolIcon_);
}

void SlicePlugin::resetParameters(){
  tool_->posSlider->setValue(0);
  tool_->sizeSlider->setValue(102);
  updateSlice();
}

void SlicePlugin::updateSlice(int /*bla*/){
 updateSlice();
}

void SlicePlugin::updateSlice(){

  if ( tool_->enabled->isChecked() ) {
    node_->set_status( ACG::SceneGraph::BaseNode::Active );
    node_->setMultipassStatus(BaseNode::ALLPASSES);
  } else {
    node_->set_status( ACG::SceneGraph::BaseNode::HideNode );
    node_->setMultipassStatus(ACG::SceneGraph::BaseNode::NOPASS);
  }

  if ( tool_->enabled->isChecked() ){

    ACG::Vec3d bbmin;
    ACG::Vec3d bbmax;

    getBoundingBox( bbmin, bbmax);

    ACG::Vec3d center = (bbmin + bbmax) * 0.5;
    ACG::Vec3f pos (center[0], center[1], center[2]);

    //get the normal
    ACG::Vec3f normal(1.0, 0.0, 0.0);
    float eps;
    float offset = 0.0;

    eps = tool_->sizeSlider->value() / 100.0;

    if (eps == 0.0)
      eps = 0.01f;

    if (tool_->radioX->isChecked()){
      normal = ACG::Vec3f(1.0, 0.0, 0.0);
      eps *= (bbmax[0] - bbmin[0]);
      offset = (bbmax[0] - bbmin[0]) * 0.5;
    }else
    if (tool_->radioY->isChecked()){
      normal = ACG::Vec3f(0.0, 1.0, 0.0);
      eps *= (bbmax[1] - bbmin[1]);
      offset = (bbmax[1] - bbmin[1]) * 0.5;
    }else
    if (tool_->radioZ->isChecked()){
      normal = ACG::Vec3f(0.0, 0.0, 1.0);
      eps *= (bbmax[2] - bbmin[2]);
      offset = (bbmax[2] - bbmin[2]) * 0.5;
    }

    pos += normal * ( (float)tool_->posSlider->value() / 100.0 ) * (offset + 0.1); //0.1 is just a little workarround

    node_->set_plane(pos, normal, eps);
  }
  emit updateView();
}

void SlicePlugin::getBoundingBox(ACG::Vec3d& bbmin, ACG::Vec3d& bbmax){

  bool firstRound = true;

  PluginFunctions::IteratorRestriction restriction;

  if (tool_->radioTarget->isChecked())
    restriction = PluginFunctions::TARGET_OBJECTS;
  else
    restriction = PluginFunctions::ALL_OBJECTS;

  for ( PluginFunctions::ObjectIterator o_it(restriction,DataType(DATA_ALL)) ;
                                        o_it != PluginFunctions::objectsEnd(); ++o_it)  {
    // get scene size
    ACG::Vec3d cur_min;
    ACG::Vec3d cur_max;

    o_it->getBoundingBox(cur_min, cur_max);

    if (firstRound){
      bbmin = cur_min;
      bbmax = cur_max;
      firstRound = false;
    }else{
      bbmin[0] = std::min( bbmin[0], cur_min[0]);
      bbmin[1] = std::min( bbmin[1], cur_min[1]);
      bbmin[2] = std::min( bbmin[2], cur_min[2]);
      bbmax[0] = std::max( bbmax[0], cur_max[0]);
      bbmax[1] = std::max( bbmax[1], cur_max[1]);
      bbmax[2] = std::max( bbmax[2], cur_max[2]);
    }
  }

  if ((bbmin[0] > bbmax[0]) || (bbmin[1] > bbmax[1]) || (bbmin[2] > bbmax[2]))
    std::cerr << "Error while computing bounding box!";

}

#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2( slicePlugin , SlicePlugin );
#endif

