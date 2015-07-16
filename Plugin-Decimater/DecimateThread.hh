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


#ifndef DECIMATETHREAD_HH
#define DECIMATETHREAD_HH


#include <OpenMesh/Tools/Decimater/DecimaterT.hh>
#include <OpenMesh/Tools/Decimater/McDecimaterT.hh>
#include <OpenMesh/Tools/Decimater/MixedDecimaterT.hh>
#include <ObjectTypes/TriangleMesh/TriangleMesh.hh>

#include <QObject>
#include <OpenFlipper/threads/OpenFlipperThread.hh>

class DecimateThread : public OpenFlipperThread, public OpenMesh::Decimater::Observer
{
  Q_OBJECT
public:
  typedef OpenMesh::Decimater::BaseDecimaterT< TriMesh >              BaseDecimaterType;
  typedef OpenMesh::Decimater::DecimaterT< TriMesh >                  DecimaterType;
  typedef OpenMesh::Decimater::McDecimaterT< TriMesh >                McDecimaterType;
  typedef OpenMesh::Decimater::MixedDecimaterT< TriMesh >             MixedDecimaterType;

  struct Params
  {
    DecimaterType* dec;
    McDecimaterType* mcDec;
    MixedDecimaterType* mixedDec;
    bool useIncremental;
    bool useMC;
    bool useMixed;
    float mc_factor;
    int verticesCount; // -1 if not decimate to vertices
    int facesCount; // -1 if not decimate to faces
    int samples;
  };

private:

  bool abort_;
  int maxCollapses_;
  Params param_;
  QString jobId_;
  BaseDecimaterType* baseDec_;
  int objId_;

public:
  virtual void notify(size_t _step);
  virtual bool abort() const;

public slots:
  void decimate();

public:
  void cancel();
  DecimateThread(Params _param, QString _jobId, int _objId);
  ~DecimateThread();

  BaseDecimaterType* baseDecimater();

  int objectId() const;

};

#endif // DECIMATETHREAD_HH
