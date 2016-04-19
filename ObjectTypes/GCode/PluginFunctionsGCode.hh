//=============================================================================
//
//  Standard Functions
//
//=============================================================================

/**
 * \file PluginFunctionsPolyLine.hh
 * This file contains functions which can be used by plugins to access polylines in the framework.
 */

#pragma once

#include "GCodeObject.hh"
#include <OpenFlipper/common/Types.hh>


/** The Namespace PluginFunctions contains functions for all plugins. */
namespace PluginFunctions {


/** This functions returns the object with the given id if it is a PolyLineObject.
 * See get_object(  int _identifier , BaseObject*& _object ) for more details.
 */
DLLEXPORT
bool getObject(  int _identifier , GCodeObject*& _object );

/** \brief Get a poly Line from an object.
 *
 * @param _object The object should be of type BaseDataObject. If the content is a poly Line, a
 *                poly line will be returned. Otherwise a NULL pointer is returned.
 */
DLLEXPORT
GCode::Shared gcode( BaseObjectData* _object );

/** \brief Cast an BaseObject to a PolyLineObject if possible
 *
 * @param _object The object should be of type BaseDataObject. If the content is a polyLine, a
 *                a PolyLineObject is returned. Otherwise a NULL pointer is returned.
 */
DLLEXPORT
GCodeObject* gcodeObject( BaseObjectData* _object );

}


