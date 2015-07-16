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




#ifndef VIEWMODEWIDGET_HH
#define VIEWMODEWIDGET_HH

#include "ui_viewMode.hh"
#include <QStringList>
#include <QVector>

struct ViewMode;

class viewModeWidget : public QDialog, public Ui::viewMode
{
  Q_OBJECT
  private:
    // Vector holding list of all available modes
    const QVector< ViewMode* >& modes_;
  public:
    viewModeWidget(const QVector< ViewMode* >& _modes, QWidget *parent = 0 );
    void show(QString _lastMode);
    //void toolbarList(Qt::ContextMenuPolicy arg1);
  private slots:

    void slotModeChanged(QString _mode);
    void slotModeClicked(QModelIndex _id);
   
    
    // ============================================
    // ViewMode Buttons
    // ============================================   
      /** \brief Button slot to remove the selected view mode
      *
      * This slot removes the currently selected view mode 
      */    
      void slotRemoveMode();
      
      /** \brief Button slot to copy the selected view mode
      *
      * This slot copies the currently selected view mode 
      */    
      void slotCopyMode();
      
      /** \brief Button slot to add a new empty mode
      *
      * This slot creates an new empty view mode
      */    
      void slotAddMode();
      

    
    
    // ============================================
    // ViewMode Context Menu
    // ============================================
      /** \brief Context Menu View Modes
      *
      * This slot shows the used toolbar Context Menu
      */
      void slotModeContextMenu ( const QPoint & _pos );
      
      /** \brief Context menu slot to change the icon for a view mode
      *
      * This slot asks the user to change the icon of the given view mode
      */    
      void slotSetIcon();      
    
    // ============================================
    // ToolBox and ToolBar Lists update functions
    // ============================================
      /** \brief Update list views
      *
      * This slot updates all list views depending on the currently selected view mode
      */
      void slotSetAllWidgets();
 
    // ============================================
    // ToolBar Views Context Menu
    // ============================================
      /** \brief Context Menu Used Toolbars
      *
      * This slot shows the used toolbar Context Menu
      */
      void slotUsedToolbarContextMenu ( const QPoint & _pos );  
      
      /** \brief Context Menu Available Toolbars
      *
      * This slot shows the available toolbar Context Menu
      */
      void slotAvailableToolbarContextMenu ( const QPoint & _pos );    
    
    // ============================================
    // ToolBox Views Context Menu
    // ============================================
      /** \brief Context Menu Used Toolboxes
      *
      * This slot shows the used toolbox Context Menu
      */
      void slotUsedToolboxContextMenu ( const QPoint & _pos );  
      
      /** \brief Context Menu Available Toolboxes
      *
      * This slot shows the available toolbox Context Menu
      */
      void slotAvailableToolboxContextMenu ( const QPoint & _pos ); 
    
    // ============================================
    // ContextMenu Views Context Menu
    // ============================================
      /** \brief Context Menu Used ContextMenus
      *
      * This slot shows the used ContextMenu Context Menu
      */
      void slotUsedContextMenuContextMenu ( const QPoint & _pos );  
      
      /** \brief Context Menu Available ContextMenus
      *
      * This slot shows the available ContextMenu Context Menu
      */
      void slotAvailableContextMenuContextMenu ( const QPoint & _pos );
  
  
    // ============================================
    // ToolBar Buttons
    // ============================================
      /** \brief remove Toolbars from Mode
      *
      * This slot removes the selected toolbars from the current view Mode
      */
      void slotRightArrowToolbar();
      
      /** \brief add Toolbars to Mode
      *
      * This slot adds the selected toolbars to the current view Mode
      */
      void slotLeftArrowToolbar();
    
    // ============================================
    //ToolBox Buttons
    // ============================================
      /** \brief remove Toolboxes from Mode
      *
      * This slot removes the selected widgets from the current view Mode
      */
      void slotRightArrowToolbox();
      
      /** \brief add Toolboxes to Mode
      *
      * This slot adds the selected widgets to the current view Mode
      */
      void slotLeftArrowToolbox();
    
      /** \brief Move Toolbox up
      *
      * This slot moves the widget upward in the list of used modes
      */
      void slotMoveToolboxUp();
      
      /** \brief Move Toolbox down
      *
      * This slot moves the widget downward in the list of used modes
      */
      void slotMoveToolboxDown();
      
    // ============================================
    //ContextMenu Buttons
    // ============================================
    /** \brief remove ContextMenu from Mode
      *
      * This slot removes the selected widgets from the current view Mode
      */
      void slotRightArrowContextMenu();
      
      /** \brief add ContextMenu to Mode
      *
      * This slot adds the selected widgets to the current view Mode
      */
      void slotLeftArrowContextMenu();
    
      /** \brief Move ContextMenu up
      *
      * This slot moves the widget upward in the list of used modes
      */
      void slotMoveContextMenuUp();
      
      /** \brief Move Toolbox down
      *
      * This slot moves the widget downward in the list of used modes
      */
      void slotMoveContextMenuDown();
      
      
    // ============================================
    // External Communication
    // ============================================     
    
    private slots:
      /// Slot for changing the current view to currently configured one
      void slotChangeView();
      
      /** \brief Save the current view mode configuration
      *
      * Takes the given configuration and saves the mode
      */
      void slotSaveMode();
    
    signals:
      /// Changes the view mode to the currently configured one
      void changeView(QString _mode, QStringList _toolboxWidgets, QStringList _toolbars, QStringList _contextmenus);
      
      /// saves the given mode
      void saveMode(QString _name, bool _custom, QStringList _toolboxWidgets, QStringList _toolbars, QStringList _contextmenus);
      
      /// This signal is emitted to remove a mode
      void removeMode(QString _name);
      
      
};

#endif //VIEWMODEWIDGET_HH
