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

#ifndef GCODEMAKERBOT_HH
#define GCODEMAKERBOT_HH

#include "GCode_Generator.hh"

class GCode_Makerbot : public GCode_Generator{
public:
    virtual ~GCode_Makerbot(){}

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


#endif //GCODEMAKERBOT_HH