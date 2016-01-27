/*
 * ValenceHistogramWidget.hh
 *
 *  Created on: Jan 27, 2016
 *      Author: hc
 */

#ifndef PLUGIN_INFOMESHOBJECT_VALENCEHISTOGRAMWIDGET_HH_
#define PLUGIN_INFOMESHOBJECT_VALENCEHISTOGRAMWIDGET_HH_

#include <QWidget>

class ValenceHistogramWidget: public QWidget {
    Q_OBJECT

    public:
        ValenceHistogramWidget(QWidget *parent);

        void setHistogram(std::vector<size_t> *histogram);

    protected:
        void paintEvent(QPaintEvent *event);

        std::vector<size_t> *histogram_;
};

#endif /* PLUGIN_INFOMESHOBJECT_VALENCEHISTOGRAMWIDGET_HH_ */
