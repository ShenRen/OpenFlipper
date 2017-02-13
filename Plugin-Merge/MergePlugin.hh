#ifndef MERGEPLUGIN_HH
#define MERGEPLUGIN_HH

#include <QObject>
#include <QtGui>
#include <QCheckBox>
#include <QMenuBar>

#ifdef CGAL_AUTOLINK
  #include <CGAL/auto_link/LAPACK.h>
#endif

#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/ToolboxInterface.hh>
#include <OpenFlipper/BasePlugin/LoadSaveInterface.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>
#include <OpenFlipper/BasePlugin/ScriptInterface.hh>
#include <OpenFlipper/common/Types.hh>
#include <Math_Tools/PCA/PCA.hh>

#include <ObjectTypes/PolyMesh/PolyMesh.hh>
#include <ObjectTypes/TriangleMesh/TriangleMesh.hh>

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
  void updatedObject(int _identifier, const UpdateType _type);

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
  int mergeObjects(const std::vector< BaseObjectData* > _objects, QString _name = "merged object", bool _deleteSeparateObjects = true);
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

#if defined(INCLUDE_TEMPLATES) && !defined(MERGEPLUGIN_C)
#define MERGEPLUGIN_TEMPLATES
#include "MergePluginT.cc"
#endif

#endif //MERGEPLUGIN_HH
