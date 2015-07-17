/* ========================================================================= *
 *                                                                           *
 *                               OpenFlipper                                 *
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
 *                                                                           *
 * ========================================================================= */

/*===========================================================================*\
*                                                                            *
*   $Revision: 13620 $                                                       *
*   $LastChangedBy: moebius $                                                *
*   $Date: 2012-02-01 14:51:25 +0100 (Mi, 01 Feb 2012) $                     *
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

