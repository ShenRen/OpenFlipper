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

#ifndef COLORPLUGIN_HH
#define COLORPLUGIN_HH

#include <OpenFlipper/common/Types.hh>
#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/MenuInterface.hh>
#include <OpenFlipper/BasePlugin/ScriptInterface.hh>
#include <OpenFlipper/BasePlugin/ToolbarInterface.hh>
#include "OpenFlipper/BasePlugin/StatusbarInterface.hh"
#include "OpenFlipper/BasePlugin/OptionsInterface.hh"
#include "OpenFlipper/BasePlugin/INIInterface.hh"

#include <QObject>
#include <QMenuBar>
#include "ColorPlugin.hh"

#include "ColorOptions.hh"

#include "OpenFlipper/INIFile/INIFile.hh"

class ColorPlugin : public QObject, BaseInterface, MenuInterface, ScriptInterface, ToolbarInterface, StatusbarInterface, OptionsInterface, INIInterface
{
  
  Q_OBJECT

  Q_INTERFACES(BaseInterface)
  Q_INTERFACES(MenuInterface)
  Q_INTERFACES(ScriptInterface)
  Q_INTERFACES(ToolbarInterface)
  Q_INTERFACES(StatusbarInterface)
  Q_INTERFACES(OptionsInterface)
  Q_INTERFACES(INIInterface)

#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "org.OpenFlipper.Plugins.Plugin-Color")
#endif

  signals:
    // BaseInterface
    void updateView();

    // ScriptInterface
    void scriptInfo( QString _functionName );

    // MenuInterface
    void addMenubarAction(QAction* _action, QString _type );

    // ToolbarInterface
    void addToolbar(QToolBar* _toolbar);
    void removeToolbar(QToolBar* _toolbar);

    // StatusbarInterface
    void showStatusMessage(QString _message, int _timeout = 0);

  private slots:

    // BaseInterface
    void pluginsInitialized();

    // OptionsInterface
    void applyOptions();

    // INIInterface
    void loadIniFileOptions(INIFile& _ini);
    void saveIniFileOptions(INIFile& _ini);

  public :

    // OptionsInterface
    bool initializeOptionsWidget(QWidget*& _widget);

    // BaseInterface
    QString name() { return (QString("Color Plugin")); };
    QString description( ) { return (tr("Sets the Default colors e.g. for slides, paper,... snapshots")); };

    /// ConstructorOptionsInterface
    ColorPlugin();


  //===========================================================================
  /** @name Show Colors in Options and Store them in INI files
    * @{ */
  //===========================================================================

  private:
    /// the options widget to set colors
    ColorOptions* optionsWidget_;

    /// generate an icon from a given color
    QIcon generateIcon(QRgb _color);

    /// generate an icon from a given color
    QIcon generateIcon(OpenMesh::Vec4f _color);

    /// get the color from a given icon
    QRgb getIconColor(QIcon _icon);

    /// set the color of a color vector to the one in the given icon
    void setNewColor(QIcon _icon, OpenMesh::Vec4f& _color);

    /// convert betweeen uint and Vec4f
    void convertColor(uint _color, OpenMesh::Vec4f& _vector);

    /// convert betweeen uint and Vec4f
    uint convertColor(OpenMesh::Vec4f& _vector);

    //internal storage of colors
    OpenMesh::Vec4f backgroundColorDefault_, backgroundColorPaper_, backgroundColorPresentation_;
    OpenMesh::Vec4f baseColorDefault_, baseColorPaper_, baseColorPresentation_;
    OpenMesh::Vec4f materialColorDefault_, materialColorPaper_, materialColorPresentation_;

    double lineWidthDefault_, lineWidthPaper_, lineWidthPresentation_;

  private slots:
    /// display a color dialog
    void getColorDialog();

  /** @} */

  //===========================================================================
  /** @name Scriptable functions to set colors
    * @{ */
  //===========================================================================

  public slots:
    /**
     * Applies Default colors to the whole scene
     */
    void setDefaultColor();

    /**
     * Applies colors for presentations to the scene
     */
    void setPresentationColor();

    /**
     * Applies colors for papers
     */
    void setPaperColor();

    /**
     * Sets the background color of the scene
    */
    void setBackgroundColor( Vector _color );

    /**
     * Sets the emissive color of an object
     */
    void setObjectEmissiveColor(int _objectId , Vector4 _color);

    /**
     * Sets the ambient color of an object
     */
    void setObjectAmbientColor(int _objectId , Vector4 _color);

    /**
     * Sets the diffuse color of an object
     */
    void setObjectDiffuseColor(int _objectId , Vector4 _color);

    /**
     * Sets the specular color of an object
     */
    void setObjectSpecularColor(int _objectId , Vector4 _color);

  /** @} */

  //===========================================================================
  /** @name Other scriptable functions
   * @{ */
  //===========================================================================

  public slots:

    /**
     * Version info of the color plugin
     */
    QString version() { return QString("1.0"); };

  /** @} */
};

#endif //COLORPLUGIN_HH
