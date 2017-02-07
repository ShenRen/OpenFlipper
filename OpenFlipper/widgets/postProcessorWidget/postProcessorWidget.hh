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
*   $Revision: 15910 $                                                       *
*   $LastChangedBy: moeller $                                                *
*   $Date: 2012-12-05 12:53:39 +0100 (Mi, 05 Dez 2012) $                     *
*                                                                            *
\*===========================================================================*/

#include "ui_postProcessorWidget.hh"

#include <vector>

#include <QDialog>
#include <QPoint>
#include <QStringList>
class QWidget;
class QFrame;
class PostProcessorInfo;

class PostProcessorDialog : public QDialog, public Ui::PostProcessorWidget
{
  Q_OBJECT

  signals:
    /// request an update for an specified viewer
    void updateExaminer(unsigned _viewer);

  public:
    explicit PostProcessorDialog(QWidget *_parent = 0);

    /// return the names of all saved post processors
    static QStringList getSavedPostProcessorNames(const unsigned _examiner);

    /// append all saved post processors
    static void loadSavedPostProcessors(const unsigned _examiner);

  public slots:
    /// refreshes the content of the dialog with current examiner
    void refresh();

  private slots:
    /// Show the custom context menu for activation
    void slotContextMenuActivate(const QPoint& _point);

    /// Show the custom context menu for deactivation
    void slotContextMenuDeactivate(const QPoint& _point);

    /// Activates the post processor (triggered via the context menu)
    void slotActivatePostProcessor();

    /// Deactivates the current postProcessor
    void slotDeactivatePostProcessor();

    /// Move the position/ordering of postprocessor in the postprocessor
    void slotMovePostProcessor(unsigned _from, unsigned _to);

    /// move the selected active postprocessor 1 up
    void slotMoveUp();

    /// move the selected active postprocessor 1 down
    void slotMoveDown();

    /// saves active post processor chain
    void slotSaveActive();

  protected:
    void closeEvent(QCloseEvent *_event);

    void showEvent ( QShowEvent * );

  private:
    /// initiaize the window with the post processors of the current examiner
    void initWindow();

    QFrame* createFrame(const PostProcessorInfo& _pPI);

    /// holds the examiner id for the window
    unsigned currentExaminer_;

    /// maps activeRow from activeList (same as chainIdx from RendererInfo) to row from list (same as activeId from RendererInfo) for each viewer
    std::vector<unsigned> activeRowToRow_;

};

