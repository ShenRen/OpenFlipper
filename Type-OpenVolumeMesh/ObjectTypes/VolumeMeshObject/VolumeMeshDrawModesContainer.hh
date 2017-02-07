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
*   $LastChangedBy$                                                 *
*   $Date$                    *
*                                                                            *
\*===========================================================================*/

#ifndef VOLUMEMESHDRAWMODESCONTAINER_HH
#define VOLUMEMESHDRAWMODESCONTAINER_HH

#include <ACG/Scenegraph/DrawModes.hh>


/*! \class VolumeMeshDrawModesContainer
 *  \brief This class provides easy access to DrawModes supported by OpenVolumeMesh.
 */
class VolumeMeshDrawModesContainer
{
public:
    VolumeMeshDrawModesContainer()
        :
          cellsFlatShaded(ACG::SceneGraph::DrawModes::addDrawMode("Cells (flat shaded)", true)),
          cellsSmoothShaded(ACG::SceneGraph::DrawModes::addDrawMode("Cells (smooth shaded)")),
          cellsPhongShaded(ACG::SceneGraph::DrawModes::addDrawMode("Cells (phong shaded)")),
          cellsColoredPerCell(ACG::SceneGraph::DrawModes::addDrawMode("Cells (colored per cell)")),
          cellsColoredPerFace(ACG::SceneGraph::DrawModes::addDrawMode("Cells (colored per face)")),
          cellsColoredPerHalfface(ACG::SceneGraph::DrawModes::addDrawMode("Cells (colored per halfface)")),
          cellsColoredPerVertex(ACG::SceneGraph::DrawModes::addDrawMode("Cells (colored per vertex)")),
          cellsTransparent(ACG::SceneGraph::DrawModes::addDrawMode("Cells (transparent)")),

          facesFlatShaded(ACG::SceneGraph::DrawModes::addDrawMode("Faces (flat shaded)")),
          facesSmoothShaded(ACG::SceneGraph::DrawModes::addDrawMode("Faces (smooth shaded)")),
          facesPhongShaded(ACG::SceneGraph::DrawModes::addDrawMode("Faces (phong shaded)")),
          facesColoredPerFace(ACG::SceneGraph::DrawModes::addDrawMode("Faces (colored per face)")),
          facesColoredPerVertex(ACG::SceneGraph::DrawModes::addDrawMode("Faces (colored per vertex)")),
          facesTextured(ACG::SceneGraph::DrawModes::addDrawMode("Faces (textured)")),
          facesTexturedShaded(ACG::SceneGraph::DrawModes::addDrawMode("Faces (textured and shaded)")),
          facesOnCells(ACG::SceneGraph::DrawModes::addDrawMode("Faces (on cells)")),
          hiddenLineBackgroundFaces(ACG::SceneGraph::DrawModes::addDrawMode("Hidden line (background colored faces)")),

          halffacesFlatShaded(ACG::SceneGraph::DrawModes::addDrawMode("Halffaces (flat shaded)")),
          halffacesSmoothShaded(ACG::SceneGraph::DrawModes::addDrawMode("Halffaces (smooth shaded)")),
          halffacesPhongShaded(ACG::SceneGraph::DrawModes::addDrawMode("Halffaces (phong shaded)")),
          halffacesColoredPerHalfface(ACG::SceneGraph::DrawModes::addDrawMode("Halffaces (colored per halfface)")),
          halffacesColoredPerVertex(ACG::SceneGraph::DrawModes::addDrawMode("Halffaces (colored per vertex)")),

          edgesWireframe(ACG::SceneGraph::DrawModes::addDrawMode("Edges (wireframe)")),
          edgesHiddenLine(ACG::SceneGraph::DrawModes::addDrawMode("Edges (hidden line)")),
          edgesColoredPerEdge(ACG::SceneGraph::DrawModes::addDrawMode("Edges (colored per edge)")),
          edgesOnCells(ACG::SceneGraph::DrawModes::addDrawMode("Edges (on cells)")),

          halfedgesWireframe(ACG::SceneGraph::DrawModes::addDrawMode("Halfedges (wireframe)")),
          halfedgesHiddenLine(ACG::SceneGraph::DrawModes::addDrawMode("Halfedges (hidden line)")),
          halfedgesColoredPerHalfedge(ACG::SceneGraph::DrawModes::addDrawMode("Halfedges (colored per halfedge)")),

          verticesFlatShaded(ACG::SceneGraph::DrawModes::addDrawMode("Vertices")),
          verticesColored(ACG::SceneGraph::DrawModes::addDrawMode("Vertices (colored)")),
          verticesOnCells(ACG::SceneGraph::DrawModes::addDrawMode("Vertices (on Cells)")),

          irregularInnerEdges(ACG::SceneGraph::DrawModes::addDrawMode("Irregular edges")),
          irregularOuterEdges(ACG::SceneGraph::DrawModes::addDrawMode("Irregular outer valence 2 edges")),

          cellBasedDrawModes(cellsFlatShaded | cellsSmoothShaded | cellsPhongShaded | cellsColoredPerCell | cellsColoredPerFace
                             | cellsColoredPerHalfface | cellsColoredPerVertex | cellsTransparent),
          faceBasedDrawModes(facesFlatShaded | facesSmoothShaded | facesPhongShaded | facesColoredPerFace |
                             facesColoredPerVertex | facesTextured | facesTexturedShaded | hiddenLineBackgroundFaces),
          halffaceBasedDrawModes(halffacesFlatShaded | halffacesSmoothShaded | halffacesPhongShaded | halffacesColoredPerHalfface
                             | halffacesColoredPerVertex),
          edgeBasedDrawModes(edgesWireframe | edgesHiddenLine | edgesColoredPerEdge | irregularInnerEdges | irregularOuterEdges | edgesOnCells),
          halfedgeBasedDrawModes(halfedgesWireframe | halfedgesHiddenLine | halfedgesColoredPerHalfedge),
          vertexBasedDrawModes(verticesFlatShaded | verticesColored)
    {
    }


    ACG::SceneGraph::DrawModes::DrawMode cellsFlatShaded;
    ACG::SceneGraph::DrawModes::DrawMode cellsSmoothShaded;
    ACG::SceneGraph::DrawModes::DrawMode cellsPhongShaded;
    ACG::SceneGraph::DrawModes::DrawMode cellsColoredPerCell;
    ACG::SceneGraph::DrawModes::DrawMode cellsColoredPerFace;
    ACG::SceneGraph::DrawModes::DrawMode cellsColoredPerHalfface;
    ACG::SceneGraph::DrawModes::DrawMode cellsColoredPerVertex;
    ACG::SceneGraph::DrawModes::DrawMode cellsTransparent;

    ACG::SceneGraph::DrawModes::DrawMode facesFlatShaded;
    ACG::SceneGraph::DrawModes::DrawMode facesSmoothShaded;
    ACG::SceneGraph::DrawModes::DrawMode facesPhongShaded;
    ACG::SceneGraph::DrawModes::DrawMode facesColoredPerFace;
    ACG::SceneGraph::DrawModes::DrawMode facesColoredPerVertex;
    ACG::SceneGraph::DrawModes::DrawMode facesTextured;
    ACG::SceneGraph::DrawModes::DrawMode facesTexturedShaded;
    ACG::SceneGraph::DrawModes::DrawMode facesOnCells;
    ACG::SceneGraph::DrawModes::DrawMode hiddenLineBackgroundFaces;

    ACG::SceneGraph::DrawModes::DrawMode halffacesFlatShaded;
    ACG::SceneGraph::DrawModes::DrawMode halffacesSmoothShaded;
    ACG::SceneGraph::DrawModes::DrawMode halffacesPhongShaded;
    ACG::SceneGraph::DrawModes::DrawMode halffacesColoredPerHalfface;
    ACG::SceneGraph::DrawModes::DrawMode halffacesColoredPerVertex;

    ACG::SceneGraph::DrawModes::DrawMode edgesWireframe;
    ACG::SceneGraph::DrawModes::DrawMode edgesHiddenLine;
    ACG::SceneGraph::DrawModes::DrawMode edgesColoredPerEdge;
    ACG::SceneGraph::DrawModes::DrawMode edgesOnCells;

    ACG::SceneGraph::DrawModes::DrawMode halfedgesWireframe;
    ACG::SceneGraph::DrawModes::DrawMode halfedgesHiddenLine;
    ACG::SceneGraph::DrawModes::DrawMode halfedgesColoredPerHalfedge;

    ACG::SceneGraph::DrawModes::DrawMode verticesFlatShaded;
    ACG::SceneGraph::DrawModes::DrawMode verticesColored;
    ACG::SceneGraph::DrawModes::DrawMode verticesOnCells;

    ACG::SceneGraph::DrawModes::DrawMode irregularInnerEdges;
    ACG::SceneGraph::DrawModes::DrawMode irregularOuterEdges;


    ACG::SceneGraph::DrawModes::DrawMode cellBasedDrawModes;
    ACG::SceneGraph::DrawModes::DrawMode faceBasedDrawModes;
    ACG::SceneGraph::DrawModes::DrawMode halffaceBasedDrawModes;
    ACG::SceneGraph::DrawModes::DrawMode edgeBasedDrawModes;
    ACG::SceneGraph::DrawModes::DrawMode halfedgeBasedDrawModes;
    ACG::SceneGraph::DrawModes::DrawMode vertexBasedDrawModes;

    /**
     * @brief Returns the first cell based draw mode that is present in the given draw mode
     * @param _drawMode A draw mode that may be non-atomic
     * @return The first cell based draw mode that is present in _drawMode or DrawModes::NONE if none was found
     */
    ACG::SceneGraph::DrawModes::DrawMode getFirstCellDrawMode(ACG::SceneGraph::DrawModes::DrawMode _drawMode)
    {
        if (cellsFlatShaded             & _drawMode) return cellsFlatShaded;
        if (cellsSmoothShaded           & _drawMode) return cellsSmoothShaded;
        if (cellsPhongShaded            & _drawMode) return cellsPhongShaded;
        if (cellsColoredPerCell         & _drawMode) return cellsColoredPerCell;
        if (cellsColoredPerFace         & _drawMode) return cellsColoredPerFace;
        if (cellsColoredPerHalfface     & _drawMode) return cellsColoredPerHalfface;
        if (cellsColoredPerVertex       & _drawMode) return cellsColoredPerVertex;
        if (cellsTransparent            & _drawMode) return cellsTransparent;
        return ACG::SceneGraph::DrawModes::NONE;
    }

    /**
     * @brief Returns the first face or halfface based draw mode that is present in the given draw mode
     * @param _drawMode A draw mode that may be non-atomic
     * @return The first face or halfface based draw mode that is present in _drawMode or DrawModes::NONE if none was found
     */
    ACG::SceneGraph::DrawModes::DrawMode getFirstFaceDrawMode(ACG::SceneGraph::DrawModes::DrawMode _drawMode)
    {
        if (facesFlatShaded             & _drawMode) return facesFlatShaded;
        if (facesSmoothShaded           & _drawMode) return facesSmoothShaded;
        if (facesPhongShaded            & _drawMode) return facesPhongShaded;
        if (facesColoredPerFace         & _drawMode) return facesColoredPerFace;
        if (facesColoredPerFace         & _drawMode) return facesColoredPerFace;
        if (facesColoredPerVertex       & _drawMode) return facesColoredPerVertex;
        if (facesTextured               & _drawMode) return facesTextured;
        if (facesTexturedShaded         & _drawMode) return facesTexturedShaded;

        if (halffacesFlatShaded         & _drawMode) return halffacesFlatShaded;
        if (halffacesSmoothShaded       & _drawMode) return halffacesSmoothShaded;
        if (halffacesPhongShaded        & _drawMode) return halffacesPhongShaded;
        if (halffacesColoredPerHalfface & _drawMode) return halffacesColoredPerHalfface;
        if (halffacesColoredPerVertex   & _drawMode) return halffacesColoredPerVertex;

        if (hiddenLineBackgroundFaces   & _drawMode) return hiddenLineBackgroundFaces;
        if (edgesHiddenLine             & _drawMode) return hiddenLineBackgroundFaces;
        if (halfedgesHiddenLine         & _drawMode) return hiddenLineBackgroundFaces;
        return ACG::SceneGraph::DrawModes::NONE;
    }

    /**
     * @brief Returns the first edge or halfedge based draw mode that is present in the given draw mode
     * @param _drawMode A draw mode that may be non-atomic
     * @return The first edge or halfedge based draw mode that is present in _drawMode or DrawModes::NONE if none was found
     */
    ACG::SceneGraph::DrawModes::DrawMode getFirstEdgeDrawMode(ACG::SceneGraph::DrawModes::DrawMode _drawMode)
    {
        if (edgesWireframe              & _drawMode) return edgesWireframe;
        if (edgesColoredPerEdge         & _drawMode) return edgesColoredPerEdge;

        if (halfedgesWireframe          & _drawMode) return halfedgesWireframe;
        if (halfedgesColoredPerHalfedge & _drawMode) return halfedgesColoredPerHalfedge;

        if (irregularInnerEdges         & _drawMode) return irregularInnerEdges;
        if (irregularOuterEdges         & _drawMode) return irregularOuterEdges;

        if (edgesHiddenLine             & _drawMode) return edgesHiddenLine;
        if (halfedgesHiddenLine         & _drawMode) return halfedgesHiddenLine;

        if (edgesOnCells                & _drawMode) return edgesOnCells;
        return ACG::SceneGraph::DrawModes::NONE;
    }

    /**
     * @brief Returns the first vertex based draw mode that is present in the given draw mode
     * @param _drawMode A draw mode that may be non-atomic
     * @return The first vertex based draw mode that is present in _drawMode or DrawModes::NONE if none was found
     */
    ACG::SceneGraph::DrawModes::DrawMode getFirstVertexDrawMode(ACG::SceneGraph::DrawModes::DrawMode _drawMode)
    {
        if (verticesFlatShaded          & _drawMode) return verticesFlatShaded;
        if (verticesColored             & _drawMode) return verticesColored;
        if (verticesOnCells             & _drawMode) return verticesOnCells;
        return ACG::SceneGraph::DrawModes::NONE;
    }


};

#endif // VOLUMEMESHDRAWMODESCONTAINER_HH
