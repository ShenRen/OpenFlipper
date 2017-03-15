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
//  Standard Functions
//
//=============================================================================

/**
 * \file PluginFunctions.hh
 * This file contains functions to setup the internal structures for PluginFunctions.
 * Dont Use these Functions in your Plugins!
 */

//
#ifndef PLUGINFUNCTIONSCORE_HH
#define PLUGINFUNCTIONSCORE_HH

#ifndef OPENFLIPPERCORE
  #ifndef OPENFLIPPERPLUGINLIB
    #ifdef WIN32
      #pragma message("PluginFunctionsCore is only allowed to be used from within OpenFlippers Core application!")
      
    #else
      #warning PluginFunctionsCore is only allowed to be used from within OpenFlippers Core application!
      #error Bla
    #endif
  #endif
#endif

#include <OpenFlipper/common/GlobalDefines.hh>

#include <OpenFlipper/widgets/glWidget/QtBaseViewer.hh>
#include <OpenFlipper/common/ViewerProperties.hh>

#include <ACG/QtWidgets/QtSceneGraphWidget.hh>
#include <OpenFlipper/BasePlugin/PluginFunctionsSceneGraphWidget.hh>

namespace PluginFunctions {

//=======================================
// Set pointers for global handling in PluginFunctions without exporting them to the Plugins
    /** @name Setup Functions
    * @{ */
//=======================================
/// Set the internal Viewer pointer ( DO NOT USE!! )
DLLEXPORT
void setViewers( std::vector< glViewer* > _viewerWidgets );

/// Get a Viewer
DLLEXPORT
glViewer* viewer(int  _viewerId );

/// Set the internal viewerProperties pointer ( DO NOT USE!! )
DLLEXPORT
void setViewerProperties( std::vector< Viewer::ViewerProperties* > _viewerProperties );

/** Set the internal scenegraph root node pointer. This is the topmost
* node of the whole scenegraph ( DO NOT USE!! )
*/
DLLEXPORT
void setSceneGraphRootNode( SeparatorNode* _root_node );

/** Set the internal scenegraph root node pointer for global nodes 
*
* This node is below the global status nodes and takes global nodes
* at one level. ( DO NOT USE!! )
*/
DLLEXPORT
void setSceneGraphRootNodeGlobal( SeparatorNode* _root_node );

/// Set the internal data root node pointers ( DO NOT USE!! )
DLLEXPORT
void setDataSeparatorNodes( SeparatorNode* _dataRootNode );

/** @} */

//=======================================
// Handle object count internally
/** @name Internal Counters
* @{ */
//=======================================
/// Decrease the number of current Object
DLLEXPORT
void increaseObjectCount();

/// Increase the number of current Object
DLLEXPORT
void decreaseObjectCount();

/// Decrease the number of current Object
DLLEXPORT
void increaseTargetCount();

/// Increase the number of current Object
DLLEXPORT
void decreaseTargetCount();

/** @} */

//=======================================
// Object container functions
/** @name Internal object handle container
* @{ */
//=======================================

/// Add object to internal object map
DLLEXPORT
void addObjectToMap(int _objectId, BaseObject* _object);

/// Remove object from internal object map
DLLEXPORT
void removeObjectFromMap(int _objectId);

/** @} */

//=======================================
// SceneGraph Generator Map
/** @name Internal container for scenegraph widget generators
* @{ */
//=======================================

DLLEXPORT
QMap< std::string ,ACG::QtWidgets::SceneGraphWidgetGenerator* > getSceneGraphGeneratorList();

/** @} */

}

#endif //PLUGINFUNCTIONSCORE_HH
