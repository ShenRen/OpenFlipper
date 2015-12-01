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
//  CLASS InfoMeshObjectPlugin - IMPLEMENTATION
//
//=============================================================================


//== INCLUDES =================================================================


#include "SkeletonObjectInfoPlugin.hh"

#include <OpenFlipper/BasePlugin/PluginFunctions.hh>
#include <MeshTools/MeshInfoT.hh>
#include <ACG/Geometry/Algorithms.hh>

#if QT_VERSION >= 0x050000
#else
#include <QtGui>
#endif

//== IMPLEMENTATION ==========================================================

void InfoSkeletonObjectPlugin::initializePlugin() {

  if ( OpenFlipper::Options::gui()) {
    // Create info dialog
    info_ = new InfoDialog();
  }

}

/// initialize the plugin
void InfoSkeletonObjectPlugin::pluginsInitialized() {
	//set the slot descriptions
	setDescriptions();

}

//-----------------------------------------------------------------------------

DataType InfoSkeletonObjectPlugin::supportedDataTypes() {
    return DataType(DATA_SKELETON);
}

//-----------------------------------------------------------------------------

void InfoSkeletonObjectPlugin::printSkeletonInfo( Skeleton* _skeleton,  unsigned int _objectId, unsigned int _index, ACG::Vec3d& _hitPoint ) {

  QLocale locale;
  QString name;

  // name
  BaseObject* obj = 0;
  if ( PluginFunctions::getObject(_objectId, obj) )
    info_->generalBox->setTitle( tr("General object information for %1").arg( obj->name() ) );



  // ID
  info_->id->setText( locale.toString(_objectId) );
  // Joints
  info_->joints->setText( locale.toString( _skeleton->jointCount() ) );


  // animation list with animation names and the frame count
  info_->comboBoxAnimations->clear();
  QString animationInfo;
  unsigned int aniCount = _skeleton->animationCount();

  for (unsigned int i = 0; i < aniCount; ++i) {
    std::string aniName = _skeleton->animationName(i);
    animationInfo = "Name: " + QString(aniName.c_str())
                     + " : Frames: " + locale.toString(_skeleton->animation(aniName)->frameCount());
    info_->comboBoxAnimations->addItem(animationInfo);
  }


  // Clicked:
  info_->jointHandle->setText( locale.toString( _index  ) );

  QString adjacentHandles;

  // Check if we have a parent joint
  if ( _skeleton->joint(_index)->parent() !=0 ) {
    adjacentHandles = adjacentHandles + "Parent: " + locale.toString( _skeleton->joint(_index)->parent()->id()  ) + " ;";
  }

  // Check for children
  if ( _skeleton->joint(_index)->size() != 0 ) {

    adjacentHandles = adjacentHandles + "Children:";

    for ( Skeleton::Joint::ChildIter it = _skeleton->joint(_index)->begin(); it != _skeleton->joint(_index)->end(); ++it) {
        Skeleton::Joint *joint = *it;

        adjacentHandles = adjacentHandles + " " + locale.toString(joint->id());
      }
  }



  info_->adjacentJointsHandles->setText(adjacentHandles);


  Skeleton::Point bbMin( FLT_MAX,  FLT_MAX,  FLT_MAX);
  Skeleton::Point bbMax(-FLT_MAX, -FLT_MAX, -FLT_MAX);
  Skeleton::Point cog(0.0,0.0,0.0);

  Skeleton::Pose* pose = _skeleton->referencePose();
  for (Skeleton::Iterator it = _skeleton->begin(); it != _skeleton->end(); ++it) {
    Skeleton::Joint *joint = *it;

    Skeleton::Point p = pose->globalTranslation(joint->id());

    cog += p;

    bbMin.minimize(p);
    bbMax.maximize(p);

  }

  //Bounding Box Size
  Skeleton::Point diff = bbMax-bbMin;

  info_->bbMinX->setText( QString::number(bbMin[0],'f') );
  info_->bbMinY->setText( QString::number(bbMin[1],'f') );
  info_->bbMinZ->setText( QString::number(bbMin[2],'f') );

  info_->bbMaxX->setText( QString::number(bbMax[0],'f') );
  info_->bbMaxY->setText( QString::number(bbMax[1],'f') );
  info_->bbMaxZ->setText( QString::number(bbMax[2],'f') );

  info_->bbSizeX->setText( QString::number(diff[0],'f') );
  info_->bbSizeY->setText( QString::number(diff[1],'f') );
  info_->bbSizeZ->setText( QString::number(diff[2],'f') );


  //COG
  cog = cog / _skeleton->jointCount() ;

  info_->cogX->setText( QString::number(cog[0],'f') );
  info_->cogY->setText( QString::number(cog[1],'f') );
  info_->cogZ->setText( QString::number(cog[2],'f') );

  //hitpoint
  info_->pointX->setText( QString::number( _hitPoint[0],'f' ) );
  info_->pointY->setText( QString::number( _hitPoint[1],'f' ) );
  info_->pointZ->setText( QString::number( _hitPoint[2],'f' ) );

  info_->setWindowFlags(info_->windowFlags() | Qt::WindowStaysOnTopHint);

  info_->show();
}

//----------------------------------------------------------------------------------------------

void InfoSkeletonObjectPlugin::slotInformationRequested(const QPoint _clickedPoint, DataType _type) {

    // Only respond on skeleton objects
    if( _type != DATA_SKELETON ) return;

    ACG::SceneGraph::PickTarget target = ACG::SceneGraph::PICK_ANYTHING;

    unsigned int   node_idx, target_idx;
    ACG::Vec3d     hit_point;

    if (PluginFunctions::scenegraphPick(target, _clickedPoint, node_idx, target_idx, &hit_point)) {
      BaseObjectData* object;

      if ( PluginFunctions::getPickedObject(node_idx, object) ) {

         emit log( LOGINFO , object->getObjectinfo() );

         if ( object->picked(node_idx) && object->dataType(DATA_SKELETON) )
            printSkeletonInfo( PluginFunctions::skeleton(object) , object->id(), target_idx, hit_point );

      } else return;
    }
}

#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2( infoskeletonobjectplugin , InfoSkeletonObjectPlugin );
#endif

