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


#include "GCode_Generator.hh"

GCode::Generator::TimeStats GCode_Generator::generate_gcode(GCode::Shared _gcode, const ptr::shared_ptr<PolyLine>& _line, double _layer_height, const Ultimaker::PrinterSettings& _printer, const Options& _options){
    const double diameter = _printer.filament_diameter;

    double total_extrude[2];// = 0.0;
    total_extrude[0] = 0.0;
    total_extrude[1] = 0.0;

    if(_line->n_vertices() < 2){
        std::cerr << "Cannot create gcode from " << _line->n_vertices() << " vertices" << std::endl;
    }

    _gcode->clear();
    add_header(_gcode, _printer, _options);

    int last_layer_id = -1;

    int init_speed = int(60.0*_printer.speeds[Ultimaker::GC_MOVE]);

    int current_tool = 0;
    if(_line->edge_scalar(0) == Ultimaker::GC_SUPPORT){
        current_tool = 1;
    }


    // Extrude 1mm of material on both extruders
 /*   add_change_tool(_gcode, _printer, _options, _line->point(0), 0);
    add_line(_gcode, _printer, _options, Ultimaker::GC_MOVE, _line->point(0), GCode::AXIS_F | GCode::AXIS_E, init_speed, 1.0);
    total_extrude[0] = 1.0;

    if(_options.dual_support){
        add_change_tool(_gcode, _printer, _options, _line->point(0), 1);
        add_line(_gcode, _printer, _options, Ultimaker::GC_MOVE, _line->point(0), GCode::AXIS_F | GCode::AXIS_E, init_speed, 1.0);
        total_extrude[1] = 1.0;
    }*/

    // Select the right tool
   // add_change_tool(_gcode, _printer, _options, _line->point(0), current_tool);

    // Move to the first vertex
    add_line(_gcode, _printer, _options, Ultimaker::GC_MOVE, _line->point(0), GCode::AXIS_F | GCode::AXIS_X | GCode::AXIS_Y | GCode::AXIS_Z, init_speed, 0.0);
    //total_extrude[current_tool] = 1.0;

    // Extrude 1mm of filament
    //add_unretract(_gcode, _printer, _options, 1.0);

    SpeedMode speed_mode = Speed_Normal;
    double part_time = get_part_time(_printer, 0, _line, 0, Speed_Normal);

    double timings[Ultimaker::GC_COUNT];
    for(int i = 0; i < Ultimaker::GC_COUNT; ++i){
        timings[i] = 0.0;
    }
    double total_time = 0.0;

    int last_build_percentage = 0;

    int last_speed = init_speed;
    for(size_t i = 1; i < _line->n_vertices(); ++i){
        int layer_id = int(round((_line->point(i)[2]-_printer.zero_layer)/_layer_height));
        int edge_type = int(_line->edge_scalar(i-1));

        int build_percentage = int(100.0 * double(i+1) / double(_line->n_vertices()));
        if(build_percentage > last_build_percentage){
            last_build_percentage = build_percentage;
            add_build_percentage(_gcode, _printer, _options, build_percentage);
        }

        if(edge_type == Ultimaker::GC_RETRACT){
            add_retract(_gcode, _printer, _options, total_extrude[current_tool]);
            continue;
        } else if(edge_type == Ultimaker::GC_UNRETRACT){
            add_unretract(_gcode, _printer, _options, total_extrude[current_tool]);
            continue;
        }else if(edge_type == Ultimaker::GC_PART || edge_type == Ultimaker::GC_PART_SYNC){

            if(_options.wait_for_layer_time && edge_type == Ultimaker::GC_PART_SYNC){
                // Calculate time to wait and do wait if required
                if(part_time < _options.min_part_time){
                    double seconds_to_wait = _options.min_part_time - part_time;
                    add_wait(_gcode, _printer, _options, seconds_to_wait);
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

            int zero_layer = 0;
            if(_options.raft){
                zero_layer += 2;
            }

            if(layer_id <= zero_layer)
                add_fan(_gcode, _printer, _options, 0);
            else if(layer_id == zero_layer+1)
                add_fan(_gcode, _printer, _options, 63);
            else if(layer_id == zero_layer+2)
                add_fan(_gcode, _printer, _options, 127);
            else if(layer_id == zero_layer+3)
                add_fan(_gcode, _printer, _options, 191);
            else if(layer_id == zero_layer+4)
                add_fan(_gcode, _printer, _options, 255);
            else if(layer_id > zero_layer+4 && last_layer_id <= zero_layer+4)
                add_fan(_gcode, _printer, _options, 255);
        }


        if(_options.dual_support && edge_type != Ultimaker::GC_MOVE){
            int new_tool = 0;

            if(edge_type == Ultimaker::GC_SUPPORT){
                new_tool = 1;
            }else{
                new_tool = 0;
            }

            if(new_tool != current_tool){
                add_retract(_gcode, _printer, _options, total_extrude[current_tool]);
                current_tool = new_tool;
                add_change_tool(_gcode, _printer, _options, _line->point(i-1), current_tool);
                add_unretract(_gcode, _printer, _options, total_extrude[current_tool]);
            }
        }



        double line_thickness = _printer.nozzle_size;
        double flow = _printer.flows[edge_type]/100.0;
        if((layer_id == 0) && (edge_type != Ultimaker::GC_BRIM)){ //on layer 0 all except Brims get special value
            flow = _printer.flow_first_layer/100.0;
        }

        double length = _line->edge_vector(i-1).norm();
        double extrude = length*_layer_height*line_thickness/(M_PI * (diameter/2.0)*(diameter/2.0))*flow;

        if(i+1 == _line->n_vertices()){
            length = 0.0;
            extrude = 0.0;
        }

        if(edge_type == Ultimaker::GC_MOVE){
            extrude = 0.0;
        }

        total_extrude[current_tool] += extrude;

        int axis = GCode::AXIS_X | GCode::AXIS_Y;
        int speed = get_speed(_printer, layer_id, edge_type, speed_mode);

        /*if(edge_type == Ultimaker::GC_SUPPORT){
            std::cout << "Support speed: " << speed/60 << std::endl;
        }else{
            std::cout << "Other speed: " << speed/60 << std::endl;
        }*/

        if(edge_type != Ultimaker::GC_MOVE){
            axis |= GCode::AXIS_E;
        }
        if(last_layer_id != layer_id){
            axis |= GCode::AXIS_Z;
        }
        if(last_speed != speed){
            axis |= GCode::AXIS_F;
        }
        add_line(_gcode, _printer, _options, edge_type, _line->point(i), axis, speed, total_extrude[current_tool]);

        double d_time = 60.0 * (length/double(speed));
        timings[edge_type] += d_time;
        total_time += d_time;

        last_layer_id = layer_id;
        last_speed = speed;
    }

    add_footer(_gcode, _printer, _options);

    std::vector<GCode::Item> timing_items;
    timing_items.push_back(new GCode::cComment(QString(" Total(sec): %2").arg(total_time).toStdString()));
    for(int i = 0; i < Ultimaker::GC_COUNT; ++i){
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

int GCode_Generator::get_speed(const Ultimaker::PrinterSettings& _printer, int _layer_id, int _line_type, SpeedMode _mode){
    if(_line_type > Ultimaker::GC_BRIM || _line_type < 0){
        return int(60.0*_printer.speeds[Ultimaker::GC_MOVE]);
    }

    int speed = int(60.0*_printer.speeds[_line_type]);

    if(_line_type != Ultimaker::GC_MOVE){
        if(_mode == Speed_Slow){
            speed /= 2;
        }else if(_mode == Speed_SuperSlow){
            speed /= 3;
        }

        if((_layer_id < 4) && (_line_type != Ultimaker::GC_BRIM)){ // on lowest layer all except brims get special value
            double i_val = double(_layer_id)/4.0;
            int first_layer_speed = int(60.0*_printer.speed_first_layer);

            speed = double(first_layer_speed)*(1.0-i_val) + double(speed)*i_val;
        }
    }

    return speed;
}

int GCode_Generator::get_next_part_type(const ptr::shared_ptr<PolyLine>& _line, int _start){
    int n_vertices = int(_line->n_vertices());

    int from = _start+1;
    int to;
    for(to = from; to <= n_vertices; ++to){
        int edge_type = round(_line->edge_scalar(to));
        if(edge_type == Ultimaker::GC_PART || edge_type == Ultimaker::GC_PART_SYNC){
            return edge_type;
        }
    }
    return Ultimaker::GC_MOVE;
}

double GCode_Generator::get_part_time(const Ultimaker::PrinterSettings& _printer, int _layer_id, const ptr::shared_ptr<PolyLine>& _line, int _start, SpeedMode _mode){
    int n_vertices = int(_line->n_vertices());

    int from = _start+1;
    int to;
    for(to = from; to <= n_vertices; ++to){
        int edge_type = round(_line->edge_scalar(to));
        if(edge_type == Ultimaker::GC_PART || edge_type == Ultimaker::GC_PART_SYNC){
            break;
        }
    }

    double time = 0.0;
    if(to > from){
        for(int i = from; i < to; ++i){
            int edge_type = round(_line->edge_scalar(i));

            double distance = _line->edge_vector(i).norm();
            double speed = get_speed(_printer, _layer_id, edge_type, _mode);

            double edge_time = 60.0 * distance / speed;
            time += edge_time;
        }
    }
    return time;
}

