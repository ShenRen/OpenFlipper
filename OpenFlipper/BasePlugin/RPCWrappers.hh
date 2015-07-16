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

/**
 * \file RPCWrappers.hh
 * This file contains functions to call functions and procedures across plugins.
 * The QT Scripting system is used to pass the calls between different plugins.
 *
 * Usage is described in \ref RPCInterfacePage
 */

#ifndef RPCWRAPPERS_HH
#define RPCWRAPPERS_HH

#include <QtScript>
#include <vector>
#include <OpenFlipper/common/Types.hh>


/** Namespace containing RPC helper functions used to call functions across plugins
 *
 * Usage is described in \ref RPCInterfacePage
 */
namespace RPC {


//===========================================================================
/** @name Script Engine Controls
 *
 * Functions to get the scripting engine. Normally you don't need them. And
 * RPC::setScriptEngine should never be used!
 * @{ */
//===========================================================================

/** \brief get a pointer to OpenFlippers core scripting engine
 *
 */
DLLEXPORT
QScriptEngine* getScriptEngine();


/** \brief DONT USE! (Function to set the internal reference to the script Engine)
 *
 * Function to set the internal reference to the script engine from the core
 */
DLLEXPORT
void setScriptEngine( QScriptEngine* _engine );

/** @} */


//===========================================================================
/** @name Call functions across plugins (simple calls)
 *
 * These functions can be used to call functions in other plugins.
 * @{ */
//===========================================================================

/** \brief call a function provided by a plugin
 *
 * @param _plugin Plugin name ( Scripting name )
 * @param _functionName Name of the remote function
 */
DLLEXPORT
QScriptValue callFunction( QString _plugin, QString _functionName );

/** \brief Call a function provided by a plugin getting multiple parameters
 *
 * This function gets the parameters which are converted to a QScriptValue on your own.
 *
 * @param _plugin Plugin name ( Scripting name of the plugin )
 * @param _functionName Name of the remote function
 * @param _parameters vector of scriptvalues containing the functions parameters in the right order
 */
DLLEXPORT
QScriptValue callFunction( QString _plugin, QString _functionName , std::vector< QScriptValue > _parameters );

/** @} */

//===========================================================================
/** @name Call functions across plugins
 *
 * These templates can be used to call functions in other plugins.
 * @{ */
//===========================================================================

/** \brief call a function in another plugin
 *
 * @param _plugin       Plugin name ( Scripting name of the plugin )
 * @param _functionName Name of the remote function
 * @param _t0           Parameter passed to the function
 */
template <typename T0>
void callFunction( QString _plugin, QString _functionName, T0 _t0) {
  QScriptEngine* engine = getScriptEngine();
  std::vector< QScriptValue > parameters;
//   QVariant bla = qVariantFromValue(_t0);
//
//   IdList list = qVariantValue<IdList>( bla ) ;
//   std::cerr << "iDList size:" <<  list.size() << std::endl;;
//   std::cerr << list[0] << std::endl;
//   std::cerr << list[1] << std::endl;
//   std::cerr << "Type id is : " << QMetaType::type("IdList") << std::endl;
//   engine->globalObject().setProperty("ParameterData22",engine->toScriptValue(list));
/*
  QVariant blubb = engine->globalObject().property("ParameterData22").toVariant();
  IdList list1 = qVariantValue<IdList>( blubb ) ;
  std::cerr << "iDList1 size:" <<  list1.size() << std::endl;;
  std::cerr << list1[0] << std::endl;
  std::cerr << list1[1] << std::endl;
  std::cerr << "Type id is : " << QMetaType::type("IdList") << std::endl;*/


  parameters.push_back( engine->toScriptValue(_t0) );
  callFunction(_plugin,_functionName,parameters);
}

/** \brief call a function in another plugin
 *
 * @param _plugin       Plugin name ( Scripting name of the plugin )
 * @param _functionName Name of the remote function
 * @param _t0           Parameter 1 passed to the function
 * @param _t1           Parameter 2 passed to the function
 */
template <typename T0, typename T1>
void callFunction( QString _plugin, QString _functionName, T0 _t0 , T1 _t1) {
  QScriptEngine* engine = getScriptEngine();
  std::vector< QScriptValue > parameters;
  parameters.push_back( engine->toScriptValue( _t0 ) );
  parameters.push_back( engine->toScriptValue( _t1 ) );
  callFunction(_plugin,_functionName,parameters);
}

/** \brief call a function in another plugin
 *
 * @param _plugin       Plugin name ( Scripting name of the plugin )
 * @param _functionName Name of the remote function
 * @param _t0           Parameter 1 passed to the function
 * @param _t1           Parameter 2 passed to the function
 * @param _t2           Parameter 3 passed to the function
 */
template <typename T0, typename T1 , typename T2>
void callFunction( QString _plugin, QString _functionName, T0 _t0 , T1 _t1 , T2 _t2 ) {
  QScriptEngine* engine = getScriptEngine();
  std::vector< QScriptValue > parameters;
  parameters.push_back( engine->toScriptValue(_t0 ) );
  parameters.push_back( engine->toScriptValue( _t1 ) );
  parameters.push_back( engine->toScriptValue( _t2 ) );
  callFunction(_plugin,_functionName,parameters);
}

/** \brief call a function in another plugin
 *
 * @param _plugin       Plugin name ( Scripting name of the plugin )
 * @param _functionName Name of the remote function
 * @param _t0           Parameter 1 passed to the function
 * @param _t1           Parameter 2 passed to the function
 * @param _t2           Parameter 3 passed to the function
 * @param _t3           Parameter 4 passed to the function
 */
template <typename T0, typename T1 , typename T2,  typename T3>
void callFunction( QString _plugin, QString _functionName, T0 _t0 , T1 _t1 , T2 _t2 , T3 _t3 ) {
  QScriptEngine* engine = getScriptEngine();
  std::vector< QScriptValue > parameters;
  parameters.push_back( engine->toScriptValue( _t0 ) );
  parameters.push_back( engine->toScriptValue( _t1 ) );
  parameters.push_back( engine->toScriptValue( _t2 ) );
  parameters.push_back( engine->toScriptValue( _t3 ) );
  callFunction(_plugin,_functionName,parameters);
}

/** \brief call a function in another plugin
 *
 * @param _plugin       Plugin name ( Scripting name of the plugin )
 * @param _functionName Name of the remote function
 * @param _t0           Parameter 1 passed to the function
 * @param _t1           Parameter 2 passed to the function
 * @param _t2           Parameter 3 passed to the function
 * @param _t3           Parameter 4 passed to the function
 * @param _t4           Parameter 5 passed to the function
 */
template <typename T0, typename T1 , typename T2,  typename T3, typename T4>
void callFunction( QString _plugin, QString _functionName, T0 _t0 , T1 _t1 , T2 _t2 , T3 _t3 , T4 _t4) {
  QScriptEngine* engine = getScriptEngine();
  std::vector< QScriptValue > parameters;
  parameters.push_back( engine->toScriptValue( _t0 ) );
  parameters.push_back( engine->toScriptValue( _t1 ) );
  parameters.push_back( engine->toScriptValue( _t2 ) );
  parameters.push_back( engine->toScriptValue( _t3 ) );
  parameters.push_back( engine->toScriptValue( _t4 ) );
  callFunction(_plugin,_functionName,parameters);
}

/** \brief call a function in another plugin
 *
 * @param _plugin       Plugin name ( Scripting name of the plugin )
 * @param _functionName Name of the remote function
 * @param _t0           Parameter 1 passed to the function
 * @param _t1           Parameter 2 passed to the function
 * @param _t2           Parameter 3 passed to the function
 * @param _t3           Parameter 4 passed to the function
 * @param _t4           Parameter 5 passed to the function
 * @param _t5           Parameter 6 passed to the function
 */
template <typename T0, typename T1 , typename T2,  typename T3, typename T4, typename T5>
void callFunction( QString _plugin, QString _functionName, T0 _t0 , T1 _t1 , T2 _t2 , T3 _t3 , T4 _t4, T5 _t5) {
  QScriptEngine* engine = getScriptEngine();
  std::vector< QScriptValue > parameters;
  parameters.push_back( engine->toScriptValue( _t0 ) );
  parameters.push_back( engine->toScriptValue( _t1 ) );
  parameters.push_back( engine->toScriptValue( _t2 ) );
  parameters.push_back( engine->toScriptValue( _t3 ) );
  parameters.push_back( engine->toScriptValue( _t4 ) );
  parameters.push_back( engine->toScriptValue( _t5 ) );
  callFunction(_plugin,_functionName,parameters);
}

/** @} */

//===========================================================================
/** @name Call functions across plugins which return a value
 *
 * These templates can be used to call functions that return a value.
 * You have to pass the type of return value as the first template parameter.
  * @{ */
//===========================================================================

/** \brief call a function in another plugin and get a return parameter
 *
 * @param _plugin       Plugin name ( Scripting name of the plugin )
 * @param _functionName Name of the remote function
 * @return value returned by the called function
 */
template <typename ReturnValue >
ReturnValue callFunctionValue( QString _plugin, QString _functionName) {
  return qscriptvalue_cast< ReturnValue >( callFunction(_plugin,_functionName) );
}

/** \brief call a function in another plugin and get a return parameter
 *
 * @param _plugin       Plugin name ( Scripting name of the plugin )
 * @param _functionName Name of the remote function
 * @param _t0           Parameter 1 passed to the function
 * @return value returned by the called function
 */
template <typename ReturnValue , typename T0>
ReturnValue callFunctionValue( QString _plugin, QString _functionName, T0 _t0) {
  QScriptEngine* engine = getScriptEngine();
  std::vector< QScriptValue > parameters;
  parameters.push_back( engine->toScriptValue( _t0 ) );
  return qscriptvalue_cast<ReturnValue>( callFunction(_plugin,_functionName,parameters) );
}

/** \brief call a function in another plugin and get a return parameter
 *
 * @param _plugin       Plugin name ( Scripting name of the plugin )
 * @param _functionName Name of the remote function
 * @param _t0           Parameter 1 passed to the function
 * @param _t1           Parameter 2 passed to the function
 * @return value returned by the called function
 */
template <typename ReturnValue , typename T0, typename T1>
ReturnValue callFunctionValue( QString _plugin, QString _functionName, T0 _t0 , T1 _t1) {
  QScriptEngine* engine = getScriptEngine();
  std::vector< QScriptValue > parameters;
  parameters.push_back( engine->toScriptValue( _t0 ) );
  parameters.push_back( engine->toScriptValue( _t1 ) );
  return qscriptvalue_cast<ReturnValue>( callFunction(_plugin,_functionName,parameters) );
}

/** \brief call a function in another plugin and get a return parameter
 *
 * @param _plugin       Plugin name ( Scripting name of the plugin )
 * @param _functionName Name of the remote function
 * @param _t0           Parameter 1 passed to the function
 * @param _t1           Parameter 2 passed to the function
 * @param _t2           Parameter 3 passed to the function
 * @return value returned by the called function
 */
template <typename ReturnValue , typename T0, typename T1 , typename T2 >
ReturnValue callFunctionValue( QString _plugin, QString _functionName, T0 _t0 , T1 _t1 , T2 _t2 ) {
  QScriptEngine* engine = getScriptEngine();
  std::vector< QScriptValue > parameters;
  parameters.push_back( engine->toScriptValue( _t0 ) );
  parameters.push_back( engine->toScriptValue( _t1 ) );
  parameters.push_back( engine->toScriptValue( _t2 ) );
  return qscriptvalue_cast<ReturnValue>( callFunction(_plugin,_functionName,parameters) );
}

/** \brief call a function in another plugin and get a return parameter
 *
 * @param _plugin       Plugin name ( Scripting name of the plugin )
 * @param _functionName Name of the remote function
 * @param _t0           Parameter 1 passed to the function
 * @param _t1           Parameter 2 passed to the function
 * @param _t2           Parameter 3 passed to the function
 * @param _t3           Parameter 4 passed to the function
 * @return value returned by the called function
 */
template <typename ReturnValue , typename T0, typename T1 , typename T2, typename T3>
ReturnValue callFunctionValue( QString _plugin, QString _functionName, T0 _t0 , T1 _t1 , T2 _t2 , T3 _t3 ) {
  QScriptEngine* engine = getScriptEngine();
  std::vector< QScriptValue > parameters;
  parameters.push_back( engine->toScriptValue( _t0 ) );
  parameters.push_back( engine->toScriptValue( _t1 ) );
  parameters.push_back( engine->toScriptValue( _t2 ) );
  parameters.push_back( engine->toScriptValue( _t3 ) );
  return qscriptvalue_cast<ReturnValue>( callFunction(_plugin,_functionName,parameters) );
}

/** \brief call a function in another plugin and get a return parameter
 *
 * @param _plugin       Plugin name ( Scripting name of the plugin )
 * @param _functionName Name of the remote function
 * @param _t0           Parameter 1 passed to the function
 * @param _t1           Parameter 2 passed to the function
 * @param _t2           Parameter 3 passed to the function
 * @param _t3           Parameter 4 passed to the function
 * @param _t4           Parameter 5 passed to the function
 * @return value returned by the called function
 */
template <typename ReturnValue , typename T0, typename T1 , typename T2, typename T3, typename T4>
ReturnValue callFunctionValue( QString _plugin, QString _functionName, T0 _t0 , T1 _t1 , T2 _t2 , T3 _t3, T4 _t4 ) {
  QScriptEngine* engine = getScriptEngine();
  std::vector< QScriptValue > parameters;
  parameters.push_back( engine->toScriptValue( _t0 ) );
  parameters.push_back( engine->toScriptValue( _t1 ) );
  parameters.push_back( engine->toScriptValue( _t2 ) );
  parameters.push_back( engine->toScriptValue( _t3 ) );
  parameters.push_back( engine->toScriptValue( _t4 ) );
  return qscriptvalue_cast<ReturnValue>( callFunction(_plugin,_functionName,parameters) );
}

/** \brief call a function in another plugin and get a return parameter
 *
 * @param _plugin       Plugin name ( Scripting name of the plugin )
 * @param _functionName Name of the remote function
 * @param _t0           Parameter 1 passed to the function
 * @param _t1           Parameter 2 passed to the function
 * @param _t2           Parameter 3 passed to the function
 * @param _t3           Parameter 4 passed to the function
 * @param _t4           Parameter 5 passed to the function
 * @param _t5           Parameter 6 passed to the function
 * @return value returned by the called function
 */
template <typename ReturnValue , typename T0, typename T1 , typename T2, typename T3, typename T4, typename T5>
ReturnValue callFunctionValue( QString _plugin, QString _functionName, T0 _t0 , T1 _t1 , T2 _t2 , T3 _t3, T4 _t4 , T5 _t5 ) {
  QScriptEngine* engine = getScriptEngine();
  std::vector< QScriptValue > parameters;
  parameters.push_back( engine->toScriptValue( _t0 ) );
  parameters.push_back( engine->toScriptValue( _t1 ) );
  parameters.push_back( engine->toScriptValue( _t2 ) );
  parameters.push_back( engine->toScriptValue( _t3 ) );
  parameters.push_back( engine->toScriptValue( _t4 ) );
  parameters.push_back( engine->toScriptValue( _t5 ) );
  return qscriptvalue_cast<ReturnValue>( callFunction(_plugin,_functionName,parameters) );
}

/** @} */


}

#endif // RPCWRAPPERS_HH
