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

#include "ScriptingBackendPlugin.hh"

#include "OpenFlipper/common/GlobalOptions.hh"
#include <qjsvalueiterator.h>

#if QT_VERSION >= 0x050000
#else
#include <QtGui>
#endif

#include <QJSEngine>
#include <QJSValue>
#include <QQmlEngine>

namespace {
    // Forces garbage collection for a QJSEngine
    void triggerGarbageCollection(QJSEngine* engine)
    {
        if(engine != nullptr) {
            engine->collectGarbage();
        }
    }
}


ScriptingBackend::ScriptingBackend()
    :   jsEngine_(new QJSEngine())
{

}

ScriptingBackend::~ScriptingBackend()
{
    jsEngine_->collectGarbage();
    delete jsEngine_;
}


void ScriptingBackend::pluginsInitialized()
{
    // NOTE: See ScriptingBackend constructor
    // TODO: Replace with std::make_unique when we start to support c++14
    //jsEngine_.reset(new QJSEngine());
    exposeObjectImpl("scriptingBackend", this, ScriptObjectOwnership::CPP_OWNERSHIP);

    /* Register PrintHelper object and create global print() and printToFile() function aliases */
    exposeObjectImpl("PrintInterface", new PrintHelper(this), ScriptObjectOwnership::CPP_OWNERSHIP);

    QJSValue jsPrint = jsEngine_->evaluate("function() { PrintInterface.print(Array.prototype.slice.apply(arguments)); }");
    jsEngine_->globalObject().setProperty("print", jsPrint);

    QJSValue jsPrintToFile = jsEngine_->evaluate("function() { PrintInterface.printToFile(Array.prototype.slice.apply(arguments)); }");
    jsEngine_->globalObject().setProperty("printToFile", jsPrintToFile);
}

void ScriptingBackend::slotExecuteScript(QString script)
{
    lastExecutionResult = jsEngine_->evaluate(script);
}

void ScriptingBackend::slotExecuteFileScript(QString script)
{
    log("executeFileScript: " + script);
}

void ScriptingBackend::slotGetLastScriptExecutionResult(ScriptExecutionResult& result)
{
	result.isError = lastExecutionResult.isError();
	result.fileName = lastExecutionResult.property("fileName").toString();
	result.lineNumber = lastExecutionResult.property("lineNumber").toInt();
	result.message = lastExecutionResult.property("message").toString();
	result.stacktrace = lastExecutionResult.property("stack").toString();
}

QJSValue ScriptingBackend::exposeObjectImpl(QString _name, QObject* _object, ScriptObjectOwnership _ownership)
{
    QStringList properties;
    return exposeObjectImpl(_name, _object, properties, _ownership);
}

QJSValue ScriptingBackend::exposeObjectImpl(QString _name, QObject* _object, QStringList& _properties, ScriptObjectOwnership _ownership)
{
    QJSValue asJsObject = jsEngine_->newQObject(_object);
    QJSValueIterator it(asJsObject);
    while (it.hasNext()) {
        it.next();
        _properties.push_back(it.name());
    }
    jsEngine_->globalObject().setProperty(_name, asJsObject);

    QQmlEngine::ObjectOwnership ownership = _ownership == ScriptObjectOwnership::CPP_OWNERSHIP ? QQmlEngine::CppOwnership : QQmlEngine::JavaScriptOwnership;
    QQmlEngine::setObjectOwnership(_object, ownership);

    return asJsObject;
}

void ScriptingBackend::slotExposeObject(QString _name, QObject* _object, ScriptObjectOwnership _ownership)
{
	QStringList properties;
    exposeObjectImpl(_name, _object, properties, _ownership);
}

void ScriptingBackend::slotExposeObject(QString _name, QObject* _object, QStringList& _properties, ScriptObjectOwnership _ownership)
{
    exposeObjectImpl(_name, _object, _properties, _ownership);
}

void ScriptingBackend::slotSetGlobalProperty(QString _name, QVariant _value)
{
    QJSValue value = _value.toString();
    jsEngine_->globalObject().setProperty(_name, value);
}

#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2( skriptingbackendplugin , ScriptingBackendPlugin );
#endif


