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

// Include the scripttools for qt script debugging if available
#ifdef QT_SCRIPTTOOLS_LIB
  #include <QtScriptTools/QScriptEngineDebugger>
#endif

#include "highLighter.hh"
#include "scriptingWidget.hh"



class ScriptingPlugin : public QObject, BaseInterface, MenuInterface, ScriptInterface, RPCInterface, LoggingInterface
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

signals:
  void updateView();

  void getScriptingEngine( QScriptEngine*& _engine  );
  void getAvailableFunctions( QStringList& _functions  );

  void log(Logtype _type, QString _message);
  void log(QString _message);

  void getMenubarMenu (QString _name, QMenu *& _menu, bool _create);

  void getDescription(QString      _function,   QString&     _description,
                      QStringList& _parameters, QStringList& _descriptions);

private slots:

  void slotScriptInfo( QString _pluginName , QString _functionName  );
  void slotExecuteScript( QString _script );
  void slotExecuteFileScript( QString _filename );

  void slotLoadScript();
  void slotSaveScript();
  void slotSaveScriptAs();
  void pluginsInitialized();

  void noguiSupported( ) {} ;

public :

  ScriptingPlugin();

  QString name() { return (QString(tr("Scripting"))); };
  QString description( ) { return (QString(tr("Provides Scripting for OpenFlipper"))); };

private slots :

  void slotExecuteScriptButton();

  /** \brief Called everytime the text in the scriptingwidget is changed by the user
   *
   * This slot also handles syntax checking!
   */
  void slotScriptChanged();

  void slotFunctionClicked(QListWidgetItem* _item);
  void slotFunctionDoubleClicked(QListWidgetItem* _item);

  void slotApplyFilter();

  /// Called when an error is detected when checking the syntax
  void slotHighlightError();

  /// Triggered by the debugger button
  void slotDebuggerButton();

private:

  /// Store the last line that contained an error
  int lastProblemLine_;

  /// Store the last error message
  QString lastError_;

  /// Timer to wait until the user stopped typing before showing an error
  QTimer* errorTimer_;

  QList< QLabel* > descriptionLabels_;
  QVBoxLayout* descrLayout_;

//===========================================================================
/** @name Gui control from script
* @{ */
//===========================================================================

public slots:

  /** \brief Show the script editor widget
   *
   */
  void showScriptWidget();

  /** \brief Hide the script editor widget
   *
   */
  void hideScriptWidget();

  /** \brief Show the given Code in the script editor
   *
   * @param _code Code to show
   */
  void showScriptInEditor (QString _code);

  /** \brief Load Script into gui from file
  *
  * Loads a script from the given file and loads it into the widget.
  *
  * @param _filename Filename of script to load, including full path.
  */
  void slotLoadScript( QString _filename );
  
  /** \brief Clear the editor window
   * Clears the script editor window
   */
  void clearEditor();

/** @} */

//===========================================================================
/** @name Interrupt script execution
* @{ */
//===========================================================================

public slots:

  /** Stops script execution and opens a message windows.
  *  Execution continues, when the user clicks on a button
  */
  void waitContinue( );

  /** Stops script execution and opens a message windows.
  *  Execution continues, when the user clicks on a button
  *  Also allowing the user to set a message
  */
  void waitContinue( QString _msg, int _x=-1, int _y=-1);


  /// Sleeps for some seconds in script execution ( Gui will remain functional)
  void sleep( int _seconds );

  /// Sleeps for some mseconds in script execution ( Gui will remain functional)
  void sleepmsecs( int _mseconds );

  /// Marks the current time as the frame start ( Use wait sleepFrameLength to wait until _mseconds have passed since frameStart )
  void frameStart( );

  /// wait until _mseconds have passed since frameStart (if more time has passed, it will return immediately)
  void waitFrameEnd( int _mseconds );

/** @} */


private :
  QString mangleScript( QString _input );

  /// Counts mseconds since a frame start has occured
  QTime frameTime_;

private :
  ScriptWidget* scriptWidget_;

  QStatusBar*   statusBar_;

  Highlighter*  highlighterCurrent_;
  Highlighter*  highlighterLive_;
  Highlighter*  highlighterList_;

  QString       lastFile_;

  /** Path to the directory where the current script is executed.
   *  This might be empty, if a script is not saved!
   */
  QString       scriptPath_;

  QAction*      debuggerButton_;

#ifdef ENABLE_SCRIPT_DEBUGGER
 #ifdef QT_SCRIPTTOOLS_LIB
  QScriptEngineDebugger* debugger_;
 #endif
#endif



public slots:
  QString version() { return QString("1.0"); };
};

#endif //SCRIPTINGPLUGIN_HH

