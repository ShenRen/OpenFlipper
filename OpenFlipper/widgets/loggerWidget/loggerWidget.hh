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




#ifndef LOGGERWIDGET_HH
#define LOGGERWIDGET_HH

#include <QListWidget>

#include <OpenFlipper/BasePlugin/LoggingInterface.hh>

#if QT_VERSION >= 0x050000 
  #include <QtWidgets>
#else
  #include <QtGui>
#endif

/** \brief Implementation of the logger Widget
 *
 * This class adds some special features to the textedit for the log window
 */
class LoggerWidget : public QWidget
{

  Q_OBJECT

  public:
    explicit LoggerWidget( QWidget *parent = 0 );
    
    ~LoggerWidget();

    /** \brief Append a new logmessage to log viewer
    *
    * Display log message of given logtype
    */
    void append(const QString& _text, Logtype _type);
    
  protected:

    /** \brief Grab key events
    *
    * This function grabs all key events and passes them back to the core to handle them correctly
    */
    void keyPressEvent (QKeyEvent * _event );
    
    /** \brief Show context menu
    *
    * This function shows the context menu
    */
    void contextMenuEvent ( QContextMenuEvent * event );
    
    /** \brief Called when the widget is shown
    *
    * Function scrolls to bottom if the object gets visible
    */
    void showEvent ( QShowEvent * event );
  
  private:
    
    QListWidget* list_;
    
    bool blockNext_;
    
    QPushButton* allButton_;
    QPushButton* infoButton_;
    QPushButton* warnButton_;
    QPushButton* errorButton_;
    
    QPushButton* filterButton_;
    
    QPushButton* clearButton_;
    
    QMenu* context_;
    
    QMenu* filterMenu_;
    
    QTimer loggerUpdateTimer_;
    
    // true if new data is available
    bool newData_;
    
  public:
    // Action for OpenMesh filters
    QAction* openMeshFilterAction_;
    
  private slots:
    /// update the list if a button was pressed
    void updateList();
    
    /// copy Selected rows to clipboard
    void copySelected();
    
    /// Called when we want to scroll to the bottom
    void slotScrollUpdate();
    
    /// Called when filter button is pressed
    void slotFilterMenu();
};

#endif //LOGGERWIDGET_HH
