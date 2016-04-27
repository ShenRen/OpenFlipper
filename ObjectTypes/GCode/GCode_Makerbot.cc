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


#include "GCode_Makerbot.hh"

#include "GCodeEdgeTypes.hh"

void GCode_Makerbot::add_header(GCode::Shared _gcode, const Ultimaker::PrinterSettings& _printer, const Options& _options){



    ACG::Vec3d waiting_point(-110.5, -74.0, 150.0);
   // ACG::Vec3d waiting_point(-105.0, -73.0, 150.0); // Air outtake installed
    ACG::Vec3d anchor_point = waiting_point; anchor_point[2] = 0.6;

    _gcode->push_back(new GCode::cComment(" FLAVOR:ACG_GCode for Makerbot"));
    _gcode->push_back(new GCode::cComment(" OF GCode"));
    _gcode->push_back(new GCode::cComment("(Header start)"));
    _gcode->push_back(new GCode::cStopExtruder()); // Disable fan
    _gcode->push_back(new GCode::cSetBuildPercentage(0));
    _gcode->push_back(new GCode::cSetUnitMode(GCode::Unit_Millimeters)); // Set metric units
    _gcode->push_back(new GCode::cSetPositioningMode(GCode::PositioningMode_Absolute)); // Set absolute positioning mode

    _gcode->push_back(new GCode::cHeatExtruder(_options.temp_bed, 0)); // For makerbot this heats the bed ???
    _gcode->push_back(new GCode::cSetExtruderTemperature(_options.temp_head, 0)); // Heat the extruder
    if(_options.dual_support){
        _gcode->push_back(new GCode::cSetExtruderTemperature(_options.temp_head2, 1)); // Heat the extruder
    }

    _gcode->push_back(new GCode::cHomeAxisMax(GCode::AXIS_X | GCode::AXIS_Y, 2500)); // Home XY axes maximum
    _gcode->push_back(new GCode::cHomeAxisMin(GCode::AXIS_Z, 1100)); // Home Z axis minimum
    _gcode->push_back(new GCode::cSetPosition(GCode::AXIS_Z, ACG::Vec3d(0.0, 0.0, -5))); // Set Z to -5
    _gcode->push_back(new GCode::cMove(GCode::Move_Normal, 1100, GCode::AXIS_Z | GCode::AXIS_F, ACG::Vec3d(0.0, 0.0, 0.0))); // Set Z to -5
    _gcode->push_back(new GCode::cHomeAxisMin(GCode::AXIS_Z, 100)); // Home Z axis minimum
    _gcode->push_back(new GCode::cStoreHomeOffsets()); // Store home offsets

    _gcode->push_back(new GCode::cMove(GCode::Move_Normal, 3300, GCode::AXIS_X | GCode::AXIS_Y | GCode::AXIS_Z | GCode::AXIS_F, waiting_point)); // Move to waiting position
    _gcode->push_back(new GCode::cSetPotentiometerValue(20, 20, 20, 20, 20)); // Lower stepper Vrefs while heating
    _gcode->push_back(new GCode::cToolChange(0)); // Wait for tool 0 to be heated
    if(_options.dual_support){
        _gcode->push_back(new GCode::cToolChange(1)); // Wait for tool 1 to be heated
    }

    int current_a = 127;
    int current_b = 20;
    if(_options.dual_support){
        current_b = 127;
    }

    _gcode->push_back(new GCode::cSetPotentiometerValue(127, 127, 40, current_a, current_b)); // Set stepper vref to default

    _gcode->push_back(new GCode::cSetExtruderSpeed(3.0, 0)); // Set extruder speed
    _gcode->push_back(new GCode::cMove(GCode::Move_Rapid, 3300, GCode::AXIS_X | GCode::AXIS_Y, anchor_point)); // Position nozzle
    _gcode->push_back(new GCode::cMove(GCode::Move_Rapid, 3300, GCode::AXIS_Z, anchor_point)); // Position height
    _gcode->push_back(new GCode::cMove(GCode::Move_Normal, 50.0, GCode::AXIS_F | GCode::AXIS_E, anchor_point, 4.0)); // Create anchor
    _gcode->push_back(new GCode::cSetPosition(GCode::AXIS_E, anchor_point, 0.0)); // Create anchor

    _gcode->push_back(new GCode::cComment("(Header end)"));
    _gcode->push_back(new GCode::cSetExtruderSpeed(3.0, 0)); // Set extruder speed

    _gcode->push_back(new GCode::cStartExtruder()); // Start extruder
    //_gcode->push_back(new GCode::cSleep(2000)); // Create anchor
}

void GCode_Makerbot::add_line(GCode::Shared _gcode, const Ultimaker::PrinterSettings& _printer, const Options& _options, int _line_type, const ACG::Vec3d& _dst, int _axis, int _speed, double _extrude){
    _gcode->push_back(new GCode::cMove(GCode::Move_Normal, _speed, _axis, _dst, _extrude));
}

void GCode_Makerbot::add_retract(GCode::Shared _gcode, const Ultimaker::PrinterSettings& _printer, const Options& _options, double _extrude){
    double retract_length = std::min(1.0, _extrude);

    _gcode->push_back(new GCode::cComment("(Retract start)"));
    _gcode->push_back(new GCode::cMove(GCode::Move_Normal, 4800, GCode::AXIS_F));
    _gcode->push_back(new GCode::cMove(GCode::Move_Normal, 1200, GCode::AXIS_E, ACG::Vec3d(0.0, 0.0, 0.0), _extrude-retract_length));
    _gcode->push_back(new GCode::cMove(GCode::Move_Normal, 3000, GCode::AXIS_F));
    _gcode->push_back(new GCode::cStopExtruder);
    _gcode->push_back(new GCode::cComment("(Retract end)"));
}

void GCode_Makerbot::add_unretract(GCode::Shared _gcode, const Ultimaker::PrinterSettings& _printer, const Options& _options, double _extrude){
    _gcode->push_back(new GCode::cComment("(Un-Retract start)"));
    _gcode->push_back(new GCode::cMove(GCode::Move_Normal, 4800, GCode::AXIS_F));
    _gcode->push_back(new GCode::cMove(GCode::Move_Normal, 1200, GCode::AXIS_E, ACG::Vec3d(0.0, 0.0, 0.0), _extrude));
    _gcode->push_back(new GCode::cMove(GCode::Move_Normal, 3000, GCode::AXIS_F));
    _gcode->push_back(new GCode::cStartExtruder);
    _gcode->push_back(new GCode::cComment("(Un-Retract end)"));
}

void GCode_Makerbot::add_wait(GCode::Shared _gcode, const Ultimaker::PrinterSettings& _printer, const Options& _options, double _seconds){
    _gcode->push_back(new GCode::cSleep(_seconds*1000.0));
}

void GCode_Makerbot::add_footer(GCode::Shared _gcode, const Ultimaker::PrinterSettings& _printer, const Options& _options){
    _gcode->push_back(new GCode::cComment("(Footer start)"));
    _gcode->push_back(new GCode::cSetBuildPercentage(100));
    _gcode->push_back(new GCode::cSetExtruderTemperature(0));

    _gcode->push_back(new GCode::cMove(GCode::Move_Rapid, 1200, GCode::AXIS_Z, ACG::Vec3d(0.0, 0.0, 150.0)));
    _gcode->push_back(new GCode::cDisableStepper());

    _gcode->push_back(new GCode::cHeatExtruder(0, 0));
    _gcode->push_back(new GCode::cSetExtruderTemperature(0, 0));
    _gcode->push_back(new GCode::cSetExtruderTemperature(0, 1));

    _gcode->push_back(new GCode::cHomeAxisMax(GCode::AXIS_X | GCode::AXIS_Y, 2500));
    _gcode->push_back(new GCode::cDisableStepper());
    _gcode->push_back(new GCode::cPlaySong(1));
    _gcode->push_back(new GCode::cComment("(Footer end)"));
}

void GCode_Makerbot::add_build_percentage(GCode::Shared _gcode, const Ultimaker::PrinterSettings& _printer, const Options& _options, int _percent){
    _gcode->push_back(new GCode::cSetBuildPercentage(_percent));
}

void GCode_Makerbot::add_fan(GCode::Shared _gcode, const Ultimaker::PrinterSettings& _printer, const Options& _options, int _value){
    _gcode->push_back(new GCode::cSetFanSpeed(_value, -1));
    _gcode->push_back(new GCode::cSetFanSpeed(_value, 0));
    _gcode->push_back(new GCode::cSetFanSpeed(_value, 1));
}

void GCode_Makerbot::add_change_tool(GCode::Shared _gcode, const Ultimaker::PrinterSettings& _printer, const Options& _options, ACG::Vec3d _position, int _tool_id){

    _gcode->push_back(new GCode::cComment("(Change tool start)"));
    _gcode->push_back(new GCode::cSetCoordinateSystem(_tool_id));
    //_gcode->push_back(new GCode::cToolChange(_tool_id));
    _gcode->push_back(new GCode::cMove(GCode::Move_Normal, 3000.0, GCode::AXIS_F, _position));
    _gcode->push_back(new GCode::cSetExtruderSpeed(0.0, (_tool_id+1)%2));
    _gcode->push_back(new GCode::cSetExtruderSpeed(3.0, _tool_id));
    _gcode->push_back(new GCode::cMove(GCode::Move_Normal, 3000.0, GCode::AXIS_F | GCode::AXIS_X | GCode::AXIS_Y | GCode::AXIS_Z, _position));
    _gcode->push_back(new GCode::cComment("(Change tool end)"));
}

/*

GCode::Generator::TimeStats GCode_Makerbot::generate_gcode(GCode::Shared _gcode, const ptr::shared_ptr<PolyLine>& _line, double _layer_height, const Ultimaker::PrinterSettings& _printer, const Options& _options){
    const double diameter = _printer.filament_diameter;
    const double diameter_r = diameter/2.0;

    //const double wait_time = 5.0;

    double total_extrude = 0.0;

    if(_line->n_vertices() < 2){
        std::cerr << "Cannot create gcode from " << _line->n_vertices() << " vertices" << std::endl;
    }

    _gcode->clear();
    _gcode->push_back(new GCode::cComment(" FLAVOR:ACG_GCode for Makerbot"));
    _gcode->push_back(new GCode::cComment(" OF GCode"));
    _gcode->push_back(new GCode::cStopExtruder()); // Disable fan
    _gcode->push_back(new GCode::cSetBuildPercentage(0));
    _gcode->push_back(new GCode::cSetUnitMode(GCode::Unit_Millimeters)); // Set metric units
    _gcode->push_back(new GCode::cSetPositioningMode(GCode::PositioningMode_Absolute)); // Set absolute positioning mode

    _gcode->push_back(new GCode::cHeatExtruder(70.0, 0)); // For makerbot this heats the bed ???
    _gcode->push_back(new GCode::cSetExtruderTemperature(190.0, 0)); // Heat the extruder

    _gcode->push_back(new GCode::cHomeAxisMax(GCode::AXIS_X | GCode::AXIS_Y, 2500)); // Home XY axes maximum
    _gcode->push_back(new GCode::cHomeAxisMin(GCode::AXIS_Z, 1100)); // Home Z axis minimum
    _gcode->push_back(new GCode::cSetPosition(GCode::AXIS_Z, ACG::Vec3d(0.0, 0.0, -5))); // Set Z to -5
    _gcode->push_back(new GCode::cMove(GCode::Move_Normal, 1100, GCode::AXIS_Z | GCode::AXIS_F, ACG::Vec3d(0.0, 0.0, 0.0))); // Set Z to -5
    _gcode->push_back(new GCode::cHomeAxisMin(GCode::AXIS_Z, 100)); // Home Z axis minimum
    _gcode->push_back(new GCode::cStoreHomeOffsets()); // Store home offsets

    _gcode->push_back(new GCode::cMove(GCode::Move_Normal, 3300, GCode::AXIS_X | GCode::AXIS_Y | GCode::AXIS_Z | GCode::AXIS_F, ACG::Vec3d(-110.5, -74.0, 150.0))); // Move to waiting position
    _gcode->push_back(new GCode::cSetPotentiometerValue(20, 20, 20, 20, 20)); // Lower stepper Vrefs while heating
    _gcode->push_back(new GCode::cToolChange(0)); // Set stepper vref to default
    _gcode->push_back(new GCode::cSetPotentiometerValue(127, 127, 40, 127, 127)); // Set stepper vref to default

    _gcode->push_back(new GCode::cSetExtruderSpeed(3.0, 0)); // Set extruder speed
    _gcode->push_back(new GCode::cMove(GCode::Move_Rapid, 3300, GCode::AXIS_X | GCode::AXIS_Y, ACG::Vec3d(-110.5, -74.0, 0.6))); // Position nozzle
    _gcode->push_back(new GCode::cMove(GCode::Move_Rapid, 3300, GCode::AXIS_Z, ACG::Vec3d(-110.5, -74.0, 0.6))); // Position height
    _gcode->push_back(new GCode::cSetExtruderSpeed(5.0, 0)); // Set extruder speed

    _gcode->push_back(new GCode::cStartExtruder()); // Start extruder
    _gcode->push_back(new GCode::cSleep(2000)); // Create anchor

    int last_layer_id = -1;

    // Move to the first vertex
    _gcode->push_back(new GCode::cMove(GCode::Move_Rapid, 3000, GCode::AXIS_F | GCode::AXIS_X | GCode::AXIS_Y | GCode::AXIS_Z, _line->point(0)));

    SpeedMode speed_mode = Speed_Normal;
    double part_time = get_part_time(_printer, 0, _line, 0, Speed_Normal);

    double timings[Ultimaker::Count];
    for(int i = 0; i < Ultimaker::Count; ++i){
        timings[i] = 0.0;
    }
    double total_time = 0.0;

    int last_build_percentage = 0;

    int last_speed = 3000;
    for(size_t i = 1; i < _line->n_vertices(); ++i){
        int layer_id = int(round((_line->point(i)[2]-_printer.zero_layer)/_layer_height));
        int edge_type = int(_line->edge_scalar(i-1));

        int build_percentage = int(100.0 * double(i+1) / double(_line->n_vertices()));
        if(build_percentage > last_build_percentage){
            last_build_percentage = build_percentage;
            _gcode->push_back(new GCode::cSetBuildPercentage(build_percentage));
        }

        if(edge_type == Ultimaker::GC_RETRACT){

            _gcode->push_back(new GCode::cMove(GCode::Move_Normal, 1200, GCode::AXIS_F));
            _gcode->push_back(new GCode::cMove(GCode::Move_Normal, 1200, GCode::AXIS_E, ACG::Vec3d(0.0, 0.0, 0.0), total_extrude-1.0));
            _gcode->push_back(new GCode::cMove(GCode::Move_Normal, 3000, GCode::AXIS_F));
            _gcode->push_back(new GCode::cStopExtruder);

            continue;
        } else if(edge_type == Ultimaker::GC_UNRETRACT){


            _gcode->push_back(new GCode::cMove(GCode::Move_Normal, 1200, GCode::AXIS_F));
            _gcode->push_back(new GCode::cMove(GCode::Move_Normal, 1200, GCode::AXIS_E, ACG::Vec3d(0.0, 0.0, 0.0), total_extrude));
            _gcode->push_back(new GCode::cMove(GCode::Move_Normal, 3000, GCode::AXIS_F));
            _gcode->push_back(new GCode::cStartExtruder);

            last_speed = 3000;
            continue;
        }else if(edge_type == Ultimaker::GC_PART || edge_type == Ultimaker::GC_PART_SYNC){

            if(_options.wait_for_layer_time && edge_type == Ultimaker::GC_PART_SYNC){
                // Calculate time to wait and do wait if required
                if(part_time < _options.min_part_time){
                    double seconds_to_wait = _options.min_part_time - part_time;
                    _gcode->push_back(new GCode::cRetract(GCode::RetractMode_Retract));
                    _gcode->push_back(new GCode::cMove(GCode::Move_Normal, 9000, GCode::AXIS_F | GCode::AXIS_Z,
                                                       _line->point(i) + ACG::Vec3d(0.0, 0.0, 2.0), total_extrude));
                    _gcode->push_back(new GCode::cSleep(seconds_to_wait*1000.0));
                    _gcode->push_back(new GCode::cMove(GCode::Move_Normal, 9000, GCode::AXIS_F | GCode::AXIS_Z,
                                                       _line->point(i), total_extrude));
                    _gcode->push_back(new GCode::cRetract(GCode::RetractMode_Unretract));
                    last_speed = 9000;
                }
            }

            // Find the next part entry and get the time needed
            if(_options.dynamic_speed){
                part_time = get_part_time(_printer, layer_id, _line, i, Speed_Normal);
                if(part_time >= _options.min_part_time){
                    speed_mode = Speed_Normal;
                }else{
                    part_time = get_part_time(_printer, layer_id, _line, i, Speed_Slow);

                    if(part_time >= _options.min_part_time){
                        speed_mode = Speed_Slow;
                    }else{
                        part_time = get_part_time(_printer, layer_id, _line, i, Speed_SuperSlow);
                        speed_mode = Speed_SuperSlow;
                    }
                }
            }

            continue;
        }



        if(last_layer_id != layer_id){
            // On layer change update fan speed
            _gcode->push_back(new GCode::cComment(QString("LAYER:%1").arg(layer_id).toStdString()));

            if(layer_id == 0)
                _gcode->push_back(new GCode::cSetFanSpeed(0));
            else if(layer_id ==1)
                _gcode->push_back(new GCode::cSetFanSpeed(63));
            else if(layer_id ==2)
                _gcode->push_back(new GCode::cSetFanSpeed(127));
            else if(layer_id ==3)
                _gcode->push_back(new GCode::cSetFanSpeed(191));
            else if(layer_id == 4)
                _gcode->push_back(new GCode::cSetFanSpeed(255));
            else if(layer_id > 4 && last_layer_id <= 4)
                _gcode->push_back(new GCode::cSetFanSpeed(255));
        }

        double line_thickness = _printer.nozzle_size;
        double layer_thickness = _layer_height;
        if(layer_id == 0){
            line_thickness *= 1.2;
            layer_thickness = _printer.zero_layer;
        }

        double length = _line->edge_vector(i-1).norm();
        double extrude = length*layer_thickness*line_thickness/(M_PI * (diameter/2.0)*(diameter/2.0));

        if(i+1 == _line->n_vertices()){
            length = 0.0;
            extrude = 0.0;
        }

        if(edge_type == Ultimaker::GC_MOVE){
            extrude = 0.0;
        }

        total_extrude += extrude;

        GCode::cMove move;
        move.axis() = GCode::AXIS_X | GCode::AXIS_Y;
        move.position() = _line->point(i);
        move.speed() = get_speed(_printer, layer_id, edge_type, speed_mode);
        if(edge_type != Ultimaker::GC_MOVE){
            move.axis() |= GCode::AXIS_E;
        }else{
            move.mode() = GCode::Move_Normal;
        }
        if(last_layer_id != layer_id){
            // Layer change
            move.axis() |= GCode::AXIS_Z;
        }
        if(last_speed != move.speed()){
            move.axis() |= GCode::AXIS_F;
        }
        move.extrude() = total_extrude;
        _gcode->push_back(new GCode::cMove(move));

        if(std::isnan(_line->point(i)[0])){
            std::cout << "NAN in creation" << std::endl;
        }

        double d_time = 60.0 * (length/double(move.speed()));
        timings[edge_type] += d_time;
        total_time += d_time;

        last_layer_id = layer_id;
        last_speed = move.speed();
    }

    _gcode->push_back(new GCode::cSetBuildPercentage(100));
    _gcode->push_back(new GCode::cSetExtruderTemperature(0));

    _gcode->push_back(new GCode::cMove(GCode::Move_Rapid, 1200, GCode::AXIS_Z, ACG::Vec3d(0.0, 0.0, 150.0)));
    _gcode->push_back(new GCode::cDisableStepper());

    _gcode->push_back(new GCode::cHeatExtruder(0, 0));
    _gcode->push_back(new GCode::cSetExtruderTemperature(0, 0));
    _gcode->push_back(new GCode::cSetExtruderTemperature(0, 1));

    _gcode->push_back(new GCode::cHomeAxisMax(GCode::AXIS_X | GCode::AXIS_Y, 2500));
    _gcode->push_back(new GCode::cDisableStepper());
    _gcode->push_back(new GCode::cPlaySong(1));


    std::vector<GCode::Item> timing_items;
    timing_items.push_back(new GCode::cComment(QString(" Total(sec): %2").arg(total_time).toStdString()));
    for(int i = 0; i < Ultimaker::Count; ++i){
        if(timings[i] > 0.0){
            timing_items.push_back(new GCode::cComment(QString(" %1: %2").arg(QString::fromStdString(Ultimaker::EdgeType_names[i])).arg(timings[i]).toStdString()));

            std::cout << Ultimaker::EdgeType_names[i] << ": " << 100.0*(timings[i]/total_time) << " %" << std::endl;
        }
    }
    _gcode->insert(_gcode->begin(), timing_items.begin(), timing_items.end());

    TimeStats stats;
    stats.total_time = total_time;
    stats.move = timings[Ultimaker::GC_MOVE];
    stats.outer_wall = timings[Ultimaker::GC_WALL_OUTER];
    stats.inner_wall = timings[Ultimaker::GC_WALL_INNER];
    stats.infill = timings[Ultimaker::GC_INFILL];
    stats.topbottom = timings[Ultimaker::GC_TOPBOTTOM];
    return stats;
}
*/
