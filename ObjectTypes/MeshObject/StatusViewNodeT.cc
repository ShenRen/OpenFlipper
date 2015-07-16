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
*   $Revision: 18128 $                                                       *
*   $LastChangedBy: moebius $                                                *
*   $Date: 2014-02-05 10:20:28 +0100 (Wed, 05 Feb 2014) $                     *
*                                                                            *
\*===========================================================================*/


//=============================================================================
//
//  StatusViewNode: Combined rendering of multiple Status Nodes
//
//=============================================================================

#ifndef STATUSVIEWNODE_C
#define STATUSVIEWNODE_C

//== INCLUDES =================================================================

#include "StatusViewNodeT.hh"
#include <ACG/Scenegraph/DrawModes.hh>

namespace ACG
{
namespace SceneGraph
{


//== IMPLEMENTATION ==========================================================

template<class MeshT>
StatusViewNodeT<MeshT>::StatusViewNodeT(
  BaseNode* _parent,
  const std::string& _name,
  ACG::SceneGraph::SelectionNodeT<MeshT>* _statusNode,
  ACG::SceneGraph::StatusNodeT<MeshT, AreaNodeMod<MeshT> >* _areaNode,
  ACG::SceneGraph::StatusNodeT<MeshT, HandleNodeMod<MeshT> >* _handleNode,
  ACG::SceneGraph::StatusNodeT<MeshT, FeatureNodeMod<MeshT> >* _featureNode)
  : BaseNode(_parent, _name), 
  statusNode_(_statusNode), 
  areaNode_(_areaNode),
  handleNode_(_handleNode),
  featureNode_(_featureNode),
  statusNodeVis_(false),
  areaNodeVis_(false),
  handleNodeVis_(false),
  featureNodeVis_(false)
{
}

//----------------------------------------------------------------------------

template <class MeshT>
void StatusViewNodeT<MeshT>::draw( GLState& _state, const DrawModes::DrawMode& _drawMode )
{
  // extract all layers from drawmode

  DrawModes::DrawMode singleLayers[4]; // polygon, edge, halfedge, point

  // remove default property layer
  for (int i = 0; i < 4; ++i)
    singleLayers[i].removeLayer(0u);

  for (unsigned int i = 0; i < _drawMode.getNumLayers(); ++i)
  {
    const DrawModes::DrawModeProperties* props = _drawMode.getLayer(i);

    switch (props->primitive())
    {
    case DrawModes::PRIMITIVE_POLYGON: singleLayers[0].setDrawModeProperties(props); break;
    case DrawModes::PRIMITIVE_EDGE: singleLayers[1].setDrawModeProperties(props); break;
    case DrawModes::PRIMITIVE_HALFEDGE: singleLayers[2].setDrawModeProperties(props); break;
    case DrawModes::PRIMITIVE_POINT: singleLayers[3].setDrawModeProperties(props); break;
    default: break;
    }
  }

  // rendering order: faces -> edges -> halfedges -> points
  //   area -> handle -> feature -> selections
  for (int i = 0; i < 4; ++i)
  {

    if (singleLayers[i].getNumLayers() == 0)
    {
      DrawModes::DrawModeProperties defaultProps;

      switch (i)
      {
      case 0: defaultProps.primitive(DrawModes::PRIMITIVE_POLYGON); break;
      case 1: defaultProps.primitive(DrawModes::PRIMITIVE_EDGE); break;
      case 2: defaultProps.primitive(DrawModes::PRIMITIVE_HALFEDGE); break;
      case 3: defaultProps.primitive(DrawModes::PRIMITIVE_POINT); break;
      default: break;
      }
      
      singleLayers[i].addLayer(&defaultProps);
    }

    // call enter() and leave() to setup the correct material

    if (areaNodeVis_)
    {
      areaNode_->enter(_state, singleLayers[i]);
      areaNode_->draw(_state, singleLayers[i]);
      areaNode_->leave(_state, singleLayers[i]);
    }

    if (handleNodeVis_)
    {
      handleNode_->enter(_state, singleLayers[i]);
      handleNode_->draw(_state, singleLayers[i]);
      handleNode_->leave(_state, singleLayers[i]);
    }

    if (featureNodeVis_)
    {
      featureNode_->enter(_state, singleLayers[i]);
      featureNode_->draw(_state, singleLayers[i]);
      featureNode_->leave(_state, singleLayers[i]);
    }

    if (statusNodeVis_)
    {
      statusNode_->enter(_state, singleLayers[i]);
      statusNode_->draw(_state, singleLayers[i]);
      statusNode_->leave(_state, singleLayers[i]);
    }
  }

}

//----------------------------------------------------------------------------

template <class MeshT>
void StatusViewNodeT<MeshT>::getRenderObjects(IRenderer* _renderer, GLState& _state , const DrawModes::DrawMode& _drawMode , const Material* _mat)
{
  // rendering order only depends on priority in render objects

  // call enter() and leave() to setup the correct material
  if (statusNodeVis_)
  {
    statusNode_->getRenderObjects(_renderer, _state, _drawMode, _mat);
  }

  if (areaNodeVis_)
  {
    areaNode_->getRenderObjects(_renderer, _state, _drawMode, _mat);
  }

  if (handleNodeVis_)
  {
    handleNode_->getRenderObjects(_renderer, _state, _drawMode, _mat);
  }

  if (featureNodeVis_)
  {
    featureNode_->getRenderObjects(_renderer, _state, _drawMode, _mat);
  }
}


//----------------------------------------------------------------------------

template <class MeshT>
void StatusViewNodeT<MeshT>::enter( GLState& _state, const DrawModes::DrawMode& _drawmode )
{
  statusNodeVis_ = statusNode_ && statusNode_->visible();
  areaNodeVis_ = areaNode_ && areaNode_->visible();
  handleNodeVis_ = handleNode_ && handleNode_->visible();
  featureNodeVis_ = featureNode_ && featureNode_->visible();

  // these nodes are manually rendered in this StatusViewNode
  //  hide nodes to prevent drawing them again in the same draw traversal

  if (statusNodeVis_)
    statusNode_->hide();
  if (areaNodeVis_)
    areaNode_->hide();
  if (handleNodeVis_)
    handleNode_->hide();
  if (featureNodeVis_)
    featureNode_->hide();
}

//----------------------------------------------------------------------------

template <class MeshT>
void StatusViewNodeT<MeshT>::leave( GLState& _state, const DrawModes::DrawMode& _drawmode )
{
  if (statusNodeVis_)
    statusNode_->show();
  if (areaNodeVis_)
    areaNode_->show();
  if (handleNodeVis_)
    handleNode_->show();
  if (featureNodeVis_)
    featureNode_->show();
}


}
}

#endif // STATUSVIEWNODE_C
