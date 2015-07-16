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
*   $Revision: 16117 $                                                       *
*   $LastChangedBy: moeller $                                                *
*   $Date: 2013-01-11 12:29:53 +0100 (Fr, 11. Jan 2013) $                     *
*                                                                            *
\*===========================================================================*/

#include "CoreWidget.hh"


void CoreWidget::addCoreLicenseInfo() {

  // ======================================================================
  // Core License Information
  // ======================================================================

  // Simple opt used for parsing command line arguments across architectures
  addAboutInfo("OpenFlipper uses SimpleOpt for platform independent command line parsing<br>"
               "SimpleOpt is licensed under the MIT license ( see below)<br>"
               "<br>"
               "<b>Author:</b>   Brodie Thiesfield [code at jellycan dot com]<br>"
               "<b>Source:</b>   http://code.jellycan.com/simpleopt/ <br>"
               "<br>"
               "The licence text below is the boilerplate \"<b>MIT Licence</b>\" used from:<br>"
               "http://www.opensource.org/licenses/mit-license.php <br>"
               "<br>"
               "Copyright (c) 2006-2007, Brodie Thiesfield"
               "<br>"
               "Permission is hereby granted, free of charge, to any person obtaining a copy "
               "of this software and associated documentation files (the \"Software\"), to "
               "deal in the Software without restriction, including without limitation the "
               "rights to use, copy, modify, merge, publish, distribute, sublicense, and/or "
               "sell copies of the Software, and to permit persons to whom the Software is "
               "furnished to do so, subject to the following conditions:<br><br>"
               "The above copyright notice and this permission notice shall be included in "
               "all copies or substantial portions of the Software."
               "<br><br>"
               "THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR "
               "IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, "
               "FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL "
               "THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER "
               "LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  TORT OR OTHERWISE, ARISING "
               "FROM, OUT OF OR IN CONNECTION WITH THE  SOFTWARE OR THE USE OR OTHER DEALINGS "
               "IN THE SOFTWARE."
               ,"SimpleOpt");


  addAboutInfo("The OpenFlipper project uses the FreeFont fonts for rendering.<br>"
               "<br>"
               "You can get the fonts from https://savannah.gnu.org/projects/freefont/"
               ,"FreeFont");

  addAboutInfo("OpenFlipper uses Stackwalker for windows stack trace creation<br>"
               "StackWalker is licensed under the BSD license ( see below)<br>"
               "<br>"
               "LICENSE (http://www.opensource.org/licenses/bsd-license.php)<br>"
               "Copyright (c) 2005-2009, Jochen Kalmbach<br>"
               "All rights reserved.<br>"
               "<br>"
               "Redistribution and use in source and binary forms, with or without modification, "
               "are permitted provided that the following conditions are met:"
               "<br>"
               "Redistributions of source code must retain the above copyright notice,"
               "this list of conditions and the following disclaimer. "
               "Redistributions in binary form must reproduce the above copyright notice,"
               "this list of conditions and the following disclaimer in the documentation "
               "and/or other materials provided with the distribution. "
               "Neither the name of Jochen Kalmbach nor the names of its contributors may be"
               "used to endorse or promote products derived from this software without "
               "specific prior written permission. "
               "<br><br>"
               "THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" "
               "AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,"
               "THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE"
               "ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE"
               "FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES"
               "(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; "
               "LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND "
               "ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT "
               "(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS"
               "SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
               ,"StackWalker");

}


//-----------------------------------------------------------------------------

