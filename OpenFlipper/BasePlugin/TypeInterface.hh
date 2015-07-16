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


#ifndef TYPEINTERFACE_HH
#define TYPEINTERFACE_HH

#include <OpenFlipper/common/Types.hh>

/** \file TypeInterface.hh
*
* Interface for registering types in OpenFlipper. \ref typeInterfacePage
*/


 /** \brief Interface class for type definitions
 *
 * \n
 * \ref typeInterfacePage "Detailed description"
 * \n
 *
 * This interface is used to register new types in OpenFlipper. The type plugins are loaded before all other plugins.
 * They have only the registerType function which registers the type to the core and a function to add new
 * objects at runtime.
 */

class TypeInterface {

  signals:

    /** \brief Emit this signal if an empty object has been created
     *
     * @param _id Id of the added object
     */
    virtual void emptyObjectAdded( int _id ) {};

  public:

    /// Destructor
    virtual ~TypeInterface() {};

  public slots:

    virtual bool registerType() = 0;
    
    /** \brief Create an empty object
       *
       * When this slot is called you have to create an object of your supported type.
       * @return Id of the new Object
       */
    virtual int addEmpty() = 0;
    
    /** \brief Return your supported object type( e.g. DATA_TRIANGLE_MESH )
    *
    * The function is used from addEmpty in the core to check if your plugin can create an object of
    * a given dataType. If so, your addEmpty function will be invoked to create it.
    */
    virtual DataType supportedType() = 0;

    /** \brief This slot should be implemented in a TypePlugin to generate type specific backups
     *
     * @param _id Id of the added object
     * @param _name name of the backup
     * @param _type the type of backup that needs to be done
     */
    virtual void generateBackup( int _id, QString _name, UpdateType _type ){};

};


/** \page typeInterfacePage Type Interface
\n
\image html TypeInterface.png
\n

This interface is used to register new types in OpenFlipper. The type plugins are loaded before all other plugins.
They have only the registerType function which registers the type to the core and a function to create objects
of the new type. The type itself has to be defined in the ObjectTypes subdirectory.

 \section TypeExample Example using custom data types

 Adding a custom data type to %OpenFlipper needs the following requirements in order to work:

 - The definition of the typeId constant, e.g.:
 \code
 #define DATA_MY_DATA typeId("MyDataType")
 \endcode
 Note: Your data type is then referenced as DATA_MY_DATA during runtime.
 - The specification of an object class for your object type that is derived from
   BaseObjectData.
 - The specification of helper functions (usually within the PluginFunctions namespace)
   allowing the casting from BaseObjectData to your object type class.

 See detailed examples for each of the three points for already existing data types in
 OpenFlipperRoot/ObjectTypes.

 Once the object class is specified, the type plugin will be responsible for its handling including

 - Adding new objects to the scenegraph
 - Setting the initial name of an object
 - Etc.

 So, type plugins usually consist of only few lines of code. Here an example of
 a type plugin handling an example object data type as mentioned above:

 \code
 bool MyDataTypePlugin::registerType() {

     addDataType("MyDataType",tr("MyDataType"));
     setTypeIcon( "MyDataType", "myDataType.png");

     return true;
 }

 int MyDataTypePlugin::addEmpty() {

     // Create new object
     MyObject* object = new MyObject();

     object->setName( QString("My Object %1.mob").arg(objectCount) );
     object->update();
     object->show();

     // Tell core that an object has been added
     emit emptyObjectAdded ( object->id() );

     return object->id();
 }
 \endcode

 Now, each time a plugin emits addEmptyObject(DATA_MY_DATA), the addEmpty() function will
 add the object to the scenegraph and return the newly created object's id.

 \section Backups
 Backups are very specific to the underlying data structures of certain types. Therefore the type
 plugins also manage backups. Backups itself are managed by perObjectData objects based on
 the BackupData class.
 When the slot gets called, we first have to check, if an BackupData is already available. If not,
 we create one. Than the backup is generated and passed to the backup data attached to the object.

 You have to derive your backups from the BaseBackup class, where you only need to implement the
 apply function and your constructor. In the following example, this class is named TriMeshBackup.

 \code
 void TypeTriangleMeshPlugin::generateBackup( int _id, QString _name, UpdateType _type ){

  // Get the object corresponding to the id
  BaseObjectData* object = 0;
  PluginFunctions::getObject(_id, object);
  TriMeshObject* meshObj = PluginFunctions::triMeshObject(object);

  // Safety check
  if ( meshObj != 0 ){

    //get backup object data
    BackupData* backupData = 0;

    // If a backup data has already been attached, get it, otherwise create it.
    if ( object->hasObjectData( OBJECT_BACKUPS ) )
      backupData = dynamic_cast< BackupData* >(object->objectData(OBJECT_BACKUPS));
    else{
      //add backup data
      backupData = new BackupData(object);
      object->setObjectData(OBJECT_BACKUPS, backupData);
    }

    // Create a new backup
    TriMeshBackup* backup = new TriMeshBackup(meshObj, _name, _type);

    // Store it in the backup data
    backupData->storeBackup( backup );
  }
 }
 \endcode

To use the TypeInterface:
<ul>
<li> include TypeInterface.hh in your plugins header file
<li> derive your plugin from the class TypeInterface
<li> add Q_INTERFACES(TypeInterface) to your plugin class
<li> Implement all slots of this Interface
</ul>

*/





Q_DECLARE_INTERFACE(TypeInterface,"OpenFlipper.TypeInterface/1.1")

#endif // TYPEINTERFACE_HH
