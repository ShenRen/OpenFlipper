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


#include <QPrintDialog>
#include <QPrinter>

#include "PrintPlugin.hh"

#include <iostream>
#include <ACG/GL/GLState.hh>

#include <OpenFlipper/BasePlugin/PluginFunctions.hh>
#include <OpenFlipper/common/GlobalOptions.hh>

#if QT_VERSION >= 0x050000 
#include <QtWidgets>
#else
#include <QtGui>
#endif

void PrintPlugin::pluginsInitialized()
{
  // Print menu
  QMenu *printMenu = new QMenu(tr("&Printing"));

  printMenu->setIcon(QIcon(OpenFlipper::Options::iconDirStr()+OpenFlipper::Options::dirSeparator()+"document-print.png"));

  emit addMenubarAction(printMenu->menuAction(), FILEMENU );

  QAction* AC_Print = new QAction(tr("&Print"), this);;
  AC_Print->setStatusTip(tr("Print the current view"));
  AC_Print->setIcon(QIcon(OpenFlipper::Options::iconDirStr()+OpenFlipper::Options::dirSeparator()+"document-print.png"));

  connect(AC_Print, SIGNAL(triggered()), this, SLOT(printView()));
  printMenu->addAction(AC_Print);
}

void PrintPlugin::printView () {


  QPrinter printer(QPrinter::HighResolution);

  QPrintDialog *dialog = new QPrintDialog(&printer);
  dialog->setWindowTitle(tr("Print Current View"));
  if (dialog->exec() != QDialog::Accepted)
      return;

  QImage image;

  // create the snapshot
  PluginFunctions::getCurrentViewImage(image);

  QPainter painter;
  painter.begin(&printer);

  painter.save();

  QRect rect = painter.viewport();
  QSize size = image.size();
  size.scale(rect.size(), Qt::KeepAspectRatio);
  painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
  painter.setWindow(image.rect());

  painter.drawImage(0, 0, image);

  painter.restore();
  painter.end();

}

void PrintPlugin::slotKeyEvent( QKeyEvent* _event ) {
  if (_event->modifiers() == Qt::ControlModifier ) {
    switch (_event->key())
    {
      case Qt::Key_P :
              printView();
              return;
    }
  }
}


#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2( printplugin , PrintPlugin );
#endif

