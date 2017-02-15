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




#ifndef TYPELIGHTPLUGIN_HH
#define TYPELIGHTPLUGIN_HH

#include <QObject>

#include <OpenFlipper/common/Types.hh>
#include <ObjectTypes/Light/Light.hh>
#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/LoadSaveInterface.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>
#include <OpenFlipper/BasePlugin/TypeInterface.hh>
#include <OpenFlipper/BasePlugin/MouseInterface.hh>
#include <OpenFlipper/BasePlugin/ToolbarInterface.hh>
#include <OpenFlipper/BasePlugin/PickingInterface.hh>

class TypeLightPlugin : public QObject, BaseInterface, LoadSaveInterface, LoggingInterface, TypeInterface, MouseInterface,
    ToolbarInterface, PickingInterface
{
   Q_OBJECT
   Q_INTERFACES(BaseInterface)
   Q_INTERFACES(LoadSaveInterface)
   Q_INTERFACES(LoggingInterface)
   Q_INTERFACES(TypeInterface)
   Q_INTERFACES(MouseInterface)
   Q_INTERFACES(ToolbarInterface)
   Q_INTERFACES(PickingInterface)

#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "org.OpenFlipper.Plugins.Plugin-TypeLight")
#endif

  signals:
    // Logging interface
    void log(Logtype _type, QString _message);
    void log(QString _message);
    
    // LoadSave Interface
    void addEmptyObject(DataType _type, int& _id);
    void emptyObjectAdded( int _id );
    void deleteObject(int _id);
    
    // BaseInterface
    void updatedObject(int _id, const UpdateType& _type);
    void updateView();
    
    // ToolbarInterface
    void getToolBar( QString _name, QToolBar*& _toolbar );
    
    // PickingInterface
    void addPickMode(const std::string& _mode);
    void addHiddenPickMode(const std::string& _mode);
    void setPickModeMouseTracking (const std::string& _mode, bool _mouseTracking);
    void setPickModeToolbar(const std::string& _mode, QToolBar * _toolbar);
    
  private slots:

    // BaseInterface
    void noguiSupported( ) {};
    
    // PickingInterface
    void slotPickModeChanged (const std::string& _pickMode);
    
    // Is called whenever the scene is cleared
    void slotAllCleared();
    
    // Adds three default light sources to scene
    void addDefaultLights();
    // ... and removes them
    void removeDefaultLights();
    
    // Load/Save Interface
    void addedEmptyObject(int _id);
    void objectDeleted(int _id);
    
    // MouseInterface
    //void slotMouseEventLight( QMouseEvent* _event);
    void slotMouseEventLight( QMouseEvent* _event );
    
  private:
        
    /// Rotate light sources
    void rotateLights(ACG::Vec3d& _axis, double _angle);
    
    /// Rotate spot light direction
    void rotateLightDirection(ACG::Vec3d& _axis, double _angle);
    
    /// Update light sources' positions
    void updateLights();
    
    /** \brief Compute click on trackball containing the light source
    *
    * Compute click on trackball having the trackball center of the scene as center point.
    */
    bool computeClickOnTrackball(const QPoint& _v2D, ACG::Vec3d& _clickOnSphere, ACG::GLState& _state);
    
    /** \brief Compute click on trackball of light source
    *
    * Compute click on trackball having the light's center as center point.
    */
    bool computeClickOnLightTrackball(const QPoint& _v2D, ACG::Vec3d& _clickOnSphere, ACG::GLState& _state);
        
  public:

     ~TypeLightPlugin() {};
     TypeLightPlugin();

     QString name() { return (QString("TypeLight")); };
     QString description( ) { return (QString(tr("Register Light type"))); };
     
     bool registerType();

  public slots:

    // Base Interface
    QString version() { return QString("1.0"); };
    
    // Type Interface
    int addEmpty();
    DataType supportedType() { return DATA_LIGHT; }; 
    
    void pluginsInitialized();
    
    // Add default light
    int addDefaultLight(QString _name);
    
    void showReducedUi(bool reduced);

  private slots:
    
    void slotLightModeRequest(bool _checked);
    void slotPickModeRequest(QAction* _action);
    
    void slotSetAllOrTarget(bool _checked);
    
  private:
    
    /// Find depth of nearest light source
    float findDepth();
    
    // Find distance of farthest light source from trackball center
    double getFarthestRadius();
    
    /// Get number of non-directional light sources
    void addAssistLines();
    
    /// Count light sources in scene
    std::vector<int> lightSources_;
    
    bool defaultLights_;
    
    // Matrix for rotating light position
    ACG::GLMatrixd light_matrix_;
    
    // Toolbar for light mode
    QToolBar* toolbar_;
    
    // Toolbar that show detailed options for light mode
    QToolBar* lightOptions_;
    
    // GUI elements
    QAction* lightAction_;
    
    QAction* moveMode_;
    QAction* translateMode_;
    QAction* rotateMode_;
    
    QAction* allLightsMode_;
    QAction* viewerToolbarAction_;
    
    // True if only target lights should be transformed
    bool onlyTargets_;

    // Mouse interactions
    QPoint      lastPoint2D_;
    ACG::Vec3d  lastPoint3D_;
    //bool        lastPoint_hitSphere_;
    // depth of plane olong which objects are translated
    // if middle mouse button is pressed
    float       planeDepth_;
    ACG::Vec3d  transVec_;
    bool        rotation_;
    
    // Radius of trackball for most distant light source
    double radius_;
    
    // Depth of light source that is closest to camera
    float depth_;
    
    // Picked light stuff
    ACG::Vec3d  lightCenter_;
    double      lightRadius_;
    int         lightId_;
    
    bool        hitLightTrackball_;
    bool        hitTrackball_;
    
};

#endif //TYPELIGHTPLUGIN_HH
