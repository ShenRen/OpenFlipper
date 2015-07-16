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

#ifndef VSI_SCENEELEMENT_HH
#define VSI_SCENEELEMENT_HH

//== INCLUDES =================================================================
#include <QGraphicsWidget>
#include <QString>
#include <QDomDocument>
#include <QDomElement>
#include <QXmlQuery>

//== FORWARDDECLARATIONS ======================================================
class QGraphicsLinearLayout;

//== NAMESPACES ===============================================================
namespace VSI {

//== FORWARDDECLARATIONS ======================================================
class GraphicsScene;
class ElementInput;
class ElementOutput;
class ElementFunction;

class Element;
class Text;

//== CLASS DEFINITION =========================================================

/** Widget that represents an context element in the scene
  */
class SceneElement : public QGraphicsWidget
{
  Q_OBJECT

  public:
    /// Constructor
    SceneElement (GraphicsScene *_scene, Element *_element);

    /// Destructor
    ~SceneElement ();

    /// Background painting
    void paint (QPainter *_painter, const QStyleOptionGraphicsItem *_option, QWidget *_widget = 0);

    /// Context VSI::Element
    Element *element () const { return element_; };

    /// Inputs
    QVector<ElementInput *> inputs () { return inputs_; };

    /// Outputs
    QVector<ElementOutput *> outputs () { return outputs_; };

    /// Functions
    QVector<ElementFunction *> functions () { return functions_; };

    /// Scene input
    ElementInput *dataIn () { return dataIn_; };

    /// Scene output
    ElementOutput *dataOut () { return dataOut_; };

    /// Reset code block for code generation
    void resetCodeGeneration ();

    /// Replace block with name _name and id _id with _value
    void replaceCodeBlock (QString _name, QString _id, QString _value);

    /// Unique variable name for code generation
    QString variableId ();

    /// Unique id for identification
    int id () { return id_; };

    /// Code block
    QString code () { return code_; };

    /// Will this element be executed before _e bacause of its connections?
    bool isBefore (SceneElement *_e);

    /// Will this element be executed after _e bacause of its connections?
    bool isAfter (SceneElement *_e);

    /// Save to xml
    void saveToXml (QDomDocument &_doc, QDomElement &_root);

    /// Load from xml
    void loadFromXml (QXmlQuery &_xml);

    /// Scene
    GraphicsScene *graphicsScene () { return scene_; };

  protected:

    // mouse handling for draging
    void mousePressEvent (QGraphicsSceneMouseEvent *_event);
    void mouseMoveEvent (QGraphicsSceneMouseEvent *_event);
    void mouseReleaseEvent (QGraphicsSceneMouseEvent *_event);
    void mouseDoubleClickEvent (QGraphicsSceneMouseEvent *_event);

    // handle movement
    void moveEvent (QGraphicsSceneMoveEvent *_event);

  private slots:

    // show input configuration dialog
    void showInputConfig ();

  private:

    // invalidate (reset) all connections of this element
    void invalidateConnections ();

  private:

    GraphicsScene *scene_;
    Element *element_;

    QVector<ElementInput *> inputs_;
    QVector<ElementOutput *> outputs_;

    ElementInput *dataIn_;
    ElementOutput *dataOut_;

    QVector<ElementFunction *> functions_;

    QVector<ElementInput *> configInputs_;

    QString code_;

    int id_;

    Text *name_;
    Text *elementName_;
    QGraphicsLinearLayout *nameLayout_;
};

//=============================================================================
}
//=============================================================================

#endif
