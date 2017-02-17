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



#ifndef SIDE_AREA_
#define SIDE_AREA_

//=============================================================================
//
//  CLASS SideElement
//
//=============================================================================

//== INCLUDES =================================================================

#include <QIcon>
#include <QWidget>
#include <QString>
#include <QSettings>

//== FORWARDDECLARATIONS ======================================================

class QVBoxLayout;

class SideElement;

/** \class SideArea SideArea.hh <OpenFlipper/widgets/coreWidget/SideArea.hh>

  A widget that holds the different plugin tool widgets
 **/


class SideArea : public QWidget {
  public:

    /** Create a SideElement
      \param _parent Parent widget
    */
    explicit SideArea (QWidget *_parent = 0);

    /** Adds a plugin tool widget
      \param _plugin plugin corresponding to the widget
      \param _w Plugin widget
      \param _name Plugin name
      \param _icon an icon
      \param _headerAreaWidget
    */
    void addItem (QObject const * const _plugin, QWidget *_w, QString _name,
            QIcon* _icon = 0, QWidget *_headerAreaWidget = 0);

    /// clears the whole tool widget area
    void clear ();

    //expand all elements
    void expandAll();

    void expand(QWidget *sideElementWidget, bool expand);

    /// returns the current state
    void saveState (QSettings &_settings);

    /// saves the active state of _viewMode
    void saveViewModeState(const QString& _viewMode);

    /// restores the state
    void restoreState (QSettings &_settings);

    /// restores the active state of _viewMode
    void restoreViewModeState(const QString& _viewMode);

    /// set the active state of given element
    void setElementActive(QString _name, bool _active);

    /// Move a toolbox widget to a given position
    void moveItemToPosition(const QString& _name, int _position);

    /// Move a toolbox widget to a given position
    void moveItemToPosition(QObject const * const _plugin, const QString& _name, int _position);

    /// Get number of widgets
    int getNumberOfWidgets() const;

    /// Get plugins in side area
    const QList<const QObject *>& plugins();

    /// Get item names
    const QStringList& names();

    // position of the last added item
    int lastPos_;

  private:
    // elements
    QVector<SideElement *> items_;

    // plugins in side area
    QList<const QObject *> plugins_;

    // item names
    QStringList itemNames_;

    // saves active state of each SideElement in each view
    QMap<QString, bool> sideElementState_;

    // main layout
    QVBoxLayout *layout_;
};

//=============================================================================
//=============================================================================
#endif // SIDE_AREA_ defined
//=============================================================================

