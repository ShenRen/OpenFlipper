 
/*===========================================================================*\
*                                                                            *
*                              OpenFlipper                                   *
*      Copyright (C) 2001-2011 by Computer Graphics Group, RWTH Aachen       *
*                           www.openflipper.org                              *
*                                                                            *
*--------------------------------------------------------------------------- *
*  This file is part of OpenFlipper.                                         *
*                                                                            *
*  OpenFlipper is free software: you can redistribute it and/or modify       *
*  it under the terms of the GNU Lesser General Public License as            *
*  published by the Free Software Foundation, either version 3 of            *
*  the License, or (at your option) any later version with the               *
*  following exceptions:                                                     *
*                                                                            *
*  If other files instantiate templates or use macros                        *
*  or inline functions from this file, or you compile this file and          *
*  link it with other files to produce an executable, this file does         *
*  not by itself cause the resulting executable to be covered by the         *
*  GNU Lesser General Public License. This exception does not however        *
*  invalidate any other reasons why the executable file might be             *
*  covered by the GNU Lesser General Public License.                         *
*                                                                            *
*  OpenFlipper is distributed in the hope that it will be useful,            *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of            *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
*  GNU Lesser General Public License for more details.                       *
*                                                                            *
*  You should have received a copy of the GNU LesserGeneral Public           *
*  License along with OpenFlipper. If not,                                   *
*  see <http://www.gnu.org/licenses/>.                                       *
*                                                                            *
\*===========================================================================*/

/*===========================================================================*\
*                                                                            *
*   $Revision: 13619 $                                                       *
*   $LastChangedBy: moebius $                                                *
*   $Date: 2012-02-01 14:48:01 +0100 (Wed, 01 Feb 2012) $                     *
*                                                                            *
\*===========================================================================*/


/**
 * \file QtWidget.hh
 * This File contains all required includes for using Qt widgets in scenegraph
*/

#ifndef QT_WIDGET_INCLUDE_HH
#define QT_WIDGET_INCLUDE_HH

//== INCLUDES =================================================================

/** \def DATA_QT_WIDGET
 * Use this macro to reference qt widget objects
 */
#define DATA_QT_WIDGET typeId("QtWidget")

#include <ObjectTypes/QtWidget/QtWidgetObject.hh>

#include <ObjectTypes/QtWidget/PluginFunctionsQtWidget.hh>

#include <ObjectTypes/QtWidget/QtWidgetTypes.hh>

//=============================================================================
#endif // QT_WIDGET_INCLUDE_HH defined
//=============================================================================

