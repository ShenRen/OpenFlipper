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

#ifndef SIDE_ELEMENT_
#define SIDE_ELEMENT_

//=============================================================================
//
//  CLASS SideElement
//
//=============================================================================

//== INCLUDES =================================================================

#include <QWidget>
#include <QSettings>

//== FORWARDDECLARATIONS ======================================================

class QIcon;
class QLabel;
class QToolButton;
class QAction;
class QDialog;
class QVBoxLayout;

class SideArea;


//== CLASS DEFINITION =========================================================


/** \class SideElement SideElement.hh <OpenFlipper/widgets/coreWidget/SideElement.hh>

  A widget that holds the plugin side area widget
 **/


class SideElement : public QWidget
{
  Q_OBJECT

  public:
    /** Create a SideElement
      @param _parent   SideArea widget that holds this widget
      @param _w        Plugin tool widget
      @param _name     Plugin name
      @param _icon     An icon that should be shown in the title bar of the side element

    */
    SideElement (SideArea *_parent, QWidget *_w, QString _name, QIcon* _icon = 0);

    /// Destructor
    ~SideElement ();

    /// Set the element as active
    void setActive(bool _active);

    /// saves the current state
    void saveState (QSettings &_settings);

    /// restores the state
    void restoreState (QSettings &_settings);

    /// return the name
    const QString& name();

    /// returns if the SideElement is active
    bool active();

    /// returns the pointer to the plugin tool widget
    QWidget const * widget();

  signals:
    void toggleActive(bool);

  private:

    /// Clickable area inside of the side element.
    class TopArea : public QWidget
    {
      public:
        TopArea (SideElement *_e);

        virtual void mousePressEvent (QMouseEvent *_event);
      private:
        SideElement *e_;
    };

    /// Called on mouse press
    void labelPress ();

  private slots:

    /// Called if the detach button was pressed
    void detachPressed (bool _checked);

    /// Called if a detached dialog was closed
    void dialogClosed ();

  private:

    // parent
    SideArea *parent_;

    // plugin widget
    QWidget *widget_;

    // plugin name
    QString name_;

    // Icon
    QIcon* icon_;

    // main layout
    QVBoxLayout *mainLayout_;

    // status
    bool active_;

    // label that display the plugin name
    QLabel *label_;
    QLabel *iconHolder_;

    // detach button & action
    QToolButton *detachButton_;
    QAction *detachAction_;

    // dialog for detached widget
    QDialog *dialog_;
};

//=============================================================================
//=============================================================================
#endif // SIDE_ELEMENT_ defined
//=============================================================================
