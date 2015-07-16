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
//  Logging System
//
//=============================================================================

#ifndef LOGGING_HH
#define LOGGING_HH

//=============================================================== INCLUDES ====

#include <iostream>

#include <QObject>
#include "OpenFlipper/BasePlugin/LoggingInterface.hh"

//======================================================= CLASS DEFINITION ====
/**
 * \file Logging.hh 
 * This File contains the header for the log wrapper used for Plugins
*/


/** A wrapper class, getting events from an plugin, prepending plugin name to the log
 * and passing it to the core. This class is generated for every plugin of the core.
 * It also does some cleanup of the passed strings.\n
 */
class PluginLogger : public QObject {
  
  Q_OBJECT
  
  public:
    
    /// Standard Constructor 
    PluginLogger(QString _pluginName , Logtype _type = LOGOUT);
    
    /// Destructor
    ~PluginLogger();
  
    /// Provide operator for streaming 
    void operator<< ( const std::string& _s ); 
    
  private slots:
    
    /// Log messages with a given Logtype
    void slotLog(Logtype _type, QString _message);
    
    /// Wrapper which logs at LOGOUT Logtype by default
    void slotLog(QString _message);
    
  signals:
    /// Sends the generated logs to the core
    void log(Logtype , QString);
    
  private:
    /// prepend this name to all output messages
    QString pluginName_;
    
    /// Default logtype ( used for streams )
    Logtype defaultLogType_;
    
};


//=============================================================================
#endif // LOGGING_HH
//=============================================================================
