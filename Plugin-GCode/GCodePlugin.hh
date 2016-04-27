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
*   $Revision$                              *
*   $Author$                            *
*   $Date$                          *
*                                                                           *
\*===========================================================================*/

#ifndef GCODEPLUGIN_HH
#define GCODEPLUGIN_HH


#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>
#include <OpenFlipper/BasePlugin/ToolboxInterface.hh>
#include <OpenFlipper/common/Types.hh>

#include <ObjectTypes/GCode/GCode.hh>
#include <ObjectTypes/PolyLine/PolyLine.hh>

#include <QObject>
#include <QMenuBar>

#include "GCodeToolbar.hh"




class GCodePlugin : public QObject, BaseInterface, ToolboxInterface, LoggingInterface
{
  Q_OBJECT
    Q_INTERFACES(BaseInterface)
    Q_INTERFACES(ToolboxInterface)
    Q_INTERFACES(LoggingInterface)

#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "org.OpenFlipper.Plugins.Plugin-GCode")
#endif

signals:
  //BaseInterface
  void updateView();
  void updatedObject(int, const UpdateType&);

  //LoggingInterface
  void log(Logtype _type, QString _message);
  void log(QString _message);

  // ToolboxInterface
  void addToolbox(QString _name, QWidget* _toolbox, QIcon* icon);

public:
  GCodePlugin() : gcode_play_timer(0) { }

  // BaseInterface
  ~GCodePlugin();
  QString name() { return (QString("GCode Plugin")); };
  QString description() { return (QString("GCode plugin")); };

  private slots:

  ///Plugin Initialization
  void initializePlugin();

  /// Button slots
  void slotSliderGCodeValueChanged();
  void slotGCodePositionBoxChanged();


  void onGCodePlayPressed();
  void onGCodePlayTimer();


  public slots:

  QString version() { return QString("1.0"); };

  // set total time to the max time of all gcode objects
  void updateTotalTime();

private:

  // Update timer
  QTimer updateTimer_;

  // The UI
  GCodeToolbarWidget* tool_;

  // The Plugin icon
  QIcon* toolIcon_;

  // Update timer for gcode visualization
  QTimer* gcode_play_timer;
};


#endif //GCODEPLUGIN_HH