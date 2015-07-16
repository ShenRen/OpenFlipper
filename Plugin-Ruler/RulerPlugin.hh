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

#ifndef RULERPLUGIN_HH
#define RULERPLUGIN_HH

#include <QObject>
#include <QString>

#include <ACG/Utils/SmartPointer.hh>

#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/MouseInterface.hh>
#include <OpenFlipper/BasePlugin/PickingInterface.hh>
#include <OpenFlipper/BasePlugin/ToolbarInterface.hh>
#include <OpenFlipper/BasePlugin/LoadSaveInterface.hh>
#include <OpenFlipper/BasePlugin/OptionsInterface.hh>

#include "Ruler.hh"
#include "RulerOptions.hh"

class RulerPlugin : public QObject, BaseInterface, MouseInterface, PickingInterface, ToolbarInterface, LoadSaveInterface, OptionsInterface
{
  Q_OBJECT
  Q_INTERFACES(BaseInterface)
  Q_INTERFACES(MouseInterface)
  Q_INTERFACES(PickingInterface)
  Q_INTERFACES(ToolbarInterface)
  Q_INTERFACES(LoadSaveInterface)
  Q_INTERFACES(OptionsInterface)

#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "org.OpenFlipper.Plugins.Plugin-Ruler")
#endif

signals:
  void addToolbar(QToolBar *_toolbar);

  void addPickMode(const std::string &_mode);

  void updateView();

  void setPickModeMouseTracking  ( const std::string &_mode, bool  _mouseTracking);

public slots:

  void slotPickModeChanged(const std::string& _mode);

  void slotMouseEvent(QMouseEvent* _event);

  void slotViewChanged();
public:

  // OptionsInterface
  bool initializeOptionsWidget(QWidget*& _widget);

  RulerPlugin();
  ~RulerPlugin();

  QString name(){return QString("RulerPlugin");}
  QString description(){return QString("Measure the distance between two points.");}

private:

  void reset();

  void enableDragMode(const int _point);

  void disableDragMode();

  bool dragModeActive(){return lineDrag_ >= 0;}

  QAction *buttonAction_;

  const std::string pickModeName_;

  //saves the index of the dragged point, if no point was dragged, it is -1
  int lineDrag_;

  //checks if a double click was provided for resetting
  bool dblClickCheck_;

  ptr::shared_ptr<Ruler> currentRuler_;

  RulerOptions* optionsWidget_;

  const QString textSizeSettingName_;
private slots:

  void initializePlugin();

  void pluginsInitialized();

  void slotChangePickMode();

  void slotAllCleared();

  void objectDeleted(int _id);

  // OptionsInterface
  void applyOptions();

public slots:
  QString version(){ return QString("1.0"); }
};


#endif //RULERPLUGIN_HH
