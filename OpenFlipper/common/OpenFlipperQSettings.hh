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
*   $Revision: 10413 $                                                       *
*   $LastChangedBy: moebius $                                                *
*   $Date: 2010-12-09 15:40:04 +0100 (Thu, 09 Dec 2010) $                     *
*                                                                            *
\*===========================================================================*/

#ifndef OPENLFLIPPERQSETTINGS_HH
#define OPENLFLIPPERQSETTINGS_HH

#include <QSettings>
#include <OpenFlipper/common/GlobalDefines.hh>

class DLLEXPORT OpenFlipperQSettings : public QSettings
{
public:
    explicit OpenFlipperQSettings(const QString &organization,
                                  const QString &application = QString(),
                                  QObject *parent = 0) : QSettings(organization, application, parent) {};

    OpenFlipperQSettings(QSettings::Scope scope,
                         const QString &organization,
                         const QString &application = QString(),
                         QObject *parent = 0) : QSettings(scope, organization, application, parent) {};

    OpenFlipperQSettings(QSettings::Format format,
                         QSettings::Scope scope, const QString &organization,
	                       const QString &application = QString(),
	                       QObject *parent = 0) : QSettings (format, scope, organization, application, parent) {};

    OpenFlipperQSettings(const QString &fileName,
                         QSettings::Format format,
                         QObject *parent = 0) : QSettings(fileName, format, parent) {};

    explicit OpenFlipperQSettings(QObject *parent = 0) : QSettings(parent) {};
    
    /// Wrapper function for QSettings::value() that outputs an error message to console if the key was not found and no default value given.
    /// This is supposed to prevent Plugin authors from forgetting to specify the defaultValue. ;)
    /// Additionally it is possible to enable Debugging output with -DOPENFLIPPER_SETTINGS_DEBUG
    QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;

    /// Wrapper function which makes it possible to enable Debugging output with -DOPENFLIPPER_SETTINGS_DEBUG
    void setValue ( const QString & key, const QVariant & value );

};

#endif // OPENFLIPPERQSETTINGS_HH
