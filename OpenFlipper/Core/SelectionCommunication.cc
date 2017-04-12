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
 *   $Revision: 9089 $                                                       *
 *   $Author: kremer $                                                       *
 *   $Date: 2010-04-21 18:42:47 +0200 (Mi, 21. Apr 2010) $                   *
 *                                                                           *
\*===========================================================================*/

//=============================================================================
//
//  CLASS Core - IMPLEMENTATION of Comunication with plugins
//
//=============================================================================

//== INCLUDES =================================================================

#include "Core.hh"

//== IMPLEMENTATION ==========================================================

//========================================================================================
// ===             Selection Communication                    ============================
//========================================================================================

void Core::slotAddSelectionEnvironment(QString _modeName, QString _description, QString _icon, QString& _handleName) {
    emit addSelectionEnvironment(_modeName, _description, _icon, _handleName);
}

void Core::slotRegisterType(QString _handleName, DataType _type) {
    emit registerType(_handleName, _type);
}

void Core::slotAddPrimitiveType(QString _handleName, QString _name, QString _icon, SelectionInterface::PrimitiveType& _typeHandle) {
    emit addPrimitiveType(_handleName, _name, _icon, _typeHandle);
}

void Core::slotAddCustomSelectionMode(QString _handleName, QString _modeName, QString _description, QString _icon,
                                      SelectionInterface::PrimitiveType _associatedTypes, QString& _customIdentifier) {
    emit addCustomSelectionMode(_handleName, _modeName, _description, _icon, _associatedTypes, _customIdentifier);
}

void Core::slotAddCustomSelectionMode(QString _handleName, QString _modeName, QString _description, QString _icon,
                                      SelectionInterface::PrimitiveType _associatedTypes, QString& _customIdentifier,
                                      DataType _objectTypeRestriction) {
    emit addCustomSelectionMode(_handleName, _modeName, _description, _icon, _associatedTypes, _customIdentifier, _objectTypeRestriction);
}

void Core::slotAddSelectionOperations(QString _handleName,QStringList _operationsList, QString _category, SelectionInterface::PrimitiveType _type) {
    emit addSelectionOperations(_handleName, _operationsList, _category, _type);
}

void Core::slotAddSelectionParameters(QString _handleName,QWidget* _widget, QString _category, SelectionInterface::PrimitiveType _type) {
    emit addSelectionParameters(_handleName, _widget, _category, _type);
}
   
void Core::slotSelectionOperation(QString _operation) {
    emit selectionOperation(_operation);
}

void Core::slotShowToggleSelectionMode(QString _handleName, bool _show, SelectionInterface::PrimitiveType _associatedTypes) {
    emit showToggleSelectionMode(_handleName, _show, _associatedTypes);
}

void Core::slotShowLassoSelectionMode(QString _handleName, bool _show, SelectionInterface::PrimitiveType _associatedTypes) {
    emit showLassoSelectionMode(_handleName, _show, _associatedTypes);
}

void Core::slotShowVolumeLassoSelectionMode(QString _handleName, bool _show, SelectionInterface::PrimitiveType _associatedTypes) {
    emit showVolumeLassoSelectionMode(_handleName, _show, _associatedTypes);
}

void Core::slotShowSurfaceLassoSelectionMode(QString _handleName, bool _show, SelectionInterface::PrimitiveType _associatedTypes) {
    emit showSurfaceLassoSelectionMode(_handleName, _show, _associatedTypes);
}

void Core::slotShowSphereSelectionMode(QString _handleName, bool _show, SelectionInterface::PrimitiveType _associatedTypes) {
    emit showSphereSelectionMode(_handleName, _show, _associatedTypes);
}

void Core::slotShowClosestBoundarySelectionMode(QString _handleName, bool _show, SelectionInterface::PrimitiveType _associatedTypes) {
    emit showClosestBoundarySelectionMode(_handleName, _show, _associatedTypes);
}

void Core::slotShowFloodFillSelectionMode(QString _handleName, bool _show, SelectionInterface::PrimitiveType _associatedTypes) {
    emit showFloodFillSelectionMode(_handleName, _show, _associatedTypes);
}

void Core::slotShowComponentsSelectionMode(QString _handleName, bool _show, SelectionInterface::PrimitiveType _associatedTypes) {
    emit showComponentsSelectionMode(_handleName, _show, _associatedTypes);
}

void Core::slotToggleSelection(QMouseEvent* _event, SelectionInterface::PrimitiveType _currentType, bool _deselect) {
    emit toggleSelection(_event, _currentType, _deselect);
}

void Core::slotLassoSelection(QMouseEvent* _event, SelectionInterface::PrimitiveType _currentType, bool _deselect) {
    emit lassoSelection(_event, _currentType, _deselect);
}

void Core::slotVolumeLassoSelection(QMouseEvent* _event, SelectionInterface::PrimitiveType _currentType, bool _deselect) {
    emit volumeLassoSelection(_event, _currentType, _deselect);
}

void Core::slotSurfaceLassoSelection(QMouseEvent* _event, SelectionInterface::PrimitiveType _currentType, bool _deselect) {
    emit surfaceLassoSelection(_event, _currentType, _deselect);
}

void Core::slotSphereSelection(QMouseEvent* _event, double _radius, SelectionInterface::PrimitiveType _currentType, bool _deselect) {
    emit sphereSelection(_event, _radius, _currentType, _deselect);
}

void Core::slotClosestBoundarySelection(QMouseEvent* _event, SelectionInterface::PrimitiveType _currentType, bool _deselect) {
    emit closestBoundarySelection(_event, _currentType, _deselect);
}

void Core::slotFloodFillSelection(QMouseEvent* _event, SelectionInterface::PrimitiveType _currentType, bool _deselect) {
    emit floodFillSelection(_event, _currentType, _deselect);
}

void Core::slotComponentsSelection(QMouseEvent* _event, SelectionInterface::PrimitiveType _currentType, bool _deselect) {
    emit componentsSelection(_event, _currentType, _deselect);
}

void Core::slotCustomSelection(QMouseEvent* _event, SelectionInterface::PrimitiveType _currentType, QString _customIdentifier, bool _deselect) {
    emit customSelection(_event, _currentType, _customIdentifier, _deselect);
}

void Core::slotGetActiveDataTypes(SelectionInterface::TypeList& _types) {
    emit getActiveDataTypes(_types);
}

void Core::slotGetActivePrimitiveType(SelectionInterface::PrimitiveType& _type) {
    emit getActivePrimitiveType(_type);
}

void Core::slotTargetObjectsOnly(bool& _targetsOnly) {
    emit targetObjectsOnly(_targetsOnly);
}

void Core::slotLoadSelection(const INIFile& _file) {
    emit loadSelection(_file);
}

void Core::slotSaveSelection(INIFile& _file) {
    emit saveSelection(_file);
}

void Core::slotRegisterKeyShortcut(int _key, Qt::KeyboardModifiers _modifiers) {
    emit registerKeyShortcut(_key, _modifiers);
}

void Core::slotKeyShortcutEvent(int _key, Qt::KeyboardModifiers _modifiers) {
    emit keyShortcutEvent(_key, _modifiers);
}
