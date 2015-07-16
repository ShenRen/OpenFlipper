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

//== INCLUDES =================================================================

#ifndef VSIPLUGIN_HH
#define VSIPLUGIN_HH

#include <QObject>
#include <QScriptValue>

#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/MenuInterface.hh>
#include <OpenFlipper/BasePlugin/ScriptInterface.hh>
#include <OpenFlipper/BasePlugin/RPCInterface.hh>

//== FORWARDDECLARATIONS ======================================================

namespace VSI {
  class Context;
  class BaseWidget;
}

//== CLASS DEFINITION =========================================================

/** Plugin that provides a visual scripting interface
*/
class VsiPlugin : public QObject, BaseInterface, MenuInterface, ScriptInterface, RPCInterface
{
  Q_OBJECT
  Q_INTERFACES(BaseInterface)
  Q_INTERFACES(MenuInterface)
  Q_INTERFACES(ScriptInterface)
  Q_INTERFACES(RPCInterface)

#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "org.OpenFlipper.Plugins.Plugin-VSI")
#endif

  signals:

    // MenuInterface
    void getMenubarMenu (QString _name, QMenu *& _menu, bool _create);

    // ScriptInterface
    void getScriptingEngine (QScriptEngine*& _engine);

    // RPC Interface
    void pluginExists (QString _pluginName, bool& _exists) ;
    void functionExists (QString _pluginName, QString _functionName, bool& _exists);

  public:

    /// Constructor
    VsiPlugin ();

    /// Destructor
    ~VsiPlugin ();

    /// Name of the Plugin
    QString name () { return QString ("Visual Scripting"); };

    /// Description of the Plugin
    QString description () { return QString ("Visual Scripting interface for OpenFlipper"); };

  public slots:

    /// Scripting function, that allows to ask the user for inputs during script execution
    QScriptValue askForInputs (QString _element, QString _inputs);

    /// Scripting function, that displays a message box
    void messageBox (QString _message);

    /// Scripting function, that displays a Yes/No message box
    bool questionBox (QString _message);

    /// Shows a non blocking stop box for use inside loops
    bool continueBox(QString _message);

    QString version () { return QString("1.0"); };

  private slots:
    void pluginsInitialized ();

    void noguiSupported () {};

    /// Shows visual script editor
    void showScriptEditor ();

    /// Opens the text based script editor with the given script
    void showInScriptEditor (QString _script);

  private:

    /// initalisation
    void initContext ();

  private:

    VSI::Context *context_;
    VSI::BaseWidget *baseWidget_;
};

//=============================================================================

class QContinueBox : public QWidget {
  Q_OBJECT

public:
  QContinueBox(QString _message,QWidget* _parent = 0) :
    QWidget(_parent),
    continue_(true)
  {
    QPushButton* stopButton = new QPushButton("Stop",this);
    QHBoxLayout* layout = new QHBoxLayout(this);

    this->setWindowTitle(_message);

    layout->addWidget(stopButton);
    this->setLayout(layout);

    connect(stopButton,SIGNAL(clicked()), this, SLOT(clicked()));
  }

public slots:
  void clicked( ) {
    continue_ = false;
  }

public:
  bool continueBox() { return continue_; };

private:
  bool continue_;
};


//=============================================================================
//=============================================================================

#endif
