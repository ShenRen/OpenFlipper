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
#include <QObject>
#include <QString>

#include <ACG/Scenegraph/LineNode.hh>
#include <ACG/Scenegraph/TextNode.hh>
#include <OpenFlipper/common/BaseObjectData.hh>

class Ruler : public QObject
{
  Q_OBJECT
signals:
  void updateView();

public:
  /**
   * creates a new ruler on a given object
   * @param _obj Object where additional nodes are added
   * @param _pluginName name of the plugin
   * @param _index the current index of the ruler. If you create multiple rulers, you can use this parameter to avoid identical nodenames
   */
  Ruler(BaseObjectData *_obj,const QString &_pluginName, unsigned _index);
  ~Ruler();

  void setPoints(const ACG::Vec3d& _start,const ACG::Vec3d& _end);
  void setStartPoint(const ACG::Vec3d& _start);
  void setEndPoint(const ACG::Vec3d& _end);
  void setTextOffset(const ACG::Vec3d& offset);

  const ACG::Vec3d* points() const{return points_;}

  const BaseObject* getBaseObj(){return obj_;}

  float textScale(){return textNode_->lastScale();}

public slots:

  // set text size in pixels
  void setTextSize(const unsigned _size){textNode_->setPixelSize(_size); emit updateView();}

private:

  void updateNodes();

  const QString pluginName_;
  const std::string lineNodeName_;
  const std::string textNodeName_;
  const std::string textTransformNodeName_;

  ACG::SceneGraph::LineNode* lineNode_;
  ACG::SceneGraph::TextNode* textNode_;
  ACG::SceneGraph::TransformNode* textTransformNode_;

  ACG::Vec3d points_[2];
  ACG::Vec3d offset_;
  BaseObjectData* obj_;
};
