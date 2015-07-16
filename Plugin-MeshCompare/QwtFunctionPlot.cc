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
//  CLASS QtFunctionPlot - IMPLEMENTATION
//
//=============================================================================

//== INCLUDES =================================================================

#ifdef WITH_QWT

#include "QwtFunctionPlot.hh"

#include <iostream>
#include <algorithm>

#include <QPen>

#include <QLineEdit>
#include <QLabel>
#include <QPainter>
#include <qwt_curve_fitter.h>
#include <qwt_plot_panner.h>
#include <qwt_symbol.h>
#include <qwt_plot_histogram.h>

#include <ACG/Utils/ColorCoder.hh>


#include <cfloat>
#include <cmath>

//== NAMESPACES ===============================================================

//== IMPLEMENTATION ==========================================================


/// Default constructor
QwtFunctionPlot::QwtFunctionPlot(QWidget* _parent) :
    QDialog( _parent ),
    Ui::QwtFunctionPlotBase(),
    plot_zoomer_(0),
    clampMinMarker_(0),
    minSymbol_(0),
    clampMaxMarker_(0),
    maxSymbol_(0),
    min_(FLT_MAX),
    max_(FLT_MIN)
{
  setupUi( this );

  qwtPlot->setAxisTitle(QwtPlot::yLeft,   "count" );

  plot_zoomer_ = new QwtPlotZoomer( qwtPlot->canvas());
  plot_zoomer_->initKeyPattern();
  
  QwtPlotPanner *panner = new QwtPlotPanner( qwtPlot->canvas() );
  panner->setMouseButton( Qt::MidButton );
  
  // delete widget on close
  setAttribute(Qt::WA_DeleteOnClose, true);

  histogram_ = new Histogram();
}

//------------------------------------------------------------------------------

void QwtFunctionPlot::setFunction(const std::vector<double>& _values)
{
  values_ = _values;
}

//------------------------------------------------------------------------------

void QwtFunctionPlot::replot()
{
  //create intervals
  const int intervalCount = 100;

  QVector<QwtIntervalSample> intervals(intervalCount);
  std::vector< QColor > colors;

  double realMin = FLT_MAX;
  double realMax = -FLT_MAX;

  for ( unsigned int i=0; i < values_.size(); i++) {
    realMin = std::min(realMin,values_[i]);
    realMax = std::max(realMax,values_[i]);
  }

  double pos   = realMin;
  double width = ( realMax - realMin ) / intervalCount;

  QColor lastColor = Qt::black;

  ACG::ColorCoder cCoder(std::max(min_,realMin),std::min(max_,realMax));

  for ( int i = 0; i < (int)intervals.size(); i++ )
  {

    intervals[i] = QwtIntervalSample(0.0,pos, pos + width);
    pos += width;

    //compute a color for the given interval
    const double intervalCenter = pos + (width/2.0);

    colors.push_back( cCoder.color_qcolor(intervalCenter) );
    lastColor = colors.back();
  }

  // sort values into intervals
  // Could be more efficient when we calculate the correct interval based on the above loop directly
  for ( uint i=0; i < values_.size(); i++)
    for ( int j = 0; j < (int)intervals.size(); j++ )
      if ( intervals[j].interval.contains( values_[i]  ) ) {
        intervals[j].value++;
        break;
      }

  //get max Count for scaling the y-axis
  double maxCount = 0;

  for ( int i = 0; i < (int)intervals.size(); i++ )
    maxCount = std::max(maxCount, intervals[i].value);


  QwtIntervalSeriesData* data = new QwtIntervalSeriesData(intervals);
  histogram_->setData(data);
  histogram_->setColors(colors);
  histogram_->attach(qwtPlot);

  qwtPlot->setAxisScale(QwtPlot::yLeft, 0.0, maxCount);
  qwtPlot->setAxisScale(QwtPlot::xBottom, realMin, realMax);

  //define this scaling as the zoomBase
  plot_zoomer_->setZoomBase();

  //Mark the clamp values in the histogram.
  if ( min_ > realMin  ) {
    if ( ! clampMinMarker_ ) {
      clampMinMarker_ = new QwtPlotMarker();
      minSymbol_ = new QwtSymbol(QwtSymbol::VLine);
      minSymbol_->setColor(cCoder.color_qcolor(std::max(min_,realMin)));
      minSymbol_->setSize(200,1000);
      QPen pen = minSymbol_->pen();
      pen.setWidth(3);
      minSymbol_->setPen(pen);
      clampMinMarker_->setSymbol(minSymbol_);
      clampMinMarker_->attach(qwtPlot);
      clampMinMarker_->show();
    }

    // Draw at right boundary if greater than the maximal value we get from the function
    clampMinMarker_->setXValue(std::max(min_,realMin));

  } else {
    if ( clampMinMarker_ ) {
      clampMinMarker_->detach();
      delete clampMinMarker_;
      clampMinMarker_ = 0;
    }
  }

  //Mark the clamp values in the histogram.
  if ( max_ < realMax ) {
    if ( ! clampMaxMarker_ ) {
      clampMaxMarker_ = new QwtPlotMarker();
      maxSymbol_ = new QwtSymbol(QwtSymbol::VLine);
      maxSymbol_->setColor(cCoder.color_qcolor(std::min(max_,realMax)));
      maxSymbol_->setSize(200,1000);
      QPen pen = maxSymbol_->pen();
      pen.setWidth(3);
      maxSymbol_->setPen(pen);
      clampMaxMarker_->setSymbol(maxSymbol_);
      clampMaxMarker_->attach(qwtPlot);
      clampMaxMarker_->show();
    }

    // Draw at right boundary if greater than the maximal value we get from the function
    clampMaxMarker_->setXValue(std::min(max_,realMax));

  } else {
    if ( clampMaxMarker_ ) {
      clampMaxMarker_->detach();
      delete clampMaxMarker_;
      clampMaxMarker_ = 0;
    }
  }

  // an plot it
  qwtPlot->replot();
}

void QwtFunctionPlot::setMinMax(double _min, double _max) {
  min_ = _min;
  max_ = _max;
}



//=============================================================================
//=============================================================================

#endif // WITH_QWT
