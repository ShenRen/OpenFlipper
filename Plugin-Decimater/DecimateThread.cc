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

#include "DecimateThread.hh"


void DecimateThread::notify(size_t _step)
{
  emit state(jobId(),(100*_step)/maxCollapses_);
}

bool DecimateThread::abort() const
{
  return abort_;
}

DecimateThread::DecimateThread(Params _param, QString _jobId, int _objId):
        OpenFlipperThread(_jobId), Observer(1),
        abort_(false), maxCollapses_(0),
        param_(_param), baseDec_(0), objId_(_objId)
{
  connect( this ,SIGNAL(function()), this,SLOT(decimate()), Qt::DirectConnection) ;
  if (param_.dec)
    baseDec_ = param_.dec;
  else if (param_.mcDec)
    baseDec_ = param_.mcDec;
  else if (param_.mixedDec)
    baseDec_ = param_.mixedDec;

  baseDec_->set_observer(this);
  if (param_.verticesCount != -1)
    maxCollapses_ = baseDec_->mesh().n_vertices() - param_.verticesCount;
  else if (param_.facesCount != -1)
    maxCollapses_ = baseDec_->mesh().n_faces() - param_.facesCount;
  else
    maxCollapses_ = 0.8*baseDec_->mesh().n_vertices(); //0.8 just magic number since we cannot say, when it stops
}

DecimateThread::~DecimateThread()
{
}

void DecimateThread::cancel()
{
  abort_ = true;
}

DecimateThread::BaseDecimaterType* DecimateThread::baseDecimater()
{
  return baseDec_;
}

int DecimateThread::objectId() const
{
  return objId_;
}

void DecimateThread::decimate()
{

  if(param_.dec )
  {
    if ( param_.verticesCount != -1 )
      param_.dec->decimate_to(param_.verticesCount);
    else if (param_.facesCount != -1)
      param_.dec->decimate_to_faces(0, param_.facesCount);
    else // constraints only
      param_.dec->decimate_to_faces(0, 1);
  }
  else if (param_.mcDec)
  {
    param_.mcDec->set_samples(param_.samples);
    if ( param_.verticesCount != -1 )
      param_.mcDec->decimate_to(param_.verticesCount);
    else if (param_.facesCount != -1 )
      param_.mcDec->decimate_to_faces(0, param_.facesCount);
    else // constraints only
      param_.mcDec->decimate_to_faces(0, 1);
  }
  else if (param_.mixedDec)
  {
    param_.mixedDec->set_samples(param_.samples);
    if ( param_.verticesCount != -1 )
      param_.mixedDec->decimate_to(param_.verticesCount,param_.mc_factor);
    else if (param_.facesCount != -1 )
      param_.mixedDec->decimate_to_faces(0, param_.facesCount,param_.mc_factor);
    else // constraints only
      param_.mixedDec->decimate_to_faces(0, 1,param_.mc_factor);
  }
  emit state(jobId(),100);
}

