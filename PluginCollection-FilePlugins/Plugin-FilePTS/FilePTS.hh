//================================================================
//
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


//================================================================
//
//  CLASS FilePTSPlugin
//
//    This class is the base class for loading and saving (reading/writing) SplatCloud objects from/to disc.
//
//================================================================


#ifndef FILEPTSPLUGIN_HH
#define FILEPTSPLUGIN_HH


//== INCLUDES ====================================================


#include <QObject>
class QCheckBox;
class QComboBox;
class QPushButton;

#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/FileInterface.hh>
#include <OpenFlipper/BasePlugin/LoadSaveInterface.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>
#include <OpenFlipper/BasePlugin/ScriptInterface.hh>
#include <OpenFlipper/BasePlugin/RPCInterface.hh>

#include <OpenFlipper/common/Types.hh>

#include <ObjectTypes/SplatCloud/SplatCloud.hh>


//== CLASS DEFINITION ============================================


class FilePTSPlugin : public QObject, BaseInterface, FileInterface, LoadSaveInterface, LoggingInterface, ScriptInterface, RPCInterface
{
  Q_OBJECT
  Q_INTERFACES( FileInterface     )
  Q_INTERFACES( LoadSaveInterface )
  Q_INTERFACES( LoggingInterface  )
  Q_INTERFACES( BaseInterface     )
  Q_INTERFACES( ScriptInterface   )
  Q_INTERFACES( RPCInterface      )
#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "org.OpenFlipper.Plugins.Plugin-FilePTS")
#endif
signals:

  // -- File Interface --
  void openedFile( int _objectId );

  // -- LoadSave Interface --
  void addEmptyObject( DataType _type, int &_objectId );
  void deleteObject ( int _objectId );
  void updatedObject( int _objectId, const UpdateType &_type );

  //-- Logging Interface --
  void log(                QString _message );
  void log( Logtype _type, QString _message );

private slots:

  // -- Base Interface --
  void noguiSupported() { }

public:

  // standard constructor/destructor
  FilePTSPlugin();
  ~FilePTSPlugin() { }

  //-- Base Interface --
  QString name()         { return QString(    "FilePTS"                            ); }
  QString description( ) { return QString( tr("Load/Save SplatCloud format files") ); }

  // -- File Interface --
  DataType supportedType() { return DATA_SPLATCLOUD; }

  // -- File Interface --
  QString getSaveFilters() { return QString( tr("SplatCloud format files ( *.pts *.bin )") ); }
  QString getLoadFilters() { return QString( tr("SplatCloud format files ( *.pts *.bin )") ); }
  QWidget *saveOptionsWidget( QString /*_currentFilter*/ );
  QWidget *loadOptionsWidget( QString /*_currentFilter*/ );

public slots:

  // -- Base Interface --
  QString version() { return QString( "1.0" ); }

  // -- File Interface --
  int  loadObject(                QString _filename );
  bool saveObject( int _objectId, QString _filename );

private:

  // read binary/text file from disc to scenegraph node
  bool readBinaryFile( const char *_filename, SplatCloud &_splatCloud ) /*const*/;
  bool readTextFile  ( const char *_filename, SplatCloud &_splatCloud ) /*const*/;

  // write binary/text file from scenegraph node to disc
  bool writeBinaryFile( const char *_filename, const SplatCloudNode *_splatCloudNode ) /*const*/;
  bool writeTextFile  ( const char *_filename, const SplatCloudNode *_splatCloudNode ) /*const*/;

  // widgets
  QWidget *loadOptions_;
  QWidget *saveOptions_;

  // options in the loading menu
  QCheckBox *loadBinaryFile_;
  QCheckBox *loadNormals_;
  QCheckBox *loadPointsizes_;
  QCheckBox *loadColors_;
  QComboBox *loadColorRange_;
  QCheckBox *loadIndices_;

  // options in the saving menu
  QCheckBox *saveBinaryFile_;
  QCheckBox *saveNormals_;
  QCheckBox *savePointsizes_;
  QCheckBox *saveColors_;
  QComboBox *saveColorRange_;
  QCheckBox *saveIndices_;

  // buttons
  QPushButton *loadMakeDefaultButton_;
  QPushButton *saveMakeDefaultButton_;

private slots:

  // slots called when the Load/Save colors checkbox or binaryfile checkbox was clicked
  void slotUpdateLoadColorRange();
  void slotUpdateSaveColorRange();

  // slots called when user wants to save the given Load/Save Options as default
  void slotLoadMakeDefaultButtonClicked();
  void slotSaveMakeDefaultButtonClicked();
};


//================================================================


#endif // FILEPTSPLUGIN_HH
