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

#ifndef OM_PROPERTY_VISUALIZER_HH
#define OM_PROPERTY_VISUALIZER_HH

#include "../PropertyVisualizer.hh"

#include <ObjectTypes/TriangleMesh/TriangleMesh.hh>
#include <ObjectTypes/PolyMesh/PolyMesh.hh>

#include <iostream>

template <typename MeshT>
class OMPropertyVisualizer: public PropertyVisualizer{

public:
    OMPropertyVisualizer(MeshT* _mesh, PropertyInfo _propertyInfo)
        : PropertyVisualizer(_propertyInfo),
          mesh(_mesh)
    { }

    virtual ~OMPropertyVisualizer(){clear();}


    /// Visualizes the property.
    virtual void visualize(bool _setDrawMode, QWidget* _widget);

    /**
     * @brief Removes the property.
     *
     * This will remove the property from the mesh. After this method the PropertyVisualizer
     * should be deleted.
     */
    virtual void removeProperty(){ /*implemented by subclass*/}

    /**
     * @brief Duplicates the property.
     *
     * This will duplicate the property.
     */
    virtual void duplicateProperty(){ /*implemented by subclass*/}

    /// Clears the property.
    virtual void clear();

    /// Returns the ID of the closest primitive.
    unsigned int getClosestPrimitiveId(unsigned int _face, ACG::Vec3d &_hitPoint);

    virtual QString getPropertyText(unsigned int index);

protected:
    MeshT* mesh;

    virtual void visualizeFaceProp(bool _setDrawMode = true);
    virtual void visualizeEdgeProp(bool _setDrawMode = true);
    virtual void visualizeHalfedgeProp(bool _setDrawMode = true);
    virtual void visualizeVertexProp(bool _setDrawMode = true);

    template <typename PropType>
    void removeProperty_stage1();
    template <typename PropType>
    void removeProperty_stage2();

    template<typename PropType>
    void duplicateProperty_stage1();
    template<typename PropHandle, typename Iterator>
    void duplicateProperty_stage2(Iterator first, Iterator last);



    virtual void clearFaceProp();
    virtual void clearEdgeProp();
    virtual void clearHalfedgeProp();
    virtual void clearVertexProp();

    virtual void setFacePropertyFromText(unsigned int index, QString text);
    virtual void setEdgePropertyFromText(unsigned int index, QString text);
    virtual void setHalfedgePropertyFromText(unsigned int index, QString text);
    virtual void setVertexPropertyFromText(unsigned int index, QString text);

    OpenMesh::Vec4f convertColor(QColor color);
    virtual void setPropertyFromText(unsigned int index, QString text);

    virtual int getEntityCount();

    virtual QString getHeader();

    unsigned int getClosestFaceId(unsigned int _face, ACG::Vec3d& _hitPoint);
    unsigned int getClosestEdgeId(unsigned int _face, ACG::Vec3d& _hitPoint);
    unsigned int getClosestHalfedgeId(unsigned int _face, ACG::Vec3d& _hitPoint);
    unsigned int getClosestVertexId(unsigned int _face, ACG::Vec3d& _hitPoint);


    template <typename InnerType >
    QString getPropertyText_(unsigned int index);

private:
    template<typename PropHandleT>
    class CopyProperty
    {
        public:
            CopyProperty(const PropHandleT& _p1, const PropHandleT& _p2, MeshT*& mesh) :
                p1(_p1), p2(_p2), mesh(mesh) {}

            template<typename PrimitiveHandleT>
            inline void operator() (const PrimitiveHandleT &pr) {
                mesh->property(p1, pr) = mesh->property(p2, pr);
            }

        private:
            const PropHandleT &p1, &p2;
            MeshT*& mesh;
    };

};



#if defined(INCLUDE_TEMPLATES) && !defined(OM_PROPERTY_VISUALIZER_CC)
#include "OMPropertyVisualizerT.cc"
#endif


#endif /* OM_PROPERTY_VISUALIZER_HH */
