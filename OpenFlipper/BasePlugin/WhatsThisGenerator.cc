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

#include "WhatsThisGenerator.hh"

/** Constructor for the whatsThisGenerator initializing the plugin name.
 *
 * @param _plugin_name the name of the plugin which is used \note use only the name and NOT the prefix Plugin- (e.g. SkeletonEditing for Plugin-SkeletonEditing).
 *  If you want to reference in the core user plugin, then _plugin = "user"
 */
WhatsThisGenerator::WhatsThisGenerator(const QString &_plugin_name) :
plugin_name_((_plugin_name != "user") ? QString("Plugin-")+_plugin_name : _plugin_name)
{
}


/**	\brief generates a clickable link to the documentation for whatsThis Messages
 * @param _ref gives the section in the documentation for the action. The help-browser will jump into the section.
 *                      An example: _jump = "Cube" in PrimitiveGenerator refs to the section "Cube" in the Documentation and the help Browser.
 *                      \note use the doxygen reference name (always one word), not the section name
 *
 * @param _site gives the documentation site (html-site) which you want to open.
 *                      Default is the site "index.html".
 *                      You need this parameter if you have more than one site in your plugin documentation and want to refer on one of this.
 */
QString WhatsThisGenerator::generateLink(const QString &_ref /*= ""*/, const QString &_site /*= "index.html"*/) const
{
  const QString baseHelpURL = QString(" <a href='qthelp://org.openflipper.")
                                         + plugin_name_.toLower()
                                         + QString("/")
                                         + plugin_name_+QString("/")
                                         + _site+QString("#")+_ref + QString("'>");

  const QString clickText = QString(QObject::tr("Click for more information"))+QObject::tr("</a>");
  return (baseHelpURL + clickText);
}

/**	\brief sets a whatsThis Message plus link to the doc for the given QAction
 * @param _action action where the message should appear.
 * @param _msg help message for the user. This is the default Whatsthis Text which you set with QAction::setWhatsThis
 *
 * @param _ref gives the section in the documentation for the action. The help-browser will jump into the section.
 *                      An example: _jump = "Cube" in PrimitiveGenerator refs to the section "Cube" in the Documentation and the help Browser.
 *                      \note use the doxygen reference name (always one word), not the section name
 *
 * @param _site gives the documentation site (html-site) which you want to open.
 * 	                    Default is the site "index.html".
 * 	                    You need this parameter if you have more than one site in your plugin documentation and want to refer on one of this.
 */
void WhatsThisGenerator::setWhatsThis(QAction* _action, const QString &_msg, const QString &_ref /*= ""*/,const QString &_site /*= "index.html"*/) const
{
  _action->setWhatsThis(_msg + generateLink(_ref, _site));
}
