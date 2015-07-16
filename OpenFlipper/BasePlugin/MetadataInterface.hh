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
*   $Revision: $                                                       *
*   $LastChangedBy: $                                                *
*   $Date: $                     *
*                                                                            *
\*===========================================================================*/

#ifndef METADATAINTERFACE_HH
#define METADATAINTERFACE_HH

#include <OpenFlipper/common/Types.hh>

#include <QObject>
#include <QString>

#if QT_VERSION >= 0x050000
#include <QJsonDocument>
#endif



/** \file MetadataInterface.hh
 *
 * Interface implementing slots related to the deserialization of meta data
 * (usually encountered in screenshots).
 */

/** \brief Enables implementers to react on deserialization of meta data.
  *
  * \ref MetaDataInterfacePage "Detailed description"
  *
  * Using functions such as BaseObject::getCommentByKey() plugins can attach
  * meta data to objects. This meta data is currently only serialized when
  * taking viewer snapshots. "Plugin-DeserializeScreenshotMetadata" allows
  * deserializing this metadata from a viewer snapshot. (In the future
  * serialization and deserialization of meta data may occur in other contexts
  * as well.)
  *
  * Whenever meta gets deserialized the slots in this interface will get
  * triggered. Please refer to the documentation of the individual slots
  * to find out which specific signal suits your needs.
  *
  * The slots are always triggered in the order
  * slotGenericMetadataDeserialized(), slotObjectMetadataDeserialized(),
  * slotObjectMetadataDeserializedJson().
  */
class MetadataInterface {
    private slots:

        /**
         * This low-level signal is very inconvenient to use and chances are you
         * want to use one of the other signals.
         *
         * Triggered for every top level meta data entry. Currently the only top
         * level meta data entries created by OpenFlipper are "Mesh Comments",
         * "Mesh Materials" and "View". "Mesh Comments" contains the raw
         * concatenation of all object meta data and is very cumbersome to
         * parse. In most cases it's a better idea to use
         * objectMetadataDeserialized().
         */
        virtual void slotGenericMetadataDeserialized(
                QString key, QString value) {};

        /**
         * Triggered for every piece of object specific meta data encapsulated
         * in "Mesh Comments".
         */
        virtual void slotObjectMetadataDeserialized(
                QString object_name, QString value) {};

#if QT_VERSION >= 0x050000
        /**
         * Triggered for every piece of object specific meta data encapsulated
         * in "Mesh Comments" if it is valid JSON.
         */
        virtual void slotObjectMetadataDeserializedJson(
                QString object_name, QJsonDocument value) {};
#endif

    signals:
        /**
         * Can be called by anyone who deserializes meta data, e.g. from a
         * viewer snapshot PNG file. The appropriate slots within
         * this interface will be triggered.
         */
        virtual void metadataDeserialized(
                const QVector<QPair<QString, QString> > &data) = 0;

    public:

        /// Destructor
        virtual ~MetadataInterface() {};

};

/** \page metaDataInterfacePage MetaData Interface
\n
\image html metaDataInterface.png
\n

Using functions such as BaseObject::getCommentByKey() plugins can attach
meta data to objects. This meta data is currently only serialized when
taking viewer snapshots. "Plugin-DeserializeScreenshotMetadata" allows
deserializing this metadata from a viewer snapshot. (In the future
serialization and deserialization of meta data may occur in other contexts
as well.)

Whenever meta gets deserialized the slots in this interface will get
triggered. Please refer to the documentation of the individual slots
to find out which specific signal suits your needs.

*/


#if QT_VERSION >= 0x050000
Q_DECLARE_INTERFACE(MetadataInterface,"OpenFlipper.MetadataInterface_qt5/1.0")
#else
Q_DECLARE_INTERFACE(MetadataInterface,"OpenFlipper.MetadataInterface_qt4/1.0")
#endif

#endif // METADATAINTERFACE_HH
