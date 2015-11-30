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

//== IMPLEMENTATION ==========================================================



void Core::slotPluginExists( QString _pluginName , bool& _exists ) {

  for ( int i = 0 ; i < (int)plugins_.size(); ++i ) {
    if ( plugins_[i].rpcName == _pluginName ) {
      _exists = true;
      return;
    }
  }

  _exists = false;
}

void Core::slotFunctionExists( QString _pluginName , QString _functionName , bool& _exists  ) {

  //Find plugin
  int plugin = -1;
  for ( int i = 0 ; i < (int)plugins_.size(); ++i ) {
    if ( plugins_[i].rpcName == _pluginName ) {
      plugin = i;
      break;
    }
  }

  if ( plugin == -1 ) {
    _exists = false;
    return;
  }

  _exists = plugins_[plugin].rpcFunctions.contains(_functionName);
}

void Core::slotCall( QString _pluginName , QString _functionName , bool& _success  ) {

  //Find plugin
  int plugin = -1;
  for ( int i = 0 ; i < (int)plugins_.size(); ++i ) {
    if ( plugins_[i].rpcName == _pluginName ) {
      plugin = i;
      break;
    }
  }

  if ( plugin == -1 ) {
    _success = false;
    emit log(LOGERR, tr("Unable to call function from Plugin : ") + _pluginName + tr(" ( Plugin not Found! )"));
    return;
  }

  if ( !plugins_[plugin].rpcFunctions.contains(_functionName) ) {
    _success = false;
    emit log(LOGERR, tr("Unable to call function from Plugin : ") + _pluginName);
    emit log(LOGERR, tr("Function ") + _functionName + tr(" not found!"));
    return;
  }

  scriptEngine_.evaluate(_pluginName + "." + _functionName );
  if ( scriptEngine_.hasUncaughtException() ) {
    _success = false;
    QScriptValue result = scriptEngine_.uncaughtException();
    QString exception = result.toString();
    emit log( LOGERR , tr("RPC failed with : ") + exception );
    return;
  }

  _success = true;

}

void Core::slotCall( QString _expression , bool& _success  ) {

  scriptEngine_.evaluate( _expression );
  if ( scriptEngine_.hasUncaughtException() ) {
    _success = false;
    QScriptValue result = scriptEngine_.uncaughtException();
    QString exception = result.toString();
    emit log( LOGERR , tr("RPC failed with : ") + exception );
    return;
  }

  _success = true;

}

void Core::slotGetValue(QString _expression, QVariant& _result ){
  //execute the expression
  bool ok;

  slotCall("var tmpValue=" + _expression + ";", ok);

  if (!ok){
    _result = QVariant();
    return;
  }

  //get the return value
  QScriptValue val = scriptEngine_.globalObject().property("tmpValue");

//   std::cerr << "Type:" << val.toVariant().userType() << std::endl; 
//   std::cerr << "Value:" << val.toVariant().toString().toStdString()<< std::endl;

  _result = val.toVariant();
}

//=============================================================================
