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
#include <OpenFlipper/BasePlugin/LoadSaveInterface.hh>
#include <OpenFlipper/BasePlugin/StatusbarInterface.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>
#include <OpenFlipper/common/Types.hh>
#include <ObjectTypes/PolyMesh/PolyMesh.hh>
#include <ObjectTypes/TriangleMesh/TriangleMesh.hh>

#include "infoDialog.hh"
#include "infoBar.hh"

//== CLASS DEFINITION =========================================================


/** \brief Plugin to visualize information about objects in the scene
 
  Plugin to visualize information about objects in the scene
*/
class InfoMeshObjectPlugin : public QObject, BaseInterface, InformationInterface, LoggingInterface, StatusbarInterface, LoadSaveInterface
{
  Q_OBJECT
      Q_INTERFACES(BaseInterface)
      Q_INTERFACES(InformationInterface)
      Q_INTERFACES(LoggingInterface)
      Q_INTERFACES(StatusbarInterface)
      Q_INTERFACES(LoadSaveInterface)

#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "org.OpenFlipper.Plugins.Plugin-MeshObjectInfo")
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
    void slotObjectUpdated( int _identifier, const UpdateType& _type );
    void slotObjectSelectionChanged( int _identifier );
    void slotAllCleared();
    
    // LoadSaveInterface
    void objectDeleted( int _identifier );

    void noguiSupported( ) {} ;

    // InformationInterface
    void slotInformationRequested(const QPoint _clickedPoint, DataType _type);
    DataType supportedDataTypes();

    void slotShowHistogram();

  public :

    // default constructor
    InfoMeshObjectPlugin();

    // default destructor
    ~InfoMeshObjectPlugin();

    /// Name of the Plugin
    QString name(){ return (QString("InfoMeshObject")); };

    /// Description of the Plugin
    QString description() { return (QString(tr("Provides Information on Mesh Objects"))); };

  private :
    InfoDialog* info_;

    InfoBar* infoBar_;
    
    template< class MeshT >
    void printMeshInfo( MeshT* _mesh, int _id, unsigned int _face, ACG::Vec3d& _hitPoint );

    /// Slot that updates the visualization
    void updateData( int _identifier , const UpdateType& _type, const bool deleted);

//===========================================================================
/** @name Scripting Functions
  * @{ */
//===========================================================================

  public slots:

    /// get total number of vertices for a given object
    int vertexCount(int _id);
    /// get total number of edges for a given object
    int edgeCount(int _id);
    /// get total number of faces for a given object
    int faceCount(int _id);
    /// get the number of boundaries for a given object
    int boundaryCount(int _id);
    /// get the number of components for a given object
    int componentCount(int _id);

    /// get the genus of the given object
    int genus(int _id);
    /// get the center of gravity
    Vector cog(int _id);
    /// get minumum bounding box point
    Vector boundingBoxMin(int _id);
    /// get maximum bounding box point
    Vector boundingBoxMax(int _id);
    /// get the size of the bounding box
    Vector boundingBoxSize(int _id);

    /// get the length of an edge
    double edgeLength(int _id, int _edgeHandle);
    /// get the area of a face
    double faceArea  (int _id, int _faceHandle);
    /// get the aspect ratio of a face
    double aspectRatio(int _id, int _faceHandle);
    /// get vertex valence
    int vertexValence  (int _id, int _vertexHandle);

    /// get the minimal edge length
    double minEdgeLength(int _id);
    /// get the maximal edge length
    double maxEdgeLength(int _id);
    /// get the mean edge length
    double meanEdgeLength(int _id);

  public slots:
    QString version() { return QString("1.0"); };

  private:
    /// set scripting slot descriptions
    void setDescriptions();
    
    /// Get closest vertex index from a face
    template< class MeshT >
    int getClosestVertexInFace(MeshT* _mesh, int _face_idx, ACG::Vec3d& _hitPoint);
    
    /// Get closest edge index from a face
    template< class MeshT >
    int getClosestEdgeInFace(MeshT* _mesh, int _face_idx, const ACG::Vec3d& _hitPoint);

    /// Get closest vertex index from an edge
    template< class MeshT >
    int getClosestVertexFromEdge(MeshT* _mesh, int _edge_idx, ACG::Vec3d& _hitPoint);

    /// Get edge lengths
    template< class MeshT >
    void getEdgeLengths(MeshT* _mesh, double &min, double &max, double &mean);

    /// Get edge lengths
    bool getEdgeLengths(int _id, double &min, double &max, double &mean);
    
/** @} */

  private:
    BaseObjectData *lastPickedObject_;
    int lastPickedObjectId_;
};

#endif //MOVEPLUGIN_HH

