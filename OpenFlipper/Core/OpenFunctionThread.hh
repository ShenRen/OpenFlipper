#ifndef __OPENFUNCTIONSLOADFROMPLUGINTHREAD_HH__
#define __OPENFUNCTIONSLOADFROMPLUGINTHREAD_HH__

#include <OpenFlipper/threads/OpenFlipperThread.hh>
#include <OpenFlipper/common/DataTypes.hh>
#include <OpenFlipper/BasePlugin/FileInterface.hh>
#include <QString>



class LoadFromPluginThread : public OpenFlipperThread
{
  Q_OBJECT

public:
  struct LoadInfos
   {
     FileInterface* plugin;
     DataType type;
     QString filename;
     LoadInfos():plugin(0),type(DATA_UNKNOWN),filename(){}//c'tor for QVector
     LoadInfos(FileInterface* _plugin, const QString& _filename):plugin(_plugin),type(DATA_UNKNOWN),filename(_filename){}
     LoadInfos(FileInterface* _plugin, DataType _type, const QString& _filename):plugin(_plugin),type(_type),filename(_filename){}
   };

signals:
  void updateView();

private:
  const QVector<LoadInfos>& infos_;
  QVector<int> loadedIds_;

public slots:
  void loadFromPlugin();

public:

  ~LoadFromPluginThread();
  LoadFromPluginThread(QVector<LoadInfos> _loadInfos, const QString& _jobName);


  int getObjId(int index)const;
  const QString& getFilename(int index) const;

};

#endif //__OPENFUNCTIONSLOADFROMPLUGINTHREAD_HH__
