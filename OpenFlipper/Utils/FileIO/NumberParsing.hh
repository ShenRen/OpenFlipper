/**
 * \file NumberParsing.hh
 * This file contains functions which can be used to parse numbers from streams.
 * The algorithms are designed to parse Numbers very fast, however for numbers with scientific notation the performance may not improve.
 * (e.g. parsing 123e10 will be slower with this algorithm)
 **/

#ifndef NUMBERPARSING_HH
#define NUMBERPARSING_HH
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

#endif // NUMBERPARSING_HH
