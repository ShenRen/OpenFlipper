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

#ifndef MATERIALPICKER_HH
#define MATERIALPICKER_HH

#include <QObject>
#include <QString>

#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/MouseInterface.hh>
#include <OpenFlipper/BasePlugin/PickingInterface.hh>
#include <OpenFlipper/BasePlugin/ToolboxInterface.hh>
#include <OpenFlipper/BasePlugin/KeyInterface.hh>

#include <OpenFlipper/common/Types.hh>
#include <ACG/Utils/SmartPointer.hh>


class MaterialPicker : public QObject, BaseInterface, MouseInterface, PickingInterface, ToolboxInterface, KeyInterface
{
  Q_OBJECT
  Q_INTERFACES(BaseInterface)
  Q_INTERFACES(MouseInterface)
  Q_INTERFACES(PickingInterface)
  Q_INTERFACES(ToolboxInterface)
  Q_INTERFACES(KeyInterface)

#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "org.OpenFlipper.Plugins.Plugin-MaterialPicker")
#endif

signals:
  void addPickMode(const std::string &_mode);

  void updateView();

  void addToolbox( QString  _name  , QWidget* _widget, QIcon* _icon);

  void registerKey(int _key, Qt::KeyboardModifiers _modifiers, QString _description, bool _multiUse = false);

public slots:

  void slotMouseEvent(QMouseEvent* _event);

  void slotPickModeChanged(const std::string& _mode);

  void slotKeyEvent (QKeyEvent* _event);

public:

  MaterialPicker();
  ~MaterialPicker();

  QString name(){return QString("MaterialPicker");}
  QString description(){return QString("Pick to store and set materials.");}

private:

  struct MaterialInfo
  {
     bool color_material;
     ACG::Vec4f base_color;
     ACG::Vec4f ambient_color;
     ACG::Vec4f diffuse_color;
     ACG::Vec4f specular_color;
     float shininess;
     double reflectance;
     int key;
  };

  const std::string pickModeName_;
  const QString propName_;
  static const unsigned supportedKeys_ = 3;

  QPushButton* pickMaterialButton_;
  QPushButton* fillMaterialButton_;

  //all these lists are synchronized:
  QListWidget* materialListWidget_;
  QVector<MaterialInfo> materialList_;
  QStringList materialStrings_;//hold materials as a String (saves/load the material at the beginning)

  //key = hotKey, value = index for lists
  std::map<int,size_t> shortKeyRow_;

  ptr::shared_ptr<MaterialNode> materialNode_;

  /// stores the state of the pick material button
  bool pickMaterial_;
  /// stores the state of the fill material button
  bool fillMaterial_;

private:
  /// returns the item name with hotkey hint
  QString itemName(const QString &_name, int _key);

  /// returns the plain name of the material without hotkey hint
  QString plainName(const QString &string, int index);

private slots:

  void initializePlugin();

  void pluginsInitialized();

  void slotPickMaterialMode();

  void slotFillMaterialMode();

  void slotRemoveCurrentItem();

  /// items can be renamed by double clicking them
  void editMode(QListWidgetItem* _item);

  void editModeCurrent();

  /// saves the new material name with hotkey hint
  void saveNewName ( QWidget * _editor, QAbstractItemDelegate::EndEditHint _hint );
  void saveNewName (QListWidgetItem* _item);

  /// returns a formatted string for saving
  QString materialString(const MaterialInfo& _mat, const QString &_name);

  void clearList();

  void removeItem(QListWidgetItem* _item);

  /// creates context menu on current item (current is the item at mouse position)
  void createContextMenu(const QPoint& _point);

  /// change specified HotKey to current item
  void changeHotKey(const int _key);

  void slotMaterialProperties();

  void slotEnableListWidget(int _save);

  void slotMaterialChanged();

public slots:
  QString version(){ return QString("1.0"); }
};


#endif //MATERIALPICKER_HH
