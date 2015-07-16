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




//=============================================================================
//
//  CLASS QtMultiViewLayout - IMPLEMENTATION
//
//=============================================================================

//== INCLUDES =================================================================

#include <OpenFlipper/common/GlobalDefines.hh>
#include <QGraphicsLayout>
#include <QGraphicsLayoutItem>
#include <QGraphicsWidget>

//== NAMESPACES ===============================================================

//== CLASS DEFINITION =========================================================


/** Graphics scene layout for multi view.
*/

class DLLEXPORT QtMultiViewLayout : public QGraphicsLayout
{
  public:

    /// MultiView display modes
    enum MultiViewMode {
       /*
       * #############
       * #           #
       * #           #
       * #     1     #
       * #           #
       * #           #
       * #############
       */
       SingleView,
       /*
       * #############
       * #     #     #
       * #     #     #
       * #  1  #  2  #
       * #     #     #
       * #     #     #
       * #############
       */
       DoubleView,
       /*
       * #############
       * #  1  #  2  #
       * #     #     #
       * #############
       * #  3  #  4  #
       * #     #     #
       * #############
       */
      Grid,
      /*
       * #############
       * #       # 2 #
       * #       #####
       * #   1   # 3 #
       * #       #####
       * #       # 4 #
       * #############
       */	
      HSplit
    };

    QtMultiViewLayout (QGraphicsLayoutItem * _parent = 0);

    /// Adds Widget to Layout
    void addItem (QGraphicsWidget *_item, unsigned int _pos);

    /// Sets layout mode
    void setMode (MultiViewMode _mode);

    /// Retruns current layout modes
    MultiViewMode mode() const { return mode_;};

    /// Sets space between items
    void setSpacing (unsigned int _s);

    /// Sets primary element for SingleView and HSplit
    void setPrimary (unsigned int _i);


    /// Pure virtual functions that have to be implemented
    virtual int count() const;
    virtual QGraphicsLayoutItem * itemAt(int _i) const;
    virtual void removeAt (int _index);

    virtual QSizeF sizeHint(Qt::SizeHint _which, const QSizeF & _constraint = QSizeF()) const;

    /// Tracks geometry changes
    virtual void setGeometry(const QRectF & rect);


  private:

    /// Recalculate layout
    void reLayout ();

    /// current modes
    MultiViewMode mode_;

    /// Spacing
    unsigned int  spacing_;

    /// Items
    QGraphicsWidget *items_[4];

    /// Primary element
    unsigned int primary_;

};

//=============================================================================

//=============================================================================
