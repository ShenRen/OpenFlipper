/*===========================================================================*\
*                                                                            *
*                              OpenFlipper                                   *
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
*                                                                            *
\*===========================================================================*/

/*===========================================================================*\
*                                                                            *
*   $Revision$                                                       *
*   $LastChangedBy$                                                *
*   $Date$                     *
*                                                                            *
\*===========================================================================*/

#ifndef ViewControlPlugin_HH
#define ViewControlPlugin_HH

#include <QObject>
#include <QMenuBar>
#include <QGroupBox>

#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>
#include <OpenFlipper/BasePlugin/PickingInterface.hh>
#include <OpenFlipper/BasePlugin/ToolbarInterface.hh>
#include <OpenFlipper/BasePlugin/ContextMenuInterface.hh>
#include <OpenFlipper/common/Types.hh>
#include <ObjectTypes/PolyMesh/PolyMesh.hh>
#include <ObjectTypes/TriangleMesh/TriangleMesh.hh>
#include "shaderWidget.hh"

struct ShaderInfo {
      QString name;
      QString description;
      QString details;
      QString version;
      QString example;

      QString directory;

      QString vertexShader;
      QString fragmentShader;

      QString pickVertexShader;
      QString pickFragmentShader;

      bool        hasUniforms;
      QStringList uniforms;
      QStringList uniformTypes;
      QStringList uniformsDefault;
      QStringList uniformsMax;
      QStringList uniformsMin;
};

class ViewControlPlugin : public QObject, BaseInterface , PickingInterface, LoggingInterface, ToolbarInterface, ContextMenuInterface
{
  Q_OBJECT
  Q_INTERFACES(BaseInterface)
  Q_INTERFACES(PickingInterface)
  Q_INTERFACES(LoggingInterface)
  Q_INTERFACES(ToolbarInterface)
  Q_INTERFACES(ContextMenuInterface)

#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "org.OpenFlipper.Plugins.Plugin-ViewControl")
#endif

  signals:
    //BaseInterface
    void updateView();

    //PickingInterface
    void addHiddenPickMode( const std::string& _mode );
    
    //LoggingInterface
    void log(Logtype _type, QString _message);
    void log(QString _message);
    
    //ToolbarInterface
    void addToolbar(QToolBar* _toolbar);
    void getToolBar(QString _name, QToolBar*& _toolbar);

    // ContextMenuInterface
    void addContextMenuItem(QAction* _action , ContextMenuType _type);
    void addContextMenuItem(QAction* _action , DataType _objectType , ContextMenuType _type );
    void hideContextMenu();

    void setSlotDescription(QString     _slotName,   QString     _slotDescription,
                            QStringList _parameters, QStringList _descriptions);

  private slots:

    void pluginsInitialized();

    void contextMenuTriggered(QAction* _action);

    void slotUpdateContextMenu( int _objectId );

  public :

    ViewControlPlugin();
    ~ViewControlPlugin() {};


    QString name() { return (QString("ViewControl")); };
    QString description( ) { return (QString("Control the current or global visualization settings")); };


  private:
    QMenu* viewControlMenu_;

    /** Last object id for context menu. Is -1 if no object was selected
     */
    int lastObjectId_;

    /** Active draw Modes for the last clicked object and its children
     */
    ACG::SceneGraph::DrawModes::DrawMode activeDrawModes_;

    /** Available draw Modes for the last clicked object and its children
     */
    ACG::SceneGraph::DrawModes::DrawMode availDrawModes_;

    ShaderWidget* shaderWidget_;

    /** List of available shaders
    */
    std::vector <ShaderInfo> shaderList_;
    
    QToolBar*     toolbar_;
    QActionGroup* toolbarViewingDirections_;
    QAction*      viewTop_;
    QAction*      viewBottom_;
    QAction*      viewLeft_;
    QAction*      viewRight_;
    QAction*      viewFront_;
    QAction*      viewBack_;

    std::map<QCheckBox*, ACG::SceneGraph::DrawModes::DrawMode> activeCheckboxes;

  private slots:
    QString version() { return QString("1.0"); };

    // This slot is called when a custom draw mode is selected from the context menu
    void slotDrawModeSelected( QAction * _action );

    // This slot is called when the user clicks on an item in the shaderWidget
    void slotShaderClicked( QListWidgetItem * _item );

    // This slot is called when the user doubleclicks on an item in the shaderWidget
    void slotShaderDoubleClicked( QListWidgetItem * _item );

    // slot for setting a shader from the widget
    void slotSetShader();

    // slot is called when the data of the uniforms-table changes
    void itemChanged(QTableWidgetItem* item);
    
    // Set the view to one of the six possible look-at directions
    void setView(int _mode, int _viewer = PluginFunctions::ACTIVE_VIEWER );
    
    // Set the view mode using the defined actions
    void setView(QAction* _action);

  private:
    // Update the list of available shaders
    void updateShaderList();

    // initialize the shader Widget
    void initShaderWidget();

    // Announce local slots
    void setDescriptions();

    /** \brief Return of all of the given modeling areas are visible
     *
     * @param _bits  Which areas to test
     * @param _id    Id of the object
     * @return       All areas visible?
     */
    bool areasVisible( StatusBits _bits, int _id );

    /** \brief Show or hide modeling areas
     *
     * @param _bits   Which areas to test
     * @param _id     Id of the object
     * @param _state  Show or hide?
     */
    void showAreas(  StatusBits _bits, int _id , bool _state  );


  public slots:
    /// For meshes returns if the selection for this object is visible
    bool selectionVisible( int _id );

    /// Allows to enable/disable visualization of the objects selection for meshes
    void showSelection( int _id , bool _state );

    /// For meshes returns if the modeling areas for this object is visible
    bool modelingAreasVisible( int _id );

    /// Allows to enable/disable visualization of the objects modeling area for meshes
    void showModelingAreas( int _id , bool _state  );

    /** Disable the given shader for a given object and draw mode.
     * @param _objectId ID of the object for which to disable the shader
     * @param _drawMode _drawMode for which the shader is disabled
     * @param _shader Shader which is disabled. If this shader is not currently active for
     *                the given _drawMode, nothing will be disabled. In order to disable
     *                all shaders for the given _drawMode, use _shader=0 (default).
     */
    void disableShader(int _objectId, ACG::SceneGraph::DrawModes::DrawMode _drawMode, ShaderInfo* _shader=0);
    
    /** Sets a shader for the object and the given drawMode( can be a combination of draw modes )
     *  @param _id                 Object Id
     *  @param _drawMode           ; separated list of drawmodes used by the shader
     *  @param _shader             Shader information
     */
    void setShader(int _id, ACG::SceneGraph::DrawModes::DrawMode _drawMode, ShaderInfo _shader);

    /**
     *
     */

    /** Sets a Shader for a specific draw mode
     *
     * @param _id        Object id that should use the shader
     * @param _drawMode  Draw mode where this shader will be active
     * @param _name      Name of the shader that should be used
     */
    void setShader(int _id, QString _drawMode, QString _name );

    /// get information about available uniforms for a given shader
    QStringList getUniforms(QString _shader);

    QString getUniformType(QString _shader, QString _uniform );
    QString getUniformDefault(QString _shader, QString _uniform );
    QString getUniformMin(QString _shader, QString _uniform );
    QString getUniformMax(QString _shader, QString _uniform );

    /// set the value of a uniform in a shader for a specific drawMode
    void setUniform(int _objID, ACG::SceneGraph::DrawModes::DrawMode _drawMode, QString _shader, QString _uniform, QString _value );


    /// Set the viewing direction
    void setViewingDirection( Vector _direction, Vector _upvector , int _viewer = PluginFunctions::ALL_VIEWERS );

    /// Set scene radius
    void setSceneRadius( double _radius , int _viewer = PluginFunctions::ALL_VIEWERS );

    /// Rotate Scene
    void rotate( Vector _axis, double _angle, Vector _center , int _viewer = PluginFunctions::ALL_VIEWERS );

    /// translate Scene
    void translate( Vector _vec , int _viewer = PluginFunctions::ALL_VIEWERS );
    
    /// Set the draw mode for a viewer
    void setDrawMode(QString _mode, int _viewer = PluginFunctions::ALL_VIEWERS );
    
    /** \brief Set the draw mode for an object
    *
    * This function can be used to set the drawmode for an object.
    * 
    * @param _mode DrawMode as String ( can be a list of drawmodes which will be combined to a new mode )
    * @param _objectID Id of the object to set the draw mode
    * @param _force Set the draw mode even if its not directly supported by the objects nodes
    */
    void setObjectDrawMode(QString _mode, int _objectID , bool _force = true );

    /// Get a viewers viewing direction
    Vector viewingDirection( int _viewer = PluginFunctions::ACTIVE_VIEWER );

    /// get a viewers up vector
    Vector upVector( int _viewer = PluginFunctions::ACTIVE_VIEWER );

    /// get a viewers eye Position
    Vector eyePosition( int _viewer = PluginFunctions::ACTIVE_VIEWER );

    /// Get the scene center
    Vector sceneCenter( int _viewer = PluginFunctions::ACTIVE_VIEWER );

    /// Set the scene center
    void setSceneCenter( Vector _center, int _viewer = PluginFunctions::ALL_VIEWERS );

    /// Enable or disable Backface culling
    void enableBackfaceCulling( bool _state , int _viewer = PluginFunctions::ALL_VIEWERS );


    void setEyePosition(Vector _eye);

    /// Change View on all viewers to view whole scene
    void viewAll();

    /// Change View on given Viewer to view whole scene
    void viewAll(int _viewer);

    /// Change View on given Viewer to view home position
    void viewHome();

    /// Change View on all Viewers to view home position
    void viewHome(int _viewer);



    /// Change Viewer to orthographicProjection
    void orthographicProjection();

    /// Change all Viewers to orthographicProjection
    void orthographicProjection(int _viewer);



    /// Change Viewer to perspectiveProjection
    void perspectiveProjection();

    /// Change all Viewers to perspectiveProjection
    void perspectiveProjection(int _viewer);
    
    /// Set fovy angle of projection
    void setFOVY( double _fovy );
    
    /// Set the mode for the coordinate system
    /// Perspective or orthogonal
    void setCoordsysProjection(bool _orthogonal);

    void setTwoSidedLighting(bool _enabled);

};

#endif //ViewControlPlugin_HH
