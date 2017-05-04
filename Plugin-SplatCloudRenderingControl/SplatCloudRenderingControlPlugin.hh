//================================================================
//
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
*   $Revision: 12588 $                                                       *
*   $LastChangedBy: moebius $                                                *
*   $Date: 2011-10-13 12:41:04 +0200 (Do, 13 Okt 2011) $                     *
*                                                                            *
\*===========================================================================*/


//================================================================
//
//  CLASS SplatCloudRenderingControlPlugin
//
//    This class represents an OpenFlipper plugin controlling the rendering of SplatClouds.
//
//================================================================


#ifndef SPLATCLOUDRENDERINGCONTROLPLUGIN_HH
#define SPLATCLOUDRENDERINGCONTROLPLUGIN_HH


//== INCLUDES ====================================================


#include <QObject>

#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/LoadSaveInterface.hh>
#include <OpenFlipper/BasePlugin/ToolboxInterface.hh>
#include <OpenFlipper/BasePlugin/ContextMenuInterface.hh>
#include <OpenFlipper/BasePlugin/ViewModeInterface.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>

#include <OpenFlipper/common/Types.hh>


//== CLASS DEFINITION ============================================


class SplatCloudRenderingControlPlugin : public QObject, BaseInterface, LoadSaveInterface, ToolboxInterface, ContextMenuInterface, ViewModeInterface, LoggingInterface
{
  Q_OBJECT
  Q_INTERFACES( BaseInterface        )
  Q_INTERFACES( LoadSaveInterface    )
  Q_INTERFACES( ToolboxInterface     )
  Q_INTERFACES( ContextMenuInterface )
  Q_INTERFACES( ViewModeInterface    )
  Q_INTERFACES( LoggingInterface     )

#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "org.OpenFlipper.Plugins.Plugin-SplatCloudRenderingControl")
#endif

signals:

  //-- Base Interface --
  void updatedObject( int _objectId, const UpdateType &_type );

  //-- Toolbox Interface --
  void addToolbox( QString _name, QWidget *_widget, QIcon *_icon );

  //-- ContextMenu Interface--
  void addContextMenuItem( QAction *_action, DataType _objectType, ContextMenuType _type );

  //-- ViewMode Interface--
  void defineViewModeToolboxes   ( QString _mode, QStringList _usedWidgets      );
  void defineViewModeToolbars    ( QString _mode, QStringList _usedToolbars     );
  void defineViewModeContextMenus( QString _mode, QStringList _usedContextMenus );
  void defineViewModeIcon        ( QString _mode, QString     _iconName         );

  //-- Logging Interface --
  void log(                QString _message );
  void log( Logtype _type, QString _message );

public:

  SplatCloudRenderingControlPlugin();

  //-- Base Interface --
  QString name()        { return QString( "SplatCloud Rendering Control Plugin"   ); }
  QString description() { return QString( "Control over rendering of SplatClouds" ); }

private:

  // toolbox options
  QDoubleSpinBox *toolboxPointsizeScale_;
  QSpinBox       *toolboxDefaultColorR_;
  QSpinBox       *toolboxDefaultColorG_;
  QSpinBox       *toolboxDefaultColorB_;
  QDoubleSpinBox *toolboxDefaultNormalX_;
  QDoubleSpinBox *toolboxDefaultNormalY_;
  QDoubleSpinBox *toolboxDefaultNormalZ_;
  QDoubleSpinBox *toolboxDefaultPointsize_;

  // context menu options
  QAction *contextScaleAction_;
  QAction *contextCullingAction_;
  QAction *contextDefaultsAction_;
  QAction *contextReloadShadersAction_;
  QAction *contextRebuildVBOAction_;

  // context menu scale widget options
  QWidget        *contextScaleWidget_;
  QDoubleSpinBox *contextScaleWidgetPointsizeScale_;

  // context menu defaults widget options
  QWidget        *contextDefaultsWidget_;
  QSpinBox       *contextDefaultsWidgetDefaultColorR_;
  QSpinBox       *contextDefaultsWidgetDefaultColorG_;
  QSpinBox       *contextDefaultsWidgetDefaultColorB_;
  QDoubleSpinBox *contextDefaultsWidgetDefaultNormalX_;
  QDoubleSpinBox *contextDefaultsWidgetDefaultNormalY_;
  QDoubleSpinBox *contextDefaultsWidgetDefaultNormalZ_;
  QDoubleSpinBox *contextDefaultsWidgetDefaultPointsize_;

private slots:

  //-- Base Interface --
  void initializePlugin();

  //-- LoadSave Interface --
  void addedEmptyObject( int _objectId );

  //-- ContextMenu Interface --
  void slotUpdateContextMenu( int _objectId );

  // slot called when a toolbox option changed value
  void slotToolboxPointsizeScaleValueChanged();
  void slotToolboxDefaultColorValueChanged();
  void slotToolboxDefaultNormalValueChanged();
  void slotToolboxDefaultPointsizeValueChanged();

  // slots called when a toolbox button was clicked
  void slotToolboxEnableBackfaceCullingButtonClicked();
  void slotToolboxDisableBackfaceCullingButtonClicked();
  void slotToolboxReloadShadersButtonClicked();
  void slotToolboxRebuildVBOsButtonClicked();
  void slotToolboxPointSizeButtonClicked();
  void slotToolboxGeometryShaderButtonClicked();

  // slots called when a context menu action was triggered
  void slotContextScaleActionTriggered();
  void slotContextCullingActionTriggered();
  void slotContextReloadShadersActionTriggered();
  void slotContextRebuildVBOActionTriggered();
  void slotContextDefaultsActionTriggered();

  // slots called when a context menu scale widget option changed value
  void slotContextScaleWidgetPointsizeScaleValueChanged();

  // slots called when a context menu defaults widget option changed value
  void slotContextDefaultsWidgetDefaultColorValueChanged();
  void slotContextDefaultsWidgetDefaultNormalValueChanged();
  void slotContextDefaultsWidgetDefaultPointsizeValueChanged();

public slots:

  // -- Base Interface --
  QString version() { return QString( "1.0" ); }
};


//================================================================


#endif // SPLATCLOUDRENDERINGCONTROLPLUGIN_HH
