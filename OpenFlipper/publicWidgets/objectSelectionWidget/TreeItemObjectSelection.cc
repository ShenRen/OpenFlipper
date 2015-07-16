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



#include "TreeItemObjectSelection.hh"

//--------------------------------------------------------------------------------

TreeItemObjectSelection::TreeItemObjectSelection(int _id, QString _name, DataType _type, TreeItemObjectSelection* _parent) :
  id_(_id),
  dataType_(_type),
  visible_(true),
  name_(_name),
  parentItem_(_parent)
{
}


// ===============================================================================
// Static Members
// ===============================================================================

int TreeItemObjectSelection::id() {
  return id_;
}

//--------------------------------------------------------------------------------

bool TreeItemObjectSelection::dataType(DataType _type) {
  if ( _type == DATA_ALL ) {
    return true;
  }

  return ( dataType_ & _type);
}

//--------------------------------------------------------------------------------

DataType TreeItemObjectSelection::dataType() {
  return dataType_;
}

//--------------------------------------------------------------------------------

int TreeItemObjectSelection::group() {
  // Skip root node
  if ( parent() == 0 )
    return -1;

  // Dont count root node as a group
  if ( parent()->parent() == 0 )
    return -1;

  // Only consider groups
  if ( !parent()->dataType(DATA_GROUP) )
    return -1;

  // Get the group id
  return ( parent()->id() );
}

//--------------------------------------------------------------------------------

bool TreeItemObjectSelection::isGroup() {
  return ( dataType(DATA_GROUP) );
}

// ===============================================================================
// Dynamic Members
// ===============================================================================

bool TreeItemObjectSelection::visible() {
  return visible_;
}

//--------------------------------------------------------------------------------

void TreeItemObjectSelection::visible(bool _visible) {
  visible_ = _visible;
}

//--------------------------------------------------------------------------------

QString TreeItemObjectSelection::name() {
  return name_;
}

//--------------------------------------------------------------------------------

void TreeItemObjectSelection::name(QString _name ) {
  name_ = _name;
}

// ===============================================================================
// Tree Structure
// ===============================================================================

TreeItemObjectSelection* TreeItemObjectSelection::next() {
  // Visit child item of this node
  if ( childItems_.size() > 0 ) {
     return childItems_[0];
  }

  // No Child Item so visit the next child of the parentItem_
  if ( parentItem_ ) {

    TreeItemObjectSelection* parentPointer = parentItem_;
    TreeItemObjectSelection* thisPointer   = this;

    // while we are not at the root node
    while ( parentPointer ) {

      // If there is an unvisited child of the parent, return this one
      if ( parentPointer->childCount() > ( thisPointer->row() + 1) ) {
        return parentPointer->childItems_[ thisPointer->row() + 1 ];
      }

      // Go to the next level
      thisPointer   = parentPointer;
      parentPointer = parentPointer->parentItem_;

    }

    return thisPointer;
  }

  return this;

}

//--------------------------------------------------------------------------------

int TreeItemObjectSelection::level() {
  int level = 0;
  TreeItemObjectSelection* current = this;

  // Go up and count the levels to the root node
  while ( current->parent() != 0 ) {
    level++;
    current = current->parent();
  }

  return level;
}

//--------------------------------------------------------------------------------

int TreeItemObjectSelection::row() const
{
    if (parentItem_)
        return parentItem_->childItems_.indexOf(const_cast<TreeItemObjectSelection*>(this));

    return 0;
}

//--------------------------------------------------------------------------------

TreeItemObjectSelection* TreeItemObjectSelection::parent()
{
  return parentItem_;
}

//--------------------------------------------------------------------------------

void TreeItemObjectSelection::setParent(TreeItemObjectSelection* _parent) {
  parentItem_ = _parent;
}

//--------------------------------------------------------------------------------

void TreeItemObjectSelection::appendChild(TreeItemObjectSelection *item)
{
    childItems_.append(item);
}

//--------------------------------------------------------------------------------

TreeItemObjectSelection *TreeItemObjectSelection::child(int row)
{
    return childItems_.value(row);
}

//--------------------------------------------------------------------------------

int TreeItemObjectSelection::childCount() const
{
    return childItems_.count();
}

//--------------------------------------------------------------------------------

TreeItemObjectSelection* TreeItemObjectSelection::childExists(int _objectId) {

  // Check if this object has the requested id
  if ( id_ == _objectId )
    return this;

  // search in children
  for ( int i = 0 ; i < childItems_.size(); ++i ) {
    TreeItemObjectSelection* tmp = childItems_[i]->childExists(_objectId);
    if ( tmp != 0)
      return tmp;
  }

  return 0;
}

//--------------------------------------------------------------------------------

TreeItemObjectSelection* TreeItemObjectSelection::childExists(QString _name) {

  // Check if this object has the requested id
  if ( name() == _name )
    return this;

  // search in children
  for ( int i = 0 ; i < childItems_.size(); ++i ) {
    TreeItemObjectSelection* tmp = childItems_[i]->childExists(_name);
    if ( tmp != 0)
      return tmp;
  }

  return 0;
}

//--------------------------------------------------------------------------------

void TreeItemObjectSelection::removeChild( TreeItemObjectSelection* _item ) {

  bool found = false;
  QList<TreeItemObjectSelection*>::iterator i;
  for (i = childItems_.begin(); i != childItems_.end(); ++i) {
     if ( *i == _item ) {
        found = true;
        break;
     }
  }

  if ( !found ) {
    std::cerr << "TreeItemObjectSelection: Illegal remove request" << std::endl;
    return;
  }

  childItems_.erase(i);
}

//--------------------------------------------------------------------------------

QList< TreeItemObjectSelection* > TreeItemObjectSelection::getLeafs() {

  QList< TreeItemObjectSelection* > items;

  for ( int i = 0 ; i < childItems_.size(); ++i ) {
    items = items + childItems_[i]->getLeafs();
  }

  // If we are a leave...
  if ( childCount() == 0 )
    items.push_back(this);

  return items;
}

//--------------------------------------------------------------------------------

void TreeItemObjectSelection::deleteSubtree() {

  // call function for all children of this node
  for ( int i = 0 ; i < childItems_.size(); ++i) {

    // remove the subtree recursively
    childItems_[i]->deleteSubtree();

    // delete child
    delete childItems_[i];
  }

  // clear the array
  childItems_.clear();
}

//=============================================================================
