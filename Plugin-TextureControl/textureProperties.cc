//=============================================================================
//
//                               OpenFlipper
//        Copyright (C) 2008 by Computer Graphics Group, RWTH Aachen
//                           www.openflipper.org
//
//-----------------------------------------------------------------------------
//
//                                License
//
//  OpenFlipper is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  OpenFlipper is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with OpenFlipper.  If not, see <http://www.gnu.org/licenses/>.
//
//-----------------------------------------------------------------------------
//
//   $Revision$
//   $Author$
//   $Date$
//
//=============================================================================




#include "textureProperties.hh"

#include <float.h>

#include <QtGui>
#include <OpenFlipper/common/GlobalOptions.hh>

texturePropertiesWidget::texturePropertiesWidget(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    connect(buttonBox, SIGNAL( clicked(QAbstractButton*) ), this , SLOT ( slotButtonBoxClicked(QAbstractButton*) ) );
    connect(textureList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(textureChanged(QListWidgetItem*)) );
    connect(textureList, SIGNAL(itemPressed(QListWidgetItem*)), this, SLOT(textureAboutToChange(QListWidgetItem*)) );

    //remember changes
    connect(repeatBox, SIGNAL( clicked() ), this , SLOT ( slotPropertiesChanged() ) );
    connect(clampBox, SIGNAL( clicked() ), this , SLOT ( slotPropertiesChanged() ) );
    connect(centerBox, SIGNAL( clicked() ), this , SLOT ( slotPropertiesChanged() ) );
    connect(absBox, SIGNAL( clicked() ), this , SLOT ( slotPropertiesChanged() ) );
    connect(scaleBox, SIGNAL( clicked() ), this , SLOT ( slotPropertiesChanged() ) );

    connect(max_val, SIGNAL( valueChanged(double) ), this , SLOT ( slotPropertiesChanged(double) ) );
    connect(clamp_min, SIGNAL( valueChanged(double) ), this , SLOT ( slotPropertiesChanged(double) ) );
    connect(clamp_max, SIGNAL( valueChanged(double) ), this , SLOT ( slotPropertiesChanged(double) ) );

    connect(changeImageButton, SIGNAL( clicked() ), this, SLOT( slotChangeImage() ) );


    texData_ = 0;

    #ifdef WITH_QWT
      QGridLayout* layout = new QGridLayout( originalData);

      functionPlot_ = new ACG::QwtFunctionPlot(0);

      layout->addWidget( functionPlot_ , 0,0  );

    #endif

}

void texturePropertiesWidget::show(TextureData* _texData, int _id, QString _name){

  texData_ = _texData;
  id_ = _id;

  textureList->clear();

  for (uint i=0; i < texData_->textures().size(); i++)
    textureList->addItem( texData_->textures()[i].name() );

  if ( textureList->count() == 0){
    QMessageBox msgBox(this);
    msgBox.setText("Cannot show Properties. No Textures available!");
    msgBox.exec();
    return;
  }

  if (id_ == -1)
    textureLabel->setText("<B>Global Textures</B>");
  else
    textureLabel->setText("<B>Textures for object '" + _name + "'</B>");

  propChanged_ = false;

  textureList->setCurrentItem( textureList->item(0) );
  textureChanged( textureList->item(0) );

  QDialog::show();
}

void texturePropertiesWidget::textureAboutToChange(QListWidgetItem* _item){

  if (propChanged_){
    QMessageBox msgBox(this);
    msgBox.setText("The properties of the current texture have been changed.");
    msgBox.setInformativeText("Do you want to apply these changes?");
    msgBox.setStandardButtons(QMessageBox::Apply | QMessageBox::Discard );
    msgBox.setDefaultButton(QMessageBox::Apply);
    int ret = msgBox.exec();

    if (ret == QMessageBox::Apply){
      //just hit the applyButton ;)
      for (int i=0; i < buttonBox->buttons().count(); i++)
        if ( buttonBox->standardButton( buttonBox->buttons()[i] ) == QDialogButtonBox::Apply )
          slotButtonBoxClicked( buttonBox->buttons()[i] );

      textureList->setCurrentItem( _item );
      textureChanged( _item );

    } else {
      propChanged_ = false;

      textureList->setCurrentItem( _item );
      textureChanged( _item );
    }
  }
}

void texturePropertiesWidget::textureChanged(QListWidgetItem* _item){

  //open changes for the last texture so switch back
  if ( propChanged_ ){
    textureList->setCurrentRow( curRow_ );
    return;
  }

  if ( !texData_->textureExists( _item->text() ) )
    return; //should not happen

  textureName_ = _item->text();

  //update the dialog
  Texture& texture = texData_->texture(textureName_);

  repeatBox->setChecked(texture.parameters.repeat);
  clampBox->setChecked(texture.parameters.clamp);
  centerBox->setChecked(texture.parameters.center);
  absBox->setChecked(texture.parameters.abs);

  max_val->setValue( texture.parameters.max_val );
  clamp_min->setValue( texture.parameters.clamp_min );
  clamp_max->setValue( texture.parameters.clamp_max );

  // Show the texture Image
  imageLabel->setPixmap(QPixmap::fromImage(texture.textureImage));
  imageLabel->setScaledContents(true);

  if ( texture.filename().startsWith("/") )
    fileLabel->setText( "File: " + texture.filename() );
  else
    fileLabel->setText( "File: " + OpenFlipper::Options::textureDirStr() + QDir::separator() + texture.filename() );

  currentImage_ = texture.filename();

  // update plot only when dimension is 1
  if ( texture.dimension() == 1 && id_ != -1) {

    #ifdef WITH_QWT

      std::vector< double > x;

      emit getCoordinates1D(textureName_, id_, x);

      if ( x.size() > 0 ){

        std::vector< double > y( x.size(), 1.0);

        functionPlot_->clear();

        functionPlot_->add_function( x, y, "Histogram" );

        functionPlot_->histogram_mode();
      }

    #endif
  }

  propChanged_ = false;
  curRow_ = textureList->currentRow();
}

void texturePropertiesWidget::slotChangeImage() {

  QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"),
                                                  OpenFlipper::Options::currentDirStr(),
                                                  tr("Images (*.png *.xpm *.jpg)"));

  if ( QFile(fileName).exists() ){

    imageLabel->setPixmap( QPixmap(fileName) );
    imageLabel->setScaledContents(true);

    fileLabel->setText( "File: " + fileName );

    currentImage_ = fileName;

    propChanged_ = true;
  }

}

void texturePropertiesWidget::slotButtonBoxClicked(QAbstractButton* _button){

  QDialogButtonBox::StandardButton btn = buttonBox->standardButton(_button);

  if ( btn == QDialogButtonBox::Apply || btn == QDialogButtonBox::Ok){

    //applySettings
    bool changed = false;

    Texture& texture = texData_->texture(textureName_);

    if ( texture.parameters.repeat != repeatBox->isChecked() ){
      texture.parameters.repeat=repeatBox->isChecked();
      changed = true;
    }
    if ( texture.parameters.clamp != clampBox->isChecked() ){
      texture.parameters.clamp=clampBox->isChecked();
      changed = true;
    }
    if ( texture.parameters.center != centerBox->isChecked() ){
      texture.parameters.center=centerBox->isChecked();
      changed = true;
    }
    if ( texture.parameters.abs != absBox->isChecked() ){
      texture.parameters.abs=absBox->isChecked();
      changed = true;
    }
    if ( texture.parameters.scale != scaleBox->isChecked() ){
      texture.parameters.scale=scaleBox->isChecked();
      changed = true;
    }

    if ( texture.parameters.max_val != max_val->value() ){
      texture.parameters.max_val = max_val->value();
      changed = true;
    }

    if ( texture.parameters.clamp_min != clamp_min->value() ){
      texture.parameters.clamp_min = clamp_min->value();
      changed = true;
    }

    if ( texture.parameters.clamp_max != clamp_max->value() ){
      texture.parameters.clamp_max = clamp_max->value();
      changed = true;
    }

    if ( texture.filename() != currentImage_ ){
      texture.filename( currentImage_ );
      texture.textureImage = QImage( currentImage_ );
      changed = true;
    }

    //inform the plugin about the update
    if (changed)
      emit applyProperties(texData_, textureName_, id_ );

    propChanged_ = false;
  }

  if ( btn == QDialogButtonBox::Apply )
    return;
  else
    hide();
}

void texturePropertiesWidget::slotPropertiesChanged(double /*_value*/){
  propChanged_ = true;
}
