#ifndef MERGEPLUGIN_HH
#define MERGEPLUGIN_HH

#include <QObject>

#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/ToolboxInterface.hh>
#include <OpenFlipper/BasePlugin/LoadSaveInterface.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>
#include <OpenFlipper/BasePlugin/ScriptInterface.hh>

#include "MergeToolbox.hh"

class MergePlugin : public QObject, BaseInterface, ToolboxInterface, LoggingInterface, LoadSaveInterface, ScriptInterface
{
Q_OBJECT
Q_INTERFACES(BaseInterface)
Q_INTERFACES(ToolboxInterface)
Q_INTERFACES(LoggingInterface)
Q_INTERFACES(LoadSaveInterface)
Q_INTERFACES(ScriptInterface)
#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "org.OpenFlipper.Plugins.Plugin-Merge")
#endif



signals:
  //BaseInterface
  void updateView();
  void updatedObject(int _identifier, const UpdateType& _type);

  //LoggingInterface:
  void log( Logtype _type, QString _message );
  void log( QString _message );

  // LoadSaveInterface
  void addEmptyObject( DataType _type, int &_objectId );
  void deleteObject( int _id );
  
  // ToolboxInterface
  void addToolbox( QString _name  , QWidget* _widget );

  // ScriptInterface
  void setSlotDescription(QString     _slotName,   QString     _slotDescription,
                          QStringList _parameters, QStringList _descriptions);

  void cleanup( DataType _type, bool _deleteSeparateObjects);

public :

  MergePlugin();
  ~MergePlugin() {};

  QString name() { return (QString("Merge")); };
  QString description( ) { return (QString("Merge target objects")); };

private :
  MergeToolBox* tool_;
  std::vector<int> convertedIds;
  std::vector< BaseObjectData* > objects;
  int polyMergeID, triMergeID;

  DataType checkType(const std::vector< BaseObjectData* > &);

public slots:
  // BaseInterface
  void initializePlugin();
  void pluginsInitialized();
  
  void mergeObjects();

  /**
   * @brief mergeObjects merges multiple OpenFlipper Objects of Type TriMesh or PolyMesh into one combined Mesh.
   * @param _objects a vector of OpenFlipper Objects that shall be merged.
   * @param _name The name of the merged object (default is "merged object")
   * @param _deleteSeparateObjects flag to determine if the separate objects shall be deleted after merging (default is true)
   * @return
   */
  int mergeObjects(const std::vector< BaseObjectData* > & _objects, QString _name = "merged object", bool _deleteSeparateObjects = true);

  /**
   * @brief slotCleanup is called when the cleanup event is processed at the end of mergeObjects.
   * @param _type the datatype that was used for merging
   * @param _deleteSeparateObjects flag to determine if separated objects shall be deleted.
   */
  void slotCleanup(DataType _type, bool _deleteSeparateObjects);

//template functions
private:

  template< class MeshT >
  void mergeMeshes( const std::vector< MeshT* >& _meshes );

  template< class MeshT >
  void mergeMeshes( const std::vector< MeshT* >& _meshes, typename MeshT::VertexHandle& _vhB);

  public slots:
    QString version() { return QString("1.1"); };

};

//#if defined(INCLUDE_TEMPLATES) && !defined(MERGEPLUGIN_C)
//#define MERGEPLUGIN_TEMPLATES
//#include "MergePluginT.cc"
//#endif

#endif //MERGEPLUGIN_HH
