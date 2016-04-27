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


#ifndef GCODE_EDGE_TYPES_HH
#define GCODE_EDGE_TYPES_HH

#include <string>

namespace Ultimaker{
 enum EdgeType{
     GC_WALL_OUTER = 0,
     GC_INFILL = 1,
     GC_MOVE = 2,
     GC_WALL_INNER = 3,
     GC_TOPBOTTOM = 4,
     GC_SUPPORT_ACC = 5,
     GC_SUPPORT_ACC_LE = 6,
     GC_SUPPORT = 7,
     GC_BRIM = 8,
     GC_RETRACT = 9, // Retract (no move allowed)
     GC_UNRETRACT = 10, // Unrectract (no move allowed)
     GC_PART = 11, // Inserted before starting a new part
     GC_PART_SYNC = 12, // Inserted instead of GC_PART, if the part is directly above the current one
     GC_COUNT
 };

 static std::string EdgeType_names[] = {
     "WALL_OUTER",
     "INFILL",
     "MOVE",
     "WALL_INNER",
     "TOPBOTTOM",
     "SUPPORT_ACC",
     "SUPPORT_ACC_LE",
     "SUPPORT",
     "BRIM",
     "RETRACT",
     "UNRETRACT",
     "GC_PART",
     "GC_PART_SYNC",
     "GC_COUNT"
 };
}


#endif //GCODE_EDGE_TYPES_HH