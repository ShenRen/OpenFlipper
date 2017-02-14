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
#include <ObjectTypes/Light/LightNode.hh>

#include <ui_lightWidget.hh>
#if QT_VERSION >= 0x050000 
  #include <QtWidgets>
#else
  #include <QtGui>
#endif


#include <OpenFlipper/common/ObjectTypeDLLDefines.hh>
#include "Light.hh"

class OBJECTTYPEDLLEXPORT LightWidget : public QDialog, public Ui::LightWidgetBase
{
  Q_OBJECT

  public:
    LightWidget( ACG::SceneGraph::BaseNode* _node, QWidget *parent = 0);
    
  private slots:
    /// The directional checkbox changed -> update object
    void directionalToggled();
    
    /// The fixed position checkbox
    void fixedPositionChanged();
    
    /// Color values have changed
    void ambientChanged();
    void diffuseChanged();
    void specularChanged();
    
    /// Brightness value has changed
    void brightnessChanged(int _newValue);
    
    /// Light radius has changed
    void radiusChanged();
    
    /// Spot direction changed
    void spotChanged();
    
  protected:
    /// Initialize contents of widget before showing it
    virtual void showEvent ( QShowEvent * event );
    
  private:
    /// Initializes the internal object. Returns true if successfull
    bool getObject();
    
    /// Called when the object has been updated
    void updated();
  
  private:
    ACG::SceneGraph::LightNode* node_;
    
    LightObject* object_;
    LightSource* light_;
    
    bool updatingWidgets_;
};

