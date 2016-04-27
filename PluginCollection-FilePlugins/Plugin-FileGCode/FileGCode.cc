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

#include "FileGCode.hh"

#include <OpenFlipper/BasePlugin/PluginFunctions.hh>
#include <OpenFlipper/common/GlobalOptions.hh>
#include <fstream>

#if QT_VERSION >= 0x050000
  #include <QtWidgets>
#else
  #include <QtGui>
#endif


void FileGCodePlugin::initializePlugin() {
}

QString FileGCodePlugin::getLoadFilters() {
  return QString( tr("GCode files ( *.gcode )") );
};

QString FileGCodePlugin::getSaveFilters() {
  return QString( tr("GCode files ( *.gcode )") );
};

DataType  FileGCodePlugin::supportedType() {
  DataType type = DATA_GCODE;
  return type;
}

int FileGCodePlugin::loadObject(QString _filename)
{
  int id = -1;

  QFileInfo fi(_filename);

  if (fi.isReadable())
  {
    emit addEmptyObject(DATA_GCODE, id);

    GCodeObject* gcode = 0;
    if (PluginFunctions::getObject(id, gcode))
    {
      if (gcode)
      {
        gcode->gcode()->parse_from_file(_filename.toStdString());
        gcode->update();

        emit updatedObject(gcode->id(), UPDATE_ALL);
      }
    }
  }

  return id;
};


bool FileGCodePlugin::saveObject(int _id, QString _filename)
{
  BaseObjectData*     obj(0);
  if(PluginFunctions::getObject( _id, obj))
  {
    GCodeObject* gcode = PluginFunctions::gcodeObject(obj);
    if (gcode)
    {
      std::ofstream file(_filename.toStdString());

      if (file.is_open())
      {
        gcode->gcode()->write(file);

        file.close();
      }

    }
  } else {
    emit log(LOGERR, tr("saveObject : cannot get object id %1 for save name %2").arg(_id).arg(_filename) );
    return false;
  }

  return true;
}
#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2( filegcodeplugin , FileGCodePlugin );
#endif


