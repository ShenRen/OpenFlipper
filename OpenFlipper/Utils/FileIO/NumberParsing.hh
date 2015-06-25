/*===========================================================================*\
*                                                                            *
*                              OpenFlipper                                   *
*      Copyright (C) 2001-2015 by Computer Graphics Group, RWTH Aachen       *
*                           www.openflipper.org                              *
*                                                                            *
*--------------------------------------------------------------------------- *
*  This file is part of OpenFlipper.                                         *
*                                                                            *
*  OpenFlipper is free software: you can redistribute it and/or modify       *
*  it under the terms of the GNU Lesser General Public License as            *
*  published by the Free Software Foundation, either version 3 of            *
*  the License, or (at your option) any later version with the               *
*  following exceptions:                                                     *
*                                                                            *
*  If other files instantiate templates or use macros                        *
*  or inline functions from this file, or you compile this file and          *
*  link it with other files to produce an executable, this file does         *
*  not by itself cause the resulting executable to be covered by the         *
*  GNU Lesser General Public License. This exception does not however        *
*  invalidate any other reasons why the executable file might be             *
*  covered by the GNU Lesser General Public License.                         *
*                                                                            *
*  OpenFlipper is distributed in the hope that it will be useful,            *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of            *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
*  GNU Lesser General Public License for more details.                       *
*                                                                            *
*  You should have received a copy of the GNU LesserGeneral Public           *
*  License along with OpenFlipper. If not,                                   *
*  see <http://www.gnu.org/licenses/>.                                       *
*                                                                            *
\*===========================================================================*/

/*===========================================================================*\
*                                                                            *
*   $Revision: 19199 $                                                       *
*   $LastChangedBy: moeller $                                                *
*   $Date: 2014-07-11 14:06:27 +0200 (Fr, 11 Jul 2014) $                     *
*                                                                            *
\*===========================================================================*/

/**
 * \file NumberParsing.hh
 * This file contains functions which can be used to parse numbers from streams.
 * The algorithms are designed to parse Numbers very fast, however for numbers with scientific notation the performance may not improve.
 * (e.g. parsing 123e10 will be slower with this algorithm)
 **/

#pragma once

#include "OpenFlipper/common/GlobalDefines.hh"
#include <QTextStream>

namespace Utils{

/**
 * @brief getDouble parses a QTextStream for a double. the double has to be the next word in the QTextStream.
 * if parsing fails, the standard extraction is performed and returned.
 * @param _source The QTextStream that holds at least one double.
 * @return double the extracted double.
 */
DLLEXPORT
double getDouble( QTextStream& _source);

/**
 * @brief getFloat parses a QTextStream for a float. the float has to be the next word in the QTextStream.
 * if parsing fails, the standard extraction is performed and returned.
 * @param _source The QTextStream that holds at least one float.
 * @return float the extracted float.
 */
DLLEXPORT
float getFloat( QTextStream& _source);

/**
 * @brief getFloat parses a std::istream for a float. the float has to be the next word in the std::istream.
 * if parsing fails, the standard extraction is performed and returned.
 * @param _source The std::istream that holds at least one float.
 * @return float the extracted float.
 */
DLLEXPORT
float getFloat( std::istream& _source);
}

