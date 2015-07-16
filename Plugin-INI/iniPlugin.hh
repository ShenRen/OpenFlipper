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

#ifndef INIPLUGIN_HH
#define INIPLUGIN_HH

#include <QObject>
#include <QMenuBar>

#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>
#include <OpenFlipper/BasePlugin/ScriptInterface.hh>
#include <OpenFlipper/BasePlugin/INIInterface.hh>

class INIPlugin : public QObject, BaseInterface, LoggingInterface, ScriptInterface, INIInterface
{
    Q_OBJECT
    Q_INTERFACES(LoggingInterface)
    Q_INTERFACES(BaseInterface)
    Q_INTERFACES(ScriptInterface)
    Q_INTERFACES(INIInterface)

#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "org.OpenFlipper.Plugins.Plugin-INI")
#endif

    signals:
        void log(Logtype _type, QString _message);
        void log(QString _message);
        
    private slots:
        void loadIniFile( INIFile& _ini ,int _id);
        void saveIniFile( INIFile& _ini ,int _id);
        
        void noguiSupported( ) {} ;
        
    public :
        
        INIPlugin();
        ~INIPlugin() {};
        
    public slots:
        
        QString name() { return (QString("INIPlugin")); };
        QString description( ) { return (QString(tr("Handle INI-files."))); };
        
        QString version() { return QString("1.0"); };
        
    private:
        // Template function to parse ini file entries
        template <class Object>
        void parseIniFileT(INIFile& _ini, Object* _object);
        
        // Template function to save ini file entries
        template <class Object>
        void saveIniFileT(INIFile& _ini, Object* _object);
        
        
        // Parse data from baseObjectData derived objects
        void parseIniFile(INIFile& _ini, BaseObjectData* _object);
        
        // Save data from baseObjectData derived objects
        void saveIniFile(INIFile& _ini, BaseObjectData* _object);
};

//=============================================================================
#if defined(INCLUDE_TEMPLATES) && !defined(INIPLUGINT_C)
#define INIPLUGINT_TEMPLATES
#include "iniPluginT.cc"
#endif
//=============================================================================

#endif //INIPLUGIN_HH
