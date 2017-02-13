#include "ui_toolbox.hh"
#include <QtGui>

class MergeToolBox : public QWidget, public Ui::MergeTool
{
  Q_OBJECT

  public:
    explicit MergeToolBox(QWidget *parent = 0);
};

