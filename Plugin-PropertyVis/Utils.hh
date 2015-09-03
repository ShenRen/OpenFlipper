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

#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <iostream>
#include <set>
#include <typeinfo>

#include <QObject>
#include <QMessageBox>

#include <ObjectTypes/TriangleMesh/TriangleMesh.hh>
#include <ObjectTypes/PolyMesh/PolyMesh.hh>
#include <ObjectTypes/HexahedralMesh/HexahedralMesh.hh>
#include <ObjectTypes/PolyhedralMesh/PolyhedralMesh.hh>

/*! \class TypeInfoWrapper
 *  \brief Wraps the information of a type.
 *
 * This class is used to handle different property types. It provides a human readable
 * string representation of the type and can also be used to find out which type a
 * property has.
 */
class TypeInfoWrapper {
    public:
        TypeInfoWrapper(const std::type_info & ti, const char *friendlyName) : ti(&ti), friendlyName(friendlyName) {}
        TypeInfoWrapper(const std::type_info & ti) : ti(&ti),friendlyName("") {}

        operator const std::type_info *() const { return ti; }
        operator const std::type_info &() const { return *ti; }
        operator const char *() const { return friendlyName; }

        const std::type_info *operator->() const { return ti; }
        const std::type_info &get() const { return *ti; }
        const char *getName() const { return friendlyName; }

        bool operator==(const TypeInfoWrapper & other) const {
            /*
             * We compare name strings, not the type ids themselves because
             * the same type from different SOs will give different type ids.
             */
            return strcmp(ti->name(), other.ti->name()) == 0;
        }

        bool operator<(const TypeInfoWrapper & other) const {
            return strcmp(ti->name(), other.ti->name()) < 0;
        }

    private:
        const std::type_info *ti;
        const char *friendlyName;
};

/*! \class PropertyInfo
 *  \brief Cellection of information about a property.
 *
 * This class combines all the interesting information about a property. It stores
 * the entity type (Vertex, Edge, ...), the properties name and its type.
 */
class PropertyInfo {
    public:
        enum ENTITY_FILTER {
            EF_ANY      = 0xFF,
            EF_FACE     = 0x01,
            EF_EDGE     = 0x02,
            EF_HALFEDGE = 0x04,
            EF_VERTEX   = 0x08,
            EF_HALFFACE = 0x10,
            EF_CELL     = 0x20
        };

        /// Returns a symbol representation for an entity
        static const char* entity2str(ENTITY_FILTER entity);

        PropertyInfo(const std::string &propName, const TypeInfoWrapper &typeinfo, ENTITY_FILTER entity) :
            propName_(propName), typeinfo_(typeinfo), entity(entity) {}

        QString toString() const {
            return QObject::tr("%3 %1 : %2").arg(propName_.c_str()).arg(friendlyTypeName()).arg(QString::fromUtf8(entity2str(entity)));
        }

        bool operator==(const PropertyInfo &rhs) const {
            return propName_ == rhs.propName_ && typeinfo_ == rhs.typeinfo_ && entity == rhs.entity;
        }

        bool operator<(const PropertyInfo &rhs) const {
            if (entity != rhs.entity) return entity < rhs.entity;

            int result = propName_.compare(rhs.propName_);
            if (result) return result < 0;

            return typeinfo_ < rhs.typeinfo_;
        }

        inline bool isCellProp() const     { return entity == EF_CELL;     }
        inline bool isFaceProp() const     { return entity == EF_FACE;     }
        inline bool isHalffaceProp() const { return entity == EF_HALFFACE; }
        inline bool isEdgeProp() const     { return entity == EF_EDGE;     }
        inline bool isHalfedgeProp() const { return entity == EF_HALFEDGE; }
        inline bool isVertexProp() const   { return entity == EF_VERTEX;   }

        inline const std::string &propName() const     { return propName_;           }
        inline const char *friendlyTypeName() const    { return typeinfo_.getName(); }
        inline const TypeInfoWrapper &typeinfo() const { return typeinfo_;           }
        inline ENTITY_FILTER entityType() const        { return entity;              }

    private:
        std::string propName_;
        TypeInfoWrapper typeinfo_;
        ENTITY_FILTER entity;
};


/*! \class NewNameMessageBox
 *  \brief Asks the user how to proceed after a name clash.
 *
 * When loading a property the provided name can already be in use. This message box is
 * used to ask the user how to proceed. Cancel loading, set a new name or replace the
 * old property by the new one?
 */
class NewNameMessageBox: public QMessageBox
{
    Q_OBJECT

public:
    NewNameMessageBox(QString propName);

private slots:
    void slotReplace() { replace = true; }
    void slotRename()  { rename = true; }
    void slotCancel()  { cancel = true; }

private:
    QLabel* problemDescription;

    QPushButton* buttonRename;
    QPushButton* buttonReplace;
    QPushButton* buttonCancel;

    QString propName;

public:
    bool replace;
    bool rename;
    bool cancel;

};

static DataType supportedDataTypes()
{
    DataType res = DATA_TRIANGLE_MESH | DATA_POLY_MESH;

    #ifdef ENABLE_OPENVOLUMEMESH_POLYHEDRAL_SUPPORT
    res |= DATA_POLYHEDRAL_MESH;
    #endif

    #ifdef ENABLE_OPENVOLUMEMESH_HEXAHEDRAL_SUPPORT
    res |= DATA_HEXAHEDRAL_MESH;
    #endif

    return res;
}

#endif /* UTILS_H */
