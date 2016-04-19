#pragma once

#include "GCode.hh"

class GCode_Generator : public GCode::Generator{
public:
    virtual ~GCode_Generator(){}

    virtual TimeStats generate_gcode(GCode::Shared _gcode, const ptr::shared_ptr<PolyLine>& _line, double _layer_height, const Ultimaker::PrinterSettings& _printer, const Options& _options = Options());

protected:
    virtual void add_header(GCode::Shared _gcode, const Ultimaker::PrinterSettings& _printer, const Options& _options) = 0;
    virtual void add_line(GCode::Shared _gcode, const Ultimaker::PrinterSettings& _printer, const Options& _options, int _line_type, const ACG::Vec3d& _dst, int _axis, int _speed, double _extrude) = 0;
    virtual void add_retract(GCode::Shared _gcode, const Ultimaker::PrinterSettings& _printer, const Options& _options, double _extrude) = 0;
    virtual void add_unretract(GCode::Shared _gcode, const Ultimaker::PrinterSettings& _printer, const Options& _options, double _extrude) = 0;
    virtual void add_wait(GCode::Shared _gcode, const Ultimaker::PrinterSettings& _printer, const Options& _options, double _seconds) = 0;
    virtual void add_footer(GCode::Shared _gcode, const Ultimaker::PrinterSettings& _printer, const Options& _options) = 0;
    virtual void add_build_percentage(GCode::Shared _gcode, const Ultimaker::PrinterSettings& _printer, const Options& _options, int _percent) = 0;
    virtual void add_fan(GCode::Shared _gcode, const Ultimaker::PrinterSettings& _printer, const Options& _options, int _value) = 0;
    virtual void add_change_tool(GCode::Shared _gcode, const Ultimaker::PrinterSettings& _printer, const Options& _options, ACG::Vec3d _position, int _tool_id) = 0;

protected:
    enum SpeedMode {Speed_SuperSlow, Speed_Slow, Speed_Normal};

protected:
    int get_speed(const Ultimaker::PrinterSettings& _printer, int _layer_id, int _line_type, SpeedMode _mode);
    double get_part_time(const Ultimaker::PrinterSettings& _printer, int _layer_id, const ptr::shared_ptr<PolyLine>& _line, int _start, SpeedMode _mode);
    int get_next_part_type(const ptr::shared_ptr<PolyLine>& _line, int _start);


};

