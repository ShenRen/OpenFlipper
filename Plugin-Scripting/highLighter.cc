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

#if QT_VERSION >= 0x050000 
  #include <QtWidgets>
#else
  #include <QtGui>
#endif


#include "highLighter.hh"

 Highlighter::Highlighter(QTextDocument *parent)
  : QSyntaxHighlighter(parent)
{
 init();
}

Highlighter::Highlighter(QTextEdit *parent)
: QSyntaxHighlighter(parent)
{
  init();
}

 void Highlighter::init() {
   // Set the basic format styles
   keywordFormat_.setForeground(Qt::darkGreen);
   keywordFormat_.setFontWeight(QFont::Bold);

   pluginFormat_.setForeground(Qt::darkBlue);
   pluginFormat_.setFontWeight(QFont::Bold);

   functionFormat_.setForeground(Qt::darkYellow);
   functionFormat_.setFontWeight(QFont::Bold);

   typeFormat_.setForeground(Qt::darkMagenta);
   typeFormat_.setFontWeight(QFont::Bold);

   quotationFormat_.setForeground(Qt::darkRed);

   listFormat_.setForeground(Qt::darkRed);

   singleLineCommentFormat_.setForeground(Qt::red);
   multiLineCommentFormat_.setForeground(Qt::red);

   commentStartExpression_ = QRegExp("/\\*");
   commentEndExpression_   = QRegExp("\\*/");

   // Define basic keywords
   keywordPatterns_ << "while" << "for" << "print" << "var" << "break" << "if";

   // Types which are accepted by the scripting system
   typePatterns_ << "int" << "Matrix4x4" << "QString" << "idList" << "bool" << "Vector" << "double";

   update();

   //   classFormat.setFontWeight(QFont::Bold);
   //   classFormat.setForeground(Qt::darkMagenta);
   //   rule.pattern = QRegExp("\\bQ[A-Za-z]+\\b");
   //   rule.format = classFormat;
   //   highlightingRules.append(rule);
   //



   //   functionFormat.setFontItalic(true);
   //   functionFormat.setForeground(Qt::blue);
   //   rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
   //   rule.format = functionFormat;
   //   highlightingRules.append(rule);
   //
}

void Highlighter::update() {

  highlightingRules_.clear();

  HighlightingRule rule;

  // Create Rules for keywords
  foreach (QString pattern, keywordPatterns_) {
    rule.pattern = QRegExp("\\b" + pattern + "\\b" );
    rule.format = keywordFormat_;
    highlightingRules_.append(rule);
  }

  // Create Rules for plugins
  foreach (QString pattern, pluginPatterns_ ) {
    rule.pattern = QRegExp("\\b" + pattern + "\\b" );
    rule.format = pluginFormat_;
    highlightingRules_.append(rule);
  }

  // Create Rules for functions
  foreach (QString pattern, functionPatterns_ ) {
    rule.pattern = QRegExp("\\b" + pattern + "\\b" );
    rule.format = functionFormat_;
    highlightingRules_.append(rule);
  }

  // Create Rules for types
  foreach (QString pattern, typePatterns_ ) {
    rule.pattern = QRegExp("\\b" + pattern + "\\b" );
    rule.format = typeFormat_;
    highlightingRules_.append(rule);
  }

  // Rule for single line comments
  rule.pattern = QRegExp("//[^\n]*");
  rule.format = singleLineCommentFormat_;
  highlightingRules_.append(rule);

  // Rules for quotations
  rule.pattern = QRegExp("\".*\"");
  rule.format = quotationFormat_;
  highlightingRules_.append(rule);

  // Rules for Lists
  rule.pattern = QRegExp("\\[.*\\]");
  rule.format = listFormat_;
  highlightingRules_.append(rule);

}

 void Highlighter::highlightBlock(const QString &text)
{

   foreach (HighlightingRule rule, highlightingRules_) {
    QRegExp expression(rule.pattern);
    int index = text.indexOf(expression);
    while (index >= 0) {
      int length = expression.matchedLength();
      setFormat(index, length, rule.format);
      index = text.indexOf(expression, index + length);
    }
  }
  setCurrentBlockState(0);

  int startIndex = 0;
  if (previousBlockState() != 1)
    startIndex = text.indexOf(commentStartExpression_);

  while (startIndex >= 0) {
    int endIndex = text.indexOf(commentEndExpression_, startIndex);
    int commentLength;
    if (endIndex == -1) {
      setCurrentBlockState(1);
      commentLength = text.length() - startIndex;
    } else {
      commentLength = endIndex - startIndex + commentEndExpression_.matchedLength();
    }
    setFormat(startIndex, commentLength, multiLineCommentFormat_);
    startIndex = text.indexOf(commentStartExpression_, startIndex + commentLength);
  }

}
