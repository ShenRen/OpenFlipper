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

#ifndef OM_PROPERTY_MODEL_H
#define OM_PROPERTY_MODEL_H

#include "../PropertyModel.hh"
#include "OMPropertyVisualizer.hh"

#include "Widgets/LoadSaveWidget.hh"
#include "Widgets/PickWidget.hh"

#include <OpenFlipper/common/ViewerProperties.hh>

class PropertyVisPlugin;

/*! \class OMPropertyModelSubclass
 *  \brief Added for signal/slot support.
 *
 * This class adds signal/slot support for OMPropertyModelSubclass. OMPropertyModel is
 * then derived from this class to make use of those signals and slots. This needs
 * to be done because "Sorry, Qt does not support templates that contain signals,
 * slots or Q_OBJECT"
 * http://doc.trolltech.com/qq/qq15-academic.html
 */
class OMPropertyModelSubclass: public PropertyModel
{
Q_OBJECT
public:
    OMPropertyModelSubclass(QObject *parent = 0)
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
class OMPropertyModel: public OMPropertyModelSubclass {

public:
    OMPropertyModel(MeshT* mesh, int objectID, QObject *parent = 0);
    virtual ~OMPropertyModel(){}

    virtual void updateWidget(const QModelIndexList& selectedIndices);

protected:

    virtual QString getLoadFilenameFilter();
    virtual QString getSaveFilenameFilter(unsigned int propId);

private:

    /// Combines two properties.
    virtual void combine();

    /// Checks if two properties are combinable.
    bool combinable(PropertyVisualizer* propertyVisualizer1, PropertyVisualizer* propertyVisualizer2);

    /// Saves the currently slected properties.
    virtual void saveProperty();

    virtual bool parseHeader(QString header, PropertyVisualizer*& propVis, unsigned int& n);
    virtual void setPropertyFromFile(QTextStream*& file_stream_, unsigned int n, PropertyVisualizer *propVis);

    /// Disables picking.
    void resetPicking();

    /// Toggle picking on and off.
    virtual void pickProperty();

    /// Handles changing of pick mode.
    virtual void pickModeChanged(const std::string& _mode);

    /// Handles mouse events for picking.
    virtual void mouseEvent(QMouseEvent* _event);

    bool isVectorType(const TypeInfoWrapper& typeInfo) const;

    /// Searches for properties with a specific entity type and creates visualizers.
    void gatherProperties( MeshT* mesh,
                           typename MeshT::prop_iterator props_first,
                           typename MeshT::prop_iterator props_last,
                           PropertyInfo::ENTITY_FILTER filter);
    /// Searches for all properties and creates the visualizers.
    void gatherProperties();

    /// Checks if visualizing this property is supported.
    bool isSupported(OpenMesh::BaseProperty* const baseProp) const;

    /// Checks if visualizing this type of property is supported.
    bool isSupported(QString friendlyName) const;

    /// Checks if we already created a PropertyVisualizer for this property.
    bool isNew(OpenMesh::BaseProperty* const baseProp, PropertyInfo::ENTITY_FILTER filter);

    /// Returns the TypeInfoWrapper for the property if it is supported.
    TypeInfoWrapper getSupportedTypeInfoWrapper(OpenMesh::BaseProperty* const baseProp);

    /// Returns the TypeInfoWrapper for the type of property if it is supported.
    TypeInfoWrapper getSupportedTypeInfoWrapper(QString friendlyName);

    /// Adds a new PropertyVisualizer.
    void addPropertyVisualizer(OpenMesh::BaseProperty* const baseProp, MeshT* mesh, PropertyInfo::ENTITY_FILTER filter);

    /// Adds a new property to the mesh.
    void addProperty(QString propName, QString friendlyTypeName, PropertyInfo::ENTITY_FILTER filter);
    
    void initializeSupportedPropertyTypes();

private:
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


    TypeInfoWrapper proptype_bool;
    TypeInfoWrapper proptype_int;
    TypeInfoWrapper proptype_uint;
    TypeInfoWrapper proptype_double;
    TypeInfoWrapper proptype_Vec3d;
    TypeInfoWrapper proptype_Vec3f;
    TypeInfoWrapper proptype_Vec2d;
    TypeInfoWrapper proptype_Vec2f;
#ifdef ENABLE_SKELETON_SUPPORT
    TypeInfoWrapper proptype_SkinWeights;
#endif

    typedef std::set<TypeInfoWrapper> TypeInfoWrapperSet;
    TypeInfoWrapperSet supportedPropertyTypes;

};

#if defined(INCLUDE_TEMPLATES) && !defined(OM_PROPERTY_MODEL_CC)
#include "OMPropertyModelT.cc"
#endif


#endif /* OM_PROPERTY_MODEL_H */
