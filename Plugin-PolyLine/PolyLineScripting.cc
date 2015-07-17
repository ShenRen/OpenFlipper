/*===========================================================================*\
 *                                                                           *
 *                              OpenFlipper                                  *
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
 *                                                                           *
\*===========================================================================*/

#include "PolyLinePlugin.hh"

//------------------------------------------------------------------------------

/** \brief Generates a polyLine of a plane intersection
*
*
* @param _objectId id of the target object
* @param _planePoint a point on the cut plane
* @param _planeNormal the normal of the cut plane
* @param _polyLineId Specify this if you don't want a new polyline to be generated but to reuse an existing one
* @return returns the id of the polyLine
*/
int PolyLinePlugin::generatePolyLineFromCut( int _objectId, Vector _planePoint, Vector _planeNormal, int _polyLineId ) {
    
    // get object
    BaseObjectData *obj;
    PluginFunctions::getObject(_objectId, obj);
    
    if (obj == 0){
        emit log(LOGERR,tr("Unable to get object"));
        return -1;
    }
    
    //get the intersection points
    
    std::vector< ACG::Vec3d > linePoints;
    bool closed = false;
    if ( obj->dataType(DATA_TRIANGLE_MESH) ) {
        
        TriMesh* mesh = PluginFunctions::triMesh(obj);
        
        if ( mesh == 0 ) {
            emit log(LOGERR,tr("Unable to get mesh"));
            return -1;
        }
        
        //get an edge of the mesh that is cut by the plane
        TriMesh::EdgeHandle eh = getCuttedEdge( *mesh, _planeNormal, _planePoint );
        
        if ( !eh.is_valid() ) {
            emit log(LOGERR,tr("Unable to find initial cut edge."));
            return -1;
        }
        
        TriMesh::FaceHandle fh = mesh->face_handle( mesh->halfedge_handle(eh, 0) );
        
        if (!fh.is_valid())
            fh = mesh->face_handle( mesh->halfedge_handle(eh, 1) );
        
        // get all intersection points
        linePoints = getIntersectionPoints( mesh, fh.idx() , _planeNormal , _planePoint, closed);
        
    } else {
        
        PolyMesh* mesh = PluginFunctions::polyMesh(obj);
        
        if ( mesh == 0 ) {
            emit log(LOGERR,tr("Unable to get mesh"));
            return -1;
        }
        
        //get a edge of the mesh that is cut by the plane
        PolyMesh::EdgeHandle eh = getCuttedEdge( *mesh, _planeNormal, _planePoint );
        
        if ( !eh.is_valid() ) {
            emit log(LOGERR,tr("Unable to find initial cut edge."));
            return -1;
        }
        
        PolyMesh::FaceHandle fh = mesh->face_handle( mesh->halfedge_handle(eh, 0) );
        
        if (!fh.is_valid())
            fh = mesh->face_handle( mesh->halfedge_handle(eh, 1) );
        
        // get all intersection points
        linePoints = getIntersectionPoints( mesh, fh.idx() , _planeNormal , _planePoint, closed);
    }
    
    if ( linePoints.empty() )  {
        emit log(LOGERR,tr("No points from cut found."));
        return -1;
    }
    
    //generate a polyLine from the intersection Points
    int polyLineId = -1;
    
    // add new polyline
    if (_polyLineId == -1)
      emit addEmptyObject(DATA_POLY_LINE,polyLineId);
    else
      polyLineId = _polyLineId;
    
    // get current polylineobject
    BaseObjectData *polyLineObj;
    PluginFunctions::getObject(polyLineId, polyLineObj);
    
    // get polyline object
    PolyLineObject* currentPolyLine = PluginFunctions::polyLineObject(polyLineObj);
    
    currentPolyLine->line()->clear();
    
    for ( uint i = 0 ; i < linePoints.size(); ++i )
        currentPolyLine->line()->add_point( (PolyLine::Point) linePoints[i] );
    
    currentPolyLine->line()->set_closed(closed);
    currentPolyLine->target(true);

    return polyLineId;
}



/** \brief Generates polyLines from a plane intersection
*
*
* @param _objectId id of the target object
* @param _planePoint a point on the cut plane
* @param _planeNormal the normal of the cut plane
* @return returns the ids of the polyLine
*/
IdList PolyLinePlugin::generatePolyLinesFromCut( int _objectId, Vector _planePoint, Vector _planeNormal) {

  // List of generated lines
  std::vector<int> lines;


  // get object
  BaseObjectData *obj;
  PluginFunctions::getObject(_objectId, obj);

  if (obj == 0){
    emit log(LOGERR,tr("Unable to get object"));
    return lines;
  }

  //get the intersection points

  std::vector< std::vector< ACG::Vec3d > > linePoints;
  bool closed = false;
  if ( obj->dataType(DATA_TRIANGLE_MESH) ) {

    TriMesh* mesh = PluginFunctions::triMesh(obj);

    if ( mesh == 0 ) {
      emit log(LOGERR,tr("Unable to get mesh"));
      return lines;
    }

    // get all intersection points
    linePoints = getMultipleIntersectionPoints( mesh, _planeNormal , _planePoint);

  } else {

    PolyMesh* mesh = PluginFunctions::polyMesh(obj);

    if ( mesh == 0 ) {
      emit log(LOGERR,tr("Unable to get mesh"));
      return lines;
    }

    // get all intersection points
    linePoints = getMultipleIntersectionPoints( mesh, _planeNormal , _planePoint);
  }


  // No lines found?
  if ( linePoints.empty() )  {
    emit log(LOGERR,tr("No cut lines found."));
    return lines;
  }


  for ( unsigned int i = 0 ; i < linePoints.size(); ++i ) {

    if ( linePoints[i].empty() )
      continue;

    //generate a polyLine from the intersection Points
    int polyLineId = -1;

    // add new polyline
    emit addEmptyObject(DATA_POLY_LINE,polyLineId);

    // get current polylineobject
    BaseObjectData *polyLineObj;
    PluginFunctions::getObject(polyLineId, polyLineObj);

    // get polyline object
    PolyLineObject* currentPolyLine = PluginFunctions::polyLineObject(polyLineObj);

    currentPolyLine->line()->clear();

    for ( unsigned int j = 0 ; j < linePoints[i].size(); ++j )
      currentPolyLine->line()->add_point( (PolyLine::Point) linePoints[i][j] );

    currentPolyLine->line()->set_closed(closed);
    currentPolyLine->target(true);

    lines.push_back(polyLineId);

  }


  return lines;
}
