#pragma once

#include <ObjectTypes/PolyLine/PolyLine.hh>
#include <ACG/Utils/SmartPointer.hh>

#include "PrinterSettings.hh"

#include <ostream>
#include <fstream>
#include <iostream>
#include <iomanip>

class DLLEXPORT GCode : public ptr::enable_shared_from_this<GCode>{
public:
    typedef ptr::shared_ptr<GCode> Shared;


    enum PositioningMode {PositioningMode_Absolute, PositioningMode_Relative};
    enum Axis {AXIS_X = 1, AXIS_Y = 2, AXIS_Z = 4, AXIS_E = 8, AXIS_F = 16, AXIS_A = 32, AXIS_B = 64};
    enum MoveMode {Move_Rapid = 0, Move_Normal = 1};
    enum RetractMode {RetractMode_Retract = 0, RetractMode_Unretract = 1};
    enum UnitType {Unit_Inches, Unit_Millimeters};

    struct State{
        State(){
            unit = Unit_Millimeters;
            mode_positioning = PositioningMode_Absolute;
            mode_extruder = PositioningMode_Absolute;
            axis = 0;
            temp_extruder = 0.0;
            temp_bed = 0.0;
            max_extruder = 0.0;
            pos = ACG::Vec3d(0.0, 0.0, 0.0);
            pos_extruder = 0.0;
            speed = 9000.0;
            time = 0.0;
            fan_speed = 0;
        }

        UnitType unit;
        PositioningMode mode_positioning;
        PositioningMode mode_extruder;

        int axis;

        double temp_extruder;
        double temp_bed;
        ACG::Vec3d pos;
        double pos_extruder;
        double speed; // mm / s
        double time;

        double max_extruder;

        std::string txt_display;

        int fan_speed;

        // Set position without move
        void set_position(int _axis, const ACG::Vec3d& _pos, double _extruder){
            if(_axis & AXIS_X){
                pos[0] = _pos[0];
            }
            if(_axis & AXIS_Y){
                pos[1] = _pos[1];
            }
            if(_axis & AXIS_Z){
                pos[2] = _pos[2];
            }
            if(_axis & AXIS_E){
                pos_extruder = _extruder;
            }
        }

        void move_head(int _axis, int _speed, const ACG::Vec3d& _pos, double _extruder){
            move_head(mode_positioning, _axis, _speed, _pos, _extruder);
        }

        void move_head(PositioningMode _pmode, int _axis, int _speed, const ACG::Vec3d& _pos, double _extruder){

            axis = _axis;

            if(_axis & AXIS_F){
                speed = double(_speed);
         //       std::cout << "Setting speed to " << speed << std::endl;
            }

            ACG::Vec3d position_from = pos;

            if(!std::isnan(_pos[0]) && !std::isnan(_pos[1]) && !std::isnan(_pos[2])){
                if(_pmode == PositioningMode_Absolute){
                    if(_axis & AXIS_X){
                        pos[0] = _pos[0];
                    }
                    if(_axis & AXIS_Y){
                        pos[1] = _pos[1];
                    }
                    if(_axis & AXIS_Z){
                        pos[2] = _pos[2];
                    }
                }else{
                    if(_axis & AXIS_X){
                        pos[0] += _pos[0];
                    }
                    if(_axis & AXIS_Y){
                        pos[1] += _pos[1];
                    }
                    if(_axis & AXIS_Z){
                        pos[2] += _pos[2];
                    }
                }
            }

            double distance = (position_from - pos).norm();

            if(speed > 1e-5){
                time += 60.0 * (distance / speed);
             //   std::cout << "Time: " << time << std::endl;

                if(std::isnan(time)){
                    std::cout << "Nan time moving to " << pos << std::endl;
                }
            }else{
                std::cerr << "Zero speed moving to " << _pos << std::endl;
            }


            if(mode_extruder == PositioningMode_Absolute){
                if(_axis & AXIS_E){
                    pos_extruder = _extruder;
                }
            }else{
                if(_axis & AXIS_E){
                    pos_extruder += _extruder;
                }
            }

        }
    };

private:
    struct Command{
        typedef ptr::shared_ptr<Command> Shared;

        Command() {}
        virtual ~Command(){}

        virtual void read(const std::string& _line) {
            std::cout << "Internal gcode parse error" << std::endl;
        }

        virtual void write(std::ostream& _stream) {
            _stream << "<<<<<< GCode writer: internal error <<<<<<<<" << std::endl;
            std::cerr << "Internal gcode write error" << std::endl;
        }

        void set_precision(std::ostream& _stream, int _i){
            _stream << std::fixed << std::setfill('0') << std::setprecision(_i);
        }


        virtual void update_state(State& _state) {
        }

    protected:
        std::pair<char, double> read_parameter(const std::string& _item){
            std::pair<char, double> result;
            result.first = ' ';

            if(_item.length() > 0){
                result.first = _item[0];
                result.second = QString::fromStdString(_item.substr(1, _item.length()-1)).toDouble();
            }

            return result;
        }

    protected:
        int int_p1;
        int int_p2;
        int int_p3;
        int int_p4;
        int int_p5;

        double double_p1;
        double double_p2;
        double double_p3;
        double double_p4;

        ACG::Vec3d vec3D_p1;

        std::string str_p1;
    };



public:
    struct Item{
        Item()
        { }

        Item(const Command::Shared& _command) :
            command(_command)
        { }

        Item(Command* _command) :
            command(Command::Shared(_command))
        { }

      /*  template <typename T>
        Item(const T& _command) :
            command(Command::Shared(new T(_command)))
        { }*/

        Command* operator* (){
            return command.get();
        }

        Command* operator-> (){
            return command.get();
        }

        operator bool() const{
            return command.get();
        }

        template <typename T>
        T* cast(){
            return ptr::dynamic_pointer_cast<T>(command).get();
        }

        template <typename T>
        bool is(){
            return bool(ptr::dynamic_pointer_cast<T>(command).get());
        }

    private:
        Command::Shared command;
    };

    struct CommandFactory{
        typedef ptr::shared_ptr<CommandFactory> Shared;

        virtual Item create() = 0;
    };

    template <typename T>
    struct CommandFactoryT : public CommandFactory{
        virtual Item create(){
            return Item(new T());
        }
    };

    typedef std::map<std::string, CommandFactory::Shared> TypeMap;


    struct Generator{
        struct Options{
            Options() :
                dynamic_speed(true),
                wait_for_layer_time(false),
                min_part_time(3.0),
                temp_head(210.0),
                temp_head2(210.0),
                temp_bed(70.0),
                dual_support(false),
                raft(false)
            { }

            bool dynamic_speed;
            bool wait_for_layer_time;
            double min_part_time;
            double temp_head;
            double temp_head2;
            double temp_bed;
            bool dual_support;
            bool raft;
        };

        struct TimeStats{
            TimeStats() :
                total_time(0.0),
                move(0.0),
                outer_wall(0.0),
                inner_wall(0.0),
                infill(0.0),
                topbottom(0.0)
            { }

            double total_time;

            double move;
            double outer_wall;
            double inner_wall;
            double infill;
            double topbottom;
        };

        typedef ptr::shared_ptr<Generator> Shared;

        virtual ~Generator(){}

        virtual TimeStats generate_gcode(GCode::Shared _gcode, const ptr::shared_ptr<PolyLine>& _line, double _layer_height, const Ultimaker::PrinterSettings& _printer, const Options& _options = Options()) = 0;

        /*void add_header(GCode::Shared _gcode, Command* _command){
            _gcode->header().push_back(Command::Shared(_command));
        }

        void add_footer(GCode::Shared _gcode, Command* _command){
            _gcode->footer().push_back(Command::Shared(_command));
        }

        void add_before(GCode::Shared _gcode, int _i, Command* _command){
            _gcode->data(_i).before.push_back(Command::Shared(_command));
        }

        void add_after(GCode::Shared _gcode, int _i, Command* _command){
            _gcode->data(_i).after.push_back(Command::Shared(_command));
        }*/
    };

    struct cPlaySong : public Command{
        cPlaySong(int _value = 0)
        {
            id() = _value;
        }
        virtual ~cPlaySong(){}

        virtual void read(const std::string& _line){
            QStringList list = QString::fromStdString(_line).split(" ", QString::SkipEmptyParts);
            if(list.size() >= 1){
                if(list[0] == "M72"){
                    for(int i = 1; i < list.size(); ++i){
                        std::pair<char, double> x = read_parameter(list[i].toStdString());
                        if(x.first == 'P'){
                            id() = x.second;
                        }
                    }
                }
            }
        }

        virtual void write(std::ostream& _stream){
            _stream << "M72 P" << id() << std::endl;
        }

        virtual void update_state(State& _state) {

        }

        inline int& id() {return int_p1;}
    };
    inline Item M72(int _id = 0) {return Item(new cPlaySong(_id));}

    struct cSetBuildPercentage : public Command{
        cSetBuildPercentage(int _value = 0)
        {
            value() = _value;
        }
        virtual ~cSetBuildPercentage(){}

        virtual void read(const std::string& _line){
            QStringList list = QString::fromStdString(_line).split(" ", QString::SkipEmptyParts);
            if(list.size() >= 1){
                if(list[0] == "M73"){
                    value() = 0;
                    for(int i = 1; i < list.size(); ++i){
                        std::pair<char, double> x = read_parameter(list[i].toStdString());
                        if(x.first == 'P'){
                            value() = x.second;
                        }
                    }
                }
            }
        }

        virtual void write(std::ostream& _stream){
            _stream << "M73 P" << value() << std::endl;
        }

        virtual void update_state(State& _state) {

        }

        inline int& value() {return int_p1;}
    };
    inline Item M73(int _value = 0) {return Item(new cSetBuildPercentage(_value));}

    struct cSetFanSpeed : public Command{
        cSetFanSpeed(int _speed = 0, int _fan_id = 0)
        {
            speed() = _speed;
            fan_id() = _fan_id;
        }
        virtual ~cSetFanSpeed(){}

        virtual void read(const std::string& _line){
            QStringList list = QString::fromStdString(_line).split(" ", QString::SkipEmptyParts);
            if(list.size() >= 1){
                if(list[0] == "M107"){
                    speed() = 0;
                    fan_id() = 0;
                }else if(list[0] == "M106"){
                    speed() = 0;
                    fan_id() = 0;
                    for(int i = 1; i < list.size(); ++i){
                        std::pair<char, double> x = read_parameter(list[i].toStdString());
                        if(x.first == 'S'){
                            speed() = x.second;
                        }else if(x.first == 'P'){
                            fan_id() = x.second;
                        }
                    }
                }else if(list[0] == "M127"){
                    speed() = 0;
                    fan_id() = -1;
                }else if(list[0] == "M126"){
                    speed() = 255;
                    fan_id() = -1;
                }
            }
        }

        virtual void write(std::ostream& _stream){
            if(fan_id() == -1){
                if(speed() < 255){
                    _stream << "M127";
                }else{
                    _stream << "M126";
                }
                _stream << std::endl;
            }else{
                if(speed() == 0){
                    _stream << "M107";
                }else{
                    _stream << "M106 S" << speed();
                }

                if(fan_id() != 0){
                    _stream << " P" << fan_id();
                }
                _stream << std::endl;
            }
        }

        virtual void update_state(State& _state) {
            _state.fan_speed = speed();
        }

        inline int& speed() {return int_p1;}
        inline int& fan_id() {return int_p2;}
    };
    inline Item M106(int _fan_speed, int _fan_id = 0) {return Item(new cSetFanSpeed(_fan_speed, _fan_id));}
    inline Item M107(int _fan_id = 0) {return Item(new cSetFanSpeed(0, _fan_id));}
    inline Item M126() {return Item(new cSetFanSpeed(255, -1));}
    inline Item M127() {return Item(new cSetFanSpeed(0, -1));}

    // Set if units are millimeters or inches
    struct cSetUnitMode : public Command{
        cSetUnitMode(UnitType _unit = Unit_Millimeters)
        {
            unit() = _unit;
        }
         //   : unit(_unit){}
        virtual ~cSetUnitMode(){}

        virtual void read(const std::string& _line){
            QStringList list = QString::fromStdString(_line).split(" ", QString::SkipEmptyParts);
            if(list.size() >= 1){
                if(list[0] == "G20"){
                    unit() = Unit_Inches;
                }else if(list[0] == "G21"){
                    unit() = Unit_Millimeters;
                }
            }
        }

        virtual void write(std::ostream& _stream){
            switch(unit()){
            case Unit_Inches: _stream << "G20" << std::endl; break;
            case Unit_Millimeters: _stream << "G21" << std::endl; break;
            }
        }

        virtual void update_state(State& _state) {
            _state.unit = UnitType(unit());
        }

        inline int& unit() {return int_p1;}
    };
    inline Item G20() {return Item(new cSetUnitMode(Unit_Inches));}
    inline Item G21() {return Item(new cSetUnitMode(Unit_Millimeters));}

    struct cSetCoordinateSystem : public Command{
        cSetCoordinateSystem(int _tool = 0)
        {
            tool() = _tool;
        }
         //   : unit(cSetCoordinateSystem){}
        virtual ~cSetCoordinateSystem(){}

        virtual void read(const std::string& _line){
            QStringList list = QString::fromStdString(_line).split(" ", QString::SkipEmptyParts);
            if(list.size() >= 1){
                if(list[0] == "G54"){
                    tool() = 0;
                }else if(list[0] == "G55"){
                    tool() = 1;
                }
            }
        }

        virtual void write(std::ostream& _stream){
            if(tool() == 0){
                _stream << "G54" << std::endl;
            }else if(tool() == 1){
                _stream << "G55" << std::endl;
            }
        }

        virtual void update_state(State& _state) {

        }

        inline int& tool() {return int_p1;}
    };
    inline Item G54() {return Item(new cSetCoordinateSystem(0));}
    inline Item G55() {return Item(new cSetCoordinateSystem(1));}

    // Set positioning mode to relative / absolute
    struct cSetPositioningMode : public Command{
        cSetPositioningMode(PositioningMode _type = PositioningMode_Absolute)
        {
            type() = _type;
        }
        virtual ~cSetPositioningMode(){}

        virtual void read(const std::string& _line){
            QStringList list = QString::fromStdString(_line).split(" ", QString::SkipEmptyParts);
            if(list.size() >= 1){
                if(list[0] == "G90"){
                    type() = PositioningMode_Absolute;
                }else if(list[0] == "G91"){
                    type() = PositioningMode_Relative;
                }
            }
        }

        virtual void update_state(State& _state) {
            _state.mode_positioning = PositioningMode(type());
        }

        virtual void write(std::ostream& _stream){
            switch(type()){
            case PositioningMode_Absolute: _stream << "G90" << std::endl; break;
            case PositioningMode_Relative: _stream << "G91" << std::endl; break;
            }
        }

        inline int& type() {return int_p1;}
    };
    inline Item G90() {return Item(new cSetPositioningMode(PositioningMode_Absolute));}
    inline Item G91() {return Item(new cSetPositioningMode(PositioningMode_Relative));}

    // Set extruder mode to relative / absolute
    struct cExtruderMode : public Command{
        cExtruderMode(PositioningMode _type = PositioningMode_Absolute)
        {
            type() = _type;
        }
        virtual ~cExtruderMode(){}

        virtual void read(const std::string& _line){
            QStringList list = QString::fromStdString(_line).split(" ", QString::SkipEmptyParts);
            if(list.size() >= 1){
                if(list[0] == "M82"){
                    type() = PositioningMode_Absolute;
                }else if(list[0] == "M83"){
                    type() = PositioningMode_Relative;
                }
            }
        }

        virtual void write(std::ostream& _stream){
            switch(type()){
            case PositioningMode_Absolute: _stream << "M82" << std::endl; break;
            case PositioningMode_Relative: _stream << "M83" << std::endl; break;
            }
        }

        virtual void update_state(State& _state) {
            _state.mode_extruder = PositioningMode(type());
        }

        inline int& type() {return int_p1;}
    };
    inline Item M82() {return Item(new cExtruderMode(PositioningMode_Absolute));}
    inline Item M83() {return Item(new cExtruderMode(PositioningMode_Relative));}

    // Retract / unretract
    struct cRetract : public Command{
        cRetract(RetractMode _type = RetractMode_Retract)
        {
            type() = _type;
        }
        virtual ~cRetract(){}

        virtual void read(const std::string& _line){
            QStringList list = QString::fromStdString(_line).split(" ", QString::SkipEmptyParts);
            if(list.size() >= 1){
                if(list[0] == "G10"){
                    type() = RetractMode_Retract;
                }else if(list[0] == "G11"){
                    type() = RetractMode_Unretract;
                }
            }
        }

        virtual void write(std::ostream& _stream){
            switch(type()){
            case RetractMode_Retract: _stream << "G10" << std::endl; break;
            case RetractMode_Unretract: _stream << "G11" << std::endl; break;
            }
        }

        virtual void update_state(State& _state) {
        }

        inline int& type() {return int_p1;}
    };
    inline Item G10() {return Item(new cRetract(RetractMode_Retract));}
    inline Item G11() {return Item(new cRetract(RetractMode_Unretract));}

    // Do nothing for a certain amount of time
    struct cSleep : public Command{
        cSleep(int _milliseconds = 0)
        {
            time() = _milliseconds;
        }
        virtual ~cSleep(){}

        virtual void read(const std::string& _line){
            QStringList list = QString::fromStdString(_line).split(" ", QString::SkipEmptyParts);
            if(list[0] == "G4"){
                for(int i = 1; i < list.size(); ++i){
                    std::pair<char, double> x = read_parameter(list[i].toStdString());
                    if(x.first == 'P'){
                        time() = int(x.second);
                    }else if(x.first == 'S'){
                        time() = int(x.second)*1000;
                    }
                }
            }
        }

        virtual void write(std::ostream& _stream){
            _stream << "G4 P" << time() << std::endl;
        }

        virtual void update_state(State& _state) {
            _state.time += double(time())/1000.0;
        }

        inline int& time() {return int_p1;}
    };
    inline Item G4(int _milliseconds) {return Item(new cSleep(_milliseconds));}

    struct cMoveToOrigin : public Command{
        cMoveToOrigin(int _axis = 0)
        {
            axis() = _axis;
        }
        virtual ~cMoveToOrigin(){}

        virtual void read(const std::string& _line){
            QStringList list = QString::fromStdString(_line).split(" ", QString::SkipEmptyParts);
            if(list.size() >= 1){
                if(list[0] == "G28"){
                    int a = 0;

                    for(int i = 1; i < list.size(); ++i){
                        std::pair<char, double> x = read_parameter(list[i].toStdString());
                        if(x.first == 'X'){
                            a |= AXIS_X;
                        }else if(x.first == 'Y'){
                            a |= AXIS_Y;
                        }else if(x.first == 'Z'){
                            a |= AXIS_Z;
                        }
                    }

                    axis() = a;
                }
            }
        }

        virtual void write(std::ostream& _stream){
            if(!(axis() & (AXIS_X | AXIS_Y | AXIS_Z))){
                return;
            }

            _stream << "G28";
            if(axis() & AXIS_X){
                _stream << " X0";
            }
            if(axis() & AXIS_Y){
                _stream << " Y0";
            }
            if(axis() & AXIS_Z){
                _stream << " Z0";
            }
            _stream << std::endl;
        }

        virtual void update_state(State& _state) {
            _state.move_head(PositioningMode_Absolute, axis(), 9000, ACG::Vec3d(0.0, 0.0, 0.0), 0.0);
        }

        inline int& axis() {return int_p1;}
    };
    inline Item G28(int _axis) {return Item(new cMoveToOrigin(_axis));}

    struct cHomeAxisMin : public Command{
        cHomeAxisMin(int _axis = 0, int _speed = 2500)
        {
            axis() = _axis;
            speed() = _speed;
        }
        virtual ~cHomeAxisMin(){}

        virtual void read(const std::string& _line){
            QStringList list = QString::fromStdString(_line).split(" ", QString::SkipEmptyParts);
            if(list.size() >= 1){
                if(list[0] == "G161"){
                    int a = 0;

                    speed() = 2500;

                    for(int i = 1; i < list.size(); ++i){
                        std::pair<char, double> x = read_parameter(list[i].toStdString());
                        if(x.first == 'X'){
                            a |= AXIS_X;
                        }else if(x.first == 'Y'){
                            a |= AXIS_Y;
                        }else if(x.first == 'Z'){
                            a |= AXIS_Z;
                        }else if(x.first == 'F'){
                            a |= AXIS_F;
                            speed() = x.second;
                        }
                    }

                    axis() = a;
                }
            }
        }

        virtual void write(std::ostream& _stream){
            if(!(axis() & (AXIS_X | AXIS_Y | AXIS_Z))){
                return;
            }

            _stream << "G161";
            if(axis() & AXIS_X){
                _stream << " X";
            }
            if(axis() & AXIS_Y){
                _stream << " Y";
            }
            if(axis() & AXIS_Z){
                _stream << " Z";
            }
            _stream << " F" << speed();

            _stream << std::endl;
        }

        virtual void update_state(State& _state) {
            _state.move_head(PositioningMode_Absolute, axis(), 9000, ACG::Vec3d(0.0, 0.0, 0.0), 0.0);
        }

        inline int& axis() {return int_p1;}
        inline int& speed() {return int_p2;}
    };
    inline Item G161(int _axis, int _speed = 2500) {return Item(new cHomeAxisMin(_axis, _speed));}

    struct cStoreHomeOffsets : public Command{
        cStoreHomeOffsets()
        {
        }
        virtual ~cStoreHomeOffsets(){}

        virtual void read(const std::string& _line){
        }

        virtual void write(std::ostream& _stream){
            _stream << "M132 X Y Z A B" << std::endl;
        }

        virtual void update_state(State& _state) {
        }
    };
    inline Item M132() {return Item(new cStoreHomeOffsets());}

    struct cSetPotentiometerValue : public Command{
        cSetPotentiometerValue(int _x = 20, int _y = 20, int _z = 20, int _a = 20, int _b = 20)
        {
            x() = _x;
            y() = _y;
            z() = _z;
            a() = _a;
            b() = _b;
        }
        virtual ~cSetPotentiometerValue(){}

        virtual void read(const std::string& _line){
            QStringList list = QString::fromStdString(_line).split(" ", QString::SkipEmptyParts);
            if(list.size() >= 1){
                if(list[0] == "G130"){
                    x() = 20;
                    y() = 20;
                    z() = 20;
                    a() = 20;
                    b() = 20;

                    for(int i = 1; i < list.size(); ++i){
                        std::pair<char, double> p = read_parameter(list[i].toStdString());
                        if(p.first == 'X'){
                            x() = p.second;
                        }else if(p.first == 'Y'){
                            y() = p.second;
                        }else if(p.first == 'Z'){
                            z() = p.second;
                        }else if(p.first == 'A'){
                            a() = p.second;
                        }else if(p.first == 'B'){
                            b() = p.second;
                        }
                    }
                }
            }
        }

        virtual void write(std::ostream& _stream){
            _stream << "G130 X" << x() << " Y" << y() << " Z" << z() << " A" << a() << " B" << b() << std::endl;
        }

        virtual void update_state(State& _state) {
        }

        inline int& x() {return int_p1;}
        inline int& y() {return int_p2;}
        inline int& z() {return int_p3;}
        inline int& a() {return int_p4;}
        inline int& b() {return int_p5;}
    };
    inline Item G130(int _x, int _y, int _z, int _a, int _b) {return Item(new cSetPotentiometerValue(_x, _y, _z, _a, _b));}

    struct cHomeAxisMax : public Command{
        cHomeAxisMax(int _axis = 0, int _speed = 2500)
        {
            axis() = _axis;
            speed() = _speed;
        }
        virtual ~cHomeAxisMax(){}

        virtual void read(const std::string& _line){
            QStringList list = QString::fromStdString(_line).split(" ", QString::SkipEmptyParts);
            if(list.size() >= 1){
                if(list[0] == "G162"){
                    int a = 0;

                    for(int i = 1; i < list.size(); ++i){
                        std::pair<char, double> x = read_parameter(list[i].toStdString());
                        if(x.first == 'X'){
                            a |= AXIS_X;
                        }else if(x.first == 'Y'){
                            a |= AXIS_Y;
                        }else if(x.first == 'Z'){
                            a |= AXIS_Z;
                        }else if(x.first == 'F'){
                            a |= AXIS_F;
                            speed() = x.second;
                        }
                    }

                    axis() = a;
                }
            }
        }

        virtual void write(std::ostream& _stream){
            if(!(axis() & (AXIS_X | AXIS_Y | AXIS_Z))){
                return;
            }

            _stream << "G162";
            if(axis() & AXIS_X){
                _stream << " X";
            }
            if(axis() & AXIS_Y){
                _stream << " Y";
            }
            if(axis() & AXIS_Z){
                _stream << " Z";
            }
            _stream << " F" << speed();

            _stream << std::endl;
        }

        virtual void update_state(State& _state) {
            _state.move_head(PositioningMode_Absolute, axis(), 9000, ACG::Vec3d(0.0, 0.0, 0.0), 0.0);
        }

        inline int& axis() {return int_p1;}
        inline int& speed() {return int_p2;}
    };
    inline Item G162(int _axis, int _speed = 2500) {return Item(new cHomeAxisMax(_axis, _speed));}


    struct cMove : public Command{
        cMove(MoveMode _mode = Move_Normal, int _speed = 1200, int _axis = AXIS_F, const ACG::Vec3d& _position = ACG::Vec3d(0.0, 0.0, 0.0), double _extrude = 0.0)
        {
            mode() = _mode; speed() = _speed; axis() = _axis; position() = _position; extrude() = _extrude;
        }
        virtual ~cMove(){}

        virtual void read(const std::string& _line){
            QStringList list = QString::fromStdString(_line).split(" ", QString::SkipEmptyParts);
            if(list.size() >= 1){
                if(list[0] == "G0" || list[0] == "G1"){
                    if(list[0] == "G0"){
                        mode() = Move_Rapid;
                    }else{
                        mode() = Move_Normal;
                    }

                    int a = 0;
                    ACG::Vec3d p(0.0, 0.0, 0.0);
                    double e = 0.0, s = 1200.0;

                    for(int i = 1; i < list.size(); ++i){
                        std::pair<char, double> x = read_parameter(list[i].toStdString());
                        if(x.first == 'X'){
                            a |= AXIS_X;
                            p[0] = x.second;
                        }else if(x.first == 'Y'){
                            a |= AXIS_Y;
                            p[1] = x.second;
                        }else if(x.first == 'Z'){
                            a |= AXIS_Z;
                            p[2] = x.second;
                        }else if(x.first == 'E'){
                            a |= AXIS_E;
                            e = x.second;
                        }else if(x.first == 'F'){
                            a |= AXIS_F;
                            s = x.second;
                        }
                    }

                    axis() = a;
                    position() = p;
                    extrude() = e;
                    speed() = s;
                }
            }
        }

        virtual void write(std::ostream& _stream){
          /*  if(!(axis() & (AXIS_X | AXIS_Y | AXIS_Z | AXIS_E))){
                return;
            }*/

            if(mode() == Move_Rapid){
                _stream << "G0";
            }else{
                _stream << "G1";
            }

            if(axis() & AXIS_F){
                _stream << " F" << speed();
            }
            if(axis() & AXIS_X){
                _stream << " X" << position()[0];
            }
            if(axis() & AXIS_Y){
                _stream << " Y" << position()[1];
            }
            if(axis() & AXIS_Z){
                _stream << " Z" << position()[2];
            }

            if(axis() & AXIS_E){
                set_precision(_stream, 5);
                _stream << " E" << extrude();
                set_precision(_stream, 3);
            }

            _stream << std::endl;
        }

        virtual void update_state(State& _state) {
            _state.move_head(axis(), speed(), position(), extrude());
        }

        inline int& axis() {return int_p1;}
        inline int& speed() {return int_p2;}
        inline int& mode() {return int_p3;}

        inline ACG::Vec3d& position() {return vec3D_p1;}
        inline double& extrude() {return double_p1;}
    };
    inline Item G0(int _speed, int _axis, const ACG::Vec3d& _position = ACG::Vec3d(), double _extrude = 0.0) {return Item(new cMove(Move_Rapid, _speed, _axis, _position, _extrude));}
    inline Item G1(int _speed, int _axis, const ACG::Vec3d& _position = ACG::Vec3d(), double _extrude = 0.0) {return Item(new cMove(Move_Normal, _speed, _axis, _position, _extrude));}

    struct cSetPosition : public Command{
        enum Axis {AXIS_X = 1, AXIS_Y = 2, AXIS_Z = 4, AXIS_E = 8};

        cSetPosition(int _axis = 0, const ACG::Vec3d& _position = ACG::Vec3d(0.0, 0.0, 0.0), double _extrude = 0.0)
        {
            axis() = _axis; position() = _position; extrude() = _extrude;
        }
        virtual ~cSetPosition(){}

        virtual void read(const std::string& _line){
            QStringList list = QString::fromStdString(_line).split(" ", QString::SkipEmptyParts);
            if(list.size() >= 1){
                if(list[0] == "G92"){
                    int a = 0;
                    ACG::Vec3d p(0.0, 0.0, 0.0);
                    double e = 0.0;

                    for(int i = 1; i < list.size(); ++i){
                        std::pair<char, double> x = read_parameter(list[i].toStdString());
                        if(x.first == 'X'){
                            a |= AXIS_X;
                            p[0] = x.second;
                        }else if(x.first == 'Y'){
                            a |= AXIS_Y;
                            p[1] = x.second;
                        }else if(x.first == 'Z'){
                            a |= AXIS_Z;
                            p[2] = x.second;
                        }else if(x.first == 'E'){
                            a |= AXIS_E;
                            e = x.second;
                        }
                    }

                    axis() = a;
                    position() = p;
                    extrude() = e;
                }
            }
        }

        virtual void write(std::ostream& _stream){
            if(!(axis() & (AXIS_X | AXIS_Y | AXIS_Z | AXIS_E))){
                return;
            }

            _stream << "G92";
            if(axis() & AXIS_X){
                _stream << " X" << position()[0];
            }
            if(axis() & AXIS_Y){
                _stream << " Y" << position()[1];
            }
            if(axis() & AXIS_Z){
                _stream << " Z" << position()[2];
            }
            if(axis() & AXIS_E){
                _stream << " E" << extrude();
            }
            _stream << std::endl;
        }

        virtual void update_state(State& _state) {
            _state.set_position(axis(), position(), extrude());

            if(axis() & AXIS_E){
                _state.max_extruder = extrude();
            }
        }

        inline int& axis() {return int_p1;}
        inline ACG::Vec3d& position() {return vec3D_p1;}
        inline double& extrude() {return double_p1;}
    };
    inline Item G92(int _axis, const ACG::Vec3d& _position = ACG::Vec3d(), double _extrude = 0.0) {return Item(new cSetPosition(_axis, _position, _extrude));}

    struct cDisplayMessage : public Command{
        cDisplayMessage(const std::string& _message = "")
        {
            message() = _message;
        }
        virtual ~cDisplayMessage(){}

        virtual void read(const std::string& _line){
            QStringList list = QString::fromStdString(_line).split(" ", QString::SkipEmptyParts);
            if(list.size() >= 1){
                if(list[0] == "M117"){
                    std::stringstream txt;
                    for(int i = 1; i < list.size(); ++i){
                        txt << list[i].toStdString() << " ";
                    }
                    message() = txt.str();
                }
            }
        }

        virtual void write(std::ostream& _stream){
            if(message() != ""){
                _stream << "M117 " << message() << std::endl;
            }
        }

        virtual void update_state(State& _state) {
            _state.txt_display = message();
        }

        inline std::string& message(){return str_p1;}
    };
    inline Item M117(const std::string& _text) {return Item(new cDisplayMessage(_text));}

    struct cComment : public Command{
        cComment(const std::string& _message = "")
        {
            message() = _message;
        }
        virtual ~cComment(){}

        virtual void read(const std::string& _line){
            message() = _line.substr(1, _line.length()-1);
        }

        virtual void write(std::ostream& _stream){
            _stream << ";" << message() << std::endl;
        }

        inline std::string& message(){return str_p1;}
    };
    inline Item COMMENT(const std::string& _text) {return Item(new cComment(_text));}



    struct cHeatBed : public Command{
        cHeatBed(const double _degree = 0.0)
        {
            degree() = _degree;
        }
        virtual ~cHeatBed(){}

        virtual void read(const std::string& _line){
            QStringList list = QString::fromStdString(_line).split(" ", QString::SkipEmptyParts);
            if(list.size() >= 1){
                if(list[0] == "M190"){
                    double d = 0.0;
                    for(int i = 1; i < list.size(); ++i){
                        std::pair<char, double> x = read_parameter(list[i].toStdString());
                        if(x.first == 'S'){
                            d = x.second;
                        }
                    }
                    degree() = d;
                }
            }
        }

        virtual void write(std::ostream& _stream){
            _stream << "M190 S" << degree() << std::endl;
        }

        virtual void update_state(State& _state) {
            _state.temp_bed = degree();
        }

        inline double& degree() {return double_p1;}
    };
    inline Item M190(double _degree) {return Item(new cHeatBed(_degree));}

    struct cHeatExtruder : public Command{
        cHeatExtruder(const double _degree = 0.0, int _tool = -1)
        {
            degree() = _degree;
            tool() = _tool;
        }
        virtual ~cHeatExtruder(){}

        virtual void read(const std::string& _line){
            QStringList list = QString::fromStdString(_line).split(" ", QString::SkipEmptyParts);
            if(list.size() >= 1){
                if(list[0] == "M109"){
                    for(int i = 1; i < list.size(); ++i){
                        std::pair<char, double> x = read_parameter(list[i].toStdString());
                        if(x.first == 'S'){
                            degree() = x.second;
                        }
                        if(x.first == 'T'){
                            tool() = x.second;
                        }
                    }
                }
            }
        }

        virtual void write(std::ostream& _stream){
            _stream << "M109 S" << degree();

            if(tool() != -1){
                _stream << " T" << tool();
            }

            _stream << std::endl;
        }

        virtual void update_state(State& _state) {
            _state.temp_extruder = degree();
            _state.time += 1.0;
        }

        inline double& degree() {return double_p1;}
        inline int& tool() {return int_p1;}
    };
    inline Item M109(double _degree, int _tool = -1) {return Item(new cHeatExtruder(_degree, _tool));}


    struct cToolChange : public Command{
        cToolChange(const int _tool = 0)
        {
            tool() = _tool;
        }
        virtual ~cToolChange(){}

        virtual void read(const std::string& _line){
            QStringList list = QString::fromStdString(_line).split(" ", QString::SkipEmptyParts);
            if(list.size() >= 1){
                if(list[0] == "M6"){
                    for(int i = 1; i < list.size(); ++i){
                        std::pair<char, double> x = read_parameter(list[i].toStdString());
                        if(x.first == 'T'){
                            tool() = x.second;
                        }
                    }
                }
            }
        }

        virtual void write(std::ostream& _stream){
            _stream << "M6 T" << tool() << std::endl;
        }

        virtual void update_state(State& _state) {
        }

        inline int& tool() {return int_p1;}
    };
    inline Item M6(int _tool) {return Item(new cToolChange(_tool));}

    struct cSetExtruderSpeed : public Command{
        cSetExtruderSpeed(double _speed = 3.0, int _tool = -1)
        {
            speed() = _speed;
            tool() = _tool;
        }
        virtual ~cSetExtruderSpeed(){}

        virtual void read(const std::string& _line){
            speed() = 0.0;
            tool() = -1;

            QStringList list = QString::fromStdString(_line).split(" ", QString::SkipEmptyParts);
            if(list.size() >= 1){
                if(list[0] == "M108"){
                    for(int i = 1; i < list.size(); ++i){
                        std::pair<char, double> x = read_parameter(list[i].toStdString());
                        if(x.first == 'T'){
                            tool() = x.second;
                        }else if(x.first == 'R'){
                            speed() = x.second;
                        }
                    }
                }
            }
        }

        virtual void write(std::ostream& _stream){
            _stream << "M108";

            if(speed() >= 0.0){
                _stream << " R" <<speed();
            }

            if(tool() >= 0){
                _stream << " T"<< tool();
            }
            _stream << std::endl;
        }

        virtual void update_state(State& _state) {
        }

        inline double& speed() {return double_p1;}
        inline int& tool() {return int_p1;}
    };
    inline Item M108(double _speed, int _tool) {return Item(new cSetExtruderSpeed(_speed, _tool));}
    inline Item M108(int _tool) {return Item(new cSetExtruderSpeed(0.0, _tool));}

    struct cSetBedTemperature : public Command{
        cSetBedTemperature(const double _degree = 0.0)
        {
            degree() = _degree;
        }
        virtual ~cSetBedTemperature(){}

        virtual void read(const std::string& _line){
            QStringList list = QString::fromStdString(_line).split(" ", QString::SkipEmptyParts);
            if(list.size() >= 1){
                if(list[0] == "M140"){
                    double d = 0.0;
                    for(int i = 1; i < list.size(); ++i){
                        std::pair<char, double> x = read_parameter(list[i].toStdString());
                        if(x.first == 'S'){
                            d = x.second;
                        }
                    }
                    degree() = d;
                }
            }
        }

        virtual void write(std::ostream& _stream){
            _stream << "M140 S" << degree() << std::endl;
        }

        virtual void update_state(State& _state) {
            _state.temp_bed = degree();
        }


        inline double& degree() {return double_p1;}
    };
    inline Item M140(double _degree) {return Item(new cSetBedTemperature(_degree));}

    struct cSetExtruderTemperature : public Command{
        cSetExtruderTemperature(const double _degree = 0.0, int _tool = -1)
        {
            degree() = _degree;
            tool() = _tool;
        }
        virtual ~cSetExtruderTemperature(){}

        virtual void read(const std::string& _line){
            QStringList list = QString::fromStdString(_line).split(" ", QString::SkipEmptyParts);
            if(list.size() >= 1){
                if(list[0] == "M104"){
                    for(int i = 1; i < list.size(); ++i){
                        std::pair<char, double> x = read_parameter(list[i].toStdString());
                        if(x.first == 'S'){
                            degree() = x.second;
                        }
                        if(x.first == 'T'){
                            tool() = x.second;
                        }
                    }
                }
            }
        }

        virtual void write(std::ostream& _stream){
            _stream << "M104 S" << degree();

            if(tool() != -1){
                _stream << " T" << tool();
            }

            _stream << std::endl;
        }

        virtual void update_state(State& _state) {
            _state.temp_extruder = degree();
        }

        inline int& tool() {return int_p1;}
        inline double& degree() {return double_p1;}
    };
    inline Item M104(double _degree) {return Item(new cSetExtruderTemperature(_degree));}

    struct cStartExtruder : public Command{
        cStartExtruder() {}
        virtual ~cStartExtruder(){}

        virtual void read(const std::string& _line){
        }

        virtual void write(std::ostream& _stream){
            _stream << "M101" << std::endl;
        }
    };
    inline Item M101() {return Item(new cStartExtruder());}

    struct cStopExtruder : public Command{
        cStopExtruder() {}
        virtual ~cStopExtruder(){}

        virtual void read(const std::string& _line){
        }

        virtual void write(std::ostream& _stream){
            _stream << "M103" << std::endl;
        }
    };
    inline Item M103() {return Item(new cStopExtruder());}

    struct cDisableStepper : public Command{
        cDisableStepper() {}
        virtual ~cDisableStepper(){}

        virtual void read(const std::string& _line){
        }

        virtual void write(std::ostream& _stream){
            _stream << "M18" << std::endl;
        }
    };
    inline Item M18() {return Item(new cDisableStepper());}

    struct cStopIdleHold : public Command{
        cStopIdleHold() {}
        virtual ~cStopIdleHold(){}

        virtual void read(const std::string& _line){
        }

        virtual void write(std::ostream& _stream){
            _stream << "M84" << std::endl;
        }
    };
    inline Item M84() {return Item(new cStopIdleHold());}

    static void build_type_list(std::map<std::string, CommandFactory::Shared>& _list){
        _list["M106"] = CommandFactory::Shared(new CommandFactoryT<cSetFanSpeed>);
        _list["M107"] = CommandFactory::Shared(new CommandFactoryT<cSetFanSpeed>);
        _list["M126"] = CommandFactory::Shared(new CommandFactoryT<cSetFanSpeed>);
        _list["M127"] = CommandFactory::Shared(new CommandFactoryT<cSetFanSpeed>);
        _list["G20"] = CommandFactory::Shared(new CommandFactoryT<cSetUnitMode>);
        _list["G21"] = CommandFactory::Shared(new CommandFactoryT<cSetUnitMode>);
        _list["G90"] = CommandFactory::Shared(new CommandFactoryT<cSetPositioningMode>);
        _list["G91"] = CommandFactory::Shared(new CommandFactoryT<cSetPositioningMode>);
        _list["M82"] = CommandFactory::Shared(new CommandFactoryT<cExtruderMode>);
        _list["M83"] = CommandFactory::Shared(new CommandFactoryT<cExtruderMode>);
        _list["G10"] = CommandFactory::Shared(new CommandFactoryT<cRetract>);
        _list["G11"] = CommandFactory::Shared(new CommandFactoryT<cRetract>);
        _list["G28"] = CommandFactory::Shared(new CommandFactoryT<cMoveToOrigin>);
        _list["G0"] = CommandFactory::Shared(new CommandFactoryT<cMove>);
        _list["G1"] = CommandFactory::Shared(new CommandFactoryT<cMove>);
        _list["G54"] = CommandFactory::Shared(new CommandFactoryT<cSetCoordinateSystem>);
        _list["G55"] = CommandFactory::Shared(new CommandFactoryT<cSetCoordinateSystem>);
        _list["G92"] = CommandFactory::Shared(new CommandFactoryT<cSetPosition>);
        _list["M117"] = CommandFactory::Shared(new CommandFactoryT<cDisplayMessage>);
        _list[";"] = CommandFactory::Shared(new CommandFactoryT<cComment>);
        _list["M190"] = CommandFactory::Shared(new CommandFactoryT<cHeatBed>);
        _list["M109"] = CommandFactory::Shared(new CommandFactoryT<cHeatExtruder>);
        _list["M140"] = CommandFactory::Shared(new CommandFactoryT<cSetBedTemperature>);
        _list["M104"] = CommandFactory::Shared(new CommandFactoryT<cSetExtruderTemperature>);
        _list["M84"] = CommandFactory::Shared(new CommandFactoryT<cStopIdleHold>);
        _list["G4"] = CommandFactory::Shared(new CommandFactoryT<cSleep>);
        _list["M101"] = CommandFactory::Shared(new CommandFactoryT<cStartExtruder>);
        _list["M103"] = CommandFactory::Shared(new CommandFactoryT<cStopExtruder>);
        _list["M108"] = CommandFactory::Shared(new CommandFactoryT<cSetExtruderSpeed>);
        _list["M6"] = CommandFactory::Shared(new CommandFactoryT<cToolChange>);
        _list["G130"] = CommandFactory::Shared(new CommandFactoryT<cSetPotentiometerValue>);
        _list["G161"] = CommandFactory::Shared(new CommandFactoryT<cHomeAxisMin>);
        _list["G162"] = CommandFactory::Shared(new CommandFactoryT<cHomeAxisMax>);
        _list["M132"] = CommandFactory::Shared(new CommandFactoryT<cStoreHomeOffsets>);

        _list["M18"] = CommandFactory::Shared(new CommandFactoryT<cDisableStepper>);


    }


    static TypeMap& type_map()
    {
        static TypeMap types;
        if(types.empty()){
            build_type_list(types);
        }
        return types;
    }

    static Item Create(const std::string& _command){
        TypeMap::iterator it = type_map().find(_command);

        if(it != type_map().end()){
            return it->second->create();
        }else{
            return Item();
        }
    }

    static Item ParseLine(const QString& _line){
        QStringList list = _line.split(" ", QString::SkipEmptyParts);
        QString cmd = "";
        if(list.size() > 0){
             cmd = list[0];
            if(cmd.size() > 0 && cmd.at(0).toLatin1() == ';'){
                cmd = ";";
            }
        }

        if(cmd.toStdString() != ""){
            Item item = Create(cmd.toStdString());
            if(item){
                item->read(_line.toStdString());
            }
            return item;
        }else{
            return Item();
        }
    }

    // Data for generated polylines
    struct NodeData{
        double time; // Time in minutes, when this vertex is printed
        int speed;
        double extrude;
    };

    inline double total_time(){return total_time_;}

public:
    GCode();
    virtual ~GCode();

    // Create gcode from polyline
    Generator::TimeStats create_from_polyline(Generator::Shared _gcode_generator, const ptr::shared_ptr<PolyLine>& _line, double _layer_height, const Ultimaker::PrinterSettings& _printer, const Generator::Options& _options = Generator::Options());

    bool parse_from_file(const std::string& _filename, double _nozzle_size);

    // Create polyline from gcode
    std::pair<ptr::shared_ptr<PolyLine>, std::vector<State> > create_polyline();
    void update_polyline();

    inline double layer_zero(){return layer_zero_;}
    inline double layer_height(){return layer_height_;}
    inline double nozzle_size(){return nozzle_size_;}

    virtual void write(std::ostream& _stream);
   // virtual void write(int _i, std::ostream& _stream);

    void push_back(const Item& _item) {gcode_.push_back(_item);}
    void push_back(Command* _command) {gcode_.push_back(Item(_command));}
    void insert(std::vector<Item>::iterator _pos, std::vector<Item>::iterator _from, std::vector<Item>::iterator _to) {gcode_.insert(_pos, _from, _to);}
    void insert(std::vector<Item>::iterator _pos, const Item& _item) {gcode_.insert(_pos, _item);}
    void insert(int _i, const Item& _item) {gcode_.insert(gcode_.begin() + _i, _item);}
    void clear(){gcode_.clear();}
    int size(){return int(gcode_.size());}
    Item& at(int _i){return gcode_[_i];}
    const Item& at(int _i) const {return gcode_[_i];}

    inline std::vector<Item>::iterator begin() {return gcode_.begin();}
    inline std::vector<Item>::iterator end() {return gcode_.end();}

    inline ptr::shared_ptr<PolyLine> polyline(){
        if(!polyline_){
            update_polyline();
        }

        return polyline_;
    }
    inline const std::vector<State>& polyline_states(){
        if(!polyline_){
            update_polyline();
        }

        return polyline_states_;
    }

protected:
    double layer_zero_;
    double layer_height_;
    double nozzle_size_;

    double total_time_; // Total time in minutes

    std::vector<Item> gcode_;

    //
    ptr::shared_ptr<PolyLine> polyline_;
    std::vector<State> polyline_states_;
};
