#include "GCode.hh"

#include "EdgeTypes.hh"

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

bool GCode::parse_from_file(const std::string& _filename, double _nozzle_size){
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
    nozzle_size_ = _nozzle_size;

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

