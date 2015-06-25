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

#include "NumberParsing.hh"
#include <QString>
#include <cmath>
#include <sstream>

namespace Utils
{

  float getFloat(QTextStream &_source)
  {
    QString rawNumber;
    _source >> rawNumber;
    //decimal part > 0
    float highPart = 0.0f;
    //decimal part < 0
    float lowPart = 0.0f;
    //sign of the decimal
    float sign = 1.0;
    //counter to concatenate high and lowaprt
    int n = 0;

    QString::Iterator it = rawNumber.begin();
    QString::Iterator end = rawNumber.end();
    if(*it == QLatin1Char('-'))
    {
      //we have a negative float
      sign = sign * -1.0;
      it++;
    }

    for (;it != end;it++)
    {
      //we have read a digit
      if(it->isDigit())
      {
        highPart *=10;
        highPart += it->digitValue();
      }
      else
      {
        //stop counting the highPart if its not a digit
        it++;
        break;
      }
    }

    for (;it != end;it++)
    {
      //we assume to have read a digit
      if(it->isDigit())
      {
        lowPart *=10;
        lowPart += it->digitValue();
        n++;
      }
      else
      {
        //stop counting the highPart dont increment here!
        // otherwise we cant detect if we successful converted
        break;
      }
    }
    // if something went wrong during decoding use the standard decoding
    if(it != end)
    {
      return rawNumber.toFloat();
    }
    return sign * (highPart + lowPart / std::pow(10.0,n));
  }

  float getFloat(std::istream &_source)
  {
    std::string rawNumber;
    _source >> rawNumber;

    //decimal part > 0
    float highPart = 0.0f;
    //decimal part < 0
    float lowPart = 0.0f;
    //sign of the decimal
    float sign = 1.0;
    //counter to concatenate high and lowaprt
    int n = 0;

    std::string::iterator it = rawNumber.begin();
    std::string::iterator end = rawNumber.end();
    if(*it == '-')
    {
      //we have a negative float
      sign = sign * -1.0;
      it++;
    }

    for (;it != end;it++)
    {
      //we have read a digit
      if(isdigit(*it))
      {
        highPart *=10;
        highPart += (int)(*it - '0');
      }
      else
      {
        //stop counting the highPart if its not a digit
        it++;
        break;
      }
    }

    for (;it != end;it++)
    {
      //we assume to have read a digit
      if(isdigit(*it))
      {
        lowPart *=10;
        lowPart += (int)(*it - '0');
        n++;
      }
      else
      {
        //stop counting the highPart dont increment here!
        // otherwise we cant detect if we successful converted
        break;
      }
    }
    // if something went wrong during decoding use the standard decoding
    if(it != end)
    {
      float fallback;
      std::stringstream converter;
      converter<<rawNumber;
      converter>>fallback;
      return fallback;
    }
    return sign * (highPart + lowPart / std::pow(10.0,n));
  }

  double getDouble(QTextStream &_source)
  {
    QString rawNumber;
    _source >> rawNumber;
    //decimal part > 0
    double highPart = 0.0f;
    //decimal part < 0
    double lowPart = 0.0f;
    //sign of the decimal
    double sign = 1.0;
    //counter to concatenate high and lowaprt
    int n = 0;

    QString::Iterator it = rawNumber.begin();
    QString::Iterator end = rawNumber.end();
    if(*it == QLatin1Char('-'))
    {
      //we have a negative float
      sign = sign * -1.0;
      it++;
    }

    for (;it != end;it++)
    {
      //we have read a digit
      if(it->isDigit())
      {
        highPart *=10;
        highPart += it->digitValue();
      }
      else
      {
        //stop counting the highPart if its not a digit
        it++;
        break;
      }
    }

    for (;it != end;it++)
    {
      //we assume to have read a digit
      if(it->isDigit())
      {
        lowPart *=10;
        lowPart += it->digitValue();
        n++;
      }
      else
      {
        //stop counting the highPart dont increment here!
        // otherwise we cant detect if we successful converted
        break;
      }
    }
    // if something went wrong during decoding use the standard decoding
    if(it != end)
    {
      return rawNumber.toDouble();
    }
    return sign * (highPart + lowPart / std::pow(10.0,n));
  }
}
