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




#ifndef MESHCOMPAREPLUGIN_HH
#define MESHCOMPAREPLUGIN_HH

#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/ToolboxInterface.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>
#include <OpenFlipper/BasePlugin/ScriptInterface.hh>
#include <OpenFlipper/BasePlugin/BackupInterface.hh>
#include <OpenFlipper/BasePlugin/TextureInterface.hh>
#include <OpenFlipper/BasePlugin/RPCInterface.hh>
#include <OpenFlipper/common/Types.hh>

#include <ACG/Scenegraph/PointNode.hh>

#include "MeshCompareToolbarWidget.hh"

#ifdef WITH_QWT
  #include "QwtFunctionPlot.hh"
#endif

class MeshComparePlugin : public QObject, BaseInterface, ToolboxInterface, LoggingInterface, ScriptInterface, BackupInterface, TextureInterface, RPCInterface
{
  Q_OBJECT
  Q_INTERFACES(BaseInterface)
  Q_INTERFACES(ToolboxInterface)
  Q_INTERFACES(LoggingInterface)
  Q_INTERFACES(ScriptInterface)
  Q_INTERFACES(BackupInterface)
  Q_INTERFACES(TextureInterface)
  Q_INTERFACES(RPCInterface)

#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "org.OpenFlipper.Plugins.Plugin-MeshCompare")
#endif

  signals:
    //BaseInterface
    void updateView();
    void updatedObject(int _id, const UpdateType& _type);
    void setSlotDescription(QString     _slotName,   QString     _slotDescription,
                            QStringList _parameters, QStringList _descriptions);

    //LoggingInterface
    void log(Logtype _type, QString _message);
    void log(QString _message);
    
    // ToolboxInterface
    void addToolbox( QString  _name  , QWidget* _widget ,QIcon* _icon);
    
    // BackupInterface
    void createBackup( int _id , QString _name, UpdateType _type = UPDATE_ALL );

    // RPC Interface
    void pluginExists( QString _pluginName , bool& _exists  );

  public:

    MeshComparePlugin();
    ~MeshComparePlugin();

    // BaseInterface
    QString name() { return (QString("Mesh Compare")); };
    QString description( ) { return (QString("Compare two meshes")); };

  private slots:

    void initializePlugin();
    void pluginsInitialized();

    /// Triggers comparison of the selected meshes
    void compareButton();

    /// Clears the visualization
    void slotClear();

    /// Called when an object gets updated
    void slotObjectUpdated( int _identifier, const UpdateType& _type );

    void noguiSupported( ) {} ;

  // Scriptable functions
  public slots:
       
    QString version() { return QString("1.0"); };

    /** Compares two meshes. Use the two getter functions to retrieve the maximal deviations.
     *
     * The source will be the reference. The difference will be rendered as points on the target mesh.
     *
     * @param _sourceId Id of the reference mesh
     * @param _targetId Id of the mesh that should be compared to the reference mesh.
     * @param _computeDist   Compute distance between meshes
     * @param _computeNormal Compute normal deviation between meshes
     * @param _computeGauss  Compute gauss curvature deviation between meshes
     * @param _computeMean   Compute mean curvature deviation between meshes
     */
    void compare(int _sourceId,int _targetId,
                 bool _computeDist   = true,
                 bool _computeNormal = true,
                 bool _computeGauss  = true ,
                 bool _computeMean   = true);

    /// Get the maximal distance of the last comparison (-1, if no comparison performed so far)
    double lastMaximalDistance() { return maximalDistance_; };

    /// Get the maximal normal deviation of the last comparison in degree (-1, if no comparison performed so far)
    double lastMaximalNormalDeviation() { return maxNormalDeviation_; };

    /// Get the maximal mean curvature deviation of the last comparison (-1, if no comparison performed so far)
    double lastMaximalMeanCurvatureDeviation() { return maxMeanCurvatureDev_; };

    /// Get the maximal gauss curvature deviation of the last comparison (-1, if no comparison performed so far)
    double lastMaximalGaussCurvatureDeviation() { return maxGaussCurvatureDev_; };

  private slots:

    /// If the checkbox is changed to be checked, the values in the labels will be written into the spin boxes.
    void slotClampBox(bool _checked);

  private:

    /** \brief Visualize data
     *
     *  This function visualizes the data via rendering points and pushes it to the histogram.
     *
     * @param _data     Vector of data points
     * @param _maxValue Maximal value found in the data
     * @param _pnode    The point node that will be used to visualize the data
     */
    void visualizeData( const std::vector<double>& _data, double _maxValue, ACG::SceneGraph::PointNode* _pnode);

  private:
    /// The toolbar widget of this plugin
    MeshCompareToolbarWidget* tool_;

    /// Last maximal computed distance
    double maximalDistance_;

    /// Last maximal computed normal deviation in degree
    double maxNormalDeviation_;

    /// Last maximal mean curvature deviation
    double maxMeanCurvatureDev_;

    /// Last maximal gauss curvature deviation
    double maxGaussCurvatureDev_;

    #ifdef WITH_QWT
      QwtFunctionPlot* plot_;
    #endif

};

#endif //MESHCOMPAREPLUGIN_HH
