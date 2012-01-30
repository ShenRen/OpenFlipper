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
*   $Revision$                                                       *
*   $LastChangedBy$                                                *
*   $Date$                     *
*                                                                            *
\*===========================================================================*/



#include "MeshComparePlugin.hh"


#include <ObjectTypes/PolyMesh/PolyMesh.hh>
#include <ObjectTypes/TriangleMesh/TriangleMesh.hh>

#include "OpenFlipper/BasePlugin/PluginFunctions.hh"

#include <OpenFlipper/common/bsp/TriangleBSPT.hh>
#include <ACG/Geometry/Algorithms.hh>
#include <ACG/Scenegraph/PointNode.hh>
#include <ACG/Utils/ColorCoder.hh>

MeshComparePlugin::MeshComparePlugin() :
  tool_(0),
  maximalDistance_(-1),
  maxNormalDeviation_(-1)
{

}

MeshComparePlugin::~MeshComparePlugin()
{

}

void MeshComparePlugin::initializePlugin()
{
  if ( OpenFlipper::Options::gui()) {
    tool_ = new MeshCompareToolbarWidget();

    connect( tool_->compare, SIGNAL(clicked()), this, SLOT(compareButton()) );

    QIcon* toolIcon = new QIcon(OpenFlipper::Options::iconDirStr()+OpenFlipper::Options::dirSeparator()+"MeshCompare.png");
    emit addToolbox( tr("Mesh Compare") , tool_ , toolIcon);
  }
}

void MeshComparePlugin::pluginsInitialized() {
    
  emit setSlotDescription(tr("compare(int,int)"), tr("Compare two meshes. Use lastMaximalDistance() and lastMaximalNormalDeviation() to get the results."),
      QStringList(tr("ObjectId,ObjectId")), QStringList(tr("Id of the reference mesh, Id of the comparison mesh")));
  emit setSlotDescription(tr("lastMaximalDistance()"), tr("Get the maximal distance between the meshes of the last comparison."),
      QStringList(tr("")), QStringList(tr("")));
  emit setSlotDescription(tr("lastMaximalNormalDeviation()"), tr("Get the maximal normal deviation in degree between the meshes of the last comparison."),
      QStringList(tr("")), QStringList(tr("")));

}

void MeshComparePlugin::compareButton() {

  // Get source and target objects
  BaseObjectData* targetObject = 0;
  for ( PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS) ; o_it != PluginFunctions::objectsEnd(); ++o_it) {

    if ( o_it->dataType(DATA_TRIANGLE_MESH)  || o_it->dataType(DATA_POLY_MESH) ) {

      // If we found a second target, something is wrong!
      if ( targetObject != 0 ) {
        emit log(LOGERR,tr("Please select one source and one target mesh to compare! Source will be the reference mesh."));
        return;
      }

      targetObject = (*o_it);
    }
  }

  BaseObjectData* sourceObject = 0;
  for ( PluginFunctions::ObjectIterator o_it(PluginFunctions::SOURCE_OBJECTS) ; o_it != PluginFunctions::objectsEnd(); ++o_it) {

    if ( o_it->dataType(DATA_TRIANGLE_MESH)  || o_it->dataType(DATA_POLY_MESH) ) {

      // If we found a second target, something is wrong!
      if ( sourceObject != 0 ) {
        emit log(LOGERR,tr("Please select one source and one target mesh to compare! Source will be the reference mesh."));
        return;
      }

      sourceObject = (*o_it);
    }
  }


  if ( (targetObject != 0) && (sourceObject != 0)  ) {
    compare(sourceObject->id(),targetObject->id());
  } else {
    emit log(LOGERR,tr("Please select one source and one target mesh to compare! Source will be the reference mesh."));
  }


}

void MeshComparePlugin::compare(int _sourceId,int _targetId) {


  TriMeshObject* source = PluginFunctions::triMeshObject(_sourceId);
  TriMeshObject* target = PluginFunctions::triMeshObject(_targetId);

  if ( (target == 0 ) || (source == 0) ) {
    emit log(LOGERR,tr("Please select one source and one target mesh to compare! Source will be the reference mesh."));
    emit log(LOGERR,tr("Only triangle meshes are currently supported!"));
    return;
  }

  TriMesh* refMesh  = PluginFunctions::triMesh(_sourceId);
  TriMesh* compMesh = PluginFunctions::triMesh(_targetId);

  ACG::SceneGraph::PointNode *pNode = 0;


  // Check if we already attached a PointNode
  if ( OpenFlipper::Options::gui() ) {

    if ( !target->getAdditionalNode(pNode,name(), "MeshCompareDistance" ) ) {

      ACG::SceneGraph::MaterialNode *pMatNode = 0;
      pMatNode = new ACG::SceneGraph::MaterialNode(target->manipulatorNode(), "MeshCompare distance visualization material");
      target->addAdditionalNode(pMatNode, name(), "MeshCompareDistanceMaterial");
      pMatNode->set_point_size(5);
      pMatNode->applyProperties(ACG::SceneGraph::MaterialNode::PointSize);

      pNode = new ACG::SceneGraph::PointNode(pMatNode, "MeshCompare distance visualization");
      pNode->drawMode(ACG::SceneGraph::DrawModes::POINTS_COLORED);
      target->addAdditionalNode(pNode, name(), "MeshCompareDistance");
    }

    // Clear but reserve memory for the required vertices
    pNode->clear();
    pNode->reserve(refMesh->n_vertices(),refMesh->n_vertices(),refMesh->n_vertices() );
  }


  // Get a bsp for the target, as we will project the reference mesh onto the target mesh.
  // It will be build automatically at this point.
  TriMeshObject::OMTriangleBSP* compBSP = target->requestTriangleBsp();

  // Remember the maximal values as output and for specifying color coding range
  maximalDistance_    = 0.0;
  maxNormalDeviation_ = 0.0;


  // Remember distances for colorCoding after we know the maximal distance
  std::vector<double> distances;
  std::vector<double> normalAngles;

  for ( TriMesh::VertexIter v_it = refMesh->vertices_begin() ; v_it != refMesh->vertices_end(); ++ v_it) {

    TriMeshObject::OMTriangleBSP::NearestNeighbor nearest = compBSP->nearest( refMesh->point(v_it) );
    TriMesh::FaceHandle closestFace = nearest.handle;

    // Remember the maximal distance between the meshes
    if ( nearest.dist > maximalDistance_ )
      maximalDistance_ = nearest.dist;

    // Remember distance for color coding
    distances.push_back(nearest.dist);

    // Get the vertices around that face and their properties
    TriMesh::CFVIter        fv_it = compMesh->cfv_iter(closestFace);

    const TriMesh::Point&   p0    = compMesh->point(fv_it);
    const TriMesh::Normal   n0    = compMesh->normal(fv_it);

    const TriMesh::Point&   p1    = compMesh->point(++fv_it);
    const TriMesh::Normal   n1    = compMesh->normal(fv_it);

    const TriMesh::Point&   p2    = compMesh->point(++fv_it);
    const TriMesh::Normal   n2    = compMesh->normal(fv_it);

    // project original point to current mesh
    TriMesh::Point projectedPoint;
    ACG::Geometry::distPointTriangle(refMesh->point(v_it), p0, p1, p2, projectedPoint);

    // Add the position to the point node
    if (pNode)
      pNode->add_point(projectedPoint);

    // compute Barycentric coordinates
    ACG::Geometry::baryCoord(projectedPoint, p0, p1, p2, projectedPoint);

    // interpolate normal on the compare mesh at the projected point via barycentric coordinates.
    TriMesh::Normal normal;
    normal  = n0 * projectedPoint[0];
    normal += n1 * projectedPoint[1];
    normal += n2 * projectedPoint[2];
    normal.normalize();

    // Compute normal deviation in degrees
    double normalDeviation = (refMesh->normal(v_it) | normal);

    if (normalDeviation < -1.0)
      normalDeviation = -1.0;
    else if (normalDeviation > 1.0)
      normalDeviation = 1.0;

    normalDeviation = 180.0 / M_PI * acos(normalDeviation);

    // Remember normal deviation for color coding
    normalAngles.push_back(normalDeviation);

    if (normalDeviation > maxNormalDeviation_)
      maxNormalDeviation_ = normalDeviation;

  }

  // Generate the colors
  if ( pNode ) {

    if ( tool_->distance->isChecked() ) {
      ACG::ColorCoder cCoder(0,maximalDistance_);

      for ( unsigned int i = 0 ; i < distances.size() ; ++i) {
        pNode->add_color(cCoder.color_float4(distances[i]));
      }
    } else {
      ACG::ColorCoder cCoder(0,maxNormalDeviation_);

      for ( unsigned int i = 0 ; i < normalAngles.size() ; ++i) {
        pNode->add_color(cCoder.color_float4(normalAngles[i]));
      }
    }

    emit updateView();
  }

}


Q_EXPORT_PLUGIN2( meshcompareplugin , MeshComparePlugin );
