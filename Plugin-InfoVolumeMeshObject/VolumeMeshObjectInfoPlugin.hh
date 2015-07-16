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
*   $Revision: 16191 $                                                       *
*   $LastChangedBy: moebius $                                                *
*   $Date: 2013-01-24 15:35:53 +0100 (Thu, 24 Jan 2013) $                     *
*                                                                            *
\*===========================================================================*/

#ifndef INFOPLUGIN_HH
#define INFOPLUGIN_HH

//== INCLUDES =================================================================

#include <QObject>

#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>
#include <OpenFlipper/common/Types.hh>

//== CLASS DEFINITION =========================================================


/** \brief Plugin to visualize information about open volume meshs in the scene
 
  Plugin to visualize information about open volume meshs in the scene
*/
class InfoVolumeMeshObjectPlugin : public QObject, BaseInterface, LoggingInterface
{
  Q_OBJECT
      Q_INTERFACES(BaseInterface)
      Q_INTERFACES(LoggingInterface)

#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "org.OpenFlipper.Plugins.Plugin-VolumeMeshObjectInfo")
#endif

  signals:
    // BaseInterface
    void setSlotDescription(QString     _slotName,   QString     _slotDescription,
                            QStringList _parameters, QStringList _descriptions);

    // LoggingInterface
    void log(Logtype _type, QString _message);
    void log(QString _message);

  private slots :

    // BaseInterface
    void initializePlugin();
    void pluginsInitialized();
    void slotObjectUpdated( int _identifier, const UpdateType& _type );
    void slotObjectSelectionChanged( int _identifier );
    void slotAllCleared();
    
    void noguiSupported( ) {}

  public :

    // default constructor
    InfoVolumeMeshObjectPlugin();

    // default destructor
    ~InfoVolumeMeshObjectPlugin();

    /// Name of the Plugin
    QString name(){ return (QString("InfoVolumeMeshObject")); }

    /// Description of the Plugin
    QString description() { return (QString(tr("Provides Information on Volume Mesh Objects"))); }

  private :
    
    template< class MeshT >
    void printMeshInfo( MeshT* _mesh, int _id, unsigned int _face, ACG::Vec3d& _hitPoint );

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
    /// get total number of cells for a given object
    int cellCount(int _id);

  public slots:
    QString version() { return QString("0.1"); }

  private:
    /// set scripting slot descriptions
    void setDescriptions();
    
/** @} */

};

#endif //MOVEPLUGIN_HH

