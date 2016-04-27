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


#include "GCode_Ultimaker2.hh"

#include "GCodeEdgeTypes.hh"

void GCode_Ultimaker2::add_header(GCode::Shared _gcode, const Ultimaker::PrinterSettings& _printer, const Options& _options){
    _gcode->push_back(new GCode::cComment(" FLAVOR:ACG_GCode for Ultimaker 2"));
    _gcode->push_back(new GCode::cComment(" OF GCode"));
    _gcode->push_back(new GCode::cHeatBed(_options.temp_bed));
    _gcode->push_back(new GCode::cHeatExtruder(_options.temp_head)); // Heat the extruder
    _gcode->push_back(new GCode::cSetUnitMode(GCode::Unit_Millimeters)); // Set metric units
    _gcode->push_back(new GCode::cSetPositioningMode(GCode::PositioningMode_Absolute)); // Set absolute positioning mode
    _gcode->push_back(new GCode::cExtruderMode(GCode::PositioningMode_Absolute)); // Set absolute extruder mode
    _gcode->push_back(new GCode::cSetFanSpeed(0)); // Disable fan
    _gcode->push_back(new GCode::cMoveToOrigin(GCode::AXIS_X | GCode::AXIS_Y)); // move X/Y to endstop
    _gcode->push_back(new GCode::cMoveToOrigin(GCode::AXIS_Z)); // move Z to endstop
    _gcode->push_back(new GCode::cMove(GCode::Move_Normal, 9000, GCode::AXIS_F | GCode::AXIS_Z, ACG::Vec3d(0.0, 0.0, 15.0))); // move the platform down 15mm
    _gcode->push_back(new GCode::cSetPosition(GCode::AXIS_E, ACG::Vec3d(0.0, 0.0, 0.0), 0.0)); // Zero extruder length
    _gcode->push_back(new GCode::cMove(GCode::Move_Normal, 200, GCode::AXIS_F | GCode::AXIS_E, ACG::Vec3d(0.0, 0.0, 0.0), 4.0)); // Extrude 4mm of feed stock
    _gcode->push_back(new GCode::cSetPosition(GCode::AXIS_E, ACG::Vec3d(0.0, 0.0, 0.0), 0.0)); // Zero extruder length
    _gcode->push_back(new GCode::cMove(GCode::Move_Normal, 9000)); // Set speed to 150mm/s
    _gcode->push_back(new GCode::cComment(" Put printing message on LCD screen"));
    _gcode->push_back(new GCode::cDisplayMessage("Printing via OpenFlipper ..."));
}

void GCode_Ultimaker2::add_line(GCode::Shared _gcode, const Ultimaker::PrinterSettings& _printer, const Options& _options, int _line_type, const ACG::Vec3d& _dst, int _axis, int _speed, double _extrude){
    GCode::MoveMode mode = GCode::Move_Normal;
    if(_line_type == Ultimaker::GC_MOVE){
        mode = GCode::Move_Rapid;
    }

    _gcode->push_back(new GCode::cMove(mode, _speed, _axis, _dst, _extrude));
}

void GCode_Ultimaker2::add_retract(GCode::Shared _gcode, const Ultimaker::PrinterSettings& _printer, const Options& _options, double _extrude){
    _gcode->push_back(new GCode::cRetract(GCode::RetractMode_Retract));
}

void GCode_Ultimaker2::add_unretract(GCode::Shared _gcode, const Ultimaker::PrinterSettings& _printer, const Options& _options, double _extrude){
    _gcode->push_back(new GCode::cRetract(GCode::RetractMode_Unretract));
}

void GCode_Ultimaker2::add_wait(GCode::Shared _gcode, const Ultimaker::PrinterSettings& _printer, const Options& _options, double _seconds){
    _gcode->push_back(new GCode::cSleep(_seconds*1000.0));
}

void GCode_Ultimaker2::add_footer(GCode::Shared _gcode, const Ultimaker::PrinterSettings& _printer, const Options& _options){
    _gcode->push_back(new GCode::cSetExtruderTemperature(0));
    _gcode->push_back(new GCode::cSetFanSpeed(0));
    _gcode->push_back(new GCode::cSetBedTemperature(0));
    _gcode->push_back(new GCode::cSetPositioningMode(GCode::PositioningMode_Relative));
    _gcode->push_back(new GCode::cMove(GCode::Move_Normal, 300, GCode::AXIS_F | GCode::AXIS_E, ACG::Vec3d(0.0, 0.0, 0.0), -1.0));
    _gcode->push_back(new GCode::cMove(GCode::Move_Normal, 9000, GCode::AXIS_F | GCode::AXIS_X | GCode::AXIS_Y | GCode::AXIS_Z | GCode::AXIS_E, ACG::Vec3d(-20.0, -20.0, 1.5), -5.0));
    _gcode->push_back(new GCode::cMoveToOrigin(GCode::AXIS_X | GCode::AXIS_Y));
    _gcode->push_back(new GCode::cStopIdleHold());
    _gcode->push_back(new GCode::cSetPositioningMode(GCode::PositioningMode_Absolute));
}

void GCode_Ultimaker2::add_build_percentage(GCode::Shared _gcode, const Ultimaker::PrinterSettings& _printer, const Options& _options, int _percent){
    //_gcode->push_back(new GCode::cDisplayMessage(QString("Progress: %1 %").arg(_percent).toStdString()));
}

void GCode_Ultimaker2::add_fan(GCode::Shared _gcode, const Ultimaker::PrinterSettings& _printer, const Options& _options, int _value){
    _gcode->push_back(new GCode::cSetFanSpeed(_value, 0));
}

void GCode_Ultimaker2::add_change_tool(GCode::Shared _gcode, const Ultimaker::PrinterSettings& _printer, const Options& _options, ACG::Vec3d _position, int _tool_id){

}
