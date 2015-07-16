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

#ifndef VSI_GRAPHICSSCENE_HH
#define VSI_GRAPHICSSCENE_HH

//== INCLUDES =================================================================

#include <QGraphicsScene>
#include <QPointF>
#include <QDomDocument>
#include <QDomElement>
#include <QXmlQuery>

//== FORWARDDECLARATIONS ======================================================
class QGraphicsRectItem;
class QMimeData;


//== NAMESPACES ===============================================================

namespace VSI {

//== FORWARDDECLARATIONS ======================================================
class Context;
class SceneElement;
class SceneTools;
class ElementArea;
class WayFind;
class ElementFunction;
class GraphicsView;
class Connection;


//== CLASS DEFINITION =========================================================

/** Implementation of the Qt graphics scene graph and the represatation of
  * the visual script
  */
class GraphicsScene : public QGraphicsScene
{
  Q_OBJECT

  public:

    /// Constructor
    GraphicsScene (VSI::Context *_ctx, ElementFunction *_function = NULL);

    /// Destructor
    ~GraphicsScene ();

    /// Graphics view of the scene
    GraphicsView *graphicsView () { return view_; };

    /// Returns a new Z value that is above all elements
    qreal getNewZ ();

    /// Moves all elements
    void moveElements (qreal _dx, qreal _dy, bool _selected = false);

    /// Scale all elements with scaling center in the center of the scene
    void scaleElements (qreal _delta);

    /// Scale all elements with scaling center _canter
    void scaleElements (qreal _delta, QPointF _center);

    /// Add the element to the scene
    void addElement (SceneElement *_element);

    /// Remove the element from the scene
    bool removeElement (SceneElement *_element);

    /// Redirect mouse movement to tools area
    void mouseMove (QPointF _pos);

    /// Redirect mouse release to tools area
    void mouseRelease (QPointF _pos, QGraphicsItem *_item);

    /// Bounding rectangle of all scene elements
    QRectF elementsBoundingRect ();

    /// Element area
    ElementArea *elementArea () const { return elementArea_; };

    /// Scene elements
    const QList<SceneElement *>& elements () const;

    /// Generate textual script code (_codeOnly = only pure code block)
    QString generateCode (QString &errors, bool _codeOnly = false);

    /// WayFind object
    WayFind *wayFind () { return wayFind_; };

    /// Save to xml
    void saveToXml (QDomDocument &_doc, QDomElement &_root);

    /// Load from xml
    void loadFromXml (QXmlQuery &_xml);

    /// Associated function
    ElementFunction *function () { return function_; };

    /// Sets the active connection
    void setActiveConnection (Connection *_conn) { activeConnection_ = _conn; };

  signals:
    /// Informs about content changes
    void contentChanged ();

  public slots:
    /// handle content changes
    void contentChange ();

    /// clear the whole scene (_start = keep start element)
    void clearScene (bool _start = false);

  protected:

    // draw background
    virtual void drawBackground(QPainter *_painter, const QRectF &_rect);

    // handle draging of elements from tool box
    void dragEnterEvent (QGraphicsSceneDragDropEvent *_event);
    void dragLeaveEvent (QGraphicsSceneDragDropEvent *_event);
    void dragMoveEvent (QGraphicsSceneDragDropEvent *_event);
    void dropEvent (QGraphicsSceneDragDropEvent *_event);

    // handle mouse for selection
    void mousePressEvent (QGraphicsSceneMouseEvent *_event);
    void mouseMoveEvent (QGraphicsSceneMouseEvent *_event);
    void mouseReleaseEvent (QGraphicsSceneMouseEvent *_event);

  private slots:
    // update all connections in the scene
    void updateConnections ();

    // scene has been resized
    void sceneResized (const QRectF &_rect);

  private:

    // Mark all inputs that are connected to the outputs of _from as valid (used during code generation)
    QString updateConnections (SceneElement *_from, bool _isStart);

    // Checks for supported mimetype
    bool validMimeData (const QMimeData *);

    // gets the element name from the mimetype data
    QString mimeDataElementName (const QMimeData *);

    // gets the mouse hotspot from the mimetype data
    QPoint mimeDataPoint (const QMimeData *);

    // load element from xml
    void loadElement (QXmlQuery &_xml);

    // load connection from xml
    void loadConnection (QXmlQuery &_xml);

    // returns all scene elements. Also all elements of sub-functions
    QList<SceneElement *> getAllElements ();

  private:

    Context *ctx_;
    ElementFunction *function_;
    GraphicsView *view_;

    SceneElement *dndElement_;
    SceneTools *tools_;

    ElementArea *elementArea_;

    QGraphicsRectItem *selection_;
    QPointF           selectionStart_;
    bool              selectionActive_;

    bool dontMoveStart_;
    bool blockChanges_;

    Connection *activeConnection_;

    qreal currentZ_;

    WayFind *wayFind_;
};

//=============================================================================
}
//=============================================================================

#endif

