//=============================================================================
//
//                               OpenFlipper
//        Copyright (C) 2008 by Computer Graphics Group, RWTH Aachen
//                           www.openflipper.org
//
//-----------------------------------------------------------------------------
//
//                                License
//
//  OpenFlipper is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
// 
//  OpenFlipper is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
// 
//  You should have received a copy of the GNU Lesser General Public License
//  along with OpenFlipper.  If not, see <http://www.gnu.org/licenses/>.
//
//-----------------------------------------------------------------------------
//
//   $Revision$
//   $Author$
//   $Date$
//
//=============================================================================

#include <QtGui>
#include <QBrush>

#include "TreeModel.hh"

#include <iostream>


#include <OpenFlipper/common/Types.hh>
#include "../OpenFlipper/BasePlugin/PluginFunctions.hh"


//******************************************************************************

/** \brief Constructor
 * 
 * @param _parent parent Object
 */
TreeModel::TreeModel( QObject *_parent) : QAbstractItemModel(_parent)
{
  rootItem_ = PluginFunctions::objectRoot();
}


//******************************************************************************

/** \brief Destructor
 * 
 */
TreeModel::~TreeModel()
{

}


//******************************************************************************

/** \brief Return the number of columns
 * 
 * @param unused
 * @return return always 4
 */
int TreeModel::columnCount(const QModelIndex &/*_parent*/) const
{
  // Name,Visible,Source,Target -> 4
  return (4);
}


//******************************************************************************

/** \brief Returns the data stored under the given role for the item referred to by the index
 * 
 * @param index a ModelIndex that defines the item in the tree
 * @param role defines the kind of data requested
 * @return requested data
 */
QVariant TreeModel::data(const QModelIndex &index, int role) const
{

  // Skip invalid requests
  if (!index.isValid())
      return QVariant();
  
  // Get the corresponding tree item
  BaseObject *item = static_cast<BaseObject*>(index.internalPointer());
  
  if ( item == rootItem_ ) {
    std::cerr << "Root" << std::endl; 
  }
    
  // Set the background color of the objects row
  if ( role == Qt::BackgroundRole ) {
    if ( !item->visible() ) {
      return QVariant( QBrush(QColor(100,100,100) ) ); 
    }
  }

  switch (index.column() ) {
    // Name
    case 0 :
      
      if (role != Qt::DisplayRole && role != Qt::EditRole )
        return QVariant();
      
      return QVariant(item->name());
      
      break;
    // Visible
    case 1 :
      if (role == Qt::CheckStateRole ) {
        bool visibility = false;
        // visiblity group
        if (item->isGroup()){
          QList< BaseObject* > children = item->getLeafs();
          bool initRound = true;
          for (int i=0; i < children.size(); i++){
            if (initRound){
              visibility = children[i]->visible();
              initRound = false;
            }else if (visibility != children[i]->visible())
              return QVariant(Qt::PartiallyChecked);
          }
        }else
          //visibility item
          visibility = item->visible();
        if (visibility) 
          return QVariant(Qt::Checked);
        else
          return QVariant(Qt::Unchecked);
      }
      return QVariant();
      
      break;
    // Source
    case 2 :
      if (role == Qt::CheckStateRole ) {
        bool source = false;
        // source group
        if (item->isGroup()){
          QList< BaseObject* > children = item->getLeafs();
          
          bool initRound = true;
          for (int i=0; i < children.size(); i++){
            if (initRound){
              source = children[i]->source();
              initRound = false;
            }else if (source != children[i]->source())
              return QVariant(Qt::PartiallyChecked);
          }
        }else
          //source item
          source = item->source();
        if (source) 
          return QVariant(Qt::Checked);
        else
          return QVariant(Qt::Unchecked);
      }
      return QVariant();

    // Target    
    case 3 :
      if (role == Qt::CheckStateRole ) {
        bool target = false;
        // target group
        if (item->isGroup()){
          QList< BaseObject* > children = item->getLeafs();

          bool initRound = true;
          for (int i=0; i < children.size(); i++){
            if (initRound){
              target = children[i]->target();
              initRound = false;
            }else if (target != children[i]->target())
              return QVariant(Qt::PartiallyChecked);
          }
        }else
          //target item
          target = item->target();
        if (target) 
          return QVariant(Qt::Checked);
        else
          return QVariant(Qt::Unchecked);
      }
      return QVariant();

      break;
      
    default:
      return QVariant();
  }

}


//******************************************************************************

/** \brief Returns the item flags for the given index
 * 
 * @param index ModelIndex that defines an item in the tree
 * @return flags for the given ModelIndex
 */
Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    Qt::ItemFlags flags = 0;

    // Show/Source/Target
    if ( ( index.column() == 1 ) || 
         ( index.column() == 2 ) ||
         ( index.column() == 3 ) )
      flags = Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    else
    if ( index.column() == 0 )
      flags = Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    else
      flags = Qt::ItemIsEnabled;

  // Get the corresponding tree item
  BaseObject *item = static_cast<BaseObject*>(index.internalPointer());

  if ( item->isGroup() )
    return flags | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
  else
    return flags | Qt::ItemIsDragEnabled;
}


//******************************************************************************

/** \brief Returns the data in the header
 * 
 * @param section the column in the header
 * @param orientation header orientation (only horizontal handled)
 * @param role the role that defines the type of data
 * @return the requested data
 */
QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                                int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
      
      switch (section) {
        case 0 : return QVariant("Name");
        case 1 : return QVariant("Show");
        case 2 : return QVariant("Source");
        case 3 : return QVariant("Target");
        default :
          return QVariant();
      }
    }
    return QVariant();
}


//******************************************************************************

/** \brief Returns the index of the item in the model specified by the given row, column and parent index.
 * 
 * @param row the row 
 * @param column the column
 * @param _parent parent item
 * @return corresponding ModelIndex
 */
QModelIndex TreeModel::index(int row, int column, const QModelIndex &_parent) const
{
    if (!hasIndex(row, column, _parent))
        return QModelIndex();

    BaseObject *parentItem;

    if (!_parent.isValid())
        parentItem = rootItem_;
    else
        parentItem = static_cast<BaseObject*>(_parent.internalPointer());

    BaseObject *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}


//******************************************************************************

/** \brief Return index of parent item
 * 
 * @param index a ModelIndex
 * @return parent of the given ModelIndex
 */
QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    BaseObject *childItem = static_cast<BaseObject*>(index.internalPointer());
    BaseObject *parentItem = childItem->parent();

    if (parentItem == rootItem_)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}


//******************************************************************************

/** \brief Returns the number of rows under given parent
 * 
 * @param _parent parent Item
 * @return number of rows that are children of given parent
 */
int TreeModel::rowCount(const QModelIndex &_parent) const
{
    BaseObject *parentItem;
    if (_parent.column() > 0)
        return 0;

    if (!_parent.isValid())
        parentItem = rootItem_;
    else
        parentItem = static_cast<BaseObject*>(_parent.internalPointer());

    return parentItem->childCount();
}


//******************************************************************************

/** \brief The object with the given id has been updated. Update the model.
 * 
 * @param _id  id of an object
 */
void TreeModel::updatedObject(int _id) {

  if ( _id != -1 ){

    BaseObject* obj = 0;
    PluginFunctions::getObject(_id, obj);

    if (obj != 0 && getModelIndex(obj,0).isValid() ){

//       //set new values for the object
//       QVector< QVariant > values;
// 
//       values.push_back( obj->name()    );
//       values.push_back( obj->visible() );
//       values.push_back( obj->source()  );
//       values.push_back( obj->target()  );
// 
//       for(int i=0; i < 4; i++)
//         setData( getModelIndex(obj,i), values[i], 0);
// 
      return;
    }
  }

  //otherwise reset the model
  reset();
}


//******************************************************************************

/** \brief Return item at given index
 * 
 * @param index a ModelIndex
 * @return item at given index
 */
BaseObject* TreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        BaseObject *item = static_cast<BaseObject*>(index.internalPointer());
        if (item) return item;
    }
    return rootItem_;
}


//******************************************************************************

/** \brief Return index of given item
 * 
 * @param _object an object
 * @param _column a column
 * @return index of object and column
 */
QModelIndex TreeModel::getModelIndex(BaseObject* _object, int _column ){

  for (int i=0; i < persistentIndexList().count(); i++){
    BaseObject* tmp = static_cast<BaseObject*>(persistentIndexList().at(i).internalPointer());
    if ( tmp == _object && persistentIndexList().at(i).column() == _column )
      return persistentIndexList().at(i);
  }

  return QModelIndex();
}


//******************************************************************************

/** \brief Recursively update source selection up to the root of the tree
 * 
 * @param _obj object to start with 
 */
void TreeModel::updateSourceSelection(BaseObject* _obj ){

  if ( isRoot(_obj) || (!_obj->isGroup()) )
    return;

  QList< BaseObject* > children = _obj->getLeafs();

  bool source = false;
  for (int i=0; i < children.size(); i++)
    source |= children[i]->source();

  _obj->source( source );

  updateSourceSelection( _obj->parent() );
}


//******************************************************************************

/** \brief Recursively update target selection up to the root of the tree
 * 
 * @param _obj object to start with
 */
void TreeModel::updateTargetSelection(BaseObject* _obj ){

  if ( isRoot(_obj) || (!_obj->isGroup()) )
    return;

  QList< BaseObject* > children = _obj->getLeafs();

  bool target = false;
  for (int i=0; i < children.size(); i++)
    target |= children[i]->target();

  _obj->target( target );

  updateTargetSelection( _obj->parent() );
}


//******************************************************************************

/** \brief Set Data at 'index' to 'value'
 * 
 * @param index a ModelIndex defining the positin in the model
 * @param value the new value
 * @param  unused
 * @return return if the data was set successfully
 */
bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int /*role*/)
{
  BaseObject *item = getItem(index);
  
  if ( item->isGroup() ) {
    int childCount = 0;
    // Decide on column what to do with the value
    switch ( index.column() ) {
      // Name
      case 0 :
        if ( value.toString() == "" ) 
          return false; 
        
        item->setName( value.toString() );
        break;
        
       // visible
      case 1 : 
        if ( value.toInt() == Qt::Unchecked ) {
                 
          QList< BaseObject* > children = item->getLeafs();
          
          childCount = children.size();

          for (int i=0; i < children.size(); i++){
              BaseObjectData* child = dynamic_cast< BaseObjectData* > (children[i]);
              if (child)
                child->hide();
          }
        }
      
        if ( value.toInt() == Qt::Checked ) {
          
          QList< BaseObject* > children = item->getLeafs();
          
          childCount = children.size();

          for (int i=0; i < children.size(); i++){
            BaseObjectData* child = dynamic_cast< BaseObjectData* > (children[i]);
            if (child)
              child->show();
          }
        }
      
        break;  
      
      // source
      case 2 : 
        if ( value.toInt() == Qt::Unchecked ) {

          item->source(false);
                 
          QList< BaseObject* > children = item->getLeafs();

          childCount = children.size();
          
          for (int i=0; i < children.size(); i++){
              if (children[i])
                children[i]->source(false);
          }
        }
      
        if ( value.toInt() == Qt::Checked ) {

          item->source(true);
          
          QList< BaseObject* > children = item->getLeafs();
          
          childCount = children.size();

          for (int i=0; i < children.size(); i++){
              if (children[i])
                children[i]->source(true);
          }
        } 
        break;
      
      //target
      case 3 : 
        if ( value.toInt() == Qt::Unchecked ) {
                 
          item->target(false);

          QList< BaseObject* > children = item->getLeafs();

          childCount = children.size();
          
          for (int i=0; i < children.size(); i++){
              if (children[i])
                children[i]->target(false);
          }
        }
      
        if ( value.toInt() == Qt::Checked ) {

          item->target(true);

          QList< BaseObject* > children = item->getLeafs();
          
          childCount = children.size();

          for (int i=0; i < children.size(); i++){
              if (children[i])
                children[i]->target(true);
          }
        }
        break;
    }

    if (childCount > 0){
      BaseObject* obj = item->getLeafs()[childCount-1];

      QModelIndex childIndex = createIndex(obj->row(),index.column() , obj);

      emit dataChanged( index,childIndex );
    }else
      emit dataChanged( index, index );

    return true; 
  }

  // <- Item is not a group

  bool checked;

  // Decide on column what to do with the value ( abort if anything goes wrong)
  switch ( index.column() ) {
    // Name :
    case 0 :
      if ( value.toString() == "" ) 
        return false; 
      item->setName(value.toString());
      break;
      
    // visible
    case 1 : 
      if ( value.toInt() == Qt::Unchecked ) {
        
        BaseObjectData* baseObject = dynamic_cast< BaseObjectData* >(item);
        
        if ( baseObject )
          baseObject->hide();
        else 
          std::cerr << " Trying to hide invisible object e.g. Group " << std::endl;
      }
      
      if ( value.toInt() == Qt::Checked ) {
        BaseObjectData* baseObject = dynamic_cast< BaseObjectData* >(item);
        
        if ( baseObject )
          baseObject->show();
        else
          std::cerr << " Trying to show invisible object e.g. Group " << std::endl;
      }
      
      break;      
      
    // source
    case 2 :

      checked = value.toBool();

      item->source( checked );

      updateSourceSelection( item->parent() );

      break;

    //target
    case 3 : 

      checked = value.toBool();

      item->target( checked );

      updateTargetSelection( item->parent() );

      break;
    
    default:
      
      break;
      
  }
  
  emit dataChanged(index,index);
  //force everything to be updated (needed to updated parent groups)
  emit dataChanged(QModelIndex(),QModelIndex());
  return true;
}


//******************************************************************************

/** \brief return if an object is equal to the root object
 * 
 * @param _item the item to be checked
 * @return is it the root object?
 */
bool TreeModel::isRoot(BaseObject * _item) {
  return ( _item == rootItem_ ); 
}

/*******************************************************************************
        drag & drop stuff
 *******************************************************************************/

/** \brief return the supported drop actions
 * 
 * @return drop actions
 */
Qt::DropActions TreeModel::supportedDropActions() const
{
    return /*Qt::CopyAction |*/ Qt::MoveAction;
}


//******************************************************************************

/** \brief return the mimeType for drag & drop
 * 
 * @return the mimeType
 */
QStringList TreeModel::mimeTypes() const
{
    QStringList types;
    types << "DataControl/dragDrop";
    return types;
}


//******************************************************************************

/** \brief generate mimeData for given ModelIndexes
 * 
 * @param indexes list of ModelIndexes
 * @return the mimeData
 */
QMimeData* TreeModel::mimeData(const QModelIndexList& indexes) const
{
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    QVector< int > rows;

    foreach (QModelIndex index, indexes) {
        if (index.isValid()) {

          if (!rows.contains( index.row() ) ){
            BaseObject *item = getItem(index);
            stream << item->id();

            rows.push_back( index.row() );
          }
        }
    }

    mimeData->setData("DataControl/dragDrop", encodedData);
    return mimeData;
}


//******************************************************************************

/** \brief this is called when mimeData is dropped
 * 
 * @param data the dropped data
 * @param action the definition of the dropAction which occured
 * @param unused
 * @param unused 
 * @param parent parent under which the drop occurred 
 * @return returns if the drop was sucessful
 */
bool TreeModel::dropMimeData(const QMimeData *data,
     Qt::DropAction action, int /*row*/, int /*column*/, const QModelIndex &parent)
 {
     if (action == Qt::IgnoreAction)
         return true;

     if (!data->hasFormat("DataControl/dragDrop"))
         return false;

     QByteArray encodedData = data->data("DataControl/dragDrop");
     QDataStream stream(&encodedData, QIODevice::ReadOnly);

     QVector< int > ids;

     while (!stream.atEnd()) {
       int id;
       stream >> id;

       ids.push_back( id ); 
     }

    if (ids.count() == 0)
      return false;

      //get new parent

      BaseObject *newParent = getItem(parent);
  
      if ( newParent == 0 || !newParent->isGroup() )
        return false;

      //and move all objects
      for (int i = 0; i < ids.count(); i++){
 
        BaseObject* item = 0;
        if (PluginFunctions::getObject(ids[i], item)){
    
          item->parent()->removeChild(item);
  
          //if parent was group and is empty now ->delete it
          if ( !isRoot( item->parent() ) &&
              item->parent()->isGroup() && item->parent()->childCount() == 0){
  
            // remove the parent itself from the parent
            item->parent()->parent()->removeChild(item->parent());
  
            // delete it
            delete item->parent();
  
          }
  
          item->setParent( newParent  );
          newParent->appendChild( item );
        }
      }

  reset();
  return true;
 }
