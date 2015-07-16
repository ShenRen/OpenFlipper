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
*   $Revision: 12439 $                                                       *
*   $LastChangedBy: moebius $                                                *
*   $Date: 2011-09-22 17:55:29 +0200 (Do, 22 Sep 2011) $                     *
*                                                                            *
\*===========================================================================*/


#if QT_VERSION >= 0x050000 
  #include <QtWidgets>
#else
  #include <QtGui>
#endif


#include "renderObjectHighLighter.hh"

RenderObjectHighlighter::RenderObjectHighlighter(QTextDocument *parent)
  : QSyntaxHighlighter(parent)
{
 init();
}

RenderObjectHighlighter::RenderObjectHighlighter(QTextEdit *parent)
: QSyntaxHighlighter(parent)
{
  init();
}

void RenderObjectHighlighter::init() {
   // Set the basic format styles
  vertexShaderFormat_.setBackground(Qt::green);
  vertexShaderFormat_.setFontWeight(QFont::Bold);

  vertexShaderStartExpression_ = QRegExp("---------------------vertex-shader--------------------");
  vertexShaderEndExpression_   = QRegExp("---------------------end-vertex-shader--------------------");

  geometryShaderFormat_.setBackground(Qt::blue);
  geometryShaderFormat_.setFontWeight(QFont::Bold);

  geometryShaderStartExpression_ = QRegExp("---------------------geometry-shader--------------------");;
  geometryShaderEndExpression_   = QRegExp("---------------------end-geometry-shader--------------------");;


  fragmentShaderFormat_.setBackground(Qt::red);
  fragmentShaderFormat_.setFontWeight(QFont::Bold);

  fragmentShaderStartExpression_ = QRegExp("---------------------fragment-shader--------------------");
  fragmentShaderEndExpression_   = QRegExp("---------------------end-fragment-shader--------------------");

  // match whole line containing the define
  defineFormat_.setForeground(Qt::green);
  defineFormat_.setFontWeight(QFont::Bold);

  // Single line comments
  singleLineCommentFormat_.setForeground(Qt::red);

  // Set the basic format styles
  keywordFormat_.setForeground(Qt::darkGreen);
  keywordFormat_.setFontWeight(QFont::Bold);

  // Define basic keywords
  keywordPatterns_ << "main" << "while" << "for" << "if" << "dot" << "sqrt" << "max" << "pow" << "return" << "normalize";
  keywordPatterns_ << "min" << "clamp" << "step";

  typeFormat_.setForeground(Qt::darkMagenta);
  typeFormat_.setFontWeight(QFont::Bold);

  // Types
  typePatterns_ << "in" << "out" << "mat3" << "mat4" << "vec2" << "vec3" << "vec4" << "float" << "double" <<"uniform" << "layout" ;

  update();
}

void RenderObjectHighlighter::update() {

  highlightingRules_.clear();

  HighlightingRule rule;

  // Define rule
  rule.pattern = QRegExp("#define.*");
  rule.format =  defineFormat_;
  highlightingRules_.append(rule);

  // version rule
  rule.pattern = QRegExp("#version.*");
  rule.format =  defineFormat_;
  highlightingRules_.append(rule);

  // Rule for single line comments
  rule.pattern = QRegExp("//[^\n]*");
  rule.format = singleLineCommentFormat_;
  highlightingRules_.append(rule);

  // Create Rules for keywords
  foreach (QString pattern, keywordPatterns_) {
    rule.pattern = QRegExp("\\b" + pattern + "\\b" );
    rule.format = keywordFormat_;
    highlightingRules_.append(rule);
  }

  // Create Rules for types
  foreach (QString pattern, typePatterns_ ) {
    rule.pattern = QRegExp("\\b" + pattern + "\\b" );
    rule.format = typeFormat_;
    highlightingRules_.append(rule);
  }

}

void RenderObjectHighlighter::highlightBlock(const QString &text)
{

  // Single word highlighting
  foreach (HighlightingRule rule, highlightingRules_) {
    QRegExp expression(rule.pattern);
    int index = text.indexOf(expression);
    while (index >= 0) {
      int length = expression.matchedLength();
      setFormat(index, length, rule.format);
      index = text.indexOf(expression, index + length);
    }
  }

  // Blockstate -1,0 : nothing
  // Blockstate  1   : vertexShader Code
  // Blockstate  2   : geometryShader Code
  // Blockstate  3   : fragment Shader Code

//  setCurrentBlockState(0);
//
//  // Vertex shader block
//  int startIndex = 0;
//  if (previousBlockState() != 1)
//    startIndex = text.indexOf(vertexShaderStartExpression_);
//
//  while (startIndex >= 0) {
//    int endIndex = text.indexOf(vertexShaderEndExpression_, startIndex);
//    int commentLength;
//    if (endIndex == -1) {
//      setCurrentBlockState(1);
//      commentLength = text.length() - startIndex;
//    } else {
//      commentLength = endIndex - startIndex + vertexShaderEndExpression_.matchedLength();
//    }
//    setFormat(startIndex, commentLength, vertexShaderFormat_);
//    startIndex = text.indexOf(vertexShaderStartExpression_, startIndex + commentLength);
//  }
//
//  // Fragment shader block
//  startIndex = 0;
//  if (previousBlockState() != 3)
//    startIndex = text.indexOf(fragmentShaderStartExpression_);
//
//  while (startIndex >= 0) {
//    int endIndex = text.indexOf(fragmentShaderEndExpression_, startIndex);
//    int commentLength;
//    if (endIndex == -1) {
//      setCurrentBlockState(3);
//      commentLength = text.length() - startIndex;
//    } else {
//      commentLength = endIndex - startIndex + fragmentShaderEndExpression_.matchedLength();
//    }
//    setFormat(startIndex, commentLength, fragmentShaderFormat_);
//    startIndex = text.indexOf(fragmentShaderStartExpression_, startIndex + commentLength);
//  }

}
