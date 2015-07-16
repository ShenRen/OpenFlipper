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
*   $LastChangedBy$                                                 *
*   $Date$                    *
*                                                                            *
\*===========================================================================*/

#include <QObject>
#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/TextureInterface.hh>
#include <OpenFlipper/BasePlugin/ToolboxInterface.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>
#include <OpenFlipper/BasePlugin/LoadSaveInterface.hh>
#include <OpenFlipper/BasePlugin/RPCInterface.hh>
#include <ACG/QtWidgets/QtExaminerViewer.hh>
#include <OpenFlipper/common/Types.hh>
#include <ObjectTypes/TriangleMesh/TriangleMesh.hh>
#include <ObjectTypes/PolyMesh/PolyMesh.hh>
#include "AnimationToolbox.hh"
#include "ObjectTypes/Skeleton/Skeleton.hh"
#include "ObjectTypes/Skeleton/PoseT.hh"
#include "ObjectTypes/Skeleton/BaseSkin.hh"

/**
 * @brief The skeletal animation plugin is used to interact with the skeleton
 *
 * this plugin controls the playback of skeletal animations
 *
 */
class SkeletalAnimationPlugin : public QObject, BaseInterface, ToolboxInterface, LoggingInterface, LoadSaveInterface, RPCInterface
{
  Q_OBJECT
  Q_INTERFACES(BaseInterface)
  Q_INTERFACES(ToolboxInterface)
  Q_INTERFACES(LoggingInterface)
  Q_INTERFACES(LoadSaveInterface)
  Q_INTERFACES(RPCInterface)

#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "org.OpenFlipper.Plugins.Plugin-SkeletalAnimation")
#endif

private:

  /// Pose typedef
  typedef PoseT<Skeleton::Point>  Pose;

signals:
  void updateView();

  void log(Logtype _type, QString _message);
  void log(QString _message);
  void addEmptyObject(DataType _type, int& _id);
  void updatedObject(int _id, const UpdateType& _type);
    
  void pluginExists(QString _pluginName, bool &_exists);
  void functionExists(QString _pluginName, QString _functionName, bool &_exists);

  // ToolboxInterface
  void addToolbox( QString _name  , QWidget* _widget, QIcon* _icon );   

public slots:
  // BaseInterface
  void slotAllCleared();
  void slotObjectUpdated( int _id, const UpdateType& _type);
  void slotObjectSelectionChanged(int _id);
  // LoadSaveInterface
  void fileOpened(int _id);
  void addedEmptyObject(int _id);
  void objectDeleted(int _id);

public slots:
  /**
    * @name Scripting interface
    * Use these methods in the script editor.
    */
  //@{
  int  getNumberOfFrames();
  bool attachSkin(int skeletonId, int skinId);
  bool detachSkin(int skeletonId, int skinId);
  bool clearSkins(int skeletonId);

  void setFrame(int _iFrame);
  int  getFrame();
  void changeFPS(int _fps);

  void playAnimation();
  void pauseAnimation();
  void stopAnimation();
  void nextFrame();
  void prevFrame();

  void updateSkins();
  //@}

  void UpdateUI();
  void setComboBoxPosition(unsigned int _animationIndex);
  void checkObjectSelection(const int _objectId);

  void slotAnimationNameChanged();

private slots:
  void setDescriptions();
  
  void initializePlugin();
  void pluginsInitialized();
  
  void exit();

  void slotAttachSkin();
  void slotClearSkins();
  void slotMethodChanged(int _index);
  void slotAnimationIndexChanged(int /*_index*/);
  void slotFrameChanged(int /*_index*/);
  void slotSkipFramesChanged(int _state);
  void slotAddAnimation();
  void slotDeleteAnimation();

  void animate();

public:
  SkeletalAnimationPlugin();
  QString name();
  QString description();

protected:

  AnimationHandle currentAnimationHandle();

  void UpdateSkins(BaseObjectData *_pSkeletonObject, AnimationHandle &_hAni);

private:
  /**
  * @name Skin management
  */
  //@{
  void attachSkin(BaseObjectData *pSkin, BaseObjectData *pSkeleton);
  void detachSkin(BaseObjectData *pSkin, BaseObjectData *pSkeleton);
  void clearSkins(BaseObjectData *_pSkeleton);
  //@}

protected:
  AnimationToolboxWidget *pToolbox_;         ///< A pointer to the toolbox widget
  QIcon* toolIcon_;

private:
  BaseSkin::Method method_; ///< The current blending method for the skin

  bool bGuiUpdating_;       ///< Used to drop a few messages while the gui is being updated
  QTimer animationTimer_;   ///< Timer used to control animations
  QTime animationTime_;     ///< Time since the animation was started, used to meet the given fps
  int animationOffset_;     ///< This frame was selected as the animation was started
  
  std::vector< int > activeSkeletons_;
};
