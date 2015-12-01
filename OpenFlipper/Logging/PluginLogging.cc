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
//  CLASS MViewWidget - IMPLEMENTATION
//
//=============================================================================


//== INCLUDES =================================================================


#include "Logging.hh"
#include <QStringList>


//== IMPLEMENTATION ==========================================================

/** 
 * @param _pluginName Name of the Plugin which uses this logger (Prepended to all Output)
 * @param _type Default log type ( used for streaming operations )
 */
PluginLogger::PluginLogger(QString _pluginName , Logtype _type) :
    pluginName_(_pluginName),
    defaultLogType_(_type)
{
}

PluginLogger::~PluginLogger() {
}

/** Receive log events from plugins and pass them to core
 * @param _type Logtype (defines the color of the output)
 * @param _message The message for output
 */
void PluginLogger::slotLog(Logtype _type, QString _message) {
  QStringList strings;
  strings = _message.split("\n",QString::SkipEmptyParts);
  for ( int i = 0 ; i < strings.size(); ++i )
    emit log(_type,pluginName_ + " : " + strings[i]);
}

/** Receive log events from plugins (defaults to LOGOUT Message type)
 * 
 * @param _message The message for output
 */
void PluginLogger::slotLog(QString _message) {
  QStringList strings;
  strings = _message.split("\n",QString::SkipEmptyParts);
  for ( int i = 0 ; i < strings.size(); ++i )
    emit log(LOGOUT,pluginName_ + " : " + strings[i]);
}

/** This operator is used for streaming ( e.g. catch omerr and omerr streams) 
 *  and redirect them to the logging widget. The default log level passed to
 *  the constructor will be used for the message
 * 
 * @param _s String to display
 */
void PluginLogger::operator<< ( const std::string& _s ) {
  slotLog(defaultLogType_,QString( _s.c_str() ));
}


//=============================================================================
