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

#ifndef BASEBACKUP_HH
#define BASEBACKUP_HH

#include <QString>
#include <OpenFlipper/common/BaseObjectData.hh>
#include <OpenFlipper/common/UpdateType.hh>
#include <OpenFlipper/common/perObjectData.hh>
#include <OpenFlipper/common/GlobalDefines.hh>

/**
 * @brief Class that encapsulates a backup
 */
class DLLEXPORT BaseBackup
{

  public:
    explicit BaseBackup(QString _name);

    /** \brief constructor
     *
     * @param _object Pointer to the object, this backup will belong to
     * @param _name   Name of the object
     * @param _type   Type of change which can be used to create more specific backups (e.g. selection only)
     *
     */
    BaseBackup(BaseObjectData* _object, QString _name, UpdateType _type);

    virtual ~BaseBackup();

  public:

    /** \brief Revert this backup
     *
     * This function has to be implemented. It will be called when the backup should be restored.
     *
     * \note You have to call the BaseBackup::apply() first in your implementation of this function.
     *
     * You have to restore your backup to the corresponding object when this function gets called.
     */
    virtual void apply();

    /** \brief Get the backups name)
     *
     */
    QString name();


    /** \brief Set links to corresponding backups
     *
     * Backups can be grouped together. The backups store the links to their group members via
     * this function.
     */
    void setLinks(IdList _objectIDs);

    /** \brief get id of this backup
     *
     * Automatically generated id for this backup.
     */
    int id();

    /** \brief Returns if this backup is blocked
     *
     * When Backups are grouped together, they can not be reversed o their own but
     * oly together with the others in that backup group. This flag indicates, that
     * the backup is part of a group and can not be reversed on its own.
     */
    bool blocked();
    
  protected:
    /// Backup of the perObjectData objects
    std::vector< std::pair<QString, PerObjectData*> > objectDatas_;

    BaseObjectData*  object_;
    QString          name_;
    IdList           links_;

    int              id_;
};

#endif //BASEBACKUP_HH
