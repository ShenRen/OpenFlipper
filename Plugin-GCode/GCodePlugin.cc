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


//== INCLUDES =================================================================


#include "GCodePlugin.hh"
#include <OpenFlipper/BasePlugin/PluginFunctions.hh>
#include <OpenFlipper/Common/GlobalOptions.hh>

// #if QT_VERSION >= 0x050000
// #include <QtWidgets>
// #else
// #include <QtGui>
// #endif

/**
* @brief GCodePlugin::slotGCodePositionBoxChanged
*/
void GCodePlugin::slotGCodePositionBoxChanged()
{

  QTime time = tool_->te_gcode_time->time();
  double seconds = double(time.second() + 60 * (time.minute() + 60 * time.hour())) + double(time.msec()) / 1000.0;

  tool_->slider_gcode->blockSignals(true);
  tool_->slider_gcode->setValue(seconds);
  tool_->slider_gcode->blockSignals(false);

  GCodeObject* gcode_object;
  PluginFunctions::getObject(gcode_object_id_, gcode_object);

  if (gcode_object)
  {
    gcode_object->set_distance(seconds);

    ptr::shared_ptr<PolyLine> polyline = gcode_object->gcode()->polyline();
    const std::vector<GCode::State>& states = gcode_object->gcode()->polyline_states();

    GCode::State state, next_state;
    for (size_t i = 1; i < polyline->n_vertices(); ++i)
    {
      if (states[i].time > seconds)
      {
        state = states[i - 1];
        next_state = states[i];
        break;
      }
    }

    double x = 0.0;
    if (fabs((next_state.time - state.time)) > 0.0)
    {
      x = (seconds - state.time) / (next_state.time - state.time);
    }

    if (state.unit == GCode::Unit_Millimeters)
    {
      tool_->lbl_gcode_unit->setText(QString("Unit: Metric"));
    }
    else
    {
      tool_->lbl_gcode_unit->setText(QString("Unit: Inch"));
    }

    if (state.mode_positioning == GCode::PositioningMode_Absolute)
    {
      tool_->lbl_gcode_posmode->setText(QString("Position Mode: Absolute"));
    }
    else
    {
      tool_->lbl_gcode_posmode->setText(QString("Position Mode: Relative"));
    }

    if (state.mode_extruder == GCode::PositioningMode_Absolute)
    {
      tool_->lbl_gcode_extmode->setText(QString("Position Mode: Absolute"));
    }
    else
    {
      tool_->lbl_gcode_extmode->setText(QString("Extruder Mode: Relative"));
    }

    tool_->lbl_gcode_fan->setText(QString("Fan Speed: %1%").arg(100.0*double(state.fan_speed) / 255.0));
    tool_->lbl_gcode_extrude->setText(QString("Extrude: %1 mm").arg(state.pos_extruder*(1.0 - x) + next_state.pos_extruder*x));

    tool_->lbl_gcode_temp_ext->setText(QString("Temp Extruder: %1°").arg(state.temp_extruder));
    tool_->lbl_gcode_temp_bed->setText(QString("Temp Bed: %1°").arg(state.temp_bed));

    emit updateView();
  }
}

/**
* @brief GCodePlugin::slotSliderGCodeValueChanged
*/
void GCodePlugin::slotSliderGCodeValueChanged()
{
  double distance = double(tool_->slider_gcode->value());

  int seconds = distance;
  int minutes = seconds / 60;
  int hours = minutes / 60;

  seconds %= 60;
  minutes %= 60;

  QTime time;
  time.setHMS(hours, minutes, seconds);

  tool_->te_gcode_time->setTime(time);
}

/**
* @brief GCodePlugin::initializePlugin
*/
void GCodePlugin::initializePlugin()
{
  // Add the Toolbar to OpenFlipper Core
  QIcon* icon = new QIcon(OpenFlipper::Options::iconDirStr() + OpenFlipper::Options::dirSeparator() + "GCodeType.png");

  tool_ = new GCodeToolbarWidget();
  QSize size(300, 300);
  tool_->resize(size);

  gcode_object_id_ = -1;

  connect(tool_->slider_gcode, SIGNAL(valueChanged(int)), this, SLOT(slotSliderGCodeValueChanged()));
  connect(tool_->te_gcode_time, SIGNAL(timeChanged(QTime)), this, SLOT(slotGCodePositionBoxChanged()));

  connect(tool_->pb_gcode_play, SIGNAL(clicked()), this, SLOT(onGCodePlayPressed()));

  emit addToolbox("GCode", tool_, icon);
}

/**
* @brief GCodePlugin::~GCodePlugin
*/
GCodePlugin::~GCodePlugin()
{
}

/**
* @brief GCodePlugin::onGCodePlayPressed
*/
void GCodePlugin::onGCodePlayPressed()
{
  if (gcode_play_timer)
  {
    // Stop
    gcode_play_timer->stop();
    gcode_play_timer->deleteLater();
    gcode_play_timer = 0;

    tool_->pb_gcode_play->setText("Start");
  }
  else
  {
    tool_->pb_gcode_play->setText("Stop");

    // Start
    gcode_play_timer = new QTimer(this);
    gcode_play_timer->setInterval(30);
    gcode_play_timer->setSingleShot(false);
    connect(gcode_play_timer, SIGNAL(timeout()), this, SLOT(onGCodePlayTimer()));
    gcode_play_timer->start();
  }
}

/**
* @brief GCodePlugin::onGCodePlayTimer
*/
void GCodePlugin::onGCodePlayTimer()
{
  double ms_passed = double(gcode_play_timer->interval());
  double forward_in_ms = tool_->ds_gcode_speed->value() * ms_passed;

  QTime time = tool_->te_gcode_time->time();
  tool_->te_gcode_time->setTime(time.addMSecs(forward_in_ms));
}
// 
// /**
// * @brief UltimakerPlugin::pb_load_gcode
// */
// void UltimakerPlugin::pb_load_gcode()
// {
//   QString filename = QFileDialog::getOpenFileName(tool_, QString("Read GCode"), QDir::currentPath(), "GCode files (*.gcode)");
// 
//   if (filename != "")
//   {
//     GCodeObject* gcode_object = create_gcode_object();
//     gcode_object_id_ = gcode_object->id();
// 
//     gcode_object->setName(QString("GCode %1").arg(gcode_object->id()));
//     gcode_object->gcode()->parse_from_file(filename.toStdString(), printer_settings_.nozzle_size);
//     gcode_object->update();
// 
//     int seconds = int(gcode_object->gcode()->total_time());
//     int minutes = seconds / 60;
//     int hours = minutes / 60;
// 
//     seconds %= 60;
//     minutes %= 60;
// 
//     tool_->lbl_gcode_total_time->setText(QString("/ %1:%2:%3").arg(hours).arg(minutes % 60, 2, 10, QChar('0')).arg(seconds % 60, 2, 10, QChar('0')));
// 
//     QTime time;
//     time.setHMS(hours, minutes, seconds);
// 
//     tool_->te_gcode_time->setMaximumTime(time);
//     tool_->slider_gcode->setMaximum(gcode_object->gcode()->total_time());
//     tool_->slider_gcode->setValue(gcode_object->gcode()->total_time());
// 
//     emit updateView();
//   }
// }

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(ultimakerplugin, UltimakerPlugin);
#endif
