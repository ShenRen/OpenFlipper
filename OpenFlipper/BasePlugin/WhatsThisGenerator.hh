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

#ifndef WHATSTHISGENERATOR_HH_
#define WHATSTHISGENERATOR_HH_

#include <QAction>
#include <QString>
#include <QObject>
#include <OpenFlipper/common/GlobalDefines.hh>

/** \brief a class which provides an link generator for WhatsThisMessages linking to the user doc
 *	If you have an QAction pointer called action, you can call:
 *	 \code{.cpp}action->setWhatsThis("Hello");\endcode
 *	This message "Hello" will be shown when somebody call this action in WhatsThisMode.
 *
 *	WhatsThisGenerator provides functions linking to the user documentation.
 *		e.g. assume that whatsThisGen is a valid WhatsThisGenerator object
 *		\code{.cpp}action->setWhatsThis(QString(tr("Hello"))+whatsThisGen.generateLink()); \endcode
 *	The code above will change the output for WhatsThisMessages to: "Hello Click here for more information"
 *	where "Click here for more information" is a link which will call the index.html (default parameter) of the plugin
 *	which you have to specify in the constructor of WhatsThisGenerator.
 *
 *	You can also use the WhatsThisGenerator to set the WhatsThisMessage:
 *		\code{.cpp}whatsThisGen.setWHatsThis(action,tr("Hello"));\endcode
 *	It has the same effect and options like the code above and is shorter.
 */
class DLLEXPORT WhatsThisGenerator: public QObject {

  Q_OBJECT

  const QString plugin_name_;

public:
  WhatsThisGenerator(const QString &_plugin_name);

  QString generateLink(const QString &_ref = "", const QString &_site = "index.html") const;

  void setWhatsThis(
      QAction* _action,
      const QString &_msg,
      const QString &_ref = "",
      const QString &_site = "index.html") const;

};


#endif /* WHATSTHISGENERATOR_HH_ */
