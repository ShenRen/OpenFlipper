#ifndef PLUGINALIGNMESHES_HH
#define PLUGINALIGNMESHES_HH

#include <QtGui>

#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>
#include <OpenFlipper/BasePlugin/ToolboxInterface.hh>

#include <OpenFlipper/common/Types.hh>

#include <OpenFlipper/BasePlugin/PluginFunctions.hh>

#include <ObjectTypes/TriangleMesh/TriangleMesh.hh>
#include <ObjectTypes/PolyMesh/PolyMesh.hh>

#include "Widgets/AlignMeshesToolbox.hh"

class PluginAlignMeshes :
    public QObject,
    BaseInterface,
    LoggingInterface,
    ToolboxInterface {
Q_OBJECT
Q_INTERFACES(BaseInterface)
Q_INTERFACES(ToolboxInterface)
Q_INTERFACES(LoggingInterface)

#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "org.OpenFlipper.Plugins.Plugin-AlignMeshes")
#endif

public:

  PluginAlignMeshes();

  ~PluginAlignMeshes();

  // BaseInterface
  QString name() { return QString("PluginAlignMeshes"); }
  QString description() { return tr("Aligns meshes"); }

signals:
  // BaseInterface
  void updateView();
  void updatedObject(int, const UpdateType);
  void nodeVisibilityChanged(int _identifier);
  void setSlotDescription(QString     _slotName,   QString     _slotDescription,
                          QStringList _parameters, QStringList _descriptions);

  // LoggingInterface
  void log(Logtype _type, QString _message);
  void log(QString _message);

  // ToolboxInterface
  void addToolbox(QString _name, QWidget* _widget);

public slots:

  // BaseInterface
  QString version() { return QString("1.0"); }

  void alignMeshes();

private slots:

  // BaseInterface
  void initializePlugin();
  void pluginsInitialized();

private:
  void moveToMean(TriMeshObject& _object);
  void rotate(TriMeshObject& _object);

private:
  AlignMeshesToolbox* toolBox_;
};

#endif // PLUGINALIGNMESHES_HH
