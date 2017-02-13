#include "ui_toolbox.hh"
#include <QtGui>

class MergeToolBox : public QWidget, public Ui::MergeTool
{
  Q_OBJECT

  public:
    MergeToolBox(QWidget *parent = 0);
};

