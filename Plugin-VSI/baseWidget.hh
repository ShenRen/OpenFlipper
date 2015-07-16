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

#ifndef BASEWIDGET_HH
#define BASEWIDGET_HH

//== INCLUDES =================================================================

#include <QMainWindow>
#include <QSplitter>
#include <QStackedWidget>
#include <QToolBox>
#include <QStack>

#include "scene/graphicsView.hh"
#include "scene/graphicsScene.hh"


//== NAMESPACES ===============================================================
namespace VSI {

//== FORWARDDECLARATIONS ======================================================
class Context;

//== CLASS DEFINITION =========================================================

/** Main widget for the visual scripting interface editor
*/
class BaseWidget : public QMainWindow
{
  Q_OBJECT

  public:

    /// Singleton constructor
    static BaseWidget *createBaseWidget (Context *_ctx, QWidget *_parent = NULL);
    static BaseWidget *getBaseWidget ();

    /// Destructor
    ~BaseWidget ();

  signals:
    void codeToScriptEditor (QString _code);

  public slots:

    /// executes current script
    void executeCode ();

    /// converts visual script to textual script
    void codeToScript ();

    /// save to file
    bool save (bool _newName = false);

    /// save to file
    bool saveAs ();

    /// load file
    void load ();

    /// new empty file
    void newFile ();

    /// used changed something
    void contentChanged ();

    /// show a new scene (function) in editor
    void pushScene (GraphicsScene *_scene);

    /// go back to last scene (function)
    void popScene ();

    /// add a new scene
    void addScene (GraphicsScene *_scene);

    /// remove a scene
    void removeScene (GraphicsScene *_scene);

  private:

    // constructor
    BaseWidget (Context *_ctx, QWidget *_parent);

    // initalize widgets
    void setupUi ();

    // update window title bar
    void updateTitle ();

    // asks the user to save his changes
    bool saveIfChanged ();

    // catch window close
    void closeEvent (QCloseEvent *_event);

  private:

    Context *ctx_;

    GraphicsScene *mainScene_;
    QStack<GraphicsScene *> scenes_;

    QSplitter *splitter_;
    QToolBox  *toolbox_;
    QStackedWidget *views_;

    QString   fileName_;
    bool changedContent_;

    static BaseWidget *base_;
};

//=============================================================================
}
//=============================================================================

#endif
