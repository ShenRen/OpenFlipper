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
// -------------------- ACG

#include <OpenFlipper/common/RecentFiles.hh>

//== IMPLEMENTATION ==========================================================

/// \todo After obj writing to one file is implemented in obj plugin, remove this Function and the whole File!
void Core::writeObjFile(QString _filename, bool _relativePaths, bool _targetOnly, std::map<int,QString>& _fileMapping)
{
  // open file
  std::string fname = _filename.toStdString();
  std::fstream out(fname.c_str(), std::ios_base::out);
  if (!out)
  {
    emit log(LOGERR,tr("Cannot open to obj file") + _filename);
    return;
  }


  if ( OpenFlipper::Options::gui() )
  {
    coreWidget_->statusMessage( tr("Saving obj File ") + _filename + " ...");
    coreWidget_->setStatus(ApplicationStatus::BLOCKED );
  }

  PluginFunctions::IteratorRestriction restriction;
  if ( _targetOnly )
    restriction = PluginFunctions::TARGET_OBJECTS;
  else
    restriction = PluginFunctions::ALL_OBJECTS;

  // write all objects to a separate obj file and save external references in the global obj file
  for ( PluginFunctions::ObjectIterator o_it (restriction) ;
                                        o_it != PluginFunctions::objectsEnd(); ++o_it)
  {
    QString file;
    std::map<int,QString>::iterator f = _fileMapping.find(o_it->id());
    if(f == _fileMapping.end()) {
        file = o_it->path() + OpenFlipper::Options::dirSeparator() + o_it->name();
    } else {
        file = f->second;
    }

    if (QFile(file).exists())
    {
      // Modify filename if relativePaths are wanted
      if (_relativePaths)
      {
        int prefixLen = _filename.section(OpenFlipper::Options::dirSeparator(),0,-2).length();
        file.remove(0, prefixLen);
        file = "." + file;
      }

      // save to global inifile
      out << "call " << file.toStdString() << "\n";
    }
  }

  out.close();

  if ( OpenFlipper::Options::gui() )
  {
    coreWidget_->statusMessage( tr("Saving obj File ") + _filename + tr(" ... done"), 4000);
    coreWidget_->setStatus(ApplicationStatus::READY );
  }

}


//=============================================================================
