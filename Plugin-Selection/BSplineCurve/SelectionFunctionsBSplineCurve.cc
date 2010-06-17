/*===========================================================================*\
 *                                                                           *
 *                              OpenFlipper                                  *
 *      Copyright (C) 2001-2009 by Computer Graphics Group, RWTH Aachen      *
 *                           www.openflipper.org                             *
 *                                                                           *
 *---------------------------------------------------------------------------*
 *  This file is part of OpenFlipper.                                        *
 *                                                                           *
 *  OpenFlipper is free software: you can redistribute it and/or modify      *
 *  it under the terms of the GNU Lesser General Public License as           *
 *  published by the Free Software Foundation, either version 3 of           *
 *  the License, or (at your option) any later version with the              *
 *  following exceptions:                                                    *
 *                                                                           *
 *  If other files instantiate templates or use macros                       *
 *  or inline functions from this file, or you compile this file and         *
 *  link it with other files to produce an executable, this file does        *
 *  not by itself cause the resulting executable to be covered by the        *
 *  GNU Lesser General Public License. This exception does not however       *
 *  invalidate any other reasons why the executable file might be            *
 *  covered by the GNU Lesser General Public License.                        *
 *                                                                           *
 *  OpenFlipper is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *  GNU Lesser General Public License for more details.                      *
 *                                                                           *
 *  You should have received a copy of the GNU LesserGeneral Public          *
 *  License along with OpenFlipper. If not,                                  *
 *  see <http://www.gnu.org/licenses/>.                                      *
 *                                                                           *
\*===========================================================================*/

/*===========================================================================*\
 *                                                                           *
 *   $Revision$                                                         *
 *   $Author$                                                      *
 *   $Date$                   *
 *                                                                           *
\*===========================================================================*/


//=============================================================================
//
//  SelectionFunctions BSplineCurve
//  Author:  Ellen Dekkers <dekkers@cs.rwth-aachen.de>
//
//=============================================================================


#include <Plugin-Selection/SelectionPlugin.hh>
#include <OpenFlipper/BasePlugin/PluginFunctions.hh>

//-----------------------------------------------------------------------------

#ifdef ENABLE_BSPLINECURVE_SUPPORT    

//-----------------------------------------------------------------------------

void
SelectionPlugin::
toggleBSplineCurveSelection(QMouseEvent* _event)
{
  if ( _event->type() != QEvent::MouseButtonPress )
    return;

  if (selectionType_ & VERTEX)
  {
    unsigned int node_idx, target_idx;
    ACG::Vec3d   hit_point;
  
    if (PluginFunctions::scenegraphPick(ACG::SceneGraph::PICK_VERTEX, _event->pos(),node_idx, target_idx, &hit_point)) 
    {
      BaseObjectData* object;
  
      if ( PluginFunctions::getPickedObject(node_idx, object) )
      {
        BSplineCurveObject* bsco = PluginFunctions::bsplineCurveObject( object );
  
        // toggle selection
        if( bsco->splineCurve()->controlpoint_selections_available() )
        {
          if( target_idx < bsco->splineCurve()->n_control_points())
          {
            if( bsco->splineCurve()->controlpoint_selection( target_idx) == 0)
              bsco->splineCurve()->controlpoint_selection( target_idx ) = 1;
            else
              bsco->splineCurve()->controlpoint_selection( target_idx ) = 0;
          }
        }
  
        emit updateView();
      }
    }

  }
  
  else if (selectionType_ & KNOT)
  {
    unsigned int node_idx, target_idx;
    ACG::Vec3d   hit_point;
  
    if (PluginFunctions::scenegraphPick(ACG::SceneGraph::PICK_SPLINE, _event->pos(),node_idx, target_idx, &hit_point)) 
    {
      BaseObjectData* object;
  
      if ( PluginFunctions::getPickedObject(node_idx, object) )
      {
        BSplineCurveObject* bsco = PluginFunctions::bsplineCurveObject( object );
  
        // toggle selection
        if( bsco->splineCurve()->get_knotvector_ref()->selections_available() )
        {
          if( target_idx < bsco->splineCurve()->get_knotvector_ref()->size()) {
            if( bsco->splineCurve()->get_knotvector_ref()->selection( target_idx) == 0)
              bsco->splineCurve()->get_knotvector_ref()->selection( target_idx ) = 1;
            else
              bsco->splineCurve()->get_knotvector_ref()->selection( target_idx ) = 0;
          }
        }
  
        emit updateView();
      }
    }
  }
  
}

#endif
