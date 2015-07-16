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

#ifdef ENABLE_OPENVOLUMEMESH_SUPPORT

#ifndef OVM_PROPERTY_MODEL_H
#define OVM_PROPERTY_MODEL_H

#include "../PropertyModel.hh"
#include "OVMPropertyVisualizer.hh"
#include "OVMPropertyVisualizerBoolean.hh"
#include "OVMPropertyVisualizerDouble.hh"
#include "OVMPropertyVisualizerInteger.hh"
#include "OVMPropertyVisualizerVector.hh"
#include "OVMPropertyVisualizerVectorFieldDifference.hh"

#include "../Utils.hh"
#include <OpenVolumeMesh/Core/BaseProperty.hh>

#include "Widgets/LoadSaveWidget.hh"
#include "Widgets/PickWidget.hh"

#include <vector>

#include "../Utils.hh"

#define PROP_VIS "PropertyVisualization"

class PropertyVisPlugin;

/* This class adds signal/slot support for OVMPropertyModelSubclass. This needs
 * to be done because "Sorry, Qt does not support templates that contain signals,
 * slots or Q_OBJECT"
 * http://doc.trolltech.com/qq/qq15-academic.html
 */
class OVMPropertyModelSubclass: public PropertyModel
{
Q_OBJECT
public:
    OVMPropertyModelSubclass(QObject *parent = 0)
        : PropertyModel(parent)
    {
    }

public slots:
    virtual void slotCombine(){ combine(); }
    virtual void slotSaveProperty(){ saveProperty(); }
    virtual void slotLoadProperty(){ loadProperty(); }
    virtual void slotPickProperty(){ pickProperty(); }

private:
    virtual void combine(){}
    virtual void saveProperty(){}
    virtual void pickProperty(){}

};

template<typename MeshT>
class OVMPropertyModel: public OVMPropertyModelSubclass {

public:
    OVMPropertyModel(MeshT* mesh, int objectID, QObject *parent = 0);
    virtual ~OVMPropertyModel(){}

    virtual void updateWidget(const QModelIndexList& selectedIndices);

    /// Disables picking.
    void resetPicking();

    /// Toggle picking on and off.
    virtual void pickProperty();

    /// Handles changing of pick mode.
    virtual void pickModeChanged(const std::string& _mode);

    /// Handles mouse events for picking.
    virtual void mouseEvent(QMouseEvent* _event);

    virtual bool parseHeader(QString header, PropertyVisualizer*& propVis, unsigned int& n);

protected:

    virtual QString getLoadFilenameFilter();
    virtual QString getSaveFilenameFilter(unsigned int propId);

private:

    /// Combines two properties.
    virtual void combine();

    /// Checks if two properties are combinable.
    bool combinable(PropertyVisualizer* propertyVisualizer1, PropertyVisualizer* propertyVisualizer2) const;

    /// Saves the currently slected properties.
    virtual void saveProperty();

    /// Searches for properties with a specific entity type and creates visualizers.
    void gatherProperties( MeshT* mesh,
                           typename MeshT::Properties::const_iterator props_first,
                           typename MeshT::Properties::const_iterator props_last,
                           PropertyInfo::ENTITY_FILTER filter);
    /// Searches for all properties and creates the visualizers.
    void gatherProperties();

    /// Checks if visualizing this property is supported.
    bool isSupported(OpenVolumeMesh::BaseProperty* const baseProp) const;

    /// Checks if visualizing this type of property is supported.
    bool isSupported(QString friendlyName) const;

    /// Checks if we already created a PropertyVisualizer for this property.
    bool isNew(OpenVolumeMesh::BaseProperty* const baseProp, PropertyInfo::ENTITY_FILTER filter) const;

    /// Checks if a property name is still available for an entity type and a property type.
    bool isPropertyFree(QString propName, PropertyInfo::ENTITY_FILTER filter, TypeInfoWrapper typeInfo);

    /// Returns the TypeInfoWrapper for the property if it is supported.
    TypeInfoWrapper getSupportedTypeInfoWrapper(OpenVolumeMesh::BaseProperty* const baseProp) const;

    /// Returns the TypeInfoWrapper for the type of property if it is supported.
    TypeInfoWrapper getSupportedTypeInfoWrapper(QString friendlyName, PropertyInfo::ENTITY_FILTER filter) const;

    bool isBoolType(const PropertyInfo& propInfo) const;
    bool isIntType(const PropertyInfo& propInfo) const;
    bool isDoubleType(const PropertyInfo& propInfo) const;
    bool isUnsignedIntType(const PropertyInfo& propInfo) const;
    bool isVec3dType(const PropertyInfo& propInfo) const;
    bool isVec3fType(const PropertyInfo& propInfo) const;
    bool isVectorType(const PropertyInfo& propInfo) const;
    bool isVectorType(const TypeInfoWrapper& typeInfo) const;

    bool isEntityType(const TypeInfoWrapper& typeInfo, PropertyInfo::ENTITY_FILTER entity_type) const;

    /// Adds a new PropertyVisualizer.
    void addPropertyVisualizer(OpenVolumeMesh::BaseProperty* const baseProp, MeshT* mesh, PropertyInfo::ENTITY_FILTER filter);

    /// Adds a new property to the mesh.
    void addProperty(QString propName, QString friendlyTypeName, PropertyInfo::ENTITY_FILTER filter);

    void initializeSupportedPropertyTypes();
    
    MeshT* mesh_;

    int objectID_;

    QPushButton bCombine;
    const PropertyInfo* mCombineProperty1;
    const PropertyInfo* mCombineProperty2;

    LoadSaveWidget mLoadSaveWidget;

    PickWidget mPickWidget;

    bool pickModeActive;
    std::string lastPickMode;
    Viewer::ActionMode lastActionMode;

#define DECLARE_PROPTYPES(primitive)                                 \
    TypeInfoWrapper proptype_##primitive##_bool;                     \
    TypeInfoWrapper proptype_##primitive##_int;                      \
    TypeInfoWrapper proptype_##primitive##_uint;                     \
    TypeInfoWrapper proptype_##primitive##_double;                   \
    TypeInfoWrapper proptype_##primitive##_Vec3d;                    \
    TypeInfoWrapper proptype_##primitive##_Vec3f;

    DECLARE_PROPTYPES(Cell)
    DECLARE_PROPTYPES(Face)
    DECLARE_PROPTYPES(HalfFace)
    DECLARE_PROPTYPES(Edge)
    DECLARE_PROPTYPES(HalfEdge)
    DECLARE_PROPTYPES(Vertex)

#undef DECLARE_PROPTYPES


    typedef std::set<TypeInfoWrapper> TypeInfoWrapperSet;
    TypeInfoWrapperSet supportedPropertyTypes;

};


#if defined(INCLUDE_TEMPLATES) && !defined(OVM_PROPERTY_MODEL_CC)
#include "OVMPropertyModelT.cc"
#endif

#endif /* OVM_PROPERTY_MODEL_H */

#endif /* ENABLE_OPENVOLUMEMESH_SUPPORT */
