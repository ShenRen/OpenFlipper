#pragma once

#include "GCode_Generator.hh"

class GCode_Ultimaker2 : public GCode_Generator{
public:
    virtual ~GCode_Ultimaker2(){}

protected:
    virtual void add_header(GCode::Shared _gcode, const Ultimaker::PrinterSettings& _printer, const Options& _options);
    virtual void add_line(GCode::Shared _gcode, const Ultimaker::PrinterSettings& _printer, const Options& _options, int _line_type, const ACG::Vec3d& _dst, int _axis, int _speed, double _extrude);
    virtual void add_retract(GCode::Shared _gcode, const Ultimaker::PrinterSettings& _printer, const Options& _options, double _extrude);
    virtual void add_unretract(GCode::Shared _gcode, const Ultimaker::PrinterSettings& _printer, const Options& _options, double _extrude);
    virtual void add_wait(GCode::Shared _gcode, const Ultimaker::PrinterSettings& _printer, const Options& _options, double _seconds);
    virtual void add_footer(GCode::Shared _gcode, const Ultimaker::PrinterSettings& _printer, const Options& _options);
    virtual void add_build_percentage(GCode::Shared _gcode, const Ultimaker::PrinterSettings& _printer, const Options& _options, int _percent);
    virtual void add_fan(GCode::Shared _gcode, const Ultimaker::PrinterSettings& _printer, const Options& _options, int _value);
    virtual void add_change_tool(GCode::Shared _gcode, const Ultimaker::PrinterSettings& _printer, const Options& _options, ACG::Vec3d _position, int _tool_id);
};
