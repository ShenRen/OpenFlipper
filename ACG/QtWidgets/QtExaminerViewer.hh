/*===========================================================================*\
 *                                                                           *
 *                              OpenFlipper                                  *
 *           Copyright (c) 2001-2015, RWTH-Aachen University                 *
 *           Department of Computer Graphics and Multimedia                  *
 *                          All rights reserved.                             *
 *                            www.openflipper.org                            *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * This file is part of OpenFlipper.                                         *
 *---------------------------------------------------------------------------*
 *                                                                           *
 * Redistribution and use in source and binary forms, with or without        *
 * modification, are permitted provided that the following conditions        *
 * are met:                                                                  *
 *                                                                           *
 * 1. Redistributions of source code must retain the above copyright notice, *
 *    this list of conditions and the following disclaimer.                  *
 *                                                                           *
 * 2. Redistributions in binary form must reproduce the above copyright      *
 *    notice, this list of conditions and the following disclaimer in the    *
 *    documentation and/or other materials provided with the distribution.   *
 *                                                                           *
 * 3. Neither the name of the copyright holder nor the names of its          *
 *    contributors may be used to endorse or promote products derived from   *
 *    this software without specific prior written permission.               *
 *                                                                           *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       *
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED *
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A           *
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER *
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,  *
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,       *
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR        *
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF    *
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING      *
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS        *
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.              *
 *                                                                           *
\*===========================================================================*/

/*===========================================================================*\
 *                                                                           *
 *   $Revision$                                                       *
 *   $Author$                                                      *
 *   $Date$                   *
 *                                                                           *
\*===========================================================================*/



//=============================================================================
//
//  CLASS QtExaminerViewer
//
//=============================================================================

#ifndef ACG_QTEXAMINERVIEWER_HH
#define ACG_QTEXAMINERVIEWER_HH


//== INCLUDES =================================================================


#include "QtBaseViewer.hh"

#include <QTime>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>


//== NAMESPACES ===============================================================


namespace ACG {
namespace QtWidgets {
  

//== FORWARDDECLARATIONS ======================================================


class QWheel; 
class QtQGLWidget_Internal;
class QtSceneGraphWidget;


//== CLASS DEFINITION =========================================================


/** \class QtExaminerViewer QtExaminerViewer.hh <ACG/QtWidgets/QtExaminerViewer.hh>

    Widgets used to display and examine scenegraphs.
    This widget is used in all viewers that want to explore scenegraphs.
    For WalkMode use "QtWalkViewer"
 **/
class ACGDLLEXPORT QtExaminerViewer : public QtBaseViewer
{
Q_OBJECT

public:


  /// Constructor
  QtExaminerViewer( QWidget* _parent=0, 
		    const char* _name=0, 
		    QStatusBar *_statusBar=0, 
		    const QGLFormat* _format=0,
		    const QtBaseViewer* _share=0,
		    Options _options=DefaultOptions );
 
  // Destructor.
  virtual ~QtExaminerViewer() {}


  /// Lock scene rotation 
  void allowRotation( bool _mode ) { allowRotation_ = _mode; };


  /// Factors for zooming with the wheel
public:
  double wheelZoomFactor();
  double wheelZoomFactorShift();
  void setWheelZoomFactor(double _factor);
  void setWheelZoomFactorShift(double _factor);
private:
  double wZoomFactor_;
  double wZoomFactorShift_;


private slots:

  void slotAnimation();


protected:

  /// handle new key events
  virtual bool viewKeyPressEvent(QKeyEvent* /* _event */ ) { return false; }
  /// handle wheel events
  virtual void viewWheelEvent(QWheelEvent* _event);
  /// handle mouse events
  virtual void viewMouseEvent(QMouseEvent* _event);
  /// handle mouse events
  virtual void lightMouseEvent(QMouseEvent* _event);


protected:

  // mouse interaction
  QPoint   lastPoint2D_;


private:

  /// virtual trackball: map 2D screen point to unit sphere
  bool mapToSphere(const QPoint& _p, Vec3d& _result) const;


  // mouse interaction
  Vec3d    lastPoint3D_;
  bool     lastPoint_hitSphere_;
  bool     allowRotation_;


  // animation stuff
  Vec3d                        lastRotationAxis_;
  double                       lastRotationAngle_;
  QTime                        lastMoveTime_;
  QTimer*                      timer_;
};


//=============================================================================
} // namespace QtWidgets
} // namespace ACG
//=============================================================================
#endif // ACG_QTEXAMINERWIDGET_HH defined
//=============================================================================

