//=============================================================================
//
//                               OpenFlipper
//        Copyright (C) 2008 by Computer Graphics Group, RWTH Aachen
//                           www.openflipper.org
//
//-----------------------------------------------------------------------------
//
//                                License
//
//  OpenFlipper is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  OpenFlipper is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with OpenFlipper.  If not, see <http://www.gnu.org/licenses/>.
//
//-----------------------------------------------------------------------------
//
//   $Revision$
//   $Author$
//   $Date$
//
//=============================================================================




//=============================================================================
//
//  CLASS glViewer
//
//=============================================================================

#ifndef BASEVIEWER_HH
#define BASEVIEWER_HH

//== INCLUDES =================================================================

#include <OpenFlipper/common/Types.hh>
#include <OpenFlipper/common/ViewerProperties.hh>

#include <ACG/GL/GLState.hh>
#include <ACG/Scenegraph/SceneGraph.hh>
#include <ACG/QtWidgets/QtSceneGraphWidget.hh>

#include <QtOpenGL/QGLFormat>
#include <QBoxLayout>
#include <QtNetwork/QUdpSocket>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QDropEvent>
#include <QContextMenuEvent>
#include <QDragEnterEvent>
#include <QMouseEvent>
#include <QAction>
#include <QSize>
#include <QMap>
#include <QString>
#include <QMenu>
#include <QToolBar>
#include <QTime>
#include <QGraphicsWidget>
#include <QGraphicsSceneDragDropEvent>

#include <vector>
#include <string>


//== FORWARDDECLARATIONS ======================================================


class QPushButton;
class QToolButton;
class QStatusBar;
class QSplitter;
class QTimer;
class QImage;
class QSocketNotifier;


//== NAMESPACES ===============================================================

//== FORWARDDECLARATIONS ======================================================

namespace ACG {
  namespace QtWidgets {
    class QtWheel;
  }
}
class QtGLGraphicsScene;
class QtGLGraphicsView;
class QtGLViewerLayout;
class QtShiftPopupMenu;


//== CLASS DEFINITION =========================================================


/** \class glViewer QtBaseViewer.hh <OpenFlipper/widgets/glWidget/QtBaseViewer.hh>

    Base Viewer Widget: All viewers are derived from this one. Implements
    Mouse & Keyboard Events.
 **/

class DLLEXPORT glViewer : public QGraphicsWidget
{
Q_OBJECT

//-------------------------------------------------------------- public methods
public:

  //--------------------------------------------------- constructor / destructor

  /** Create a glViewer.
    \param _parent parent widget
    \param _name name (qt internal, qt debugging)
    \param _properties viewerOptions controlled by PluginFunctions
    \param _id examiner id
    \param _statusBar pointer to an existing status bar for this widget,
     if \c statusBar==0 then a \a private status bar will be created
    \param _format OpenGL context settings, will be passed to glarea()
  */
  glViewer( QtGLGraphicsScene* _scene,
            QGLWidget* _glWidget,
            Viewer::ViewerProperties& _properties,
            unsigned int _id,
            QGraphicsWidget* _parent=0,
            const char* _name=0,
            QStatusBar *_statusBar=0);

  /// Destructor.
  virtual ~glViewer();





  //------------------------------------------------------------- public methods

  virtual QSize sizeHint () const;

  /** Set status bar.
      Sets the \c QStatusBar widget that is used for information
      output, e.g. the frame rate of the rotation animation.
  */
  void setStatusBar(QStatusBar* _sb);

  /// Makes this widget the current widget for OpenGL operations.
  virtual void makeCurrent();
  /// Swaps the screen contents with the off-screen buffer.
  virtual void swapBuffers();

  /// Lock update of projection matrix.
  void lockProjectionUpdate( void ) { projectionUpdateLocked_ = true; }

  /// Unlock update of projection matrix.
  void unlockProjectionUpdate( void ) { projectionUpdateLocked_ = false; }

  signals :
    void functionMenuUpdate();

  public:

  /// Enable/disable mouse tracking (move events with no button press)
  void trackMouse(bool _track);

  /** Set scene graph.
      Sets the scene graph to the the graph rooted at \c _root.
      The available draw modes in the scene graph will be collected
      and the popup menu will be updated.<br>
      The \c sceneGraphToched() signal will be emitted (even if
      \c _root does not actually change).
  */
  void sceneGraph(ACG::SceneGraph::BaseNode* _root);


   /// projection mode
  enum ProjectionMode {
    ORTHOGRAPHIC_PROJECTION, //!< orthographic
    PERSPECTIVE_PROJECTION   //!< perspective
  };

  /// Changes the projection mode and updates the projection matrix.
  void projectionMode(ProjectionMode _p);
  /// get current projection mode
  ProjectionMode projectionMode() const { return projectionMode_; }


  /** Sets the center and dimension of the whole scene.  This point is
      used as fixpoint for rotations and to set the eye point far
      enough from the scene so that the whole scene is visible.
    */
  void setScenePos( const ACG::Vec3d& _center, double _radius );
  /** Get scene's center
      \see setScenePos()
  */
  const ACG::Vec3d& scene_center() const { return scene_center_; }
  /** Get scene's radius
      \see setScenePos()
  */
  double scene_radius() const { return scene_radius_; }


  /// set the viewing direction
  void viewingDirection( const ACG::Vec3d& _dir, const ACG::Vec3d& _up );

  /// Automatically normalize normals?
  enum NormalsMode {
    /// use provided normals as is
    DONT_TOUCH_NORMALS,
    /** Automatically normalize normals.
        Input normals are not normalized \a or the transformation matrix
	does scaling or shearing.
     */
    NORMALIZE_NORMALS
  };

  /// set treatment of normals
  void normalsMode(NormalsMode _mode);
  /// get treatment of normals
  NormalsMode normalsMode() const { return normalsMode_; }


  /// get light rotation matrix
  ACG::GLMatrixd& light_matrix() { return light_matrix_; }
  /// rotete light sources
  void rotate_lights(ACG::Vec3d& _axis, double _angle);
  /// update light position
  void update_lights();

  void draw_lights();


  /// copy current framebuffer to an QImage
  void copyToImage( QImage& _image, GLenum _buffer=GL_BACK) {
    copyToImage(_image, 0, 0, glWidth(), glHeight(), _buffer);
  }

  /// copy current framebuffer to an QImage
  void copyToImage( QImage& _image,
		    unsigned int _left,  unsigned int _top,
		    unsigned int _width, unsigned int _height,
		    GLenum _buffer );


  /// get width of QGLWidget
  unsigned int glWidth() const;
  /// get height of QGLWidget
  unsigned int glHeight() const;
  /// get size of QGLWIdget
  QSize glSize() const;
  /// map global to glarea coords
  QPoint glMapFromGlobal( const QPoint& _pos ) const;
  /// map glarea coords to global coords
  QPoint glMapToGlobal( const QPoint& _pos ) const;

  /// convert current view to text representation
  void encodeView(QString& _view);
  /** Decode and apply text representation of view encoded by encodeView().
      If \c _view was successfully decoded it will immediately be applied
      and \c true is returned, \c false is returned else.
   */
  bool decodeView(const QString& _view);

  /// initialize modelview matrix to identity
  void initModelviewMatrix();

  // get all Mouse & Key Events for GlWidget
  void grabGLArea();

  // undo grabbing GLArea
  void releaseGLArea();

  /// One or two pass Drawing
  void drawBlendedObjects(bool _status) { blending_ = _status; }

  /// translate the scene and update modelview matrix
  void translate(const ACG::Vec3d& trans);

  /// rotate the scene (around its center) and update modelview matrix
  void rotate(const ACG::Vec3d& axis, double angle)
  { rotate(axis, angle, trackball_center_); }

  /// rotate the scene and update modelview matrix
  void rotate(const ACG::Vec3d& axis, double angle, const ACG::Vec3d& _center);

//---------------------------------------------------------------- public slots
public slots:

  /// Redraw scene. Triggers paint event for updating the view (cf. drawNow()).
  virtual void updateGL();

  /// set home position
  virtual void setHome();
  /// go to home pos
  virtual void home();
  /// view the whole scene
  virtual void viewAll();
  /// Fly to. Get closer if \c _move_back=\c false, get more distant else.
  virtual void flyTo(const QPoint& _pos, bool _move_back);
  /// Fly to, get closer.
  virtual void flyTo(const QPoint& _pos) { flyTo(_pos,false); }
  /// Fly to, get more distant.
  virtual void flyFrom(const QPoint& _pos) { flyTo(_pos,true); }

  /**  Fly to point and set new viewing direction (animated).
   * @param _position New viewer position ( the new eye point of the viewer )
   * @param _center   The new scene center ( the point we are looking at )
   * @param _time     Animation time in ms
   */
  virtual void flyTo(const ACG::Vec3d& _position,
		               const ACG::Vec3d& _center,
		               double       _time = 1000.0);

  /// set perspective view (projectionMode(PERSPECTIVE_PROJECTION))
  virtual void perspectiveProjection();
  /// set orthographic view (projectionMode(ORTHOGRAPHIC_PROJECTION))
  virtual void orthographicProjection();
  /// toggle projection mode
  virtual void toggleProjectionMode();

  signals:

    void projectionModeChanged( bool _ortho );

  public slots:

  /// set view, used for synchronizing
  virtual void setView( const ACG::GLMatrixd& _modelview,
			               const ACG::GLMatrixd& _inverse_modelview );

  void actionPasteView();
  void actionCopyView();

//-------------------------------------------------------------- public signals
signals:

  /** A signal for giving mouse events to the application.
      This is done if the Pick-Button (top-most button of the toolbar)
      is activated. Connect to this signal to implement application
      specific handling of mouse events.<br>
      \a Notes:
      \arg You have to test for the QMouseEvent::type() of the
      event yourself (press/release/move)!
      \arg signalMouseEventIdentify() is emitted on button press if
      actionMode()==Question.
  */
  void signalMouseEvent(QMouseEvent*, const std::string&);

  /// This signal does not send the pick mode and is kept for compatibility
  void signalMouseEvent(QMouseEvent*);

  /// Emitted in Pick mode. Uses pick mode.
  void signalWheelEvent(QWheelEvent*, const std::string&);

  /** Emitted instead o signalMouseEvent() if actionMode()==Question
      and \c _event is a mouse press event.
   */
  void signalMouseEventIdentify( QMouseEvent* );

  /// scene graph has changed
  void signalSceneGraphChanged(ACG::SceneGraph::BaseNode* _root);


  signals:

  /** If popups are disabled context menu mode is set to custom Menu and the signal is passed
   * from the QGLWidget to this signal. You can use mapToGlobal for mapping this point to
   * global coords.
   */
  void signalCustomContextMenuRequested ( const QPoint & );


//----------------------------------------------------------- protected methods
protected:

  friend class QtGLGraphicsScene;
  friend class QtGLGraphicsView;

  /// initialize OpenGL states
  virtual void initializeGL();
  /// draw the scene. Triggered by updateGL().
  virtual void paintGL();
  /// handle resize events
  virtual void resizeEvent(QGraphicsSceneResizeEvent * _e);
  /// handle move events
  virtual void moveEvent (QGraphicsSceneMoveEvent * _e);

  /// handle mouse press events
  virtual void mousePressEvent(QGraphicsSceneMouseEvent* _event);
  /// handle mouse double click events
  virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* _event);
  /// handle mouse move events
  virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* _event);
  /// handle mouse release events
  virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* _event);
  /// handle mouse wheel events
  virtual void wheelEvent(QGraphicsSceneWheelEvent* _event);
  /// handle mouse press events
  virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* _e);


  /// specialized viewer: hande mouse events
  void viewMouseEvent( QMouseEvent* _event);
  /// specialized viewer: handle wheel events
  void viewWheelEvent(QWheelEvent* _event);

  /// optional: hande mouse events to rotate light
  void lightMouseEvent( QMouseEvent* /* _event */ );


  protected:


  /// updates projection matrix
  void updateProjectionMatrix();


//------------------------------------------------------------- protected slots
protected slots:

  /// process signals from wheelX_
  virtual void slotWheelX(double _dAngle);
  /// process signals from wheelX_
  virtual void slotWheelY(double _dAngle);
  /// process signals from wheelZ_
  virtual void slotWheelZ(double _dist);

  /// correct ??? (same function as in qt src)
  virtual void cleanupEventFilter()
  { removeEventFilter( sender());}

//----------------------------------------------------------- private functions
private:

  /// Copy constructor. Never used!
  glViewer(const glViewer&);
  /// Assignment operator. Never used!
  glViewer& operator=(const glViewer&);

  // create widgets
  void createWidgets(QStatusBar* _sb);

  /* Recursively draws each node in the scene graph.
      Called by paintGL(). */
  void drawScene();
  // helper called by drawScene().
  void drawScene_mono();
  // helper called by drawScene() when stereo viewing is active.
  void drawScene_stereo();

  // helper called by drawScene_stereo() when opengl stereo viewing is active.
  void drawScene_glStereo();

  // helper called by drawScene_stereo() when anaglyph stereo viewing is active.
  void drawScene_anaglyphStereo();

  // helper called by drawScene_stereo() when custom anaglyph stereo viewing is active.
  void drawScene_customAnaglyphStereo();

  // helper called to initialize/update custom anaglyph stereo
  void updateCustomAnaglyphStereo();

  // helper called to cleanup custom anaglyph stereo
  void finiCustomAnaglyphStereo();

//-------------------------------------------------------------- protected data
protected:


  // scene center and radius
  ACG::Vec3d                   scene_center_, trackball_center_;
  double                       scene_radius_, trackball_radius_;


  // projection stuff
  GLdouble                     orthoWidth_;
  GLdouble                     near_, far_, fovy_;

  // helper
  bool                         isRotating_;

  // current status bar
  QStatusBar* statusbar_;


//---------------------------------------------------------------- private data
private:


  // data stored for home view
  ACG::Vec3d                   home_center_;
  double                       home_radius_;
  ACG::GLMatrixd               home_modelview_,
                               home_inverse_modelview_;
  double                       homeOrthoWidth_;


  // matrix for rotating light position
  ACG::GLMatrixd               light_matrix_;


  // modi
  NormalsMode                  normalsMode_;
  ProjectionMode               projectionMode_;


  // helper
  bool                         trackMouse_;
  bool                         glareaGrabbed_;
  double                       frame_time_;


  // scenegraph stuff
  ACG::SceneGraph::BaseNode*   sceneGraphRoot_;

  bool                         projectionUpdateLocked_;
  bool                         blending_;

  // graphics scene used to paint gl context and widgets
  QtGLGraphicsScene* glScene_;

  // gl widget used as drawing area to paint the graphics scene
  QGLWidget* glWidget_;

  // Base graphics widget layout
  QtGLViewerLayout* glBaseLayout_;

  // examiner id
  unsigned int id_;



  // vector of current draw mode actions
  std::vector< QAction * > drawMenuActions_;

  // Used to calculate the time passed between redraws
  QTime redrawTime_;

  //===========================================================================
  /** @name Wheels
   * @{ */
  //===========================================================================

  public:
    bool wheelsVisible();

  public slots:
    void slotShowWheels();
    void slotHideWheels();

  private:
    // rotate around x-axis
    ACG::QtWidgets::QtWheel* wheelX_;
    // rotate around y-axis
    ACG::QtWidgets::QtWheel* wheelY_;
    // translate along z-axis
    ACG::QtWidgets::QtWheel* wheelZ_;


  /** @} */

  //===========================================================================
  /** @name view handling
   * @{ */
  //===========================================================================
  signals:
    void viewUpdated();

  /** @} */

  //===========================================================================
  /** @name Stereo
   * @{ */
  //===========================================================================

  public slots:

    /// enable/disable stereo mode
    virtual void setStereoMode(bool _b);

  private:
    // stereo stuff
    bool                         stereo_;

    // custom anaglyph stuff
    int    agTexWidth_;
    int    agTexHeight_;
    GLuint agTexture_[2];
    GLuint agProgram_;
    bool   customAnaglyphSupported_;


  /** @} */


  //===========================================================================
  /** @name Key handling
   * @{ */
  //===========================================================================

  signals:
    /** \brief Key Event received
     *
     * This signal is emitted if a Key press event is received which is not handled by
     * the widget itself. If internal key handling is disabled the events are passed to
     * the parent of this widget.
     */
    void signalKeyPressEvent(QKeyEvent*);

  protected:

    /** \brief Get keyPress events from the glArea
     *
     * This function is called by the internal gl widget when receiving a key press event.
     */
    virtual void keyPressEvent(QKeyEvent* _event) { _event->ignore(); };

    /** \brief Get keyRelease events from the glArea
     *
     * This function is called by the internal gl widget when receiving a key release event.
     * Here these events are simply passed to the parent widget.
     *
     */
    virtual void keyReleaseEvent(QKeyEvent* _event) { _event->ignore(); };

    /** \brief Handle key events in view mode
     *
     * This funtion is called by the BaseViewer if a key press event occured in view mode.
     * This function has to be implemented by every viewer!
     *
     * @return If the derived class handled the event it has to return true otherwise false
     */
    virtual bool viewKeyPressEvent(QKeyEvent* /*_event*/) { return false; };

  /** @} */

  //===========================================================================
  /** @name Drag and Drop
   * @{ */
  //===========================================================================

  public:

    /// drag & drop for modelview copying
    virtual void dragEnterEvent(QGraphicsSceneDragDropEvent* _e);

    /// drag & drop for modelview copying
    virtual void dropEvent(QGraphicsSceneDragDropEvent* _e);

  signals:
    /** Signal is emitted when Control modifier is pressed and mouse moded.
     * It will only be emitted if in externalDragMode ( see setExternalDrag )
     */
    void startDragEvent( QMouseEvent* _event );

    /** Signal is emitted only if in externalDragMode ( see setExternalDrag )
     * It will be send if a drag enters the gl widget.
     */
    void dragEnterEvent(QDragEnterEvent* _event);

    /** Signal is emitted only if in externalDragMode ( see setExternalDrag )
     * It will be send if a drop occurs in the gl widget.
     */
    void dropEvent( QDropEvent* _event );


  /** @} */

  //===========================================================================
  /** @name Snapshots
   * @{ */
  //===========================================================================

  public slots:


    /** Trigger a snapshot and increase the snapshot counter.
        Save snapshot to file determined by snapshotBaseFileName() and
        the current snapshot counter. The \a back buffer will be saved.
    */
    virtual void snapshot();

    /// Take a snapshot and store it in the given image
    virtual void snapshot(QImage& _image);

  /** @} */

  //===========================================================================
  /** @name Picking
   * @{ */
  //===========================================================================
  public:
    /** Apply pick action. <br>
     *   Information about the picked primitive is stored in the
     *   provided pointers. No information is stored if 0-pointers
     *   are given. Resulting values are defined only if \c true
     *   has been returned!<br>
     *
     * @param _pickTarget Select what should be picked: Faces/Vertices/...
     * @param _mousePos   The position to be used for picking
     * @param _nodeIdx    If picking is successfull this will contain the id of the scenegraph node picked.
     * @param _targetIdx  Special index defined by the picked Node. E.g. Facehandle/VertexHandle/.. for MeshNodes
     * @param _hitPointPtr Pointer to 3D point from picking
     * @return Successfull?
     */
    bool pick( ACG::SceneGraph::PickTarget _pickTarget,
               const QPoint& _mousePos,
               unsigned int& _nodeIdx,
               unsigned int& _targetIdx,
               ACG::Vec3d*   _hitPointPtr=0 );

    /** Apply pick action. <br>
     *   Picks all objects in the given Region. Information about the picked primitives is stored in the
     *   provided list. Resulting values are defined only if \c true
     *   has been returned!
     *  <br>
     *
     * @param _pickTarget Select what should be picked: Faces/Vertices/...
     * @param _region     Area for picking
     * @param _list       List of found scenegraph objects (node/target pairs)
     * @return Successfull?
     */
    bool pick_region( ACG::SceneGraph::PickTarget                _pickTarget,
                      const QRegion&                             _region,
                      QList<QPair<unsigned int, unsigned int> >& _list);

    /** get the coordinates of the picked point by z-buffer re-projection
     * @param _mousePos The position to pick
     * @param _hitPoint The point returned by the reprojection
     * @return Successfull?
     */
    bool fast_pick( const QPoint&  _mousePos,
                    ACG::Vec3d&    _hitPoint );

  private:

    /// pick using colors
    int pickColor( ACG::SceneGraph::PickTarget _pickTarget,
                   const QPoint& _mousePos,
                   unsigned int& _nodeIdx,
                   unsigned int& _targetIdx,
                   ACG::Vec3d*   _hitPointPtr=0 );

    /// pick using selection buffer
    bool pickGL( ACG::SceneGraph::PickTarget _pickTarget,
                 const QPoint& _mousePos,
                 unsigned int& _nodeIdx,
                 unsigned int& _targetIdx,
                 ACG::Vec3d*   _hitPointPtr=0 );

  /** @} */

  //===========================================================================
  /** @name Merge from examiner
   * @{ */
  //===========================================================================

  public:

  /// Lock scene rotation
  void allowRotation( bool _mode ) { allowRotation_ = _mode; };


  private slots:

    void slotAnimation();

  protected:

    // mouse interaction
    QPoint   lastPoint2D_;

  private:

    /// virtual trackball: map 2D screen point to unit sphere
    bool mapToSphere(const QPoint& _p, ACG::Vec3d& _result) const;


    // mouse interaction
    ACG::Vec3d    lastPoint3D_;
    bool          lastPoint_hitSphere_;
    bool          allowRotation_;

    // animation stuff
    ACG::Vec3d                   lastRotationAxis_;
    double                       lastRotationAngle_;
    QTime                        lastMoveTime_;
    QTimer*                      timer_;

    /** @} */

  //===========================================================================
  /** @name Viewer State
   * @{ */
  //===========================================================================

  public:
    /// Returns a pointer to the Viewer Status
    Viewer::ViewerProperties* properties(){ return &properties_; };

  private:
    /// All properties for this viewer
    Viewer::ViewerProperties& properties_;

    /// Gl State
    ACG::GLState                 *glstate_;

  private slots:
    /** Called when properties for the viewer require a redraw
     * The slot will trigger the redraw after setting the right properties.
     */
    void slotPropertiesUpdated();

  private:
    /** This will apply all properties without redrawing
     *  You have to set the context yourself!
     */
    void applyProperties();

  /** @} */
};


//=============================================================================
//=============================================================================
#endif // BASEVIEWER_HH defined
//=============================================================================
