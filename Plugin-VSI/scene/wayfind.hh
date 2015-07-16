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

#ifndef VSI_WAYFIND_HH
#define VSI_WAYFIND_HH

//== INCLUDES =================================================================
#include <QRegion>
#include <QPolygonF>
#include <QPoint>
#include <QLinkedList>

//== NAMESPACES ===============================================================
namespace VSI {

//== FORWARDDECLARATIONS ======================================================
class GraphicsScene;
class Connection;

//== CLASS DEFINITION =========================================================

/** A-Star way finding implementation with dynamic growing area
  */
class WayFind {

  public:
    /// Constructor
    WayFind (GraphicsScene *_scene);

    /// Destructor
    ~WayFind ();

    /// Finds a way from _from to _to ignoring any already existent connections from _conn
    QPolygonF findWay (Connection *_conn, QPoint _from, QPoint _to);

  private:

    // Area node class
    class Node {
      public:

        // node direction
        enum Type {
          Vertical,
          Horizontal
        };

        Node (unsigned int _counter);
        ~Node ();

      public:
        // last used counter
        unsigned int counter_;

        // current direction
        Type type_;

        // neighbor nodes
        Node *n_[4];

        // linked list handling
        Node *prev_;
        Node *next_;

        // node position
        QPoint pos_;

        // initialized from
        Node *from_;

        // A-Star consts
        unsigned int g_;
        unsigned int f_;
        unsigned int h_;

        // node cost
        unsigned int cost_;

        // already traversed
        bool closed_;
    };

    // Next position in distance _step from _pnt in direction _dir
    QPoint validPos (unsigned int _dir, int _step, QPoint _pnt);

    // Heuristic distance from _from to _to
    int heuristicDistance (const QPoint &_from, const QPoint &_to) const;

    // cleanup ununsed nodes
    void cleanup ();

  private:

    GraphicsScene *scene_;

    unsigned int counter_;

    QLinkedList<Node *> nodes_;
    QHash<QPair<int, int>, Node*> map_;

    Node *ll_;
    QPoint oldFrom_;
    QRegion oldReg_;
};

}

#endif
