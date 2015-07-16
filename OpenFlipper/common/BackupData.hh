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

#ifndef BACKUPDATA_HH
#define BACKUPDATA_HH

#include <QString>
#include <OpenFlipper/common/perObjectData.hh>
#include <OpenFlipper/common/UpdateType.hh>
#include <OpenFlipper/common/BaseBackup.hh>
#include <OpenFlipper/common/GlobalDefines.hh>

#define OBJECT_BACKUPS "Object Backups"

/**
 * @brief Abstract class that is used to store backups
 *
 * The default implementation can only backup perObjectDatas
 */
class DLLEXPORT BackupData : public PerObjectData
{

  public:
    /// Constructor
    BackupData(BaseObjectData* _object = 0);
    /// Destructor
    virtual ~BackupData();

  public:
    /// store a backup
    void storeBackup(BaseBackup* _backup);

    /// perform an undo if possible
    void undo();
    /// perform a redo if possible
    void redo();

    /// return the id of the next undo backup
    int undoID();
    /// return the id of the next redo backup
    int redoID();
    /// return the id of the current backup state
    int currentID();
    
    /// return the name of the next undo backup
    QString undoName();
    /// return the name of the next redo backup
    QString redoName();

    /// return if an undo backup is available
    bool undoAvailable();
    /// return if a redo backup is available
    bool redoAvailable();

    /// return if an undo backup is blocked
    bool undoBlocked();
    /// return if a redo backup is blocked
    bool redoBlocked();

    /// remove all backups
    void clear();

    /// add links to other backups for the current state
    /// this is used to ensure that the backups are only applied simultaneously
    void setLinks(IdList _objectIDs);

    /// return the current state
    BaseBackup* currentState();

    /// return the maximum of backups which are saved
    size_t maxBackups();

    /// set the maximum of saved backups
    void setMaxBackups(size_t _max);

  protected:
    std::vector< BaseBackup* > undoStates_;
    std::vector< BaseBackup* > redoStates_;
    BaseBackup* currentState_;

    BaseObjectData* object_;

    size_t maxBackups_;
};

#endif //BACKUPDATA_HH
