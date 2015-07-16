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
// CLASS QwtFunctionPlot
//
//
// Author:  David Bommes <bommes@cs.rwth-aachen.de>
//
// Version: $Revision: 1$
// Date:    $Date: 200X-XX-XX$
//
//=============================================================================


#ifndef ACG_QWTFUNCTIONPLOT_HH
#define ACG_QWTFUNCTIONPLOT_HH

#ifdef WITH_QWT

//== INCLUDES =================================================================

#if QT_VERSION >= 0x050000 
  #include <QtWidgets>
#else
  #include <QtGui>
#endif

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_zoomer.h>
#include <qwt_scale_engine.h>
#include <qwt_plot_marker.h>

#include <QDialog>

// qmake users have to include pc
#include <ui_QwtFunctionPlotBase.hh>

// ACGMake users have to include
// #include "QtFunctionPlotBase.hh"

#if QWT_VERSION >= 0x060000
 #include <ACG/QwtWidgets/QwtHistogramm.hh>
#else
 #include "HistogramItem.hh"
#endif

#include <vector>
#include "TextureParameters.hh"


//== FORWARDDECLARATIONS ======================================================

//== NAMESPACES ===============================================================

namespace ACG
{

//== CLASS DEFINITION =========================================================



class QwtFunctionPlot : public QDialog, public Ui::QwtFunctionPlotBase
{
  Q_OBJECT
public:

  /// Default constructor
  QwtFunctionPlot( QWidget*    _parent = 0 );

  /// Destructor
  ~QwtFunctionPlot() {}

  // set the function to plot
  void setFunction( std::vector<double>& _values );

  /// Set all parameters as one block
  void setParameters(const TexParameters& _parameters);

  /// Set the parameters separately
  void setParameters(bool repeat, double repeatMax,
                     bool clamp,  double clampMin, double clampMax,
                     bool center,
                     bool absolute,
                     bool scale);

  /** The function plot accesses an image to get colors from them.
   *  The colors are equivalent to the rendered color textures.
   *
   * @param _image Image that is used to take the colors from
   */
  void setImage(QImage* _image);

public slots:
  void zoomIn();
  void zoomOut();
  void clamp();
  
  void replot();

private:

  QwtPlotZoomer* plot_zoomer_;

  QwtPlotMarker* clampMinMarker_;
  QwtSymbol*     minSymbol_;

  QwtPlotMarker* clampMaxMarker_;
  QwtSymbol*     maxSymbol_;

  // Histogramm Plot
#if QWT_VERSION >= 0x060000
  Histogram* histogram_;
#else
  HistogramItem* histogram_;
#endif

  std::vector<double> values_;

  QImage* image_;

  TexParameters parameters_;

  double min_,max_;

};


//=============================================================================
} // namespace ACG

#endif // WITH_QWT

//=============================================================================
#endif // ACG_QWTFUNCTIONPLOT_HH defined
//=============================================================================

