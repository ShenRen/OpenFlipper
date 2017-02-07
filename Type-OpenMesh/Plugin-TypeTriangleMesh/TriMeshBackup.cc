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
*   $Revision: 21016 $                                                       *
*   $LastChangedBy: schultz $                                                *
*   $Date: 2015-07-16 16:48:42 +0200 (Do, 16 Jul 2015) $                     *
*                                                                            *
\*===========================================================================*/

#include "TriMeshBackup.hh"

//-----------------------------------------------------------------------------

TriMeshBackup::TriMeshBackup(TriMeshObject* _meshObj, QString _name, UpdateType _type)
  : BaseBackup( _meshObj, _name, _type ),
    meshObj_(_meshObj)
{
//   std::cerr << "Create TriMeshBackup with name:" << name_.toStdString() << "(id : " << id_ << ")" << std::endl;
  meshBackup_ = new TriMesh( *(PluginFunctions::triMesh(meshObj_)) );
}

//-----------------------------------------------------------------------------

TriMeshBackup::~TriMeshBackup(){
//   std::cerr << "Delete TriMeshBackup with name:" << name_.toStdString() << "(id : " << id_ << ")" << std::endl;
  delete meshBackup_;
}

//-----------------------------------------------------------------------------

void TriMeshBackup::apply(){

  //first apply the baseBackup
  BaseBackup::apply();

//   std::cerr << "Apply TriMeshBackup with name:" << name_.toStdString() << "(id : " << id_ << ")" << std::endl;
  *(PluginFunctions::triMesh(meshObj_)) = *meshBackup_;
}

//-----------------------------------------------------------------------------
