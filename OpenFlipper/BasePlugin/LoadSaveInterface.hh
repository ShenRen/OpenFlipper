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

#ifndef LOADSAVEINTERFACE_HH
#define LOADSAVEINTERFACE_HH

#include <OpenFlipper/common/Types.hh>

/** \file LoadSaveInterface.hh
*
* Interface for adding and removing objects.\ref loadSaveInterfacePage
*/

/** \brief Interface for all plugins which want to Load or Save files and create Objects
  *
  * \ref loadSaveInterfacePage "Detailed description"
  * \n
  *
  * Using this interface you can instruct the core to open/save objects or
  * to create new empty objects.
 */
class LoadSaveInterface {

   public:

      /// Destructor
      virtual ~LoadSaveInterface() {};

   signals :
      /** \brief Save object to a file
       *
       * @param _id Object to Save
       * @param _filename Filename to save it to (Leave as "" to automatically determine filename)
      */
      virtual void save(int _id , QString _filename ) {}

      /** \brief Load object from file with a specific DataType
       *
       * @param _filename Filename of the File to load
       * @param _type Type to be loaded
       * @param _id Here the id of the loaded object is returned. id is -1 something went wrong
       */
      virtual void load(QString _filename, DataType _type, int& _id) {};


      /** Add an empty object of the given type\n
       * This signal can be called from any thread.\n
       *
       * @param _type Type to be created
       * @param _id Here the id of the loaded object is returned. id is -1 something went wrong
       */
      virtual void addEmptyObject( DataType _type, int& _id) {};

      /** Create a copy of an existing object
       *
       * @param _oldId id of the object to copy
       * @param _newId id of the new object created
       */
      virtual void copyObject( int _oldId, int& _newId) {};


      /** \brief DEPRECATED HERE (Moved to Type Interface)! Emit this signal if an empty object has been created
       *
       * Deprecated! Objects are added by the Type Plugins and this is signal is in the TypeInterface now
       *
       * @param _id Id of the added object
       */
      virtual void emptyObjectAdded( int _id ) {};

      /** \brief Delete an object\n
       *  This signal can be called from any thread.\n
       * @param _id Id of the object
       */
      virtual void deleteObject( int _id ) {};

      /** \brief Delete all Objects
       *
       */
      virtual void deleteAllObjects() {};

      /** \brief Get all file filters that are registered
       *
       */
      virtual void getAllFileFilters(QStringList& _filters) {};

  private slots :

    /**  \brief A file has been opened
     *
     *  This slot is called if a file has been opened by the core.\n
     *  This slot will be executed in the main thread.\n
     *  @param _id Id of the new object
     */
    virtual void fileOpened( int _id ) {};

    /**  \brief An empty object has been added
     *
     *  Slot is called if an empty object has been added by the core.\n
     *  This slot will be executed on the main thread.\n
     *  @param _id Id of the new object
     */
    virtual void addedEmptyObject( int _id ) {};

    /** \brief An object was deleted
      *
      * This function is called by the core if an object gets deleted. It is called immediately
      * before the object is removed from the scenegraph. So if this function is invoked, the object still
      * exists. All plugins get informed via this slot.\
      *
      * After this function got called for all plugins, the object is removed from the scene with all
      * nodes attached to it and all PerObjectData attached to it.\n
      *
      * This slot will be executed on the main thread.
      *
      * @param _id Id of the object that is deleted.
      */
    virtual void objectDeleted( int _id ){};

};

/** \page loadSaveInterfacePage Load/Save Interface
 * \n
\image html loadSaveInterface.png
\n

The LoadSaveInterface can be used by plugins to add new objects to the scene either by creating empty objects
or by loading them from files. The interface also triggers saving of existing objects to files.


\section loadSaveInterface_load_save Load/Save Files
You can load a file from within your plugin via
\code
  emit LoadSaveInterface::load(QString _filename, DataType _type, int& _id);
\endcode
and to save an object to a file, call LoadSaveInterface::
\code
  emit LoadSaveInterface::save(int _id , QString _filename );
\endcode

The corresponding File Plugin will than be activated and load or save the object. for load, the id of the new object
is returned.

\section loadSaveInterface_createObjects Creating/Copying Objects
To add objects to the scene you can use the following two functions:
\code
  emit LoadSaveInterface::addEmptyObject( DataType _type, int& _id);
\endcode
This will create a new object of the given DataType and return the id of it, while
\code
  emit LoadSaveInterface::copyObject( int _oldId, int& _newId);
\endcode
will copy the given object and return the new object id.

\section loadSaveInterface_deleteObjects Deleting Objects

You can delete a specific object via
\code
  emit LoadSaveInterface::deleteObject( int _id );
\endcode
or clear the whole scene with:
\code
  emit LoadSaveInterface::deleteAllObjects();
\endcode

\section loadSaveInterface_callbacks Object Information
Additionally the interface informs plugins if new objects have been added to the scenes or if previous objects
have been removed.
LoadSaveInterface::fileOpened( int _id ) will be triggered each time an object is loaded from a file. If simply an
empty object has been added, the slot LoadSaveInterface::addedEmptyObject( int _id ) will be called. If an object
gets deleted, the LoadSaveInterface::objectDeleted( int _id ) will be called immediately before the object is removed
from the scenegraph. So if this function is invoked, the object still exists. All plugins get informed via this slot.

If you want to know if the whole scene got cleared, use BaseInterface::slotAllCleared( ).

\section loadSaveInterface_usage Usage
To use the LoadSaveInterface:
<ul>
<li> include LoadSaveInterface.hh in your plugins header file
<li> derive your plugin from the class LoadSaveInterface
<li> add Q_INTERFACES(LoadSaveInterface) to your plugin class
<li> And add the signals or slots you want to use to your plugin class (You don't need to implement all of them)
</ul>

*/


Q_DECLARE_INTERFACE(LoadSaveInterface,"OpenFlipper.LoadSaveInterface/1.1")

#endif // LOADSAVEINTERFACE_HH
