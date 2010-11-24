/*===========================================================================*\
*                                                                            *
*                              OpenFlipper                                   *
*      Copyright (C) 2001-2010 by Computer Graphics Group, RWTH Aachen       *
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


//=============================================================================
//
//  CLASS DecimaterPlugin - IMPLEMENTATION
//
//=============================================================================


//== INCLUDES =================================================================

#include <QtGui>

#include "DecimaterPlugin.hh"

#include <iostream>
#include <ACG/GL/GLState.hh>
#include <QStringList>
#include <ACG/Scenegraph/ManipulatorNode.hh>

#include <OpenFlipper/BasePlugin/PluginFunctions.hh>

#define DECIMATER "DecimaterData"

//== IMPLEMENTATION ==========================================================

void DecimaterPlugin::initializePlugin()
{
  tool_ = new DecimaterToolbarWidget();
  QSize size(100, 100);
  tool_->resize(size);

  // connect signals->slots
  connect(tool_->pbDecimate,SIGNAL(clicked() ),this,SLOT(slot_decimate()));

  connect(tool_->roundness,SIGNAL(valueChanged(double) ),this,SLOT(updateRoundness(double)) );
  connect(tool_->roundnessSlider,SIGNAL(valueChanged(int) ),this,SLOT(updateRoundness(int)) );
  connect(tool_->distance,SIGNAL(valueChanged(double) ),this,SLOT(updateDistance()) );
  connect(tool_->normalDeviation,SIGNAL(valueChanged(int) ),this,SLOT(updateNormalDev()) );
  connect(tool_->normalDeviationSlider,SIGNAL(valueChanged(int) ),this,SLOT(updateNormalDev()) );
  connect(tool_->verticesCount,SIGNAL(valueChanged(int) ),this,SLOT(updateVertices()) );
  connect(tool_->verticesCountSlider,SIGNAL(valueChanged(int) ),this,SLOT(updateVertices()) );

  // Force update if the Toolbox gets visible
  connect(tool_, SIGNAL(showing()), this, SLOT( slotUpdateNumVertices() ) );

  emit addToolbox( tr("Decimater") , tool_ );
}

/** \brief Initialization of the plugin when it is loaded by the core
 * 
 */
void DecimaterPlugin::pluginsInitialized() {

  emit setSlotDescription("decimate(int,QVariantMap)",tr("Decimate a given object"),
                          QString(tr("objectId,constraints")).split(","),
                          QString(tr("ID of an object; Object that can has one or more constraint properties (distance,normal_deviation,roundness,vertices)")).split(";"));
}


//-----------------------------------------------------------------------------

/** \brief sync between values of roundness slider and spinbox in the toolbox
 *
 * @param _value new roundness value
 */
void DecimaterPlugin::updateRoundness(int _value)
{
  tool_->roundness->setValue( (double) _value / 100.0 );
  tool_->cbRoundness->setChecked (true);
}


//-----------------------------------------------------------------------------

/** \brief sync between values of roundness slider and spinbox in the toolbox
 *
 * @param _value new roundness value
 */
void DecimaterPlugin::updateRoundness(double _value)
{
  tool_->roundnessSlider->setValue( (int) (_value * 100) );
  tool_->cbRoundness->setChecked (true);
}

//-----------------------------------------------------------------------------

/** \brief Decimation called by toolbox
 *
 */
void DecimaterPlugin::slot_decimate()
{
  
  for ( PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS,DATA_TRIANGLE_MESH) ;
                                        o_it != PluginFunctions::objectsEnd(); ++o_it)  {

    //initialize
    TriMeshObject* object = PluginFunctions::triMeshObject(*o_it);

    if ( object == 0 )
      emit log(LOGWARN , tr("Unable to get object"));

    DecimaterInfo* decimater = dynamic_cast< DecimaterInfo* > ( o_it->objectData(DECIMATER) );

    TriMesh* mesh = PluginFunctions::triMesh(*o_it);
  
    if (decimater == 0){
      decimater = new DecimaterInfo();
      o_it->setObjectData(DECIMATER, decimater);
    }
    
    // Constraint handles for decimation
    ModNormalFlippingH     hModNormalFlipping;
    ModQuadricH            hModDistance;
    ModRoundnessH          hModRoundness;
    ModQuadricH            hModPriorityQuadric;
    
    // Create decimater
    DecimaterType decimater_object( *mesh );
    decimater_object.add( hModPriorityQuadric );
    decimater_object.module( hModPriorityQuadric ).unset_max_err();
    
    // Remove old constraints
    if(decimater->distance()) {
        decimater->removeDistanceConstraint();
        decimater_object.remove(hModDistance);
    }
    if(decimater->normalDeviation()) {
        decimater->removeNormalDeviationConstraint();
        decimater_object.remove(hModNormalFlipping);
    }
    if(decimater->roundness()) {
        decimater->removeRoundnessConstraint();
        decimater_object.remove(hModRoundness);
    }

    //and set new constraints
    if ( tool_->cbDistance->isChecked() ) {
      if (  decimater_object.add( hModDistance ) ) {
          decimater->setDistanceConstraint( tool_->distance->value() );
          decimater_object.module( hModDistance ).set_max_err( decimater->distanceValue() );
      }
    }

    if ( tool_->cbNormalDev->isChecked() ) {
      if (  decimater_object.add( hModNormalFlipping ) ) {
          decimater->setNormalDeviationConstraint( tool_->normalDeviation->value() );
          decimater_object.module( hModNormalFlipping ).set_normal_deviation( decimater->normalDeviationValue() );
      }
    }

    if ( tool_->cbRoundness->isChecked() ) {      
      if (  decimater_object.add( hModRoundness ) ) {
          decimater->setRoundnessConstraint( tool_->roundness->value() );
          decimater_object.module( hModRoundness ).set_min_roundness( decimater->roundnessValue(), true );
      }
    }

    // Initialize the decimater
    if( ! decimater_object.initialize() ){
      emit log(LOGWARN, tr("Decimater could not be initialized"));
      continue;
    }

    //decimate
    if ( tool_->cbVertices->isChecked() )
        decimater_object.decimate_to(tool_->verticesCount->value());
    else
        decimater_object.decimate();

    object->mesh()->garbage_collection();
    object->mesh()->update_normals();
    object->update();
    
    // Create backup
    emit createBackup(o_it->id(), "Decimation");
    emit updatedObject( o_it->id() , UPDATE_TOPOLOGY );
  }

  emit updateView();
}


//-----------------------------------------------------------------------------

/** \brief Decimation called by Scripting
 *
 * @param _objID id of an object
 * @param _constraints A string containing a comma separated list of constraints (distance,normal_deviation,roundness,triangles)
 * @param _values a string containing a comma separated list of constraint values suited to the _constraints parameter
 */
void DecimaterPlugin::decimate(int _objID, QVariantMap _constraints) {


  BaseObjectData* baseObjectData;
  if ( ! PluginFunctions::getObject(_objID,baseObjectData) ) {
    emit log(LOGERR,tr("Unable to get Object"));
    return;
  }

  if ( baseObjectData->dataType() == DATA_TRIANGLE_MESH ) {
    TriMeshObject* object = PluginFunctions::triMeshObject(baseObjectData);

    if ( object == 0 ) {
      emit log(LOGWARN , tr("Unable to get object ( Only Triangle Meshes supported)"));
      return;
    }

    DecimaterInfo* decimater = dynamic_cast< DecimaterInfo* > ( object->objectData(DECIMATER) );

    TriMesh* mesh = PluginFunctions::triMesh(baseObjectData);
    
    if (decimater == 0){
      decimater = new DecimaterInfo();
      object->setObjectData(DECIMATER, decimater);
    }
    
    // Constraint handles for decimation
    ModNormalFlippingH     hModNormalFlipping;
    ModQuadricH            hModDistance;
    ModRoundnessH          hModRoundness;
    ModQuadricH            hModPriorityQuadric;
    
    // Create decimater
    DecimaterType decimater_object( *mesh );
    decimater_object.add( hModPriorityQuadric );
    decimater_object.module( hModPriorityQuadric ).unset_max_err();

    // Remove old constraints
    if(decimater->distance()) {
        decimater->removeDistanceConstraint();
        decimater_object.remove(hModDistance);
    }
    if(decimater->normalDeviation()) {
        decimater->removeNormalDeviationConstraint();
        decimater_object.remove(hModNormalFlipping);
    }
    if(decimater->roundness()) {
        decimater->removeRoundnessConstraint();
        decimater_object.remove(hModRoundness);
    }

    //distance constraint
    if ( _constraints.contains("distance") ){

      bool ok;

      double value = _constraints["distance"].toDouble(&ok);

      if (ok) {
        if (  decimater_object.add( hModDistance ) ) {
            decimater->setDistanceConstraint( value );
            decimater_object.module( hModDistance ).set_max_err( decimater->distanceValue() );
        }
      }
    }

    //normal deviation constraint
    if ( _constraints.contains("normal_deviation") ){

      bool ok;

      int value = _constraints["normal_deviation"].toInt(&ok);

      if (ok) {
        if (  decimater_object.add( hModNormalFlipping ) ) {
            decimater->setNormalDeviationConstraint( value );
            decimater_object.module( hModNormalFlipping ).set_normal_deviation( decimater->normalDeviationValue() );
        }
      }
    }

    //roundness constraint
    if ( _constraints.contains("roundness") ){

      bool ok;

      double value = _constraints["roundness"].toDouble(&ok);

      if (ok) {
        if (  decimater_object.add( hModRoundness ) ) {
            decimater->setRoundnessConstraint( value );
            decimater_object.module( hModRoundness ).set_min_roundness( decimater->roundnessValue(), true );
        }
      }
    }

    //triangleCount constraint
    bool triangleCount = false;
    int triangles = 0;

    if ( _constraints.contains("vertices") ){

      bool ok;

      int value = _constraints["vertices"].toInt(&ok);

      if (ok){
        triangleCount = true;
        triangles = value;
      }
    }

    //init the decimater
    if( ! decimater_object.initialize() ){
      emit log(LOGWARN, tr("Decimater could not be initialized"));
      return;
    }

    //decimate
    if ( triangleCount )
      decimater_object.decimate_to( triangles ); // do decimation
    else
      decimater_object.decimate(); // do decimation

    object->mesh()->garbage_collection();
    object->mesh()->update_normals();
    object->update();

    // Create backup
    emit createBackup(_objID, "Decimation");
    
    // Create QVariantMap parameter string
    QString param = "("  + (_constraints.contains("distance") ? tr("distance = %1").arg(_constraints["distance"].toString()) : "") +
                    ", " + (_constraints.contains("normal_deviation") ? tr("normal_deviation = %1").arg(_constraints["normal_deviation"].toString()) : "") +
                    ", " + (_constraints.contains("roundness") ? tr("roundness = %1").arg(_constraints["roundness"].toString()) : "") +
                    ", " + (_constraints.contains("vertices") ? tr("vertices = %1").arg(_constraints["vertices"].toString()) : "") + ")";
    
    emit scriptInfo( "decimate(" + QString::number(_objID) + ", " + param + ")" );
    
    emit updatedObject( baseObjectData->id() , UPDATE_TOPOLOGY);

  } else {
    emit log(LOGERR,tr("Unsupported object type for decimater"));
    return;
  }

  emit updateView();
}

//-----------------------------------------------------------------------------

void DecimaterPlugin::slotUpdateNumVertices()
{
  // Only update if tool is visible
  if ( !tool_->isVisible() ) {
    return;
  }

  int max = 0;
  int div = 0;

  bool ok;
  emit functionExists( "info" , "vertexCount(int)", ok  ) ;
  if (!ok)
  {
    tool_->currentNumVertices->setText ("<not available>");
    return;
  }

  for ( PluginFunctions::ObjectIterator o_it(PluginFunctions::TARGET_OBJECTS,DataType(DATA_TRIANGLE_MESH)) ;
                                        o_it != PluginFunctions::objectsEnd(); ++o_it)  {


    max = std::max( RPC::callFunctionValue<int>   ("info" , "vertexCount",o_it->id()) , max );
    div++;
  }

  if (div <= 0)
    tool_->currentNumVertices->setText ("<not available>");
  else {
    tool_->currentNumVertices->setText (QString::number(max));
    tool_->verticesCount->setMaximum(max);
    tool_->verticesCountSlider->setMaximum(max);
    
    if ( tool_->verticesCount->value() < 2 )
      tool_->verticesCount->setValue( max / 2 );
  }
}

//-----------------------------------------------------------------------------

void DecimaterPlugin::slotObjectSelectionChanged(int /*_identifier*/)
{
  slotUpdateNumVertices ();
}

//-----------------------------------------------------------------------------

void DecimaterPlugin::slotObjectUpdated(int /*_identifier*/ , const UpdateType _type )
{
  if ( _type.contains(UPDATE_TOPOLOGY) )
    slotUpdateNumVertices ();
}

//-----------------------------------------------------------------------------

// activate checkbox if value has changed
void DecimaterPlugin::updateVertices()
{
  tool_->cbVertices->setChecked (true);
}

//-----------------------------------------------------------------------------

// activate checkbox if value has changed
void DecimaterPlugin::updateNormalDev()
{
  tool_->cbNormalDev->setChecked (true);
}

//-----------------------------------------------------------------------------

// activate checkbox if value has changed
void DecimaterPlugin::updateDistance()
{
  tool_->cbDistance->setChecked (true);
}

//-----------------------------------------------------------------------------

Q_EXPORT_PLUGIN2(DecimaterPlugin , DecimaterPlugin );

