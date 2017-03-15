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

#ifndef STATUSVIEWNODE_HH
#define STATUSVIEWNODE_HH

//== INCLUDES =================================================================

#include <OpenFlipper/common/GlobalDefines.hh>

#include <ACG/Scenegraph/StatusNodesT.hh>

#include "StatusNodeMods.hh"


//== CLASS DEFINITION =========================================================

namespace ACG
{
namespace SceneGraph
{

template <class MeshT>
class DLLEXPORTONLY StatusViewNodeT : public ACG::SceneGraph::BaseNode
{
public:
  StatusViewNodeT(
    BaseNode* _parent,
    const std::string& _name,
    ACG::SceneGraph::SelectionNodeT<MeshT>* _statusNode,
    ACG::SceneGraph::StatusNodeT<MeshT, AreaNodeMod<MeshT> >* _areaNode,
    ACG::SceneGraph::StatusNodeT<MeshT, HandleNodeMod<MeshT> >* _handleNode,
    ACG::SceneGraph::StatusNodeT<MeshT, FeatureNodeMod<MeshT> >* _featureNode);

  // BaseNode implementation

  ACG_CLASSNAME(StatusNodeView);


  void draw(GLState& , const DrawModes::DrawMode& );

  void getRenderObjects(IRenderer* _renderer, GLState& _state , const DrawModes::DrawMode& _drawMode , const Material* _mat);

  void enter(GLState& _state, const DrawModes::DrawMode& _drawmode);
  void leave(GLState& _state, const DrawModes::DrawMode& _drawmode);

private:

  ACG::SceneGraph::SelectionNodeT<MeshT>*                       statusNode_;
  ACG::SceneGraph::StatusNodeT<MeshT, AreaNodeMod<MeshT> >*     areaNode_;
  ACG::SceneGraph::StatusNodeT<MeshT, HandleNodeMod<MeshT> >*   handleNode_;
  ACG::SceneGraph::StatusNodeT<MeshT, FeatureNodeMod<MeshT> >*  featureNode_;

  // visibility of nodes
  bool statusNodeVis_;
  bool areaNodeVis_;
  bool handleNodeVis_;
  bool featureNodeVis_;
};


}
}

//=============================================================================
#if defined(INCLUDE_TEMPLATES) && !defined(STATUSVIEWNODE_C)
#define STATUS_VIEW_NODE_TEMPLATES
#include "StatusViewNodeT.cc"
#endif
//=============================================================================
#endif // STATUSVIEWNODE_HH
//=============================================================================
