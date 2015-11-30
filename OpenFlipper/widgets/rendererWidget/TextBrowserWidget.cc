
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
*   $Revision: 17080 $                                                       *
*   $LastChangedBy: moeller $                                                *
*   $Date: 2013-07-19 12:58:31 +0200 (Fri, 19 Jul 2013) $                     *
*                                                                            *
\*===========================================================================*/


#if QT_VERSION >= 0x050000
  #include <QtWidgets>
#else
  #include <QtGui>
#endif

#include "TextBrowserWidget.hh"

QString const TextBrowserWidget::startRenderObjectTag_ = "name:";
QString const TextBrowserWidget::startVertexShaderTag_ = "--vertex-shader--";
QString const TextBrowserWidget::endVertexShaderTag_ = "--end-vertex-shader--";
QString const TextBrowserWidget::startTessControlShaderTag_ = "---tesscontrol-shader--";
QString const TextBrowserWidget::endTessControlShaderTag_ = "--end-tesscontrol-shader--";
QString const TextBrowserWidget::startTessEvalShaderTag_ = "--tesseval-shader--";
QString const TextBrowserWidget::endTessEvalShaderTag_ = "--end-tesseval-shader--";
QString const TextBrowserWidget::startGeometryShaderTag_ = "--geometry-shader--";
QString const TextBrowserWidget::endGeometryShaderTag_ = "--end-geometry-shader--";
QString const TextBrowserWidget::startFragmentShaderTag_ = "--fragment-shader--";
QString const TextBrowserWidget::endFragmentShaderTag_ = "--end-fragment-shader--";


TextBrowserWidget::TextBrowserWidget(QWidget *parent) : QPlainTextEdit(parent) {
  sideArea_ = new TextBrowserSideArea(this);
  updateTextBrowserSideAreaWidth();

  connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateTextBrowserSideAreaWidth()));
  connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateFolds()));
  connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateTextBrowserSideArea(QRect,int)));

  setReadOnly(true);
}



int TextBrowserWidget::sideAreaWidth() {
  int digits = 1;
  int max = qMax(1, blockCount());
  while (max >= 10) {
    max /= 10;
    ++digits;
  }

  int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

  return space;
}



void TextBrowserWidget::updateTextBrowserSideAreaWidth() {
  setViewportMargins(sideAreaWidth(), 0, 0, 0);
}

void TextBrowserWidget::updateTextBrowserSideArea(const QRect &rect, int dy) {
  if (dy)
    sideArea_->scroll(0, dy);
  else
    sideArea_->update(0, rect.y(), sideArea_->width(), rect.height());

  if (rect.contains(viewport()->rect()))
    updateTextBrowserSideAreaWidth();
}



void TextBrowserWidget::resizeEvent(QResizeEvent *e) {
  QPlainTextEdit::resizeEvent(e);

  QRect cr = contentsRect();
  sideArea_->setGeometry(QRect(cr.left(), cr.top(), sideAreaWidth(), cr.height()));
}

void TextBrowserWidget::sideAreaPaintEvent(QPaintEvent *event) {

  QPainter painter(sideArea_);
  painter.fillRect(event->rect(), Qt::lightGray);
  painter.setPen(Qt::black);

  QTextBlock block = firstVisibleBlock();

  int blockNumber = block.blockNumber();
  int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
  int bottom = top + (int) blockBoundingRect(block).height();

  Fold found_fold;
  while (block.isValid() && top <= event->rect().bottom()) {
    if (block.isVisible() && bottom >= event->rect().top()) {
      if (getFold(block.position(), found_fold)) {
        if (found_fold.type == SHADER) {
          int fold_first_block = document()->findBlock(found_fold.start).blockNumber();
          QString text = block.text();
          // only draw line numbers on actual shader code
          if (text.contains(TextBrowserWidget::startVertexShaderTag_) ||
              text.contains(TextBrowserWidget::endVertexShaderTag_) ||
              text.contains(TextBrowserWidget::startTessControlShaderTag_) ||
              text.contains(TextBrowserWidget::endTessControlShaderTag_) ||
              text.contains(TextBrowserWidget::startTessEvalShaderTag_) ||
              text.contains(TextBrowserWidget::endTessEvalShaderTag_) ||
              text.contains(TextBrowserWidget::startGeometryShaderTag_) ||
              text.contains(TextBrowserWidget::endGeometryShaderTag_) ||
              text.contains(TextBrowserWidget::startFragmentShaderTag_) ||
              text.contains(TextBrowserWidget::endFragmentShaderTag_)) {
            if (found_fold.folded)
              painter.drawText(0, top, sideArea_->width(), fontMetrics().height(),Qt::AlignRight, "+");
            else
              painter.drawText(0, top, sideArea_->width(), fontMetrics().height(),Qt::AlignRight, "-");
          } else {
            QString number = QString::number(blockNumber - fold_first_block);
            painter.drawText(0, top, sideArea_->width(), fontMetrics().height(),Qt::AlignRight, number);
          }
        } else {
          if (found_fold.folded)
            painter.drawText(0, top, sideArea_->width(), fontMetrics().height(),Qt::AlignRight, "+");
          else
            painter.drawText(0, top, sideArea_->width(), fontMetrics().height(),Qt::AlignRight, "-");
        }
      } else
        painter.drawText(0, top, sideArea_->width(), fontMetrics().height(),Qt::AlignRight, " ");
    }

    block = block.next();
    top = bottom;
    bottom = top + (int) blockBoundingRect(block).height();
    ++blockNumber;
  }
}

bool TextBrowserWidget::getFold(int _position, Fold& _fold) {
  std::map<int,size_t>::iterator it = blockPosToFold_.find(_position);
  if (!folds_.empty() && it != blockPosToFold_.end()) {
    _fold = folds_[it->second];
    return true;
  } else
    return false;
}

void TextBrowserWidget::mouseDoubleClickEvent(QMouseEvent* e) {
  QTextBlock block = firstVisibleBlock();
  int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
  int bottom = top + (int) blockBoundingRect(block).height();
  const int y = e->y();
  // find the block that was clicked and toggle the folding
  while (block.isValid()) {
    if (top <= y && y <= bottom) {
      toggleFold(block.position());
      break;
    }

    block = block.next();
    top = bottom;
    bottom = top + (int) blockBoundingRect(block).height();
  }
}

void TextBrowserWidget::foldAll() {
  for (std::vector<Fold>::iterator it = folds_.begin(); it != folds_.end(); ++it) {
    fold(*it);
  }
}

void TextBrowserWidget::unfoldAll() {
  for (std::vector<Fold>::iterator it = folds_.begin(); it != folds_.end(); ++it) {
    unfold(*it);
  }
}

void TextBrowserWidget::fold(Fold& _fold) {
  if (_fold.folded)
    return;

  QTextBlock startBlock = document()->findBlock(_fold.start);
  QTextBlock endBlock = document()->findBlock(_fold.end);

  startBlock = startBlock.next();
  while (startBlock.isValid() && startBlock != endBlock) {
    startBlock.setVisible(false);
    startBlock = startBlock.next();
  }
  if (_fold.type == RENDEROBJECT)
    endBlock.setVisible(false);

  _fold.folded = true;
  document()->markContentsDirty(_fold.start, _fold.end - _fold.start);
}

void TextBrowserWidget::unfold(Fold& _fold) {
  if (!_fold.folded)
    return;

  QTextBlock startBlock = document()->findBlock(_fold.start);
  QTextBlock endBlock = document()->findBlock(_fold.end);

  startBlock = startBlock.next();
  while (startBlock.isValid() && startBlock != endBlock) {
    startBlock.setVisible(true);
    startBlock = startBlock.next();
  }
  if (_fold.type == RENDEROBJECT)
    endBlock.setVisible(true);

  _fold.folded = false;
  document()->markContentsDirty(_fold.start, _fold.end-_fold.start);
}

void TextBrowserWidget::toggleFold(int _position) {
  for (std::vector<Fold>::iterator it = folds_.begin(); it != folds_.end(); ++it) {
    if (it->contains(_position)) {
      if (it->folded)
        unfold(*it);
      else
        fold(*it);

      break;
    }
  }
}

void TextBrowserWidget::updateFolds() {
  folds_.clear();

  // search for all vertex shader
  QTextCursor startCursor = document()->find(TextBrowserWidget::startVertexShaderTag_, 0, QTextDocument::FindWholeWords);
  QTextCursor endCursor = document()->find(TextBrowserWidget::endVertexShaderTag_, 0, QTextDocument::FindWholeWords);

  while (!startCursor.isNull() && !endCursor.isNull()) {
    startCursor.movePosition(QTextCursor::StartOfLine);
    endCursor.movePosition(QTextCursor::EndOfLine);
    folds_.push_back(Fold(startCursor.position(),endCursor.position(),SHADER));

    // map block position to fold
    int startPos = startCursor.position();
    const int endPos = endCursor.position();
    for (; startPos < endPos; ++startPos) {
      QTextBlock block = document()->findBlock(startPos);
      blockPosToFold_[block.position()] = folds_.size() - 1;
    }

    bool moved = startCursor.movePosition(QTextCursor::Down);
    if (!moved)
      break;
    moved = endCursor.movePosition(QTextCursor::Down);
    if (!moved)
      break;

    startCursor = document()->find(TextBrowserWidget::startVertexShaderTag_, startCursor, QTextDocument::FindWholeWords);
    endCursor = document()->find(TextBrowserWidget::endVertexShaderTag_, endCursor, QTextDocument::FindWholeWords);
  }

  // search for all tesscontrol shader
  startCursor = document()->find(TextBrowserWidget::startTessControlShaderTag_, 0, QTextDocument::FindWholeWords);
  endCursor = document()->find(TextBrowserWidget::endTessControlShaderTag_, 0, QTextDocument::FindWholeWords);

  while (!startCursor.isNull() && !endCursor.isNull()) {
    startCursor.movePosition(QTextCursor::StartOfLine);
    endCursor.movePosition(QTextCursor::EndOfLine);
    folds_.push_back(Fold(startCursor.position(),endCursor.position(),SHADER));

    // map block position to fold
    int startPos = startCursor.position();
    const int endPos = endCursor.position();
    for (; startPos < endPos; ++startPos) {
      QTextBlock block = document()->findBlock(startPos);
      blockPosToFold_[block.position()] = folds_.size() - 1;
    }

    bool moved = startCursor.movePosition(QTextCursor::Down);
    if (!moved)
      break;
    moved = endCursor.movePosition(QTextCursor::Down);
    if (!moved)
      break;

    startCursor = document()->find(TextBrowserWidget::startTessControlShaderTag_, startCursor, QTextDocument::FindWholeWords);
    endCursor = document()->find(TextBrowserWidget::endTessControlShaderTag_, endCursor, QTextDocument::FindWholeWords);
  }

  // search for all tesseval shader
  startCursor = document()->find(TextBrowserWidget::startTessEvalShaderTag_, 0, QTextDocument::FindWholeWords);
  endCursor = document()->find(TextBrowserWidget::endTessEvalShaderTag_, 0, QTextDocument::FindWholeWords);

  while (!startCursor.isNull() && !endCursor.isNull()) {
    startCursor.movePosition(QTextCursor::StartOfLine);
    endCursor.movePosition(QTextCursor::EndOfLine);
    folds_.push_back(Fold(startCursor.position(),endCursor.position(),SHADER));

    // map block position to fold
    int startPos = startCursor.position();
    const int endPos = endCursor.position();
    for (; startPos < endPos; ++startPos) {
      QTextBlock block = document()->findBlock(startPos);
      blockPosToFold_[block.position()] = folds_.size() - 1;
    }

    bool moved = startCursor.movePosition(QTextCursor::Down);
    if (!moved)
      break;
    moved = endCursor.movePosition(QTextCursor::Down);
    if (!moved)
      break;

    startCursor = document()->find(TextBrowserWidget::startTessEvalShaderTag_, startCursor, QTextDocument::FindWholeWords);
    endCursor = document()->find(TextBrowserWidget::endTessEvalShaderTag_, endCursor, QTextDocument::FindWholeWords);
  }

  // search for all geometry shader
  startCursor = document()->find(TextBrowserWidget::startGeometryShaderTag_, 0, QTextDocument::FindWholeWords);
  endCursor = document()->find(TextBrowserWidget::endGeometryShaderTag_, 0, QTextDocument::FindWholeWords);

  while (!startCursor.isNull() && !endCursor.isNull()) {
    startCursor.movePosition(QTextCursor::StartOfLine);
    endCursor.movePosition(QTextCursor::EndOfLine);
    folds_.push_back(Fold(startCursor.position(),endCursor.position(),SHADER));

    // map block position to fold
    int startPos = startCursor.position();
    const int endPos = endCursor.position();
    for (; startPos < endPos; ++startPos) {
      QTextBlock block = document()->findBlock(startPos);
      blockPosToFold_[block.position()] = folds_.size() - 1;
    }

    bool moved = startCursor.movePosition(QTextCursor::Down);
    if (!moved)
      break;
    moved = endCursor.movePosition(QTextCursor::Down);
    if (!moved)
      break;

    startCursor = document()->find(TextBrowserWidget::startGeometryShaderTag_, startCursor, QTextDocument::FindWholeWords);
    endCursor = document()->find(TextBrowserWidget::endGeometryShaderTag_, endCursor, QTextDocument::FindWholeWords);
  }

  // search for all fragment shader
  startCursor = document()->find(TextBrowserWidget::startFragmentShaderTag_, 0, QTextDocument::FindWholeWords);
  endCursor = document()->find(TextBrowserWidget::endFragmentShaderTag_, 0, QTextDocument::FindWholeWords);

  while (!startCursor.isNull() && !endCursor.isNull()) {
    startCursor.movePosition(QTextCursor::StartOfLine);
    endCursor.movePosition(QTextCursor::EndOfLine);
    folds_.push_back(Fold(startCursor.position(),endCursor.position(),SHADER));

    // map block position to fold
    int startPos = startCursor.position();
    const int endPos = endCursor.position();
    for (; startPos < endPos; ++startPos) {
      QTextBlock block = document()->findBlock(startPos);
      blockPosToFold_[block.position()] = folds_.size() - 1;
    }

    bool moved = startCursor.movePosition(QTextCursor::Down);
    if (!moved)
      break;
    moved = endCursor.movePosition(QTextCursor::Down);
    if (!moved)
      break;

    startCursor = document()->find(TextBrowserWidget::startFragmentShaderTag_, startCursor, QTextDocument::FindWholeWords);
    endCursor = document()->find(TextBrowserWidget::endFragmentShaderTag_, endCursor, QTextDocument::FindWholeWords);
  }

  // search for all render objects
  startCursor = document()->find(TextBrowserWidget::startRenderObjectTag_, 0, QTextDocument::FindWholeWords);
  endCursor = document()->find(TextBrowserWidget::startVertexShaderTag_, 0, QTextDocument::FindWholeWords);

  while (!startCursor.isNull() && !endCursor.isNull()) {
    startCursor.movePosition(QTextCursor::StartOfLine);
    // vertex shader does not belong to this fold
    endCursor.movePosition(QTextCursor::Up);
    endCursor.movePosition(QTextCursor::EndOfLine);
    folds_.push_back(Fold(startCursor.position(),endCursor.position(),RENDEROBJECT));

    // map block position to fold
    int startPos = startCursor.position();
    const int endPos = endCursor.position();
    for (; startPos < endPos; ++startPos) {
      QTextBlock block = document()->findBlock(startPos);
      blockPosToFold_[block.position()] = folds_.size() - 1;
    }

    bool moved = startCursor.movePosition(QTextCursor::Down);
    if (!moved)
      break;
    // skip to after the vertex shader starts
    moved = endCursor.movePosition(QTextCursor::Down);
    if (!moved)
      break;
    moved = endCursor.movePosition(QTextCursor::Down);
    if (!moved)
      break;

    startCursor = document()->find(TextBrowserWidget::startRenderObjectTag_, startCursor, QTextDocument::FindWholeWords);
    endCursor = document()->find(TextBrowserWidget::startVertexShaderTag_, endCursor, QTextDocument::FindWholeWords);
  }

  // fold shader blocks
  foldAll();
}
