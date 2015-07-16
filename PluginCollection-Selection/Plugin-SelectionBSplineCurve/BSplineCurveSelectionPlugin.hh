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

/*===========================================================================*\
 *                                                                           *
 *   $Revision$                                                       *
 *   $Author$                                                       *
 *   $Date$                   *
 *                                                                           *
\*===========================================================================*/

#ifndef BSPLINECURVESELECTIONPLUGIN_HH
#define BSPLINECURVESELECTIONPLUGIN_HH

#include <QObject>

#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/KeyInterface.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>
#include <OpenFlipper/BasePlugin/ToolboxInterface.hh>
#include <OpenFlipper/BasePlugin/ToolbarInterface.hh>
#include <OpenFlipper/BasePlugin/LoadSaveInterface.hh>
#include <OpenFlipper/BasePlugin/BackupInterface.hh>
#include <OpenFlipper/BasePlugin/INIInterface.hh>
#include <OpenFlipper/BasePlugin/ScriptInterface.hh>
#include <OpenFlipper/BasePlugin/SelectionInterface.hh>
#include <OpenFlipper/common/Types.hh>
#include <OpenFlipper/BasePlugin/PluginFunctions.hh>
#include <OpenFlipper/INIFile/INIFile.hh>

#include <ObjectTypes/BSplineCurve/BSplineCurve.hh>

class BSplineCurveSelectionPlugin : public QObject, BaseInterface, KeyInterface,
            INIInterface, BackupInterface, LoggingInterface, ScriptInterface, SelectionInterface
{
    Q_OBJECT
    Q_INTERFACES(BaseInterface)
    Q_INTERFACES(KeyInterface)
    Q_INTERFACES(INIInterface)
    Q_INTERFACES(BackupInterface)
    Q_INTERFACES(LoggingInterface)
    Q_INTERFACES(ScriptInterface)    
    Q_INTERFACES(SelectionInterface)

#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "org.OpenFlipper.Plugins.Plugin-SelectionBSplineCurve")
#endif

public:
    
    /// Default constructor
    BSplineCurveSelectionPlugin();

    /// Default destructor
    ~BSplineCurveSelectionPlugin();
    
signals:

    // BaseInterface
    void updateView();
    void updatedObject(int, const UpdateType&);
    void nodeVisibilityChanged(int _identifier);
    void setSlotDescription(QString     _slotName,   QString     _slotDescription,
                            QStringList _parameters, QStringList _descriptions);

    // BackupInterface
    void createBackup( int _objectid, QString _name, UpdateType _type = UPDATE_ALL);

    // LoggingInterface
    void log(Logtype _type, QString _message);
    void log(QString _message);

    // ScriptInterface
    void scriptInfo(QString _functionName);

    // SelectionInterface
    void addSelectionEnvironment(QString _modeName, QString _description, QString _icon, QString& _handleName);
    void registerType(QString _handleName, DataType _type);
    void addPrimitiveType(QString _handleName, QString _name, QString _icon, SelectionInterface::PrimitiveType& _typeHandle);
    void addSelectionOperations(QString _handleName, QStringList _operationsList, QString _category, SelectionInterface::PrimitiveType _type = 0u);
    void showToggleSelectionMode(QString _handleName, bool _show, SelectionInterface::PrimitiveType _associatedTypes);
    void showVolumeLassoSelectionMode(QString _handleName, bool _show, SelectionInterface::PrimitiveType _associatedTypes);

    void getActiveDataTypes(SelectionInterface::TypeList& _types);
    void getActivePrimitiveType(SelectionInterface::PrimitiveType& _type);
    void targetObjectsOnly(bool& _targetsOnly);
    
    void registerKeyShortcut(int _key, Qt::KeyboardModifiers _modifiers = Qt::NoModifier);

private slots:

    // INIInterface
    void loadIniFile(INIFile& _ini, int _id);
    void saveIniFile(INIFile& _ini, int _id);

    // BaseInterface
    void initializePlugin();
    void pluginsInitialized();
    void noguiSupported() {};

    // SelectionInterface
    void slotSelectionOperation(QString _operation);
    void slotToggleSelection(QMouseEvent* _event, SelectionInterface::PrimitiveType _currentType, bool _deselect);
    void slotVolumeLassoSelection(QMouseEvent* _event, SelectionInterface::PrimitiveType _currentType, bool _deselect);

    void slotLoadSelection(const INIFile& _file);
    void slotSaveSelection(INIFile& _file);
    
    void slotKeyShortcutEvent(int _key, Qt::KeyboardModifiers _modifiers);

public slots:

    // SelectionInterface
    void loadSelection(int _objId, const QString& _filename);

public:

    // BaseInterface
    QString name() {
        return (QString(tr("BSplineCurve Selection")));
    };
    
    QString description() {
        return (QString(tr("Allows to select B-Spline curves")));
    };

    //===========================================================================
    /** @name Private methods
    * @{ */
    //===========================================================================
private:
    
    enum SelectionViewMode {CP, K};
    
    /// Change selection view mode for every B-spline curve in the scene
    void setSelectionViewMode(const SelectionViewMode _mode);
    
    /// Set slot descriptions for scripting functions
    void updateSlotDescriptions();
    
    /** @} */
    
public slots:

    QString version() {
        return QString("1.0");
    };

    //===========================================================================
    /** @name Scriptable slots
    * @{ */
    //===========================================================================
    
    // Control point selection:
    
    /// Select all control points of a curve
    void selectAllControlPoints(int _objectId);
    
    /// Deselect all control points of a curve
    void deselectAllControlPoints(int _objectId);
    
    /// Invert control point selection
    void invertControlPointSelection(int _objectId);
    
    /// Delete selected control points
    void deleteSelectedControlPoints(int _objectId);
    
    /// Select specific control points of a curve
    void selectControlPoints(int _objectId, const IdList& _ids, bool _deselect = false);
    
    /// Get current control point selection
    IdList getControlPointSelection(int _objectId);
    
    // Knot selection:
    
    /// Select all knots of a curve
    void selectAllKnots(int _objectId);
    
    /// Deselect all knots of a curve
    void deselectAllKnots(int _objectId);
    
    /// Invert knot selection
    void invertKnotSelection(int _objectId);
    
    /// Delete selected knots
    void deleteSelectedKnots(int _objectId);
    
    /// Select specific knots of a curve
    void selectKnots(int _objectId, const IdList& _ids, bool _deselect = false);
    
    /// Get current knot selection
    IdList getKnotSelection(int _objectId);
    
    /** @} */

    //===========================================================================
    /** @name Member variables
    * @{ */
    //===========================================================================

private:

    /// Handle to selection environment
    QString environmentHandle_;
   
    /// Primitive type handles:
    unsigned int controlPointType_;
    unsigned int knotType_;
    
    unsigned int allSupportedTypes_;
    
    /// Keep volume lasso points
    QVector<QPoint> volumeLassoPoints_;

    /** @} */

};

//=============================================================================
#endif // BSPLINECURVESELECTIONPLUGIN_HH defined
//=============================================================================
