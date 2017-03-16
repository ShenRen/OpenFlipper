
//#include <QtGui>

#include "MergePlugin.hh"

#include "OpenFlipper/BasePlugin/PluginFunctions.hh"
#include <OpenFlipper/BasePlugin/RPCWrappers.hh>

#include <ObjectTypes/PolyMesh/PolyMesh.hh>
#include <ObjectTypes/TriangleMesh/TriangleMesh.hh>

/**
 * Helper functions
 */
namespace
{
  template< class MeshT >
  void getAllMeshes(std::vector< MeshT* > & meshes, const std::vector< BaseObjectData* > & objects)
  {
    for (uint i=0; i < objects.size(); i++)
    {
      MeshT* t;
      if(PluginFunctions::getMesh(objects[i]->id(),t))
        meshes.push_back( t );
    }
  }

  void getTargets(std::vector< BaseObjectData* > & _objects)
  {
    _objects.clear();
    //read all target objects
    for ( PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS,DataType(DATA_TRIANGLE_MESH | DATA_POLY_MESH)) ;
                                          o_it != PluginFunctions::objectsEnd(); ++o_it)
        _objects.push_back( *o_it );
  }

  template< class MeshT >
  void convertMeshes(const DataType & _type, std::vector< int >& convertedIds, std::vector< MeshT* >& _meshes)
  {
    for ( PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS,_type) ;
                                          o_it != PluginFunctions::objectsEnd(); ++o_it)
    {
        //convert polyMesh to triMesh or vice versa
        convertedIds.push_back(RPC::callFunctionValue<int>("meshconvert","convert",(*o_it)->id(), _type != DATA_TRIANGLE_MESH));
        MeshT* ptr;
        PluginFunctions::getMesh(convertedIds.back(),ptr);
        _meshes.push_back(ptr);
    }
  }
}

void MergePlugin::slotCleanup( DataType _type, bool _deleteSeparateObjects )
{
  //clean up conversion objects
  for(int i : convertedIds)     
     emit deleteObject( i );

  convertedIds.clear();

  if(_deleteSeparateObjects)
  {
    //clean up separated objects
    for (size_t i=0; i < objects.size(); i++)
      emit deleteObject( (objects[i])->id() );

    objects.clear();
  }
  //clean up unused merge target
  if(_type == DATA_TRIANGLE_MESH)
    emit deleteObject( polyMergeID );
  else
    emit deleteObject( triMergeID );
}

DataType MergePlugin::checkType(const std::vector< BaseObjectData* > & objects )
{
  DataType type = (objects[0])->dataType();
  bool askForType = false;
  for (uint i=1; i < objects.size(); i++)
    if ( type != (objects[i])->dataType() ){
      askForType = true;
      break;
    }

  if(askForType)
  {
    QStringList types;
    types.append(dataTypeName(DATA_TRIANGLE_MESH));
    types.append(dataTypeName(DATA_POLY_MESH));
    bool ok;
    QString result = QInputDialog::getItem(nullptr,
                          tr("Select Mesh Type"),
                          tr("Convert meshes to:"),
                          types,
                          1,
                          false,
                          &ok);
    if(ok)
    {
      if( result == dataTypeName(DATA_POLY_MESH))
      {
        type = DATA_POLY_MESH;        
      }
      else
      {
        type = DATA_TRIANGLE_MESH;        
      }
    }
    else
      return DataType();
  }
  return type;
}


MergePlugin::MergePlugin() :
  tool_(nullptr),
  polyMergeID(0),
  triMergeID(0)
{

}

/// init the Toolbox
void MergePlugin::initializePlugin() {
   tool_ = new MergeToolBox();

   QSize size(300, 300);
   tool_->resize(size);

   connect(tool_->mergeButton, SIGNAL( clicked() ), this, SLOT( mergeObjects() ) );
   tool_->mergeButton->setStatusTip("Merge all target objects into one without changing geometry");
   tool_->mergeButton->setToolTip( tool_->mergeButton->statusTip() );
   QIcon* toolIcon_ = new QIcon(OpenFlipper::Options::iconDirStr()+OpenFlipper::Options::dirSeparator()+"merge.png");

   connect(this,SIGNAL(cleanup(DataType, bool)),this,SLOT(slotCleanup(DataType, bool)),Qt::QueuedConnection);

   emit addToolbox( tr("Merge") , tool_, toolIcon_ );
}

void MergePlugin::pluginsInitialized()
{
  //populate scripting function
  emit setSlotDescription("mergeObjects(const std::vector< BaseObjectData* >,QString,bool)", "Merges multiple meshes into one mesh. returns the ID of the new mesh or -1 in case of error.",
                          QString("objects,mergedName,deleteSeparateObjects").split(","),
                          QString(" vector of BaseObjectData* containing Poly or TriMeshes to be merged, name for the merged object, flag to remove separated objects default is true").split(","));
}

int MergePlugin::mergeObjects(const std::vector< BaseObjectData* > & _objects, QString _name, bool _deleteSeparateObjects)
{
  int result = -1;
  if (_objects.size() < 2)
    return -1; //nothing to do

  objects = _objects;

  //check dataType
  DataType type = checkType(objects);

  //user pushed the cancel button
  if(type != DATA_TRIANGLE_MESH && type != DATA_POLY_MESH)
    return -1;

  convertedIds.clear();
  std::vector< TriMesh* > triMeshes;
  std::vector< PolyMesh* > polyMeshes;
  TriMesh* triMergePtr;
  PolyMesh* polyMergePtr;

  emit addEmptyObject(DATA_POLY_MESH, polyMergeID);
  PluginFunctions::getMesh(polyMergeID, polyMergePtr);
  polyMeshes.push_back(polyMergePtr);
  emit addEmptyObject(DATA_TRIANGLE_MESH, triMergeID);
  PluginFunctions::getMesh(triMergeID,triMergePtr);
  triMeshes.push_back(triMergePtr);

  getAllMeshes(triMeshes,objects);
  getAllMeshes(polyMeshes,objects);

  if ( type == DATA_TRIANGLE_MESH ) {
    // Convert PolyMeshes to TriMeshes
    convertMeshes(DATA_POLY_MESH,convertedIds,triMeshes);

    *triMergePtr = *triMeshes[1];
    mergeMeshes(triMeshes);
    (triMeshes[0])->update_normals();
    BaseObject* bo;
    PluginFunctions::getObject(triMergeID,bo);
    bo->setName(_name);
    result = bo->id();
  } else {
    // Convert TriMeshes to PolyMeshes
    convertMeshes(DATA_TRIANGLE_MESH,convertedIds,polyMeshes);

    *polyMergePtr = *polyMeshes[1];
    mergeMeshes(polyMeshes);
    (polyMeshes[0])->update_normals();
    BaseObject* bo;
    PluginFunctions::getObject(polyMergeID,bo);
    bo->setName(_name);
    result = bo->id();
  }

  emit updatedObject(triMergeID,UPDATE_ALL);
  emit updatedObject(polyMergeID,UPDATE_ALL);

  //clean up after merging (removes OF objects)
  emit cleanup( type, _deleteSeparateObjects );
  return result;
}

/// merge two objects with target flag
void MergePlugin::mergeObjects()
{
  getTargets(objects);
  mergeObjects(objects,tool_->mergedName->text(),tool_->deleteObjects->isChecked());
}


#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2( mergePlugin , MergePlugin );
#endif

