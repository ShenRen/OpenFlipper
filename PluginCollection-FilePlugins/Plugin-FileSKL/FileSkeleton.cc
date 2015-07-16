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
*   $LastChangedBy$                                                 *
*   $Date$                    *
*                                                                            *
\*===========================================================================*/

#include "FileSkeleton.hh"
#include <ACG/GL/GLState.hh>

#include <OpenFlipper/BasePlugin/PluginFunctions.hh>
#include <OpenFlipper/common/GlobalOptions.hh>

#include <OpenMesh/Core/IO/IOManager.hh>

#if QT_VERSION >= 0x050000 
  #include <QtWidgets>
#else
  #include <QtGui>
#endif

#include <QFileInfo>



#include <iostream>
#include <fstream>


void FileSKLPlugin::initializePlugin() {
}

QString FileSKLPlugin::getLoadFilters() {
  return QString( "Skeleton files ( *.skl )" );
};

QString FileSKLPlugin::getSaveFilters() {
  return QString( "Skeleton files ( *.skl )" );
};

DataType  FileSKLPlugin::supportedType() {
  DataType type = DATA_SKELETON;
  return type;
}

template<typename Skeleton>
bool FileSKLPlugin::LoadSkeleton(Skeleton *_pSkeleton, QString _filename)
{
  typedef JointT<typename Skeleton::Point> Joint;
  typedef  PoseT<typename Skeleton::Point> Pose;
  typedef ACG::Matrix4x4T<typename Skeleton::Scalar> Matrix;

  unsigned int nJoints = 0;

  std::ifstream in(_filename.toStdString().c_str(), std::ofstream::in);

  // read number of joints
  in >> nJoints;

  Pose *ref = _pSkeleton->referencePose();
  // remember parent joints
  std::map<unsigned int, Joint*> parents;

  std::map<unsigned int, unsigned int> jointMap;

  for(unsigned int i = 0; i < nJoints; ++i)
  {
    // id not stored in skeleton; the read order is increasing for joints
    unsigned int id;

    // read joint id
    in >> id;

    // read its matrix
    Matrix mat;
    mat.identity();
    for(int y = 0; y < 3; ++y)
      for(int x = 0; x < 3; ++x)
        in >> mat(y, x);
    for(int y = 0; y < 3; ++y)
      in >> mat(y, 3);

    // try to find the parent joint
    Joint *pParent = 0;
    if(parents.find(id) != parents.end())
      pParent = parents[id];

    // setup the new joint
    Joint *pJoint = new Joint(pParent);
    _pSkeleton->addJoint(pParent, pJoint);

    jointMap[ id ] = pJoint->id();
    
    // save the joints position
    ref->setGlobalMatrix(jointMap[ id ], mat);

    // how many child nodes
    unsigned int nChildren;
    in >> nChildren;

    for(unsigned int j = 0; j < nChildren; ++j)
    {
      // remember to attach this child joint once its being load
      unsigned int idChild;
      in >> idChild;

      parents[idChild] = pJoint;
    }
  }
  
  unsigned int num_anim = 0;
  while(in.good()) {

      num_anim++;

      // Test whether animation name is provided
      std::string identifier;
      in >> identifier;
      std::string animationName = (QString("Animation") + QString::number(num_anim)).toStdString();

	  //read animation
      unsigned int frameCount = 0;
	  

      if(identifier == "animation") {
          std::getline(in, animationName);
          // Trim string
          animationName = QString(animationName.c_str()).trimmed().toStdString();

		  in >> frameCount;
      } else {

		  std::istringstream tmp(identifier);
		  tmp >> frameCount;

      }

      if ( frameCount > 0 ){

        FrameAnimationT<ACG::Vec3d>* animation = new FrameAnimationT<ACG::Vec3d>(_pSkeleton, frameCount);
        AnimationHandle animHandle = _pSkeleton->addAnimation(animationName, animation);

        for (unsigned int k = 0; k < frameCount; k++){

          animHandle.setFrame(k);
          typename Skeleton::Pose* pose = _pSkeleton->pose( animHandle );

          for(unsigned int i = 0; i < nJoints; ++i)
          {
            unsigned int id; // id not stored in skeleton; the read order is increasing for joints

            // read joint id
            in >> id;

            // read its matrix
            Matrix mat;
            mat.identity();
            for(int y = 0; y < 3; ++y)
              for(int x = 0; x < 3; ++x)
                in >> mat(y, x);
            for(int y = 0; y < 3; ++y)
              in >> mat(y, 3);

            //set matrix
            pose->setGlobalMatrix(jointMap[ id ], mat);
          }
        }
      }
  }

  in.close();

  return true;
}

int FileSKLPlugin::loadObject(QString _filename)
{
  int id = -1;
  emit addEmptyObject(DATA_SKELETON, id);
  
  BaseObjectData *obj(0);
  if(PluginFunctions::getObject(id, obj))
  {
    SkeletonObject* skel = PluginFunctions::skeletonObject(obj);

    LoadSkeleton(skel->skeleton(), _filename);

    //general stuff
    obj->source( PluginFunctions::objectCount() > 4 );
    obj->setFromFileName(_filename);
    obj->setName(obj->filename());
    emit updatedObject( obj->id(), UPDATE_ALL );
    emit openedFile( obj->id() );
  } else {
    emit log(LOGERR,tr("Unable to add empty skeleton"));
  }
    

  return id;
};

template<typename Skeleton>
bool FileSKLPlugin::SaveSkeleton(Skeleton *_pSkeleton, QString _filename)
{
  typedef JointT<typename Skeleton::Point> Joint;
  typedef  PoseT<typename Skeleton::Point>  Pose;
  typedef ACG::Matrix4x4T<typename Skeleton::Scalar> Matrix;

  std::ofstream out(_filename.toStdString().c_str(), std::ofstream::out);

  // write the number of joints
  out << _pSkeleton->jointCount() << std::endl;

  Pose *ref = _pSkeleton->referencePose();
  // write all the joints
  for (typename Skeleton::Iterator it = _pSkeleton->begin(); it != _pSkeleton->end(); ++it ){
  
    unsigned int i = (*it)->id();
    Joint *pJoint   = *it;

    // write this joints id
    out << pJoint->id() << " ";

    // write its position
    const Matrix &mat = ref->globalMatrix(i);
    for(int y = 0; y < 3; ++y)
      for(int x = 0; x < 3; ++x)
        out << mat(y, x) << " ";
    for(int y = 0; y < 3; ++y)
      out << mat(y, 3) << " ";

    // write this joints number of children
    out << pJoint->size() << " ";

    // write the children
    for(unsigned int j = 0; j < pJoint->size(); ++j)
      out << pJoint->child(j)->id() << " ";

    out << std::endl;
  }
  
  // now store animations
  AnimationT<ACG::Vec3d>* animation = 0;

  for (unsigned int i = 0; i < _pSkeleton->animationCount(); i++) {

      animation = _pSkeleton->animation(AnimationHandle(i, 0));

        if (animation != 0) {

            std::string name = animation->name();

            out << "animation " << name << std::endl;

            out << animation->frameCount() << std::endl;

            AnimationHandle animHandle = _pSkeleton->animationHandle(name);

            // every frame of that animation
            for (unsigned int k = 0; k < animation->frameCount(); ++k) {

                animHandle.setFrame(k);
                typename Skeleton::Pose* pose = _pSkeleton->pose(animHandle);

                for (typename Skeleton::Iterator it = _pSkeleton->begin(); it != _pSkeleton->end(); ++it) {

                    unsigned int i = (*it)->id();
                    Joint *pJoint = *it;

                    // write this joints id
                    out << pJoint->id() << " ";

                    // write its position
                    const Matrix &mat = pose->globalMatrix(i);
                    for (int y = 0; y < 3; ++y)
                        for (int x = 0; x < 3; ++x)
                            out << mat(y, x) << " ";
                    for (int y = 0; y < 3; ++y)
                        out << mat(y, 3) << " ";
                }

                out << std::endl;
            }
        } else {
            out << "0" << std::endl;
        }
    }

  out.close();
  return !out.fail();
}

bool FileSKLPlugin::saveObject(int _id, QString _filename)
{
	BaseObjectData *obj(0);
	if(PluginFunctions::getObject(_id, obj))
	{
		SkeletonObject *skel = PluginFunctions::skeletonObject(obj);
		if(skel)
		{
		  obj->setFromFileName(_filename);
		  obj->setName(obj->filename());
			SaveSkeleton(skel->skeleton(), _filename);
		}
	} else {
	  emit log(LOGERR, tr("saveObject : cannot get object id %1 for save name %2").arg(_id).arg(_filename) );
	  return false;
	}

	return true;
}

void FileSKLPlugin::loadIniFile( INIFile& _ini ,int _id ) {
  BaseObjectData* baseObject;
  if ( !PluginFunctions::getObject(_id,baseObject) ) {
    emit log(LOGERR,"Cannot find object for id " + QString::number(_id) + " in saveFile" );
    return;
  }

  if ( baseObject->materialNode() != 0 ) {
     ACG::Vec4f col(0.0,0.0,0.0,0.0);

    if ( _ini.get_entryVecf( col, baseObject->name() , "BaseColor" ) )
      baseObject->materialNode()->set_base_color(col);
  }

}

void FileSKLPlugin::saveIniFile( INIFile& _ini ,int _id) {
  BaseObjectData* baseObject;
  if ( !PluginFunctions::getObject(_id,baseObject) ) {
    emit log(LOGERR,"Cannot find object for id " + QString::number(_id) + " in saveFile" );
    return;
  }


  if ( baseObject->materialNode() != 0 ) {
      _ini.add_entryVec( baseObject->name() ,
                         "BaseColor" ,
                         baseObject->materialNode()->base_color() );
  }
}

#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2( filesklplugin , FileSKLPlugin );
#endif


