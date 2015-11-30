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



//=============================================================================
//
//  CLASS Core - IMPLEMENTATION
//
//=============================================================================


//== INCLUDES =================================================================

// -------------------- mview
#include "Core.hh"
// -------------------- ACG

// -------------------- Qt

//== IMPLEMENTATION ========================================================== 

// 0m - all attibutes off
// 1m - intensity
// 4m - underscore
// 5m - blinking
// 7m - reverse

// 30m - black
// 31m - red
// 32m - green
// 33m - yellow
// 34m - blue
// 35m - pink
// 36m - cyan
// 37m - white
// 
// 
// to get background colours change the 3's to 4's - eg 43m is yellow background, 33m is yellow text.

/** \brief Slot writing everything to the Logger widget
 * 
 * This slot has to be called by all loggers. It is used to serialize
 * and color the Output.
 * 
 * @param _type Logtype (defines the color of the output)
 * @param _message The message for output
 **/
void 
Core::
slotLog(Logtype _type, QString _message) {
  emit externalLog( _type , _message );
  if ( OpenFlipper::Options::logToConsole()  ) {
    std::string output( _message.toLatin1() ) ;
    
    switch (_type) {
      case LOGINFO:
        std::clog << "\33[32m" << output << std::endl;
        break;
      case LOGOUT:
        std::cout << "\33[0m"  << output << std::endl;
        break;
      case LOGWARN:
        std::cout << "\33[33m" << output << std::endl;
        break;
      case LOGERR:
        std::cerr << "\33[31m" << output << std::endl;
        break;
      case LOGSTATUS:
        std::cerr << "\33[34m" << output << std::endl;
        break;
    }
  }
  
}

//=============================================================================
