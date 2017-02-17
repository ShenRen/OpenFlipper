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

#ifndef QTPLANESELECT_HH
#define QTPLANESELECT_HH

#include <ACG/GL/GLState.hh>
#include <ACG/Math/VectorT.hh>

#include <OpenFlipper/common/ObjectTypeDLLDefines.hh>

#include <ACG/Scenegraph/TransformNode.hh>
#include <ACG/Scenegraph/LineNode.hh>
#include <ObjectTypes/Plane/PlaneNode.hh>

#include <qnamespace.h>
#include <QObject>

#include <list>

class OBJECTTYPEDLLEXPORT QtPlaneSelect : public QObject
{
    Q_OBJECT

    /*******************************************************************************
             Initialization and deinitialization
     *******************************************************************************/
    public:
        explicit QtPlaneSelect( ACG::GLState& glState );
        ~QtPlaneSelect();

    
    /*******************************************************************************
             Public slots.
     *******************************************************************************/
    public slots:

        /*******************************************************************************
                     Call this to notify the scissor about mouse actions.
         *******************************************************************************/
        void slotMouseEvent(QMouseEvent* _event);

        /*******************************************************************************
                     Call this to notify the scissor about mouse actions.
         *******************************************************************************/
        void slotKeyReleaseEvent(QKeyEvent* _event);

    /*******************************************************************************
             Signals ("callbacks")
     *******************************************************************************/
    signals:
        void signalTriggerCut( );
        void updateViewProxy();
        void nodeVisChangedProxy(int _id);


    /*******************************************************************************
            Public accessors.
     *******************************************************************************/
    public:
        ACG::Vec3d           getNormal( ) { return normal; }
        ACG::Vec3d           getSourcePoint( ) { return sourcePoint3D; }
        size_t               getNode() { return nodeIdx_; };
        size_t               getTargetIndex() { return targetIdx_; };

    
    /*******************************************************************************
             Members
     *******************************************************************************/
    protected:
        ACG::GLState&       glState;
        ACG::Vec3d          sourcePoint3D;
        ACG::Vec3d          normal;
        size_t              nodeIdx_;
        size_t              targetIdx_;
        bool                isDragging;

        PlaneNode*         planeNode_;

    private:
        Plane plane_;
        void setPlaneAndSize(const ACG::Vec3d& _sourcePoint3D,const ACG::Vec3d& _target2D);
};



#endif // QTPLANESELECT_HH
