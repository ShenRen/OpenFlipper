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


#include "TypeSphere.hh"

#include "OpenFlipper/BasePlugin/PluginFunctions.hh"

#include <OpenFlipper/common/GlobalOptions.hh>

TypeSpherePlugin::TypeSpherePlugin() {
  
}

bool TypeSpherePlugin::registerType() {
  addDataType("Sphere",tr("Sphere"));
//   setTypeIcon( "Sphere", "SphereType.png");
  return true;
  
}

int TypeSpherePlugin::addEmpty(){
    
  // new object data struct
  SphereObject * object = new SphereObject();

  if ( OpenFlipperSettings().value("Core/File/AllTarget",false).toBool() )
    object->target(true);
  else {

    // Only the first object in the scene will be target
    if ( PluginFunctions::objectCount() == 1 )
       object->target(true);

    // If no target is available, we set the new object as target
    if (PluginFunctions::targetCount() == 0 )
       object->target(true);
  }

  QString name = QString(tr("New Sphere %1.sph").arg( object->id() ));

  // call the local function to update names
  QFileInfo f(name);
  object->setName( f.fileName() );

  // set the default colors
  const QColor color = OpenFlipper::Options::defaultColor();
  const ACG::Vec4f default_color(color.redF(), color.greenF(), color.blueF(), color.alphaF());
  object->materialNode()->set_color(default_color);

  object->update();

  object->show();

  emit emptyObjectAdded (object->id() );

  return object->id();
}

#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2( typesphereplugin , TypeSpherePlugin );
#endif

