#ifndef ALIGN_MESHES_TOOLBOX_HH
#define ALIGN_MESHES_TOOLBOX_HH

#include "ui_AlignMeshesToolbox.hh"
#include <QtGui>

class AlignMeshesToolbox : public QWidget, public Ui::AlignMeshesToolbox
{
  Q_OBJECT

  public:
    explicit AlignMeshesToolbox(QWidget *parent = 0);
};

#endif // ALIGN_MESHES_TOOLBOX_HH
