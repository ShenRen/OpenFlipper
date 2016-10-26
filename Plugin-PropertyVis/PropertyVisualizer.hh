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

#ifndef PROPERTY_VISUALIZER_HH
#define PROPERTY_VISUALIZER_HH

#include "Utils.hh"

#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>
#include <ACG/QtWidgets/QtHistogramWidget.hh>
#include <ACG/Utils/IColorCoder.hh>

#include "OpenMesh/Core/Geometry/VectorT.hh"

#ifdef ENABLE_SKELETON_SUPPORT
    #include <ObjectTypes/Skeleton/BaseSkin.hh>
#endif

#include <stdexcept>

class VizException : public std::logic_error {
    public:
        VizException(const std::string &msg) : std::logic_error(msg) {}
};


/*! \class PropertyVisualizer
 *  \brief This class vizualizes a property.
 *
 * For each property one object of this class is created. It is responsible for
 * the visualization of that property. It provides a widget that can be used
 * to set options for the visualization.
 *
 */
class PropertyVisualizer: public QObject
{
    Q_OBJECT

signals:
    void log(Logtype _type, QString _message);
    void log(QString _message);

public:
    /**
     * @brief Constructor
     *
     * @param _propertyInfo Information about the property this visualizer should visualize.
     */
    PropertyVisualizer(PropertyInfo _propertyInfo)
        :
          propertyInfo(_propertyInfo),
          widget(0)
    {
    }

    /// Destructor
    virtual ~PropertyVisualizer(){delete widget; }

    /// Visualizes the property.
    virtual void visualize(bool _setDrawMode, QWidget* _widget);

    /// Removes the property.
    virtual void removeProperty();

    /// Duplicates the property.
    virtual void duplicateProperty();

    /// Clears the property visualization.
    virtual void clear();

    /**
     * @brief Returns a beautiful name.
     *
     * The beautiful name consists of a symbol representing the entity type, the property's
     * name and the property's type.
     */
    virtual QString getName() { return propertyInfo.toString(); }

    /** @brief Returns the visualizer's widget.
     *
     * Returns the visualizer's widget that can be used to set some options
     * for the visualization.
     */
    virtual QWidget* getWidget() { return widget; }

    /// Returns the PropertyInfo.
    const PropertyInfo& getPropertyInfo() const { return propertyInfo; }

    /**
     * @brief Returns the value of a property in text form.
     * @param i The id of the entity whose property we want to know.
     * @return The property value in text form.
     */
    virtual QString getPropertyText(unsigned int i) = 0;


    /**
     * @brief Returns the value of a property in text form.
     * @param index The id of the entity whose property should be set.
     * @param text The value in text form.
     *
     * This method sets the porperty of a given entity by transforming
     * the text into the correct type. This is used when a property is
     * loaded from a file.
     */
    virtual void setPropertyFromText(unsigned int index, QString text) = 0;

    /// Returns the number of entities.
    virtual int getEntityCount() = 0;

    /// Returns the header for saving.
    virtual QString getHeader() = 0;

    static inline QString toStr(bool b)               { return b ? QObject::tr("True") : QObject::tr("False"); }
    static inline QString toStr(double d)             { return QObject::tr("%1").arg(d); }
    static inline QString toStr(int i)                { return QObject::tr("%1").arg(i); }
    static inline QString toStr(uint8_t i)                { return QObject::tr("%1").arg(i); }
    static inline QString toStr(unsigned int i)       { return QObject::tr("%1").arg(i); }
    static        QString toStr(OpenMesh::Vec3d v);
    static        QString toStr(OpenMesh::Vec2d v);
    static        QString toStr(OpenMesh::Vec2f v);
#ifdef ENABLE_SKELETON_SUPPORT
    static        QString toStr(BaseSkin::SkinWeights sw);
#endif

    static inline bool            strToBool  (QString str) { return (str.compare(QObject::tr("True"))==0); }
    static inline double          strToDouble(QString str) { return str.toDouble() ;    }
    static inline int             strToInt   (QString str) { return str.toInt();        }
    static inline unsigned int    strToUInt  (QString str) { return str.toUInt();       }
    static        OpenMesh::Vec3d strToVec3d (QString str);
    static        OpenMesh::Vec2d strToVec2d (QString str);
    static        OpenMesh::Vec2f strToVec2f (QString str);


protected:
    virtual ACG::IColorCoder *buildColorCoder();

    template<typename PropType, typename Iterable>
    void showHistogramT(ACG::QtWidgets::QtHistogramWidget *widget,
                       Iterable data);

    PropertyInfo propertyInfo;

public:
    QWidget* widget;
};

template<typename PropType, typename Iterable>
void PropertyVisualizer::showHistogramT(
        ACG::QtWidgets::QtHistogramWidget *widget,
        Iterable data)
{
    const size_t max_bins = 50; // TODO: expose in GUI?
    widget->setMinimumHeight(300);
    widget->setColorCoder(buildColorCoder());
    ACG::Histogram *hist = new ACG::HistogramT<PropType>(data.begin(), data.end(), max_bins);
    widget->setHistogram(hist);
}

#endif /* PROPERTY_VISUALIZER_HH */
