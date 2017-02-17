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
*   $Revision: 13770 $                                                       *
*   $LastChangedBy: moebius $                                                *
*   $Date: 2012-02-13 16:19:41 +0100 (Mo, 13. Feb 2012) $                     *
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

#include <ui_QwtFunctionPlotBase.hh>

#include <ACG/QwtWidgets/QwtHistogramm.hh>
#include <vector>

#include <QDialog>

//== FORWARDDECLARATIONS ======================================================

//== NAMESPACES ===============================================================

//== CLASS DEFINITION =========================================================



class QwtFunctionPlot : public QDialog, public Ui::QwtFunctionPlotBase
{
  Q_OBJECT
public:

  /// Default constructor
  explicit QwtFunctionPlot( QWidget*    _parent = 0 );

  /// Destructor
  ~QwtFunctionPlot() {}

  /// set the function to plot
  void setFunction(const std::vector<double>& _values );

  void setMinMax(double _min, double _max);

public slots:
  
  void replot();

private:

  QwtPlotZoomer* plot_zoomer_;

  QwtPlotMarker* clampMinMarker_;
  QwtSymbol*     minSymbol_;

  QwtPlotMarker* clampMaxMarker_;
  QwtSymbol*     maxSymbol_;

  /// Histogram Plot
  Histogram*     histogram_;

  /// Values that will define the function to plot
  std::vector<double> values_;

  double min_,max_;
};


//=============================================================================
#endif // WITH_QWT
//=============================================================================
#endif // ACG_QWTFUNCTIONPLOT_HH defined
//=============================================================================

