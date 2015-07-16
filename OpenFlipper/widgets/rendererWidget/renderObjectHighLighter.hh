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
*   $Date: 2011-09-22 17:16:17 +0200 (Do, 22 Sep 2011) $                     *
*                                                                            *
\*===========================================================================*/

#pragma once

#include <QSyntaxHighlighter>

#include <QHash>
#include <QTextCharFormat>
#include <QTextEdit>
#include <QTextDocument>

class RenderObjectHighlighter : public QSyntaxHighlighter
{
  Q_OBJECT

  public:
    RenderObjectHighlighter( QTextDocument* parent = 0);
    RenderObjectHighlighter( QTextEdit* parent);

    /// Updates the highlighter with the current rule set defined in the patterns
    void update();

  protected:

    void highlightBlock(const QString &text);

  private:
    /// common initializer function called by the constructors
    void init();

    struct HighlightingRule
    {
      QRegExp pattern;
      QTextCharFormat format;
    };



    // Single line rules
    QStringList keywordPatterns_;
    QStringList typePatterns_;

    // All single line rules combined with their format
    QVector<HighlightingRule> highlightingRules_;

    // Multi line rules
    QRegExp vertexShaderStartExpression_;
    QRegExp vertexShaderEndExpression_;

    QRegExp geometryShaderStartExpression_;
    QRegExp geometryShaderEndExpression_;

    QRegExp fragmentShaderStartExpression_;
    QRegExp fragmentShaderEndExpression_;

    // Single formats
    QTextCharFormat keywordFormat_;
    QTextCharFormat defineFormat_;
    QTextCharFormat typeFormat_;

    QTextCharFormat singleLineCommentFormat_;

    // Multi Line formats
    QTextCharFormat vertexShaderFormat_;
    QTextCharFormat geometryShaderFormat_;
    QTextCharFormat fragmentShaderFormat_;

};

