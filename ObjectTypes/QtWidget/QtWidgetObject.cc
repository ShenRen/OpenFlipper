 
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
//  QtWidget Object
//
//=============================================================================

//== INCLUDES =================================================================
#include "QtWidgetObject.hh"
#include "QtWidget.hh"

//== DEFINES ==================================================================

//== TYPEDEFS =================================================================
QtWidgetObject::QtWidgetObject( ) :
  BaseObjectData(),
  widgetNode_(NULL)
{
  setDataType(DATA_QT_WIDGET);
  setTypeIcon(DATA_QT_WIDGET,"QtWidgetType.png");
  init(NULL);
}

/**
 * Copy Constructor - generates a copy of the given object
 */
QtWidgetObject::QtWidgetObject(const QtWidgetObject & _object) :
  BaseObjectData(_object),
  widgetNode_(NULL)
{
  init(_object.widget());
  setName( name() );
}

/** Destructor for Light Objects. The destructor deletes the light and all
*  Scenegraph nodes associated with the Light or the object.
*/
QtWidgetObject::~QtWidgetObject()
{
  // Delete the data attached to this object ( this will remove all perObject data)
  // Not the best way to do it but it will work.
  // This is only necessary if people use references to the light below and
  // they do something with the light in the destructor of their
  // perObjectData.
  deleteData();

  // Move children to parent
  BaseNode* parent = static_cast<BaseNode*>(widgetNode_)->parent();

  // First, collect all children as the iterator will get invalid if we delete while iterating!
  std::vector< BaseNode*> children;
  for (BaseNode::ChildIter cIt=widgetNode_->childrenBegin(); cIt!=widgetNode_->childrenEnd(); ++cIt)
    children.push_back( (*cIt) );

  // Move the children
  for (unsigned int i = 0 ; i < children.size(); ++i )
    children[i]->set_parent(parent);

  // Delete the scenegraph node
  delete widgetNode_;
}

/** Cleanup Function for Light Objects. Deletes the contents of the whole object and
* calls QtWidgetObject::init afterwards.
*/
void QtWidgetObject::cleanup() {

  BaseObjectData::cleanup();

  widgetNode_   = NULL;

  setDataType(DATA_QT_WIDGET);
  setTypeIcon(DATA_QT_WIDGET,"QtWidgetType.png");

}

/**
 * Generate a copy
 */
BaseObject* QtWidgetObject::copy() {
    QtWidgetObject* object = new QtWidgetObject(*this);
    return dynamic_cast< BaseObject* >(object);
}

/** This function initializes the light object. It creates the scenegraph nodes.
*/
void QtWidgetObject::init(QWidget* _widget) {

  widgetNode_ = new QtWidgetNode( _widget ,materialNode() , "QtWidgetNode");
}

// ===============================================================================
// Name/Path Handling
// ===============================================================================

/** Set the name of an object. All Scenegraph nodes are renamed too. It also calls
* BaseObjectData::setName.
*/
void QtWidgetObject::setName( QString _name ) {
  BaseObjectData::setName(_name);

  std::string nodename = std::string("Qt WidgetNode "     + _name.toUtf8() );
  widgetNode_->name( nodename );
}

// ===============================================================================
// Visualization
// ===============================================================================

QtWidgetNode* QtWidgetObject::qtWidgetNode() {
  return widgetNode_;
}


bool QtWidgetObject::hasNode(BaseNode* _node) {
 if ( BaseObjectData::hasNode(_node) )
   return true;

 if ( _node == widgetNode_ )
   return true;

 return false;
}

// ===============================================================================
// Object information
// ===============================================================================

/** Returns a string containing all information about the current object. This also
* includes the information provided by BaseObjectData::getObjectinfo
*
* @return String containing the object information
*/
QString QtWidgetObject::getObjectinfo() {
  QString output;

  output += "========================================================================\n";
  output += BaseObjectData::getObjectinfo();

  if ( dataType( DATA_QT_WIDGET ) )
    output += "Qt Accessible Name: ";
    output += widgetNode_->widget()->accessibleName();
    output += "\n";
  output += "========================================================================\n";
  return output;
}

// ===============================================================================
// Picking
// ===============================================================================

/** Given an node index from PluginFunctions::scenegraphPick this function can be used to
* check if the lightNode of the object has been picked.
*
* @param _node_idx Index of the picked QtWidget node
* @return bool if the widgetNode of this object is the picking target.
*/
bool QtWidgetObject::picked( uint _node_idx ) {
  return ( _node_idx == widgetNode_->id() );
}

void QtWidgetObject::enablePicking( bool _enable ) {
  widgetNode_->enablePicking( _enable );
}

bool QtWidgetObject::pickingEnabled() {
  return widgetNode_->pickingEnabled();
}

void QtWidgetObject::show() {
  if ( !visible_ ) {
    BaseObjectData::show();
    visible_ = true;

    static_cast<BaseNode*>(widgetNode_)->show();

    emit visibilityChanged( id() );
  }
}

void QtWidgetObject::hide() {
  if ( visible_ ) {
    BaseObjectData::hide();
    visible_ = false;
    static_cast<BaseNode*>(widgetNode_)->hide();

    emit visibilityChanged( id() );
  }
}

void QtWidgetObject::visible(bool _visible) {
  if ( _visible )
    show();
  else
    hide();
}

bool QtWidgetObject::visible() {
  return visible_;
}


//=============================================================================

