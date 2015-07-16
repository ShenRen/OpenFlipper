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
//  CLASS Type BSpline Surface Plugin
//
//  Author:  Ellen Dekkers <dekkers@cs.rwth-aachen.de>
//
//  Date: 2009-12-14 19:18:57 +0100 (Mo, 14. Dez 2009)
//
//=============================================================================


#ifndef TYPEBSPLINESURFACEPLUGIN_HH
#define TYPEBSPLINESURFACEPLUGIN_HH

#include <QObject>


#include <OpenFlipper/common/Types.hh>
#include <OpenFlipper/BasePlugin/BaseInterface.hh>
#include <OpenFlipper/BasePlugin/TypeInterface.hh>
#include <OpenFlipper/BasePlugin/LoggingInterface.hh>
#include <OpenFlipper/BasePlugin/LoadSaveInterface.hh>
#include <OpenFlipper/BasePlugin/ContextMenuInterface.hh>

#include <ObjectTypes/BSplineSurface/BSplineSurface.hh>

class TypeBSplineSurfacePlugin : public QObject, BaseInterface, TypeInterface, LoggingInterface, LoadSaveInterface, ContextMenuInterface
{
   Q_OBJECT
   Q_INTERFACES(BaseInterface)
   Q_INTERFACES(TypeInterface)
   Q_INTERFACES(LoggingInterface)
   Q_INTERFACES(LoadSaveInterface)
   Q_INTERFACES(ContextMenuInterface)

#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "org.OpenFlipper.Plugins.Plugin-TypeBSplineSurface")
#endif

  signals:

    void log(Logtype _type, QString _message);
    void log(QString _message);
    
    // LoadSave Interface
    void emptyObjectAdded( int _id );

    // ContextMenuInterface
    void addContextMenuItem(QAction* _action , ContextMenuType _type);
    void addContextMenuItem(QAction* _action , DataType _objectType , ContextMenuType _type );

  private slots:

    // BaseInterface
    void pluginsInitialized();
    void noguiSupported( ) {} ;

    // ContextMenuInterface
    void slotUpdateContextMenu( int _objectId );


  private slots:

    /// Slot triggered from context menu, if the control net should be rendered
    void slotRenderControlNet();

    /// Slot triggered from context menu, if the surface should be rendered
    void slotRenderSurface();

    /// Slot triggered from context menu, if the selection rendering should be altered
    void slotRenderSelection(QAction* _action);

  public :

     ~TypeBSplineSurfacePlugin() {};
     TypeBSplineSurfacePlugin();

     QString name() { return (QString("TypeBSplineSurface")); };
     QString description( ) { return (QString(tr("Register BSpline Surface type "))); };
     
     bool registerType();

     DataType supportedType();
     
     int addEmpty();
     
  public slots:

    QString version() { return QString("1.1"); };

  private:
    /// Context menu action
    QAction* renderControlNetAction_;

    /// Context menu action
    QAction* renderSurfaceAction_;

    /// Context menu action (render selection texture)
    QAction* renderCPSelectionAction_;
    QAction* renderKnotSelectionAction_;
    QAction* renderNoSelectionAction_;

};

#endif //TYPEBSPLINESURFACEPLUGIN_HH
