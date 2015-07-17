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
