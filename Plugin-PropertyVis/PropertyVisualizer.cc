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

#include "PropertyVisualizer.hh"

/*! \brief Visualizes the property
 *
 * This method should be implemented by a subclass that specializes on visualizing
 * a specific property type for a specific object type. This method here only
 * implements a warning which will tell the user that visualizing the property for
 * that object type is not implemented yet.
 */
void PropertyVisualizer::visualize(bool _setDrawMode, QWidget* _widget)
{
    log(LOGERR, "Visualizing for this type of object not yet supported.");
}

/*! \brief Removes the property
 *
 * This method should be implemented by a subclass that specializes on removing
 * properties for a specific object type. This method here only implements a
 * warning which will tell the user that removing the property is not implemented
 * yet for that object type.
 */
void PropertyVisualizer::removeProperty()
{
    log(LOGERR, "Removing for this type of object not yet supported.");
}

/*! \brief Duplicates the property
 *
 * This method should be implemented by a subclass that specializes on duplicating
 * properties for a specific object type. This method here only implements a warning
 * which will tell the user that removing the property is not implemented yet
 * for that object type.
 */
void PropertyVisualizer::duplicateProperty()
{
    log(LOGERR, "Duplicating for this type of object not yet supported.");
}

/*! \brief Clears the property visualization
 *
 * This method should be implemented by a subclass that specializes on clearing
 * property visualizations for a specific object type. This method here only
 * implements a warning which will tell the user that clearing the property
 * visualitaion is not implemented yet for that object type.
 */
void PropertyVisualizer::clear()
{
    log(LOGERR, "Clearing for this type of object not yet supported.");
}

QString PropertyVisualizer::toStr(OpenMesh::Vec3d v)
{
    return QObject::trUtf8("∥(%1, %2, %3)∥ = %4").arg(v[0]).arg(v[1]).arg(v[2])
            .arg(v.norm());
}

QString PropertyVisualizer::toStr(OpenMesh::Vec2d v)
{
    return QObject::tr("∥(%1, %2)∥ = %3").arg(v[0]).arg(v[1]).arg(v.norm());
}

QString PropertyVisualizer::toStr(OpenMesh::Vec2f v)
{
    return QObject::tr("∥(%1, %2)∥ =  %3").arg(v[0]).arg(v[1]).arg(v.norm());
}

#ifdef ENABLE_SKELETON_SUPPORT
QString PropertyVisualizer::toStr(BaseSkin::SkinWeights sw)
{
    QString text = "";
    for (std::map<unsigned int, double>::iterator it = sw.begin(); it != sw.end(); ++it) {
      text += "(";
      text += QString::number((*it).first);
      text += ",";
      text += QString::number((*it).second);
      text += ")\n";
    }
    return text;
}
#endif


OpenMesh::Vec3d PropertyVisualizer::strToVec3d (QString str)
{
    QString s = str;
    s.remove(0,2);
    s.truncate(s.lastIndexOf(")"));
    QStringList strList = s.split(QObject::tr(", "));
    return OpenMesh::Vec3d(strList[0].toDouble(),strList[1].toDouble(),strList[2].toDouble());
}

OpenMesh::Vec2d PropertyVisualizer::strToVec2d (QString str)
{
    QString s = str;
    s.remove(0,2);
    s.truncate(s.lastIndexOf(")"));
    QStringList strList = s.split(QObject::tr(", "));
    return OpenMesh::Vec2d(strList[0].toDouble(),strList[1].toDouble());
}

OpenMesh::Vec2f PropertyVisualizer::strToVec2f (QString str)
{
    QString s = str;
    s.remove(0,2);
    s.truncate(s.lastIndexOf(")"));
    QStringList strList = s.split(QObject::tr(", "));
    return OpenMesh::Vec2f(strList[0].toFloat(),strList[1].toFloat());
}

std::unique_ptr<ACG::IColorCoder> PropertyVisualizer::buildColorCoder()
{
    throw std::runtime_error("Requested color coder on a Propvis that does not implement it");
}
