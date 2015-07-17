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

#include <QApplication>
#include <QStatusBar>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>

//== NAMESPACES ===============================================================

namespace ACG {
namespace QtWidgets {


//== IMPLEMENTATION ==========================================================

void QtBaseViewer::snapshotBaseFileName(const QString& _fname)
{
  snapshotName_=_fname;
  snapshotCounter_=0;
}


//-----------------------------------------------------------------------------


void QtBaseViewer::snapshot()
{
   makeCurrent();

   glView_->raise();
   qApp->processEvents();
   makeCurrent();
   paintGL();
   glFinish();
   copyToImage(*snapshot_, 0, 0, glWidth(), glHeight(), GL_BACK);

   QFileInfo fi(snapshotName_);

   QString fname = fi.path() + QDir::separator() +fi.baseName() + "." + QString::number(snapshotCounter_).rightJustified(7, '0') + ".";

   QString format="png";

   if (fi.completeSuffix() == "ppm")
     format="ppmraw";

   fname += format;

   bool rval=snapshot_->save(fname,format.toUpper().toLatin1());


   assert(statusbar_!=0);
   if (rval)
   {
     statusbar_->showMessage(QString("snapshot: ")+fname,5000);
     if ( action_["SnapshotSavesView"]->isChecked() )
     {
       QString view; encodeView(view);
       QFile f(fname+".view"); f.open(QIODevice::WriteOnly|QIODevice::Truncate);
       QTextStream sf(&f); sf << view; f.close();
     }
   }
   else
   {
     statusbar_->showMessage(QString("could not save snapshot to ")+fname);
   }

   ++snapshotCounter_;
}

//-----------------------------------------------------------------------------


void QtBaseViewer::actionSnapshot()
{
  snapshot();
}


//-----------------------------------------------------------------------------


void QtBaseViewer::actionSnapshotName()
{
  QString fname=snapshotName_;

  fname.replace('%', '$');
  fname = QFileDialog::getSaveFileName ( 0, "Save snapshot name" );
  if (!fname.isEmpty())
  {
    fname.replace('$', '%');

    snapshotBaseFileName(fname);
    assert(statusbar_!=0);
    QString msg="next snapshot: ";
    statusbar_->showMessage(msg);
  }
}


//-----------------------------------------------------------------------------


void QtBaseViewer::actionSnapshotSavesView()
{
}

//=============================================================================
} // namespace QtWidgets
} // namespace ACG
//=============================================================================
