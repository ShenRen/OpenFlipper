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

//== INCLUDES ====================================================


#include "SplatCloudBackup.hh"


//== DEFINES =====================================================


//#define REPORT_BACKUP


//== IMPLEMENTATION ==============================================


SplatCloudBackup::SplatCloudBackup( SplatCloudObject *_object, QString _name, UpdateType _type ) :
  BaseBackup       ( _object, _name, _type ),
  splatCloudObject_( _object ),
  splatCloudBackup_( 0 )
{
# ifdef REPORT_BACKUP
  std::cerr << "Create SplatCloudBackup with name: \"" << name_.toStdString() << "\" (id: " << id_ << ")." << std::endl;
# endif

  const SplatCloud *splatCloud = PluginFunctions::splatCloud( _object );

  if( splatCloud == 0 )
  {
    std::cerr << "Could not create SplatCloudBackup with name: \"" << name_.toStdString() << "\" (id: " << id_ << ") : SplatCloud not found." << std::endl;
    return;
  }

  splatCloudBackup_ = new SplatCloud( *splatCloud );

  if( splatCloudBackup_ == 0 )
  {
    std::cerr << "Could not create SplatCloudBackup with name: \"" << name_.toStdString() << "\" (id: " << id_ << ") : Out of memory." << std::endl;
    return;
  }
}


//----------------------------------------------------------------


SplatCloudBackup::~SplatCloudBackup()
{
# ifdef REPORT_BACKUP
  std::cerr << "Delete SplatCloudBackup with name: \"" << name_.toStdString() << "\" (id: " << id_ << ")." << std::endl;
# endif

  delete splatCloudBackup_;
}


//----------------------------------------------------------------


void SplatCloudBackup::apply()
{
  // first apply the baseBackup
  BaseBackup::apply();

# ifdef REPORT_BACKUP
  std::cerr << "Apply SplatCloudBackup with name: \"" << name_.toStdString() << "\" (id: " << id_ << ")." << std::endl;
# endif

  SplatCloud *splatCloud = PluginFunctions::splatCloud( splatCloudObject_ );

  if( splatCloud == 0 )
  {
    std::cerr << "Could not apply SplatCloudBackup with name: \"" << name_.toStdString() << "\" (id: " << id_ << ") : SplatCloud not found." << std::endl;
    return;
  }

  if( splatCloudBackup_ == 0 )
  {
    std::cerr << "Could not apply SplatCloudBackup with name: \"" << name_.toStdString() << "\" (id: " << id_ << ") : No backup available." << std::endl;
    *splatCloud = SplatCloud(); // restore empty SpaltCloud
    return;
  }

  *splatCloud = *splatCloudBackup_;
}
