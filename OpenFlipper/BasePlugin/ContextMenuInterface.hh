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


#ifndef CONTEXTMENUINTERFACE_HH
#define CONTEXTMENUINTERFACE_HH

#include <OpenFlipper/common/Types.hh>
#if QT_VERSION >= 0x050000 
  #include <QtWidgets>
#else
  #include <QtGui>
#endif

#include <QMenuBar>


/** \file ContextMenuInterface.hh
*
* Interface for adding context menus to OpenFlippers UI from a plugin.\ref contextMenuInterfacePage
*/


/** Enum to specify the type of context menus
*/
enum ContextMenuType {
  
  /// The Menu will be shown when an object was picked
  CONTEXTOBJECTMENU,   
  
  /// The Menu will be shown when a node was picked
  CONTEXTNODEMENU,      
  
  /// The Menu will be shown when the background was picked
  CONTEXTBACKGROUNDMENU 
};




/** \brief Interface class for creating custom context menus
  *
  * \ref contextMenuInterfacePage "Detailed description"
  * \n
  * \n
  * Using this interface you can create custom context menus for your plugin. You
  * can choose between context menus for objects, nodes or the background.\n
  * Before a menu of the requested type is shown, an update function for the specific type
  * will be invoked by the core.
  * You have to create a QAction. The signals and slots of your actions have to be connected
  * to your plugin. Just connect them as usual. Only visibility of the menu is handled
  * by the core. You can also add submenus to the context menus. Just add the action for
  * the menu ( menu->menuAction() )
*/
class ContextMenuInterface {

public :

  /// Destructor
  virtual ~ContextMenuInterface() {};

signals:

  /**  \brief Add an entry for a context Menu
    *
    * Create an Action (Can also be the action of a Menu) and register this menu as a context menu to the core.
    * This Action will be visible  when you right click in the viewer widget on an item
    * of the given context menu type. You can add a whole menu here by adding the action:
    * menu->menuAction() of your own menu.
    *
    * @param _action Pointer to the new Action
    * @param _type   Type context menu type
  */
  virtual void addContextMenuItem(QAction* _action , ContextMenuType _type) {};

  /**  \brief Add an entry for a  context Menu
    *
    * Create an action (Can also be the action of a Menu) and register this action as a context menu entry to the core.
    * This Action will only be visible if the picked object is of the given DataType.
    * To support multiple object types with your menu, you can emit this signal multiple
    * times with the same action but different DataTypes. You can add a whole Menu here by adding the action:
    * menu->menuAction()
    *
    * @param _action Pointer to the new action
    * @param _objectType Type of the picked object
   *  @param _type Type of the context Menu ( See ContextMenuType )
  */
  virtual void addContextMenuItem(QAction* _action ,DataType _objectType , ContextMenuType _type ) {};

  /// hide the main context menu
  virtual void hideContextMenu() {};

private slots:

  /** When the main application requests a context menu, this slot is called before showing the window.
   * If an object is picked the id will be given in this call so you can change the contents of your menu
   * depending on the given object.
   *
   * @param _objectId id of the object
   */
  virtual void slotUpdateContextMenu( int _objectId ) {};

  /** When the main application requests a context menu, this slot is called before showing the window.
   * This slot will be called indicating that a scenegraph node not belonging to an object
   * has been picked.
   *
   * @param _nodeId id of the picked node
   */
  virtual void slotUpdateContextMenuNode( int _nodeId ) {};

  /** When the main application requests a context menu, this slot is called before showing the window.
   * This slot will be called indicating that the background has been picked.
   *
   */
  virtual void slotUpdateContextMenuBackground( ) {};

};


/** \page contextMenuInterfacePage Context Menu Interface
\image html ContextMenuInterface.png
\n
The ContextMenuInterface can be used by plugins to add menu entries to OpenFlippers
UI. The entries will be added to OpenFlippers contextMenus in the glView (see image). 
You can choose between context menus for objects, nodes or the background.\n

The given context menu entries will be shown when the user right clicks on the corresponding
primitive (node,object) in the GL viewer.

To use the ContextMenuInterface:
<ul>
<li> include ContextMenuInterface.hh in your plugins header file
<li> derive your plugin from the class ContextMenuInterface
<li> add Q_INTERFACES(ContextMenuInterface) to your plugin class 
<li> And add the signals or slots you want to use to your plugin class (You don't need to implement all of them)
</ul>


Usually you should implement the BaseInterface::pluginsInitialized() function from BaseInterface. In this function you can setup
your menus.\n

You have to create a QAction. The signals and slots of your actions have to be connected
to your plugin. Just connect them as usual. Only visibility of the menu is handled
by the core. You can also add submenus to the context menus. Just add the action for
the menu ( menu->menuAction() )

Before a menu of the requested type is shown, an update function for the specific type
will be invoked by the core depending on your type of context menu 
( ContextMenuInterface::slotUpdateContextMenu(), ContextMenuInterface::slotUpdateContextMenuNode(), ContextMenuInterface::slotUpdateContextMenuBackground() ). 
In this function you can update entries based on the object that was clicked on. The id of the node
or the object is provided by these update functions.


The following code shows a simple example to create a menu entry in the context menu.
\code 

// Setup the menus in initialize plugins
void ExamplePlugin::pluginsInitialized()
{
  // create a global QMenu in the plugin that contains our menu
  // in the header is: 
  // QMenu* contextMenu_
  contextMenu_ = new QMenu(tr("Select"));
  
  // Create a menu action called all in the Selection context menu
  // in the header is:   
  // QAction* menuAction_
  menuAction_ = contextMenu_->addAction( tr("All Mesh vertices") );
  menuAction->setToolTip(tr("Select all"));
  
  // Add the new menu to OpenFlippers context menu fot objects
  // Show the context menu for triangle meshes
  emit addContextMenuItem(contextMenu_->menuAction() , DATA_TRIANGLE_MESH , CONTEXTOBJECTMENU );
  
  // Show the context menu for poly meshes meshes
  emit addContextMenuItem(contextMenu_->menuAction() , DATA_POLY_MESH     , CONTEXTOBJECTMENU );
  }
  
  
// Example function to update the context menu based on the given object id
void ExamplePlugin::slotUpdateContextMenu( int _objectId ){
  
  // Get the corresponding object of the given id or return
  BaseObjectData* object = 0;
  if ( !PluginFunctions::getObject( _objectId, object ) ) {
    return;
  }
  
  // If its a triangle mesh, rename the to triangle mesh selection
  if (object->dataType( DATA_TRIANGLE_MESH ) ) 
    menuAction_->setText("All Triangle mesh vertices");
  
  // If its a triangle mesh, rename the to poly mesh selection
  if ( object->dataType( DATA_POLY_MESH ) )  
    menuAction_->setText("All Triangle mesh vertices");    
  
}


}
\endcode


Signals and slots of your menus (e.g. from an action inside it) can be directly connected to signals and slots in
your plugin. Therefore the embedding of your menus into the OpenFlippers context menu list is fully transparent.

*/

Q_DECLARE_INTERFACE(ContextMenuInterface,"OpenFlipper.ContextMenuInterface/1.0")

#endif // CONTEXTMENUINTERFACE_HH
