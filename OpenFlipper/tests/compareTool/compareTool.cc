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

#include <QString> 
#include <QStringList> 
#include <QSettings>
#include <QFileInfo>
#include <QTextStream>

#include <cmath>
#include <limits>
#include <iostream>


/* Test File specification:
Doubles can be given as:

Exact test:
VALUE_DOUBLE=1.0

Tolerance test (0.0001)
VALUE_DOUBLE=1.0,0.0001


*/


bool compareDouble(QString _key , QVariant _result, QVariant _reference) {

  // maximal allowed double tolerance
  double tolerance = std::numeric_limits<double>::epsilon();

  // Check if the reference consists of two colon separated values
  // Second value would specify maximal allowed tolerance
  QStringList referenceData = _reference.toString().split(':');
  if ( referenceData.size() == 2) {
    tolerance = referenceData[1].toDouble();
  }


  double result    = _result.toDouble();
  double reference = referenceData[0].toDouble();

  if ( fabs(result-reference) <= tolerance )  {
    return true;
  } else {

    //qcerr.setRealNumberPrecision(40);
	std::cerr.precision(40);
    std::cerr << "===================================================================\n";
	std::cerr << "Comparison failed for key " <<  _key.toStdString() << " :\n";
    std::cerr << "Result:                " << result << "\n";
    std::cerr << "Expected:              " << reference << "\n";
    std::cerr << "Difference:            " << fabs(result-reference) << "\n";
    std::cerr << "Allowed tolerance was: " << tolerance << "\n";
    std::cerr << "===================================================================\n";
    return false;
  }
  
}

bool compareString(QString _key ,QVariant _result, QVariant _reference) {

  QString resultStr = _result.toString().simplified();
  QString resultRef = _reference.toString().simplified();
  
  if (resultStr == resultRef ) {
    return true;
  } else {
	std::cerr << "Comparison failed for key " <<  _key.toStdString() << " :\n";
    std::cerr << "Result: " << resultStr.toStdString() << " ; Expected: " << resultRef.toStdString() << "\n";
    return false;
  }

}

int main(int argv, char **args)
{
  std::cout << "============================================================\n" ;
  std::cout << "Executing compare tool\n";
  std::cout << "Comparing results to reference:\n" ;

  // Flag if everything went fine
  bool ok = true;

  // Return if we did not get exactly two arguments
  if ( argv != 3 ) {
    std::cerr << "Wrong number of arguments!\n";
    std::cerr << "Usage:\n";
    std::cerr << "compareTool ResultFile ReferenceFile\n";
    return(1);
  }

  QString file1(args[1]);
  QString file2(args[2]);

  QFileInfo resultFileInfo(file1);
  if ( !resultFileInfo.exists() ) {
    std::cerr << "ERROR! Result file: " << file1.toStdString() << " does not exist!\n";

    return 1;
  }

  QFileInfo referenceFileInfo(file2);
  if ( !referenceFileInfo.exists() ) {
    std::cerr << "ERROR! Reference file: " << file2.toStdString() << " does not exist!\n";
    return 1;
  }

  QSettings resultFile(file1,QSettings::IniFormat);
  QSettings referenceFile(file2,QSettings::IniFormat);
  
  if ( resultFile.status() != QSettings::NoError) {
    std::cerr << "QSettings error when opening result file: " << file1.toStdString() << "\n";
    return 1;
  }

  if ( referenceFile.status() != QSettings::NoError) {
    std::cerr << "QSettings error when opening result reference file: " << file2.toStdString() << "\n";
    return 1;
  }

  QStringList toplevelKeys = referenceFile.childKeys();
  QStringList groups       = referenceFile.childGroups();
   
  if ( groups.size() == 0 ) {
   for ( int i = 0 ; i < toplevelKeys.size(); ++i) {
     if ( resultFile.contains(toplevelKeys[i]) ) {
       if ( toplevelKeys[i].endsWith("_DOUBLE") ) {
         ok &= compareDouble(toplevelKeys[i],resultFile.value(toplevelKeys[i]), referenceFile.value(toplevelKeys[i]));
       } else
         ok &= compareString( toplevelKeys[i],resultFile.value(toplevelKeys[i]), referenceFile.value(toplevelKeys[i]));
     } else {
       std::cerr << "Missing key in result file: " << toplevelKeys[i].toStdString() << "\n";
       ok = false;
     }
 
   }   
 

  } else {
    std::cerr << "Multiple levels!" << "\n";
    return 1;
  }
  

  if ( ! ok ) {
    std::cerr << "At least one of the tests failed!\n";
    return 1;
  }

  return(0);
}
