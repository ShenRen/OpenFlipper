#include "OpenFunctionThread.hh"
#include <OpenFlipper/common/GlobalOptions.hh>
#include <OpenFlipper/BasePlugin/PluginFunctions.hh>

/////////////////////////////////////////////////
void LoadFromPluginThread::loadFromPlugin()
{
  for ( unsigned int i = 0 ; i < OpenFlipper::Options::examinerWidgets() ; ++i )
    PluginFunctions::viewerProperties(i).lockUpdate();

  for (int i = 0; i < infos_.size(); ++i)
  {
    LoadInfos info = infos_[i];
    if (info.type != DATA_UNKNOWN)
      loadedIds_[i] = info.plugin->loadObject(info.filename,info.type );
    else
      loadedIds_[i] = info.plugin->loadObject(info.filename);
    emit state(this->jobId(),i);
  }
  for ( int i = 0 ; i < PluginFunctions::viewers() ; ++i )
    PluginFunctions::viewerProperties(i).unLockUpdate();
  emit updateView();
}
/////////////////////////////////////////////////
LoadFromPluginThread::~LoadFromPluginThread()
{

}
/////////////////////////////////////////////////
LoadFromPluginThread::LoadFromPluginThread(QVector<LoadInfos> _loadInfos, const QString& _jobName)
: OpenFlipperThread(_jobName), infos_(_loadInfos), loadedIds_(_loadInfos.size(),-1)
{
  connect( this ,SIGNAL(function()), this, SLOT(loadFromPlugin()), Qt::DirectConnection) ;
}
/////////////////////////////////////////////////
int LoadFromPluginThread::getObjId(int index)const
{
  return loadedIds_[index];
}
/////////////////////////////////////////////////
const QString& LoadFromPluginThread::getFilename(int index) const
{
  return infos_[index].filename;
}

