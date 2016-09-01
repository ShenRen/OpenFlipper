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
#include <QUiLoader>


//== IMPLEMENTATION ==========================================================



void Core::slotScriptInfo( QString _pluginName , QString _functionName  ) {
  emit scriptInfo( _pluginName , _functionName );
}

void Core::slotExecuteScript( QString _script ) {
  emit executeScript( _script );
}

void Core::slotExecuteFileScript( QString _filename ) {
  emit executeFileScript( _filename );
}

void Core::slotGetScriptingEngine( QScriptEngine*& _engine  ) {
  _engine = &scriptEngine_;
}

void Core::slotGetAllAvailableFunctions( QStringList& _functions  ) {
  _functions = scriptingFunctions_;
}

void Core::scriptLogFunction( QString _output) {
   emit scriptLog(_output);
}

void Core::createWidget(QString _objectName, QString _uiFilename, bool _show) {
  if ( OpenFlipper::Options::gui()) {
    QUiLoader loader;

    QFile uiFile(_uiFilename);

    if ( !uiFile.exists() ) {
      emit log(LOGERR,tr("File does not exist : ") + _uiFilename );
      return;
    }

    uiFile.open(QIODevice::ReadOnly);
    QWidget *ui = loader.load(&uiFile, coreWidget_);
    uiFile.close();

    if ( ui == 0 ) {
      emit log(LOGERR,tr("Unable to create QWidget from ui file for ") + _objectName );
      return;
    }

    QScriptValue scriptUi = scriptEngine_.newQObject(ui, QScriptEngine::ScriptOwnership);

    if ( !scriptUi.isValid() ) {
      emit log(LOGERR,tr("Unable to generate script interface for ") + _objectName );
      return;
    }

    scriptEngine_.globalObject().setProperty(_objectName, scriptUi);


    if(_show) ui->show();
  } else {
    emit log(LOGERR,tr("Error! Script tried to create Widget in ui less batc mode! Creation Aborted!"));
  }

}

//-----------------------------------------------------------------------------

int Core::getObjectId( const QString _name ) {

	return PluginFunctions::getObjectId(_name);
}

//-----------------------------------------------------------------------------

void Core::setViewMode(QString _viewMode){

  if ( OpenFlipper::Options::gui() )
    coreWidget_->setViewMode( _viewMode );
}

//-----------------------------------------------------------------------------

QString Core::getCurrentViewMode() {
  return OpenFlipper::Options::currentViewMode();
}

//-----------------------------------------------------------------------------

void Core::setViewModeIcon(QString _mode, QString _iconName){

  if ( OpenFlipper::Options::gui() ){

    QFile file(_iconName);
    QFileInfo fileInfo(file);
  
    if ( ! file.exists() ){
      emit log(LOGERR, tr("Icon not found (%1)").arg(_iconName) );
      return;
    }

    file.copy(OpenFlipper::Options::configDirStr() + QDir::separator() + "Icons" + QDir::separator() + "viewMode_" + fileInfo.fileName() );

    coreWidget_->slotSetViewModeIcon( _mode, "viewMode_" + fileInfo.fileName() );
  }
}

//-----------------------------------------------------------------------------

void Core::moveToolBoxToTop(QString _name) {
    
  if(OpenFlipper::Options::gui()) {
    coreWidget_->moveToolBoxToTop(_name);
  }
}

//-----------------------------------------------------------------------------

void Core::moveToolBoxToBottom(QString _name) {
    
  if(OpenFlipper::Options::gui()) {
    coreWidget_->moveToolBoxToBottom(_name);
  }
}

//-----------------------------------------------------------------------------

void Core::addViewModeToolboxes(QString _modeName, QString _toolboxList) {

  QStringList list = _toolboxList.split(";");
  coreWidget_->slotAddViewModeToolboxes(_modeName,list);
}

//-----------------------------------------------------------------------------

void Core::addViewModeToolbars(QString _modeName, QString _toolbarList) {
  
  QStringList list = _toolbarList.split(";");
  coreWidget_->slotAddViewModeToolbars(_modeName,list);
}

//-----------------------------------------------------------------------------

void Core::addViewModeContextMenus(QString _modeName, QString _contextMenuList) {
  
  QStringList list = _contextMenuList.split(";");
  coreWidget_->slotAddViewModeContextMenus(_modeName,list);
}

void Core::addViewModeIcon(QString _modeName, QString _iconName) {
  coreWidget_->slotSetViewModeIcon(_modeName,true,_iconName);
}

//-----------------------------------------------------------------------------

void Core::setToolBoxSide(QString _side) {
  
    if(_side.toLower() == "left") {
        coreWidget_->setToolBoxOrientationOnTheRight(false);
    } else if(_side.toLower() == "right") {
        coreWidget_->setToolBoxOrientationOnTheRight(true);
    } else {
        emit log(LOGERR, QString("Could not display toolboxes on side '%1'. Use either 'left' or 'right' as string!").arg(_side));
    }
}

//-----------------------------------------------------------------------------

QWidget *Core::getToolbox(QString _pluginName, QString _toolboxName) {
    std::vector<PluginInfo>::const_iterator pluginIt = plugins_.end();
    for (std::vector<PluginInfo>::const_iterator it = plugins_.begin(), it_end = plugins_.end(); it != it_end; ++it) {
        if (it->name == _pluginName) {
            pluginIt = it;
        }
    }
    if (pluginIt == plugins_.end()) return 0;

    for (std::vector<std::pair<QString , QWidget*> >::const_iterator it = pluginIt->toolboxWidgets.begin(), it_end = pluginIt->toolboxWidgets.end();
            it != it_end; ++it) {
        if (it->first == _toolboxName)
            return it->second;
    }

    return 0;
}

void Core::activateToolbox(QString _pluginName, QString _toolboxName, bool activate) {
    QWidget *toolbox = Core::getToolbox(_pluginName, _toolboxName);
    coreWidget_->expandToolBoxWidget(toolbox, activate);
}

void Core::addToolbox(QString _name ,QWidget* _widget) {
    addToolbox(_name, _widget, 0, 0);
}

void Core::addToolbox(QString _name ,QWidget* _widget, QIcon* _icon) {
    addToolbox(_name, _widget, _icon, 0);
}

void Core::addToolbox(QString _name ,QWidget* _widget, QIcon* _icon,
        QWidget *_headerAreaWidget) {
  int id = -1;

  // Find the plugin which added this Toolbox
  for ( uint i = 0 ; i < plugins_.size(); ++i ) {
    if ( plugins_[i].plugin == sender() ) {
      id = i;
      break;
    }
  }

  // Find the scripting plugin because we assign this toolBox to it as we did not find the original sender
  if ( id == -1 ) {
    for ( uint i = 0 ; i < plugins_.size(); ++i ) {
      if ( plugins_[i].name == "Scripting" ) {
        id = i;
        break;
      }
    }


    if ( id == -1 ) {
      std::cerr << "Unknown sender plugin when adding Toolbox!" << std::endl;
      return;
    }
  }

  spinBoxEventFilter_.hookUpToWidgetTree(_widget);
  plugins_[id].toolboxWidgets.push_back( std::pair< QString,QWidget* >( _name , _widget) );
  plugins_[id].toolboxIcons.push_back( _icon );
  plugins_[id].headerAreaWidgets.push_back( std::pair< QString,QWidget* >( _name , _headerAreaWidget) );

  // add widget name to viewMode 'all'
  if ( !viewModes_[0]->visibleToolboxes.contains(_name) ){
    viewModes_[0]->visibleToolboxes << _name;
    viewModes_[0]->visibleToolboxes.sort();
  }

  setViewMode( OpenFlipper::Options::currentViewMode() );
}

void Core::setToolBoxActive(QString _toolBoxName, bool _active)
{
  if ( OpenFlipper::Options::gui() ){
    coreWidget_->toolBox_->setElementActive(_toolBoxName,_active);
  }
}

/// Block the scenegraph updates
void Core::blockSceneGraphUpdates() {
  OpenFlipper::Options::blockSceneGraphUpdates();
}

/// Unblock the scenegraph updates
void Core::unblockSceneGraphUpdates() {
  OpenFlipper::Options::unblockSceneGraphUpdates();
}

void Core::setView(QString view) {
    coreWidget_->slotSetView(view);
}

void Core::setViewAndWindowGeometry(QString view) {
    coreWidget_->slotSetViewAndWindowGeometry(view);
}

//=============================================================================
//== Script Special Functions =================================================
//=============================================================================

void Core::slotScriptError(const QScriptValue &error) {
    emit log(LOGERR, tr("Script error: ") + error.toString());
}

QScriptValue myPrintFunction(QScriptContext *context, QScriptEngine *engine)
{
  QString result;
  for (int i = 0; i < context->argumentCount(); ++i) {
    if (i > 0)
        result.append(" ");
    result.append(context->argument(i).toString());
  }

  // Get the textedit for Output ( Set in Core.cc )
  QScriptValue calleeData = context->callee().property("textedit");
  Core *widget = qobject_cast<Core*>(calleeData.toQObject());

  widget->scriptLogFunction(result);

  return engine->undefinedValue();
}

QScriptValue printToFileFunction(QScriptContext *context, QScriptEngine *engine)
{
  if ( context->argumentCount() < 2 ) {
    context->throwError( QScriptContext::SyntaxError, "Error! printToFileFunction needs at least two arguments, filename and what should be printed" );
    return  engine->undefinedValue();
  }

  QString result;
  for (int i = 1; i < context->argumentCount(); ++i) {
    if (i > 1)
        result.append(" ");
    result.append(context->argument(i).toString());
  }

  QFile file(context->argument(0).toString());

  file.open(QIODevice::Append);
  QTextStream stream(&file);

  stream << result << "\n";

  file.close();

  return engine->undefinedValue();
}

QScriptValue helpFunction(QScriptContext *context, QScriptEngine *engine)
{
  if ( context->argumentCount() != 1 ) {
    context->throwError( QScriptContext::SyntaxError, "Error! helpFunction needs one argument" );
    return  engine->undefinedValue();
  }

  QString helpString = context->argument(0).toString();

  // Get the corewidget poiter ( Set in Core.cc )
  QScriptValue calleeData = context->callee().property("core");
  Core *core = qobject_cast<Core*>(calleeData.toQObject());

  const std::vector<PluginInfo> plugins = core->plugins();

  for (unsigned int i=0; i < plugins.size(); i++) {
    if (plugins[i].rpcName == helpString) {
      core->scriptLogFunction( "=======================================================\n" );
      core->scriptLogFunction( "Found Plugin \"" + plugins[i].name +  "\" \n" );
      core->scriptLogFunction( "Description: " + plugins[i].description + " \n");
      core->scriptLogFunction( "=======================================================\n" );
      core->scriptLogFunction( "Scripting functions: \n");

      for ( int j = 0 ; j < plugins[i].rpcFunctions.size() ; ++j ) {
        core->scriptLogFunction( plugins[i].rpcFunctions[j]+"\n");
      }

      core->scriptLogFunction( "\n\n");
    }
  }



  return engine->undefinedValue();
}

