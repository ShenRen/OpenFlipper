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

#ifndef TESTPLUGIN_HH
#define TESTPLUGIN_HH

#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>
#include <OpenFlipper/BasePlugin/ToolboxInterface.hh>
#include <OpenFlipper/common/Types.hh>

class TestPlugin: public QObject, BaseInterface, LoggingInterface, ToolboxInterface {
  Q_OBJECT
  Q_INTERFACES(BaseInterface)
  Q_INTERFACES(LoggingInterface)
  Q_INTERFACES(ToolboxInterface)

#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "org.OpenFlipper.Plugins.Plugin-Test")
#endif

  signals:
  //BaseInterface
  void updateView();
  void updatedObject(int _id, const UpdateType& _type);

  //LoggingInterface
  void log(Logtype _type, QString _message);
  void log(QString _message);

  //ToolboxInterface
  void addToolbox(QString  _name, QWidget* _widget);

public:

  // BaseInterface
  QString name() { return (QString("Test Plugin"));  } ;
  QString description() {  return (QString("Plugin used for testing OpenFlipper functions")); } ;

private slots:

  /// BaseInterface
  void initializePlugin();
  void pluginsInitialized();

  /// Tell system that this plugin runs without ui
  void noguiSupported( ) {} ;


public slots:

  QString version() { return QString("1.0"); };

  void btnclick();

};

#endif //TESTPLUGIN_HH
