#pragma once

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
