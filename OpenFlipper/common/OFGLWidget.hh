
#pragma once


#include <QtGlobal>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 4, 0))
#include <QOpenGLWidget>
#include <QSurfaceFormat>
#include <QOpenGLContext>
typedef QOpenGLWidget OFGLWidget;
typedef QSurfaceFormat OFGLFormat;

/*
class OFGLWidget : public QOpenGLWidget
{
public:
  OFGLWidget(const OFGLFormat& format, QWidget* parent = 0, const QOpenGLWidget* shareWidget = 0, Qt::WindowFlags f = 0)
    : QOpenGLWidget(parent, f)
  {
    setFormat(format);
  }

  void swapBuffers()
  {
    context()->swapBuffers(context()->surface());
  }

  QImage grabFrameBuffer(bool withAlpha)
  {
    return grabFramebuffer();
  }
};
*/

#else
#include <QGLWidget>
#include <QGLFormat>
typedef QGLWidget OFGLWidget;
typedef QGLFormat OFGLFormat;
#endif