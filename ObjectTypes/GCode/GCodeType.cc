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


#include "GCode.hh"

#include "GCodeEdgeTypes.hh"

#include <iomanip>      // std::setprecision

#include <set>

GCode::GCode(){
}

GCode::~GCode(){
}

void GCode::update_polyline(){
    std::pair<ptr::shared_ptr<PolyLine>, std::vector<State> > line = create_polyline();

    polyline_ = line.first;
    polyline_states_ = line.second;

    if(polyline_states_.size() > 0){
        total_time_ = polyline_states_[polyline_states_.size()-1].time;
    }
}

// Create polyline from gcode
std::pair<ptr::shared_ptr<PolyLine>, std::vector<GCode::State> > GCode::create_polyline(){
    std::pair<ptr::shared_ptr<PolyLine>, std::vector<GCode::State> > result;

    ptr::shared_ptr<PolyLine> polyline(new PolyLine());
    polyline->request_vertex_selections();
    polyline->request_edge_selections();
    polyline->request_vertex_vhandles();
    polyline->request_vertex_ehandles();
    polyline->request_vertex_fhandles();
    polyline->request_edge_scalars();

    std::vector<State> states;

    State state;
    polyline->add_point(state.pos);
    int current_idx = 0;
    states.push_back(state);

    //std::cout << "Time: " << state.time << std::endl;
    for(size_t i = 0; i < gcode_.size(); ++i){
        State state_before = state;

        Item item = gcode_[i];
        item->update_state(state);

        //if((state_before.pos - state.pos).norm() > 1e-5){
            polyline->add_point(state.pos);

            if(state.axis & AXIS_E && state.pos_extruder >= state_before.pos_extruder){
                polyline->edge_scalar(current_idx) = Ultimaker::GC_WALL_OUTER;
            }else{
                polyline->edge_scalar(current_idx) = Ultimaker::GC_MOVE;
            }

            states.push_back(state);

            current_idx++;
        //}
        state.max_extruder = std::max(state.max_extruder, state.pos_extruder);
    }

    result.first = polyline;
    result.second = states;

    return result;
}

bool GCode::parse_from_file(const std::string& _filename){
    std::ifstream in(_filename.c_str());

    if(!in){
        return false;
    }




    std::string line;
    gcode_.clear();
    while(in.good()){
        std::getline(in, line);

        GCode::Item item = GCode::ParseLine(QString::fromStdString(line));
        if(item){
            gcode_.push_back(item);
        }
    }


    std::set<double> layers;
    GCode::State state;
    for(size_t i = 0; i < gcode_.size(); ++i){
        GCode::State old_state = state;

        gcode_[i]->update_state(state);

        if(fabs(old_state.pos[2] - state.pos[2]) < 1e-4){
            if(state.pos[2] > 1e-4){
                layers.insert(state.pos[2]);
            }
        }
    }

    if(layers.size() < 2){
        return false;
    }
    std::set<double>::iterator it = layers.begin();

    layer_zero_ = *it; it++;
    layer_height_ = *it - layer_zero_;

    // default nozzle size
    nozzle_size_ = Ultimaker::PrinterSettings().nozzle_size;

    update_polyline();

    return true;
}

GCode::Generator::TimeStats GCode::create_from_polyline(Generator::Shared _gcode_generator, const ptr::shared_ptr<PolyLine>& _line, double _layer_height, const Ultimaker::PrinterSettings& _printer, const Generator::Options& _options){
    layer_zero_ = _printer.zero_layer;
    layer_height_ = _layer_height;
    nozzle_size_ = _printer.nozzle_size;

    GCode::Generator::TimeStats stats;

    if(_gcode_generator){
        stats = _gcode_generator->generate_gcode(shared_from_this(), _line, _layer_height, _printer, _options);
        update_polyline();
    }

    return stats;
}

/*void GCode::write(int _i, std::ostream& _stream){
    for(size_t j = 0; j < data(_i).before.size(); ++j){
        data(_i).before[j]->write(_stream);
    }

    data(_i).move.write(_stream);

    for(size_t j = 0; j < data(_i).after.size(); ++j){
        data(_i).after[j]->write(_stream);
    }
}*/

void GCode::write(std::ostream& _stream){
    _stream << std::fixed << std::setfill('0') << std::setprecision(3);

    for(size_t i = 0; i < gcode_.size(); ++i){
        gcode_[i]->write(_stream);
    }

    /*for(size_t i = 0; i < header_.size(); ++i){
        header_[i]->write(_stream);
    }

    _stream << std::fixed << std::setfill('0') << std::setprecision(3);

    for(size_t i = 0; i < line_->n_vertices(); ++i){
        if(i > 0 && fabs(line_->point(i-1)[2] - line_->point(i)[2]) > 1e-5){
            // Layer change
            int layer_id = int(round((line_->point(i+1)[2]-layer_zero_) / layer_height_));
            _stream << "; Layer: " << layer_id << std::endl;
        }

        write(i, _stream);
    }

    for(size_t i = 0; i < footer_.size(); ++i){
        footer_[i]->write(_stream);
    }*/
}

