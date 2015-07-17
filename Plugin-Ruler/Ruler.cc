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

#include "Ruler.hh"

//------------------------------------------------------------------------------
Ruler::Ruler(BaseObjectData* _obj,const QString &_pluginName, unsigned index)
:pluginName_(_pluginName),
 lineNodeName_(QString("Ruler-Plugin LineNode%1").arg(index).toStdString()),
 textNodeName_(QString("Ruler-Plugin TextNode%1").arg(index).toStdString()),
 textTransformNodeName_(QString("Ruler-Plugin TransformNode%1").arg(index).toStdString()),
 lineNode_(0),
 textNode_(0),
 textTransformNode_(0),
 offset_(),
 obj_(_obj)
{
  points_[0] = points_[1] = ACG::Vec3d(0.0,0.0,0.0);

  //create line, point, text and transformation nodes
  if (!_obj->getAdditionalNode(lineNode_,_pluginName,lineNodeName_.c_str()))
  {
    lineNode_ = new ACG::SceneGraph::LineNode(ACG::SceneGraph::LineNode::LineSegmentsMode,
        _obj->manipulatorNode(),lineNodeName_);
    _obj->addAdditionalNode(lineNode_,_pluginName,lineNodeName_.c_str());
  }

  if (!_obj->getAdditionalNode(textTransformNode_,_pluginName,textTransformNodeName_.c_str()))
  {
    textTransformNode_ = new ACG::SceneGraph::TransformNode(lineNode_,textTransformNodeName_.c_str());
    _obj->addAdditionalNode(textTransformNode_,_pluginName,textTransformNodeName_.c_str());
  }

  if (!_obj->getAdditionalNode(textNode_,_pluginName,textNodeName_.c_str()))
  {
    textNode_ = new ACG::SceneGraph::TextNode(textTransformNode_,textNodeName_,ACG::SceneGraph::TextNode::SCREEN_ALIGNED_STATIC_SIZE,true);
    _obj->addAdditionalNode(textNode_,_pluginName,textNodeName_.c_str());

  }
}
//------------------------------------------------------------------------------
Ruler::~Ruler()
{
  if (obj_->getAdditionalNode(textNode_,pluginName_,textNodeName_.c_str()))
  {
    obj_->removeAdditionalNode(textNode_,pluginName_,textNodeName_.c_str());
  }
  if (obj_->getAdditionalNode(textTransformNode_,pluginName_,textTransformNodeName_.c_str()))
  {
    obj_->removeAdditionalNode(textTransformNode_,pluginName_,textTransformNodeName_.c_str());
  }
  if (obj_->getAdditionalNode(lineNode_,pluginName_,lineNodeName_.c_str()))
  {
    obj_->removeAdditionalNode(lineNode_,pluginName_,lineNodeName_.c_str());
  }
}
//------------------------------------------------------------------------------
void Ruler::setPoints(const ACG::Vec3d &_start,const ACG::Vec3d &_end)
{
  points_[0] = _start;
  points_[1] = _end;
  updateNodes();
}

void Ruler::setStartPoint(const ACG::Vec3d& _start)
{
  points_[0] = _start;
  updateNodes();
}
//------------------------------------------------------------------------------
void Ruler::setEndPoint(const ACG::Vec3d& _end)
{
  points_[1] = _end;
  updateNodes();
}
//------------------------------------------------------------------------------

void Ruler::updateNodes()
{
  ACG::Vec3d Point1 = points_[0];
  ACG::Vec3d Point2 = points_[1];

  //creates the line
  lineNode_->clear_points();
  lineNode_->set_color(OpenMesh::Vec4f(1.0f,0.0f,0.0f,1.0f));
  lineNode_->set_line_width(3);
  lineNode_->add_line(Point1,Point2);
  lineNode_->alwaysOnTop() = true;

  //set params for the text
  ACG::Vec3d distVec = Point1 - Point2;
  QString distanceStr = QString().number(distVec.length());
  textNode_->setText(distanceStr.toStdString());
  textNode_->multipassNodeSetActive(8, true);

  //translate
  setTextOffset(offset_);

  emit updateView();
}

void Ruler::setTextOffset(const ACG::Vec3d& offset)
{
  offset_ = offset;
  ACG::Vec3d distVec = points_[0] - points_[1];
  ACG::Vec3d halfDist = distVec/2.f;
  textTransformNode_->loadIdentity();

  textTransformNode_->translate((points_[0]-halfDist)+offset_);
}
