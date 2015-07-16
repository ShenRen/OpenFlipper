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
//  CLASS VsiPlugin - IMPLEMENTATION
//
//=============================================================================

//== INCLUDES =================================================================

#include <QAction>
#include <QIcon>
#include <QScriptEngine>
#include <QMessageBox>

#include <OpenFlipper/BasePlugin/PluginFunctions.hh>

#include <OpenFlipper/common/GlobalOptions.hh>

#include "vsiPlugin.hh"

#include "baseWidget.hh"
#include "parser/context.hh"
#include "parser/element.hh"
#include "parser/input.hh"
#include "config/dynamicDialog.hh"

//------------------------------------------------------------------------------

/// Constructor
VsiPlugin::VsiPlugin () :
  context_ (0),
  baseWidget_ (0)
{ 
}

//------------------------------------------------------------------------------

/// Destructor
VsiPlugin::~ VsiPlugin()
{
  if (context_)
    delete context_;
  if (baseWidget_)
    delete baseWidget_;
}

//------------------------------------------------------------------------------

/// Register in menubar
void VsiPlugin::pluginsInitialized ()
{
  if (OpenFlipper::Options::nogui ())
    return;

  QMenu *scriptingMenu;

  emit emit getMenubarMenu(tr("&Scripting"), scriptingMenu, true );

  QAction* showEditor = scriptingMenu->addAction ("Visual script editor");
  showEditor->setIcon( QIcon(OpenFlipper::Options::iconDirStr()+OpenFlipper::Options::dirSeparator()+"vsi_vsiEditor.png") );

  connect (showEditor, SIGNAL(triggered()) ,
           this      , SLOT(showScriptEditor()));
  
  emit addMenubarAction(showEditor, TOOLSMENU );

}

//------------------------------------------------------------------------------

/// Show visual script editor
void VsiPlugin::showScriptEditor()
{
  initContext ();
  if (!baseWidget_)
  {
    baseWidget_ = VSI::BaseWidget::createBaseWidget (context_);
    connect (baseWidget_, SIGNAL (codeToScriptEditor (QString)),
             this, SLOT (showInScriptEditor(QString)));
  }

  baseWidget_->show ();
}

/// initalize context
void VsiPlugin::initContext()
{
  if (context_)
    return;

  QScriptEngine *engine;
  emit getScriptingEngine (engine);

  // empty context
  context_ = new VSI::Context (engine);

  // parse all metadata xml files
  QDir dir = OpenFlipper::Options::dataDir ();

  dir.cd ("VsiMetadata");

  if (!dir.exists ())
    return;

  foreach (QString sub, dir.entryList(QDir::Dirs))
  {
    if (sub == "..")
      continue;
    
    QDir subdir = dir;
    subdir.cd (sub);
    subdir.makeAbsolute();
    
    foreach (QString file, subdir.entryList (QStringList("*.xml"), QDir::Files))
    {
      QFile f (subdir.filePath (file));
      if (!f.open (QIODevice::ReadOnly))
	continue;

      context_->parse (f.readAll ());
    }
  }
  
}

//------------------------------------------------------------------------------

/// Gererates a dialog for the given element inputs
QScriptValue VsiPlugin::askForInputs(QString _element, QString _inputs)
{
  initContext ();

  VSI::Element *e = context_->element (_element);

  if (!e)
    return QScriptValue ();

  QVector<VSI::Input *> inputs;

  foreach (QString s, _inputs.split (",", QString::SkipEmptyParts))
    foreach (VSI::Input *i, e->inputs ())
    {
      if (i->name () == s)
      {
        inputs.append (i);
        break;
      }
    }

  if (inputs.isEmpty ())
    return QScriptValue ();

  VSI::DynamicDialog d (inputs);
  d.setWindowTitle (e->shortDescription () + " Input Configuration");
  d.exec ();

  QMap<QString, QString> results = d.getResults ();

  QString script = "inputs = { ";

  foreach (QString s, _inputs.split (",", QString::SkipEmptyParts))
    script += s + ": " + results[s] + ",";

  script.remove (script.length () - 1, 1);

  script += "};";

  context_->scriptEngine ()->pushContext ();
  QScriptValue rv = context_->scriptEngine ()->evaluate (script);
  context_->scriptEngine ()->globalObject ().setProperty ("inputs", rv);
  //rv = context_->scriptEngine ()->globalObject ().property ("inputs");
  context_->scriptEngine ()->popContext ();

  return rv;
}

//------------------------------------------------------------------------------

/// shows the given script in the textual script editor
void VsiPlugin::showInScriptEditor(QString _script)
{
  bool ok;

  emit functionExists ("scripting", "showScriptInEditor(QString)", ok);

  if (!ok)
    return;

  RPC::callFunction ("scripting", "showScriptInEditor", _script);
}

//------------------------------------------------------------------------------

/// Scripting function, that displays a message box
void VsiPlugin::messageBox (QString _message)
{
  QMessageBox msgBox;
  msgBox.setText(_message);
  msgBox.exec();
}

//------------------------------------------------------------------------------

/// Scripting function, that displays a Yes/No message box
bool VsiPlugin::questionBox (QString _message)
{
  QMessageBox msgBox;
  msgBox.setText(_message);
  msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  msgBox.setDefaultButton(QMessageBox::Yes);
  int ret = msgBox.exec();
  if (ret == QMessageBox::Yes) 
    return true;
  return false;
}

/// Non blocking box which can be used inside scripting loops to stop on clicks
bool VsiPlugin::continueBox (QString _message)
{
  static QContinueBox* msgBox = NULL;

  if ( !msgBox ) {
    msgBox = new QContinueBox(_message);
    msgBox->show();
  } else {

    if ( msgBox->continueBox() ) {
      return true;
    } else {
      delete msgBox;
      msgBox = NULL;
      return false;
    }
  }

  return true;
}

//------------------------------------------------------------------------------

#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2(Vsiplugin,VsiPlugin)
#endif
