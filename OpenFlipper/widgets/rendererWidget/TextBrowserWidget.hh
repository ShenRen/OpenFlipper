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
*   $Revision: 12438 $                                                       *
*   $LastChangedBy: moebius $                                                *
*   $Date: 2011-09-22 17:16:17 +0200 (Thu, 22 Sep 2011) $                     *
*                                                                            *
\*===========================================================================*/

#ifndef TEXTBROWSERWIDGET_HH
#define TEXTBROWSERWIDGET_HH

#include <QPlainTextEdit>
#include <QObject>

#include <vector>

class TextBrowserSideArea;


class TextBrowserWidget : public QPlainTextEdit
{
    Q_OBJECT

  private:
    enum FoldType {
      SHADER,
      RENDEROBJECT
    };

    struct Fold {
      Fold() :
        start(-1),
        end(-1),
        folded(false),
        type(SHADER)
      {}

      Fold(int _start, int _end, FoldType _type) :
        start(_start),
        end(_end),
        folded(false),
        type(_type)
      {}

      bool contains (int n) const {
        return (start <= n) && (n <= end);
      }

      // start positition in the document
      int start;
      // end positition in the document
      int end;
      bool folded;
      FoldType type;
    };


  public:
    explicit TextBrowserWidget(QWidget *parent = 0);

    void sideAreaPaintEvent(QPaintEvent *event);
    int sideAreaWidth();

  protected:
    void resizeEvent(QResizeEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent* e);

  private slots:
    void updateTextBrowserSideAreaWidth();
    void updateTextBrowserSideArea(const QRect &, int);
    void foldAll();
    void unfoldAll();
    void fold(Fold& _fold);
    void unfold(Fold& _fold);
    void toggleFold(int _position);
    void updateFolds();

    /** \brief get the _fold corresponding to the document _position
     *
     * @param _position position in the document
     * @param _fold     fold in folds at the position if found
     * @return          true if fold was found (otherwise false is returned)
     */
    bool getFold(int _position, Fold& _fold);

  private:
    TextBrowserSideArea* sideArea_;
    static QString const startRenderObjectTag_;
    static QString const startVertexShaderTag_;
    static QString const endVertexShaderTag_;
    static QString const startTessControlShaderTag_;
    static QString const endTessControlShaderTag_;
    static QString const startTessEvalShaderTag_;
    static QString const endTessEvalShaderTag_;
    static QString const startGeometryShaderTag_;
    static QString const endGeometryShaderTag_;
    static QString const startFragmentShaderTag_;
    static QString const endFragmentShaderTag_;

    std::vector<Fold> folds_;
    /// maps positions in the document to indices in folds_
    std::map<int, size_t> blockPosToFold_;
};


class TextBrowserSideArea : public QWidget
{
  public:
    explicit TextBrowserSideArea(TextBrowserWidget* _textBrowser) :
      QWidget(_textBrowser),
      textBrowser_(_textBrowser)
  {}

    QSize sizeHint() const {
      return QSize(textBrowser_->sideAreaWidth(), 0);
    }

  protected:
    void paintEvent(QPaintEvent *event) {
      textBrowser_->sideAreaPaintEvent(event);
    }

  private:
    TextBrowserWidget* textBrowser_;
};


#endif // TEXTBROWSERWIDGET_HH
