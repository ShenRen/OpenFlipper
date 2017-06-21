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

#ifndef SCRIPTINGPLUGIN_HH
#define SCRIPTINGPLUGIN_HH

#include <QObject>
#include <QMenuBar>

#include <OpenFlipper/common/Types.hh>
#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/MenuInterface.hh>
#include <OpenFlipper/BasePlugin/ScriptInterface.hh>
#include <OpenFlipper/BasePlugin/RPCInterface.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>



class QJSEngine;

class ScriptingBackend : public QObject, BaseInterface, MenuInterface, ScriptInterface, RPCInterface, LoggingInterface
{
Q_OBJECT
Q_INTERFACES(BaseInterface)
Q_INTERFACES(MenuInterface)
Q_INTERFACES(ScriptInterface)
Q_INTERFACES(RPCInterface)
Q_INTERFACES(LoggingInterface)

#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "org.OpenFlipper.Plugins.Plugin-Scripting")
#endif

    QJSEngine* jsEngine_ = nullptr;

public:
signals:

    void log(Logtype _type, QString _message);
    void log(QString _message);

private slots:


  void pluginsInitialized();

  void noguiSupported( ) {} ;

public :

  ScriptingBackend();
  virtual ~ScriptingBackend();

  QString name() { return (QString(tr("Scripting Backend"))); };
  QString description( ) { return (QString(tr("Provides Updated Scripting for OpenFlipper"))); };


public slots:
  QString version() { return QString("1.0"); };

  void slotExecuteScript(QString script);
  void slotExecuteFileScript(QString script);
};


class PrintHelper : public QObject
{
Q_OBJECT

    ScriptingBackend* backend_;
public:
    PrintHelper(ScriptingBackend* backend)
        :   backend_(backend) {}

public slots:
    void print(QVariantList args)
    {
        QString result;
        for(int i= 0; i < args.size(); ++i) {
            if(i > 0) {
               result.append(" ");
            }
            result.append(args[i].toString());
        }
        backend_->log(result);
    }

    void printToFile(QVariantList args)
    {
        QString result;
        // first argument is the file path
        for(int i= 1; i < args.size(); ++i) {
            if(i > 0) {
               result.append(" ");
            }
            result.append(args[i].toString());
        }
        QFile file(args[0].toString());
        file.open(QIODevice::Append);
        QTextStream stream(&file);
        stream << result << "\n";
        file.close();
    }
};




#endif //SCRIPTINGPLUGIN_HH

