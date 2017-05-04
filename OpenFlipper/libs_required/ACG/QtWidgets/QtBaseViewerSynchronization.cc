/*===========================================================================*\
 *                                                                           *
 *                              OpenFlipper                                  *
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
 *                                                                           *
\*===========================================================================*/

/*===========================================================================*\
 *                                                                           *
 *   $Revision$                                                       *
 *   $Author$                                                      *
 *   $Date$                   *
 *                                                                           *
\*===========================================================================*/



//=============================================================================
//
//  CLASS QtBaseViewer - IMPLEMENTATION
//
//=============================================================================


//== INCLUDES =================================================================

#include "QtBaseViewer.hh"
#include "QtGLGraphicsScene.hh"
#include "QtGLGraphicsView.hh"
#include <QHostInfo>

//== NAMESPACES ===============================================================

namespace ACG {
namespace QtWidgets {


//== IMPLEMENTATION ==========================================================


void QtBaseViewer::sync_connect(const QtBaseViewer* _other)
{
  connect(_other,
	  SIGNAL(signalSetView(const GLMatrixd&, const GLMatrixd&)),
	  this,
	  SLOT(setView(const GLMatrixd&, const GLMatrixd&)));
}


//-----------------------------------------------------------------------------


void QtBaseViewer::sync_disconnect(const QtBaseViewer* _other)
{
  disconnect(_other,
	     SIGNAL(signalSetView(const GLMatrixd&, const GLMatrixd&)),
	     this,
	     SLOT(setView(const GLMatrixd&, const GLMatrixd&)));
}


//-----------------------------------------------------------------------------


void QtBaseViewer::sync_send(const GLMatrixd&, const GLMatrixd&)
{
  const GLMatrixd& m = glstate_->modelview();
  const GLMatrixd& p = glstate_->projection();

  QByteArray   datagram;
  QDataStream  out(&datagram, QIODevice::WriteOnly);
  out.setVersion(5);

  quint64 myId = (quint64) winId();

  out << myId
      << m(0,0) << m(0,1) << m(0,2) << m(0,3)
      << m(1,0) << m(1,1) << m(1,2) << m(1,3)
      << m(2,0) << m(2,1) << m(2,2) << m(2,3)
      << m(3,0) << m(3,1) << m(3,2) << m(3,3)
      << p(0,0) << p(0,1) << p(0,2) << p(0,3)
      << p(1,0) << p(1,1) << p(1,2) << p(1,3)
      << p(2,0) << p(2,1) << p(2,2) << p(2,3)
      << p(3,0) << p(3,1) << p(3,2) << p(3,3)
      << glWidth() << glHeight()
      << projectionMode_
      << orthoWidth_;

  for (int h=0, n=sync_hosts_.size(); h<n; ++h)
    for (int i=6666; i<6676; ++i)
      if (i != socket_->localPort() || h != 0)
	     socket_->writeDatagram( datagram, datagram.size(),sync_hosts_[h], i);
}


void QtBaseViewer::sync_receive()
{
GLMatrixd     m, p;
  int           w, h, pMode;
  quint64       id, myId = (quint64) winId();


  QByteArray  datagram( socket_->pendingDatagramSize(), 'x' );
  socket_->readDatagram( datagram.data(), datagram.size() );


  if (datagram.size() < 280)
     return;

  QDataStream  in( & datagram, QIODevice::ReadOnly);
  in.setVersion(5);


  in >> id;
  if (id == myId)
    return;

  in >> m(0,0) >> m(0,1) >> m(0,2) >> m(0,3)
     >> m(1,0) >> m(1,1) >> m(1,2) >> m(1,3)
     >> m(2,0) >> m(2,1) >> m(2,2) >> m(2,3)
     >> m(3,0) >> m(3,1) >> m(3,2) >> m(3,3)
     >> p(0,0) >> p(0,1) >> p(0,2) >> p(0,3)
     >> p(1,0) >> p(1,1) >> p(1,2) >> p(1,3)
     >> p(2,0) >> p(2,1) >> p(2,2) >> p(2,3)
     >> p(3,0) >> p(3,1) >> p(3,2) >> p(3,3)
     >> w >> h >> pMode >> orthoWidth_;


  blockSignals(true);

  makeCurrent();

  if (projectionMode_ != (ProjectionMode)pMode)
    projectionMode((ProjectionMode)pMode);

  glstate_->set_modelview(m);

  if (w>0 && h>0 &&
      action_["PasteDropSize"]->isChecked() )
  {
    glstate_->set_projection(p);
    glView_->setFixedSize(w,h);
    updateGeometry();
  }

  blockSignals(false);

  updateGL();
}


//-----------------------------------------------------------------------------


void QtBaseViewer::setSynchronization(bool _b)
{
  synchronized_ = _b;
  action_["Synchronize"]->setChecked( synchronized_ );

  if (synchronized_)
  {
    connect(socket_,
	    SIGNAL(readyRead()),
	    this,
	    SLOT(sync_receive()));

    connect(this,
	    SIGNAL(signalSetView(const GLMatrixd&, const GLMatrixd&)),
	    this,
	    SLOT(sync_send(const GLMatrixd&, const GLMatrixd&)));
  }
  else
  {
    disconnect(socket_,
	       SIGNAL( readyRead() ),
	       this,
	       SLOT(sync_receive()));

    disconnect(this,
	       SIGNAL(signalSetView(const GLMatrixd&, const GLMatrixd&)),
	       this,
	       SLOT(sync_send(const GLMatrixd&, const GLMatrixd&)));
  }

}


//----------------------------------------------------------------------------


bool QtBaseViewer::add_sync_host(const QString& _name)
{
  QHostAddress adr;
  if (adr.setAddress(_name))
  {
    add_sync_host(adr);
    return true;
  }

  // QT4

  // use DNS otherwise
  QHostInfo hi = QHostInfo::fromName( _name );
  QList<QHostAddress> list = hi.addresses();
  if (!list.empty())
  {
    add_sync_host(list.front());
    return true;
  }


  return false;
}


void QtBaseViewer::add_sync_host(QHostAddress& _adr)
{
  sync_hosts_.push_back(_adr);
}

//=============================================================================
} // namespace QtWidgets
} // namespace ACG
//=============================================================================
