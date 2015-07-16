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
// CLASS InfoMeshObjectPlugin
//
// Author:  David Bommes <bommes@cs.rwth-aachen.de>
//=============================================================================

#ifndef INFOPLUGIN_HH
#define INFOPLUGIN_HH

//== INCLUDES =================================================================

#include <QObject>

#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/InformationInterface.hh>

#include <OpenFlipper/BasePlugin/StatusbarInterface.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>
#include <OpenFlipper/common/Types.hh>
#include <ObjectTypes/Skeleton/Skeleton.hh>

#include "infoDialog.hh"

//== CLASS DEFINITION =========================================================


/** \brief Plugin to visualize information about objects in the scene
 
  Plugin to visualize information about objects in the scene
*/
class InfoSkeletonObjectPlugin : public QObject, BaseInterface, InformationInterface, LoggingInterface, StatusbarInterface
{
  Q_OBJECT
      Q_INTERFACES(BaseInterface)
      Q_INTERFACES(InformationInterface)
      Q_INTERFACES(LoggingInterface)
      Q_INTERFACES(StatusbarInterface)

#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "org.OpenFlipper.Plugins.Plugin-SkeletonObjectInfo")
#endif


  signals:

    // BaseInterface
    void setSlotDescription(QString     _slotName,   QString     _slotDescription,
                            QStringList _parameters, QStringList _descriptions);

    // LoggingInterface
    void log(Logtype _type, QString _message);
    void log(QString _message);

    // StatusbarInterface
    void addWidgetToStatusbar(QWidget* _widget);

  private slots :

    // BaseInterface
    void initializePlugin();
    void pluginsInitialized();
    
    void noguiSupported( ) {} ;

    // InformationInterface
    void slotInformationRequested(const QPoint _clickedPoint, DataType _type);
    DataType supportedDataTypes();

  public :

  // default constructor
    InfoSkeletonObjectPlugin(): info_(0) {};

  // default destructor
      ~InfoSkeletonObjectPlugin() {};

      /// Name of the Plugin
      QString name(){ return (QString("InfoSkeletonObject")); };

      /// Description of the Plugin
      QString description() { return (QString(tr("Provides Information on Skeleton Objects"))); };

  private :
    void printSkeletonInfo( Skeleton* _id,   unsigned int _objectId, unsigned int _index, ACG::Vec3d& _hitPoint );

    InfoDialog* info_;

  public slots:
  		/// get total number of joints for a given skeleton
      int jointCount(int _skeletonID);
      /// get total number of branches for a given skeleton
      int branchCount(int _skeletonID);
      /// get total number of leaves for a given skeleton
      int leafCount(int _skeletonID);

      /// get minumum bounding box point
      Vector boundingBoxMin(int _skeletonID);
      /// get maximum bounding box point
      Vector boundingBoxMax(int _skeletonID);
      /// get the size of the bounding box
      Vector boundingBoxSize(int _skeletonID);

      /// get the length of a bone
      double boneLength(int _skeletonID, int _jointID);

      /// get the minimal bone length
      double minBoneLength(int _skeletonID);
      /// get the maximal bone length
      double maxBoneLength(int _skeletonID);
      /// get the mean bone length
      double meanBoneLength(int _skeletonID);

  private:
     /// set scripting slot descriptions
     void setDescriptions();

  public slots:
    QString version() { return QString("1.0"); };

/** @} */

};

#endif //MOVEPLUGIN_HH

