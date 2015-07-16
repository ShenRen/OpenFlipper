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
//  MyTypes
//
//=============================================================================

#define BASEOBJECTDATA_C


//== INCLUDES =================================================================

#include "Types.hh"

//== TYPEDEFS =================================================================

//== CLASS DEFINITION =========================================================

// ===============================================================================
// Additional Nodes
// ===============================================================================

template< typename NodeT >
bool BaseObjectData::addAdditionalNode(NodeT* _node , QString _pluginName, QString _nodeName , int _id )
{
   if ( hasAdditionalNode(_pluginName,_nodeName,_id) )  {
     std::cerr << "Trying to inserted additional node twice in " <<  _pluginName.toStdString()
                   << " with Name " << _nodeName.toStdString() << " and id " << _id << std::endl;
     return false;
   }

   QString name = _pluginName+"#"+_nodeName+"#"+QString::number(_id);

   std::pair <BaseNode*,QString> newNode(dynamic_cast<BaseNode*>(_node) , name);
   additionalNodes_.push_back(newNode);

   return true;
}

template< typename NodeT >
bool BaseObjectData::getAdditionalNode(NodeT*& _node , QString _pluginName, QString _nodeName , int _id )
{
  QString searchname = _pluginName + "#" + _nodeName + "#" + QString::number(_id);

  for ( uint i =0 ; i < additionalNodes_.size() ; ++i ) {
    if ( additionalNodes_[i].second == searchname ) {
        _node = dynamic_cast<NodeT*>(additionalNodes_[i].first);
        return ( _node != NULL);
    }
  }

  return false;
}

template< typename NodeT >
bool BaseObjectData::removeAdditionalNode(NodeT*& _node, QString _pluginName, QString _nodeName , int _id )
{
  QString searchname = _pluginName + "#" + _nodeName + "#" + QString::number(_id);

  for ( uint i =0 ; i < additionalNodes_.size() ; ++i ) {
    if ( additionalNodes_[i].second == searchname ) {
        _node = dynamic_cast<NodeT*>(additionalNodes_[i].first);
        if (_node != NULL) //valid node delete subtree
        {
           // Delete parent node from additional nodes list
           additionalNodes_.erase (additionalNodes_.begin()+i);
           
           // Remove children from list, too, since the objects will be deleted
           // on delete_subtree() but (invalid) pointers reside in the additionalNodes_ list
           // if not removed manually. We need to do this recursively:
           std::stack<BaseNode *> children;
           children.push(_node);
           while(!children.empty())
           {
             BaseNode *current = children.top();
             children.pop();
             
             for(BaseNode::ChildIter it = current->childrenBegin(); it != current->childrenEnd(); ++it) 
               children.push(*it);
             
             for(int j = additionalNodes_.size()-1; j >= 0; --j)
               if(additionalNodes_[j].first == current)
                 additionalNodes_.erase(additionalNodes_.begin()+j);
           }
          // Delete nodes recursively
          _node->delete_subtree();
        }
        return true;
    }
  }

  return false;
}

//=============================================================================
