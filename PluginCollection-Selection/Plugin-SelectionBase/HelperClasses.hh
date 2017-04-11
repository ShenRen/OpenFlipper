/*===========================================================================*\
 *                                                                           *
 *                              OpenFlipper                                  *
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
 *                                                                           *
\*===========================================================================*/

/*===========================================================================*\
 *                                                                           *
 *   $Revision$                                                       *
 *   $Author$                                                       *
 *   $Date$                   *
 *                                                                           *
\*===========================================================================*/

#ifndef HELPERCLASSES_HH_INCLUDED
#define HELPERCLASSES_HH_INCLUDED

#include <OpenFlipper/common/DataTypes.hh>

#include <QAction>
#include <QPushButton>
#include <QGridLayout>

class HandleAction : public QAction {
    
    public:
        /// Default constructor
        HandleAction(QIcon _icon, QString _description, QObject* _parent = 0, DataType _objectTypeRestriction = DATA_ALL) :
            QAction(_icon, _description, _parent),
            associatedTypes_(0u),
            objectTypeRestriction_(_objectTypeRestriction) {}
       
        /// Default destructor
        ~HandleAction() {}
        
        /// Get/Set selection environment handle name
        void selectionEnvironmentHandle(QString _handle) { selectionEnvHandle_ = _handle; }
        QString selectionEnvironmentHandle() { return selectionEnvHandle_; }
        
        /// Get/Set selection mode handle name
        void selectionModeHandle(QString _handle) { selectionModeHandle_ = _handle; }
        QString selectionModeHandle() { return selectionModeHandle_; }
        
        /// Get/Set associated primitive types
        void addAssociatedType(unsigned int _associatedType) { associatedTypes_ |= _associatedType; }
        void removeAssociatedType(unsigned int _associatedType) { associatedTypes_ &= ~_associatedType; }
        bool isAssociated(unsigned int _type, bool _associatedWithAll = false) {

            if(_associatedWithAll && _type != 0u)
                return ((associatedTypes_ | _type) == associatedTypes_);
            else
                return ((associatedTypes_ & _type) != 0);
        }
        
        void objectTypeRestriction(const DataType& _type) {
            objectTypeRestriction_ = _type;
        }

        const DataType& objectTypeRestriction() const {
            return objectTypeRestriction_;
        }

    private:
        QString selectionEnvHandle_;
        QString selectionModeHandle_;
        
        unsigned int associatedTypes_;

        DataType objectTypeRestriction_;
};

class PrimitiveAction : public QAction {
    
    public:
        /// Default constructor
        PrimitiveAction(QIcon _icon, QString _description, QObject* _parent = 0) :
            QAction(_icon, _description, _parent),
            primitiveType_(0),
            selectionEnvironmentHandle_("")
        {};
       
        /// Default destructor
        ~PrimitiveAction() {};
        
        /// Get/Set primitive type
        void primitiveType(unsigned int _type) { primitiveType_ = _type; }
        unsigned int primitiveType() { return primitiveType_; }
        
        /// Get/Set associated selection environment handle
        void selectionEnvironmentHandle(const QString _handle) {
            selectionEnvironmentHandle_ = _handle;
        }
        QString selectionEnvironmentHandle() const {
            return selectionEnvironmentHandle_;
        }
        
    private:
        unsigned int primitiveType_;
        QString selectionEnvironmentHandle_;
};

class ActionButton : public QPushButton {
    Q_OBJECT
    
    public:
        /// Default constructor
        ActionButton(QAction* _action, QWidget* _parent = 0) :
            QPushButton(_parent), action_(_action) {
            // Initialize button with action parameters
            setCheckable(true);
            setIcon(action_->icon());
            setIconSize(QSize(48,48));
            setToolTip(action_->text());
            
            connect(action_, SIGNAL(toggled(bool)), this, SLOT(setChecked(bool)));
            connect(this, SIGNAL(toggled(bool)), action_, SLOT(setChecked(bool)));
        };
        
        /// Default destructor
        ~ActionButton() {};

    private slots:

        // Avoid back sending of signal (this results in an infinite loop)
        void setChecked(bool _checked) {
            blockSignals(true);
            QPushButton::setChecked(_checked);
            blockSignals(false);
        }

        void changeRegistered() {
            setChecked(action_->isChecked());
        }
        
    private:
        QAction* action_;
};

class FillingLayout : public QGridLayout {
    public:
        /// Default Constructor
        explicit FillingLayout(int _numColumns) :
            QGridLayout(),
            currentRow_(0),
            currentColumn_(0),
            numColumns_(_numColumns) {};
            
        void addWidget(QWidget* _widget) {
            
            QGridLayout::addWidget(_widget, currentRow_, currentColumn_);
            
            currentColumn_++;
            if(currentColumn_ == numColumns_) {
                currentColumn_ = 0;
                currentRow_++;
            }
        };
        
    private:
        unsigned int currentRow_;
        unsigned int currentColumn_;
        unsigned int numColumns_;
};

#endif // HELPERCLASSES_HH_INCLUDED
