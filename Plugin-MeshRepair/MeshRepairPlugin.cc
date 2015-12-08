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


#include "MeshRepairPlugin.hh"

#if QT_VERSION >= 0x050000
#else
#include <QtGui>
#endif

//-----------------------------------------------------------------------------

MeshRepairPlugin::MeshRepairPlugin() :
tool_(0),
toolIcon_(0)
{

}
//-----------------------------------------------------------------------------

void
MeshRepairPlugin::
initializePlugin()
{
  tool_ = new MeshRepairToolbarWidget();
  QSize size(300, 300);
  tool_->resize(size);

  //==================
  // Vertex operations
  //==================

  connect(tool_->valenceThreeButton, SIGNAL(clicked()), this, SLOT(slotDetectFlatValence3Vertices()) );
  connect(tool_->repairRemoveVButton, SIGNAL(clicked()), this, SLOT(slotRemoveSelectedVal3Vertices()) );

  //==================
  // Edge operations
  //==================

  connect(tool_->detectEShorterButton, SIGNAL(clicked()), this, SLOT(slotDetectEdgesShorter()) );
  connect(tool_->detectELargerButton, SIGNAL(clicked()), this, SLOT(slotDetectEdgesLonger()) );
  connect(tool_->repairCollapseEButton, SIGNAL(clicked()), this, SLOT(slotRemoveSelectedEdges()) );
  connect(tool_->detectCapAngle, SIGNAL(clicked()), this, SLOT(slotDetectSkinnyTriangleByAngle()) );
  connect(tool_->repairFlipEButton, SIGNAL(clicked()), this, SLOT(slotRemoveSkinnyTriangleByAngle()) );
  connect(tool_->detectFoldoverButton, SIGNAL(clicked()), this, SLOT(slotDetectFoldover()) );

  //==================
  // Face operations
  //==================
  connect(tool_->triangleAspectButton,SIGNAL(clicked()),this,SLOT(slotDetectTriangleAspect()));
  connect(tool_->flipOrientation,SIGNAL(clicked()),this,SLOT(slotFlipOrientation()));

  //==================
  // Normal operations
  //==================
  connect(tool_->computeNormals,SIGNAL(clicked()),this,SLOT(slotUpdateNormals()));
  connect(tool_->computeVertexNormals,SIGNAL(clicked()),this,SLOT(slotUpdateVertexNormals()));
  connect(tool_->computeFaceNormals,SIGNAL(clicked()),this,SLOT(slotUpdateFaceNormals()));
  connect(tool_->computeHalfedgeNormals,SIGNAL(clicked()),this,SLOT(slotUpdateHalfedgeNormals()));

  //==================
  // General
  //==================
  connect(tool_->snapBoundaryButton, SIGNAL(clicked()), this, SLOT(slotSnapBoundary()) );
  connect(tool_->fixNonManifoldVerticesButton,SIGNAL(clicked()),this,SLOT(slotFixNonManifoldVertices()));
  connect(tool_->fixMeshButton,SIGNAL(clicked()),this,SLOT(slotFixMesh()));


  toolIcon_ = new QIcon(OpenFlipper::Options::iconDirStr()+OpenFlipper::Options::dirSeparator()+"meshrepair-toolbox.png");
  tool_->repairCollapseEButton->setIcon(*toolIcon_);
  tool_->repairFlipEButton->setIcon(*toolIcon_);
  tool_->repairRemoveVButton->setIcon(*toolIcon_);

  emit addToolbox( tr("Mesh Repair") , tool_,  toolIcon_);
}

//===========================================================================
// Button Slots
//===========================================================================

void MeshRepairPlugin::slotRemoveSelectedVal3Vertices() {

  for ( PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS,DataType( DATA_TRIANGLE_MESH ) );  o_it != PluginFunctions::objectsEnd(); ++o_it)
    removeSelectedVal3Vertices(o_it->id());

  emit updateView();
}

//-----------------------------------------------------------------------------

void MeshRepairPlugin::slotRemoveSelectedEdges(){

  for ( PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS,DataType( DATA_TRIANGLE_MESH ) );  o_it != PluginFunctions::objectsEnd(); ++o_it) 
    removeSelectedEdges(o_it->id());

  emit updateView();
}

//-----------------------------------------------------------------------------

void MeshRepairPlugin::slotDetectSkinnyTriangleByAngle()
{
  for ( PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS,DataType( DATA_TRIANGLE_MESH | DATA_POLY_MESH ) );  o_it != PluginFunctions::objectsEnd(); ++o_it)
    detectSkinnyTriangleByAngle( o_it->id(), tool_->capAngleSpinbox->value(), false );

  emit updateView();
}

//-----------------------------------------------------------------------------

void MeshRepairPlugin::slotRemoveSkinnyTriangleByAngle()
{
  //rewrite!!!
  for ( PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS,DataType( DATA_TRIANGLE_MESH | DATA_POLY_MESH ) );  o_it != PluginFunctions::objectsEnd(); ++o_it)
    detectSkinnyTriangleByAngle( o_it->id(), tool_->capAngleSpinbox->value(), true );

  emit updateView();
}

//-----------------------------------------------------------------------------

void MeshRepairPlugin::slotDetectFoldover() {

  for ( PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS,DataType( DATA_TRIANGLE_MESH | DATA_POLY_MESH ) );  o_it != PluginFunctions::objectsEnd(); ++o_it)
    detectFoldover(o_it->id(), tool_->detectFoldoverSpinbox->value());

  emit updateView();
}

//-----------------------------------------------------------------------------

void MeshRepairPlugin::slotDetectTriangleAspect() {

  for ( PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS,DataType( DATA_TRIANGLE_MESH | DATA_POLY_MESH ) );  o_it != PluginFunctions::objectsEnd(); ++o_it)
    detectTriangleAspect(o_it->id(), tool_->triangleAspectSpinbox->value());

  emit updateView();
}

//-----------------------------------------------------------------------------

void MeshRepairPlugin::slotFlipOrientation(){

  for ( PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS,DataType( DATA_TRIANGLE_MESH | DATA_POLY_MESH ) );  o_it != PluginFunctions::objectsEnd(); ++o_it)
    flipOrientation(o_it->id());

  emit updateView();
}

//-----------------------------------------------------------------------------

void MeshRepairPlugin::slotFixMesh() {


  for ( PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS,DataType( DATA_TRIANGLE_MESH ) );  o_it != PluginFunctions::objectsEnd(); ++o_it)
    fixMesh(o_it->id() , tool_->fixMeshBox->value() );

  emit updateView();

}

//-----------------------------------------------------------------------------

void MeshRepairPlugin::slotUpdateVertexNormals() {
  for ( PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS,DataType( DATA_TRIANGLE_MESH | DATA_POLY_MESH ) );  o_it != PluginFunctions::objectsEnd(); ++o_it)
    updateVertexNormals(o_it->id());

  emit updateView();
}

//-----------------------------------------------------------------------------

void MeshRepairPlugin::slotUpdateFaceNormals() {
  for ( PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS,DataType( DATA_TRIANGLE_MESH | DATA_POLY_MESH ) );  o_it != PluginFunctions::objectsEnd(); ++o_it)
    updateFaceNormals(o_it->id());

  emit updateView();
}

//-----------------------------------------------------------------------------

void MeshRepairPlugin::slotUpdateHalfedgeNormals() {
  for ( PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS,DataType( DATA_TRIANGLE_MESH | DATA_POLY_MESH ) );  o_it != PluginFunctions::objectsEnd(); ++o_it)
    updateHalfedgeNormals(o_it->id());

  emit updateView();
}

//-----------------------------------------------------------------------------

void MeshRepairPlugin::slotUpdateNormals(){
  for ( PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS,DataType( DATA_TRIANGLE_MESH | DATA_POLY_MESH ) );  o_it != PluginFunctions::objectsEnd(); ++o_it)
    updateNormals(o_it->id());

  emit updateView();
}

//-----------------------------------------------------------------------------

void MeshRepairPlugin::slotDetectEdgesShorter(){
  double length = tool_->edgeSpin->value();

  for ( PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS,DataType( DATA_TRIANGLE_MESH | DATA_POLY_MESH ) );  o_it != PluginFunctions::objectsEnd(); ++o_it)
    selectEdgesShorterThan(o_it->id(),length);

  emit updateView();
}

//-----------------------------------------------------------------------------

void MeshRepairPlugin::slotDetectEdgesLonger(){
  double length = tool_->edgeSpin->value();

  for ( PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS,DataType( DATA_TRIANGLE_MESH | DATA_POLY_MESH ) );  o_it != PluginFunctions::objectsEnd(); ++o_it)
    selectEdgesLongerThan(o_it->id(),length);

  emit updateView();
}

//-----------------------------------------------------------------------------

void MeshRepairPlugin::slotDetectFlatValence3Vertices() {
  double angle = tool_->valenceThreeSpinbox->value();

  for ( PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS,DataType( DATA_TRIANGLE_MESH | DATA_POLY_MESH ) );  o_it != PluginFunctions::objectsEnd(); ++o_it)
    detectFlatValence3Vertices(o_it->id(), angle);

  emit updateView();
}

//-----------------------------------------------------------------------------

void MeshRepairPlugin::slotSnapBoundary()
{
  double eps = tool_->snapBoundarySpinBox->value();
  for ( PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS,DataType( DATA_TRIANGLE_MESH | DATA_POLY_MESH ) );  o_it != PluginFunctions::objectsEnd(); ++o_it)
      snapBoundary(o_it->id(), eps);
  emit updateView();
}

//-----------------------------------------------------------------------------

void MeshRepairPlugin::slotFixNonManifoldVertices()
{
  for ( PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS,DataType( DATA_TRIANGLE_MESH | DATA_POLY_MESH ) );  o_it != PluginFunctions::objectsEnd(); ++o_it)
    fixNonManifoldVertices(o_it->id());
  emit updateView();
}


//-----------------------------------------------------------------------------

/** \brief Initialization of the plugin when it is loaded by the core
 *
 */
void MeshRepairPlugin::pluginsInitialized() {

  // ===============================
  // Vertex Operations
  // ===============================

  emit setSlotDescription("removeSelectedVal3Vertices(int)",tr("Remove all selected valence 3 vertices"),
                          QStringList(tr("objectId")),
                          QStringList(tr("ID of an object")));

  emit setSlotDescription("detectFlatValence3Vertices(int,double)",tr("Selects all vertices that have valence 3 and the normals of their neighboring faces have an angle less then the given angle"),
                          QString(tr("objectId,angle")).split(","),
                          QString(tr("ID of an object;the maximal angle between the adjacent faces")).split(";"));

  // ===============================
  // Edge Operations
  // ===============================

  emit setSlotDescription("selectEdgesShorterThan(int,double)",tr("Selects all edges of an object which are shorter than the given length"),
                          QString(tr("objectId,length")).split(","),
                          QString(tr("ID of an object;All edges shorter than this length will be selected")).split(";"));

  emit setSlotDescription("selectEdgesLongerThan(int,double)",tr("Selects all edges of an object which are longer than the given length"),
                          QString(tr("objectId,length")).split(","),
                          QString(tr("ID of an object;All edges longer than this length will be selected")).split(";"));

  emit setSlotDescription("removeSelectedEdges(int)",tr("Remove the selected edges"),
                          QStringList(tr("objectId")),
                          QStringList(tr("ID of an object")));

  emit setSlotDescription("detectSkinnyTriangleByAngle(int,double,bool)",tr("Select or remove skinny triangles (determined by a minimum angle threshold)."),
                          QString(tr("objectId,angle,remove")).split(","),
                          QString(tr("ID of an object;Minimum angle threshold;Remove")).split(";"));

  emit setSlotDescription("detectFoldover(int,float)",tr("Selects edges that are incident to folded over faces."),
                          QString(tr("objectId,angle")).split(","),
                          QString(tr("ID of an object;Minimum threshold angle for fold-overs")).split(";"));

  // ===============================
  // Face Operations
  // ===============================

  emit setSlotDescription("detectTriangleAspect(int,float)",tr("Selects all faces that have a larger aspect ratio than the given one."),
                           QString(tr("objectId,aspect")).split(","),
                           QString(tr("ID of an object;The minimal aspect ratio to select")).split(";"));

  emit setSlotDescription("flipOrientation(int)",tr("Flips the normals of all faces by changing the vertex order in each face"),
                           QStringList(tr("objectId")),
                           QStringList(tr("ID of an object")));


  // ===============================
  // Normal Fixing
  // ===============================

  emit setSlotDescription("updateFaceNormals(int)",tr("Recompute Face normals"),
                          QStringList(tr("objectId")),
                          QStringList(tr("ID of an object")));

  emit setSlotDescription("updateHalfedgeNormals(int)",tr("Recompute Halfedge normals"),
                          QStringList(tr("objectId")),
                          QStringList(tr("ID of an object")));

  emit setSlotDescription("updateVertexNormals(int)",tr("Recompute Vertex normals"),
                          QStringList(tr("objectId")),
                          QStringList(tr("ID of an object")));

  emit setSlotDescription("updateNormals(int)",tr("Recompute Face and Vertex normals"),
                          QStringList(tr("objectId")),
                          QStringList(tr("ID of an object")));


  // ===============================
  // General Mesh fixing
  // ===============================

  emit setSlotDescription("snapBoundary(int,double)",tr("Snaps selected boundary vertices if the distance is less than the given maximal distance."),
                           QString(tr("objectId,epsilon")).split(","),
                           QString(tr("ID of an object;Max Distance")).split(";"));

  emit setSlotDescription("(int)",tr("Fixes non manifold vertices."),
                            QString(tr("objectId")).split(","),
                            QString(tr("ID of an object;Non manifold vertices are splitted.")).split(";"));

  emit setSlotDescription("fixMesh(int,double)",tr("Fixes a mesh."),
                          QString(tr("objectId,distance")).split(","),
                          QString(tr("ID of an object;Vertices with distance lower than epsilon will be treated as one.")).split(";"));

}

#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2( meshrepairplugin , MeshRepairPlugin );
#endif


