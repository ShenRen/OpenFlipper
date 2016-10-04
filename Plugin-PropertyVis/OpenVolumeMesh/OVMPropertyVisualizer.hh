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

#ifndef OVM_PROPERTY_VISUALIZER_HH
#define OVM_PROPERTY_VISUALIZER_HH

#include "../PropertyVisualizer.hh"

#include <ObjectTypes/VolumeMeshObject/VolumeMeshObject.hh>

#include <OpenFlipper/BasePlugin/PluginFunctionsViewControls.hh>

#include <ObjectTypes/VolumeMeshObject/VolumeMeshDrawModesContainer.hh>

#include <ACG/QtWidgets/QtHistogramWidget.hh>

#include <iostream>

template <typename MeshT>
class OVMPropertyVisualizer: public PropertyVisualizer{

public:
    OVMPropertyVisualizer(MeshT* _mesh, int objectID, PropertyInfo _propertyInfo)
        : PropertyVisualizer(_propertyInfo),
          mesh(_mesh),
          mObjectID(objectID),
          drawModes()
    {}

    virtual ~OVMPropertyVisualizer(){ clear(); }

    /// Visualizes a property.
    virtual void visualize(bool _setDrawMode, QWidget* _widget);

    /**
     * @brief Should remove the property.
     *
     * Removing properties is not yet supported by OpenVolumeMesh. Therefor this method only informs the user of this fact via
     * a log message.
     */
    virtual void removeProperty(){ emit log("Removing properties not yet implemented for OpenVolumeMeshs."); }

    /// Duplicates a property.
    virtual void duplicateProperty(){ /*implemented by subclass*/}

    /// Clears a property.
    virtual void clear();

    virtual QString getPropertyText(unsigned int index)=0;

    /// Returns the ID of the closest primitive.
    unsigned int getClosestPrimitiveId(unsigned int _face, ACG::Vec3d &_hitPoint);

#ifdef ENABLE_PROPVIS_HISTOGRAMS
protected slots:
    template <typename Type>
    void showHistogram(ACG::QtWidgets::QtHistogramWidget *histogramWidget);
#endif

protected:
    MeshT* mesh;

    virtual void visualizeFaceProp(bool _setDrawMode = true);
    virtual void visualizeEdgeProp(bool _setDrawMode = true);
    virtual void visualizeHalfedgeProp(bool _setDrawMode = true);
    virtual void visualizeVertexProp(bool _setDrawMode = true);
    virtual void visualizeCellProp(bool _setDrawMode = true);
    virtual void visualizeHalffaceProp(bool _setDrawMode = true);

    template<typename PropType>
    void duplicateProperty_stage1();

    template <typename InnerType>
    QString getPropertyText_(unsigned int index);

    virtual void setCellPropertyFromText(unsigned int index, QString text);
    virtual void setFacePropertyFromText(unsigned int index, QString text);
    virtual void setHalffacePropertyFromText(unsigned int index, QString text);
    virtual void setEdgePropertyFromText(unsigned int index, QString text);
    virtual void setHalfedgePropertyFromText(unsigned int index, QString text);
    virtual void setVertexPropertyFromText(unsigned int index, QString text);

    virtual void setPropertyFromText(unsigned int index, QString text);

    virtual int getEntityCount();

    virtual QString getHeader();

    unsigned int getClosestCellId(unsigned int _face, ACG::Vec3d& _hitPoint);
    unsigned int getClosestFaceId(unsigned int _face, ACG::Vec3d& _hitPoint);
    unsigned int getClosestHalffaceId(unsigned int _face, ACG::Vec3d& _hitPoint);
    unsigned int getClosestEdgeId(unsigned int _face, ACG::Vec3d& _hitPoint);
    unsigned int getClosestHalfedgeId(unsigned int _face, ACG::Vec3d& _hitPoint);
    unsigned int getClosestVertexId(unsigned int _face, ACG::Vec3d& _hitPoint);

    int mObjectID;

    VolumeMeshDrawModesContainer drawModes;

private:

    template<typename Property>
    class CopyProperty
    {
        public:
            CopyProperty(Property& p1, const Property& p2, MeshT*& mesh) :
                p1(p1), p2(p2), mesh(mesh) {}

            template<typename PrimitiveHandleT>
            inline void operator() (const PrimitiveHandleT &pr) {
                p1[pr] = p2[pr];
            }

        private:
            Property &p1;
            const Property &p2;
            MeshT*& mesh;
    };
};

#define CALLS_TO_VISUALIZE_PROP(Classname, Template, PropType) \
template <Template> \
void Classname::visualizeCellProp(bool _setDrawMode) \
{\
    OpenVolumeMesh::CellPropertyT<PropType> prop = OVMPropertyVisualizer<MeshT>::mesh->template request_cell_property<PropType>(OVMPropertyVisualizer<MeshT>::propertyInfo.propName());\
    visualizeProp(prop, OVMPropertyVisualizer<MeshT>::mesh->cells_begin(), OVMPropertyVisualizer<MeshT>::mesh->cells_end());\
    if (_setDrawMode)\
    {\
        VolumeMeshObject<MeshT>* object;\
        PluginFunctions::getObject(OVMPropertyVisualizer<MeshT>::mObjectID, object);\
        object->setObjectDrawMode(OVMPropertyVisualizer<MeshT>::drawModes.cellsColoredPerCell);\
    }\
}\
template <Template>\
void Classname::visualizeFaceProp(bool _setDrawMode)\
{\
    OpenVolumeMesh::FacePropertyT<PropType> prop = OVMPropertyVisualizer<MeshT>::mesh->template request_face_property<PropType>(OVMPropertyVisualizer<MeshT>::propertyInfo.propName());\
    visualizeProp(prop, OVMPropertyVisualizer<MeshT>::mesh->faces_begin(), OVMPropertyVisualizer<MeshT>::mesh->faces_end());\
    if (_setDrawMode)\
    {\
        VolumeMeshObject<MeshT>* object;\
        PluginFunctions::getObject(OVMPropertyVisualizer<MeshT>::mObjectID, object);\
        object->setObjectDrawMode(OVMPropertyVisualizer<MeshT>::drawModes.facesColoredPerFace);\
    }\
}\
template <Template>\
void Classname::visualizeHalffaceProp(bool _setDrawMode)\
{\
    OpenVolumeMesh::HalfFacePropertyT<PropType> prop = OVMPropertyVisualizer<MeshT>::mesh->template request_halfface_property<PropType>(OVMPropertyVisualizer<MeshT>::propertyInfo.propName());\
    visualizeProp(prop, OVMPropertyVisualizer<MeshT>::mesh->halffaces_begin(), OVMPropertyVisualizer<MeshT>::mesh->halffaces_end());\
    if (_setDrawMode)\
    {\
        VolumeMeshObject<MeshT>* object;\
        PluginFunctions::getObject(OVMPropertyVisualizer<MeshT>::mObjectID, object);\
        object->setObjectDrawMode(OVMPropertyVisualizer<MeshT>::drawModes.halffacesColoredPerHalfface);\
    }\
}\
template <Template>\
void Classname::visualizeEdgeProp(bool _setDrawMode)\
{\
    OpenVolumeMesh::EdgePropertyT<PropType> prop = OVMPropertyVisualizer<MeshT>::mesh->template request_edge_property<PropType>(OVMPropertyVisualizer<MeshT>::propertyInfo.propName());\
    visualizeProp(prop, OVMPropertyVisualizer<MeshT>::mesh->edges_begin(), OVMPropertyVisualizer<MeshT>::mesh->edges_end());\
    if (_setDrawMode)\
    {\
        VolumeMeshObject<MeshT>* object;\
        PluginFunctions::getObject(OVMPropertyVisualizer<MeshT>::mObjectID, object);\
        object->setObjectDrawMode(OVMPropertyVisualizer<MeshT>::drawModes.edgesColoredPerEdge);\
    }\
}\
template <Template>\
void Classname::visualizeHalfedgeProp(bool _setDrawMode)\
{\
    OpenVolumeMesh::HalfEdgePropertyT<PropType> prop = OVMPropertyVisualizer<MeshT>::mesh->template request_halfedge_property<PropType>(OVMPropertyVisualizer<MeshT>::propertyInfo.propName());\
    visualizeProp(prop, OVMPropertyVisualizer<MeshT>::mesh->halfedges_begin(), OVMPropertyVisualizer<MeshT>::mesh->halfedges_end());\
    if (_setDrawMode)\
    {\
        VolumeMeshObject<MeshT>* object;\
        PluginFunctions::getObject(OVMPropertyVisualizer<MeshT>::mObjectID, object);\
        object->setObjectDrawMode(OVMPropertyVisualizer<MeshT>::drawModes.halfedgesColoredPerHalfedge);\
    }\
}\
template <Template>\
void Classname::visualizeVertexProp(bool _setDrawMode)\
{\
    OpenVolumeMesh::VertexPropertyT<PropType> prop = OVMPropertyVisualizer<MeshT>::mesh->template request_vertex_property<PropType>(OVMPropertyVisualizer<MeshT>::propertyInfo.propName());\
    visualizeProp(prop, OVMPropertyVisualizer<MeshT>::mesh->vertices_begin(), OVMPropertyVisualizer<MeshT>::mesh->vertices_end());\
    if (_setDrawMode)\
    {\
        VolumeMeshObject<MeshT>* object;\
        PluginFunctions::getObject(OVMPropertyVisualizer<MeshT>::mObjectID, object);\
        object->setObjectDrawMode(OVMPropertyVisualizer<MeshT>::drawModes.verticesColored);\
    }\
}\


#if defined(INCLUDE_TEMPLATES) && !defined(OVM_PROPERTY_VISUALIZER_CC)
#include "OVMPropertyVisualizerT.cc"
#endif

#endif /* OVM_PROPERTY_VISUALIZER_HH */

#endif /* ENABLE_OPENVOLUMEMESH_SUPPORT */

