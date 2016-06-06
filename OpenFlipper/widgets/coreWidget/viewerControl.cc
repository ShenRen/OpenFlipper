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
//  CLASS CoreWidget - IMPLEMENTATION
//
//=============================================================================


//== INCLUDES =================================================================

#include "CoreWidget.hh"

#if QT_VERSION > 0x050000
#include <QtConcurrent>
#else
#include <QtCore>
#endif


#include <OpenFlipper/widgets/snapshotDialog/SnapshotDialog.hh>
#include <cmath>

#ifdef _MSC_VER
  #include <ACG/Utils/VSToolsT.hh>
#endif


//== IMPLEMENTATION ==========================================================

//=============================================================================

void CoreWidget::slotToggleStereoMode()
{
  stereoActive_ = !stereoActive_;

  if ( stereoActive_ ) {
    statusBar_->showMessage(tr("Stereo enabled"));
    stereoButton_->setIcon( QIcon(OpenFlipper::Options::iconDirStr()+OpenFlipper::Options::dirSeparator()+"stereo.png") );
  } else {
    statusBar_->showMessage(tr("Stereo disabled"));
    stereoButton_->setIcon( QIcon(OpenFlipper::Options::iconDirStr()+OpenFlipper::Options::dirSeparator()+"mono.png") );
  }

  cursorPainter_->setEnabled (stereoActive_);

  for ( unsigned int i = 0 ; i < OpenFlipper::Options::examinerWidgets() ; ++i )
    examiner_widgets_[i]->properties()->stereo(stereoActive_);
}

void CoreWidget::slotSetGlobalBackgroundColor() {
  ACG::Vec4f bc = PluginFunctions::viewerProperties().backgroundColor() * 255.0;

  QColor backCol((int)bc[0], (int)bc[1], (int)bc[2], (int)bc[3]);
  QColor c = QColorDialog::getColor(backCol,this);

  if (c != backCol && c.isValid())
    for ( uint i = 0 ; i < OpenFlipper::Options::examinerWidgets(); ++i )
      PluginFunctions::viewerProperties(i).backgroundColor(ACG::Vec4f(((double) c.redF())   ,
                                                                      ((double) c.greenF()) ,
                                                                      ((double) c.blueF())  ,
                                                                        1.0));

  OpenFlipperSettings().setValue("Core/Gui/glViewer/defaultBackgroundColor",c);

}

void CoreWidget::slotSetContextBackgroundColor() {
  ACG::Vec4f bc = PluginFunctions::viewerProperties().backgroundColor() * 255.0;

  QColor backCol((int)bc[0], (int)bc[1], (int)bc[2], (int)bc[3]);
  QColor c = QColorDialog::getColor(backCol,this);

  if (c != backCol && c.isValid())
    PluginFunctions::viewerProperties().backgroundColor(ACG::Vec4f(((double) c.redF())   ,
                                                                    ((double) c.greenF()) ,
                                                                    ((double) c.blueF())  ,
                                                                     1.0));
}


/// Set the viewer to home position
void CoreWidget::slotContextHomeView() {
  examiner_widgets_[PluginFunctions::activeExaminer()]->home();
}

/// Show / hide wheels in examiner windows
void CoreWidget::slotSwitchWheels(bool _state) {
  std::vector< glViewer* >::iterator it = examiner_widgets_.begin();

  for(; it != examiner_widgets_.end(); ++it)
    _state ? (*it)->slotShowWheels() : (*it)->slotHideWheels();
}

/// Switch navigation mode
void CoreWidget::slotSwitchNavigation(bool _egomode) {
  std::vector< glViewer* >::iterator it = examiner_widgets_.begin();

  for(; it != examiner_widgets_.end(); ++it) {
    _egomode ? (*it)->navigationMode(glViewer::FIRSTPERSON_NAVIGATION) :
    	(*it)->navigationMode(glViewer::NORMAL_NAVIGATION);
  }
}

/// Set the viewer to home position
void CoreWidget::slotGlobalHomeView() {
  for ( int i = 0 ; i < PluginFunctions::viewers() ; ++i )
    examiner_widgets_[i]->home();
}

/// Set the viewers home position
void CoreWidget::slotContextSetHomeView() {
  examiner_widgets_[PluginFunctions::activeExaminer()]->setHome();
}


/// Set the home position for all viewers
void CoreWidget::slotGlobalSetHomeView() {
  for ( int i = 0 ; i < PluginFunctions::viewers() ; ++i )
    examiner_widgets_[i]->setHome();
}

/// Change view on active viewer to view complete scene
void CoreWidget::slotContextViewAll() {
  examiner_widgets_[PluginFunctions::activeExaminer()]->viewAll();
}


/// Change view on all viewers to view complete scene
void CoreWidget::slotGlobalViewAll() {
  for ( int i = 0 ; i < PluginFunctions::viewers() ; ++i )
    examiner_widgets_[i]->viewAll();
}

/// Toggle projection mode of the active viewer
void CoreWidget::slotContextSwitchProjection() {
  examiner_widgets_[PluginFunctions::activeExaminer()]->toggleProjectionMode();
}

/// Toggle coordsys projection mode of the active viewer
ACG::SceneGraph::CoordsysNode::ProjectionMode CoreWidget::getCoordsysProjection() {  
    // Find coordsys node
    ACG::SceneGraph::BaseNode* node = 0;
    node = PluginFunctions::getSceneGraphRootNode()->find("Core Coordsys Node");
    if (node != 0) {
	return dynamic_cast<ACG::SceneGraph::CoordsysNode*> (node)->getProjectionMode();
    } else {
        emit statusMessage(QString(tr("getCoordsysProjection(): Could not find coordsys node. Assuming default orthographic projection.")));
	return ACG::SceneGraph::CoordsysNode::ORTHOGRAPHIC_PROJECTION;
    }
}

/// Toggle coordsys projection mode of the active viewer
void CoreWidget::slotContextSwitchCoordsysProjection() {
    // Find coordsys node
    ACG::SceneGraph::BaseNode* node = 0;
    node = PluginFunctions::getSceneGraphRootNode()->find("Core Coordsys Node");
    if (node != 0) {
        ACG::SceneGraph::CoordsysNode* cnode = dynamic_cast<ACG::SceneGraph::CoordsysNode*> (node);
        if (cnode->getProjectionMode() == ACG::SceneGraph::CoordsysNode::PERSPECTIVE_PROJECTION) {
            cnode->setProjectionMode(ACG::SceneGraph::CoordsysNode::ORTHOGRAPHIC_PROJECTION);
        }
        else {
            cnode->setProjectionMode(ACG::SceneGraph::CoordsysNode::PERSPECTIVE_PROJECTION);
        }
    } else {
        emit statusMessage(QString(tr("slotContextSwitchCoordsysProjection(): Could not find coordsys node, thus its projection mode will not be toggled.")));
    }

    for ( unsigned int i = 0 ; i < OpenFlipper::Options::examinerWidgets() ; ++i )
        examiner_widgets_[i]->updateGL();
}

/// Toggle projection mode of all viewers to perspective projection
void CoreWidget::slotGlobalPerspectiveProjection() {
  for ( int i = 0 ; i < PluginFunctions::viewers() ; ++i )
    examiner_widgets_[i]->perspectiveProjection();
}

/// Toggle projection mode of all viewers to orthogonal projection
void CoreWidget::slotGlobalOrthographicProjection() {
  for ( int i = 0 ; i < PluginFunctions::viewers() ; ++i )
    examiner_widgets_[i]->orthographicProjection();
}


/// If animation is disabled in all viewers, enable it in all viewers. Otherwise disable it.
void CoreWidget::slotGlobalToggleAnimation() {
  int enabledCount  = 0;

  for ( int i = 0 ; i< PluginFunctions::viewers(); ++i ) {
    if ( PluginFunctions::viewerProperties(i).animation() )
      enabledCount++;
  }
  
  slotGlobalChangeAnimation (enabledCount == 0);
}

/// Set the animation mode for all viewers
void CoreWidget::slotGlobalChangeAnimation(bool _animation){
  for ( uint i = 0 ; i < OpenFlipper::Options::examinerWidgets() ; ++i )
    PluginFunctions::viewerProperties(i).animation(_animation);
}

/// Set the animation mode for active viewer
void CoreWidget::slotLocalChangeAnimation(bool _animation){
  PluginFunctions::viewerProperties().animation(_animation);
}


/// If backface culling is disabled in all viewers, enable it in all viewers. Otherwise disable it.
void CoreWidget::slotGlobalToggleBackFaceCulling() {
  int enabledCount  = 0;

  for ( int i = 0 ; i< PluginFunctions::viewers(); ++i ) {
    if ( PluginFunctions::viewerProperties(i).backFaceCulling() )
      enabledCount++;
  }
  
  slotGlobalChangeBackFaceCulling (enabledCount == 0);
}

/// Set backface culling for all viewers
void CoreWidget::slotGlobalChangeBackFaceCulling(bool _backFaceCulling){
  for ( uint i = 0 ; i < OpenFlipper::Options::examinerWidgets() ; ++i )
    PluginFunctions::viewerProperties(i).backFaceCulling(_backFaceCulling);

}

/// Set backface culling for active viewer
void CoreWidget::slotLocalChangeBackFaceCulling(bool _backFaceCulling){
  PluginFunctions::viewerProperties().backFaceCulling(_backFaceCulling);
}


/// If two-sided lighting is disabled in all viewers, enable it in all viewers. Otherwise disable it.
void CoreWidget::slotGlobalToggleTwoSidedLighting() {
  int enabledCount  = 0;

  for ( int i = 0 ; i< PluginFunctions::viewers(); ++i ) {
    if ( PluginFunctions::viewerProperties(i).twoSidedLighting() )
      enabledCount++;
  }
  
  slotGlobalChangeTwoSidedLighting (enabledCount == 0);
}

/// Set two-sided lighting for all viewers
void CoreWidget::slotGlobalChangeTwoSidedLighting(bool _lighting) {
  for ( uint i = 0 ; i < OpenFlipper::Options::examinerWidgets() ; ++i )
    PluginFunctions::viewerProperties(i).twoSidedLighting(_lighting);
}

/// Set two-sided lighting for active viewer
void CoreWidget::slotLocalChangeTwoSidedLighting(bool _lighting) {
  PluginFunctions::viewerProperties().twoSidedLighting(_lighting);
}


/// If multisampling is disabled in all viewers, enable it in all viewers. Otherwise disable it.
void CoreWidget::slotGlobalToggleMultisampling() {
  int enabledCount  = 0;

  for ( int i = 0 ; i< PluginFunctions::viewers(); ++i ) {
    if ( PluginFunctions::viewerProperties(i).multisampling() )
      enabledCount++;
  }
  
  slotGlobalChangeMultisampling (enabledCount == 0);
}

/// Set multisampling for all viewers
void CoreWidget::slotGlobalChangeMultisampling(bool _multisampling) {
  for ( uint i = 0 ; i < OpenFlipper::Options::examinerWidgets() ; ++i )
    PluginFunctions::viewerProperties(i).multisampling( _multisampling );
}

/// Set multisampling for active viewer
void CoreWidget::slotLocalChangeMultisampling(bool _multisampling) {
  PluginFunctions::viewerProperties().multisampling( _multisampling );
}


/// If mipmapping is disabled in all viewers, enable it in all viewers. Otherwise disable it.
void CoreWidget::slotGlobalToggleMipmapping() {
  int enabledCount  = 0;

  for ( int i = 0 ; i< PluginFunctions::viewers(); ++i ) {
    if ( PluginFunctions::viewerProperties(i).mipmapping() )
      enabledCount++;
  }
  
  slotGlobalChangeMipmapping (enabledCount == 0);
}

/// Set mipmapping for all viewers
void CoreWidget::slotGlobalChangeMipmapping(bool _mipmapping) {
  for ( uint i = 0 ; i < OpenFlipper::Options::examinerWidgets() ; ++i )
    PluginFunctions::viewerProperties(i).mipmapping( _mipmapping );
}

/// Set mipmapping for active viewer
void CoreWidget::slotLocalChangeMipmapping(bool _mipmapping) {
  PluginFunctions::viewerProperties().mipmapping( _mipmapping );
}

/// Take a snapshot of the current Viewer
void CoreWidget::slotExaminerSnapshot() {

  QFileInfo fi(PluginFunctions::viewerProperties().snapshotName());
  int counter = PluginFunctions::viewerProperties().snapshotCounter();

    // Add leading zeros
  QString number = QString::number(counter);
  while ( number.size() < 7 )
    number = "0" + number;

  QString suggest = fi.baseName() + "." + number + ".";

  QString format="png";

  if (fi.completeSuffix() == "ppm")
    format="ppmraw";

  if (fi.completeSuffix() == "jpg")
    format="jpg";

  suggest += format;

  QFileDialog dialog(this);
  dialog.setFileMode(QFileDialog::AnyFile);
  dialog.setDefaultSuffix("png");
  dialog.setNameFilter(tr("Images (*.png *.ppm *.jpg)"));
  dialog.setFileMode(QFileDialog::AnyFile);
  dialog.setConfirmOverwrite(true);
  dialog.setDirectory( fi.path() );
  dialog.selectFile( suggest );
  dialog.setAcceptMode(QFileDialog::AcceptSave);
  dialog.setWindowTitle(tr("Save Snapshot"));

 if (dialog.exec()){
     QString newName = dialog.selectedFiles()[0];

      if (newName != fi.path() + OpenFlipper::Options::dirSeparator() + suggest)
        PluginFunctions::viewerProperties().snapshotBaseFileName(newName);

      QImage image;
      examiner_widgets_[PluginFunctions::activeExaminer()]->snapshot(image);

      image.save(newName);
  }
}

static QString suggestSnapshotFilename(QString mostRecentPath) {
    if (mostRecentPath.isEmpty()) {
        mostRecentPath = QString("%1%2snap.0000000.png")
            .arg(OpenFlipperSettings().value("Core/CurrentDir").toString())
            .arg(QDir::separator());
    }

    QFileInfo fi(mostRecentPath);
    QString path = fi.path();

    if (!fi.exists() && QFileInfo(path).isWritable()) {
#ifndef NDEBUG
        std::cout << "suggestSnapshotFilename(): mostRecentPath feasible as "
                "file name. Using it." << std::endl;
#endif
        return mostRecentPath;
    }

    if (!QFileInfo(path).isWritable()) {
#ifndef NDEBUG
        std::cout << "suggestSnapshotFilename(): Most recent path invalid. "
                "Doesn't exist. Returning empty string." << std::endl;
#endif
        return QString::null;
    }

    QString base_name = fi.completeBaseName();
    QString suffix = fi.suffix();

    if (suffix.isEmpty())
        suffix = "png";

    QRegExp base_name_re("(\\D*)(\\d+)?(.*)");
    base_name_re.setPatternSyntax(QRegExp::RegExp2);
    if (!base_name_re.exactMatch(base_name)) {
#ifndef NDEBUG
        std::cout << "suggestSnapshotFilename(): Regexp didn't match. This "
            "should be impossible." << std::endl;
#endif
        return QString::null;
    }

    QString pre = base_name_re.cap(1),
            num = base_name_re.cap(2),
            post = base_name_re.cap(3);

#ifndef NDEBUG
    std::cout << (QString("suggestSnapshotFilename(): Decomposition of "
        "\"%1\": \"%2\", \"%3\", \"%4\"")
            .arg(base_name)
            .arg(pre)
            .arg(num)
            .arg(post)).toStdString() << std::endl;
#endif

    if (pre.isEmpty() && num.isEmpty() && post.isEmpty()) {
        pre = "snap.";
    }

    size_t num_len = num.length();
    bool num_is_int;
    int file_no = num.toInt(&num_is_int);
    if (!num_is_int) {
        file_no = 0;
        num_len = 6;
    }

    size_t sanity_counter = 0;
    for (; sanity_counter < 100000; ++file_no, ++sanity_counter) {
        QString suggested_file_name =
                QString("%1%2%3%4%5.%6")
                .arg(path)
                .arg(QDir::separator())
                .arg(pre)
                .arg(file_no, num_len, 10, QLatin1Char('0'))
                .arg(post)
                .arg(suffix)
                ;
        QFileInfo suggested_fi(suggested_file_name);
        if (!suggested_fi.exists()){
#ifndef NDEBUG
            std::cout << "suggestSnapshotFilename(): Found a feasible file "
                    "name. Returning it." << std::endl;
#endif
            return suggested_file_name;
        }
    }

#ifndef NDEBUG
    std::cout << "suggestSnapshotFilename(): No luck incrementing file_no. "
            "Aborting, returning empty string." << std::endl;
#endif
    return QString::null;
}

///Take a snapshot of the whole application
void CoreWidget::applicationSnapshotDialog() {
  int w = width();
  int h = height();

  SnapshotDialog dialog(suggestSnapshotFilename(snapshotName_), false, w, h, 0);
  
  connect(&dialog, SIGNAL(resizeApplication(int,int)), this, SIGNAL(resizeApplication(int,int)) );

  bool ok = dialog.exec();

  if ( ok ){
    QString newName = dialog.filename->text();

    OpenFlipperSettings().setValue("Core/CurrentDir", QFileInfo(newName).absolutePath() );
      
    snapshotName_ = newName;

    //grabs only the widget (espacially in windows)
    //todo: deprecated in QT 5.0, use QScreen instead
    QPixmap pic = QPixmap::grabWindow( winId() );

    QPainter painter (&pic);

    //so we have to add the content from the GLContext manually
    for (std::vector< glViewer* >::iterator iter = examiner_widgets_.begin(); iter != examiner_widgets_.end(); ++iter)
    {
      if (*iter)
      {
        QImage fillImage;

        (*iter)->snapshot(fillImage, (*iter)->glWidth() , (*iter)->glHeight());

        QPoint localPos = QPoint((*iter)->pos().x(),(*iter)->pos().y());
        QPointF pos = glView_->mapTo(this,localPos);
        painter.drawImage(pos,fillImage);
      }
    }

    pic.save(newName);
  }

  emit resizeApplication(w,h);
}

///Take a snapshot of the whole application
void CoreWidget::applicationSnapshot() {
  // Write image asynchronously
  QImage* pic = new QImage(QPixmap::grabWindow( winId() ).toImage());
  writeImageAsynchronously(pic, suggestSnapshotFilename(snapshotName_));
}

void CoreWidget::viewerSnapshot(QString file_name, bool store_comments,
        bool comments_visible_only, bool comments_targeted_only,
        bool store_material_info, int snapshot_width, int snapshot_height,
        bool snapshot_transparent, bool hide_coord_sys,
        int snapshot_multisampling, bool store_view,
        bool snapshot_supersampling, float snapshot_sample_dist,
        float snapshot_subpixel_area, int snapshot_res_incr) {

    if (snapshot_height < 0) {
        int w = glView_->width();
        int h = glView_->height();
        snapshot_height = static_cast<int>(round(
                static_cast<double>(snapshot_width) / w * h));
    }

    QString comments;
    if (store_comments) {
        comments = PluginFunctions::collectObjectComments(
                comments_visible_only,
                comments_targeted_only).join("\n");
    }

    QString materials;
    if (ACG::SceneGraph::Material::support_json_serialization() &&
    //if (ACG::SceneGraph::Material::CP_JSON_SERIALIZABLE &&
            store_material_info) {
        materials = PluginFunctions::collectObjectMaterials(
                comments_visible_only,
                comments_targeted_only).join("\n");
    }

    //now take the snapshot
    switch ( baseLayout_->mode() ){

      case QtMultiViewLayout::SingleView:
      {
        QImage finalImage;

        examiner_widgets_[PluginFunctions::activeExaminer()]->snapshot(finalImage,
            snapshot_width, snapshot_height,
            snapshot_transparent, hide_coord_sys,
            snapshot_multisampling, 
            snapshot_supersampling, snapshot_sample_dist,
            snapshot_subpixel_area, snapshot_res_incr);

        if (!comments.isEmpty())
            finalImage.setText("Mesh Comments", comments);
        if (!materials.isEmpty())
            finalImage.setText("Mesh Materials", materials);
        if (store_view) {
            QSize window_size;
            if (isMaximized())
              window_size = QSize(-width(), -height());
            else
              window_size = QSize (width(), height());

            int splitter_size = 0;
            if (OpenFlipperSettings().value("Core/Gui/ToolBoxes/ToolBoxOnTheRight",true).toBool())
              splitter_size = toolSplitter_->sizes()[1];
            else
              splitter_size = toolSplitter_->sizes()[0];

            QString view;
            examiner_widgets_[PluginFunctions::activeExaminer()]->encodeView(view, window_size, splitter_size);
            finalImage.setText("View", view);
        }
        finalImage.save(file_name);

        break;
      }
      case QtMultiViewLayout::DoubleView:
      {
        int w = snapshot_height;

        double relSizeW = static_cast<double>( examiner_widgets_[0]->glWidth() / static_cast<double>( glScene_->width() ) );

        //Get the images
        QImage img[2];
        examiner_widgets_[0]->snapshot(
                img[0], static_cast<int>(relSizeW * w),
                snapshot_width, snapshot_transparent,
                hide_coord_sys);
        examiner_widgets_[1]->snapshot(
                img[1], static_cast<int>(relSizeW * w),
                snapshot_width, snapshot_transparent,
                hide_coord_sys);

        QImage finalImage(img[0].width() + img[1].width() +2, img[0].height(),
                QImage::Format_ARGB32_Premultiplied);

        QPainter painter(&finalImage);

        painter.fillRect(0,0,finalImage.width(),
                finalImage.height(), QBrush(Qt::gray));

        painter.drawImage(QRectF(           0,     0, img[0].width(), img[0].height()),img[0],
                          QRectF(           0,     0, img[0].width(), img[0].height()) );
        painter.drawImage(QRectF(img[0].width()+2, 0, img[1].width(), img[1].height()),img[1],
                          QRectF(           0,     0, img[1].width(), img[1].height()) );

        if (!comments.isEmpty())
            finalImage.setText("Mesh Comments", comments);
        finalImage.save(file_name);

        break;
      }

      case QtMultiViewLayout::Grid:
      {
        // Relative size of first viewer (in relation to the other viewers
        double relSizeW = (double)examiner_widgets_[0]->glWidth() / (double)glScene_->width();
        double relSizeH = (double)examiner_widgets_[0]->glHeight() / (double)glScene_->height();
          
        QImage img0,img1,img2,img3;

        examiner_widgets_[0]->snapshot(img0,
                (int)((double)snapshot_width * relSizeW),
                (int)((double)snapshot_height * relSizeH),
                snapshot_transparent, hide_coord_sys);
        examiner_widgets_[1]->snapshot(img1,
                (int)((double)snapshot_width * (1.0 - relSizeW)),
                (int)((double)snapshot_height * relSizeH),
                snapshot_transparent, hide_coord_sys);
        examiner_widgets_[2]->snapshot(img2,
                (int)((double)snapshot_width * relSizeW),
                (int)((double)snapshot_height * (1.0 - relSizeH)),
                snapshot_transparent, hide_coord_sys);
        examiner_widgets_[3]->snapshot(img3,
                (int)((double)snapshot_width * (1.0 - relSizeW)),
                (int)((double)snapshot_height * (1.0 - relSizeH)),
                snapshot_transparent, hide_coord_sys);

        QImage finalImage(img0.width() + img1.width()+2,
                img0.height() + img2.height()+2,
                QImage::Format_ARGB32_Premultiplied);

        QPainter painter(&finalImage);

        painter.fillRect(0,0,finalImage.width(), finalImage.height(), QBrush(Qt::gray));

        painter.drawImage(QRectF(           0,             0, img0.width(), img0.height()),img0,
                          QRectF(           0,             0, img0.width(), img0.height()) );
        painter.drawImage(QRectF(img0.width()+2,           0, img1.width(), img1.height()),img1,
                          QRectF(           0,             0, img1.width(), img1.height()) );
        painter.drawImage(QRectF(          0,img0.height()+2, img2.width(), img2.height()),img2,
                          QRectF(           0,             0, img2.width(), img2.height()) );
        painter.drawImage(QRectF(img0.width()+2, img0.height()+2, img3.width(), img3.height()),img3,
                          QRectF(           0,             0, img3.width(), img3.height()) );

        if (!comments.isEmpty())
            finalImage.setText("Mesh Comments", comments);
        finalImage.save(file_name);

        break;
      }
      case QtMultiViewLayout::HSplit:
      {
        // Relative size of first viewer (in relation to the other viewers
        double relSizeW = (double)examiner_widgets_[0]->glWidth() / (double)glScene_->width();
        double relSizeH1 = (double)examiner_widgets_[1]->glHeight() / (double)glScene_->height();
        double relSizeH2 = (double)examiner_widgets_[2]->glHeight() / (double)glScene_->height();
        double relSizeH3 = (double)examiner_widgets_[3]->glHeight() / (double)glScene_->height();
          
        QImage img0,img1,img2,img3;

        examiner_widgets_[0]->snapshot(img0,
                (int)((double)snapshot_width * relSizeW), snapshot_height,
                snapshot_transparent, hide_coord_sys);
        examiner_widgets_[1]->snapshot(img1,
                (int)((double)snapshot_width * (1.0 - relSizeW)),
                relSizeH1 * (double)snapshot_height,
                snapshot_transparent, hide_coord_sys);
        examiner_widgets_[2]->snapshot(img2,
                (int)((double)snapshot_width * (1.0 - relSizeW)),
                relSizeH2 * (double)snapshot_height,
                snapshot_transparent, hide_coord_sys);
        examiner_widgets_[3]->snapshot(img3,
                (int)((double)snapshot_width * (1.0 - relSizeW)),
                relSizeH3 * (double)snapshot_height,
                snapshot_transparent, hide_coord_sys);

        QImage finalImage(img0.width() + img1.width() +2, img0.height(), QImage::Format_ARGB32_Premultiplied);

        QPainter painter(&finalImage);

        painter.fillRect(0,0,finalImage.width(), finalImage.height(), QBrush(Qt::gray));

        painter.drawImage(QRectF(           0,             0, img0.width(), img0.height()),img0,
                          QRectF(           0,             0, img0.width(), img0.height()) );
        painter.drawImage(QRectF(img0.width()+2,             0, img1.width(), img1.height()),img1,
                          QRectF(           0,             0, img1.width(), img1.height()) );
        painter.drawImage(QRectF(img0.width()+2, img1.height()+2, img2.width(), img2.height()),img2,
                          QRectF(           0,             0, img2.width(), img2.height()) );
        painter.drawImage(QRectF(img0.width()+2, img1.height()+img2.height()+4, img3.width(),img3.height()),img3,
                          QRectF(           0,             0, img3.width(), img3.height()) );

        if (!comments.isEmpty())
            finalImage.setText("Mesh Comments", comments);
        finalImage.save(file_name);

        break;
      }
      default: break;

    }
}

///Take a snapshot of all viewers
void CoreWidget::viewerSnapshotDialog() {
  int w = glView_->width();
  int h = glView_->height();

  SnapshotDialog dialog(suggestSnapshotFilename(snapshotName_), true, w, h, 0);

  if (!ACG::SceneGraph::Material::support_json_serialization())
      dialog.metaData_storeMatInfo_cb->setVisible(false);

  bool ok = dialog.exec();

  if (ok){
    QString newName = dialog.filename->text();

    OpenFlipperSettings().setValue("Core/CurrentDir", QFileInfo(newName).absolutePath() );

    snapshotName_ = newName;

    const bool storeComments = dialog.metaData_storeComments_cb->isChecked();
    const bool comments_visible_only =
            dialog.metaData_comments_visibleOnly_cb->isChecked();
    const bool comments_targeted_only =
            dialog.metaData_comments_targetedOnly_cb->isChecked();
    const bool store_material_info =
            dialog.metaData_storeMatInfo_cb->isChecked();
    const int snapshot_width = dialog.snapWidth->value();
    const int snapshot_height = dialog.snapHeight->value();
    const bool snapshot_transparent = dialog.transparent->isChecked();
    const bool hide_coord_sys = dialog.hideCoordsys->isChecked();
    const int snapshot_multisampling =
            dialog.multisampling->isChecked() ?
                    dialog.num_samples->value() : 1;
    const bool store_view = dialog.metaData_storeView_cb->isChecked();

    const bool snapshot_supersampling = dialog.supersampling->isChecked();
    const float snapshot_sample_dist = dialog.supersampling_dist->value();
    const float snapshot_sample_area = dialog.supersampling_area->value();
    const int snapshot_supersample_res_incr = dialog.supersampling_res_incr->value();

    viewerSnapshot(newName, storeComments, comments_visible_only,
            comments_targeted_only, store_material_info, snapshot_width,
            snapshot_height, snapshot_transparent, hide_coord_sys,
            snapshot_multisampling, store_view,
            snapshot_supersampling, snapshot_sample_dist,
            snapshot_sample_area, snapshot_supersample_res_incr);
  }
  //glView_->resize(w, h);
}

///Take a snapshot of all viewers
void CoreWidget::viewerSnapshot() {

    switch ( baseLayout_->mode() ){

    case QtMultiViewLayout::SingleView:
    {
      QImage* finalImage = new QImage();

      examiner_widgets_[PluginFunctions::activeExaminer()]->snapshot(*finalImage);

      writeImageAsynchronously(finalImage, suggestSnapshotFilename(snapshotName_));

      break;
    }
    case QtMultiViewLayout::DoubleView:
    {
    	//Get the images
    	QImage img[2];
    	examiner_widgets_[0]->snapshot(img[0]);
    	examiner_widgets_[1]->snapshot(img[1]);

    	QImage* finalImage = new QImage(img[0].width() + img[1].width() +2, img[0].height(), QImage::Format_ARGB32_Premultiplied);

    	QPainter painter(finalImage);

    	painter.fillRect(0,0,finalImage->width(), finalImage->height(), QBrush(Qt::gray));

    	painter.drawImage(QRectF(           0,             0, img[0].width(), img[0].height()),img[0],
    			              QRectF(           0,             0, img[0].width(), img[0].height()) );
    	painter.drawImage(QRectF(img[0].width()+2,         0, img[1].width(), img[1].height()),img[1],
    	              		QRectF(           0,             0, img[1].width(), img[1].height()) );

    	writeImageAsynchronously(finalImage, suggestSnapshotFilename(snapshotName_));

    	break;
    }

    case QtMultiViewLayout::Grid:
    {
      QImage img0,img1,img2,img3;
      
      examiner_widgets_[0]->snapshot(img0);
      examiner_widgets_[1]->snapshot(img1);
      examiner_widgets_[2]->snapshot(img2);
      examiner_widgets_[3]->snapshot(img3);

      QImage* finalImage = new QImage(img0.width() + img1.width() + 2, img0.height() + img2.height() + 2, QImage::Format_ARGB32_Premultiplied);

      QPainter painter(finalImage);

      painter.fillRect(0,0,finalImage->width(), finalImage->height(), QBrush(Qt::gray));

      painter.drawImage(QRectF(           0,             0, img0.width(), img0.height()),img0,
                        QRectF(           0,             0, img0.width(), img0.height()) );
      painter.drawImage(QRectF(img0.width()+2,           0, img1.width(), img1.height()),img1,
                        QRectF(           0,             0, img1.width(), img1.height()) );
      painter.drawImage(QRectF(           0, img0.height()+2, img2.width(), img2.height()),img2,
                        QRectF(           0,             0, img2.width(), img2.height()) );
      painter.drawImage(QRectF(img0.width()+2, img0.height()+2, img3.width(), img3.height()),img3,
                        QRectF(           0,             0, img3.width(), img3.height()) );

      writeImageAsynchronously(finalImage, suggestSnapshotFilename(snapshotName_));

      break;
    }
    case QtMultiViewLayout::HSplit:
    {
      QImage img0,img1,img2,img3;

      examiner_widgets_[0]->snapshot(img0);
      examiner_widgets_[1]->snapshot(img1);
      examiner_widgets_[2]->snapshot(img2);
      examiner_widgets_[3]->snapshot(img3);

      QImage* finalImage = new QImage(img0.width() + img1.width() + 2, img0.height(), QImage::Format_ARGB32_Premultiplied);

      QPainter painter(finalImage);

      painter.fillRect(0,0,finalImage->width(), finalImage->height(), QBrush(Qt::gray));

      painter.drawImage(QRectF(           0,             0, img0.width(), img0.height()),img0,
                        QRectF(           0,             0, img0.width(), img0.height()) );
      painter.drawImage(QRectF(img0.width()+2,             0, img1.width(), img1.height()),img1,
                        QRectF(           0,             0, img1.width(), img1.height()) );
      painter.drawImage(QRectF(img0.width()+2, img1.height()+2, img2.width(), img2.height()),img2,
                        QRectF(           0,             0, img2.width(), img2.height()) );
      painter.drawImage(QRectF(img0.width()+2, img1.height()+img2.height()+4, img3.width(),img3.height()),img3,
                        QRectF(           0,             0, img3.width(), img3.height()) );

      writeImageAsynchronously(finalImage, suggestSnapshotFilename(snapshotName_));

      break;
    }
    default: break;

  }
}

void CoreWidget::applicationSnapshotName(QString _name) {
  snapshotName_ = _name;
}


void writeImageQImage(QImage* _image, const QString _name) {

    _image->save(_name);
    delete _image;
}

void CoreWidget::writeImageAsynchronously(QImage* _image, const QString _name) {

    QFuture<void>* future = new QFuture<void>();
    *future = QtConcurrent::run(writeImageQImage, _image, _name);
    QFutureWatcher<void>* watcher = new QFutureWatcher<void>();
    watcher->setFuture(*future);

    watcher_garbage_.insert(std::pair<QFutureWatcher<void>*,QFuture<void>*>(watcher, future));

    connect(watcher, SIGNAL(finished()), this, SLOT(delete_garbage()));
}



void CoreWidget::delete_garbage() {

    QObject* obj = QObject::sender();
    QFutureWatcher<void>* watcher = dynamic_cast<QFutureWatcher<void>*>(obj);
    if(!watcher) {
        return;
    }

    map_mutex_.lock();

    std::map<QFutureWatcher<void>*,QFuture<void>*>::iterator f;
    f = watcher_garbage_.find(watcher);
    if(f != watcher_garbage_.end()) {
        delete f->second;
        delete f->first;
        watcher_garbage_.erase(f);
    }

    map_mutex_.unlock();
}

void CoreWidget::slotPasteView( ) {
  examiner_widgets_[PluginFunctions::activeExaminer()]->actionPasteView();
}

void CoreWidget::slotSetView( QString view ) {
  examiner_widgets_[PluginFunctions::activeExaminer()]->actionSetView(view);
}

void CoreWidget::slotSetViewAndWindowGeometry(QString view) {
    const unsigned int viewerId = PluginFunctions::activeExaminer();

    QSize windowSize(0, 0);
    int splitterWidth = 0;
    QSize viewportSize(0, 0);
    examiner_widgets_[viewerId]->decodeView (
            view, &windowSize, &splitterWidth, &viewportSize);

    if (windowSize.height() != 0 && windowSize.width() != 0) {
        if (windowSize.width() < 0) {
            windowSize *= -1;
            showNormal();
            resize(windowSize);
            showMaximized();
        } else {
            showNormal();
            resize(windowSize);
        }
    }

    if (splitterWidth > 0) {
        QList<int> splitter_sizes = toolSplitter_->sizes();
        if (splitter_sizes.size() < 2) {
            std::cerr << "The tool splitter has less than two children. This "
                    "shouldn't happen." << std::endl;
        } else {
            const size_t primary_idx = OpenFlipperSettings().value(
                    "Core/Gui/ToolBoxes/ToolBoxOnTheRight",true).toBool()
                    ? 1 : 0;

            const int diff = splitterWidth - splitter_sizes[primary_idx];
            splitter_sizes[primary_idx] += diff;
            splitter_sizes[1-primary_idx] -= diff;
        }
        toolSplitter_->setSizes(splitter_sizes);
    }

    /*
     * Viewport size has precedence. Manipulate window size so that the
     * viewport size is matched exactly.
     */
    if (viewportSize.width() > 0 && viewportSize.height() > 0) {
        /*
         * Try twice: Sometimes sizes of elements get readjusted after resizing
         * and the viewport will not have the desired size.
         */
        for (int i = 0; i < 2; ++i) {
            const QSize cur_viewport_size = examiner_widgets_[viewerId]->size().toSize();
            if (cur_viewport_size != viewportSize) {
                std::cout << "Stored viewport size is " << viewportSize.width()
                        << " x " << viewportSize.height() << ". Actual size is "
                        << cur_viewport_size.width() << " x "
                        << cur_viewport_size.height() << ". Resizing window."
                        <<  std::endl;

                showNormal();
                QSize diff = viewportSize - cur_viewport_size;
                resize(size() + diff);
                const QSize new_viewport_size =
                        examiner_widgets_[viewerId]->size().toSize();
                diff = viewportSize - new_viewport_size;
                if (diff.width() != 0) {
                    std::cout << "New viewport size is "
                            << new_viewport_size.width()
                            << " x " << new_viewport_size.height() << "."
                            << " Moving splitter by " << diff.width() << "."
                            << std::endl;
                    // Move splitter.
                    QList<int> splitter_sizes = toolSplitter_->sizes();
                    if (splitter_sizes.size() < 2) {
                        std::cerr << "The tool splitter has less than two children. This "
                                "shouldn't happen." << std::endl;
                    } else {
                        const size_t primary_idx = OpenFlipperSettings().value(
                                "Core/Gui/ToolBoxes/ToolBoxOnTheRight",true).toBool()
                                ? 0 : 1;

                        splitter_sizes[primary_idx] += diff.width();
                        splitter_sizes[1-primary_idx] -= diff.width();
                    }
                    toolSplitter_->setSizes(splitter_sizes);

                }
            } else {
                break;
            }
        }
    }
}

void CoreWidget::slotPasteViewAndWindow()
{
  QSize size;
  int splitterWidth;
  examiner_widgets_[PluginFunctions::activeExaminer()]->actionPasteView(&size,&splitterWidth);

  //resize the toolbox and splitter
  if (splitterWidth != -1)
  {
    QList<int> sizes;

    //std::cerr << "Sizes : " << size[0] << " " << size[1] << " " << sum_size << std::endl;

    bool onRight = OpenFlipperSettings().value("Core/Gui/ToolBoxes/ToolBoxOnTheRight",true).toBool();
    if (onRight)
    {
      sizes.push_back(size.width() - splitterWidth);
      sizes.push_back(splitterWidth);
    }
    else
    {
      sizes.push_back(splitterWidth);
      sizes.push_back(size.width() - splitterWidth);
    }

    toolSplitter_->setSizes(sizes);
  }

  //resize window
  if (size.isValid())
  {
    if (size == QSize(0,0))
    {
      showMaximized();
    }
    else
    {
      showNormal();
      resizeApplication(size.width(),size.height());
    }
  }
}

void CoreWidget::slotCopyView( ) {
  QSize size;
  if (isMaximized())
    size = QSize(0,0);
  else
    size = QSize (width(),height());

  int splitter_size = 0;
  if (OpenFlipperSettings().value("Core/Gui/ToolBoxes/ToolBoxOnTheRight",true).toBool())
    splitter_size = toolSplitter_->sizes()[1];
  else
    splitter_size = toolSplitter_->sizes()[0];

  const bool make_c_string = (QApplication::keyboardModifiers() & Qt::ControlModifier);
  examiner_widgets_[PluginFunctions::activeExaminer()]->actionCopyView(
          size, splitter_size, make_c_string);
}

void CoreWidget::slotCoordSysVisibility(bool _visible){

  ACG::SceneGraph::BaseNode* root = PluginFunctions::getSceneGraphRootNode();
  ACG::SceneGraph::BaseNode* coordSys = root->find("Core Coordsys Node");

  if (coordSys == 0){
    emit log( LOGERR, tr("CoordSys Node not found"));
    return;
  }

if (_visible)
    coordSys->show();
  else
    coordSys->hide();

  for ( unsigned int i = 0 ; i < OpenFlipper::Options::examinerWidgets() ; ++i )
    examiner_widgets_[i]->updateGL();

}

void CoreWidget::slotSetViewingDirection(QAction* _action) {

  PluginFunctions::setFixedView( _action->data().toInt() );
  if (_action->data().toInt() != PluginFunctions::VIEW_FREE)
    PluginFunctions::allowRotation( false, PluginFunctions::activeExaminer() );
  
    // Update view
    examiner_widgets_[PluginFunctions::activeExaminer()]->updateGL();
}

void CoreWidget::slotLockRotation(bool _lock) {

  PluginFunctions::allowRotation( !_lock, PluginFunctions::activeExaminer() );
  if (!_lock)
    PluginFunctions::setFixedView( PluginFunctions::VIEW_FREE );
}

void CoreWidget::moveBack() {
    examiner_widgets_[PluginFunctions::activeExaminer()]->moveBack();
}

void CoreWidget::moveForward() {
    examiner_widgets_[PluginFunctions::activeExaminer()]->moveForward();
}

void CoreWidget::strafeLeft() {
    examiner_widgets_[PluginFunctions::activeExaminer()]->strafeLeft();
}

void CoreWidget::strafeRight() {
    examiner_widgets_[PluginFunctions::activeExaminer()]->strafeRight();
}

//=============================================================================
